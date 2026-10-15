"""Run production storage diagnostics on a host with simulated EEPROM.

Requires Python 3 and gcc. Run: python tests/test_storage_diag.py
This checks watchdog servicing, not real AVR timing or physical I2C.
"""

import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile


SOURCE = Path(__file__).resolve().parents[1] / "main.c"


def production_function(source, name):
    match = re.search(
        rf"^uint8_t {name}\([^\n]*\)\n\{{.*?^\}}",
        source,
        re.MULTILINE | re.DOTALL,
    )
    if match is None:
        raise RuntimeError(f"Production function not found: {name}")
    return match.group(0)


HARNESS = r"""
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef union {
    uint8_t bytes[4];
    uint16_t ints[2];
    uint32_t longs;
} chunk;

static uint8_t memory[EEPROM_END_ADDR];
static int fail_address, reads, resets, gap, max_gap, writes;

void lock_mode_validate(void) {}
void auto_collect_validate(void) {}
void wdt_reset(void) { resets++; gap = 0; }

uint8_t i2c_read_c64(uint16_t address, uint8_t *data)
{
    reads++;
    if (++gap > max_gap) max_gap = gap;
    if (address == fail_address) return 1;
    memcpy(data, memory + address, 4);
    return 0;
}

uint8_t i2c_write_c64(uint16_t address, uint8_t *data)
{
    writes++;
    memcpy(memory + address, data, 4);
    return 0;
}

/* PRODUCTION */

static void put_key(uint16_t address, uint16_t id)
{
    uint8_t *data = memory + address;
    data[0] = (uint8_t)id;
    data[1] = (uint8_t)(id >> 8);
    data[2] = 0x80;
    uint8_t crc = 0;
    for (int i = 0; i < 3; i++) crc = ds_crc(crc, data[i]);
    data[3] = crc;
}

static void set_count(uint16_t count)
{
    memory[0] = (uint8_t)(count >> 8);
    memory[1] = (uint8_t)count;
}

static void prepare(uint16_t count)
{
    memset(memory, 0xFF, sizeof(memory));
    set_count(count);
    memory[2] = DEFAULT_OPEN_TIME >> 8;
    memory[3] = DEFAULT_OPEN_TIME & 0xFF;
    put_key(MASTER_KEY_ADDR, 0x7000);
    for (uint16_t i = 0; i < count; i++)
        put_key(USER_KEYS_ADDR + i * 4, i + 1);
    fail_address = -1;
    reads = resets = gap = max_gap = writes = 0;
}

#define CHECK(test) do { if (!(test)) { \
    fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #test); return 1; \
} } while (0)

int main(void)
{
    prepare(0);
    CHECK(storage_diag() == 0);
    CHECK(reads == MAX_USER_KEYS + 2);
    CHECK(max_gap <= 3);
    CHECK(resets == MAX_USER_KEYS);
    CHECK(writes == 0);
    puts("PASS initialized empty database: all slots scanned, watchdog serviced");

    prepare(2);
    CHECK(storage_diag() == 0);
    CHECK(max_gap <= 3 && resets == MAX_USER_KEYS);
    puts("PASS partially filled database");

    prepare(MAX_USER_KEYS);
    CHECK(storage_diag() == 0);
    CHECK(max_gap <= 3 && resets == MAX_USER_KEYS);
    puts("PASS full database");

    prepare(0);
    set_count(1);
    CHECK(storage_diag() == 5);
    puts("PASS incorrect stored count detected");

    prepare(0);
    put_key(USER_KEYS_ADDR + 4, 1);
    CHECK(storage_diag() == 5);
    puts("PASS orphan key after blank slot detected even with zero count");

    prepare(1);
    memory[USER_KEYS_ADDR + 3] ^= 1;
    CHECK(storage_diag() == 3);
    puts("PASS invalid user CRC detected");

    prepare(1);
    memcpy(memory + USER_KEYS_ADDR, memory + MASTER_KEY_ADDR, 4);
    CHECK(storage_diag() == 4);
    puts("PASS duplicate master detected");

    prepare(0);
    memset(memory + MASTER_KEY_ADDR, 0xFF, 4);
    CHECK(storage_diag() == 2);
    puts("PASS missing master detected");

    prepare(0);
    fail_address = USER_KEYS_ADDR + 4;
    CHECK(storage_diag() == 1);
    puts("PASS I2C read failure detected");

    prepare(0);
    memory[2] = memory[3] = 0xFF;
    CHECK(storage_diag() == 0);
    CHECK(writes == 1);
    CHECK(((uint16_t)memory[2] << 8 | memory[3]) == DEFAULT_OPEN_TIME);
    puts("PASS invalid open time restored");

    prepare(0);
    set_count(MAX_USER_KEYS + 1);
    CHECK(storage_diag() == 5);
    puts("PASS out-of-range count detected");
    return 0;
}
"""


def main():
    source = SOURCE.read_text(encoding="utf-8").replace("\r\n", "\n")
    constants = "\n".join(
        re.search(rf"^#define {name}\s+[^\n]+", source, re.MULTILINE).group(0)
        for name in (
            "EEPROM_INFO_ADDR", "MASTER_KEY_ADDR", "USER_KEYS_ADDR",
            "EEPROM_END_ADDR", "MAX_OPEN_TIME", "DEFAULT_OPEN_TIME",
            "MAX_USER_KEYS",
        )
    )
    functions = "\n\n".join(
        production_function(source, name)
        for name in ("ds_crc", "key_blank", "key_bad", "chunk_compare", "storage_diag")
    )
    compiler = os.environ.get("CC", "gcc")
    if shutil.which(compiler) is None:
        raise RuntimeError(f"C compiler not found: {compiler}")
    with tempfile.TemporaryDirectory(prefix="tc01-storage-test-") as directory:
        root = Path(directory)
        c_file = root / "storage_diag_test.c"
        executable = root / "storage_diag_test.exe"
        c_file.write_text(
            constants + "\n" + HARNESS.replace("/* PRODUCTION */", functions),
            encoding="utf-8",
        )
        subprocess.run(
            [compiler, "-std=c99", "-Wall", "-Wextra", "-Werror", str(c_file),
             "-o", str(executable)],
            check=True,
        )
        subprocess.run([str(executable)], check=True)


if __name__ == "__main__":
    main()
