/**
 ******************************************************************************
 * @file    SysTick_metoder.c
 * @author  Sivert Sliper and Stian Sørensen
 * @version V1.0
 * @date    08-February-2016
 * @brief   This file contains all the functions prototypes for the SysTick
 *          timer.
 *
 ******************************************************************************
 */

/* Include---- ------------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "stm32f30x_gpio.h"
#include "core_cm4.h"
#include "rov2016_canbus.h"
#include "rov2016_UART.h"
#include "rov2016_VESC.h"
/* Global variables --------------------------------------------------------------------*/
#include "extern_decl_global_vars.h"

/* Private variables -------------------------------------------------------------------*/
static uint8_t kjor = 0, timestamp=0;
static float rpm_counter = 0.3f;
static float dir = 1.0;
/* Private function declarations ---------------------------------------------------------------*/

/* Function definitions ----------------------------------------------------------------*/

/**
 * @brief  Configures the SysTick timer for 100 Hz interrupt frequency.
 * @param  None
 * @retval None
 */
void SysTick_init(void) {
	NVIC_SetPriority(SysTick_IRQn, 1);
	SysTick->CTRL = 0; /* Disable SysTick */
	SysTick->LOAD = 72000000/1000;  // 1000 Hz interrupt frequency.
	SysTick->VAL = 0;
	SysTick->CTRL = (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk
			| SysTick_CTRL_CLKSOURCE_Msk);
} // end Systick_init()


/**
 * @brief  Configures the CAN-Controller peripheral for 500 kbps communication.
 * 		   Also configures Rx filters according to ID's specified in "can_metoder.h"
 * @param  None
 * @retval None
 */

uint16_t teller = 0;
uint16_t alivecount = 0;
uint16_t val = 0;
uint32_t valVoltage = 0;
uint8_t timeStamp = 0;

void SysTick_Handler(void){
	teller++;

	/* Check for USART messages, start if 'k' */
		if (USART_getNewBytes()>0){
			uint8_t melding = USART_getRxMessage();
			if (melding == 'k') {
				kjor = 1;
				VESC_setDutyCycle(ESC_ID_1, 0.0f);
			}
			if (melding == 's'){
				kjor = 0;
				VESC_setDutyCycle(ESC_ID_1, 0.0f);
			}
			if (melding == 'd'){
				dir = -dir;

				volatile uint32_t i = 360000;
				while(i-->0);

				VESC_setDutyCycle(ESC_ID_1, VESC_DUTY_CYCLE_DIR_CHANGE);
				i = 360000;
				while(i-->0);

				VESC_setDutyCycle(ESC_ID_1, 0.0f);
				i = 360000;
				while(i-->0);

				VESC_setDutyCycle(ESC_ID_1, VESC_DUTY_CYCLE_DIR_CHANGE);
				i = 360000;
				while(i-->0);
			}
		}

	if((teller>100) && kjor){
		GPIOE->ODR ^= SYSTICK_LED << 8; // Update status LED.

		if(rpm_counter <= 0.9){
			rpm_counter += 0.01;
		} else rpm_counter = 0.3;

		VESC_setDutyCycle(ESC_ID_1, rpm_counter);

		teller = 0;
	} // end if

} // end Systick_Handler()
