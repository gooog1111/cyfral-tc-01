#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include "i2c.h"

#define TD		(1<<PB1)
#define RED		(1<<PB3)
#define SYS		(1<<PB4)
#define GREEN	(1<<PB5)

#define E_TD	(1<<PD5)
#define TC		(1<<PD6)
#define E_TC	(1<<PD7)

#define WP		(1<<PC0)
#define LOCK	(1<<PC1)
#define BEEP	(1<<PC2)
#define SW		(1<<PC3)
#define SDA		(1<<PC4)
#define SLC		(1<<PC5)

#define EEPROM_INFO_ADDR	0x0000
#define MASTER_KEY_ADDR		0x0004
#define USER_KEYS_ADDR		0x0008
#define EEPROM_END_ADDR		0x2000
#define MAX_OPEN_TIME		0x0C00
#define DEFAULT_OPEN_TIME	0x0032
#define MAX_USER_KEYS		((EEPROM_END_ADDR - USER_KEYS_ADDR) / 4)
#define STORAGE_ERROR_LIMIT	3
#define LOCK_MODE_ADDR		0x09
#define LOCK_MODE_MAGNETIC	0xFF
#define LOCK_MODE_MECH		0x00
#define AUTO_COLLECT_ADDR	0x0A
#define AUTO_COLLECT_OFF	0xFF
#define AUTO_COLLECT_ON		0x00
#define DS1990_FAMILY		0x01
#define DS1992_FAMILY		0x08
#define DS1995_FAMILY		0x0A

typedef union union_chunk{
	uint8_t bytes[4];
	uint16_t ints[2];
	uint32_t longs;
} chunk;

typedef enum enum_modes{NORMAL,JUMPER} modes;
typedef enum enum_key{NO_KEY,ERROR,MASTER_KEY,FOUND,NOT_FOUND} key;
typedef enum enum_add{ADD_OK,ADD_DUP,ADD_FULL,ADD_BAD,ADD_ERROR} add_result;
typedef enum enum_setup{SET_ADD,SET_TIME,SET_DEL,SET_LOCK,SET_AUTO_COLLECT} setup_mode;

uint8_t storage_error = 0;
uint8_t storage_error_count = 0;

void __attribute__ ((noinline)) delay_01()
{
	_delay_ms(100);
}

void __attribute__ ((noinline)) delay_5000()
{
	_delay_ms(5);
}

void __attribute__ ((noinline)) ds_450us()
{
	_delay_us(450);
}

void __attribute__ ((noinline)) ds_70us()
{
	_delay_us(70);
}

void __attribute__ ((noinline)) ds_5us()
{
	_delay_us(4);
}

uint8_t eeprom_read(uint8_t address)
{
	while(EECR & (1<<EEWE));
	EEAR = address;
	EECR |= (1<<EERE);
	return EEDR;
}

void eeprom_write(uint8_t address, uint8_t data)
{
	while(EECR & (1<<EEWE));
	EEAR = address;
	EEDR = data;
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
}

uint8_t lock_mode()
{
	return eeprom_read(LOCK_MODE_ADDR) == LOCK_MODE_MECH ? LOCK_MODE_MECH : LOCK_MODE_MAGNETIC;
}

void lock_mode_validate()
{
	uint8_t mode = eeprom_read(LOCK_MODE_ADDR);
	if((mode == LOCK_MODE_MAGNETIC) || (mode == LOCK_MODE_MECH)) return;
	eeprom_write(LOCK_MODE_ADDR,LOCK_MODE_MAGNETIC);
}

void lock_mode_default()
{
	eeprom_write(LOCK_MODE_ADDR,LOCK_MODE_MAGNETIC);
}

uint8_t auto_collect_mode()
{
	return eeprom_read(AUTO_COLLECT_ADDR) == AUTO_COLLECT_ON ? AUTO_COLLECT_ON : AUTO_COLLECT_OFF;
}

void auto_collect_validate()
{
	uint8_t mode = eeprom_read(AUTO_COLLECT_ADDR);
	if((mode == AUTO_COLLECT_OFF) || (mode == AUTO_COLLECT_ON)) return;
	eeprom_write(AUTO_COLLECT_ADDR,AUTO_COLLECT_OFF);
}

void auto_collect_default()
{
	eeprom_write(AUTO_COLLECT_ADDR,AUTO_COLLECT_OFF);
}

