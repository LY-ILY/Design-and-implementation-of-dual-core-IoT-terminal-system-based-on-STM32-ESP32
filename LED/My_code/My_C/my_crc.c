#include "my_crc.h"


// ����CRC-16���ұ�256����Ŀ��
void generate_crc16_table(uint16_t *table) {
    uint16_t polynomial = 0x1021;  // CRC-16-CCITT����ʽ
    
    for (uint32_t i = 0; i < 256; i++) {
        uint16_t crc = i << 8;  // ���ֽ��Ƶ���λ
        
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

// ���������ֽڵ�CRC-16У����
uint16_t calculate_crc16(const uint8_t *data, size_t length) {
    static uint16_t table[256];
    static int table_generated = 0;
    
    // �״�ʹ��ʱ���ɲ��ұ�
    if (!table_generated) {
        generate_crc16_table(table);
        table_generated = 1;
    }

    uint16_t crc = 0xFFFF;  // ��ʼֵ
    
    // ���ֽڴ�������
    for (size_t i = 0; i < length; i++) {
        uint8_t byte = data[i];
        uint8_t index = (crc >> 8) ^ byte;  // ������ұ�����
        crc = (crc << 8) ^ table[index];
    }
    
    return crc;
}


Bool CRC16_check(uint8_t * data,int len){
	//����֡β
	uint16_t old_crc=*(uint16_t*)(data+(len-2));
	//��������CRCУ��Ĵ���
	uint32_t crc = calculate_crc16((const uint8_t *)data, len-2);
	//���岼���������ؽ��
	//Bool bool;
	//�ж����ε�CRC��ֵ�Ƿ���ȣ����ж�����֡�Ƿ���ȷ
	if(crc==old_crc)
		return my_true;
	else 
		return my_false;
}
