#include "my_command_manager.h"

Com_Node *head=NULL;//头指针，指向头节点
//Com_Node *p=NULL;//迭代指针，遍历数据用
Com_Node *tail=NULL;//尾指针，数据入队用

void com_node_DeInit(){
    //已经初始化过来
    if (head != NULL) {
        //定义指针p用于遍历
        Com_Node *p = head;
        while (p != NULL) {
            //定义next指针，指向p的下一个节点
            Com_Node *next = p->next;
            //释放p节点中的数据的空间,头节点的data是NULL
            if (p->data) free(p->data);
            //释放p节点中的空间
            free(p);
            //p指向下一个节点
            p = next;
        }
        //指针指向空
        head=NULL;
        tail=NULL;
    }
}
//链表初始化
void com_node_Init(){
    // 如果已初始化，先释放旧链表
    com_node_DeInit();
    //申请空间
    Com_Node *head_node=(Com_Node*)malloc(sizeof(Com_Node));
    //空间申请不成功
    if (!head_node) {
        // 内存分配失败处理
        // 可以添加日志或错误处理
        return;
    }
    //头节点不存数据
    head_node->frame_id=0;
    head_node->data_len=0;
    head_node->re_sd_nm=0;
    head_node->data=NULL;
    head_node->next=NULL;
    head=head_node; //头指针指向头节点
    //p=head;         //迭代指针指向头节点
    tail=head_node; //初始状态尾指针指向头节点
}

//参数帧数据地址，以及帧的长度
void com_node_add(uint8_t *frame_data,const uint8_t len){
    printf("com_node_add begin!:\n");
    //申请节点空间
    Com_Node *frame_node=(Com_Node*)malloc(sizeof(Com_Node));
    if (!frame_node) {
        // 处理内存不足：记录日志/返回错误
        return;
    }
    /*frame_data也是malloc空间，而且就是数据，所以直接更改指针指向就行，不需要再申请空间*/
    /*
    //申请数据空间
    frame_node->data=(uint8_t*)malloc(len*sizeof(uint8_t));
    //复制数据内容
    memcpy(frame_node->data,frame_data,len*sizeof(uint8_t));
    */
    //修改指针，指向frame_data所指的malloc空间
    frame_node->data=frame_data;
    //获取帧的序号
    frame_node->frame_id=frame_data[Command_Frame_ID_Index];
    //写入数据长度
    frame_node->data_len=len;
    //写入重发次数
    frame_node->re_sd_nm=0;
    //从尾部插入，属于尾节点
    frame_node->next=NULL;
    //插入到链表中
    tail->next=frame_node;
    //尾节点后移
    tail=tail->next;
    //保险起见，尾节点的next置空
    tail->next=NULL;
     printf("com_node_add sucess!:%s\n",(char*)frame_data);
}

//参数为帧的id
void com_node_free(const uint8_t frame_id){
    /*
    头节点不参与遍历，
    头节点的frame_id为0，
    会和实际帧id为0的帧冲突
    */
    //迭代指针，遍历数据用
    Com_Node *p=NULL;
    //p指向头节点的下一个节点
    p=head->next;
    //定义一个指针q指向p的前一个节点
    Com_Node *q=head;
    //p指向的空间有数据
    while (p!=NULL)
    {   
        //找到了ACK帧对应的命令帧的节点
        if(frame_id==p->frame_id){
            //当节点是尾节点时
            if(p==tail)
                tail=q;
            /*修改链表结构*/
            q->next=p->next;
            /*释放p指针对应的空间*/
            //首先释放节点中的数据空间
            free(p->data);
            //释放p指针指向的空间
            free(p);
            //停止遍历
            break;
        }
        q=p;
        p=p->next;
    }
}

//若CRC校验未通过查找对应的帧，参数为要存放数据的指针，返回值为数据帧的长度
//这个指针为数组指针（已经分配好空间了）
uint8_t com_node_frame_find(uint8_t **data,const uint8_t frame_id){
    /*
    头节点不参与遍历，
    头节点的frame_id为0，
    会和实际帧id为0的帧冲突
    */
   //定义一个长度变量，用来返回数据的长度
    uint8_t len=0;
    //迭代指针，遍历数据用
    Com_Node *p=NULL;
    //p指向头节点的下一个节点
    p=head->next;
    //定义一个指针q指向p的前一个节点
    Com_Node *q=head;
    //p指向的空间有数据
    while (p!=NULL)
    {   
        //找到了NACK帧对应的命令帧的节点
        if(frame_id==p->frame_id){
            //将外部指针指向该数据所在的空间
            *data=p->data;
            //获取帧长度用于返回
            len=p->data_len;
            /*重发时不改变帧id，故数据不需要先出去再进来，也就不需要删除节点*/
            /*对上一条注释进行纠正说明，因为在串口发送任务中所有的命令指令都会进入一次命令管理队列，
            所以，必须要将该节点删除，否者将造成节点重复，
            但是，节点中的数据确是还需要使用的，所以使用一个外部指针指向该数据所在的空间即可，
            且改空间不能释放
            */
            //当节点是尾节点时
            if(p==tail)
                tail=q;
            /*修改链表结构*/
            q->next=p->next;
            
            /*释放p指针对应的空间*/
            //不需要释放节点中的数据空间，它已经被外部指针指向，数据还要再次进入命令管理队列
            //free(p->data);
            //释放p指针指向的空间
            free(p);
 
            //退出循环
            break;
        }
        q=p;
        p=p->next;
    }
    return len;    
}