void beep_on()
{
	wdt_reset();
	PORTB |= GREEN;
	PORTB &= ~(RED);
	PORTC &= ~(BEEP);
	delay_01();
	wdt_reset();
	delay_01();
	wdt_reset();
}

void beep_off()
{
	wdt_reset();
	PORTB &= ~(GREEN|RED);
	PORTC |= BEEP;
	delay_01();
	wdt_reset();
}

void beep_err()
{
	for(uint8_t i=0;i<32;i++){
		PORTC &= ~(BEEP);
		_delay_ms(10);
		PORTC |= BEEP;
		_delay_ms(10);
		wdt_reset();
	}
	for(uint8_t i=0;i<10;i++){
		_delay_ms(100);
		wdt_reset();
	}
	
}

void lock_on()
{
	if(lock_mode() == LOCK_MODE_MAGNETIC) PORTC &= ~LOCK;
	else PORTC |= LOCK;
}

void lock_off()
{
	if(lock_mode() == LOCK_MODE_MAGNETIC) PORTC |= LOCK;
	else PORTC &= ~LOCK;
}

void green()
{
	PORTB |= GREEN;
	//asm volatile("sbi %0, 0x05" :: "I" (_SFR_IO_ADDR(PORTB)));
	//asm volatile("cbi %0, 0x03" :: "I" (_SFR_IO_ADDR(PORTB)));
	PORTB &= ~RED;
	
}

void red()
{
	PORTB &= ~GREEN;
	PORTB |= RED;
}

void led_off()
{
	PORTB &= ~(RED|GREEN);
}

void __attribute__ ((noinline)) beep(uint8_t times)
{
	for(;times>0;times--){
		wdt_reset();
		beep_on();
		beep_off();
		delay_01();
		wdt_reset();
		delay_01();
		wdt_reset();
	}
}

void beep_long(uint8_t times)
{
	for(;times>0;times--){
		PORTB |= GREEN;
		PORTB &= ~(RED);
		PORTC &= ~(BEEP);
		for(uint8_t i=0;i<8;i++){
			delay_01();
			wdt_reset();
		}
		PORTB &= ~(GREEN|RED);
		PORTC |= BEEP;
		delay_01();
		wdt_reset();
		delay_01();
		wdt_reset();
		delay_01();
		wdt_reset();
	}
}

void diag_beep(uint8_t code)
{
	beep_long(1);
	beep(code);
}

void deny_beep()
{
	beep(2);
	beep_long(2);
}

/*************************************************************************	
  Софтверный UART для отладки, посылает один байт.
*************************************************************************/
void soft_uart(uint8_t data)
{
	DDRD |= 1<<PD1;
	PORTD &= ~(1<<PD1);
	_delay_us(103);
	for(uint8_t i=0;i<8;i++){
		if(data & 0x01)PORTD |= 1<<PD1;
		else PORTD &= ~(1<<PD1);
		data >>= 1;
		_delay_us(103);
	}
	PORTD |= 1<<PD1;
	_delay_us(103);
}

/*************************************************************************	
  Посылает HEX числа через UART.
*************************************************************************/
void soft_uart_hex(uint8_t data)
{
	soft_uart('0');
	soft_uart('x');
	uint8_t temp = data>>4;
	if(temp > 0x09) temp += 'A' - 10;
	else temp += '0';
	soft_uart(temp);
	temp = data & 0x0F;
	if(temp > 0x09) temp += 'A' - 10;
	else temp += '0';
	soft_uart(temp);
	soft_uart(' ');
}

/*************************************************************************	
  Инициализация портов.
*************************************************************************/
void init()
{
	DDRB = RED|GREEN;
	PORTB = SYS;
	DDRC = LOCK|BEEP;
	PORTC = BEEP|SW;
	lock_on();
	DDRD = E_TD;
	PORTD = E_TD;
}

/*************************************************************************	
  Вычисляет CRC.
*************************************************************************/
uint8_t ds_crc(uint8_t crc, uint8_t data)
{
	crc ^= data;
	for (uint8_t i=0;i<8;i++){
		if (crc & 0x01) crc = (crc >> 1) ^ 0x8C;
		else crc >>= 1;
	}
	return crc;
}

uint8_t key_blank(chunk* data)
{
	return data->longs == 0xFFFFFFFF;
}

uint8_t key_bad(chunk* data)
{
	uint8_t crc = 0;
	if(key_blank(data)) return 1;
	for(uint8_t i=0;i<4;i++) crc = ds_crc(crc, data->bytes[i]);
	return crc;
}

