#ifndef CRC16_H
#define CRC16_H

void init_crc16_table(void) ;
void crc16Init(unsigned short *pCrc16);
void crc16Update(unsigned short *pCrc16, unsigned char *pData, unsigned int uSize) ;
void crc16Finish(unsigned short *pCrc16) ;

#endif