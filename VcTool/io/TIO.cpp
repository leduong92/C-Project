//===================================================================================================
//	TIO.cpp
//
//		IO Class
//		SuperClass = None
//	
//		Copyright  1997 Nok Corporation.
//			All rights reserved.
//			Coded by T.Hiraguchi
//		Modify	:1999.12.23 coba IOポート指定コンストラクタの追加
//				:1999.12.24 coba ブロックリード関数を追加
//===================================================================================================
 
#include "TIo.h"
#include <conio.h>
#include <stdio.h>
#include <math.h>
/*** Global Variables ***/
 
TIO::TIO()
{
	IsIO=0;//存在しない
	IOPORT=DEFAULT_PORT_ADDRESS;
	IO_setup();
}

TIO::TIO(int addr)
{
	IsIO=0;//存在しない
	IOPORT=addr;
	IO_setup();
}

TIO::~TIO()
{


}
void TIO::IO_setup()
{
	if(IsIO == 1) return;
		port1=0;
		port2=0;
		_outp(IOPORT,port1);
		_outp(IOPORT+1,port2);
		_outp(IOPORT,255);
		_outp(IOPORT+1,128);

}

//========================================================================================================
//	BOOL TIO::ScanAll(IOSTATUS_16* stat);
//　機能　：ポート一括読み出し
//	引数　：結果出力用バッファ
//	戻り値：TRUE=取得成功。FALSE=取得失敗(ポート未初期化)
/*			↓の構造体の配列に、対応する点のｽﾃｰﾀｽを書き込む 0=OFF, 1=ON
			typedef struct{
				short pinstat[16];
			} IOSTATUS_16;
*/
//========================================================================================================
BOOL TIO::ScanAll(IOSTATUS_16 *statbuffer)
{
	if(IsIO == 1) return(FALSE);

	int i;
	short data;
	IOSTATUS_16 stat16;

	memset( &stat16, 0, sizeof(IOSTATUS_16));

	// 0-7pin
	data = _inp(IOPORT);
	for(i=0; i<8; i++){
		stat16.pinstat[i]= data & (short)pow(2,i) ? 1:0;
		statbuffer->pinstat[i] = stat16.pinstat[i]; 
	}

	// 8-15pin
	data = _inp(IOPORT+1);
	for(i=0; i<8; i++){
		stat16.pinstat[i+8]= data & (short)pow(2,i) ? 1:0;
		statbuffer->pinstat[i+8] = stat16.pinstat[i+8]; 
	}
//	memcpy( &statbuffer, &stat16, sizeof(IOSTATUS_16));


	return(TRUE);
}

short	TIO::IO_In0_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);

		ret = _inp(IOPORT);
		ret = ret & 0x01;
		return(ret);
}

short	TIO::IO_In1_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);

		ret = _inp(IOPORT);
		ret = ret & 0x02;
		ret = ret>>1;

		return(ret);
}

short	TIO::IO_In2_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);

		ret = _inp(IOPORT);
		ret = ret & 0x04;
		ret = ret>>2;

		return(ret);
}

short	TIO::IO_In3_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT);
		ret = ret & 0x08;
		ret = ret>>3;

		return(ret);
}

short	TIO::IO_In4_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT);
		ret = ret & 0x10;
		ret = ret>>4;
		return(ret);
}

short	TIO::IO_In5_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT);
		ret = ret & 0x20;
		ret = ret>>5;
		return(ret);
}

short	TIO::IO_In6_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT);
		ret = ret & 0x40;
		ret = ret>>6;
		return(ret);
}

short	TIO::IO_In7_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT);
		ret = ret & 0x80;
		ret = ret>>7;
		return(ret);
}
short	TIO::IO_In8_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT+1);
		ret = ret & 0x01;
		return(ret);
}

short	TIO::IO_In9_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT+1);
		ret = ret & 0x02;
		ret = ret>>1;
		return(ret);
}

short	TIO::IO_In10_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT+1);
		ret = ret & 0x04;
		ret = ret>>2;
		return(ret);
}

short	TIO::IO_In11_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT+1);
		ret = ret & 0x08;
		ret = ret>>3;
		return(ret);
}

short	TIO::IO_In12_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT+1);
		ret = ret & 0x10;
		ret = ret>>4;
		return(ret);
}