/*************************************************************************	
  Сравнивает чанки по 4 байта с учётом багов. 0 - равны
*************************************************************************/
uint8_t chunk_compare(chunk* key, chunk* eeprom)
{
	uint8_t temp = 0;
	for(uint8_t i=0;i<4;i++){
		temp = ds_crc(temp, eeprom->bytes[i]);
	}
	if(temp)return 1;
	if(key->bytes[0] != eeprom->bytes[0]) return 1;
	if(key->bytes[1] != eeprom->bytes[1]) return 1;
	eeprom->bytes[2] |= 0x80;
	if((key->bytes[2] & 0x3F) != (eeprom->bytes[2] & 0x3F)) return 1;
	return 0;
}

/*************************************************************************	
  Открывает дверь на время прочитанное из ПЗУ.
*************************************************************************/
void open()
{
	chunk buffer;
	uint16_t open_time = 0;
	if(i2c_read_c64(EEPROM_INFO_ADDR,buffer.bytes)) open_time = DEFAULT_OPEN_TIME;
	else open_time = buffer.bytes[2]<<8 | buffer.bytes[3];
	if((open_time == 0xFFFF) || (open_time > MAX_OPEN_TIME)) open_time = DEFAULT_OPEN_TIME;
	lock_off();
	beep_on();
	for(uint16_t i=0; i<open_time;i++){
		_delay_ms(100);
		wdt_reset();
	}
	lock_on();
	beep_off();
}

/*************************************************************************	
  Читает ключи Cyfral. 0 - ключ прочитан в data
*************************************************************************/
uint8_t cyfral_rx(chunk* data)
{
	register uint8_t level asm("r2") = 0;
	uint8_t bit = 0;
	uint8_t bits = 0;
	uint8_t word = 0;
	uint8_t words = 0;
	uint8_t count = 0;
	uint8_t one = 0;
	uint8_t timerH = 0;
	uint8_t timerL = 0;
	uint8_t timer = 0;
	uint8_t start = 0;
	for(uint16_t i=0;i<4000;i++){
		if((ACSR & (1<<ACO)) != level){
			level = ACSR & (1<<ACO);
			if(level){
				timerH = timer;
			}else{
				level = ACSR & (1<<ACO);
				timerL = timer;
				if(timerH > timerL) bit = 0;
				else bit = 1;
				count++;
				if(!start){
					if(bit){
						if(count > 3) start = 1;
						count = 0;
					}
				} else {
					if(!bit){
						count = 0;
						one++;
					}
					bits++;
					if(bits >= 4){
						if(one != 1) return 1;
						word >>= 2;
						word |= count<<2;
						if(words & 0x01){
							data->ints[0] <<= 4;
							data->ints[0] |= word;
							word = 0;
						}
						bits = 0;
						count = 0;
						one = 0;
						words++;
					}
					if(words > 7){
						return 0;
					}
				}
			}
			timer = 0;
		}
		timer++;
		if(timer > 120) return 1;
		_delay_us(5);
	}
	return 1;
}

/*************************************************************************	
  Читает ключи Metakom. 0 - ключ прочитан в data
*************************************************************************/
uint8_t mk_rx(chunk* data)
{
	register uint8_t level asm("r2") = 0;
	uint8_t bit = 0;
	uint8_t bits = 0;
	uint8_t byte = 0;
	uint8_t bytes = 0;
	uint8_t count = 0;
	uint16_t timerA = 0;
	uint8_t timerL = 0;
	uint8_t timer = 0;
	uint8_t start = 0;
	for(uint16_t i=0;i<4000;i++){
		if((ACSR & (1<<ACO)) != level){
			level = ACSR & (1<<ACO);
			if(level){
				timerL = timer;
			}else{
				if(timer > timerA) bit = 1;
				else bit = 0;
				if(!start){
					count++;
					if(count < 32){
						timerA += timerL + timer;
					} else if(count == 32) {
						timerA >>= 6;
					} else if(timerL > (timerA<<1)){
						start = 1;
					}
				} else {
					bits++;
					byte <<= 1;
					byte |= bit;
					if(bit)count++;
					if(start == 1 && byte == 0x02){
						start = 2;
						bits = 0;
						count = 0;
					}
					if(bits == 8){
						data->bytes[bytes] = byte;
						bytes++;
						byte = 0;
						bits = 0;
						if(count & 1) return 1;
					}
					if(bytes == 4) return 0;
				}
			}
			timer = 0;
		}
		timer++;
		if(timer > 120) return 1;
		_delay_us(5);
	}
	return 1;
}

