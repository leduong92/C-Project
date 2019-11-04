//========================================================================================================
//
//	FileName	:TPlcGpp.cpp
//	Class		:TPlcGpp (SubClass of "TRS232C")
// 				:三菱シーケンサＣＰＵポートでのコントロール
//	作成日		:1998. 9.21	Coded by Shiba
//	修正日		: 
//
//========================================================================================================
#include <stdio.h>
#include <conio.h>

//#include "globldef.h"
#include "TPlcGpp.h"

extern HWND			ghwnd;

//===========================================================================================
//	TPlcGpp::TPlcGpp(void) 
//	引数　：なし
//	返値　：
//　機能　：コンストラクタ
//===========================================================================================
TPlcGpp::TPlcGpp(void) 
{
	m_fOpen=FALSE;
	CpuType=0x00;
	// 通信オープン
//	PortOpen();
}


//===========================================================================================
//	TPlcGpp::~TPlcGpp()
//	引数　：なし
//	返値　：
//　機能　：デストラクタ
//===========================================================================================
TPlcGpp::~TPlcGpp()
{
	TRS232C::SetRTS(FALSE);
	TRS232C::SetDTR(FALSE);
	TRS232C::Close();

}
//========================================================================================================
//	void TPlcGpp::PortClose(void)
//	ポートクローズ
//	引数：なし
//	返値：なし
//========================================================================================================
void TPlcGpp::PortClose(void)
{
	TRS232C::SetRTS(FALSE);
	TRS232C::SetDTR(FALSE);
	TRS232C::Close();

	m_fOpen=FALSE;
	CpuType=0x00;
}
//========================================================================================================
//	BOOL TPlcGpp::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type　：private
//	引数１：ポート№
//	引数２：ボーレート 1200,2400,4800,9600,19200
//	引数２：データビット 7,8
//	引数２：ｽﾄｯﾌﾟﾋﾞｯﾄ 1,2
//	引数２：パリティ 0=なし、 1=奇数、2=偶数
//	引数２：ウエイト
//	返値　：TRUE or FALSE
//　機能　：ポートを開く
//========================================================================================================
BOOL TPlcGpp::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait)
{
	// ｵｰﾌﾟﾝ中はOK
	if( m_fOpen ) return(TRUE);

	//m_Wait = wait;

	PORTSETTINGEX ps;
	memset( &ps, 0, sizeof(PORTSETTINGEX));

	switch( baudrate ){
		case 1200:
			ps.BauRate=CBR_1200;				//1200bps
			break;
		case 2400:
			ps.BauRate=CBR_2400;				//1200bps
			break;
		case 4800:
			ps.BauRate=CBR_4800;				//1200bps
			break;
		case 19200:
			ps.BauRate=CBR_19200;				//1200bps
			break;
		case 9600:
		default:
			ps.BauRate=CBR_9600;				//1200bps
			break;
	}

	// DataBit
	switch( databit ){
		case 7:
			ps.DByte=7;						// データﾋﾞｯﾄ=8
			break;
		case 8:
		default:
			ps.DByte=8;						// データﾋﾞｯﾄ=8
			break;
	}

	// STOP BIT
	switch( stopbit){
		case 1:
			ps.StopBits=ONESTOPBIT;			// ｽﾄｯﾌﾟﾋﾞｯﾄ=1
			break;
		case 2:
		default:
			ps.StopBits=TWOSTOPBITS;			// ｽﾄｯﾌﾟﾋﾞｯﾄ=2
			break;
	}

	// PARITY
	switch( parity){
		case 1:
			ps.Parity=ODDPARITY;				// 奇数パリティ
			break;
		case 2:									// 偶数
			ps.Parity=EVENPARITY;				// 奇数パリティ
			break;
		case 0:
		default:
			ps.Parity=NOPARITY;				// パリティなし
			break;
	}
	ps.fBinary = TRUE;
	ps.fOutxCtsFlow = FALSE;	// TRUE or FALSE
	ps.fOutxDsrFlow = FALSE;	// TRUE or FALSE
	ps.fDtrControl = DTR_CONTROL_DISABLE;		// DTR_CONTROL_ENABLE or DTR_CONTROL_HANDSHAKE or DTR_CONTROL_HANDSHAKE
	ps.fRtsControl = RTS_CONTROL_ENABLE;		// RTS_CONTROL_HANDSHAKE or RTS_CONTROL_DISABLE or RTS_CONTROL_HANDSHAKE or RTS_CONTROL_TOGGLE


	if(!TRS232C::CommOpenEx((long)portNo,&ps)){
		return(FALSE);
	}

	TRS232C::SetRTS(TRUE);
	TRS232C::SetDTR(TRUE);

	// モデムのポートをクリア
	TRS232C::ClearRcvBuffer();
	m_fOpen=TRUE;

	return(TRUE);
}

