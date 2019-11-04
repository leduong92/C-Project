#ifndef TPLCGPP_H
#define TPLCGPP_H
//================================================================================================
//
//	FileName	:TPlcGpp.H
//	Class		:TPlcGpp (SubClass of "Trs232C")
// 				:三菱シーケンサのＣＰＵポートを利用した通信
//	作成日		:1998.2.10	Coded by M.Ishida
//
//================================================================================================

#include "TRS232C.h"

#define READ_LEN_MAX	1024					// 一回に読み込み可能な最大バイト数

// 受信バッファのデフォルト値

// 
#define READ_DEVICES	0
#define WRITE_DEVICES	1

#define READ_WORD_DEVICE_MAXPOINT	128		// D/W ﾚｼﾞの一回に読み込み可能な最大点数
#define READ_WORD_DEVICE_MAXBYTE	512		// D/W ﾚｼﾞの一回に読み込み可能な最大バイト数
#define WORD_DEVICE_MAXPOINT		6144	// D/W ﾃﾞｰﾀﾚｼﾞｽﾀﾃﾞﾊﾞｲｽ点数



class TPlcGpp:public TRS232C
{
	private:
		HWND hwParent;		// ﾀﾞｲｱﾛｸﾞﾊﾝﾄﾞﾙ

		BOOL m_fOpen;		// ｵｰﾌﾟﾝ中=TRUE,close中=FALSE

		unsigned char CpuType;
	public:
	
		TPlcGpp( void );
		~TPlcGpp( void );

		BOOL Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);

		short	PortOpen(void);			// ﾎﾟｰﾄのｵｰﾌﾟﾝ
		short	PortOpenFx(HWND hwnd);		// ﾎﾟｰﾄのｵｰﾌﾟﾝ
		void	PortClose(void);			// ﾎﾟｰﾄのｸﾛｰｽﾞ
		short	SRead( char memory, short address, short dvno, char *data );
												// ﾜｰﾄﾞﾃﾞﾊﾞｲｽからのﾃﾞｰﾀ読み込み
		short	SWrite( char memory, short address,  char *data );
												// ﾜｰﾄﾞﾃﾞﾊﾞｲｽへのﾃﾞｰﾀ書き込み
		short	SWriteN( char memory, short address, short dvno, char *data );
												// ﾜｰﾄﾞﾃﾞﾊﾞｲｽへのﾃﾞｰﾀ書き込み
		unsigned char htoi( char *hexstr , short len );
		short	SumCheck( unsigned char *p, short theLen );
		short	SumCheckC( unsigned char *p, short theLen );
		short	MakeCodeFunction(short function, short dvno, unsigned char *code, short theLen);
		short	MakeCodeDevice(char memory, short address, short dvno, unsigned char *code, short theLen);

		int		Hex2Dec( char* hexstr, short len );
	protected:


};
#endif
