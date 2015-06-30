#ifndef UTILITY_H
#define UTILITY_H

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Uart.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define WRITE_REGISTER(var) 	__asm volatile("mov " TOSTRING(var) ", %[" TOSTRING(var) "]" "\n\t" : : [var] "r" (var))
#define READ_REGISTER(var) 		__asm volatile("mov %[" TOSTRING(var) "], " TOSTRING(var) "\n\t" : [var] "=r" (var))

char* itoa(int32_t value, char* result, int base);

void checksum32bit();

void assert_failed(uint8_t* file, uint32_t line);

int abs(int value);

void simpleDelay(int count);

float LimitToRange(float input, float min, float max);
s16 BitConverterToInt16(uint8_t * byte, int index);
s32 BitConverterToInt32(uint8_t * byte, int index);
float BitConverterToFloat(uint8_t * byte, int index);
#endif