// シーケンサー関係関数 ↓ ****************************************************


//-------------------------------------------------------------------------------------------
//   関数名：PortOpen
//   書　式：ret=PortOpen(void)
//   引き数：なし
//   返し値：short ret=0	:正常
//                 ret=1～6	:通信ｴﾗｰ
//   機　能：通信オープン処理
//-------------------------------------------------------------------------------------------
short TPlcGpp::PortOpen(void)
{
	DWORD theLength,nToRead;

    unsigned char buf[4],ReadBuf[32];
	int InQue;
//char msg[256];

	RTS_Off();							// 送信前 ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();

	SleepEx(500L,TRUE);

	buf[0]=0xAA;		//ｺｰﾄﾞ送信  0xAA
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(1);

	SleepEx(14L,TRUE);	
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();

	ClearRcvBuffer();

    buf[0]=0xB0;		//ｺｰﾄﾞ送信  0xB0
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(2);

	SleepEx(14L,TRUE);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0xAA;		//ｺｰﾄﾞ送信  0xAA
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(3);

	SleepEx(14L,TRUE);	
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();

	buf[0]=0xB2;		//ｺｰﾄﾞ送信  0xB2
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(4);

	SleepEx(14L,TRUE);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();
	SleepEx(100L,TRUE);


//	TRS232C::Status();
	InQue = TRS232C::ReadLength();			// 受信ﾊﾞｲﾄ数
//	printf("\nInQue=%d\n",InQue);
//sprintf(msg,"InQue=%d",InQue);
//MessageBox(ghwnd,msg,MSGTITLE, MB_OK);

	nToRead=InQue;
	if( !TRS232C::Read((BYTE*)ReadBuf, nToRead) ) return(5);
//	for(int i=0 ;i< (int)nToRead;i++) printf("%02x:",ReadBuf[i]);
//sprintf(msg,"%02x:%02x:%02x:%02x",ReadBuf[0],ReadBuf[1],ReadBuf[2],ReadBuf[3]);
//MessageBox(ghwnd,msg,MSGTITLE, MB_OK);

	if( ReadBuf[0]!=0xB0 ) return(6);

	CpuType=ReadBuf[1];
//sprintf(msg,"%02x",CpuType);
//MessageBox(ghwnd,msg,MSGTITLE, MB_OK);

	return(0);

}

//-------------------------------------------------------------------------------------------
//   関数名：PortOpenFx
//   書　式：ret=PortOpenFx(void)
//   引き数：なし
//   返し値：short ret=0	:正常
//                 ret=1～6	:通信ｴﾗｰ
//   機　能：通信オープン処理
//-------------------------------------------------------------------------------------------
short TPlcGpp::PortOpenFx(HWND hwnd)
{
	DWORD theLength,nToRead;

    unsigned char buf[4],ReadBuf[32];
	int InQue;

	// ウインドウハンドルの代入
	hwParent = hwnd;

	RTS_Off();							// 送信前 ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();

	SleepEx(500L,TRUE);

	buf[0]=0x82;		//ｺｰﾄﾞ送信  0x82
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(1);

	SleepEx(14L,TRUE);	
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();

    buf[0]=0x30;		//ｺｰﾄﾞ送信  0x30
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(2);

	SleepEx(14L,TRUE);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0x30;		//ｺｰﾄﾞ送信  0x30
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(3);

	SleepEx(14L,TRUE);	
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();

    buf[0]=0xC5;		//ｺｰﾄﾞ送信  0xC5
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(4);

	SleepEx(14L,TRUE);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0x30;		//ｺｰﾄﾞ送信  0x30
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(5);

	SleepEx(14L,TRUE);	
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();

    buf[0]=0xB2;		//ｺｰﾄﾞ送信  0xB2
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(6);

	SleepEx(14L,TRUE);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0x30;		//ｺｰﾄﾞ送信  0x30
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(7);

	SleepEx(14L,TRUE);	
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();

    buf[0]=0xB2;		//ｺｰﾄﾞ送信  0xB2
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(8);

	SleepEx(14L,TRUE);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0x03;		//ｺｰﾄﾞ送信  0x03
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(9);

	SleepEx(14L,TRUE);	
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();

    buf[0]=0x36;		//ｺｰﾄﾞ送信  0x36
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(10);

	SleepEx(14L,TRUE);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0xC3;		//ｺｰﾄﾞ送信  0xC3
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(11);

	SleepEx(14L,TRUE);	
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();
/*
	buf[0]=0xB2;		//ｺｰﾄﾞ送信  0xB2
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(12);
*/
	SleepEx(14L,TRUE);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();
	SleepEx(30L,TRUE);


//	TRS232C::Status();
	InQue = TRS232C::ReadLength();			// 受信ﾊﾞｲﾄ数
//	printf("\nInQue=%d\n",InQue);

	nToRead=InQue;
	if( !TRS232C::Read((BYTE*)ReadBuf, nToRead) ) return(13);
//	for(int i=0 ;i< (int)nToRead;i++) printf("%02x:",ReadBuf[i]);
		char buf2[256];
		wsprintf(buf2,"%d",nToRead);
		//wsprintf(buf2,"%02X",ReadBuf[11]);
		MessageBox(hwParent,buf2,"SEQUENCER-PC",MB_OK|MB_ICONEXCLAMATION);

//	if( ReadBuf[0]!=0xB0 ) return(14);

	CpuType=ReadBuf[5];

	return(0);

}


