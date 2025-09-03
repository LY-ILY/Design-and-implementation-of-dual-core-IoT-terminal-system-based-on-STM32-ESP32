#ifndef __MY__FRAME__H__
#define __MY__FRAME__H__
#include "stdint.h"
#include "string.h"
#include "my_crc.h"
#include "my_semaphore.h"
/*���յ�����֡��ʽ*/
#define Command_Frame_Head_Index 0 //֡ͷ�±�
#define Command_Frame_Ctgr_Index 1 //֡�����±�
#define Command_Frame_ID_Index 2 //֡����±�
#define Command_Frame_Data_Len_Index 3//���ݳ��ȵ��±�
#define Command_Frame_Topic_Index 4//�����ֽ������±�
#define Command_Frame_Data_Begin_Index 5 //������ʼ�ֽ������±�
extern uint8_t Command_Frame_CRC_Begin_Index; //CRCУ����ʼ�ֽ������±�,��Ҫ���м���


/*���͵�����֡��ʽ*/
#define Data_Frame_Head_Index 0 //֡ͷ�±�
#define Data_Frame_Ctgr_Index 1 //֡�����±�
#define Data_Frame_ID_Index 2 //֡����±�
#define Data_Frame_Data_Len_Index 3//���ݳ��ȵ��±�
#define Data_Frame_Sensor_Id_Index 4//�������ֽ������±�
#define Data_Frame_Data_Begin_Index 5 //������ʼ�ֽ������±�
extern uint8_t Data_Frame_CRC_Begin_Index; //CRCУ����ʼ�ֽ������±�,��Ҫ���м���



/*���͵�Ӧ��֡��ʽACK*/
#define Ack_Frame_Head_Index 0 //֡ͷ�±�
#define Ack_Frame_Ctgr_Index 1 //֡�����±�
#define Ack_Frame_ID_Index 2 //֡����±�
#define Ack_Frame_Ack_Index 3//Ӧ���ֶε��±�
#define Ack_Frame_CRC_Begin_Index 4  //CRCУ����ʼ�ֽ������±�,��Ҫ���м���
extern uint8_t ack[32];

uint8_t adc_data_frame_constuction(uint8_t *data,uint8_t data_len,uint8_t sensor_id,uint8_t *adc_data_frame);
#endif

