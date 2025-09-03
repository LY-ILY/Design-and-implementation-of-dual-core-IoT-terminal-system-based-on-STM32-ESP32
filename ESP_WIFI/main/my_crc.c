#include "my_crc.h"
Bool crc_check=my_false;

// 生成CRC-16查找表（256个条目）
void generate_crc16_table(uint16_t *table) {
    uint16_t polynomial = 0x1021;  // CRC-16-CCITT多项式
    
    for (uint32_t i = 0; i < 256; i++) {
        uint16_t crc = i << 8;  // 将字节移到高位
        
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ polynomial;
            } else {
                crc = crc << 1;
            }
        }
        table[i] = crc;
    }
}

// 计算两个字节的CRC-16校验码
uint16_t calculate_crc16(const uint8_t *data, size_t length) {
    static uint16_t table[256];
    static int table_generated = 0;
    
    // 首次使用时生成查找表
    if (!table_generated) {
        generate_crc16_table(table);
        table_generated = 1;
    }

    uint16_t crc = 0xFFFF;  // 初始值
    
    // 逐字节处理数据
    for (size_t i = 0; i < length; i++) {
        uint8_t byte = data[i];
        uint8_t index = (crc >> 8) ^ byte;  // 计算查找表索引
        crc = (crc << 8) ^ table[index];
    }
    
    return crc;
}

//数据帧的地址以及完整的帧的长度
Bool CRC16_check(uint8_t * data,int len){
	//不带帧尾
    /*这里获取原始CRC最好用移位的方式,将两个8位数据拼成一个16位数据*/
    /*最后两个字节的数据为CRC校验*/
	uint16_t old_crc = ((uint16_t)data[len-2]<<8)|(uint16_t)data[len-1];
	//调用生成CRC校验的代码
	uint32_t crc = calculate_crc16((const uint8_t *)data, len-2);
	//定义布尔变量返回结果
	//Bool bool;
	//判断两次的CRC的值是否相等，来判断数据帧是否正确
	if(crc==old_crc)
		return my_true;
	else 
		return my_false;
}