//-------------------------------------------------------------------------------------------
//   関数名：SRead
//   書　式：ret=SRead( char memory, short address, short dvno, char *data )
//   引き数：char  memory		:ﾃﾞｨﾊﾞｲｽ名				:'Ｄ'
//           short address		:先頭ﾃﾞｨﾊﾞｲｽｱﾄﾞﾚｽ		:0～1023
//           short dvno			:ﾃﾞｨﾊﾞｲｽ 点数			:1～128まで
//           char  *data		:読みだしﾃﾞｰﾀ格納ﾊﾞｯﾌｧｰ	:確保してね
//   返し値：short ret=ﾃﾞｰﾀ長	:正常
//           short ret=1		:ｻﾑﾁｪｯｸｴﾗｰ
//   機　能：ＭＥＭＯＲＹとＡＤＲＥＳＳで指定した先頭ﾃﾞｨﾊﾞｲｽから、ＤＶＮＯ
//           で指定したﾃﾞｨﾊﾞｲｽ点数分のﾃﾞｰﾀを読み込みdataに格納する
//           SRead('D',0,10,buf);  D0000からD0009の読み出し
//           D0=H0001 D2=H0005 のとき　buf[]="00010005......と入る。
//-------------------------------------------------------------------------------------------
short TPlcGpp::SRead( char memory, short address, short dvno, char *data )
{
	DWORD theLength,nToRead;

    unsigned char buf[512],ReadBuf[READ_LEN_MAX+1];
	short i,j,hex;
	int InQue,pnbyte;

	RTS_Off();							// 送信前 ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();

	buf[0]=CpuType;								// CPUﾀｲﾌﾟ
	theLength=1;
	TRS232C::Write( (BYTE*)buf, theLength );

	Sleep(14);
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();

	// 伝送ｺｰﾄﾞの作成
	MakeCodeFunction(READ_DEVICES, 0, buf, 0);	// ｺﾏﾝﾄﾞ
	MakeCodeDevice(memory,address,dvno,buf,4);	// ﾃﾞﾊﾞｲｽ
	SumCheck(buf,8);							// ｻﾑﾁｪｯｸ

// @
//	for( i=0; i<9; i++ ) printf(":%02X:",buf[i]);printf("\n");

	// ｺｰﾄﾞ送信
	theLength=9;
	TRS232C::Write( (BYTE*)buf, theLength );

	// 受信待ち時間
	Sleep(500);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();

// @
//	TRS232C::Status();

	// ﾃﾞｰﾀの取得
	j=0;
	InQue=TRS232C::ReadLength();

// @
//	printf("InQue=%d\n",InQue);
	if( InQue > 0 )
	{
		nToRead=InQue;
		pnbyte = TRS232C::Read( (BYTE*)ReadBuf, nToRead );

		if( SumCheckC( ReadBuf, pnbyte ) ) return(1);
// @
//		printf("pnbyte=%d\n",pnbyte);
//		int tt = TRS232C::ReadLength();
//		printf("buflen=%d\n",tt);
//		for( i=0; i<pnbyte; i++ ) printf(":%02X:",ReadBuf[i]);printf("\n");

		for( i=4; i<pnbyte-1; i+=2 )
		{
			hex=((ReadBuf[i+1] & 0xF0)  >> 4);if(hex >= 0x0A) data[j++]=hex+'7';else data[j++]=hex+'0';
			hex=((ReadBuf[i+1] & 0x0F)		);if(hex >= 0x0A) data[j++]=hex+'7';else data[j++]=hex+'0';
			hex=((ReadBuf[i]   & 0xF0)  >> 4);if(hex >= 0x0A) data[j++]=hex+'7';else data[j++]=hex+'0';
			hex=((ReadBuf[i]   & 0x0F)		);if(hex >= 0x0A) data[j++]=hex+'7';else data[j++]=hex+'0';
		}
	}
	else return(InQue);

	return((pnbyte-5)*2);
}


