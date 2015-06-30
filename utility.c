#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "utility.h"

static __IO uint32_t TM_TimingDelay = 0;
static __IO uint32_t TM_Time = 0, TM_Time2 = 0;
uint8_t TM_DELAY_Initialized = 0;

static inline int min(int a, int b);
static inline int max(int a, int b);
static inline float fpow(float f, int n);

void HardFault_Handler(void) {
	__asm volatile
	(
			" tst lr, #4                                                \n"
			" ite eq                                                    \n"
			" mrseq r0, msp                                             \n"
			" mrsne r0, psp                                             \n"
			" B hard_fault_handler_c                                    \n"
	);
}

void hard_fault_handler_c(unsigned int * hardfault_args) {
	unsigned int stacked_r0;
	unsigned int stacked_r1;
	unsigned int stacked_r2;
	unsigned int stacked_r3;
	unsigned int stacked_r12;
	unsigned int stacked_lr;
	unsigned int stacked_pc;
	unsigned int stacked_psr;

	stacked_r0 = ((unsigned long) hardfault_args[0]);
	stacked_r1 = ((unsigned long) hardfault_args[1]);
	stacked_r2 = ((unsigned long) hardfault_args[2]);
	stacked_r3 = ((unsigned long) hardfault_args[3]);

	stacked_r12 = ((unsigned long) hardfault_args[4]);
	stacked_lr = ((unsigned long) hardfault_args[5]);
	stacked_pc = ((unsigned long) hardfault_args[6]);
	stacked_psr = ((unsigned long) hardfault_args[7]);

	myPrintf3("\n\n[Hard fault handler - all numbers in hex]\n");

	myPrintf3("R0 = %x\n", (unsigned int) stacked_r0);
	myPrintf3("R1 = %x\n", (unsigned int) stacked_r1);
	myPrintf3("R2 = %x\n", (unsigned int) stacked_r2);
	myPrintf3("R3 = %x\n", (unsigned int) stacked_r3);
	myPrintf3("R12 = %x\n", (unsigned int) stacked_r12);
	myPrintf3("LR [R14] = %x  subroutine call return address\n",
			(unsigned int) stacked_lr);
	myPrintf3("PC [R15] = %x  program counter\n", (unsigned int) stacked_pc);
	myPrintf3("PSR = %x\n", stacked_psr);
	myPrintf3("BFAR = %x\n",
			(unsigned int) (*((volatile unsigned long *) (0xE000ED38))));
	myPrintf3("CFSR = %x\n",
			(unsigned int) (*((volatile unsigned long *) (0xE000ED28))));
	myPrintf3("HFSR = %x\n",
			(unsigned int) (*((volatile unsigned long *) (0xE000ED2C))));
	myPrintf3("DFSR = %x\n",
			(unsigned int) (*((volatile unsigned long *) (0xE000ED30))));
	myPrintf3("AFSR = %x\n",
			(unsigned int) (*((volatile unsigned long *) (0xE000ED3C))));
	myPrintf3("SCB_SHCSR = %x\n", (unsigned int) SCB->SHCSR);

	while (1)
		;
}

char* itoa(int32_t value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ =
				"zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35
						+ (tmp_value - value * base)];
	} while (value);

	// Apply negative sign
	if (tmp_value < 0) {
		*ptr++ = '-';
	}
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

int abs(int value) {
	if (value < 0) {
		return -value;
	} else {
		return value;
	}
}

void simpleDelay(int count) {
	while (count--)
		;
}

float LimitToRange(float input, float min, float max) {
	if (input > max)
		input = max;
	if (input < min)
		input = min;
	return input;
}

s16 BitConverterToInt16(uint8_t * byte, int index) {
	s16 ret = ((s16) byte[index]) + ((s16) byte[index + 1] << 8);
	return ret;
}
s32 BitConverterToInt32(uint8_t * byte, int index) {
	s32 ret = (((s32) byte[index]) + ((s32) byte[index + 1] << 8)
			+ ((s32) byte[index + 2] << 16) + ((s32) byte[index + 3] << 24));
	return ret;
}
float BitConverterToFloat(uint8_t * byte, int index) {
	float ret = (float) (((u32) byte[index]) + ((u32) byte[index + 1] << 8)
			+ ((u32) byte[index + 1] << 16) + ((u32) byte[index + 1] << 24));
	return ret;
}

static inline int min(int a, int b) {
	return a < b ? a : b;
}
static inline int max(int a, int b) {
	return a < b ? b : a;
}
static inline float fpow(float f, int n) {
	float rv = 1;
	while (n-- > 0) {
		rv *= f;
	}
	return rv;
}
