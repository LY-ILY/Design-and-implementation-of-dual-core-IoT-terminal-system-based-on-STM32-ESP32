# ifndef __MQTT__H__
# define __MQTT__H__

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "nvs_flash.h"
#include "my_semaphore.h"
#include "my_command_manager.h"
#include "my_frame.h"
#include "my_topic.h"
# define Subscribe "jtest/led"
# define Publish "jtest/topic"
/*MQTT的URI需要根据本地IP或者对面服务器IP的具体值进行更改*/
# define MQTT_URI "mqtt://192.168.105.54:1883"
# define MQTT_user_Name "ESP32"
# define MQTT_password "0.123456"
extern esp_mqtt_client_handle_t client;
//MQTT任务优先级是5
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void Mqtt_connect();
void mqtt_send_task(void * pra);

//esp_mqtt_client_publish(client, "topic", "message", 0, 0, 0);
// 参数：客户端句柄, 主题, 数据, 数据长度(0=自动), QoS, 保留标志

# endif

