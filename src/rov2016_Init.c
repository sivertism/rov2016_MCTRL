/*
 * File: init.c
 *
 * Brief: Contains function(s) for calling initialization functions
 */

/* Include -------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "rov2016_canbus.h"
#include "rov2016_UART.h"
#include "rov2016_TIM.h"
#include "rov2016_SysTick.h"
#include "rov2016_GPIO.h"
/* Funtion Prototypes --------------------------------------------------------*/
extern void init(void);

/* Funtion Definitions -------------------------------------------------------*/

extern void init(void){
	GPIO_init();
	USART2_init();
	CAN_Config();
	SysTick_init();
}
