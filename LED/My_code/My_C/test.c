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
	//��ʼ��
	//OLED��ʼ��
	OLED_Init();
	//������ʱ��
	My_Tim_Start();
	
	//����LED��˸����
	//my_taskCreate(&LED_task);
	//����OLED��ʾ��������
	//my_taskCreate(&OLED_task);
	//�������ڽ��շ���
	Start_UART_DMA_Receive();
	
	//Init_Motor();
	//�ź�����ʼ��
	SemaphoresInit();
	
	//��������
	
	//����ADC��ȡ����
	my_taskCreate(&ADC_task);
	
	//�������ڷ�������
	my_taskCreate(&UART_send_task);
	
	//�������ڽ������ݴ�������
	my_taskCreate(&UART_receiver_data_process_task);
	//
	my_taskCreate(&Control_task);
	
	
	//�����綯������
	//my_taskCreate(&Motor_task);
	//char *str = "Hello, UART!1\r\n";
	//HAL_UART_Transmit(&huart1, (uint8_t *)str, 15, HAL_MAX_DELAY);
	
}

