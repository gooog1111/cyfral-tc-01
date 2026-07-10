<!-- LANG_START -->
🇷🇺 [Русская версия](README.md)
<!-- LANG_END -->

<div align="center">

<img src="resources/header.svg" alt="Cyfral TC-01" width="900"/>

</div>





<!-- STATS_START -->
<!-- auto-updated by GitHub Actions · 2026-07-10 15:00 UTC -->

[![Views local](https://img.shields.io/badge/Views_local-61-ff6900?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01)
[![Views GitHub](https://img.shields.io/badge/Views_GitHub-13-ff6900?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01)
[![Unique visitors](https://img.shields.io/badge/Unique-3-blue?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01)
[![Clones](https://img.shields.io/badge/Clones-2361-purple?style=for-the-badge&logo=github)](https://github.com/gooog1111/cyfral-tc-01)
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
<!-- auto-updated by GitHub Actions · 2026-07-10 15:00 UTC -->

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
| `1`, `2` | device power `12 V AC` || `3`, `4` | key reader, contact `4` - general minus |
| `4`, `5` | lock opening button `SW`, contact `4` - common minus |
| `M+` and minus | exit to the castle; on `M+` to the lock there should be about `19 V DC` |

Contact `4` is used as a common negative for the reader and the open button.

## Lock control

The `LOCK` output drives the power transistor active low.

## #Mode 1: Electromagnetic lock

Used for a magnet that needs constant power when closed.

- Waiting: `PC1/LOCK = 0`, transistor is open, magnet is holding.
- Opening: `PC1/LOCK = 1`, the transistor is closed, power is removed from the magnet.
- After the opening timer: `PC1/LOCK = 0` again.

## # Mode 2: electromechanical lock

Used for a lock to which power is supplied only during opening.

- Waiting: `PC1/LOCK = 1`, the transistor is closed, there is no power to the lock.
- Opening: `PC1/LOCK = 0`, the transistor is open, power is supplied.
- After the opening timer: `PC1/LOCK = 1` again.

The mode is stored in the internal EEPROM of the ATmega8A at `0x09`:

- `0xFF` - mode 1, electromagnetic;
- `0x00` - mode 2, electromechanical.

If the value is corrupted, the firmware restores mode 1.

The autocollection mode is stored at `0x0A`:

- `0xFF` - auto-collection is disabled;
- `0x00` - auto collection is enabled.

If the value is damaged and after a complete reset, auto-collection is turned off.

## Opening time

The opening time is stored in an external EEPROM `24C64` in the service block. The unit of measurement is `100 ms`.

- default value: `0x0032`, that is, `50 * 100 ms = 5 секунд`;
- maximum: `0x0C00`, that is, about `307.2 секунд`;
- if the value is corrupted or too large, the default value is used.

During opening, the buzzer is constantly on, the green LED is on, the `LOCK` output is switched to the opening state.

## EEPROM memory 24C64

External EEPROM `24C64` is used for the key and parameter database.

| Address | Size | Destination |
| --- | ---: | --- |
| `0x0000..0x0003` | 4 bytes | service block |
| `0x0000..0x0001` | 2 bytes | number of user keys |
| `0x0002..0x0003` | 2 bytes | opening time in ticks of 100 ms |
| `0x0004..0x0007` | 4 bytes | master key |
| `0x0008..0x1FFF` | 4 bytes | custom keys |

Empty key cell: `0xFFFFFFFF`. This key is prohibited from being written to.

At startup, a self-diagnosis is performed:

- EEPROM availability is checked;
- the master key is checked;
- CRC keys are checked;- проверяется отсутствие непустых ключей после первой пустой ячейки;
- проверяется счётчик ключей в заголовке.

После трёх подряд ошибок EEPROM включается аварийное поведение: корректно прочитанный ключ может открыть дверь по общему таймеру.

## Обычный режим

При запуске устройство входит в обычный режим. Перемычка `XP3` для входа в сервисный режим больше не используется.

Поведение:

- в ожидании периодически мигает красный LED;
- кнопка `SW` открывает дверь сразу;
- короткое прикладывание мастер-ключа в обычном режиме открывает дверь;
- удержание мастер-ключа дольше `5 секунд` включает сервисный режим;
- пользовательский ключ из базы открывает дверь;
- неизвестный ключ не открывает дверь и получает сигнал отказа;
- при включённом автосборе любой корректный ключ открывает дверь, а неизвестный сохраняется в базе;
- если EEPROM повреждена, после нескольких ошибок включается аварийная логика.

## Сервисный режим

Сервисный режим включается удержанием мастер-ключа на 5 сек. (перемычка `XP3` не требуется)

Вход:

- приложить мастер-ключ и удерживать его дольше `5 секунд`;
- при входе звучат `3` длинных сигнала.

В сервисном режиме управление выполняется кнопкой `SW` и мастер-ключом.
Для выхода из сервисного режима снова удерживать мастер-ключ дольше `5 секунд`; выход подтверждается `2` длинными сигналами.

## Сервисное меню

Вход в меню:

- войти в сервисный режим удержанием мастер-ключа дольше `5 секунд`;
- после входа сразу озвучивается пункт `1`, меню готово к навигации кнопкой `SW`.

Навигация:

- короткое нажатие `SW` - следующий пункт меню;
- длинное нажатие `SW` - выбрать текущий пункт;
- пункты идут по кругу;
- номер пункта озвучивается короткими сигналами.

Пункты меню:

| Пункт | Сигнал | Назначение |
| ---: | --- | --- |
| 1 | `1` короткий | добавление пользовательских ключей |
| 2 | `2` коротких | настройка времени открытия |
| 3 | `3` коротких | удаление пользовательских ключей |
| 4 | `4` коротких | настройка режима замка |
| 5 | `5` коротких | настройка автосбора ключей |

## # Пункт 1: добавление ключей

После входа в пункт:

- приложить новый ключ;
- если приложен читаемый, но невалидный RW1990/TM01/RW2004/Dallas-совместимый ключ, прошивка пробует записать в него случайный код, затем перечитать и только после этого добавить в базу;
- если ключ добавлен, звучит `1` длинный сигнал;
- если ключ уже есть, звучат `2` коротких;
- мастер-ключ не добавляется как пользовательский;
- короткое нажатие `SW` выходит из пункта обратно в меню.

## # Пункт 2: время открытия

После входа в пункт:

- нажать `SW`, чтобы начать отсчёт;
- каждую секунду звучит `1` короткий сигнал;
- нажать `SW` ещё раз, чтобы сохранить время;
- сохранение подтверждается `2` короткими сигналами.

Внутри прошивки сохранённое значение переводится в тики по 100 ms.

## # Пункт 3: удаление ключей

После входа в пункт:

- приложить пользовательский ключ;
- если ключ удалён, звучит `1` короткий сигнал;
- если ключа нет в базе, звучат `2` коротких;
- мастер-ключ не удаляется;
- короткое нажатие `SW` выходит из пункта обратно в меню.

## # Пункт 4: режим замка

После выбора пункта длинным нажатием:

- устройство входит в настройку режима замка;
- сразу озвучивает текущий режим: `1` короткий или `2` коротких;
- короткое нажатие переключает `1 -> 2 -> 1` по кругу;
- длинное нажатие сохраняет выбранный режим;
- после сохранения звучит `1` длинный сигнал выхода и затем `1` или `2` коротких, какой режим сохранён.

Для электромагнитного замка на текущей плате нужен режим `1`.

## # Пункт 5: автосбор ключей

- устройство сразу озвучивает текущий режим: `1` короткий - выключен, `2` коротких - включён;
- короткое нажатие переключает режимы `1 -> 2 -> 1`;
- длинное нажатие сохраняет выбранный режим;
- при включённом автосборе любой корректно прочитанный ключ открывает дверь;
- неизвестный ключ перед открытием добавляется в базу, уже записанный ключ повторно не сохраняется;
- если приложен читаемый, но невалидный RW1990/TM01/RW2004/Dallas-совместимый ключ, прошивка пробует записать в него случайный код, затем перечитать и только после этого добавить в базу;
- если ключ читается, но не записался из-за ошибки, заполненной памяти или неудачной перезаписи RW-заготовки, звучит трель ошибки и дверь не открывается.

Автоматическая перезапись реализована для RW1990-подобных, TM01/TM01C и RW2004/TM2004 заготовок. Для Metakom/Cyfral-совместимых ключей в прошивке есть чтение, но нет протокола записи.

## Сброс и новый мастер-ключ

Базу можно полностью очистить и записать новый мастер-ключ даже при потерянном старом мастер-ключе. Для защиты от случайного запуска сброс требует установленной перемычки `XP3`.

1. Установить перемычку `XP3`.
2. Удерживать кнопку `SW` около `10 секунд`, не снимая `XP3`.
3. После сигнала приложить новый мастер-ключ.
4. Прошивка очистит пользовательские ключи, восстановит параметры по умолчанию и запишет мастер-ключ.

Эта процедура работает из обычного режима и не требует старого мастер-ключа. В сервисном режиме сброс запускается так же: `XP3` плюс удержание `SW`.

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
```If USBasp is old and `avrdude` writes a warning about `cannot set sck period`, this is not always an error. If the connection is unstable, place the `slow SCK` jumper on USBasp or add the `-B 10` key:

```bash
avrdude -c usbasp -B 10 -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

## Firmware via AVRISP / AVRISP mkII

For original Atmel/Microchip AVRISP mkII and compatible programmers, `avrdude` uses `-c avrispmkII`.

Check:

```bash
avrdude -c avrispmkII -P usb -p m8 -U lfuse:r:-:h -U hfuse:r:-:h -U lock:r:-:h
```

Entry:

```bash
avrdude -c avrispmkII -P usb -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

If you are using an older AVRISP/STK500 compatible serial programmer, the command usually looks like this:

```bash
avrdude -c avrisp -P COM3 -b 19200 -p m8 -U flash:w:TC-01.hex:i -U lfuse:w:0xE3:m -U hfuse:w:0x99:m
```

On Linux, use your own port instead of `COM3`, for example `/dev/ttyUSB0`.

## ST-Link V2

ST-Link V2 does not directly flash `ATmega8A` via AVR ISP. It is designed for STM8/STM32 via SWIM/SWD, while the ATmega8A uses SPI ISP.

You can use ST-Link V2 for this board only if it is flashed with alternative firmware, which turns it into an AVR ISP-compatible programmer. Normally TC-01 requires ArduinoISP, USBasp/AVRASP, AVRISP mkII or other programmer supported by `avrdude` for `-p m8`.

## Fuse bits

The current firmware is designed for an internal RC oscillator around `4 MHz`:

```text
lfuse = 0xE3
hfuse = 0x99
lock  = 0xFF
```

Before writing fuse, it is advisable to first read the current values. Invalid fuse may disable the ISP or select an unconnected clock source.

![Расстановка fuse-бит](fuse.jpg)

## Lock diagnostics

If the electromagnetic lock does not hold

1. Make sure that in step 4 the `1` mode is saved.
2. Measure the voltage directly at the magnet terminals with the magnet connected.
3. Measure `PC1/LOCK`, pin 24 ATmega8A:
   - in standby mode `1` should be about `0 V`;
   - when opening it should be about `5 V`.
4. If the `PC1` switches correctly and the magnet is weak, look for a problem in the power supply, power transistor, wiring, ground, the magnet itself, or the mating part.

The firmware does not use PWM for the lock. The `LOCK` output is kept constantly in one state and switches only during the opening time.
