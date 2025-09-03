#include "my_frame.h"

uint8_t ack[32];
uint8_t Fame_test_num4=0;
static uint8_t data_frame_id=0;
uint8_t Data_Frame_CRC_Begin_Index=0;
uint8_t adc_data_frame_constuction(uint8_t *data,uint8_t data_len,uint8_t sensor_id,uint8_t *adc_data_frame){

	uint8_t total_frame_len = Data_Frame_Data_Begin_Index + data_len + 2;
	adc_data_frame[Data_Frame_Head_Index]=0x55;
	adc_data_frame[Data_Frame_Ctgr_Index]=0x00;//֡���ͣ�����֡��
	xSemaphoreTake(xFrameIdMutex, portMAX_DELAY);//����
	adc_data_frame[Data_Frame_ID_Index]=data_frame_id++;//����֡��֡ID��ͬ����֡��֡ID�໥����
	xSemaphoreGive(xFrameIdMutex);//����
	adc_data_frame[Data_Frame_Data_Len_Index]=data_len;//���ݳ���
	adc_data_frame[Data_Frame_Sensor_Id_Index]=sensor_id;//������ID

	memcpy((adc_data_frame)+Data_Frame_Data_Begin_Index,data,data_len);//������
	Data_Frame_CRC_Begin_Index=Data_Frame_Data_Begin_Index+data_len;//����CRC��֡�е���ʼλ��
	uint16_t data_frame_crc=calculate_crc16(adc_data_frame, Data_Frame_Data_Begin_Index+data_len);//���㷢�����ݵ�CRC
	//д��CRC����
	adc_data_frame[Data_Frame_CRC_Begin_Index]=(uint8_t)(data_frame_crc>>8);//�߰�λ����
    adc_data_frame[Data_Frame_CRC_Begin_Index+1]=(uint8_t)(data_frame_crc&0xFF);//�Ͱ�λ����
	//��������֡����

	return total_frame_len;
}
