#include "TTacho.h"

static tachoStruct myTacho;

void TTachoConfig(void) {
	SystemTimerInit(1000000);
	UartConfig();
	TachoInit(&myTacho);
	TachoConfig0(&myTacho);
}

void TTachoFBSpeed(void) {
	static unsigned long lastTempTimer = 0;
	unsigned long tempTimer = GetSystemTimer();
	unsigned long tempTimer1;

	GetTachoFBSpeed(&myTacho);
	tempTimer1 = tempTimer - lastTempTimer;
	if (tempTimer1 > 100000) {
		myPrintf3("speed:%.1f\r\n", myTacho.speed);
	}
}

void TTachoFBCounts(void) {
	static unsigned long lastTempTimer = 0;
	unsigned long tempTimer = GetSystemTimer();
	unsigned long tempTimer1;

	GetTachoCounts(&myTacho);
	tempTimer1 = tempTimer - lastTempTimer;
	if (tempTimer1 > 100000) {
		myPrintf3("counts:%u\r\n", myTacho.counts);
	}
}

