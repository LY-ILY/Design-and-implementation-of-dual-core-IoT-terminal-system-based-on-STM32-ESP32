#include "my_frame.h"

uint8_t ack[32];
uint8_t Fame_test_num4=0;
static uint8_t data_frame_id=0;
uint8_t Data_Frame_CRC_Begin_Index=0;
uint8_t adc_data_frame_constuction(uint8_t *data,uint8_t data_len,uint8_t sensor_id,uint8_t *adc_data_frame){

	uint8_t total_frame_len = Data_Frame_Data_Begin_Index + data_len + 2;
	adc_data_frame[Data_Frame_Head_Index]=0x55;
	adc_data_frame[Data_Frame_Ctgr_Index]=0x00;//帧类型（数据帧）
	xSemaphoreTake(xFrameIdMutex, portMAX_DELAY);//加锁
	adc_data_frame[Data_Frame_ID_Index]=data_frame_id++;//数据帧的帧ID，同命令帧的帧ID相互独立
	xSemaphoreGive(xFrameIdMutex);//解锁
	adc_data_frame[Data_Frame_Data_Len_Index]=data_len;//数据长度
	adc_data_frame[Data_Frame_Sensor_Id_Index]=sensor_id;//传感器ID

	memcpy((adc_data_frame)+Data_Frame_Data_Begin_Index,data,data_len);//拷贝数
	Data_Frame_CRC_Begin_Index=Data_Frame_Data_Begin_Index+data_len;//计算CRC再帧中的起始位置
	uint16_t data_frame_crc=calculate_crc16(adc_data_frame, Data_Frame_Data_Begin_Index+data_len);//计算发送数据的CRC
	//写入CRC数据
	adc_data_frame[Data_Frame_CRC_Begin_Index]=(uint8_t)(data_frame_crc>>8);//高八位数据
    adc_data_frame[Data_Frame_CRC_Begin_Index+1]=(uint8_t)(data_frame_crc&0xFF);//低八位数据
	//返回数据帧长度

	return total_frame_len;
}
