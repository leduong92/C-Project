#ifndef MELSEC_P3E_H
#define MELSEC_P3E_H
//================================================================================================
//	FileName	:CMelP3E.h
//	Class		:CMelsecP3E
// 				:Melsec プロトコル3E形式通信(QnA互換3Eフレーム)
//	作成日		:2007.02.17 coba
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
//================================================================================================
#include <winsock2.h>
#include <windows.h>

// Melsecのデフォルトポート番号(バイナリ通信でしか使えない)
#define UDP_MELPORT 5000

// デバイスの種類
#define DEV_WORD 0
#define DEV_BIT  1

// 送信データ長の固定部h
// CPU監視タイマ(4)+コマンド(4)+サブコマンド(4)+デバイスコード(2)
// +Devコード(2)+先頭デバイス(6)+点数(2)
#define P3E_FIX_DATALEN 24

class CMelsecP3E
{
	public:
		CMelsecP3E( void );
		~CMelsecP3E( void );

		BOOL Init(void);


		// 指定数のワードデバイス読み出し&書き込み(2Cモード)
		int P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data);
		int P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize
							,int devtype,char* devaddr, short counts);

		// 文字列変換
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);

		// 上位下位のワード入れ替え(BCD８桁の変換)
		BOOL ULChg( char* buf );

		unsigned short htoi( char *hexstr , short len );
	protected:

	private:
		BOOL m_fOpen;					// ｵｰﾌﾟﾝ中=TRUE,close中=FALSE


		int UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize);


};
#endif