/*************************************************************************	
  Читает ключи Dallas. 0 - ключ прочитан в data
*************************************************************************/
uint8_t ds_read(chunk* data)
{
	uint8_t crc = 0;
	uint8_t result = 0;
	uint8_t bytes[8];
	//PORTD |= E_TD;
	//DDRD |= E_TD;
	DDRB |= TD;
	ds_450us();
	ds_450us();
	DDRB &= ~TD;
	ds_5us();
	ds_5us();
	if(!(PINB & TD)) result = 1;
	ds_70us();
	if(PINB & TD) result = 1;
	ds_450us();
	if(!(PINB & TD)) result = 1;
	if(result){
		//DDRD &= ~E_TD;
		//PORTD &= ~E_TD;
		return 1;
	}
	result = 0x33;
	for (uint8_t i=0;i<8;i++){
		DDRB |= TD;
		if (result & 0x01){
			ds_5us();
			DDRB &= ~TD;
			ds_5us();			
			ds_70us();
		}else{
			ds_70us();
			DDRB &= ~TD;
			ds_5us();
			ds_5us();
		}
		result >>= 1;
	}
	for(uint8_t i=0;i<8;i++){
		for (uint8_t j=0;j<8;j++){
			result >>= 1;
			DDRB |= TD;
			ds_5us();
			DDRB &= ~TD;
			ds_5us();
			ds_5us();
			if (PINB & TD) result |= 0x80;
			ds_70us();
		}
		bytes[i] = result;
		crc = ds_crc(crc,bytes[i]);
	}
	//DDRD &= ~E_TD;
	//PORTD &= ~E_TD;
	
	if(crc) return 2;
	if((bytes[0] != DS1990_FAMILY) && (bytes[0] != DS1992_FAMILY) && (bytes[0] != DS1995_FAMILY)) return 2;

	for(uint8_t i=0;i<3;i++)data->bytes[i] = bytes[i+1];
	data->bytes[2] |= 0x80;
	data->bytes[2] &= 0xBF;
	crc = 0;
	for(uint8_t i=0;i<3;i++)crc = ds_crc(crc, data->bytes[i]);
	data->bytes[3] = crc;
	return 0;
}

/*************************************************************************	
  Пытается прочитать разные типы ключей. 0 - ключ прочитан в data
*************************************************************************/
uint8_t key_read(chunk* data)
{
	chunk read, check;
	uint8_t temp = 0;
	for(uint8_t i=0;i<4;i++)data->bytes[i] = 0;
	if((mk_rx(&read) == 0) && (mk_rx(&check) == 0)){
		if(read.longs == check.longs){
			*data = read;
			data->bytes[2] |= 0x80;
			data->bytes[2] &= 0xBF;
			for(uint8_t i=0;i<3;i++) temp = ds_crc(temp, data->bytes[i]);
			data->bytes[3] = temp;
			temp = 0;
			for(uint8_t i=0;i<4;i++) if(data->bytes[i]) temp++;
			if(!temp) return 1;
			return 0;
		}
		return 1;
	}
	if(!ds_read(data)){
		return 0;
	}
	_delay_ms(10);
	for(uint8_t i=0;i<4;i++)data->bytes[i] = 0;
	if((cyfral_rx(&read) == 0) && (cyfral_rx(&check) == 0)){
		if(read.ints[0] == check.ints[0]){
			data->bytes[0] = read.ints[0];
			data->bytes[1] = read.ints[0]>>8;
			data->bytes[2] = 0x80;
			for(uint8_t i=0;i<3;i++) temp = ds_crc(temp, data->bytes[i]);
			data->bytes[3] = temp;
			return 0;
		}
		return 1;
	}
	return 1;
}

/*************************************************************************	
  Обновляет количество пользовательских ключей в заголовке ПЗУ.
*************************************************************************/
uint8_t users_count_write(uint16_t count)
{
	chunk buffer;
	if(i2c_read_c64(EEPROM_INFO_ADDR,buffer.bytes)) return 1;
	buffer.bytes[0] = count>>8;
	buffer.bytes[1] = count;
	return i2c_write_c64(EEPROM_INFO_ADDR,buffer.bytes);
}

