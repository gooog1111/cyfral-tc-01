<!-- LANG_START -->
🇷🇺 [Русская версия](README.md)
<!-- LANG_END -->

<div align="center">

<img src="resources/header.svg" alt="Cyfral TC-01" width="900"/>

</div>





<!-- STATS_START -->
<!-- auto-updated by GitHub Actions · 2026-07-10 07:00 UTC -->

[![Views local](https://img.shields.io/badge/Views_local-53-ff6900?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01)
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
<!-- auto-updated by GitHub Actions · 2026-07-10 07:00 UTC -->

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




## Цифрал TC-01

Ошибки, предложения по добавлению функционала и другие задачи описывайте в [Issues](https://github.com/gooog1111/cyfral-tc-01/issues).

Проект не является оригинальной разработкой. Исходный код взят из публикации [Cyfral TC-01](https://allaboutdomofon.blogspot.com/2021/05/cyfral-tc-01.html), этот репозиторий содержит доработанную и документированную версию.

Прошивка для контроллера доступа Цифрал TC-01 на `ATmega8A`. Устройство работает с ключами iButton/1-Wire и совместимыми ключами, хранит базу во внешней EEPROM `24C64`, управляет электромагнитным или электромеханическим замком, имеет сервисное меню, кнопку открытия, светодиодную индикацию и зуммер.

## Текущая версия

Основные изменения этой версии:

- добавлена поддержка Dallas/Maxim `DS1992L+F5` и `DS1995L+F5`;
- сохранена поддержка `DS1990A/RW1990` и совместимых ключей;
- режим замка настраивается через 4-й пункт сервисного меню;
- автосбор ключей настраивается через 5-й пункт сервисного меню и по умолчанию выключен;
- для этой платы выход `LOCK` работает активным низким уровнем: удержание магнита соответствует `PC1 = 0`;
- короткие звуковые сигналы сбрасывают watchdog, поэтому 4-й пункт меню больше не вызывает перезагрузку;
- прошивка собирается под `F_CPU=4000000UL`, fuse `LOW=0xE3`.

## Поддерживаемые ключи

Прошивка пытается прочитать ключ несколькими способами:

| Тип | Поддержка | Примечание |
| --- | --- | --- |
| `DS1990A`, `RW1990`, аналоги | да | Dallas/1-Wire family `0x01` |
| `DS1992L+F5` | да | Dallas/1-Wire family `0x08` |
| `DS1995L+F5` | да | Dallas/1-Wire family `0x0A` |
| Metakom/MK-совместимые | частичная | читаются отдельным декодером `mk_rx()` |
| Cyfral-совместимые | частичная | читаются отдельным декодером `cyfral_rx()`  |

Важное ограничение: база ключей пока использует старый 4-байтный внутренний формат. Для Dallas/1-Wire ключей читается полный 64-битный ROM, проверяется CRC, затем идентификатор сворачивается в 4 байта для совместимости с уже существующей базой EEPROM. Это сохраняет совместимость с текущими записанными ключами, но не использует весь 48-битный серийный номер DS1992/DS1995.

## Контроллер

Целевой микроконтроллер: `ATmega8A-PU`, корпус `PDIP-28`.

Параметры:

- Flash: `8 KB`;
- SRAM: `1 KB`;
- внутренняя EEPROM: `512 B`;
- питание логики: `5 V`;
- тактирование: внутренний RC около `4 MHz`;
- сборка: `avr-gcc`, цель `atmega8`, `F_CPU=4000000UL`.

Fuse-биты, использованные при прошивке:

```text
lfuse = 0xE3
hfuse = 0x99
lock  = 0xFF
```

`lock=0xFF` означает, что Flash не заблокирована. Финальную блокировку можно ставить отдельно только после полной проверки устройства.

## Pinout ATmega8A-PU

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
4. The firmware will clear user keys, restore default settings and write the master key.Эта процедура работает из обычного режима и не требует старого мастер-ключа. В сервисном режиме сброс запускается так же: `XP3` плюс удержание `SW`.

После сброса:

- мастер-ключ записан заново;
- список пользовательских ключей пуст;
- время открытия возвращается к `5 секундам`;
- режим замка возвращается к режиму `1`.
- автосбор ключей выключается.

## Сигналы

| Событие | Сигнал |
| --- | --- |
| Вход в сервисный режим | `3` длинных |
| Выход из сервисного режима | `2` длинных |
| Пункт меню | `1..5` коротких |
| Выбор пункта меню | `1` длинный |
| Ключ добавлен | `1` длинный |
| Ключ уже есть | `2` коротких |
| Ключ удалён | `1` короткий |
| Ключ не найден при удалении | `2` коротких |
| Отказ неизвестному ключу | `2` коротких + `2` длинных |
| Ошибка операции | частая трель |
| Сохранение времени открытия | `2` коротких |
| Сохранение режима замка | `1` длинный + номер режима |

## Сборка

Прошивка собирается обычным AVR toolchain под `ATmega8`:

- `avr-gcc`;
- `avr-libc`;
- `avr-objcopy`;
- `avr-size`;
- `make`;
- `avrdude` для прошивки.

## # Windows

Самый простой вариант - пакет ZakKemble AVR-GCC:

```powershell
winget install ZakKemble.avr-gcc
```

После установки открыть новый PowerShell и проверить инструменты:

```powershell
avr-gcc --version
make --version
avrdude -?
```

Если `make` или `avrdude` не находятся, добавить каталог `bin` установленного AVR-GCC в `PATH` или запускать инструменты по полному пути. Для пакета из `winget` путь обычно похож на:

```powershell
$env:LOCALAPPDATA\Microsoft\WinGet\Packages\ZakKemble.avr-gcc_Microsoft.Winget.Source_8wekyb3d8bbwe\avr-gcc-14.1.0-x64-windows\bin
```

Сборка:

```powershell
make
```

Очистка и повторная сборка:

```powershell
make clean
make
```

## # Linux

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

Сборка:

```bash
make clean
make
```

Результат сборки:

- `TC-01.elf` - ELF-файл для отладки и просмотра размера;
- `TC-01.hex` - Intel HEX для записи во Flash ATmega8A.

Ожидаемый размер текущей версии:

```text
text = 7294
data = 2
bss  = 3
dec  = 7299
hex  = 1c83
```

## Разбивка HEX на чанки

Обычно прошивку можно писать одним файлом `TC-01.hex`. Чанки нужны только если программатор или схема зависает на длинной записи/проверке, чаще всего это встречается с ArduinoISP.

Для разбиения удобно использовать `srec_cat` из пакета `srecord`. ATmega8 имеет `8 KB` Flash, поэтому можно разбить файл на блоки по `1 KB`.

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

Пустые верхние чанки допустимы, если прошивка занимает меньше 8 KB. Для записи блоками первый блок пишется с очисткой чипа, остальные - с `-D`, чтобы не стирать уже записанные страницы.

## Firmware via ArduinoISP

Preparing Arduino Uno:

1. Connect Arduino Uno to USB.
2. Open Arduino IDE.
3. Select the board `Arduino Uno`.
4. Open example `File -> Examples -> 11.ArduinoISP -> ArduinoISP`.
5. Upload the sketch to the Arduino Uno.
6. Place a capacitor `10 uF` between `RESET` and `GND` Arduino Uno so that the Uno does not reboot when `avrdude` starts.

ISP connection:

| Arduino Uno | ATmega8A |
| --- | --- |
| `D10` | `RESET`, pin 1 |
| `D11` | `MOSI/PB3`, pin 17 |
| `D12` | `MISO/PB4`, pin 18 |
| `D13` | `SCK/PB5`, pin 19 |
| `5V` | `VCC`, pin 7 |
| `5V` | `AVCC`, pin 20 |
| `GND` | `GND`, pins 8 and 22 |

Ping, fuse and lock:

```powershell
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -U lfuse:r:-:h -U hfuse:r:-:h -U lock:r:-:h
```

Expected:

```text
lfuse = 0xe3
hfuse = 0x99
lock  = 0xff
```

Regular entry:

```powershell
avrdude -c stk500v1 -P COM3 -b 19200 -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

For Linux, the port usually looks like `/dev/ttyACM0` or `/dev/ttyUSB0`:

```bash
avrdude -c stk500v1 -P /dev/ttyACM0 -b 19200 -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

In this circuit, the ArduinoISP may hang on a long Flash read during verify. If verify hangs, use writing in short blocks from the `chunks/` directory with read verification disabled:

- the first block is written with chip erase;
- the remaining blocks are written with `-D`;
- all blocks are written with `-V`.

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

After recording in blocks, check only the signature, fuse and lock, do not do a long dump/verify Flash.

If `avrdude` is installed via the ZakKemble AVR-GCC package and does not find `avrdude.conf`, run with an explicit `-C`, for example:

```powershell
avrdude -C "$env:LOCALAPPDATA\Microsoft\WinGet\Packages\ZakKemble.avr-gcc_Microsoft.Winget.Source_8wekyb3d8bbwe\avr-gcc-14.1.0-x64-windows\bin\avrdude.conf" -c stk500v1 -P COM3 -b 19200 -p m8 -U lfuse:r:-:h -U hfuse:r:-:h -U lock:r:-:h
```

## Firmware via USBasp / AVRASP / USBASP 2.0

USBasp, AVRASP and many boards labeled `USBASP 2.0` in `avrdude` are usually used as `usbasp` programmer.

ISP connection:

| USBasp | ATmega8A |
| --- | --- |
| `MOSI` | `MOSI/PB3`, pin 17 |
| `MISO` | `MISO/PB4`, pin 18 |
| `SCK` | `SCK/PB5`, pin 19 |
| `RST` / `RESET` | `RESET`, pin 1 |
| `VCC` | `VCC`, pin 7 and `AVCC`, pin 20 |
| `GND` | `GND`, pins 8 and 22 |

If the board is powered separately, do not connect `VCC` from the programmer, leave only the common ground `GND`.

Connection check:

```powershell
avrdude -c usbasp -p m8 -U lfuse:r:-:h -U hfuse:r:-:h -U lock:r:-:h
```

Write firmware and fuse:

```powershell
avrdude -c usbasp -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

On Linux the commands are the same. If a regular user does not have access to USBasp, run via `sudo` or add a udev rule:

```bash
sudo avrdude -c usbasp -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```Если USBasp старый и `avrdude` пишет предупреждение про `cannot set sck period`, это не всегда ошибка. Если связь нестабильна, поставить перемычку `slow SCK` на USBasp или добавить ключ `-B 10`:

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
