#ifndef __MY__SEMAPHORE__
#define __MY__SEMAPHORE__
#include <FreeRTOS.h>
#include "semphr.h"
#include "queue.h"
#include "timers.h"
//Ϊ�ź���������ʱ�䣬��λ����
#define SEM_WAIT_TIME pdMS_TO_TICKS(1000) 

//������
extern StaticSemaphore_t xMutexBuffer;
extern SemaphoreHandle_t xFrameIdMutex;


/*�����ź���������Ϣ����*/
//ADC��ȡ�ź������������ʱ������
extern SemaphoreHandle_t ADC_Read;

//UART���ճɹ��ź�������UART��Idle�жϵĻص���������
extern SemaphoreHandle_t UART_receive;
/*��Uart��������ʹ��һ����Ϣ���м���*/

//ADC��UART������Ϣ����Ϣ���еĳ���
#define QUEUE_frame_to_uart_len 10
//��Ϣ���о��
extern QueueHandle_t Queue_frame_to_uart;
//ADC��ȡ�õ����ݴ��ݸ�UART�����������Ϣ���У�������Ϣ���ж�Ӧ�Ľṹ��
typedef struct{
	uint8_t frame_data[16];
	uint8_t data_len;
}Frame_to_uart_t;




//UART�������ݺ������ݴ�������
#define QUEUE_cmd_frame_process_len 5
//��Ϣ���о��
extern QueueHandle_t Queue_cmd_frame_process;
//UART�������ݺ������ݴ�������Ľṹ��
typedef struct{
	uint8_t frame_data[16];
	uint8_t data_len;
}Cmd_frame_process_t;



//UART���յ�������֡�������������Ϣ����Ϣ���еĳ���
#define QUEUE_command_to_control_len 5
//��Ϣ���о��
extern QueueHandle_t Queue_command_to_control;
//UART���յ�����֡���������������Ϣ����Ϣ���У�������Ϣ���ж�Ӧ�Ľṹ��
typedef struct{
	uint8_t frame_data[16];
	uint8_t data_len;
}Command_frame_to_control_t;

void SemaphoresInit(void);

#endif

