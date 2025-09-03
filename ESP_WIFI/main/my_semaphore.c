#include "my_semaphore.h"


// 定义队列句柄
QueueHandle_t queue_mqtt_to_uatr=NULL;
QueueHandle_t queue_uatr_to_mqtt=NULL;
QueueHandle_t queue_uart_receive_data_prosess=NULL;
// 初始化所有消息队列
void Init_message_queues() {
    // 创建队列，根据消息大小和预期深度调整参数
   
    //mqtt_pub_queue = xQueueCreate(10, sizeof(queue_message_t));
    queue_mqtt_to_uatr = xQueueCreate(10, sizeof(uart_frame_t));
    queue_uatr_to_mqtt = xQueueCreate(10, sizeof(mqtt_msg_t));
    queue_uart_receive_data_prosess=xQueueCreate(10, sizeof(uart_receive_data_prosess_t));
    
    if (queue_mqtt_to_uatr==NULL||queue_uatr_to_mqtt ==NULL||queue_uart_receive_data_prosess==NULL) {
        ESP_LOGE("QUEUES", "Failed to create one or more queues");
        // 可以根据需要采取恢复措施
    } else {
        ESP_LOGI("QUEUES", "All message queues created successfully");
    }
}

// MQTT接收到消息，传递给串口的发送消息到队列
BaseType_t send_queue_mqtt_to_uatr(QueueHandle_t queue, const uart_frame_t *message, TickType_t timeout) {
    if (queue == NULL) {
        ESP_LOGE("QUEUES", "Queue handle is NULL");
        return pdFAIL;
    }
    
    BaseType_t status = xQueueSend(queue, message, timeout);
    if (status != pdPASS) {
        ESP_LOGW("QUEUES", "Failed to send message to queue (timeout or full)");
        // 可以根据消息类型决定是否重试或丢弃
    }
    return status;
}

// 串口接收到消息，传递给MQTT的发送消息到队列
BaseType_t send_queue_uatr_to_mqtt(QueueHandle_t queue, const mqtt_msg_t *message, TickType_t timeout){
    if (queue == NULL) {
        ESP_LOGE("QUEUES", "Queue handle is NULL");
        return pdFAIL;
    }
    BaseType_t status = xQueueSend(queue, message, timeout);
    if (status != pdPASS) {
        ESP_LOGW("QUEUES", "Failed to send message to queue (timeout or full)");
        // 可以根据消息类型决定是否重试或丢弃
    }
    return status;
}

BaseType_t send_queue_uart_receive_data_prosess(QueueHandle_t queue, const uart_receive_data_prosess_t *message, TickType_t timeout){
    if (queue == NULL) {
        ESP_LOGE("QUEUES", "Queue handle is NULL");
        return pdFAIL;
    }
    BaseType_t status = xQueueSend(queue, message, timeout);
    if (status != pdPASS) {
        ESP_LOGW("QUEUES", "Failed to send message to queue (timeout or full)");
        // 可以根据消息类型决定是否重试或丢弃
    }
    return status;

}

