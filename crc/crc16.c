/* CRC-16-CCITT 1D0F*/
#define endian16	1
#define Poly16_Normal	0x1021
#define Poly16_Mirror	0x8408
#define Crc16_Init	0x1D0F
#define Crc16_XorOut	0x0000

/* CRC-16-CCITT
#define endian16	0
#define Poly16_Normal	0x1021
#define Poly16_Mirror	0x8408
#define Crc16_Init	0x0000
#define Crc16_XorOut	0x0000
*/

/* CRC-16-CCITT Xmodem
#define endian16	1
#define Poly16_Normal	0x1021
#define Poly16_Mirror	0x8408
#define Crc16_Init	0x0000
#define Crc16_XorOut	0x0000
*/
/* CRC-16 X25
#define endian16	0
#define Poly16_Normal	0x1021
#define Poly16_Mirror	0x8408
#define Crc16_Init	0xFFFF
#define Crc16_XorOut	0xFFFF
*/
/* CRC-16 IBM
#define endian16	0
#define Poly16_Normal	0x8005
#define Poly16_Mirror	0xA001
#define Crc16_Init	0x0000
#define Crc16_XorOut	0x0000
*/

unsigned short crc16table[256];
void init_crc16_table(void) {
#if endian16
	unsigned short Poly = Poly16_Normal;
	for (unsigned short i = 0; i < 256; i++ ) {
		unsigned short crc = i << 8;
		for ( unsigned char j = 0; j < 8; j++ )
			crc = (crc << 1) ^ ((crc & 0x8000) ? Poly : 0);	
		crc16table[i] = crc & 0xFFFF;
	}
#else
	unsigned short Poly = Poly16_Mirror;
	for (unsigned short i = 0; i < 256; i++ ) {
		unsigned short crc = i;
		for ( unsigned char j = 0; j < 8; j++ )
			crc = (crc >> 1) ^ ((crc & 0x0001) ? Poly : 0);	
		crc16table[i] = crc & 0xFFFF;
	}
#endif
}
void crc16Init(unsigned short *pCrc16) {
	*pCrc16 = Crc16_Init;
}
void crc16Update(unsigned short *pCrc16, unsigned char *pData, unsigned int uSize) {
#if endian16
	for(unsigned int i = 0; i < uSize; i++)
		*pCrc16 = ((*pCrc16) << 8) ^ crc16table[(pData[i] ^ (*pCrc16 >> 8)) & 0xFF];
#else
	for(unsigned int i = 0; i < uSize; i++)
		*pCrc16 = ((*pCrc16) >> 8) ^ crc16table[(pData[i] ^ *pCrc16) & 0xFF];
#endif
}
void crc16Finish(unsigned short *pCrc16) {
	*pCrc16 ^=  Crc16_XorOut;
}