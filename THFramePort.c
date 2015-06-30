#include "THFramePort.h"

static tachoStruct myTacho;

void THFramePortConfig(void) {
	SystemTimerInit(1000000);
	UartConfig();
	HframePortConfig0();
	TachoInit(&myTacho);
	TachoConfig0(&myTacho);

}

void THFramePortMoveAtSpeed(void) {
	static unsigned long lastTempTimer = 0;
	static int mySpeed = 0;
	unsigned long tempTimer = GetSystemTimer();
	unsigned long tempTimer1;

	GetTachoFBSpeed(&myTacho);
	tempTimer1 = tempTimer - lastTempTimer;
	if (tempTimer1 > 200000) {
		myPrintf3("speed:%.1f\r\n", myTacho.speed);
		if(mySpeed>=1000){
			mySpeed = -1000;
		}
		mySpeed += 200;
		MoveAtSpeed(mySpeed);
	}


}
