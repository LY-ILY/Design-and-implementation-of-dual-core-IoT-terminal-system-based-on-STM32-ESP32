
#ifndef __CRC__H__
#define __CRC__H__
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef enum {
	my_false=0,
	my_true=1
}Bool;


void generate_crc16_table(uint16_t *table);
uint16_t calculate_crc16(const uint8_t *data, size_t length);
Bool CRC16_check(uint8_t * data,int len);
#endif



