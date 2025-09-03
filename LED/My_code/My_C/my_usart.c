# include <my_usart.h>


uint8_t rx_buf1[RX_BUF_SIZE];
uint8_t rx_buf2[RX_BUF_SIZE];
volatile uint8_t *active_buf = rx_buf1;  // ��ǰ����Ļ�����
volatile uint8_t frame_ready = 0;        // ֡������־
uint8_t processing_buf[RX_BUF_SIZE];     // ��������
TaskHandle_t uart_task_handle = NULL;  // ������
uint8_t UART_transmit_test_num=0;
volatile uint8_t uart_tx_busy = 0;
void uart_transmit(void* Parameters){
	 //char str[] = "uart_transmit Start!2\r\n";
	//HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
	Frame_to_uart_t send_msg;
	uart_task_handle = xTaskGetCurrentTaskHandle(); // ���浱ǰ������
    
   while (1) {
        if (xQueueReceive(Queue_frame_to_uart, &send_msg, portMAX_DELAY) == pdPASS) {
            
            if (HAL_UART_Transmit_DMA(&huart1, send_msg.frame_data, send_msg.data_len) == HAL_OK) {
                // �ȴ� DMA ������ɻص���֪ͨ
                //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

                // DMA ��ɺ�ȫˢ�� OLED
                OLED_ShowNum(1, 2*16, UART_transmit_test_num++, 3, OLED_8X16);OLED_Update();
                
            } else {
                // ��� HAL_BUSY��������΢��һ������
                vTaskDelay(pdMS_TO_TICKS(1));
				OLED_ShowNum(1, 0*16, UART_transmit_test_num++, 3, OLED_8X16);OLED_Update();
            }
        }
    }
}
/*
//���ڷ��ͻص����� û��ʹ���жϺ�DMA���ͣ��������ô��ڻص�
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
		//uart_tx_busy = 0;
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(uart_task_handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        // ������ɺ���߼�
        // ���磬����ȷ����Ϣ
        //uint8_t confirm_msg[] = "Transmit_Callack is OK!\r\n";
        //HAL_UART_Transmit(huart, confirm_msg, sizeof(confirm_msg), HAL_MAX_DELAY);  // �����������Ͳ���
		
    }
}


*/


uint8_t uart_receive_buff[8];

//�����ڽ�����������
void uart_receiver_data_process(void* Parameters) {
	//char str[] = "uart_receiver_data_process Start!3\r\n";
	//HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
	//�������Ϣ���н�����Ϣ�Ľṹ������
	Cmd_frame_process_t cmd_frame_data;
	
	//�����Ҫ����ACK��Ϣ����������Ľṹ������
	Frame_to_uart_t ack_frame_data;
	Bool crc_check=my_false;
    while(xQueueReceive(Queue_cmd_frame_process, &cmd_frame_data, portMAX_DELAY) == pdPASS){
		//ָ��ָ������
		uint8_t *cmd_frame=cmd_frame_data.frame_data;
		uint8_t  data_len=cmd_frame_data.data_len;
		//CRC16У��
		crc_check=CRC16_check(cmd_frame,data_len);
		//��ʼ��֡
		ack[Ack_Frame_Head_Index] 	 =0x55;
		ack[Ack_Frame_Ctgr_Index]	|= cmd_frame[Command_Frame_Ctgr_Index];//�ڶ����ֽڣ�֡����
		ack[Ack_Frame_ID_Index] 	 = cmd_frame[Command_Frame_ID_Index];//�������ֽڣ�֡���
		
		//CRCУ��ɹ�
		if(crc_check==my_true){
			//���ͳɹ�Ӧ��
			ack[Ack_Frame_Ack_Index]  = 0x00;//CRCУ��ɹ�	
		}
		//CRCУ��ʧ��
		else if(crc_check==my_false){
			ack[Ack_Frame_Ack_Index]  = 0x01;//CRCУ��ʧ��	
			//����У��ʧ�ܣ�����֡�����ͣ����ռ��ͷ�
			//free(cmd_frame);
		}else{
			ack[Ack_Frame_Ack_Index]  = 0xff;
		}
		
		uint16_t crc	=calculate_crc16(ack, 4);//����crc��ֵ
		ack[Ack_Frame_CRC_Begin_Index] 	= crc & 0xFF; // ���ֽ� (LSB)
		ack[Ack_Frame_CRC_Begin_Index+1] = (crc >> 8) & 0xFF;// ���ֽ� (MSB)
		//��ack���͸�UART����Ľṹ��������д������
		ack_frame_data.data_len=Ack_Frame_CRC_Begin_Index+1;
		memcpy(ack_frame_data.frame_data,ack,Ack_Frame_CRC_Begin_Index+1);
		//��UART�����������Ϣ���з�����Ϣ
		xQueueSend(Queue_frame_to_uart, &ack_frame_data, portMAX_DELAY);
		
		//CRCУ��ɹ����������������Ϣ
		if(crc_check==my_true){
			Command_frame_to_control_t control_frame_data;
			//֡����
			memcpy(control_frame_data.frame_data,cmd_frame,data_len);
			//control_frame_data.frame_data=cmd_frame;
			control_frame_data.data_len=data_len;
			xQueueSend(Queue_command_to_control, &control_frame_data, portMAX_DELAY);
		}
		
	}
}

