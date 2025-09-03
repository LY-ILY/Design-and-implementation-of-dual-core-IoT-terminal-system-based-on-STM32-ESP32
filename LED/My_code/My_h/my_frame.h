#ifndef __MY__FRAME__H__
#define __MY__FRAME__H__
#include "stdint.h"
#include "string.h"
#include "my_crc.h"
#include "my_semaphore.h"
/*接收的命令帧格式*/
#define Command_Frame_Head_Index 0 //帧头下标
#define Command_Frame_Ctgr_Index 1 //帧类型下标
#define Command_Frame_ID_Index 2 //帧序号下标
#define Command_Frame_Data_Len_Index 3//数据长度的下标
#define Command_Frame_Topic_Index 4//话题字节所在下标
#define Command_Frame_Data_Begin_Index 5 //数据起始字节所在下标
extern uint8_t Command_Frame_CRC_Begin_Index; //CRC校验起始字节所在下标,需要进行计算


/*发送的数据帧格式*/
#define Data_Frame_Head_Index 0 //帧头下标
#define Data_Frame_Ctgr_Index 1 //帧类型下标
#define Data_Frame_ID_Index 2 //帧序号下标
#define Data_Frame_Data_Len_Index 3//数据长度的下标
#define Data_Frame_Sensor_Id_Index 4//传感器字节所在下标
#define Data_Frame_Data_Begin_Index 5 //数据起始字节所在下标
extern uint8_t Data_Frame_CRC_Begin_Index; //CRC校验起始字节所在下标,需要进行计算



/*发送的应答帧格式ACK*/
#define Ack_Frame_Head_Index 0 //帧头下标
#define Ack_Frame_Ctgr_Index 1 //帧类型下标
#define Ack_Frame_ID_Index 2 //帧序号下标
#define Ack_Frame_Ack_Index 3//应答字段的下标
#define Ack_Frame_CRC_Begin_Index 4  //CRC校验起始字节所在下标,需要进行计算
extern uint8_t ack[32];

uint8_t adc_data_frame_constuction(uint8_t *data,uint8_t data_len,uint8_t sensor_id,uint8_t *adc_data_frame);
#endif

