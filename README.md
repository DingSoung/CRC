---
title:	高性能CRC算法
date:	2014.08.01
tags:	算法, C, 数据
---

### CRC32 CRC16 CRC8
use table

### Usage
	endian =
	0: msb,	RefIn = TRUE,	RefOut = TRUE	Mirror_Poly
	1: lsb,	RefIn = FALSE,	RefOut = FALSE	Normal_Poly

### Different criterion
refer [Wikipedia](http://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks) to add a rule ,for example CRC16 IBM below, uncommant it, and commant the last one. the some examples, if I miss any， pull request is welcome

	/* CRC-16 IBM
	#define endian16        0
	#define Poly16_Normal	0x8005
	#define Poly16_Mirror	0xA001
	#define Crc16_Init      0x0000
	#define Crc16_XorOut	0x0000
	*/

#### [C code and DEMO](https://github.com/DingSoung/CRC.git)

#### Asmbly code example
CRC8.asm
```
crc8TableInit:	;output crc table start from sram 0x20, sram 0x0C wil over write
	PUSH	R3
	PUSH	R2
	MOV	R3,	#0x00		;for (i = 0; i<256 ;) {
	fori:
	LDR	R0,	R3
	JP	B8,	=foriend
		
		MOV	R1,	#0x0C	;crc = i
		LDR	[R1],	R3
		
		MOV	R2,	#0x00	;for (j = 0; j<8 ;) {
		forj:
		LDR	R0,	R2
		JP	B3,	=forjend
			PUSH	R3
			PUSH	R2
			MOV	R1,	#0x0C	;C1 = (crc << 1) => R2
			LDR	R0,	[R1]
			RSL	R0,	#1
			AND	R0,	#0xFF
			LDR	R2,	R0
			MOV	R1,	#0x0C	;C2 = ((crc & 0x80) ? Poly : 0) => R3
			LDR	R0,	[R1]
			JP	B7,	=Poly
			MOV	R3,	#0x00
			JP	A1,	=afterPoly
			Poly:
			MOV	R3,	#0x07
			afterPoly:		;crc = (C1 ^ C2) & 0xFF
			LDR	R0,	R2
			XOR	R0,	R3
			AND	R0,	#0xFF
			MOV	R1,	#0x0C
			LDR	[R1],	R0
			POP	R2
			POP	R3
		LDR	R0,	R2	;j++
		ADD	R0,	#0x01
		LDR	R2,	R0
		JP	A1,	=forj
		forjend:
		
		MOV	R1,	#0x0C	;crc8table[i] = crc
		LDR	R0,	[R1]
		PUSH	R0
			MOV	R0,	#0x20
			ADD	R0,	R3
			LDR	R1,	R0
		POP	R0
		LDR	[R1],	R0
		
	LDR	R0,	R3	;i++
	ADD	R0,	#0x01
	LDR	R3,	R0
	JP	A1,	=fori
	foriend:
	POP	R2
	POP	R3
	RET
crc8Init:	;output sram 0x0C
	PUSH	R1
	MOV	R1,	#0x0C
	MOV	[R1],	#0x00
	POP	R1
	RET
crc8Update:	;input 1 byte date R0, output sram 0x0B
	PUSH	R0
	PUSH	R1
		MOV	R1,	#0x0C	;*pCrc8 = crc8table[pData[i] ^ *pCrc8];
		AND	R0,	#0xFF
		XOR	R0,	[R1]
		ADD	R0,	#0x20
		LDR	R1,	R0
		LDR	R0,	[R1]
		MOV	R1,	#0x0C
		LDR	[R1],	R0
	POP	R1
	POP	R0
	RET
crc8Fihshed:	;output SRAM 0x0C
	PUSH	R0
	PUSH	R1
		MOV	R0,	#0x00
		MOV	R1,	#0x0C
		XOR	R0,	[R1]
		LDR	[R1],	R0
	POP	R1
	POP	R0
	RET
```

CRC32.asm
```
crc32TableInit:	;output crc table start from sram 0x10, sram 0x0B wil over write
	MOV	R3,	#0x00		;for (i = 0; ;) {
	gennumber:
		MOV	R1,	#0x0B	;c = (unsigned int)i
		LDR	[R1],	R3
		MOV	R2,	#0x00	;for (j = 0; ;) {
		genbit:
			PUSH	R2
			MOV	R1,	#0x0B	;c' = c
			LDR	R0,	[R1]
			LDR	R2,	R0	;temp of c'
			AND	R0,	#0x01	;if ((c' & 1) == 0)
			JP	Z,	=genbit0
			JP	A1,	=genbit1
			genbit1:		;c' = 0xedb88320L ^ (c' >> 1)
				LDR	R0,	R2
				RSR	R0,	#0x01
				LDR	R2,	R0
				MOV	R0,	#0x7FFF ;bit0 not save
				RSL	R0,	#0x10
				OR	R0,	#0xFFFF
				AND	R0,	R2
				LDR	R2,	R0
				MOV	R0,	#0xEDB8
				RSL	R0,	#0x10
				OR	R0,	#0x8320
				XOR	R0,	R2
				JP	A1,	=genbitdone
			genbit0:		;c' = (c' >> 1)
				LDR	R0,	R2
				RSR	R0,	#0x01
				JP	A1,	=genbitdone
			genbitdone:
			MOV	R1,	#0x0B	;c = c'
			LDR	[R1],	R0
			POP	R2
		LDR	R0,	R2	;j < 8
		XOR	R0,	#0x07
		JP	Z,	=genNext
		LDR	R0,	R2	; j++
		ADD	R0,	#0x01
		LDR	R2,	R0
		JP	A1,	=genbit
		genNext:
		MOV	R1,	#0x0B	;crc_table[i] = c
		LDR	R2,	[R1]
		LDR	R0,	R3	;offset at 0x10
		ADD	R0,	#0x10
		LDR	R1,	R0
		LDR	[R1],	R2
	LDR	R0,	R3		;i < 256;
	XOR	R0,	#0xFF
	JP	Z,	=crc32TableInitDone
	LDR	R0,	R3		; i++
	ADD	R0,	#0x01
	LDR	R3,	R0
	JP	A1,	=gennumber
	crc32TableInitDone:
	RET
crc32Init:	;output sram 0x0B
	PUSH	R1
	PUSH	R0
	MOV	R0,	#0xFFFF
	RSL	R0,	#0x10
	OR	R0,	#0xFFFF
	MOV	R1,	#0x0B
	LDR	[R1],	R0
	POP	R0
	POP	R1
	RET
crc32Update:	;input byte date R0, output sram 0x0B
	PUSH	R3
	PUSH	R2
	PUSH	R1
		LDR	R2,	R0
		;R0 = (*pCrc32) & 0x000000FF
		MOV	R1,	#0x0B
		LDR	R0,	[R1]
		AND	R0,	#0x000000FF
		;R0 = (pData[i]) ^ ((*pCrc32) & 0x000000FF)
		XOR	R0,	R2
		;R3 = crc32tab[(pData[i]) ^ (*pCrc32) & 0x000000FF]
		ADD	R0,	#0x10	;table start address
		LDR	R1,	R0
		LDR	R3,	[R1]
		;R0 = (*pCrc32) >> 8
		MOV	R1,	#0x0B
		LDR	R0,	[R1]
		RSR	R0,	#0x08
		LDR	R2,	R0
		MOV	R0,	#0x00FF
		RSL	R0,	#0x10
		OR	R0,	#0xFFFF
		AND	R0,	R2
		;Store crc = R0 ^ R3
		XOR	R0,	R3
		MOV	R1,	#0x0B
		LDR	[R1],	R0
	POP	R1
	POP	R2
	POP	R3
	RET
crc32Fihshed:	;output SRAM 0x0B
	PUSH	R1
	PUSH	R0
	MOV	R0,	#0xFFFF
	RSL	R0,	#0x10
	OR	R0,	#0xFFFF
	MOV	R1,	#0x0B
	XOR	R0,	[R1]
	LDR	[R1],	R0
	POP	R0
	POP	R1
	RET
```

### References:
[Wikipedia](http://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks),
[HezhouW](https://github.com/HezhouW/CRC16_CCITT), 
[dtdns](http://www.macs123.dtdns.net/algo/cpp/cpp025.html),
[ocasas](https://github.com/ocasas/crc8),
[htmlxx](http://blog.csdn.net/htmlxx/article/details/17369105#comments),
[http://depa.usst.edu.cn/](http://depa.usst.edu.cn/chenjq/www2/software/crc/CRC_Javascript/CRCcalculation.htm),
[How to call c in swift](http://stackoverflow.com/questions/24004732/how-to-call-c-from-swift)


