#include "my_topic.h"

/*
// 第 1 步：定义枚举，代表所有需要处理的话题类型
typedef enum {
    TOPIC_UNKNOWN = 0,    // 未知话题，用于错误处理
    TOPIC_LIVING_ROOM_TEMP,
    TOPIC_LIVING_ROOM_HUMIDITY,
    TOPIC_KITCHEN_LIGHT_STATUS,
    // ... 可以继续添加其他话题
    TOPIC_COUNT // 这个可以用来计算枚举项的数量，方便循环
} topic_enum_t;

// 第 2 步：定义映射表的结构体
typedef struct {
    const char* topic_str;   // 话题字符串
    topic_enum_t topic_enum; // 对应的枚举值
} topic_map_t;

// 第 3 步：初始化映射表
// 这是一个静态数组，初始化了所有 字符串-枚举 的对应关系
topic_map_t topic_mapping_table[] = {
    {"home/living-room/temperature",    TOPIC_LIVING_ROOM_TEMP},
    {"home/living-room/humidity",       TOPIC_LIVING_ROOM_HUMIDITY},
    {"home/kitchen/light/status",       TOPIC_KITCHEN_LIGHT_STATUS},
    // ... 添加其他映射
};

// 计算映射表中有多少项
const int topic_map_size = sizeof(topic_mapping_table) / sizeof(topic_mapping_table[0]);

// 第 4 步：实现查找函数
// 这个函数接收一个 topic 字符串，返回对应的枚举值
topic_enum_t map_topic_to_enum(const char* topic) {
    for (int i = 0; i < topic_map_size; i++) {
        // 使用 strcmp 进行精确字符串匹配
        if (strcmp(topic, topic_mapping_table[i].topic_str) == 0) {
            return topic_mapping_table[i].topic_enum;
        }
    }
    // 如果没有找到匹配项，返回未知类型
    return TOPIC_UNKNOWN;
}
*/

char *topic_list[2]={"jtest/led","jtest/motor"};
uint8_t topic_to_topicid(char *topic){
    int i=0;
    for(;i<2;i++){
        if(strcmp(topic_list[i],topic)==0){
            return i+1;
        }
    }
    return i;
}

// 转换字符串
uint8_t string_to_uint8(const char* str) {
    if (str == NULL || strlen(str) == 0) {
        return 0;
    }
    
    // 检查所有字符是否为数字
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i]<'0'||str[i]>'9') {
            return 0;
        }
    }
    
    uint16_t value = atoi(str); // 使用atoi获取整数值
    
    // 确保值在0-255范围内
    if (value > 255) {
        return 255;
    }
    
    return (uint8_t)value;
}




