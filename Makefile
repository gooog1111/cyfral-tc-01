MCU = atmega8
F_CPU = 4000000UL
TARGET = TC-01
SRC = main.c i2c.c

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -std=gnu99 -Wall -Wextra
LDFLAGS = -mmcu=$(MCU)

all: $(TARGET).hex size

$(TARGET).elf: $(SRC)
	avr-gcc $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

size: $(TARGET).elf
	avr-size $<

clean:
	del /Q $(TARGET).elf $(TARGET).hex 2>NUL || exit /B 0

.PHONY: all size clean
