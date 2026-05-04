/*
 * i2c.c
 *
 * Created: 19.12.2018 10:36:32
 *  Author: GunMan
 */
#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

/*************************************************************************
 Функция ожидания, для ожидания завершения записи.
*************************************************************************/
void __attribute__ ((noinline)) delay_50()
{
	_delay_ms(50);
}

/*************************************************************************
 Функция ожидания, для установки скорости обмена.
*************************************************************************/
void __attribute__ ((noinline)) delay_3()
{
	_delay_us(2);
}

/*************************************************************************	
  Посылает стартовый импульс и занимает шину.
*************************************************************************/
void i2c_start(void)
{
	SDA_UP();
	delay_3();
	SCL_UP();
	delay_3();	
	SDA_DOWN();
	delay_3();
	SCL_DOWN();
	delay_3();
}/* i2c_start */

/*************************************************************************
  Завершает обменн данными о освобождает шину.
*************************************************************************/
void i2c_stop(void)
{
	SDA_DOWN();
	delay_3();
	SCL_UP();
	delay_3();
	SDA_UP();
	delay_3();
}/* i2c_stop */

/*************************************************************************
 Посылает один байт и возвращает ACK.
*************************************************************************/
uint8_t i2c_transmit(uint8_t byte)
{
	for(uint8_t i=0;i<8;i++){
		if (byte & (1<<7)) SDA_UP(); else SDA_DOWN();
		byte = byte<<1;
		SCL_UP();
		delay_3();
		SCL_DOWN();
		delay_3();
	}
	uint8_t ack;
	SCL_UP();
	delay_3();
	if(IS_ACK())ack = 0; else ack = 1;
	SCL_DOWN();
	delay_3();
	return ack;
}/* i2c_transmit */

/*************************************************************************
 Читает один байт в переменную переданную по ссылке и посылает ACK.
*************************************************************************/
void i2c_receive( uint8_t* byte, uint8_t ack)
{
	for(char i=0;i<8;i++){
		SCL_UP();
		delay_3();
		*byte = *byte<<1;
		if(IS_ACK()) *byte &= ~1; else *byte |= 1;
		SCL_DOWN();
		delay_3();
	}
	if(ack) SDA_DOWN();
	SCL_UP();
	delay_3();
	SCL_DOWN();
	SDA_UP();
	delay_3();
}/* i2c_receive */

/*************************************************************************
  Переводит регистр адреса ПЗУ на заданный адрес 24c64
*************************************************************************/
uint8_t i2c_set_address_c64(uint16_t address, uint8_t for_read)
{
	uint8_t ack = 0;
	i2c_start();
	ack |= i2c_transmit(CONTROL & (~1));
	ack |= i2c_transmit(address>>8);
	ack |= i2c_transmit(address);
	if(for_read){
		i2c_start();
		ack |= i2c_transmit(CONTROL | 1);
	}
	return ack;
}/*i2c_set_address_c64*/

/*************************************************************************
  Записывает 4 байта по заданному адресу 24c64
*************************************************************************/
uint8_t i2c_write_c64( uint16_t address, uint8_t* data )
{
	uint8_t error = 0;
	WP_OFF();
	error |= i2c_set_address_c64(address,0);
	for(uint8_t i=0;i<4;i++) error |= i2c_transmit(data[i]);
	i2c_stop();
	delay_50();
	WP_ON();
	return error;
}/* i2c_write_c64 */

/*************************************************************************
  Читает 4 байта по заданному адресу 24c64
*************************************************************************/
uint8_t i2c_read_c64( uint16_t address, uint8_t* data )
{
	uint8_t error = 0;
	error |= i2c_set_address_c64(address,1);
	i2c_receive(data,1);
	i2c_receive(data+1,1);
	i2c_receive(data+2,1);
	i2c_receive(data+3,0);
	i2c_stop();
	return error;
}/* i2c_read_c64 */