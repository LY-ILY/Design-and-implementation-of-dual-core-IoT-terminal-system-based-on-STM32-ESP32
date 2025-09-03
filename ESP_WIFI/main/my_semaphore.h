#ifndef __MY__SEMAPHORE__H__
#define __MY__SEMAPHORE__H__

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "stdint.h"
#include "esp_log.h"

//为信号设置阻塞时间，单位毫秒
#define SEM_WAIT_TIME pdMS_TO_TICKS(1000) 
// 消息类型定义
typedef enum {
    MSG_UART_RX_DATA,      // UART接收到的数据
    MSG_MQTT_RX_CMD,       // MQTT接收到的命令
    MSG_ACK_RESPONSE,      // ACK响应
    MSG_NACK_RESPONSE,     // NACK响应
    MSG_RETRANSMIT,        // 需要重传的帧
    MSG_ADC_DATA,          // ADC采集的数据
    MSG_CONTROL_CMD        // 控制命令
} message_type_t;

// UART数据帧结构
typedef struct {
    uint8_t data_len;
    uint8_t *data;
} uart_frame_t;

// MQTT消息结构
typedef struct {
    char *topic;
    uint8_t *data;
    uint8_t data_len;
    uint8_t topic_len;
} mqtt_msg_t;

// 串口数据处理结构
typedef struct {
    uint8_t data[24];
    uint8_t data_len;
} uart_receive_data_prosess_t;



// 队列句柄声明
//extern QueueHandle_t uart_rx_queue;
extern QueueHandle_t queue_mqtt_to_uatr;
extern QueueHandle_t queue_uatr_to_mqtt;
extern QueueHandle_t queue_uart_receive_data_prosess;
// 初始化所有消息队列
void Init_message_queues();

// 发送消息到队列（带超时）
BaseType_t send_queue_mqtt_to_uatr(QueueHandle_t queue, const uart_frame_t *message, TickType_t timeout);
BaseType_t send_queue_uatr_to_mqtt(QueueHandle_t queue, const mqtt_msg_t *message, TickType_t timeout);
BaseType_t send_queue_uart_receive_data_prosess(QueueHandle_t queue, const uart_receive_data_prosess_t *message, TickType_t timeout);



#endif