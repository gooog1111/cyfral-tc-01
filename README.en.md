<!-- LANG_START -->
🇷🇺 [Русская версия](README.md)
<!-- LANG_END -->

<div align="center">

<img src="resources/header.svg" alt="Cyfral TC-01" width="900"/>

</div>





<!-- STATS_START -->
<!-- auto-updated by GitHub Actions · 2026-07-10 08:00 UTC -->

[![Views local](https://img.shields.io/badge/Views_local-54-ff6900?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01)
[![Views GitHub](https://img.shields.io/badge/Views_GitHub-23-ff6900?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01)
[![Unique visitors](https://img.shields.io/badge/Unique-5-blue?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01)
[![Clones](https://img.shields.io/badge/Clones-2197-purple?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01)
[![Stars](https://img.shields.io/badge/Stars-1-yellow?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01/stargazers)
[![Forks](https://img.shields.io/badge/Forks-0-green?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01/network/members)
[![Downloads latest release](https://img.shields.io/badge/Downloads_latest_release-2-brightgreen?style=for-the-badge)](https://github.com/gooog1111/cyfral-tc-01/releases/latest)
[![Downloads total assets](https://img.shields.io/badge/Downloads_total_assets-6-brightgreen?style=for-the-badge)](https://github.com/gooog1111/cyfral-tc-01/releases)

<!-- STATS_END -->









<!-- GRAPH_START -->
<p align="center">
  <img src="./traffic-views.png" width="100%" alt="GitHub Traffic">
</p>
<!-- GRAPH_END -->










<!-- ISSUES_START -->
<!-- auto-updated by GitHub Actions · 2026-07-10 08:00 UTC -->

## Issues

<p>
  <a href="https://github.com/gooog1111/cyfral-tc-01/issues">
    <img alt="Open issues" src="https://img.shields.io/badge/Open_issues-0-blue?style=for-the-badge&logo=github">
  </a>
  <a href="https://github.com/gooog1111/cyfral-tc-01/issues/new/choose">
    <img alt="Create issue" src="https://img.shields.io/badge/Create_issue-new-success?style=for-the-badge&logo=github">
  </a>
</p>

<details open>
<summary><b>Open issues</b></summary>


<p align="center">
  <b>No open issues.</b><br>
  <sub>The service issue <code>views-counter</code> is hidden from the list.</sub>
</p>


</details>

<p>
  <a href="https://github.com/gooog1111/cyfral-tc-01/issues/new/choose">Create new issue</a> ·
  <a href="https://github.com/gooog1111/cyfral-tc-01/issues">All issues</a>
</p>

<!-- ISSUES_END -->




## Digital TC-01

Describe errors, suggestions for adding functionality and other tasks in [Issues](https://github.com/gooog1111/cyfral-tc-01/issues).

The project is not an original development. The source code is taken from the publication [Cyfral TC-01](https://allaboutdomofon.blogspot.com/2021/05/cyfral-tc-01.html), this repository contains a modified and documented version.

Firmware for access controller Digital TC-01 on `ATmega8A`. The device works with iButton/1-Wire keys and compatible keys, stores the base in an external EEPROM `24C64`, controls an electromagnetic or electromechanical lock, has a service menu, an opening button, LED indication and a buzzer.

## Current version

Main changes in this version:

- added support for Dallas/Maxim `DS1992L+F5` and `DS1995L+F5`;
- support for `DS1990A/RW1990` and compatible keys is preserved;
- the lock mode is configured through the 4th item of the service menu;
- automatic key collection is configured through the 5th item of the service menu and is disabled by default;
- for this board, the `LOCK` output operates at an active low level: magnet retention corresponds to `PC1 = 0`;
- short beeps reset the watchdog, so the 4th menu item no longer causes a reboot;
- the firmware is compiled under `F_CPU=4000000UL`, fuse `LOW=0xE3`.

## Supported keys

The firmware tries to read the key in several ways:

| Type | Support | Note |
| --- | --- | --- |
| `DS1990A`, `RW1990`, analogues | yes | Dallas/1-Wire family `0x01` |
| `DS1992L+F5` | yes | Dallas/1-Wire family `0x08` |
| `DS1995L+F5` | yes | Dallas/1-Wire family `0x0A` |
| Metakom/MK-compatible | partial | read by a separate decoder `mk_rx()` |
| Cyfral-compatible | partial | read by a separate decoder `cyfral_rx()` |

An important limitation: the key database currently uses the old 4-byte internal format. For Dallas/1-Wire keys, the full 64-bit ROM is read, the CRC is checked, then the identifier is collapsed into 4 bytes for compatibility with the existing EEPROM base. This maintains compatibility with currently recorded keys, but does not use the entire DS1992/DS1995 48-bit serial number.

## Controller

Target microcontroller: `ATmega8A-PU`, package `PDIP-28`.

Parameters:

- Flash: `8 KB`;
- SRAM: `1 KB`;
- internal EEPROM: `512 B`;
- logic power: `5 V`;
- clocking: internal RC about `4 MHz`;
- assembly: `avr-gcc`, target `atmega8`, `F_CPU=4000000UL`.

Fuse bits used for firmware:

```text
lfuse = 0xE3
hfuse = 0x99
lock  = 0xFF
```

`lock=0xFF` means Flash is not blocked. The final blocking can be set separately only after a complete check of the device.## Pinout ATmega8A-PU

`PDIP-28` body, top view, key/notch on top.

![Распиновка ATmega8A-PU](atmega8a_pin.png)

| Leg | Port | Destination |
| ---: | --- | --- |
| 1 | `PC6/RESET` | Reset, ISP |
| 2 | `PD0/RXD` | not used |
| 3 | `PD1/TXD` | soft-UART debugging |
| 4 | `PD2/INT0` | not used |
| 5 | `PD3/INT1` | not used |
| 6 | `PD4/T0` | not used |
| 7 | `VCC` | food `+5V` |
| 8 | `GND` | earth |
| 9 | `PB6/XTAL1` | not used, internal RC |
| 10 | `PB7/XTAL2` | not used, internal RC |
| 11 | `PD5/T1` | `E_TD` |
| 12 | `PD6/AIN0` | `TC` |
| 13 | `PD7/AIN1` | `E_TC` |
| 14 | `PB0/ICP1` | not used |
| 15 | `PB1/OC1A` | `TD`, Dallas/1-Wire key line |
| 16 | `PB2/SS/OC1B` | not used |
| 17 | `PB3/MOSI/OC2` | `RED`, red LED, ISP MOSI |
| 18 | `PB4/MISO` | `XP3`, hard reset enable, ISP MISO |
| 19 | `PB5/SCK` | `GREEN`, green LED, ISP SCK |
| 20 | `AVCC` | power supply of the analog part, connect to `+5V` |
| 21 | `AREF` | not used |
| 22 | `GND` | earth |
| 23 | `PC0/ADC0` | `WP`, write protection EEPROM 24C64 |
| 24 | `PC1/ADC1` | `LOCK`, lock control |
| 25 | `PC2/ADC2` | `BEEP`, buzzer |
| 26 | `PC3/ADC3` | `SW`, door open button |
| 27 | `PC4/ADC4/SDA` | `SDA`, EEPROM 24C64 |
| 28 | `PC5/ADC5/SCL` | `SCL`, EEPROM 24C64 |

ISP lines `MOSI/MISO/SCK` are simultaneously used by the board for LED and pin `XP3`. If the ArduinoISP is not flashing the controller, temporarily disable external loads with `PB3/PB4/PB5` or check that they are not interfering with the SPI levels.

## Connection

Photo of the board:

![Плата TC-01](layout.jpg)

![Схема подключения TC-01](connect.png)

Main nodes:

- `TD` (`PB1`) - Dallas/1-Wire key reading line;
- `SDA/SCL` (`PC4/PC5`) - software I2C for `24C64`;
- `WP` (`PC0`) - record protection management `24C64`;
- `LOCK` (`PC1`) - power key control output for the lock;
- `BEEP` (`PC2`) - buzzer;
- `SW` (`PC3`) - button to open the lock;
- `XP3` (`PB4`) - full reset enable jumper, not used to enter the service mode;
- `RED/GREEN` (`PB3/PB5`) - indication.

Board terminals:

| Contacts | Destination |
| --- | --- |
| `1`, `2` | device power `12 V AC` || `3`, `4` | считыватель ключей, контакт `4` - общий минус |
| `4`, `5` | кнопка открытия замка `SW`, контакт `4` - общий минус |
| `M+` и минус | выход на замок; на `M+` к замку должно быть около `19 V DC` |

Контакт `4` используется как общий минус для считывателя и кнопки открытия.

## Управление замком

Выход `LOCK` управляет силовым транзистором активным низким уровнем.

## # Режим 1: электромагнитный замок

Используется для магнита, которому нужно постоянное питание в закрытом состоянии.

- Ожидание: `PC1/LOCK = 0`, транзистор открыт, магнит удерживает.
- Открытие: `PC1/LOCK = 1`, транзистор закрыт, питание с магнита снимается.
- После таймера открытия: снова `PC1/LOCK = 0`.

## # Режим 2: электромеханический замок

Используется для замка, которому питание подаётся только на время открытия.

- Ожидание: `PC1/LOCK = 1`, транзистор закрыт, питания на замке нет.
- Открытие: `PC1/LOCK = 0`, транзистор открыт, питание подаётся.
- После таймера открытия: снова `PC1/LOCK = 1`.

Режим хранится во внутренней EEPROM ATmega8A по адресу `0x09`:

- `0xFF` - режим 1, электромагнитный;
- `0x00` - режим 2, электромеханический.

Если значение повреждено, прошивка восстанавливает режим 1.

Режим автосбора хранится по адресу `0x0A`:

- `0xFF` - автосбор выключен;
- `0x00` - автосбор включён.

При повреждённом значении и после полного сброса автосбор выключается.

## Время открытия

Время открытия хранится во внешней EEPROM `24C64` в служебном блоке. Единица измерения - `100 ms`.

- значение по умолчанию: `0x0032`, то есть `50 * 100 ms = 5 секунд`;
- максимум: `0x0C00`, то есть около `307.2 секунд`;
- если значение повреждено или слишком большое, используется значение по умолчанию.

Во время открытия зуммер включён постоянно, зелёный LED горит, выход `LOCK` переведён в состояние открытия.

## Память EEPROM 24C64

Внешняя EEPROM `24C64` используется для базы ключей и параметров.

| Адрес | Размер | Назначение |
| --- | ---: | --- |
| `0x0000..0x0003` | 4 байта | служебный блок |
| `0x0000..0x0001` | 2 байта | количество пользовательских ключей |
| `0x0002..0x0003` | 2 байта | время открытия в тиках по 100 ms |
| `0x0004..0x0007` | 4 байта | мастер-ключ |
| `0x0008..0x1FFF` | по 4 байта | пользовательские ключи |

Пустая ячейка ключа: `0xFFFFFFFF`. Такой ключ запрещён к записи.

При старте выполняется самодиагностика:

- проверяется доступность EEPROM;
- проверяется мастер-ключ;
- проверяется CRC ключей;
- checks for the absence of non-empty keys after the first empty cell;
- the key counter in the header is checked.

After three consecutive EEPROM errors, emergency behavior is activated: a correctly read key can open the door according to a common timer.

## Normal mode

When started, the device enters normal mode. The `XP3` jumper to enter the service mode is no longer used.

Behavior:

- while waiting, the red LED flashes periodically;
- the `SW` button opens the door immediately;
- briefly applying the master key in normal mode opens the door;
- holding the master key longer than `5 секунд` activates the service mode;
- a user key from the database opens the door;
- an unknown key does not open the door and receives a refusal signal;
- when auto-collection is turned on, any correct key opens the door, and the unknown one is saved in the database;
- if the EEPROM is damaged, after several errors the alarm logic is activated.

## Service mode

The service mode is activated by holding the master key for 5 seconds. (jumper `XP3` is not required)

Login:

- attach the master key and hold it longer `5 секунд`;
- upon entry, `3` long beeps sound.

In service mode, control is performed with the `SW` button and the master key.
To exit the service mode, hold the master key again longer than `5 секунд`; output is confirmed by `2` long beeps.

## Service menu

Entering the menu:

- enter the service mode by holding the master key longer than `5 секунд`;
- after logging in, the `1` item is immediately announced, the menu is ready to be navigated with the `SW` button.

Navigation:

- short press `SW` - next menu item;
- long press `SW` - select the current item;
- the points go in a circle;
- the item number is announced with short signals.

Menu items:

| Item | Signal | Destination |
| ---: | --- | --- |
| 1 | `1` short | adding custom keys |
| 2 | `2` short | setting opening time |
| 3 | `3` short | removing user keys |
| 4 | `4` short | setting the lock mode |
| 5 | `5` short | setting up automatic key collection |

## # Point 1: adding keys

After entering the location:

- attach a new key;
- if a readable but invalid RW1990/TM01/RW2004/Dallas-compatible key is attached, the firmware tries to write a random code into it, then re-read it and only then add it to the database;
- if the key is added, a long signal sounds `1`;
- if the key is already there, `2` short sounds;
- the master key is not added as a user key;
- short press `SW` exits the item back to the menu.

## # Point 2: opening timeAfter entering the location:

- press `SW` to start the countdown;
- every second a short signal sounds `1`;
- press `SW` again to save time;
- saving is confirmed by `2` short signals.

Inside the firmware, the saved value is converted into ticks of 100 ms.

## # Point 3: deleting keys

After entering the location:

- attach a user key;
- if the key is removed, a short signal sounds `1`;
- if the key is not in the database, `2` short sounds;
- the master key is not deleted;
- short press `SW` exits the item back to the menu.

## # Point 4: Lock mode

After selecting an item with a long press:

- the device enters the lock mode setting;
- immediately announces the current mode: `1` short or `2` short;
- short press switches `1 -> 2 -> 1` in a circle;
- long press saves the selected mode;
- after saving, a `1` long exit signal sounds and then a `1` or `2` short signal sounds, which mode is saved.

The electromagnetic lock on the current board requires the `1` mode.

## # Point 5: Automatic key collection

- the device immediately announces the current mode: `1` short - off, `2` short - on;
- short press switches `1 -> 2 -> 1` modes;
- long press saves the selected mode;
- when auto collection is enabled, any correctly read key opens the door;
- an unknown key is added to the database before opening, an already recorded key is not saved again;
- if a readable but invalid RW1990/TM01/RW2004/Dallas-compatible key is attached, the firmware tries to write a random code into it, then re-read it and only then add it to the database;
- if the key is read, but not written due to an error, full memory or unsuccessful rewriting of the RW blank, an error trill sounds and the door does not open.

Automatic rewriting is implemented for RW1990-like, TM01/TM01C and RW2004/TM2004 blanks. For Metakom/Cyfral-compatible keys, the firmware has a reading protocol, but no writing protocol.

## Reset and new master key

The database can be completely cleared and a new master key can be written even if the old master key is lost. To protect against accidental startup, the reset requires the `XP3` jumper to be installed.

1. Install jumper `XP3`.
2. Hold the `SW` button near `10 секунд` without removing `XP3`.
3. After the signal, attach a new master key.
4. The firmware will clear user keys, restore default settings and write the master key.This procedure works from normal mode and does not require the old master key. In service mode, the reset is launched in the same way: `XP3` plus hold `SW`.

After reset:

- the master key is recorded again;
- the list of user keys is empty;
- opening time returns to `5 секундам`;
- the lock mode returns to the `1` mode.
- automatic key collection is turned off.

## Signals

| Event | Signal |
| --- | --- |
| Entering service mode | `3` long |
| Exiting service mode | `2` long |
| Menu item | `1..5` short |
| Selecting a menu item | `1` long |
| Key added | `1` long |
| The key is already there | `2` short |
| Key removed | `1` short |
| Key not found during deletion | `2` short |
| Failure to unknown key | `2` short + `2` long |
| Operation error | frequent trill |
| Saving opening time | `2` short |
| Saving Lock Mode | `1` long + mode number |

## Assembly

The firmware is assembled using the usual AVR toolchain under `ATmega8`:

- `avr-gcc`;
- `avr-libc`;
- `avr-objcopy`;
- `avr-size`;
- `make`;
- `avrdude` for firmware.

## #Windows

The simplest option is the ZakKemble AVR-GCC package:

```powershell
winget install ZakKemble.avr-gcc
```

After installation, open a new PowerShell and check the tools:

```powershell
avr-gcc --version
make --version
avrdude -?
```

If `make` or `avrdude` are not found, add the `bin` directory of the AVR-GCC installation to `PATH` or run the tools in the full path. For a package from `winget` the path is usually something like:

```powershell
$env:LOCALAPPDATA\Microsoft\WinGet\Packages\ZakKemble.avr-gcc_Microsoft.Winget.Source_8wekyb3d8bbwe\avr-gcc-14.1.0-x64-windows\bin
```

Assembly:

```powershell
make
```

Cleaning and reassembly:

```powershell
make clean
make
```

## #Linux

Debian/Ubuntu:

```bash
sudo apt update
sudo apt install gcc-avr avr-libc binutils-avr make avrdude srecord
```

Arch Linux:

```bash
sudo pacman -S avr-gcc avr-libc avr-binutils make avrdude srecord
```

Fedora:

```bash
sudo dnf install avr-gcc avr-libc avr-binutils make avrdude srecord
```

Assembly:

```bash
make clean
make
```

Build result:

- `TC-01.elf` - ELF file for debugging and viewing the size;
- `TC-01.hex` - Intel HEX for writing to Flash ATmega8A.

Expected size of current version:

```text
text = 7294
data = 2
bss  = 3
dec  = 7299
hex  = 1c83
```

## Breaking HEX into chunks

Typically, firmware can be written in one file `TC-01.hex`. Chunks are needed only if the programmer or circuit freezes on a long write/check, most often this occurs with ArduinoISP.

For partitioning, it is convenient to use `srec_cat` from the `srecord` package. ATmega8 has `8 KB` Flash, so you can split the file into `1 KB` blocks.

Windows PowerShell:

```powershell
New-Item -ItemType Directory -Force chunks | Out-Null
srec_cat TC-01.hex -Intel -crop 0x0000 0x0400 -o chunks/TC-01_0000_03ff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x0400 0x0800 -o chunks/TC-01_0400_07ff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x0800 0x0c00 -o chunks/TC-01_0800_0bff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x0c00 0x1000 -o chunks/TC-01_0c00_0fff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x1000 0x1400 -o chunks/TC-01_1000_13ff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x1400 0x1800 -o chunks/TC-01_1400_17ff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x1800 0x1c00 -o chunks/TC-01_1800_1bff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x1c00 0x2000 -o chunks/TC-01_1c00_1fff.hex -Intel
```

Linux:

```bash
mkdir -p chunks
srec_cat TC-01.hex -Intel -crop 0x0000 0x0400 -o chunks/TC-01_0000_03ff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x0400 0x0800 -o chunks/TC-01_0400_07ff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x0800 0x0c00 -o chunks/TC-01_0800_0bff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x0c00 0x1000 -o chunks/TC-01_0c00_0fff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x1000 0x1400 -o chunks/TC-01_1000_13ff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x1400 0x1800 -o chunks/TC-01_1400_17ff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x1800 0x1c00 -o chunks/TC-01_1800_1bff.hex -Intel
srec_cat TC-01.hex -Intel -crop 0x1c00 0x2000 -o chunks/TC-01_1c00_1fff.hex -Intel
```

Empty upper chunks are acceptable if the firmware takes up less than 8 KB. To write in blocks, the first block is written with the chip cleared, the rest - with `-D`, so as not to erase already written pages.## Прошивка через ArduinoISP

Подготовка Arduino Uno:

1. Подключить Arduino Uno к USB.
2. Открыть Arduino IDE.
3. Выбрать плату `Arduino Uno`.
4. Открыть пример `File -> Examples -> 11.ArduinoISP -> ArduinoISP`.
5. Загрузить скетч в Arduino Uno.
6. Поставить конденсатор `10 uF` между `RESET` и `GND` Arduino Uno, чтобы Uno не перезагружалась при запуске `avrdude`.

Подключение ISP:

| Arduino Uno | ATmega8A |
| --- | --- |
| `D10` | `RESET`, pin 1 |
| `D11` | `MOSI/PB3`, pin 17 |
| `D12` | `MISO/PB4`, pin 18 |
| `D13` | `SCK/PB5`, pin 19 |
| `5V` | `VCC`, pin 7 |
| `5V` | `AVCC`, pin 20 |
| `GND` | `GND`, pins 8 и 22 |

Проверка связи, fuse и lock:

```powershell
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -U lfuse:r:-:h -U hfuse:r:-:h -U lock:r:-:h
```

Ожидаемо:

```text
lfuse = 0xe3
hfuse = 0x99
lock  = 0xff
```

Обычная запись:

```powershell
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

Для Linux порт обычно выглядит как `/dev/ttyACM0` или `/dev/ttyUSB0`:

```bash
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

На этой схеме ArduinoISP может зависать при длинном чтении Flash во время verify. Если verify зависает, использовать запись короткими блоками из каталога `chunks/` с отключенной проверкой чтением:

- первый блок пишется с chip erase;
- остальные блоки пишутся с `-D`;
- все блоки пишутся с `-V`.

Windows PowerShell:

```powershell
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -V -U flash:w:chunks/TC-01_0000_03ff.hex:i
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_0400_07ff.hex:i
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_0800_0bff.hex:i
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_0c00_0fff.hex:i
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_1000_13ff.hex:i
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_1400_17ff.hex:i
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_1800_1bff.hex:i
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_1c00_1fff.hex:i
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

Linux:

```bash
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -V -U flash:w:chunks/TC-01_0000_03ff.hex:i
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_0400_07ff.hex:i
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_0800_0bff.hex:i
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_0c00_0fff.hex:i
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_1000_13ff.hex:i
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_1400_17ff.hex:i
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_1800_1bff.hex:i
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -D -V -U flash:w:chunks/TC-01_1c00_1fff.hex:i
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

После записи блоками проверять только сигнатуру, fuse и lock, не делать длинный dump/verify Flash.

Если `avrdude` установлен через пакет ZakKemble AVR-GCC и не находит `avrdude.conf`, запускать с явным `-C`, например:

```powershell
avrdude -C "$env:LOCALAPPDATA\Microsoft\WinGet\Packages\ZakKemble.avr-gcc_Microsoft.Winget.Source_8wekyb3d8bbwe\avr-gcc-14.1.0-x64-windows\bin\avrdude.conf" -c stk500v1 -P COM3 -b 19200 -p m8 -U lfuse:r:-:h -U hfuse:r:-:h -U lock:r:-:h
```

## Прошивка через USBasp / AVRASP / USBASP 2.0

USBasp, AVRASP и многие платы с надписью `USBASP 2.0` в `avrdude` обычно используются как программатор `usbasp`.

Подключение ISP:

| USBasp | ATmega8A |
| --- | --- |
| `MOSI` | `MOSI/PB3`, pin 17 |
| `MISO` | `MISO/PB4`, pin 18 |
| `SCK` | `SCK/PB5`, pin 19 |
| `RST` / `RESET` | `RESET`, pin 1 |
| `VCC` | `VCC`, pin 7 и `AVCC`, pin 20 |
| `GND` | `GND`, pins 8 и 22 |

Если плата питается отдельно, не подключать `VCC` от программатора, оставить только общую землю `GND`.

Проверка связи:

```powershell
avrdude -c usbasp -p m8 -U lfuse:r:-:h -U hfuse:r:-:h -U lock:r:-:h
```

Запись прошивки и fuse:

```powershell
avrdude -c usbasp -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

На Linux команды такие же. Если обычный пользователь не имеет доступа к USBasp, запускать через `sudo` или добавить udev-правило:

```bash
sudo avrdude -c usbasp -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

Если USBasp старый и `avrdude` пишет предупреждение про `cannot set sck period`, это не всегда ошибка. Если связь нестабильна, поставить перемычку `slow SCK` на USBasp или добавить ключ `-B 10`:

```bash
avrdude -c usbasp -B 10 -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

## Прошивка через AVRISP / AVRISP mkII

Для оригинального Atmel/Microchip AVRISP mkII и совместимых программаторов в `avrdude` используется `-c avrispmkII`.

Проверка:

```bash
avrdude -c avrispmkII -P usb -p m8 -U lfuse:r:-:h -U hfuse:r:-:h -U lock:r:-:h
```

Запись:

```bash
avrdude -c avrispmkII -P usb -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

Если используется старый последовательный AVRISP/STK500-совместимый программатор, команда обычно выглядит так:

```bash
avrdude -c avrisp -P COM3 -b 19200 -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

На Linux вместо `COM3` использовать свой порт, например `/dev/ttyUSB0`.

## ST-Link V2

ST-Link V2 напрямую не прошивает `ATmega8A` по AVR ISP. Он рассчитан на STM8/STM32 через SWIM/SWD, а у ATmega8A используется SPI ISP.

Использовать ST-Link V2 для этой платы можно только если он перепрошит альтернативной прошивкой, которая превращает его в AVR ISP-совместимый программатор. В обычном состоянии для TC-01 нужны ArduinoISP, USBasp/AVRASP, AVRISP mkII или другой программатор, поддерживаемый `avrdude` для `-p m8`.

## Fuse-биты

Текущая прошивка рассчитана на внутренний RC-генератор около `4 MHz`:

```text
lfuse = 0xE3
hfuse = 0x99
lock  = 0xFF
```

Перед записью fuse желательно сначала прочитать текущие значения. Неверные fuse могут отключить ISP или выбрать неподключенный источник тактирования.

![Расстановка fuse-бит](fuse.jpg)

## Диагностика замка

Если электромагнитный замок не держит

1. Убедиться, что в пункте 4 сохранён режим `1`.
2. Измерить напряжение прямо на клеммах магнита при подключённом магните.
3. Измерить `PC1/LOCK`, pin 24 ATmega8A:
   - в режиме ожидания режима `1` должно быть около `0 V`;
   - при открытии должно быть около `5 V`.
4. Если `PC1` переключается правильно, а магнит слабый, искать проблему в питании, силовом транзисторе, проводке, земле, самом магните или ответной части.

Прошивка не использует ШИМ для замка. Выход `LOCK` держится постоянно в одном состоянии и переключается только на время открытия.
