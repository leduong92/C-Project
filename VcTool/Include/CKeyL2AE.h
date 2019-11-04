#ifndef KEYENCE_LV21A_H
#define KEYENCE_LV21A_H
//================================================================================================
//	FileName	:CKeyL2AE.h
//	Class		:CKeyL2AE
// 				:
//	作成日		:2012.09.05 kobashi
//
//	ネットワークパラメータをGX Developerで設定する
//	・動作設定
//		更新ﾃﾞｰﾀｺｰﾄﾞ	ASCII更新
//		ｲﾆｼｬﾙﾀｲﾐﾝﾝｸﾞ	常にOPEN待ち
//		IPアドレス		PLC側のIPアドレスを設定
//		送信フレーム	Ethernet(V2.0)
//		RUN中書き込みを許可
//	・ｵｰﾌﾟﾝ設定
//		プロトコル		UDP
//		ｵｰﾌﾟﾝ方式		(設定不可)
//		固定ﾊﾞｯﾌｧ		送信
//		更新手順		手順あり
//		ﾍﾟｱﾘﾝｸﾞｵｰﾌﾟﾝ	ペアにしない
//		生存確認		確認しない
//		自局ﾎﾟｰﾄ番号	PLC側の待ち受けﾎﾟｰﾄ
//		更新相手IP		このプログラムを走らせるPC側のIP
//		更新相手ﾎﾟｰﾄ	FFFF
//	・ﾙｰﾁﾝｸﾞ情報
//		ﾙｰﾀ中継機能		使用する
//		ｻﾌﾞﾈｯﾄﾏｽｸ		ネットワークにあわせて設定
//		ﾃﾞﾌｫﾙｰﾀｱﾄﾞﾚｽ	ダミーでもいいから設定する
//================================================================================================
//	修正
//	2011.03.11 coba Str2Mes()のバグ(SJISが送れない)を修正
//	2014.12.05 coba ビットのONとOFFを追加
//================================================================================================
#include <winsock2.h>
#include <windows.h>



// 上位リンクのデフォルトポート番号(バイナリ通信でしか使えない)
//#define UDP_MELPORT 5000

// デバイスの種類
#define DEV_WORD 0
#define DEV_BIT  1

// 送信データ長の固定部h
// CPU監視タイマ(4)+コマンド(4)+サブコマンド(4)+デバイスコード(2)
// +Devコード(2)+先頭デバイス(6)+点数(2)
//#define P3E_FIX_DATALEN 24

class CKeyL2AE
{
	public:
		CKeyL2AE( void );
		~CKeyL2AE( void );

		BOOL Init(void);


		// 指定数のワードデバイス読み出し&書き込み(2Cモード)
		int L2AWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data);
		int L2ARead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts);

		int L2ASetBit(char* ipaddr,unsigned short port,char* devaddr,BOOL on );

		//接続テスト用
		int GetPlcType(char* ipaddr,unsigned short port,char* recvBuf);

		// 文字列変換
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);

		// 上位下位のワード入れ替え(BCD８桁の変換)
		BOOL ULChg( char* buf );

		unsigned short htoi( char *hexstr , short len );
	protected:

	private:
		BOOL m_fOpen;					// ｵｰﾌﾟﾝ中=TRUE,close中=FALSE

		//送信処理
		int UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize);


};
#endif
