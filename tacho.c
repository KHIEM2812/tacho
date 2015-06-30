#include "tacho.h"

static __IO uint16_t IC1Value = 0, latchIC1Value = 0;
static __IO unsigned int tachoEdgeCounts = 0;

void TachoInit(tachoStruct* me) {
	memset(me, 0, sizeof(tachoStruct));
}

void Timer9PWMInputCaptureConfig(void) {
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM9);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_Prescaler = (uint16_t)((SystemCoreClock) / 10000)
			- 1;
	TIM_TimeBaseInitStruct.TIM_Period = 0xffffffff;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseInitStruct);

	/*
	 * channel2 ll be interrupted by rising edge n reset by another rising edge
	 */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV2;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_PWMIConfig(TIM9, &TIM_ICInitStructure);

	TIM_SelectInputTrigger(TIM9, TIM_TS_TI2FP2);
	TIM_SelectSlaveMode(TIM9, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM9, TIM_MasterSlaveMode_Enable);

	TIM_ITConfig(TIM9, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM9, TIM_IT_CC1, ENABLE);
	TIM_Cmd(TIM9, ENABLE);

}

void TachoConfig0(tachoStruct* me) {
	if (me->isConfiged == 0) {
		Timer9PWMInputCaptureConfig();
		me->tachoAdd = (volatile unsigned short *) &latchIC1Value;
		me->isConfiged = 1;
	}
}

void ResetTachoCounts(tachoStruct* me) {
	me->counts = 0;
}
void GetTachoCounts(tachoStruct* me) {
	me->counts = tachoEdgeCounts;
}

void GetTachoFBSpeed(tachoStruct* me) {
	unsigned short tempIC1 = *me->tachoAdd;
	me->speed = TACHO_CLK_FRE * 1.0 / (2 * tempIC1);
	if (tempIC1 > MIN_ACFre || tempIC1 == 0) {
		me->speed = 0;
	}
}

void TIM1_BRK_TIM9_IRQHandler(void) {
	if (TIM_GetITStatus(TIM9, TIM_IT_CC2) == SET) {
		IC1Value = TIM_GetCapture2(TIM9);
		if (IC1Value != 0) {
			latchIC1Value = IC1Value;
		}
		TIM_ClearITPendingBit(TIM9, TIM_IT_CC2);
	}

	tachoEdgeCounts++;
	TIM_ClearITPendingBit(TIM9, TIM_IT_CC1);
}
