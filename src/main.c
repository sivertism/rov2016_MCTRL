/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_can.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
#include "comm_uart.h"
#include "bldc_interface.h"
/* GLOBAL VARIABLES ----------------------------------------------------------*/
#include "def_global_vars.h"

/* Funtion Prototypes --------------------------------------------------------*/
void init(void);

/* Funtion Definitions -------------------------------------------------------*/
void bldc_val_received(mc_values *val) {
	printf("\r\n");
	printf("Input voltage: %.2f V\r\n", val->v_in);
	printf("Temp:          %.2f degC\r\n", val->temp_pcb);
	printf("Current motor: %.2f A\r\n", val->current_motor);
	printf("Current in:    %.2f A\r\n", val->current_in);
	printf("RPM:           %.1f RPM\r\n", val->rpm);
	printf("Duty cycle:    %.1f %%\r\n", val->duty_now * 100.0);
	printf("Ah Drawn:      %.4f Ah\r\n", val->amp_hours);
	printf("Ah Regen:      %.4f Ah\r\n", val->amp_hours_charged);
	printf("Wh Drawn:      %.4f Wh\r\n", val->watt_hours);
	printf("Wh Regen:      %.4f Wh\r\n", val->watt_hours_charged);
	printf("Tacho:         %i counts\r\n", val->tachometer);
	printf("Tacho ABS:     %i counts\r\n", val->tachometer_abs);
	printf("Fault Code:    %s\r\n", bldc_interface_fault_to_string(val->fault_code));
}

int main(void){
	/* Initialization *********************************************************/
	init();
	comm_uart_init();
	GPIOE->ODR = 0; // Turn off LED's
	// Give bldc_interface a function to call when valus are received.
	bldc_interface_set_rx_value_func(bldc_val_received);
	/* Private vars ***********************************************************/
	printf("Init complete\n");

	/* Main loop *************************************************************/
	while(1){
		GPIOE->ODR ^= MAIN_LOOP_LED << 8;
	} // end while
} // end main





