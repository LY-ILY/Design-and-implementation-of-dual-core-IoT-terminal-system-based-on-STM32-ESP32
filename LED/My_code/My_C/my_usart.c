# include <my_usart.h>


uint8_t rx_buf1[RX_BUF_SIZE];
uint8_t rx_buf2[RX_BUF_SIZE];
volatile uint8_t *active_buf = rx_buf1;  // 当前激活的缓冲区
volatile uint8_t frame_ready = 0;        // 帧就绪标志
uint8_t processing_buf[RX_BUF_SIZE];     // 处理缓冲区
TaskHandle_t uart_task_handle = NULL;  // 任务句柄
uint8_t UART_transmit_test_num=0;
volatile uint8_t uart_tx_busy = 0;
void uart_transmit(void* Parameters){
	 //char str[] = "uart_transmit Start!2\r\n";
	//HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
	Frame_to_uart_t send_msg;
	uart_task_handle = xTaskGetCurrentTaskHandle(); // 保存当前任务句柄
    
   while (1) {
        if (xQueueReceive(Queue_frame_to_uart, &send_msg, portMAX_DELAY) == pdPASS) {
            
            if (HAL_UART_Transmit_DMA(&huart1, send_msg.frame_data, send_msg.data_len) == HAL_OK) {
                // 等待 DMA 发送完成回调的通知
                //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

                // DMA 完成后安全刷新 OLED
                OLED_ShowNum(1, 2*16, UART_transmit_test_num++, 3, OLED_8X16);OLED_Update();
                
            } else {
                // 如果 HAL_BUSY，可以稍微等一下再试
                vTaskDelay(pdMS_TO_TICKS(1));
				OLED_ShowNum(1, 0*16, UART_transmit_test_num++, 3, OLED_8X16);OLED_Update();
            }
        }
    }
}
/*
//串口发送回调函数 没有使用中断和DMA发送，不会启用串口回调
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
		//uart_tx_busy = 0;
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(uart_task_handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        // 发送完成后的逻辑
        // 例如，发送确认消息
        //uint8_t confirm_msg[] = "Transmit_Callack is OK!\r\n";
        //HAL_UART_Transmit(huart, confirm_msg, sizeof(confirm_msg), HAL_MAX_DELAY);  // 重新启动发送操作
		
    }
}


*/


uint8_t uart_receive_buff[8];

//处理串口接收数据任务
void uart_receiver_data_process(void* Parameters) {
	//char str[] = "uart_receiver_data_process Start!3\r\n";
	//HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), HAL_MAX_DELAY);
	//定义从消息队列接收消息的结构体数据
	Cmd_frame_process_t cmd_frame_data;
	
	//定义从要发送ACK消息到串口任务的结构体数据
	Frame_to_uart_t ack_frame_data;
	Bool crc_check=my_false;
    while(xQueueReceive(Queue_cmd_frame_process, &cmd_frame_data, portMAX_DELAY) == pdPASS){
		//指针指向数据
		uint8_t *cmd_frame=cmd_frame_data.frame_data;
		uint8_t  data_len=cmd_frame_data.data_len;
		//CRC16校验
		crc_check=CRC16_check(cmd_frame,data_len);
		//开始主帧
		ack[Ack_Frame_Head_Index] 	 =0x55;
		ack[Ack_Frame_Ctgr_Index]	|= cmd_frame[Command_Frame_Ctgr_Index];//第二个字节，帧类型
		ack[Ack_Frame_ID_Index] 	 = cmd_frame[Command_Frame_ID_Index];//第三个字节，帧序号
		
		//CRC校验成功
		if(crc_check==my_true){
			//发送成功应答
			ack[Ack_Frame_Ack_Index]  = 0x00;//CRC校验成功	
		}
		//CRC校验失败
		else if(crc_check==my_false){
			ack[Ack_Frame_Ack_Index]  = 0x01;//CRC校验失败	
			//数据校验失败，任务帧不发送，将空间释放
			//free(cmd_frame);
		}else{
			ack[Ack_Frame_Ack_Index]  = 0xff;
		}
		
		uint16_t crc	=calculate_crc16(ack, 4);//计算crc的值
		ack[Ack_Frame_CRC_Begin_Index] 	= crc & 0xFF; // 低字节 (LSB)
		ack[Ack_Frame_CRC_Begin_Index+1] = (crc >> 8) & 0xFF;// 高字节 (MSB)
		//向ack发送给UART任务的结构体数据中写入数据
		ack_frame_data.data_len=Ack_Frame_CRC_Begin_Index+1;
		memcpy(ack_frame_data.frame_data,ack,Ack_Frame_CRC_Begin_Index+1);
		//向UART发送任务的消息队列发送消息
		xQueueSend(Queue_frame_to_uart, &ack_frame_data, portMAX_DELAY);
		
		//CRC校验成功，向控制任务发送消息
		if(crc_check==my_true){
			Command_frame_to_control_t control_frame_data;
			//帧内容
			memcpy(control_frame_data.frame_data,cmd_frame,data_len);
			//control_frame_data.frame_data=cmd_frame;
			control_frame_data.data_len=data_len;
			xQueueSend(Queue_command_to_control, &control_frame_data, portMAX_DELAY);
		}
		
	}
}

