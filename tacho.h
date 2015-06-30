/* Khiem
 * Jan 2, 2015
 * project: kineticScreen
 * description: use tacho to est motor speed n moving distance.
 * Method: count the external interrup. timing periodically.
 * 		speed = counts/(counts per turn)/timing
 * 		when change direction, counts will be reset
 * Note:
 * 		consider the counts may overflow. assume maxSp = 1000rpm, 4 counts per turn
 * 		--> maxSpeed = 67 counts per sec --> getTacho need to be called every 2^32/67= very large->safe
 */

#ifndef	_tacho_h
#define _tacho_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include <stdio.h>
#include <string.h>

#define MIN_ACFre		(5000)
#define TACHO_CLK_FRE	(10000)
typedef struct tachoStruct {
	int isConfiged;
	unsigned int counts;
	volatile unsigned short *tachoAdd;
	float speed;
} tachoStruct;

void TachoInit(tachoStruct* me);
void TachoConfig0(tachoStruct* me);
void ResetTachoCounts(tachoStruct* me);
void GetTachoCounts(tachoStruct* me);

void GetTachoFBDistance(tachoStruct* me);
void GetTachoFBSpeed(tachoStruct* me);

#endif
