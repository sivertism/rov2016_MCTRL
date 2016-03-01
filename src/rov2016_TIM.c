/**
  **************************************************************************************
  * @file    TIM_metoder.c
  * @author  Sivert Sliper, Stian Soerensen
  * @version V1.0
  * @date    10-February-2016
  * @brief   This file contains all functions used for the TIM peripheral.
  **************************************************************************************
  */

/* Include------------------------------------------------------------------------------*/
#include "stm32f30x.h"
#include <stm32f30x_rcc.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_tim.h>
#include "rov2016_TIM.h"

/* Global variables --------------------------------------------------------------------*/

/* Local variables ---------------------------------------------------------------------*/

/* Private function declarations -------------------------------------------------------*/


/* Function definitions ----------------------------------------------------------------*/

/**
 * @brief  	Initialize TIM4 as a 1 kHz clock signal for the analog filter IC.
 * 			This sets the cut-off frequency throught the filter to
 * 			1000/100 = 10 Hz.
 * @param  	None
 * @retval 	None
 */
void TIM4_init(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Clock access */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* GPIOD settings */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	/* Configure PD12 as alternative function TIM4 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_2);

	/* TIM4 settings */
	TIM_TimeBaseStructure.TIM_Period = 1000000/1000; // 1 kHz
	TIM_TimeBaseStructure.TIM_Prescaler = 72;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* TIM_OC settings */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM modus
	/* Activate comparison with value located in Capture Compare Register */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = (1000000/1000)/2; // 50% duty cycle.
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);

	TIM_Cmd(TIM4, ENABLE);
} // end TIM4_init()

/**
 * @brief  	Initialize TIM2 as a trigger for ADC 1 and 4.
 * @param  	None
 * @retval 	None
 */
void TIM2_init(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Time base settings ***********************************************/
	TIM_TimeBaseStructure.TIM_Period = (1000000/100) - 1; //50 Hz
	TIM_TimeBaseStructure.TIM_Prescaler = 72; // Clocks TIM2 at 1 MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Activate TIM2_TRGO for triggering on each  TIM2 update event. */
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
	TIM_Cmd(TIM2, ENABLE);
}// end TIM2_init()