//-------------------------------------------------------------------------------------------
//   関数名：SWrite
//   書　式：ret=SWrite( char memory, short address, char *data )
//   引き数：char  memory		:ﾃﾞｨﾊﾞｲｽ			:'Ｄ'
//           short address		:ﾃﾞｨﾊﾞｲｽｱﾄﾞﾚｽ		:０から１０２３
//           char  *data		:書き込みﾃﾞｰﾀ
//   返し値：short ret=0		:正常
//           short ret=1		:ｴﾗｰ
//   機　能：ＭＥＭＯＲＹとＡＤＲＥＳＳで指定したﾃﾞﾊﾞｲｽ番号にﾃﾞｰﾀdataを書き込む
//           SWrite('D',0,buf);  D0000に書き込む
//           D0=H0001 と書き込むとき　buf[]="0001"と入れる。
//-------------------------------------------------------------------------------------------
short TPlcGpp::SWrite( char memory, short address, char *data)
{
	DWORD theLength,nToRead;

    unsigned char buf[512],ReadBuf[512],work1,work2;
	int byteswritten;
	int InQue,pnbyte;

	RTS_Off();							// 送信前 ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();

	buf[0]=CpuType;								// CPUﾀｲﾌﾟ
	theLength=1;
	TRS232C::Write( (BYTE*)buf, theLength );

	Sleep(14);
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();

	// 伝送ｺｰﾄﾞの作成
	MakeCodeFunction(WRITE_DEVICES, 1, buf, 0);	// ｺﾏﾝﾄﾞ
	MakeCodeDevice(memory,address,1,buf,4);		// ﾃﾞﾊﾞｲｽ
	work1=htoi(&data[0],2);
	work2=htoi(&data[2],2);
	buf[8]=work2;
	buf[9]=work1;
	SumCheck(buf,10);							// ｻﾑﾁｪｯｸ

	// ｺｰﾄﾞ送信
	theLength=11;
	byteswritten=TRS232C::Write( (BYTE*)buf, theLength );
//	for( int i=0; i<11; i++ ) printf(":%02X:",buf[i]);printf("\n");
	if( byteswritten!=11 ) return(1);

	Sleep(100);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();

	// 受信ﾊﾞｯﾌｧのｸﾘｱ
	InQue=TRS232C::ReadLength();
	if( InQue > 0 )
	{
		nToRead=InQue;
		pnbyte = TRS232C::Read( (BYTE*)ReadBuf, nToRead );
	}
	if( pnbyte!=5 ) return(1);

	return(0);
}


