#include "my_uart.h"

static const char *USART_TAG = "UART2_EXAMPLE";

// 初始化 UART2
void uart2_init(void) {
    // UART 配置结构体
    uart_config_t uart_config = {
        .baud_rate = 115200,          // 波特率
        .data_bits = UART_DATA_8_BITS, // 数据位
        .parity = UART_PARITY_DISABLE, // 无校验
        .stop_bits = UART_STOP_BITS_1, // 停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // 无流控
        .source_clk = UART_SCLK_APB,  // 时钟源
    };
    
    // 配置 UART2 参数
    ESP_ERROR_CHECK(uart_param_config(UART2_PORT, &uart_config));
    
    // 设置 UART2 引脚
    ESP_ERROR_CHECK(uart_set_pin(UART2_PORT, UART2_TX_PIN, UART2_RX_PIN, 
                                UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
    // 安装 UART 驱动程序
    ESP_ERROR_CHECK(uart_driver_install(UART2_PORT, BUF_SIZE * 2, 
                                       BUF_SIZE * 2, 0, NULL, 0));
    
    ESP_LOGI(USART_TAG, "UART2 Init completed (TX:GPIO%d, RX:GPIO%d)", UART2_TX_PIN, UART2_RX_PIN);
}

// UART2 发送数据
void uart_send_task(void *pvParameters) {
    uart_frame_t received_frame;
    while(1) {
        if ((xQueueReceive(queue_mqtt_to_uatr, &received_frame, portMAX_DELAY) == pdPASS)) {
            // 通过UART发送数据
            printf("uart_write_bytes received_frame sucess!\n");
            uart_write_bytes(UART2_PORT,received_frame.data, received_frame.data_len);
        
            printf("uart_write_bytes sucess!\n");
            /*发送完成后春不释放，而是直接存入命令管理队列中，可以节省开销*/
            //数据存入命令光临队列
            //free(received_frame.data);
            com_node_add(received_frame.data, received_frame.data_len);
            
        }
    }
}

// UART2 接收数据任务
void uart2_receive(void* pra) {
    uint8_t data[BUF_SIZE];//BUF_SIZE=32
    while (1) {
        // 等待数据
        const int rxBytes = uart_read_bytes(UART2_PORT, data, BUF_SIZE - 1, pdMS_TO_TICKS(1000));
        /********************************************/
        /*******这里UART接收函数最好只负责接收数据******/
        /***判断、解析和发送可以交由另外一个任务器完成***/
        /********************************************/
        if (rxBytes > 0) {
            //向消息队列发送数据
            uart_receive_data_prosess_t uart_receive_data_to_process;
            //复制数据到要发送的结构体中
            memcpy(uart_receive_data_to_process.data,data,rxBytes);
            uart_receive_data_to_process.data_len=rxBytes;
            printf("Receive Data Success!\n");
            if (send_queue_uart_receive_data_prosess(queue_uart_receive_data_prosess, 
                                        &uart_receive_data_to_process, SEM_WAIT_TIME) != pdPASS) {
                // 如果发送失败，需要释放内存
                printf("Failed to send to queue\n");
                //free(send_to_uart_frame.data);
                continue;; // 或适当的错误处理
            }
        } else if (rxBytes == 0) {
            ESP_LOGD(USART_TAG, "NOT Receive data");
        } else {
            ESP_LOGE(USART_TAG, "Receive Error: %d", rxBytes);
        }
        //vTaskDelay(pdMS_TO_TICKS(100));  // 增加这一行
    }
}

void Uart_Receive_Frame_Process_task(void* pra){
    uart_receive_data_prosess_t uart_receive_data;
    
    while(1) {
        //从消息队列中读取数据，并发送
        if (xQueueReceive(queue_uart_receive_data_prosess, &uart_receive_data, portMAX_DELAY) == pdPASS) {
            uint8_t *data=uart_receive_data.data;
            uint8_t rxBytes=uart_receive_data.data_len;
            
            ESP_LOGI(USART_TAG, "UART Receive %d Byte: %s", rxBytes, (char*)data);
            uint8_t frame_category = data[Data_Frame_Ctgr_Index];//帧类型
            if(frame_category==0x00){
                //命令帧字段
                /*stm32没有命令发送给ESP32*/
            }else if(frame_category>0x00&&frame_category<0x80){
                //数据帧
                /*这里收到数据之后做CRC校验，校验通过上传，未通过就丢弃该帧数据*/
                if(CRC16_check(data,rxBytes)==my_true){
                    //通过CRC校验，向MQTT发送数据
                    //MQTT_publish((char*)data);
                    /*需要将收到的数据进行解帧*/
                    mqtt_msg_t send_to_mqtt_data;
                    /*从收到的数据正中解析出话题和数据，并写入send_to_mqtt_data中*/
                    data_frame_analytic(data,rxBytes,&send_to_mqtt_data);
                    //向MQTT发送任务发送消息队列
                    if (send_queue_uatr_to_mqtt(queue_mqtt_to_uatr, &send_to_mqtt_data, SEM_WAIT_TIME) != pdPASS) {
                        // 如果发送失败，需要释放内存
                        printf("Failed to send to queue\n");
                        free(send_to_mqtt_data.data);
                        continue; // 或适当的错误处理
                     }
                    printf("%s\n",(char*)data);
                }else {
                    //数据帧校验未通过，放弃该帧
                    printf("%s\n","Data Frame CRC check failed!");
                    continue;
                }
                
            }else if(frame_category>=0x80){
                //应答帧
                /*判断应答是否成功*/
                if(data[Ack_Frame_Ack_Index]==0x01){
                    //应答成功，移除命令管理队列中相应的数据,参数为帧ID
                    com_node_free(data[Ack_Frame_ID_Index]);
                }else if(data[Ack_Frame_Ack_Index]==0x00) {
                    //应答不成功，在队列中查找对应的消息，并将其发送到给串口的消息队列
                    //定义一个指针，用来指向命令管理队列中的数据所在的malloc空间
                    uint8_t *resend_data=NULL;
                    //返回数据的长度
                    uint8_t data_len= com_node_frame_find(&resend_data,data[Ack_Frame_ID_Index]);
                    // 检查帧构建是否成功
                    if (data_len == 0 || resend_data == NULL) {
                        // 处理错误情况
                        printf("Frame construction failed\n");
                        // 如果有分配内存但失败，确保释放
                        if (resend_data != NULL) {
                            free(resend_data);
                        }
                        continue; // 或适当的错误处理
                    }
                    //向消息队列发送数据
                     //定于用于消息队列的结构体
                    uart_frame_t resend_to_uart_frame;
                    resend_to_uart_frame.data_len=data_len;//写入帧长度
                    resend_to_uart_frame.data=resend_data;//修改指针指向
                    resend_data=NULL;//原指针指向空
                    //向消息队列发送消息
                    /*消息队列浅拷贝，故此处不释放malloc空间，该空间交由uart发送线程发送数据后释放*/
                    if (send_queue_mqtt_to_uatr(queue_mqtt_to_uatr, &resend_to_uart_frame, SEM_WAIT_TIME) != pdPASS) {
                        // 如果发送失败，需要释放内存
                        printf("Failed to send to queue\n");
                        free(resend_to_uart_frame.data);
                        continue; // 或适当的错误处理
                    }

                    printf("%s\n","ACK failed!");
                    continue;
                }else{
                    //其他情况
                    printf("%s\n","Other ACK Condition!");
                }
            }
            
        }
    }
    

}

