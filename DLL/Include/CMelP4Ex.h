#ifndef COM_PLC4EX_H
#define COM_PLC4EX_H
//================================================================================================
//
//	FileName	:CMelP4Ex.h
//	Class		:CMelsecP4Ex
// 				:Melsec プロトコル４形式通信
//	sumチェック有り、xon/off制御なし。
//	作成日		:2002. 7.1 N.Maeda
//	TRS232Cのサブクラスになっていますので、そちらも加えてください
//================================================================================================
//	2004. 1. 7 ボーレートの設定を追加 300bps-115200bps
//	2008.01.09 coba Init()に、制御線使用有無を追加(キーエンス互換モード用)
//	2008.09.01 ﾏﾂﾀﾞ UStr2Melを追加(前田SJHのソースからコピーしただけ)
//	2009.01.26 coba ComClose()を追加
//	2011.03.11 coba Str2Mes()のバグ(SJISが送れない)を修正
//	2011.06.18 coba ビットデバイスのブロック読み出しP4ReadBB()を追加
//	2012.06.20 coba ビットデバイス多点一括書き込みP4WriteBB()を追加
//	2013.02.17 coba 1Cプロトコルの拡張コマンドQR,QWに対応(AnSで、D1024以降をアクセスする場合に使用)
//	2013.04.11 coba リトライ回数overでも正常終了を返すバグ修正
//	2013.11.22 matsuda P4ReadWで無限ループに入る可能性のある処理に対して、タイムアウトを設定するよう変更
//	2014.01.07 ryuu ↑の修正だと20byte以上受信できないので外す
//================================================================================================

#include "TRS232C.h"


// アドレース＋データ部以外の送信ヘッダ長 ENQ(1)+ST(2)+PC(2)+CMD(2)+WAIT(1)+SUM(2)+デバイス数(2)+CRLF(2)
const int P4_HEADLEN=14;

class CMelsecP4Ex:public TRS232C
{
	public:
		CMelsecP4Ex( void );
		~CMelsecP4Ex( void );

		BOOL Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait,BOOL ctrl_line=TRUE);
		void ComClose(void);	// 2009.01.26

		// 指定数のワードデバイス読み出し&書き込み
		int P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// ファイルレジスタ（Ａ互換、バンク指定書き込み"EW")
// エラー回避処理が入っているか不明、そのためコメントアウトする。（←見りゃいいじゃん！！）
//		int P4WriteWExEW( char* station, char* pcstation,char* addr, short counts, char* msg);

		// 指定数のワードデバイス読み出し&書き込み(2Cモード)
		int P2C4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P2C4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		int P4Send( char* station, char* pcstation,char* command,char* addr, short counts, char* msg);

		// 1点のビットデバイス読み出し&書き込み
		int P4WriteB( char* station, char* pcstation,char* addr, int onoff);
		int P4ReadB( char* station,char* pcstation,char* addr);
		// 多点のビットデバイス読み出し&書き込み
		int P4ReadBB( char* buf,int bufsize,char* station,char* pcstation,char* addr,short counts);
		int P4WriteBB( char* station, char* pcstation,char* addr, int cnt,char* dat); // 2012.06.20 coba

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
		int Receive( char* buf, int bufsize, int waitlen );
		int Answer( char flg, char* station,char* pc );

		// 送受信バッファ
		char m_buf[4096];

		unsigned char htoi( char *hexstr , short len );

};
#endif