//-------------------------------------------------------------------------------------------
//   関数名：SWriteN
//   書　式：ret=SWriteN( char memory, short address, char *data )
//   引き数：char  memory		:ﾃﾞｨﾊﾞｲｽ			:'Ｄ'
//           short address		:先頭ﾃﾞｨﾊﾞｲｽｱﾄﾞﾚｽ	:０から１０２３
//           short dvno			:ﾃﾞｨﾊﾞｲｽ 点数		:１から１２８まで
//           char  *data		:書き込みﾃﾞｰﾀ
//   返し値：short ret=0		:正常
//           short ret=1		:ｴﾗｰ
//   機　能：ＭＥＭＯＲＹとＡＤＲＥＳＳで指定したﾃﾞﾊﾞｲｽ番号から、ＤＶＮＯ
//           で指定したﾃﾞｨﾊﾞｲｽ点数分、ﾃﾞｰﾀdataを書き込む
//           SWrite('D',0,10,buf);  D0000からD0009に書き込む
//           D0=H0001 D2=H0005 ...と書き込むとき　buf[]="00010005..."と入れる。
//-------------------------------------------------------------------------------------------
short TPlcGpp::SWriteN( char memory, short address, short dvno, char *data )
{
	DWORD theLength,nToRead;

    unsigned char buf[512],ReadBuf[512],work1,work2;
	int i;
	int byteswritten;
	int InQue,pnbyte;
	short theLen;

	RTS_Off();							// 送信前 ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();

	buf[0]=CpuType;									// CPUﾀｲﾌﾟ
	theLength=1;
	TRS232C::Write( (BYTE*)buf, theLength );

	Sleep(14);
	RTS_On();							// ＲＴＳ・ＤＴＲ 共にオン
	DTR_On();

	// 伝送ｺｰﾄﾞの作成
	MakeCodeFunction(WRITE_DEVICES, dvno, buf, 0);	// ｺﾏﾝﾄﾞ
	MakeCodeDevice(memory,address,dvno,buf,4);		// ﾃﾞﾊﾞｲｽ
	theLen=8;
	for( i=0; i<dvno*4; i+=4 )
	{
		work1=htoi(&data[i],2);
		work2=htoi(&data[i+2],2);
		buf[theLen++]=work2;
		buf[theLen++]=work1;
	}
	SumCheck(buf,theLen);							// ｻﾑﾁｪｯｸ

	// ｺｰﾄﾞ送信
	theLength=theLen+1;
	byteswritten=TRS232C::Write( (BYTE*)buf, theLength );
//	for( i=0; i<11; i++ ) printf(":%02X:",buf[i]);printf("\n");
	if( byteswritten!=theLen+1 ) return(1);

	Sleep(100);
	RTS_Off();							// ＲＴＳ・ＤＴＲ 共にオフ
	DTR_Off();

	// 受信ﾊﾞｯﾌｧのｸﾘｱ
	InQue=TRS232C::ReadLength();
	if( InQue > 0 )
	{
		nToRead=InQue;
		pnbyte = TRS232C::Read( (BYTE*)ReadBuf, nToRead );
	}
	if( pnbyte!=5 ) return(1);

	return(0);
}
//========================================================================================================
//	int TPlcGpp::Hex2Dec( char* hexstr, short len )
//	機能　：16進（文字列４文字のみ）を整数値(int型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
int TPlcGpp::Hex2Dec( char* hexstr, short len )
{
	int	ret;
	int hi,lo;

	if( len!=4 ) return(-1);
	len = strlen(hexstr);

	hi= htoi( &hexstr[0],2);
	lo= htoi( &hexstr[2],2);

	ret= hi*0x100 + lo;

	return(ret);
}

//-------------------------------------------------------------------------------------------
//	関数名：short htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//-------------------------------------------------------------------------------------------
unsigned char TPlcGpp::htoi( char *hexstr , short len )
{
	unsigned char ret = 0;
	short i;

	if( len > 4 ) return 0;
	for( i = 0 ; i < len ; i++ ){
		ret *= 16;
		if( hexstr[i] < 'A' ) ret += hexstr[i] - '0';
		else
		if( hexstr[i] < 'a' ) ret += hexstr[i] - 'A'+10;
		else
			ret += hexstr[i] - 'a'+10;
	}
	return(ret);
}


//-------------------------------------------------------------------------------------------
//   関数名：SumCheck
//   書　式：SumCheck( unsigned char *p, short theLen )
//   引き数：unsigned char *p	:ｻﾑﾁｪｯｸを付加する文字列
//           short theLen		:文字列の長さ
//   返し値：０：正常付加  １：異常終了
//   機　能：ｻﾑﾁｪｯｸｺｰﾄﾞを文字列最後に付加する
//           ＢＩＮﾃﾞｰﾀとして加算した結果の下位１ﾊﾞｲﾄをＡＳＣＩＩ２桁に変換
//           してﾃﾞｰﾀの最後に付加する
//-------------------------------------------------------------------------------------------
short TPlcGpp::SumCheck( unsigned char *p, short theLen )
{
	int i,sum=0;

	for(i=0;i<theLen;i++)
	{
		sum = sum + p[i];
	}

	sum = sum - (sum/256)*256;

	p[theLen]=(unsigned char)sum;

	return(0);
}


//-------------------------------------------------------------------------------------------
//   関数名：SumCheckC
//   書　式：ret=SumCheckC( char *p, short theLen )
//   引き数：char *p:ｻﾑﾁｪｯｸをする文字列
//   返し値：short ret=0:正常
//                 ret=1:ｻﾑﾁｪｯｸｴﾗ-
//   機　能：ｻﾑﾁｪｯｸする
//-------------------------------------------------------------------------------------------
short TPlcGpp::SumCheckC( unsigned char *p, short theLen )
{
	int i,sum=0;

	for(i=0;i<theLen-1;i++)
	{
		sum = sum + p[i];
	}

	sum = sum - (sum/256)*256;

	if( p[theLen-1] != sum ) return(1);

	return(0);
}

