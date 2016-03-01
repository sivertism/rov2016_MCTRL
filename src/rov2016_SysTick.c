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
#include "bldc_interface.h"
/* Global variables --------------------------------------------------------------------*/
#include "extern_decl_global_vars.h"

/* Private variables -------------------------------------------------------------------*/
static uint8_t kjor = 0, timestamp=0;
static uint16_t rpm_counter = 600;

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
uint16_t val = 0;
uint32_t valVoltage = 0;
uint8_t timeStamp = 0;

void SysTick_Handler(void){
	/* BLDC timing function, needs to be called every 1 ms. */
	bldc_interface_uart_run_timer();
	teller++;

	/* Check for new message on CAN and update LEDs */
	if(CAN_getRxMessages()>0){
		GPIOE->ODR ^= (1u << CAN_getByteFromMessage(2,0)) << 8;
	} // end if

	/* Check for USART messages, start if 'k' */
		if (USART_getNewBytes()>0){
			uint8_t melding = USART_getRxMessage();
			if (melding == 'k') {
				kjor = 1;
				USART_transmit(USART2, 0x02); // STX
			}
			if (melding == 's'){
				kjor = 0;
				USART_transmit(USART2, 0x03); //ETX
			}
		}

	if((teller>1000) && kjor){
		printf("Attempting to set rpm...\n");
		GPIOE->ODR ^= SYSTICK_LED << 8;
		bldc_interface_set_rpm(rpm_counter);
		if(rpm_counter <= 4000){
		rpm_counter += 10;
		} else rpm_counter = 600;

		teller = 0;
	} // end if

} // end Systick_Handler()
