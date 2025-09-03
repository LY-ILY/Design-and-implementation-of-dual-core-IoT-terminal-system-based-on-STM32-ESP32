#ifndef __TESK__H__
#define __TESK__H__
// 首先包含标准库头文件
#include <stdint.h>
#include <stdarg.h>  // 用于va_list

#include "freertos/FreeRTOS.h"//配置参数必须在任务头文件之前
#include "freertos/task.h"
#include "esp_log.h"
#include "my_uart.h"
#include "my_mqtt.h"
//配置任务的结构体
typedef struct 
{
    TaskFunction_t TaskCode;    //任务实现函数的指针
    char name[32];              //任务名称字符串
    uint16_t stack_size;         //任务栈的大小
    void *parameter;            //传递给任务函数的参数指针
    uint8_t priority;           //任务的优先级。数值越高，优先级越高，任务将会被优先执行。
    TaskHandle_t *Task_Handle;   //任务句柄的指针。
}Task_Config;
/*
实时操作系统需要完成的功能如下：
1、串口接收STM32数据
2、向MQTT发送来自STM32的数据
1和2可以使用信号量实现同步，也可以直接按照顺序逻辑编写代码
——————————————————————————————————————————————————————
3、接收来自MQTT的数据
4、使用串口向STM32发送数据
3和4可以使用信号量实现同步，也可以直接按照顺序逻辑编写代码
*/
extern Task_Config Uart_Receive;
extern Task_Config Uart_Send;
extern Task_Config MQTT_Send;
extern Task_Config Uart_Receive_Frame_Process;
void Create_Task(Task_Config my_task);
#endif
