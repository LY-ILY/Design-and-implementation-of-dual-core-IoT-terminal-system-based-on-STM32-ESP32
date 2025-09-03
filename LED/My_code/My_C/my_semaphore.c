#include "my_semaphore.h"
#include "usart.h"

StaticSemaphore_t xMutexBuffer;
SemaphoreHandle_t xFrameIdMutex = NULL;
SemaphoreHandle_t ADC_Read = NULL;
SemaphoreHandle_t UART_receive = NULL;
QueueHandle_t Queue_frame_to_uart = NULL;
QueueHandle_t Queue_cmd_frame_process = NULL;
QueueHandle_t Queue_command_to_control = NULL;

// ��ʼ�����о��
//QueueHandle_t QueueHandle_ADC=NULL;
void SemaphoresInit(void){
	// ����һ��������
	xFrameIdMutex = xSemaphoreCreateMutexStatic(&xMutexBuffer);
	//��ʼ�������ź���
	ADC_Read = xSemaphoreCreateBinary();
	UART_receive = xSemaphoreCreateBinary();
	//��ʼ��������Ϣ����
	Queue_frame_to_uart			= xQueueCreate( QUEUE_frame_to_uart_len, sizeof(Frame_to_uart_t));
	Queue_cmd_frame_process 	= xQueueCreate( QUEUE_cmd_frame_process_len, sizeof(Cmd_frame_process_t));
	Queue_command_to_control 	= xQueueCreate( QUEUE_command_to_control_len, sizeof(Command_frame_to_control_t));
	
}