// 启动DMA接收
void Start_UART_DMA_Receive(void)
{
    // 启动DMA接收
    HAL_UART_Receive_DMA(&huart1, (uint8_t*)active_buf, RX_BUF_SIZE);
	 
    
    // 使能IDLE中断
    //__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}

void HAL_UART_IdleCallback(UART_HandleTypeDef *huart) {
	
  if (huart->Instance == USART1) {
    //  停止DMA以安全访问计数器
    HAL_UART_DMAStop(huart);
	 
   
    // 计算接收到的数据长度
    uint8_t frame_length = RX_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx);
	  
	
	  //数据在active_buf指向的数组中,将其发送给数据处理任务
	Cmd_frame_process_t cmd_frame_data;
	cmd_frame_data.data_len=frame_length;
	//申请数据空间
	//cmd_frame_data.frame_data=(uint8_t*)malloc(frame_length*sizeof(uint8_t));
	//复制数据
	memcpy(cmd_frame_data.frame_data,(const void *)active_buf,frame_length);
	//向消息队列发送数据
	BaseType_t xHigherPriorityTaskWoken = pdFALSE; // 初始化为 pdFALS
	xQueueSendToBackFromISR(Queue_cmd_frame_process, &cmd_frame_data, &xHigherPriorityTaskWoken);
	  
    // 切换缓冲区并重启DMA
    active_buf = (active_buf == rx_buf1) ? rx_buf2 : rx_buf1;
    HAL_UART_Receive_DMA(huart, (uint8_t*)active_buf, RX_BUF_SIZE);

    // 重新使能IDLE中断
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
  }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
		
        // 发送完成后的逻辑
        // 例如，发送确认消息
        //uint8_t confirm_msg[] = "Transmit_Callack is OK!\r\n";
        //HAL_UART_Transmit(huart, confirm_msg, sizeof(confirm_msg), HAL_MAX_DELAY);  // 重新启动发送操作
    }
}


/*
标志位
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
    // 1. 获取并清除错误标志
    uint32_t errors = huart->ErrorCode;
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE | UART_FLAG_FE | 
                         UART_FLAG_NE | UART_FLAG_ORE);
    
    // 2. 重新初始化UART
    HAL_UART_DeInit(huart);
    MX_USART1_UART_Init();
    
    // 3. 重启DMA接收
    HAL_UART_Receive_DMA(huart, (uint8_t*)active_buf, RX_BUF_SIZE);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
  }
}
/*
void USART1_IRQHandler(void) {
  // 检测IDLE中断标志
  if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)) {
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);  // 清除IDLE标志
    HAL_UART_IdleCallback(&huart1);      // 调用IDLE回调
  }
  
  // 处理其他UART中断
  HAL_UART_IRQHandler(&huart1);
}*/





















