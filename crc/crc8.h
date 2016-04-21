#ifndef CRC8_H
#define CRC8_H

void init_crc8_table();
void crc8Init(unsigned char *pCrc8);
void crc8Update(unsigned char *pCrc8, unsigned char *pData, unsigned int uSize);
void crc8Update_Direct(unsigned char *pCrc8, unsigned char *pData, unsigned int uSize);
void crc8Finish(unsigned char *pCrc8);

#endif