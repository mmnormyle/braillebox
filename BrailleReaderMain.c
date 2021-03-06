#include <stdio.h>
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Switches.h"
#include "Solenoids.h"
#include "BrailleConverter.h"
#include "PLL.h"
#include "Sound.h"
#include "Sound.h"
#include "SysTick.h"
#include "Communication.h"
#include "MessageFIFO.h"
#include "Graphics.h"

extern unsigned char printing;

void DisableInterrupts(void);
void EnableInterrupts(void);

void printTopMessage(void) {
	if(!printing) {
		printing = 1;
		BrailleString message;
		int val = messageFifoGet(&message);
		if(val) {
			printBraille(message);
		}
	}
}

int main(void){ 
  DisableInterrupts();
	solenoidInit();
	PLL_Init();
	SysTickWaitInit();
	communicationInit();
	ST7735_InitR(INITR_REDTAB);
	ST7735_SetTextColor(0x07E0);
	switchInit(&printTopMessage);
	messageFifoInit();
	EnableInterrupts();
	setSpeed(100);
	
	while(1) {
		graphicsMainScreen();
		BrailleString string = getUARTString();
		messageFifoPut(string);
		graphicsMainScreen();
		playAlarm(100);
		printTopMessage();
		while(printing){}
	}
}

