#include "my_adc.h"

uint16_t my_adcBuffer[ADC_BUFFER_SIZE] = {0};
volatile bool adc_conversion_complete = false;
uint8_t ADC_test_num=0;
void my_get_adc(void *pvParameters)
{    
    while(1) {
		
        if(xSemaphoreTake(ADC_Read, portMAX_DELAY) == pdTRUE) {
			
			
            // ����ת����ɱ�־Ϊfalse
            adc_conversion_complete = false;
            
            // ����ADC DMAת��
            HAL_ADC_Start_DMA(&hadc1, (uint32_t*)my_adcBuffer, ADC_BUFFER_SIZE);
            
            // �ȴ�ת�����
            while(!adc_conversion_complete) {
                vTaskDelay(1); // �����ӳ٣�����æ�ȴ�
            }
           
            // ����ADC����
            Frame_to_uart_t frame_to_uart;
            uint8_t adc_uint8_data[ADC_BYTE_NUMS];
            
            for(int i = 0; i < ADC_BUFFER_SIZE; i++) {
				 
                // ��ȡADC���ݵĸ�λ�͵�λ
                adc_uint8_data[0] = (uint8_t)(my_adcBuffer[i] >> 8);
                adc_uint8_data[1] = (uint8_t)(my_adcBuffer[i] & 0xFF);
                
                // ��������֡
                uint8_t frame_len = adc_data_frame_constuction(
                    adc_uint8_data, 
                    ADC_BYTE_NUMS+1, 
                    i + 1, 
                    frame_to_uart.frame_data
                );
                
                frame_to_uart.data_len = frame_len;
                
                // ���͵�����
                if(xQueueSend(Queue_frame_to_uart, &frame_to_uart, portMAX_DELAY) != pdPASS) {
                    // ������ʧ��
                }
				OLED_ShowNum(1,1*16,ADC_test_num++,3,OLED_8X16);OLED_Update();
            }
        }
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1) {
		
        // ֹͣDMA
        HAL_ADC_Stop_DMA(&hadc1);
        
        // ����ת����ɱ�־
        adc_conversion_complete = true;
        
        // ����ת�������Ϣ
        //uint8_t send_msg[] = {0, 1, 2, 3};
        //HAL_UART_Transmit(&huart1, send_msg, sizeof(send_msg), HAL_MAX_DELAY);
    }
}
