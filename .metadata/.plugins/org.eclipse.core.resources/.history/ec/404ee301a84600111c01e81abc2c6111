#include "main.h"
#include "bl_jump.h"
#include "flash_layout.h"

typedef void (*pFunction)(void);

void jumpToApplication(void) {
	uint32_t appStack;
	uint32_t appResetHandler;
	pFunction appEntry;

	/* Read application stack pointer */
	appStack = *(volatile uint32_t*)APP_START_ADDR;

	/* Read reset handler address */
	appResetHandler = *(volatile uint32_t*)(APP_START_ADDR + 4);
	appEntry = (pFunction)appResetHandler;

	__disable_irq();

	/* Stop SysTick */
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	/* Set main stack pointer */
	__set_MSP(appStack);

	/* Jump to app reset handler */
	appEntry();
}
