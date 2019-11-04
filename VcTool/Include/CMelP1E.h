#ifndef MELSEC_P1E_H
#define MELSEC_P1E_H
//================================================================================================
//	FileName	:CMelP3E.h
//	Class		:CMelsecP3E
// 				:Melsec プロトコル1E形式通信(QnA互換1Eフレーム)
//	作成日		:2008.01.10 coba
//
//	Etherユニットの設定で、ｵｰﾌﾟﾝ設定のところで、このプログラムを走らせる機器のIPを設定する
//		プロトコル		UDP
//		ｵｰﾌﾟﾝ方式		(設定不可)
//		固定ﾊﾞｯﾌｧ		送信
//		ﾍﾟｱﾘﾝｸﾞｵｰﾌﾟﾝ	ペアにしない
//		生存確認		確認しない
//		自局ﾎﾟｰﾄ番号	PLC側の待ち受けﾎﾟｰﾄ
//		更新相手IP		このプログラムを走らせる機器のIP
//		更新相手ﾎﾟｰﾄ	FFFF
//================================================================================================
//	修正
//	2011.03.11 coba Str2Mel()のバグ(SJISが送れない)を修正
//	2012.12.21 coba ENET-ADAPでのUDPがうまくいかないのでTCPを追加
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

class CMelsecP1E
{
	public:
		CMelsecP1E( void );
		~CMelsecP1E( void );

		BOOL Init(BOOL tcp=FALSE);


		// 指定数のワードデバイス読み出し&書き込み(2Cモード)
		int P1EWrite(char* ipaddr,unsigned short port,int devtype,char* devname,int devaddr, short counts, char* data);
		int P1ERead( char* ipaddr,unsigned short port,char* buf,int bufsize
							,int devtype,char* devname,int devaddr, short counts);

		// 文字列変換
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);

		// 上位下位のワード入れ替え(BCD８桁の変換)
		BOOL ULChg( char* buf );

	protected:

	private:
		BOOL m_fOpen;					// ｵｰﾌﾟﾝ中=TRUE,close中=FALSE

		// 2012.12.21 coba TCP追加
		BOOL m_tcp;
		int TcpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize);

		int UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize);

		unsigned char htoi( char *hexstr , short len );

};
#endif
