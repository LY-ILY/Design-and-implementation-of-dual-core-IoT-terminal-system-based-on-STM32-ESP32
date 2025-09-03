#include "my_tim.h"
#include "usart.h"
#include "string.h"

TimerHandle_t My_Tim_handle = NULL;


void My_Tim_Init(void) {
    // 创建软件定时器
    My_Tim_handle = xTimerCreate(
        "ADC_Timer", 
        pdMS_TO_TICKS(1000), 
        pdTRUE,
        NULL, 
        My_Timer_Callback
    );
    
    // 检查定时器是否创建成功
    if (My_Tim_handle == NULL) {
        // 处理创建失败的情况
        //char str1[] = "Timer creation failed!\r\n";
        //HAL_UART_Transmit(&huart1, (uint8_t *)str1, strlen(str1), HAL_MAX_DELAY);
    }
}

void My_Tim_Start(void) {
    if (My_Tim_handle == NULL) {
        My_Tim_Init();
    }

    // 确保定时器已创建
    if (My_Tim_handle != NULL) {
        if (xTimerIsTimerActive(My_Tim_handle) == pdFALSE) {
            if (xTimerStart(My_Tim_handle, 0) != pdPASS) {
                // 处理启动失败的情况
                //char str2[] = "Timer start failed!\r\n";
                //HAL_UART_Transmit(&huart1, (uint8_t *)str2, strlen(str2), HAL_MAX_DELAY);
            }
        }
    } else {
        // 定时器创建失败的处理
        //char str3[] = "Timer handle is NULL!\r\n";
        //HAL_UART_Transmit(&huart1, (uint8_t *)str3, strlen(str3), HAL_MAX_DELAY);
    }
}
uint8_t tim_test_num=0;
void My_Timer_Callback(TimerHandle_t xTimer) {
    // 软件定时器回调在守护任务中执行，不是ISR上下文
    // 因此使用普通版本的信号量给予函数
    xSemaphoreGive(ADC_Read);
	//OLED_ShowNum(1,0*16,tim_test_num++,5,OLED_8X16);OLED_Update();
	
}
