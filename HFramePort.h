/* Khiem
 * Jan 5, 2015
 * project: Kinetic screen
 * description: define the driver for Hframe
 * 		adc pin: pa5
 */

#ifndef _HFramePort_h
#define _HFramePort_h
#include "STM32F4-Discovery/stm32f4_discovery.h"

void HframePortConfig0(void);

/**
  * @brief  cmd HFrame mov with a specified speed.
  * 	the speed is normalized (-1000;1000)
  * @param  setSpeed
  * @retval None
  */
void MoveAtSpeed(int setSpeed);

#endif
