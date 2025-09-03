#include "my_task.h"
//串口接收任务
TaskHandle_t Uart_Receive_handle;
Task_Config Uart_Receive={
  .TaskCode=uart2_receive,
  .name="Uart_Receive",
  .parameter=NULL,
  .priority=5,
  .stack_size=4096,
  .Task_Handle=&Uart_Receive_handle
};
//串口发送任务
TaskHandle_t Uart_Send_handle;
Task_Config Uart_Send={
  .TaskCode=uart_send_task,
  .name="Uart_Send",
  .parameter=NULL,
  .priority=5,
  .stack_size=4096,
  .Task_Handle=&Uart_Send_handle
};

//MQTT发送任务
TaskHandle_t MQTT_Send_handle;
Task_Config MQTT_Send={
  .TaskCode=mqtt_send_task,
  .name="MQTT_Send",
  .parameter=NULL,
  .priority=5,
  .stack_size=4096,
  .Task_Handle=&MQTT_Send_handle
};

//串口接收数据处理任务
TaskHandle_t Uart_Receive_Frame_Process_handle;
Task_Config Uart_Receive_Frame_Process={
  .TaskCode=Uart_Receive_Frame_Process_task,
  .name="Uart_Receive_Frame_Process",
  .parameter=NULL,
  .priority=5,
  .stack_size=4096,
  .Task_Handle=&Uart_Receive_Frame_Process_handle
};

// Function that creates a task.
void Create_Task(Task_Config my_tesk)
{
    // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
    // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
    // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
    // the new task attempts to access it.
    BaseType_t result =xTaskCreate( my_tesk.TaskCode,
                my_tesk.name,
                my_tesk.stack_size, 
                my_tesk.parameter, 
                my_tesk.priority, 
                my_tesk.Task_Handle);
    
    if (result != pdPASS) {
        ESP_LOGE("TASK", "任务 %s 创建失败！", my_tesk.name);
    }   
}

