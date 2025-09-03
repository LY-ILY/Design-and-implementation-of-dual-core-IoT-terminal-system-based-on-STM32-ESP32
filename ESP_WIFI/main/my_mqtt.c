#include "my_mqtt.h"

esp_mqtt_client_handle_t client = NULL;
static const char *TAG = "MQTT_EXAMPLE";
//static esp_mqtt_client_handle_t client;

// MQTT事件处理函数
void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    client = event->client;

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED://MQTT成功连接事件
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            esp_mqtt_client_subscribe(client, Subscribe, 0);
            ESP_LOGI(TAG, "MQTT_EVENT_Subscribe");
            esp_mqtt_client_publish(client, Publish, "Hello from ESP32-S3", 0, 0, 0);
            ESP_LOGI(TAG, "MQTT_EVENT_Publish");
            break;

        case MQTT_EVENT_DISCONNECTED://MQTT断开连接事件
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_DATA://MQTT接收到数据事件
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            //将MQTT接收到的数据组成能够直接发送的数据帧
            /*MQTT1接收到的话题需要进行映射，接收到的数据也需要进行处理*/
            //将话题的字符串映射成对应的数值
            uint8_t tcpic=topic_to_topicid(event->topic);
            if(tcpic==0){
                printf("Topic map failed!\n");
                return; // 或适当的错误处理
            }
            //将mqtt接收到命令转换成对应的数值数据
            uint8_t com_num=string_to_uint8(event->data);
            //定于一个用于返回帧的指针
            uint8_t *frame_send_to_uart=NULL;
            //使用函数构建帧
            uint8_t frame_len=command_frame_construct(tcpic,&com_num,1,&frame_send_to_uart);
            printf("command_frame_construct sucessed!\n");
            // 检查帧构建是否成功
            if (frame_len == 0 || frame_send_to_uart == NULL) {
                // 处理错误情况
                printf("Frame construction failed\n");
                // 如果有分配内存但失败，确保释放
                if (frame_send_to_uart != NULL) {
                    free(frame_send_to_uart);
                }
                return; // 或适当的错误处理
            }
            //定于用于消息队列的结构体
            uart_frame_t send_to_uart_frame;
            send_to_uart_frame.data_len=frame_len;//写入帧长度
            send_to_uart_frame.data=frame_send_to_uart;//修改指针指向
            frame_send_to_uart=NULL;//原指针指向空
            printf("send_to_uart_frame sucessed!\n");
            //向消息队列发送消息
            /*消息队列浅拷贝，故此处不释放malloc空间，该空间交由uart发送线程发送数据后释放*/
            if (send_queue_mqtt_to_uatr(queue_mqtt_to_uatr, &send_to_uart_frame, SEM_WAIT_TIME) != pdPASS) {
                // 如果发送失败，需要释放内存
                printf("Failed to send to queue\n");
                free(send_to_uart_frame.data);
                return; // 或适当的错误处理
            }
            printf("send_queue_mqtt_to_uatr sucessed!\n");
            break;
            /*
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            //调用串口，实现数据传递给STM32
            //应该创建一个堆变量，控制具体长度，这样log时才不会有多余内容
            size_t data_len=event->data_len;
            char *data=(char*)malloc((data_len+1)*sizeof(char));
            data[data_len]='\0';
            memcpy(data,event->data,data_len);
            uart2_send(data,data_len);
            free(data);
            */
            
            

        default:
            break;
    }
}

void Mqtt_connect(){

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .address.uri = MQTT_URI, // 电脑IP
            //.verification.skip_cert_common_name_check = true,
        },
        .credentials = {
            .username = MQTT_user_Name,  // EMQX用户名
            .authentication.password = MQTT_password, // EMQX密码
        }
    };
    // MQTT客户端配置
    // 使用本地EMQX服务器
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}
void  mqtt_send_task(void * pra){
    
    mqtt_msg_t data_send_to_mqtt;
    
    while(1) {
        //从消息队列中读取数据，并发送
        if (xQueueReceive(queue_uatr_to_mqtt, &data_send_to_mqtt, portMAX_DELAY) == pdPASS) {
            
            // 向MQTT发送数据
            esp_mqtt_client_publish(client, 
                                    data_send_to_mqtt.topic, 
                                    (char*)(data_send_to_mqtt.data), 
                                    data_send_to_mqtt.data_len, 1, 0);
            //释放申请的数据空间
            free(data_send_to_mqtt.topic);
            free(data_send_to_mqtt.data);
        }
    }
}
