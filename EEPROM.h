/*
 * EEPROM.h
 *
 *  Created on: Feb 19, 2025
 *      Author: Avilash.Sawant
 */

#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "stdint.h"
#include "stm32g4xx_hal.h"



// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0

// Define the Page Size and number of pages
#define PAGE_SIZE 128     // in Bytes
#define PAGE_NUM  512    // number of pages

#define PRE_PROG_PAGES		 2

void AL_EEPROM_Page_Write (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void AL_EEPROM_Page_Read (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void AL_EEPROM_PageErase (uint16_t page);
uint16_t AL_bytestowrite (uint16_t size, uint16_t offset);
void AL_EEPROM_Write_NUM (uint16_t page, uint16_t offset, float  fdata);
float AL_EEPROM_Read_NUM (uint16_t page, uint16_t offset);
void AL_EEPROM_Soft_Reset(void);
void AL_float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable);
float AL_Bytes2float(uint8_t * ftoa_bytes_temp);


#endif

/* _EEPROM_H_ */
