#ifndef __MY__SEMAPHORE__
#define __MY__SEMAPHORE__
#include <FreeRTOS.h>
#include "semphr.h"
#include "queue.h"
#include "timers.h"
//为信号设置阻塞时间，单位毫秒
#define SEM_WAIT_TIME pdMS_TO_TICKS(1000) 

//创建锁
extern StaticSemaphore_t xMutexBuffer;
extern SemaphoreHandle_t xFrameIdMutex;


/*创建信号量，和消息队列*/
//ADC读取信号量，由软件定时器发出
extern SemaphoreHandle_t ADC_Read;

//UART接收成功信号量，由UART的Idle中断的回调函数发出
extern SemaphoreHandle_t UART_receive;
/*向Uart发送任务，使用一个消息队列即可*/

//ADC向UART发送信息的消息队列的长度
#define QUEUE_frame_to_uart_len 10
//消息队列句柄
extern QueueHandle_t Queue_frame_to_uart;
//ADC读取好的数据传递给UART发送任务的消息队列，及其消息队列对应的结构体
typedef struct{
	uint8_t frame_data[16];
	uint8_t data_len;
}Frame_to_uart_t;




//UART接收数据后发向数据处理任务
#define QUEUE_cmd_frame_process_len 5
//消息队列句柄
extern QueueHandle_t Queue_cmd_frame_process;
//UART接收数据后发向数据处理任务的结构体
typedef struct{
	uint8_t frame_data[16];
	uint8_t data_len;
}Cmd_frame_process_t;



//UART接收到的命令帧向控制任务发送信息的消息队列的长度
#define QUEUE_command_to_control_len 5
//消息队列句柄
extern QueueHandle_t Queue_command_to_control;
//UART接收到命令帧后，向控制任务发送信息的消息队列，及其消息队列对应的结构体
typedef struct{
	uint8_t frame_data[16];
	uint8_t data_len;
}Command_frame_to_control_t;

void SemaphoresInit(void);

#endif

