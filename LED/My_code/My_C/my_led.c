#include "my_led.h"
#include "usart.h"

void my_led(void *pvParameters){
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while(1){
				
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
		vTaskDelayUntil(&xLastWakeTime,1000); // DelayUntil for 1000 milliseconds (1 second)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
		vTaskDelayUntil(&xLastWakeTime,1000); // DelayUntil for 1000 milliseconds (1 second)
	}
	
}