// ����DMA����
void Start_UART_DMA_Receive(void)
{
    // ����DMA����
    HAL_UART_Receive_DMA(&huart1, (uint8_t*)active_buf, RX_BUF_SIZE);
	 
    
    // ʹ��IDLE�ж�
    //__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}

void HAL_UART_IdleCallback(UART_HandleTypeDef *huart) {
	
  if (huart->Instance == USART1) {
    //  ֹͣDMA�԰�ȫ���ʼ�����
    HAL_UART_DMAStop(huart);
	 
   
    // ������յ������ݳ���
    uint8_t frame_length = RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx);
	  
	
	  //������active_bufָ���������,���䷢�͸����ݴ�������
	Cmd_frame_process_t cmd_frame_data;
	cmd_frame_data.data_len=frame_length;
	//�������ݿռ�
	//cmd_frame_data.frame_data=(uint8_t*)malloc(frame_length*sizeof(uint8_t));
	//��������
	memcpy(cmd_frame_data.frame_data,(const void *)active_buf,frame_length);
	//����Ϣ���з�������
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; // ��ʼ��Ϊ pdFALS
	xQueueSendToBackFromISR(Queue_cmd_frame_process, &cmd_frame_data, &xHigherPriorityTaskWoken);
	  
    // �л�������������DMA
    active_buf = (active_buf == rx_buf1) ? rx_buf2 : rx_buf1;
    HAL_UART_Receive_DMA(huart, (uint8_t*)active_buf, RX_BUF_SIZE);

    // ����ʹ��IDLE�ж�
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
  }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
		
        // ������ɺ���߼�
        // ���磬����ȷ����Ϣ
        //uint8_t confirm_msg[] = "Transmit_Callack is OK!\r\n";
        //HAL_UART_Transmit(huart, confirm_msg, sizeof(confirm_msg), HAL_MAX_DELAY);  // �����������Ͳ���
    }
}


/*
��־λ
#define UART_FLAG_CTS                       ((uint32_t)USART_SR_CTS)
#define UART_FLAG_LBD                       ((uint32_t)USART_SR_LBD)
#define UART_FLAG_TXE                       ((uint32_t)USART_SR_TXE)
#define UART_FLAG_TC                        ((uint32_t)USART_SR_TC)
#define UART_FLAG_RXNE                      ((uint32_t)USART_SR_RXNE)
#define UART_FLAG_IDLE                      ((uint32_t)USART_SR_IDLE)
#define UART_FLAG_ORE                       ((uint32_t)USART_SR_ORE)
#define UART_FLAG_NE                        ((uint32_t)USART_SR_NE)
#define UART_FLAG_FE                        ((uint32_t)USART_SR_FE)
#define UART_FLAG_PE                        ((uint32_t)USART_SR_PE)
*/

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1) {
    // 1. ��ȡ����������־
    uint32_t errors = huart->ErrorCode;
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE | UART_FLAG_FE | 
                         UART_FLAG_NE | UART_FLAG_ORE);
    
    // 2. ���³�ʼ��UART
    HAL_UART_DeInit(huart);
    MX_USART1_UART_Init();
    
    // 3. ����DMA����
    HAL_UART_Receive_DMA(huart, (uint8_t*)active_buf, RX_BUF_SIZE);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
  }
}
/*
void USART1_IRQHandler(void) {
  // ���IDLE�жϱ�־
  if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)) {
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);  // ���IDLE��־
    HAL_UART_IdleCallback(&huart1);      // ����IDLE�ص�
  }
  
  // ��������UART�ж�
  HAL_UART_IRQHandler(&huart1);
}*/





















