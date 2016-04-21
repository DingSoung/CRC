#ifndef CRC32_H
#define CRC32_H

void init_crc32_table(void) ;
void crc32Init(unsigned int *pCrc32) ;
void crc32Update(unsigned int *pCrc32, unsigned char *pData, unsigned int uSize);
void crc32Finish(unsigned int *pCrc32);

#endif