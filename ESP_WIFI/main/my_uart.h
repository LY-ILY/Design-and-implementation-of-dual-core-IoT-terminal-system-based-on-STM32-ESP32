#ifndef __USART_H__
#define __USART_H__
#include "soc/uart_channel.h"
#include <stdio.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <string.h>
#include "my_frame.h"
#include "my_semaphore.h"
#include "my_crc.h"
#include "my_command_manager.h"

// UART2 配置参数
#define UART2_TX_PIN GPIO_NUM_17   // UART2 发送引脚
#define UART2_RX_PIN GPIO_NUM_18   // UART2 接收引脚
#define UART2_PORT   UART_NUM_2    // UART2 端口号
#define BUF_SIZE     256          // 缓冲区大小
#define FRAME_SIZE   256          // 命令帧大小

extern uint8_t frame_index;//帧序号
extern uint8_t frame_nums[FRAME_SIZE];//命令帧数组

void uart2_init(void);
void uart_send_task(void *pvParameters);
void uart2_receive(void* pra);
void Uart_Receive_Frame_Process_task(void* pra);

#endif