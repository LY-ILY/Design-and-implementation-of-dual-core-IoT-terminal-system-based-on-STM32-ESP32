#ifndef __WIFI_H__
#define __WIFI_H__
#include "esp_now.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/sys.h"

/*

#define WIFI_SSID      "TP-LINK_3CC9"
#define WIFI_PASS      "15070466320"
*/
#define WIFI_SSID      "123"
#define WIFI_PASS      "12345678"

#define MAX_RETRY      5  // 最大重连次数


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void My_wifi_Init(void);
void My_wifi_connect(void);

#endif
