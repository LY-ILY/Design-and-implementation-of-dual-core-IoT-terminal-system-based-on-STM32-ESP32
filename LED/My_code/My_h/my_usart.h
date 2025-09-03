#ifndef __MYUSART__H__
#define __MYUSART__H__
#include <FreeRTOS.h>
#include <usart.h>
#include <stdint.h>
#include "my_oled.h"
#include <string.h>
#include <stdio.h>
#include <task.h>
#include <stdlib.h>
#include "my_semaphore.h"
#include "my_adc.h"
#include "event_groups.h" 
#include "my_crc.h"
#include "my_frame.h"
#include "stdbool.h"
//ADC发送同步信号量
extern SemaphoreHandle_t ADC_Read;

// 双缓冲区设计防止数据覆盖
#define RX_BUF_SIZE 128
extern uint8_t rx_buf1[RX_BUF_SIZE];
extern uint8_t rx_buf2[RX_BUF_SIZE];
extern volatile uint8_t *active_buf;  // 当前激活的缓冲区
extern volatile uint8_t frame_ready;        // 帧就绪标志
extern uint8_t processing_buf[RX_BUF_SIZE];     // 处理缓冲区


void uart_transmit(void* Parameters);

void uart_receiver_data_process(void* Parameters);
void Start_UART_DMA_Receive(void);
//Idle中断的回调函数
void HAL_UART_IdleCallback(UART_HandleTypeDef *huart);

#endif
