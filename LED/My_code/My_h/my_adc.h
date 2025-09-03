#ifndef __MYADC__H__
#define __MYADC__H__
#include <FreeRTOS.h>
#include <adc.h>
#include <task.h>
#include <stdint.h>
# include <string.h>
#include <stdio.h>
#include <usart.h>
#include <stdbool.h>
#include "queue.h"
#include "my_semaphore.h"
#include "my_frame.h"
#include "my_task.h"
#define ADC_BUFFER_SIZE  2//ADC����������
#define ADC_BYTE_NUMS  2//ADC���������ݿ�ȣ��ֽ�����
//extern Queue_ADC2UART queue_adc2uart;
extern uint16_t my_adcBuffer[ADC_BUFFER_SIZE];  // [0] ��ͨ�� 1 ��ֵ, [1] ��ͨ�� 2 ��ֵ
//extern float read_vgg[ADC_BUFFER_SIZE];
void my_get_adc(void *pvParameters);

#endif
