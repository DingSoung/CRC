/* CRC-8-CCITT*/
#define endian8         1
#define Poly8_Normal	0x07
#define Poly8_Mirror	0xE0
#define Crc8_Init       0x00
#define Crc8_XorOut     0x00

/*CRC-8-Dallas/Maxim
 #define endian8         0
 #define Poly8_Normal	0x31
 #define Poly8_Mirror	0x8C
 #define Crc8_Init      0x00
 #define Crc8_XorOut    0x00
 */
unsigned char crc8table[256];
void init_crc8_table() {
#if endian8
    unsigned char Poly = Poly8_Normal;
    for (int i=0; i<256; i++) {
        unsigned char crc = i;
        for (unsigned char j=0; j<8; j++)
            crc = (crc << 1) ^ ((crc & 0x80) ? Poly : 0);
        crc8table[i] = crc & 0xFF;
    }
#else
    unsigned char Poly = Poly8_Mirror;
    for (unsigned short i = 0; i < 256; i++ ) {
        unsigned short crc = i;
        for ( unsigned char j = 0; j < 8; j++ )
            crc = (crc >> 1) ^ ((crc & 0x01) ? Poly : 0);
        crc8table[i] = crc & 0xFF;
    }
#endif
}
void crc8Init(unsigned char *pCrc8) {
    *pCrc8 = Crc8_Init;
}
void crc8Update(unsigned char *pCrc8, unsigned char *pData, unsigned int uSize){
#if endian8
    for (unsigned int i = 0; i < uSize; i++)
        *pCrc8 = crc8table[pData[i] ^ *pCrc8];
    //*pCrc8 = ((*pCrc8) << 8) ^ crc8table[(pData[i] ^ (*pCrc8 >> 0)) & 0xFF];
    //*pCrc8 = 0 ^ crc8table[(pData[i] ^ *pCrc8) & 0xFF];
#else
    for(unsigned int i = 0; i < uSize; i++)
        *pCrc8 = ((*pCrc8) >> 8) ^ crc8table[(pData[i] ^ *pCrc8) & 0xFF];
#endif
}
void crc8Update_Direct(unsigned char *pCrc8, unsigned char *pData, unsigned int uSize) {
#if endian8
    //code needed
#else
    for (int i = 0; i < uSize; i++) {
        *pCrc8 ^= pData[i];
        for (int j = 0; j < 8; j++) {
            *pCrc8 = (*pCrc8 >> 1) ^ ((*pCrc8 & 0x01) ? Poly8_Mirror : 0);
        }
    }
#endif
}
void crc8Finish(unsigned char *pCrc8) {
    *pCrc8 ^= Crc8_XorOut;
}