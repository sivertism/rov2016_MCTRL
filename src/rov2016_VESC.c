/**
  ******************************************************************************
  * @file    rov2016_VESC.c
  * @author  Sivert Sliper, Stian G. Sørensen
  * @version V1.0
  * @date    3-February-2016
  * @brief 	This file provides functionality for controlling multiple VESC
  * 		BLDC controllers over CAN-bus.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "rov2016_VESC.h"
#include "rov2016_canbus.h"

/* Private variables ---------------------------------------------------------*/
typedef enum {
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,
	CAN_PACKET_SET_CURRENT_BRAKE,
	CAN_PACKET_SET_RPM,
	CAN_PACKET_SET_POS,
	CAN_PACKET_FILL_RX_BUFFER,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,
	CAN_PACKET_STATUS
} CAN_PACKET_ID;

/* Private function prototypes -----------------------------------------------*/

/* Function declarations -----------------------------------------------------*/

/**
 * @brief  	Sets the duty cycle of the specified VESC BLDC controller.
 * @param  	esc_id:	Can be a value of ESC_ID_x where x can be 1-8.
 * 			duty:	Duty cycle, can be a value between DUTY_CYCLE_MIN and
 * 					DUTY_CYCLE_MAX.
 * @retval 	None
 */
void VESC_setDutyCycle(uint8_t esc_id, float duty){
	/* Regn ut ID
	 * skaler float x 10 000
	 * legg float inn i 4 byte array
	 * send med metoden can_transmitbuffer.
	 */

	/* Check parameters. */
	if (duty < VESC_DUTY_CYCLE_MIN) return;
	if (duty > VESC_DUTY_CYCLE_MAX) return;
	if(esc_id > 255) return;

	uint32_t id = (uint32_t)(CAN_PACKET_SET_DUTY << 8) | esc_id;

	uint32_t temp_duty = (uint32_t)(duty * 10000.0f);

	uint8_t buffer[4];
	buffer[0] = temp_duty >> 24;	// MSB(Most significant byte).
	buffer[1] = temp_duty >> 16;
	buffer[2] = temp_duty >> 8;
	buffer[3] = temp_duty;			// LSB(Least significant byte).

	CAN_transmitBuffer(id, buffer, 4, CAN_ID_TYPE_EXT);

#ifdef DEBUG_MODE
	printf("Setting duty cycle of ESC %d to %.2f...\n", esc_id, duty);
#endif
}
