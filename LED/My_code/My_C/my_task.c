#include "my_task.h"


BaseType_t my_taskCreate(Task_Config *Task){
	if (Task == NULL || Task->TaskCode == NULL) {
        return pdFAIL;
    }
    return xTaskCreate(Task->TaskCode,
                       Task->Name,
                       Task->StackDepth,
                       Task->Parameters,
                       Task->Priority,
                       Task->CreatedTask); 
}
//LED任务
TaskHandle_t LED_task_handle;
Task_Config LED_task={
	.TaskCode=my_led,
	.Name="LED",
	.StackDepth=32,
	.Parameters=NULL,
	.Priority=1,
	.CreatedTask = &LED_task_handle
};
//ADC任务
TaskHandle_t ADC_task_handle= NULL;
Task_Config ADC_task={
	.TaskCode=my_get_adc,
	.Name="ADC",
	.StackDepth=256,
	.Parameters=NULL,
	.Priority=3,
	.CreatedTask = &ADC_task_handle
};

//发送任务  没有使用发送任务
TaskHandle_t UART_send_task_handle;
Task_Config UART_send_task={
	.TaskCode=uart_transmit,
	.Name="UART_send",
	.StackDepth=256,
	.Parameters=NULL,
	.Priority=3,
	.CreatedTask = &UART_send_task_handle
};

//数据处理任务
TaskHandle_t UART_receiver_data_process_handle;
Task_Config UART_receiver_data_process_task={
	.TaskCode=uart_receiver_data_process,
	.Name="UART_data_pro",
	.StackDepth=512,
	.Parameters=NULL,
	.Priority=3,
	.CreatedTask = &UART_receiver_data_process_handle
};

//控制任务
TaskHandle_t Control_handle;
Task_Config Control_task={
	.TaskCode=control_funtion,
	.Name="Control_task",
	.StackDepth=128,
	.Parameters=NULL,
	.Priority=3,
	.CreatedTask = &Control_handle
};



//OLED任务
TaskHandle_t OLED_task_handle;
Task_Config OLED_task={
	.TaskCode=add,
	.Name="OLED",
	.StackDepth=128,
	.Parameters=NULL,
	.Priority=2,
	.CreatedTask = &OLED_task_handle
};
/*
TaskHandle_t Motor_task_handle;
Task_Config Motor_task={
	.TaskCode=NULL,
	.Name="Motor_Task",
	.StackDepth=0,
	.Parameters=NULL,
	.Priority=2,
	.CreatedTask = &Motor_task_handle
};
*/
//Motor任务

//使静态变量作为计数器
static uint32_t OLED_test_num=0;
void add(void *pvParameters){
	while(1){
		
		OLED_ShowNum(1,1,OLED_test_num,5,OLED_8X16);OLED_Update();
		//OLED_ShowNum(1,1,read_vgg[0]/1,5,OLED_8X16);OLED_Update();
		OLED_test_num++ ;
		vTaskDelay(1000);
	}
}

/*
LED_task.TaskCode=my_led;
LED_task.Name="LED";
LED_task.StackDepth=8;
LED_task.Parameters=NULL;
LED_task.Priority=1;
*/
