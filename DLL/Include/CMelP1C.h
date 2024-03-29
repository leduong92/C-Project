#ifndef COM_PLC1C_H
#define COM_PLC1C_H

#include "TRS232C.h"


const int P4_HEADLEN=14;

class CMelsecP1C:public TRS232C
{
	public:
		CMelsecP1C( void );
		~CMelsecP1C( void );

		BOOL Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);

		int P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// ファイルレジスタ（Ａ互換、バンク指定書き込み"EW")

		// 指定数のワードデバイス読み出し&書き込み(2Cモード)
		int P4Send( char* station, char* pcstation,char* command,char* addr, short counts, char* msg);

		// 1点のビットデバイス読み出し&書き込み
		int P4WriteB( char* station, char* pcstation,char* addr, int onoff);
		int P4ReadB( char* station,char* pcstation,char* addr);

		// 文字列変換
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);

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

		// 内部で使う送受信関数
		int Receive( char* buf, int bufsize, int waitlen );
		int Answer( char flg, char* station,char* pc );

		// 送受信バッファ
		char m_buf[4096];

		unsigned char htoi( char *hexstr , short len );

};
#endif
