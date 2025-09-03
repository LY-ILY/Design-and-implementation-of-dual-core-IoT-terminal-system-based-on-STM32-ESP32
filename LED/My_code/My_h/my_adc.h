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
#define ADC_BUFFER_SIZE  2//ADC传感器数量
#define ADC_BYTE_NUMS  2//ADC传感器数据宽度（字节数）
//extern Queue_ADC2UART queue_adc2uart;
extern uint16_t my_adcBuffer[ADC_BUFFER_SIZE];  // [0] 是通道 1 的值, [1] 是通道 2 的值
//extern float read_vgg[ADC_BUFFER_SIZE];
void my_get_adc(void *pvParameters);

#endif