//-------------------------------------------------------------------------------------------
//   関数名：MakeCodeFunction
//   書　式：ret=MakeCodeFunction(short function, short dvno, unsigned char *code, short theLen)
//   引き数：short function		:機能
//           unsigned char *code:送信ﾃﾞｰﾀ格納ﾊﾞｯﾌｧｰ
//           short theLen		:文字列の長さ
//   返し値：short ret=0		:正常
//           short ret=1		:ｴﾗｰ
//   機　能：ＰＬＣ番号と、ＦＵＮＣＴＩＯＮで指定した機能を示す
//           伝送ﾃﾞｰﾀをＣＯＤＥに付加する
//-------------------------------------------------------------------------------------------
short TPlcGpp::MakeCodeFunction(short function, short dvno, unsigned char *code, short theLen)
{
	unsigned short n;

	if( dvno<1 || dvno>128 ) dvno=1;

	switch(function){
		case READ_DEVICES:						// Read Devices
			code[theLen]=0x07;
			code[theLen+1]=0x00;
			code[theLen+2]=0xFF;// PLC番号
			code[theLen+3]=0x02;// 読み出し
			break;
		case WRITE_DEVICES:						// Write Devices
			n = 7 + dvno*2;
			code[theLen] = n%256;
			code[theLen+1]=n/256;
			code[theLen+2]=0xFF;// PLC番号
			code[theLen+3]=0x04;// 書き込み
			break;
		default:
			return(1);
	}
	return(0);
}

//-------------------------------------------------------------------------------------------
//   関数名：MakeCodeDevice
//   書　式：ret=MakeCodeDevice(short memory,short address,short dvno,unsigned char *code,short theLen)
//   引き数：short memory		:ﾃﾞｨﾊﾞｲｽ
//           short address		:先頭ﾃﾞｨﾊﾞｲｽｱﾄﾞﾚｽ
//           unsigned char *code:送信ﾃﾞｰﾀ格納ﾊﾞｯﾌｧｰ
//           short theLen		:文字列の長さ
//   返し値：short ret=0		:正常
//           short ret=1		:ｴﾗｰ
//   機　能：ＭＥＭＯＲＹとＡＤＲＥＳＳで指定した先頭ﾃﾞｨﾊﾞｲｽと、ＤＶＮＯで
//           指定したﾃﾞｨﾊﾞｲｽ点数を示す伝送ﾃﾞｰﾀをＣＯＤＥに付加する
//-------------------------------------------------------------------------------------------
short TPlcGpp::MakeCodeDevice(char memory, short address, short dvno, unsigned char *code, short theLen)
{
	unsigned short n;
	code[theLen+2]= 0x00;

	switch(memory){
		case 0x58:											// Ｘ-Input
			if( address<0 || address>2047 ) address=0;
			n = 0x8000 + (address/16)*2;
			break;
		case 0x59:											// Ｙ-Output
			if( address<0 || address>2047 ) address=0;
			n = 0x8200 + (address/16)*2;
			break;
		case 0x4D:											// Ｍ-Internal Relay
			if( address<0 || address>2047 ) address=0;
			n = 0x8400 + (address/16)*2;
			break;
		case 0x4C:											// Ｌ-Latch Relay
			if( address<0 || address>2047 ) address=0;
			n = 0x8400 + (address/16)*2;
			break;
		case 0x44:										// Ｄ-Data Register
			if( address<0 || address>=WORD_DEVICE_MAXPOINT ) address=0;
//			n = 0x8800 + address*2;
			n = 0x4000 + address*2;
			code[theLen+2]= 0x08;
			// 0x4000を使用してcode[theLen+2]=0x08を入れる。とaddressが6143まで使える
			break;
		case 0x57:										// Ｗ-Link Register
			if( address<0 || address>=WORD_DEVICE_MAXPOINT ) address=0;
			n = 0x9000 + address*2;
			break;
		default:
			return(1);
	}

	if( dvno<1 || dvno>128 ) dvno=1;

	code[theLen] = n%256;
	code[theLen+1]=n/256;
//	code[theLen+2]= 0x00;
	code[theLen+3]=(dvno * 2)%256;

	return(0);
}
