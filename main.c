#include "STM32F4-Discovery/stm32f4_discovery.h"
//#include "TTacho.h"
#include "THFramePort.h"

int main(void) {
	THFramePortConfig();

	while (1) {
		THFramePortMoveAtSpeed();
	}
}