short	TIO::IO_In13_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT+1);
		ret = ret & 0x20;
		ret = ret>>5;
		return(ret);
}

short	TIO::IO_In14_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT+1);
		ret = ret & 0x40;
		ret = ret>>6;
		return(ret);
}

short	TIO::IO_In15_Stat()
{
		short ret;
		if(IsIO == 1) return(-1);
		ret = _inp(IOPORT+1);
		ret = ret & 0x80;
		ret = ret>>7;
		return(ret);
}

void	TIO::IO_Out0_Off()
{
		port1=port1 | 0x01;
		if(IsIO == 1) return;
		_outp(IOPORT,port1);
	
}

void	TIO::IO_Out1_Off()
{
		port1=port1 | 0x02;
		if(IsIO == 1) return;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out2_Off()
{
		port1=port1 | 0x04;
		if(IsIO == 1) return;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out3_Off()
{
		port1=port1 | 0x08;
		if(IsIO == 1) return;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out4_Off()
{
		port1=port1 | 0x10;
		if(IsIO == 1) return;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out5_Off()
{
		port1=port1 | 0x20;
		if(IsIO == 1) return;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out6_Off()
{
		port1=port1 | 0x40;
		if(IsIO == 1) return;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out7_Off()
{
		port1=port1 | 0x80;
		if(IsIO == 1) return;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out8_Off()
{
		port2=port2 | 0x01;
		if(IsIO == 1) return;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out9_Off()
{
		port2=port2 | 0x02;
		if(IsIO == 1) return;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out10_Off()
{
		port2=port2 | 0x04;
		if(IsIO == 1) return;
		_outp(IOPORT+1,port2);
}
				
void	TIO::IO_Out11_Off()
{
		port2=port2 | 0x08;
		if(IsIO == 1) return;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out12_Off()
{
		port2=port2 | 0x10;
		if(IsIO == 1) return;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out13_Off()
{
		port2=port2 | 0x20;
		if(IsIO == 1) return;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out14_Off()
{
		if(IsIO == 1) return;
		port2=port2 | 0x40;
		_outp(IOPORT+1,port2);
}
				
void	TIO::IO_Out15_Off()
{
		if(IsIO == 1) return;
		port2=port2 | 0x80;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out0_On()
{
		if(IsIO == 1) return;
		port1=port1 & 0xFE;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out1_On()
{	
		if(IsIO == 1) return;
		port1=port1 & 0xFD;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out2_On()
{
		if(IsIO == 1) return;
		port1=port1 & 0xFB;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out3_On()
{
		if(IsIO == 1) return;
		port1=port1 & 0xF7;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out4_On()
{
		if(IsIO == 1) return;
		port1=port1 & 0xEF;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out5_On()
{
		if(IsIO == 1) return;
		port1=port1 & 0xDF;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out6_On()
{
		if(IsIO == 1) return;
		port1=port1 & 0xBF;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out7_On()
{
		if(IsIO == 1) return;
		port1=port1 & 0x7F;
		_outp(IOPORT,port1);
}

void	TIO::IO_Out8_On()
{
		if(IsIO == 1) return;
		port2=port2 & 0xFE;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out9_On()
{
		if(IsIO == 1) return;
		port2=port2 & 0xFD;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out10_On()
{
		if(IsIO == 1) return;
		port2=port2 & 0xFB;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out11_On()
{
		if(IsIO == 1) return;
		port2=port2 & 0xF7;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out12_On()
{
		if(IsIO == 1) return;
		port2=port2 & 0xEF;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out13_On()
{
		if(IsIO == 1) return;
		port2=port2 & 0xDF;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out14_On()
{
		if(IsIO == 1) return;
		port2=port2 & 0xBF;
		_outp(IOPORT+1,port2);
}

void	TIO::IO_Out15_On()
{
		if(IsIO == 1) return;
		port2=port2 & 0x7F;
		_outp(IOPORT+1,port2);
}

void    TIO::IO_Out_Port2(short port2)
{				
		if(IsIO == 1) return;
		if(port2 >= 0 && port2 < 256)
		{
			port2=port2 ^ 0xFF;
			_outp(IOPORT+1,port2);
		}
}
short	TIO::IO_Out_Stat()
{
		return(0);
}

short	TIO::BoardCheck()
{
		return(0);
}

