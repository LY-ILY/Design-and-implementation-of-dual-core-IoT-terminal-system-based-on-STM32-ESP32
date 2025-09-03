#ifndef __MYTASK__H__
#define __MYTASK__H__
#include <FreeRTOS.h>
#include "semphr.h"
#include "task.h"
#include "my_adc.h"
#include "my_usart.h"
#include "my_led.h"
#include "my_semaphore.h"
#include "my_control.h"

//封装任务参数的结构体
typedef struct task_config{
	TaskFunction_t TaskCode;          //任务实现函数的指针
	char  Name[16];                   //任务名称字符串
	configSTACK_DEPTH_TYPE StackDepth;//任务栈的大小
	void * Parameters;                //传递给任务函数的参数指针
	UBaseType_t Priority;             //任务的优先级。数值越高，优先级越高，任务将会被优先执行。
	TaskHandle_t *CreatedTask;        //任务句柄的指针。
}Task_Config;

//任务创建函数
BaseType_t my_taskCreate(Task_Config *Task);
//声明任务句柄
extern TaskHandle_t ADC_task_handle;
//声明任务的结构体变量
extern Task_Config LED_task;
extern Task_Config ADC_task;
extern Task_Config UART_send_task;
extern Task_Config UART_recevie;
extern Task_Config UART_receiver_data_process_task;
extern Task_Config Control_task;
extern Task_Config OLED_task;
extern Task_Config Motor_task;
//创建全局变量，用于测试OLED
extern uint32_t OLED_test_num;
//创建add函数，用于测试OLED
void add(void *pvParameters);

// 为二进制信号量分配静态内存


#endif
