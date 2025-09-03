/* led_driver.c - LED驱动实现 */
#include "led_driver.h"

#define LED_TASK_STACK_SIZE 256
#define LED_QUEUE_LENGTH    5

/* 私有函数声明 */
static void write_led(LED_Handle_t *handle, uint8_t level);
static void process_breath(LED_Handle_t *handle);

/* 初始化LED驱动 */
LED_Handle_t* LED_Init(LED_Config_t *config) {
    LED_Handle_t *handle = pvPortMalloc(sizeof(LED_Handle_t));
    configASSERT(handle);
    
    memcpy(&handle->config, config, sizeof(LED_Config_t));
    handle->state = LED_OFF;
    handle->blink_interval = 500; // 默认500ms
    
    // 创建命令队列
    handle->cmd_queue = xQueueCreate(LED_QUEUE_LENGTH, sizeof(LED_Command_t));
    configASSERT(handle->cmd_queue);
    
    // 创建控制任务
    BaseType_t result = xTaskCreate(
        LED_Control_Task,
        "LED_Ctrl",
        LED_TASK_STACK_SIZE,
        handle,
        tskIDLE_PRIORITY + 2,
        &handle->task_handle
    );
    
    configASSERT(result == pdPASS);
    return handle;
}

/* 设置LED状态 */
void LED_SetState(LED_Handle_t *handle, LED_State_t state) {
    LED_Command_t cmd = {
        .new_state = state,
        .param = 0
    };
    xQueueSend(handle->cmd_queue, &cmd, portMAX_DELAY);
}

/* 设置闪烁频率 */
void LED_SetBlinkRate(LED_Handle_t *handle, uint32_t interval_ms) {
    LED_Command_t cmd = {
        .new_state = LED_BLINK_SLOW, // 任意闪烁状态
        .param = interval_ms
    };
    xQueueSend(handle->cmd_queue, &cmd, portMAX_DELAY);
}

/* 切换LED状态 */
void LED_Toggle(LED_Handle_t *handle) {
    LED_State_t new_state = (handle->state == LED_ON) ? LED_OFF : LED_ON;
    LED_SetState(handle, new_state);
}

/* LED控制任务 */
void LED_Control_Task(void *argument) {
    LED_Handle_t *handle = (LED_Handle_t*)argument;
    LED_Command_t cmd;
    TickType_t last_wake_time = xTaskGetTickCount();
    uint32_t blink_timer = 0;
    uint8_t breath_dir = 0; // 0=渐亮, 1=渐暗
    uint8_t breath_val = 0;
    
    for(;;) {
        // 检查新命令 (非阻塞)
        if(xQueueReceive(handle->cmd_queue, &cmd, 0) == pdPASS) {
            if(cmd.new_state <= LED_BREATH) {
                handle->state = cmd.new_state;
            }
            
            if(cmd.param > 0) {
                handle->blink_interval = cmd.param;
            }
            
            // 立即响应状态变化
            if(handle->state == LED_ON) {
                write_led(handle, 1);
            } else if(handle->state == LED_OFF) {
                write_led(handle, 0);
            }
        }
        
        // 状态机处理
        switch(handle->state) {
            case LED_BLINK_SLOW:
            case LED_BLINK_FAST:
                if((xTaskGetTickCount() - blink_timer) >= 
                   pdMS_TO_TICKS(handle->blink_interval)) {
                    HAL_GPIO_TogglePin(handle->config.port, handle->config.pin);
                    blink_timer = xTaskGetTickCount();
                }
                break;
                
            case LED_BREATH:
                process_breath(handle);
                break;
                
            default:
                break;
        }
        
        // 精确周期控制 (50Hz)
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(20));
    }
}

/* 私有函数：写入LED电平 */
static void write_led(LED_Handle_t *handle, uint8_t level) {
    if(handle->config.active_level) {
        HAL_GPIO_WritePin(handle->config.port, handle->config.pin, 
                          level ? GPIO_PIN_SET : GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(handle->config.port, handle->config.pin, 
                          level ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}

/* 呼吸效果处理 */
static void process_breath(LED_Handle_t *handle) {
    static uint8_t pwm_val = 0;
    static uint8_t dir = 0; // 0=递增, 1=递减
    
    // 简单PWM实现 (实际项目中应使用硬件PWM)
    static uint32_t pwm_counter = 0;
    const uint32_t pwm_period = 20; // 20ms周期
    
    if(pwm_counter == 0) {
        write_led(handle, (pwm_val > 0) ? 1 : 0);
    } else if(pwm_counter == pwm_val) {
        write_led(handle, 0);
    }
    
    // 更新计数器
    pwm_counter = (pwm_counter + 1) % pwm_period;
    
    // 每10个周期调整PWM占空比
    static uint8_t delay_counter = 0;
    if(++delay_counter >= 10) {
        delay_counter = 0;
        
        if(dir == 0) {
            if(++pwm_val >= 100) {
                pwm_val = 100;
                dir = 1;
            }
        } else {
            if(--pwm_val == 0) {
                dir = 0;
            }
        }
    }
}