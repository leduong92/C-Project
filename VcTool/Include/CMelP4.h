#ifndef COM_PLC4_H
#define COM_PLC4_H
//================================================================================================
//
//	FileName	:CMelP4.h
//	Class		:CMelsecP4
// 				:Melsec プロトコル４形式通信
//	sumチェック有り、xon/off制御なし。
//	作成日		:2001.12.13	Coded by coba
//	TRS232Cのサブクラスになっていますので、そちらも加えてください
//================================================================================================
//	修正
//	2003.09.04 coba 拡張ファイルレジスタへの書き込み関数を追加
//					QnAの簡易フレーム通信を追加（前田さんのFIX）
//	2004.07.13 coba  waitの指定方法のミスを修正
//	2004.07.15 coba Close()を追加
//	2011.03.11 coba Str2Mes()のバグ(SJISが送れない)を修正
//	2013.04.11 coba 1Cプロトコルの拡張コマンドQR,QWに対応(AnSで、D1024以降をアクセスする場合に使用)
//	2013.04.11 coba リトライ回数overでも正常終了を返すバグ修正
//	2014.07.24 coba ビットデバイスのブロック読み書きを追加
//================================================================================================

#include "TRS232C.h"


// アドレース＋データ部以外の送信ヘッダ長 ENQ(1)+ST(2)+PC(2)+CMD(2)+WAIT(1)+SUM(2)+デバイス数(2)+CRLF(2)
const int P4_HEADLEN=14;

class CMelsecP4:public TRS232C
{
	public:
		CMelsecP4( void );
		~CMelsecP4( void );

		BOOL Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
		void Close(void);

		// 指定数のワードデバイス読み出し&書き込み
		int P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// 指定数の拡張ワードデバイス読み出し&書き込み
		int P4WriteWEx( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadWEx( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// 指定数の拡張ファイルレジスタ読み出し&書き込み
		int P4WriteREx( char* station, char* pcstation,char* addr, short counts, char* msg);

		// 指定数のワードデバイス読み出し&書き込み(2Cモード)
		int P2C4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P2C4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// 1点のビットデバイス読み出し&書き込み
		int P4WriteB( char* station, char* pcstation,char* addr, int onoff);
		int P4ReadB( char* station,char* pcstation,char* addr);

		// 多点のビットデバイス読み出し&書き込み 2014.07.24
		int P4ReadBB( char* buf,int bufsize,char* station,char* pcstation,char* addr,short counts);
		int P4WriteBB( char* station, char* pcstation,char* addr, int cnt,char* dat);
		// 文字列変換
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);

/*
		// 1ビットデバイスの読み出し&書き込み
		int P4WriteB( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadB( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);
*/

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
		int Receive( char* buf, int bufsize, int waitlen );
		int Answer( char flg, char* station,char* pc );

		// 送受信バッファ
		char m_buf[4096];

		unsigned char htoi( char *hexstr , short len );

};
#endif