uint8_t storage_header_init()
{
	chunk buffer;
	buffer.bytes[0] = 0;
	buffer.bytes[1] = 0;
	buffer.bytes[2] = DEFAULT_OPEN_TIME>>8;
	buffer.bytes[3] = DEFAULT_OPEN_TIME;
	return i2c_write_c64(EEPROM_INFO_ADDR,buffer.bytes);
}

/*************************************************************************	
  Ищет ключ в ПЗУ. 0 - найден или дошли до пустой ячейки.
*************************************************************************/
uint8_t key_find(chunk* data, uint16_t* address, key* result)
{
	chunk buffer;
	*result = NOT_FOUND;
	for(*address=MASTER_KEY_ADDR;*address<EEPROM_END_ADDR;*address+=4){
		if(i2c_read_c64(*address,buffer.bytes)) return 1;
		if(key_blank(&buffer)) return 0;
		if(key_bad(&buffer)){
			*result = ERROR;
			return 1;
		}
		if(chunk_compare(data,&buffer)) continue;
		*result = (*address == MASTER_KEY_ADDR) ? MASTER_KEY : FOUND;
		return 0;
	}
	return 0;
}

/*************************************************************************	
  Записывает ключ в свободную ячейку ПЗУ.
*************************************************************************/
add_result key_add(chunk* data)
{
	uint16_t address;
	key result;
	if(key_bad(data)) return ADD_BAD;
	if(key_find(data,&address,&result)) return ADD_ERROR;
	if(result != NOT_FOUND) return ADD_DUP;
	if(address >= EEPROM_END_ADDR) return ADD_FULL;
	if(i2c_write_c64(address,data->bytes)) return ADD_ERROR;
	address = (address - USER_KEYS_ADDR) >> 2;
	if(users_count_write(address + 1)) return ADD_ERROR;
	return ADD_OK;
}

/*************************************************************************	
  Очищает список пользовательских ключей. Мастер-ключ не трогает.
*************************************************************************/
uint8_t users_clear()
{
	chunk buffer;
	buffer.longs = 0xFFFFFFFF;
	for(uint16_t address=USER_KEYS_ADDR;address<EEPROM_END_ADDR;address+=4){
		chunk check;
		if(i2c_read_c64(address,check.bytes)) return 1;
		if(!key_blank(&check)){
			if(i2c_write_c64(address,buffer.bytes)) return 1;
		}
		wdt_reset();
	}
	return users_count_write(0);
}

uint8_t storage_clear_all()
{
	chunk buffer;
	buffer.longs = 0xFFFFFFFF;
	lock_mode_default();
	auto_collect_default();
	if(i2c_write_c64(MASTER_KEY_ADDR,buffer.bytes)) return 1;
	if(users_clear()) return 1;
	return storage_header_init();
}

uint8_t master_write(chunk* data)
{
	chunk buffer;
	if(key_bad(data)) return 1;
	if(i2c_write_c64(MASTER_KEY_ADDR,data->bytes)) return 1;
	if(i2c_read_c64(EEPROM_INFO_ADDR,buffer.bytes)) return 1;
	uint16_t open_time = buffer.bytes[2]<<8 | buffer.bytes[3];
	if((open_time == 0xFFFF) || (open_time > MAX_OPEN_TIME)){
		buffer.bytes[2] = DEFAULT_OPEN_TIME>>8;
		buffer.bytes[3] = DEFAULT_OPEN_TIME;
	}
	return i2c_write_c64(EEPROM_INFO_ADDR,buffer.bytes);
}

uint8_t key_delete(chunk* data)
{
	chunk buffer, next;
	uint16_t address;
	key result;
	if(key_bad(data)) return 2;
	if(key_find(data,&address,&result)) return 2;
	if(result != FOUND) return 1;
	for(uint16_t next_addr=address+4;next_addr<EEPROM_END_ADDR;next_addr+=4){
		if(i2c_read_c64(next_addr,next.bytes)) return 2;
		if(i2c_write_c64(address,next.bytes)) return 2;
		if(key_blank(&next)) break;
		address = next_addr;
		wdt_reset();
	}
	uint16_t count = 0;
	for(address=USER_KEYS_ADDR;address<EEPROM_END_ADDR;address+=4){
		if(i2c_read_c64(address,buffer.bytes)) return 2;
		if(key_blank(&buffer)) break;
		count++;
	}
	if(users_count_write(count)) return 2;
	return 0;
}

