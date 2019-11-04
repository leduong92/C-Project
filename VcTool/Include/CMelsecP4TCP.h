#ifndef COM_PLC4E_TCP_H
#define COM_PLC4E_TCP_H
//================================================================================================
//	FileName	:CMelsecP4TCP.h
//	Class		:CMelsecP4TCP
// 				:Melsec プロトコル４形式通信onTCP/IP
//				 Serial-Ethernetコンバータのサーバモード用に、通信内容をTCPに乗せて送る
//	sumチェック有り、xon/off制御なし。
//	作成日		:2013.09.20 coba
//================================================================================================
//	修正
//================================================================================================
#include <winsock2.h>
#include <windows.h>

// 制御コード定義
#ifndef CR
	#define CR 0x0d
#endif
#ifndef LF
	#define LF 0x0a
#endif
#ifndef SOH
	#define SOH 0x01
#endif
#ifndef STX
	#define STX 0x02
#endif
#ifndef ETX
	#define ETX 0x03
#endif
#ifndef EOT
	#define EOT 0x04
#endif
#ifndef ENQ
	#define ENQ 0x05
#endif
#ifndef ACK
	#define ACK 0x06
#endif
#ifndef NAK
	#define NAK 0x15
#endif


// アドレース＋データ部以外の送信ヘッダ長 ENQ(1)+ST(2)+PC(2)+CMD(2)+WAIT(1)+SUM(2)+デバイス数(2)+CRLF(2)
const int P4_HEADLEN=14;

class CMelsecP4TCP
{
	public:
		CMelsecP4TCP( void );
		~CMelsecP4TCP( void );

		BOOL Init(int wait);

		// 指定数のワードデバイス読み出し&書き込み
		int P4WriteW( char* ip ,unsigned short port,char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadW( char* ip ,unsigned short port,char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// 1点のビットデバイス読み出し&書き込み
		int P4WriteB( char* ip ,unsigned short port,char* station, char* pcstation,char* addr, int onoff);
		int P4ReadB( char* ip ,unsigned short port,char* station,char* pcstation,char* addr);

		// 多点のビットデバイス読み出し&書き込み
		int P4ReadBB( char* ip ,unsigned short port,char* buf,int bufsize,char* station,char* pcstation,char* addr,short counts);
		int P4WriteBB( char* ip ,unsigned short port,char* station, char* pcstation,char* addr, int cnt,char* dat); // 2012.06.20 coba

		// 指定数のワードデバイス読み出し&書き込み(2Cモード)
		int P2C4WriteW( char* ip ,unsigned short port,char* station, char* pcstation,char* addr, short counts, char* msg);
		int P2C4ReadW( char* ip ,unsigned short port,char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);


		// 文字列変換
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);
		int UStr2Mel(char* buf, unsigned int bufsize, unsigned char* str);

		// 上位下位のワード入れ替え(BCD８桁の変換)
		BOOL ULChg( char* buf );

	protected:

	private:
		BOOL m_fOpen;					// ｵｰﾌﾟﾝ中=TRUE,close中=FALSE
		int m_Wait;

		// sumチェックの計算
		int GetCheckSum(char* str );

		// コマンドの作成
		void MakeCmd(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg);
		void MakeCmd2C(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg);

		// 内部で使う送受信関数
		int TcpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize);
		int Answer( char* ip ,unsigned short port,char flg, char* station,char* pc );

		// 送受信バッファ
		char m_buf[4096];

		unsigned char htoi( char *hexstr , short len );

};
#endif
