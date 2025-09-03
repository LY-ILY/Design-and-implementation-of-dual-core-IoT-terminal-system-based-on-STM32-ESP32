/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "my_uart.h"
#include "my_wifi.h"
#include "my_mqtt.h"
#include "my_task.h"
#include "my_command_manager.h"
#include "my_semaphore.h"
/*
系统需要完成的功能如下：
1、wifi连接
2、串口接收STM32数据
3、向MQTT发送来自STM32的数据
4、接收来自MQTT的数据
5、使用串口向STM32发送数据
*/

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    /*
    初始化
    */
    uart2_init();
    com_node_Init();
    My_wifi_Init();
    My_wifi_connect();
    Init_message_queues();
    //不再使用TCP网络

    
    /*MQTT连接，同时开启事件触发*/
    Mqtt_connect();
    
    /*创建任务*/
    Create_Task(Uart_Receive);
    Create_Task(Uart_Send);
    Create_Task(MQTT_Send);
    Create_Task(Uart_Receive_Frame_Process);
    while(1){
        //esp_mqtt_client_publish(client, Publish, "Hello! Im ESP32", 0, 1, 0);
       
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

}


/*
void My_LED_Init(void){
    //ESP_ERROR_CHECK(example_connect());
    gpio_config_t pGPIOConfig= {
        .intr_type=GPIO_INTR_DISABLE,//中断类型
        .mode=GPIO_MODE_OUTPUT,//输入输出模式
        .pin_bit_mask=1ULL<<LED_GPIO,//引脚掩码
        .pull_down_en=GPIO_PULLDOWN_DISABLE,//是否使能内部下拉电阻。
        .pull_up_en= GPIO_PULLUP_DISABLE,//是否使能内部上拉电阻。
    };
    
    gpio_config(&pGPIOConfig);
    return;
}
*/