/*************************************************************************	
  Самодиагностика ПЗУ: мастер, заголовок, CRC и переполнение.
*************************************************************************/
uint8_t storage_diag()
{
	chunk info, master, buffer;
	uint16_t count = 0;
	uint16_t header_count = 0;
	uint8_t blank = 0;
	lock_mode_validate();
	auto_collect_validate();
	if(i2c_read_c64(EEPROM_INFO_ADDR,info.bytes)) return 1;
	header_count = info.bytes[0]<<8 | info.bytes[1];
	if(header_count > MAX_USER_KEYS) return 5;
	uint16_t open_time = info.bytes[2]<<8 | info.bytes[3];
	if((open_time == 0xFFFF) || (open_time > MAX_OPEN_TIME)){
		info.bytes[2] = DEFAULT_OPEN_TIME>>8;
		info.bytes[3] = DEFAULT_OPEN_TIME;
		if(i2c_write_c64(EEPROM_INFO_ADDR,info.bytes)) return 1;
	}
	if(i2c_read_c64(MASTER_KEY_ADDR,master.bytes)) return 1;
	if(key_bad(&master)) return 2;
	for(uint16_t address=USER_KEYS_ADDR;address<EEPROM_END_ADDR;address+=4){
		if(i2c_read_c64(address,buffer.bytes)) return 1;
		if(key_blank(&buffer)){
			blank = 1;
			continue;
		}
		if(blank) return 5;
		if(key_bad(&buffer)) return 3;
		if(!chunk_compare(&master,&buffer)) return 4;
		count++;
		wdt_reset();
	}
	if(count > MAX_USER_KEYS) return 5;
	if(count != header_count) return 5;
	return 0;
}

void storage_ok()
{
	storage_error = 0;
	storage_error_count = 0;
}

void storage_fail(uint8_t code)
{
	if(storage_error_count < STORAGE_ERROR_LIMIT) storage_error_count++;
	diag_beep(code);
	if(storage_error_count >= STORAGE_ERROR_LIMIT) storage_error = code;
}

void storage_check_start(uint8_t sound)
{
	uint8_t code = 0;
	for(uint8_t i=0;i<STORAGE_ERROR_LIMIT;i++){
		code = storage_diag();
		if(!code){
			storage_ok();
			return;
		}
		if(storage_error_count < STORAGE_ERROR_LIMIT) storage_error_count++;
		wdt_reset();
	}
	storage_error = code;
	if(sound) diag_beep(code);
}

uint8_t storage_check_once()
{
	uint8_t code = storage_diag();
	if(code) storage_fail(code);
	else storage_ok();
	return code;
}

void wait_key_release()
{
	chunk buffer;
	while(!key_read(&buffer)){
		delay_01();
		wdt_reset();
	}
}

/*************************************************************************	
  Читает ключ и ищет его в ПЗУ.
*************************************************************************/
key key_get(chunk* data)
{
	if(key_read(data))return NO_KEY;
	if(key_bad(data)) return NOT_FOUND;
	uint16_t address = 0;
	key result = NOT_FOUND;
	if(key_find(data,&address,&result)) return ERROR;
	if(result == NOT_FOUND) red();
	return result;
}

uint8_t key_read_stable(chunk* data)
{
	chunk check;
	if(key_read(data)) return 1;
	if(key_bad(data)) return 1;
	delay_01();
	if(key_read(&check)) return 1;
	if(key_bad(&check)) return 1;
	if(chunk_compare(data,&check)) return 1;
	return 0;
}

uint8_t sw_raw_pressed()
{
	return !(PINC & SW);
}

uint8_t sw_pressed()
{
	if(!sw_raw_pressed()) return 0;
	_delay_ms(20);
	wdt_reset();
	return sw_raw_pressed();
}

void sw_wait_release()
{
	while(sw_raw_pressed()){
		delay_01();
		wdt_reset();
	}
}

uint16_t sw_hold_time(uint16_t limit)
{
	uint16_t time = 0;
	while(sw_raw_pressed()){
		delay_01();
		wdt_reset();
		if(time < limit) time++;
	}
	return time;
}

uint8_t sw_action()
{
	if(!sw_pressed()) return 0;
	uint16_t time = sw_hold_time(30);
	return (time >= 15) ? 2 : 1;
}

void setup_signal(setup_mode setup)
{
	beep((uint8_t)setup + 1);
}

