# include<motor.h>

void Init_Motor(void){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
}


void Motor_On(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

void Motor_Off(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

void Motor_PWM_SET(uint8_t speed){
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
}

void Motor_Task(void *pvParameters){
	while(1){
		Motor_On();
		Motor_PWM_SET(50);
		HAL_Delay(5*1000);
		Motor_Off();
	}
	
}
