#include "my_control.h"

void control_funtion(void *pram){
	Command_frame_to_control_t control_frame_data;
	while(xQueueReceive(Queue_command_to_control, &control_frame_data, portMAX_DELAY) == pdPASS){
		//"jtest/led","jtest/motor"话题id分别为1、2
		uint8_t equipment_id=control_frame_data.frame_data[Command_Frame_Data_Begin_Index];
		uint8_t control_cmd =control_frame_data.frame_data[Command_Frame_Data_Begin_Index+1];
		if(equipment_id==1){//led控制
			if (control_cmd==0){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
			}else if(control_cmd==1){
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
			}else {
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
			}
		}else if(equipment_id==2){//电机控制+
			
		}
		//free(control_frame_data.frame_data);
	}
}
