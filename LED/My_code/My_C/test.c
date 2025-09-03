#include <test.h>
#include <stdio.h>
#include <my_oled.h>
#include <string.h>
#include "my_task.h"
#include "queue.h"
#include "motor.h"
#include "my_tim.h"
#include "usart.h"
void my_main(void){ 
	//初始化
	//OLED初始化
	OLED_Init();
	//启动定时器
	My_Tim_Start();
	
	//创建LED闪烁任务
	//my_taskCreate(&LED_task);
	//创建OLED显示任务任务
	//my_taskCreate(&OLED_task);
	//启动串口接收服务
	Start_UART_DMA_Receive();
	
	//Init_Motor();
	//信号量初始化
	SemaphoresInit();
	
	//创建任务
	
	//创建ADC读取任务
	my_taskCreate(&ADC_task);
	
	//创建串口发送任务
	my_taskCreate(&UART_send_task);
	
	//创建串口接收数据处理任务
	my_taskCreate(&UART_receiver_data_process_task);
	//
	my_taskCreate(&Control_task);
	
	
	//创建电动机任务
	//my_taskCreate(&Motor_task);
	//char *str = "Hello, UART!1\r\n";
	//HAL_UART_Transmit(&huart1, (uint8_t *)str, 15, HAL_MAX_DELAY);
	
}

