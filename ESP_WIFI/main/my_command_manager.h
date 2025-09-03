#ifndef __COMMAND_MANAGER__H__
#define __COMMAND_MANAGER__H__
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "my_frame.h"


typedef struct Com_Node Com_Node;  // 前置声明
struct Com_Node {
    uint8_t frame_id;   // 重帧ID
    uint8_t data_len;   //数据长度
    uint8_t re_sd_nm;   // 重发次数
    uint8_t *data;      //数据指针
    Com_Node *next;     // 现在可以正确引用
};

extern Com_Node *head;//头指针，指向头节点
//extern Com_Node *p;//迭代指针，遍历数据用
extern Com_Node *tail;//尾指针，数据入队用

void com_node_DeInit();//注销函数
void com_node_Init();//初始化函数
void com_node_add(uint8_t *frame_data,uint8_t len);//添加节点的函数
void com_node_free(uint8_t frame_id);//删除节点的函数
uint8_t com_node_frame_find(uint8_t **data,const uint8_t frame_id);//查找帧的函数
#endif