void delay_5s()
{
	for(uint8_t i=0;i<50;i++){
		delay_01();
		wdt_reset();
	}
}

void set_open_time()
{
	chunk buffer;
	uint16_t seconds = 0;
	while(!sw_pressed()){
		if(PINB & SYS) return;
		wdt_reset();
	}
	sw_wait_release();
	while(1){
		for(uint8_t i=0;i<10;i++){
			if(sw_pressed()){
				sw_wait_release();
				if(i || !seconds) seconds++;
				uint16_t open_time = seconds * 10;
				if(open_time > MAX_OPEN_TIME) open_time = MAX_OPEN_TIME;
				if(i2c_read_c64(EEPROM_INFO_ADDR,buffer.bytes)) { beep_err(); return; }
				buffer.bytes[2] = open_time>>8;
				buffer.bytes[3] = open_time;
				if(i2c_write_c64(EEPROM_INFO_ADDR,buffer.bytes)) beep_err();
				else beep(2);
				return;
			}
			if(PINB & SYS) return;
			delay_01();
			wdt_reset();
		}
		seconds++;
		if(seconds > (MAX_OPEN_TIME / 10)) seconds = MAX_OPEN_TIME / 10;
		beep(1);
	}
}

void lock_mode_signal(uint8_t mode)
{
	beep(mode == LOCK_MODE_MAGNETIC ? 1 : 2);
}

void set_lock_mode()
{
	uint8_t mode = lock_mode();
	lock_mode_signal(mode);
	while(!(PINB & SYS)){
		uint8_t action = sw_action();
		if(action == 1){
			mode = (mode == LOCK_MODE_MAGNETIC) ? LOCK_MODE_MECH : LOCK_MODE_MAGNETIC;
			lock_mode_signal(mode);
		}else if(action == 2){
			eeprom_write(LOCK_MODE_ADDR,mode);
			lock_on();
			beep_long(1);
			lock_mode_signal(mode);
			return;
		}
		wdt_reset();
	}
}

void auto_collect_signal(uint8_t mode)
{
	beep(mode == AUTO_COLLECT_OFF ? 1 : 2);
}

void set_auto_collect_mode()
{
	uint8_t mode = auto_collect_mode();
	auto_collect_signal(mode);
	while(!(PINB & SYS)){
		uint8_t action = sw_action();
		if(action == 1){
			mode = (mode == AUTO_COLLECT_OFF) ? AUTO_COLLECT_ON : AUTO_COLLECT_OFF;
			auto_collect_signal(mode);
		}else if(action == 2){
			eeprom_write(AUTO_COLLECT_ADDR,mode);
			beep_long(1);
			auto_collect_signal(mode);
			return;
		}
		wdt_reset();
	}
}

void stream_add()
{
	chunk buffer;
	key search;
	while(1){
		if(PINB & SYS) return;
		if(sw_pressed()){ sw_wait_release(); return; }
		search = key_get(&buffer);
		switch(search){
			case NO_KEY: break;
			case MASTER_KEY: { wait_key_release(); break; }
			case FOUND: { beep(2); wait_key_release(); break; }
			case NOT_FOUND:{
				add_result add = key_add(&buffer);
				if(add == ADD_OK) beep_long(1);
				else if(add == ADD_DUP) beep(2);
				else beep_err();
				wait_key_release();
				break;
			}
			default: { beep_err(); break; }
		}
		wdt_reset();
	}
}

void stream_delete()
{
	chunk buffer;
	key search;
	while(1){
		if(PINB & SYS) return;
		if(sw_pressed()){ sw_wait_release(); return; }
		search = key_get(&buffer);
		switch(search){
			case NO_KEY: break;
			case MASTER_KEY: { wait_key_release(); break; }
			case FOUND: {
				uint8_t del = key_delete(&buffer);
				if(del == 0) beep(1);
				else if(del == 1) beep(2);
				else beep_err();
				wait_key_release();
				break;
			}
			case NOT_FOUND: { beep(2); wait_key_release(); break; }
			default: { beep_err(); break; }
		}
		wdt_reset();
	}
}

