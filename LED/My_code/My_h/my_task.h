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

//��װ��������Ľṹ��
typedef struct task_config{
	TaskFunction_t TaskCode;          //����ʵ�ֺ�����ָ��
	char  Name[16];                   //���������ַ���
	configSTACK_DEPTH_TYPE StackDepth;//����ջ�Ĵ�С
	void * Parameters;                //���ݸ��������Ĳ���ָ��
	UBaseType_t Priority;             //��������ȼ�����ֵԽ�ߣ����ȼ�Խ�ߣ����񽫻ᱻ����ִ�С�
	TaskHandle_t *CreatedTask;        //��������ָ�롣
}Task_Config;

//���񴴽�����
BaseType_t my_taskCreate(Task_Config *Task);
//����������
extern TaskHandle_t ADC_task_handle;
//��������Ľṹ�����
extern Task_Config LED_task;
extern Task_Config ADC_task;
extern Task_Config UART_send_task;
extern Task_Config UART_recevie;
extern Task_Config UART_receiver_data_process_task;
extern Task_Config Control_task;
extern Task_Config OLED_task;
extern Task_Config Motor_task;
//����ȫ�ֱ��������ڲ���OLED
extern uint32_t OLED_test_num;
//����add���������ڲ���OLED
void add(void *pvParameters);

// Ϊ�������ź������侲̬�ڴ�


#endif
