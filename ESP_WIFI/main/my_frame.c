#include "my_frame.h"

static uint8_t Command_frame_id=0;
uint8_t command_frame_construct(const uint8_t tcpic,const uint8_t *data,uint8_t data_len,uint8_t **output_frame){
    //计算帧所需空间大小
    uint8_t total_frame_len = Command_Frame_Data_Begin_Index + data_len + 2;
    //申请帧空间
    uint8_t *frame_command  =(uint8_t *)malloc(total_frame_len*sizeof(uint8_t));
     // 检查是否会溢出缓冲区
    if (frame_command==NULL) {
        return 0; // 空间申请失败
    }
    /*构建帧*/
    frame_command[Command_Frame_Head_Index]=0x55;//帧头
    frame_command[Command_Frame_Ctgr_Index]=0x01;//帧类型，命令帧
    frame_command[Command_Frame_ID_Index]=Command_frame_id++;//写入帧的序号，结束后帧数加一
    frame_command[Command_Frame_Data_Len_Index]=1+data_len;//确定数据帧有效数据的长度 1表示话题
    frame_command[Command_Frame_Topic_Index]=tcpic;//写入以及映射好的话题
    memcpy(frame_command+Command_Frame_Data_Begin_Index,data,data_len);//将data中的数据复制到帧中对应的位置
    uint8_t cmd_frame_crc_begin_index=0;
    cmd_frame_crc_begin_index=Command_Frame_Data_Begin_Index+data_len;//计算CRC再帧中的起始位置
    //计算crc16,参数分别为数据地址和数据长度
    uint16_t crc16 = calculate_crc16(frame_command,Command_Frame_Data_Begin_Index+data_len);
    frame_command[cmd_frame_crc_begin_index]=(uint8_t)(crc16>>8);//高八位数据
    frame_command[cmd_frame_crc_begin_index+1]=(uint8_t)(crc16&0xFF);//低八位数据
    *output_frame=frame_command;//将返回指针指向省的空间
    frame_command=NULL;//将原来的指针指向NULL
    return total_frame_len;
}
void data_frame_analytic(uint8_t *frame_data,uint8_t frame_data_len,mqtt_msg_t *send_to_mqtt_data){
    //获取传感器编号
    uint8_t sonsor_id=frame_data[Data_Frame_Sensor_Id_Index];
    /*根据编号查找要发送的话题*/
    char *topic= "jtest/topic";
    uint8_t topic_len=0;
    //存放数据的空间，字节数为数据长度减1（减去话题的那个字节）
    uint8_t *data=(uint8_t *)malloc(sizeof(uint8_t)*(frame_data[Data_Frame_Data_Len_Index]-1));
    //将帧中的数据复制到已经申请好的空间中，地2个参数是基址加偏移地址，
    memcpy(data,frame_data+Data_Frame_Data_Begin_Index,frame_data[Data_Frame_Data_Len_Index]-1);
    //将数据写入对应的结构体中，方便后续发送，数据空间交由MQTT的发送函数释放；
    send_to_mqtt_data->data=data;
    send_to_mqtt_data->data_len=frame_data[Data_Frame_Data_Len_Index]-1;
    send_to_mqtt_data->topic=topic;
    send_to_mqtt_data->topic_len=topic_len;
}