void setup_menu()
{
	setup_mode setup = SET_ADD;
	setup_signal(setup);
	while(!(PINB & SYS)){
		uint8_t action = sw_action();
		if(action == 1){
			setup = (setup == SET_AUTO_COLLECT) ? SET_ADD : setup + 1;
			setup_signal(setup);
		}else if(action == 2){
			beep_long(1);
			switch(setup){
				case SET_ADD: stream_add(); break;
				case SET_TIME: set_open_time(); break;
				case SET_DEL: stream_delete(); break;
				case SET_LOCK: set_lock_mode(); continue;
				case SET_AUTO_COLLECT: set_auto_collect_mode(); continue;
			}
			setup_signal(setup);
		}
		wdt_reset();
	}
}

uint8_t master_reset_flow()
{
	chunk buffer;
	uint8_t led_timer = 0;
	if(storage_clear_all()){ beep_err(); return 0; }
	beep(3);
	while(!(PINB & SYS)){
		led_timer++;
		if(led_timer >= 8){
			if(PORTB & GREEN) led_off();
			else green();
			led_timer = 0;
		}
		if(!key_read_stable(&buffer)){
			led_off();
			if(master_write(&buffer)){ beep_err(); return 0; }
			beep(2);
			delay_5s();
			wait_key_release();
			break;
		}
		wdt_reset();
	}
	while(!(PINB & SYS)){
		if(key_read_stable(&buffer)){
			wdt_reset();
			continue;
		}
		uint16_t address = 0;
		key search = NOT_FOUND;
		if(key_find(&buffer,&address,&search)){
			beep_err();
			wait_key_release();
			continue;
		}
		if(search == MASTER_KEY){
			beep_long(1);
			wait_key_release();
			storage_check_once();
			while(!sw_pressed()){
				if(PINB & SYS) return 0;
				wdt_reset();
			}
			sw_wait_release();
			return 1;
		}else{
			beep_err();
			wait_key_release();
		}
		wdt_reset();
	}
	return 0;
}

void jumper_service()
{
	chunk buffer;
	lock_on();
	beep_long(3);
	while(!(PINB & SYS)){
		lock_on();
		if(sw_pressed()){
			uint16_t time = 0;
			while(sw_raw_pressed()){
				delay_01();
				wdt_reset();
				time++;
				if(time >= 100){
					sw_wait_release();
					if(master_reset_flow()){
						setup_menu();
					}
					break;
				}
			}
		}
		key search = key_get(&buffer);
		if(search == MASTER_KEY){
			beep_long(1);
			wait_key_release();
			setup_menu();
			storage_check_once();
			if(!(PINB & SYS)) beep_long(3);
		}else if(search != NO_KEY){
			wait_key_release();
		}
		wdt_reset();
	}
}

int main(void)
{
	WDTCR = (1<<WDE)|(1<<WDCE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0);
	WDTCR = (1<<WDE)|(1<<WDP2)|(1<<WDP1)|(1<<WDP0);
	wdt_reset();
	init();
	uint8_t cold_service = 0;
	if(!(PINB & SYS)){
		jumper_service();
		cold_service = 1;
	}
	storage_check_start(cold_service);
	if(storage_error){
		red();
	}
	modes mode = NORMAL;
	uint8_t led_timer = 0;
	while (1)
	{
		chunk buffer;
		key search = 0;
		switch(mode){
			case NORMAL:{//дежурный режим
				led_timer++;
				wdt_reset();
				lock_on();
				if(led_timer >= 0x7F){
					if(PORTB & RED)led_off();
					else red();
					led_timer = 0;
				}
				if(!(PINB & SYS)){ mode = JUMPER; break; }
				if(sw_pressed()){
					open();
					break;
				}
				if(storage_error){
					if(!key_read(&buffer) && !key_bad(&buffer)){
						open();
						wait_key_release();
					}
					break;
				}
				if(auto_collect_mode() == AUTO_COLLECT_ON){
					if(!key_read(&buffer) && !key_bad(&buffer)){
						key_add(&buffer);
						open();
						wait_key_release();
					}
					break;
				}
				search = key_get(&buffer);
				switch(search){
					case NO_KEY: break;
					case ERROR:{
						storage_fail(1);
						if(storage_error){
							open();
							wait_key_release();
						}
						break;
					}
					case MASTER_KEY:{
						storage_ok();
						open();
						break;
					}
					case FOUND:{
						storage_ok();
						open();
						break;
					}
					case NOT_FOUND:{
						red();
						deny_beep();
						break;
					}
				}
				break;
			}
			case JUMPER:{//установлена перемычка
				jumper_service();
				mode = NORMAL;
				break;
			}
			default: { mode = NORMAL; break; }
		}
	}
}
