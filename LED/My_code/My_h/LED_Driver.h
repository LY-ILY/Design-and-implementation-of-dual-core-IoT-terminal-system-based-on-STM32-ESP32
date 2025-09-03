/* led_driver.h - LED驱动头文件 */
#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* LED状态枚举 */
typedef enum {
    LED_OFF = 0,
    LED_ON,
    LED_BLINK_SLOW,  // 慢速闪烁 (1Hz)
    LED_BLINK_FAST,  // 快速闪烁 (5Hz)
    LED_BREATH       // 呼吸效果
} LED_State_t;

/* LED配置结构体 */
typedef struct {
    GPIO_TypeDef* port;    // GPIO端口
    uint16_t pin;          // GPIO引脚
    uint8_t active_level;  // 有效电平 (0=低电平有效, 1=高电平有效)
} LED_Config_t;

/* LED控制句柄 */
typedef struct {
    LED_Config_t config;
    LED_State_t state;
    uint32_t blink_interval;  // 闪烁间隔(ms)
    TaskHandle_t task_handle; // 控制任务句柄
    QueueHandle_t cmd_queue;  // 命令队列
} LED_Handle_t;

/* LED命令类型 */
typedef struct {
    LED_State_t new_state;
    uint32_t param;  // 附加参数 (如闪烁频率)
} LED_Command_t;

/* 驱动API */
LED_Handle_t* LED_Init(LED_Config_t *config);
void LED_SetState(LED_Handle_t *handle, LED_State_t state);
void LED_SetBlinkRate(LED_Handle_t *handle, uint32_t interval_ms);
void LED_Toggle(LED_Handle_t *handle);

/* 内部任务 */
void LED_Control_Task(void *argument);

#endif /* LED_DRIVER_H */