#ifndef TCPServer_H
#define TCPServer_H
////=====================================================================================================================
//
//	FileName:TcpServer.h
//	Class	:TcpServer
//			:ソケット通信クラス
//	作成日	:2002.07.22	  TAKE
//=====================================================================================================================
//	更新日
//	更新日　 2004.04.05 TAKE　MAX_SENDLENGTH,MAX_RECVLENGTHの値を4096にアップ（通信トラブル対応）
//	2005.04.14 coba 待ちキューの数を、5から1にへらした
//	2005.07.11 coba 他のソースと合体のため、いくつか関数を増やした
//	2005.12.01 coba Wait()に引数を付け、待ち時間を指定できるように変更
//	2006.03.16 coba Send()での変数初期化忘れを修正
//	2008.04.24 coba Send()後にこちら側でclosesocket()してたバグを修正
//					ダイレクト送受信関数を追加
//	2008.04.25 coba Send()関数統一、Send()で、相方のCloseを待ってからこちらのソケットを閉じる
//	2008.05.08 coba 後方互換のため、Send()関数で、close待ちの秒数を引数で指定するようにした
//	2008.05.09 ﾏﾂﾀﾞ MAX_SENDLEN、MAX_RECVLENを4096→8192に変更
//  2008.05.29 Maeda PortNoチェックのパラメータ追加
//	2008.07.26 coba ReceiveDirect()で、分割受信に対応していなかったのを修正
//	2008.11.11 matuda get to be enable client ip address
//	2013.09.24 coba 最大送受信サイズを8192=>20480に変更
//	2014.05.22 coba バイナリ受信に対応:Recv()x2
//=====================================================================================================================

//#include <winsock.h>                    /* for socket API           */
#include <winsock2.h>


#define DEF_TRANSPORT_NO	26578               /* 転送ﾎﾟｰﾄ番号       */

// 2004.04.28 TAKE
//#define MAX_SENDLENGTH	4096
//#define MAX_RECVLENGTH	4096
// 2008.05.09 ﾏﾂﾀﾞ
// #define MAX_SENDLENGTH	8192
// #define MAX_RECVLENGTH	8192
// 2013.09.24 coba
#define MAX_SENDLENGTH	8192//20480
#define MAX_RECVLENGTH	8192//20480

#define SHORT_SENDLENGTH	254
#define SHORT_RECVLENGTH	254

class TcpServer
{

	char	fPcName[30];
	char	fProgramName[30];
	int	 	fCommand;
	long 	fRcvPacketNo,fSendPaketNo;
//	SOCKET 	fSocket;
	int  	m_PortNo;
	char 	m_IPAdress[20];
	char 	m_ClientIp[20];	//client ip

//    WSAEVENT fEvent;
	int m_Status;
  	struct sockaddr_in  m_AddrAcpt;
  	SOCKET              m_FdAcpt;
  	struct sockaddr_in  m_Addr;
  	SOCKET              m_Fd;
  	WSADATA             WSAData;
    WSAEVENT			m_Event;

  	struct timeval      time;
  	fd_set              r_socket, w_socket, e_socket;

public:

			 TcpServer(int port);
			 ~TcpServer();

	// 2005.07.11
	 TcpServer();
	int Initialize(int PortNo = DEF_TRANSPORT_NO,int PortCheck=TRUE);
	int Send( char *message,int sendlen,DWORD close_wait);	// 08.05.08 close_waitを追加
	int Recv( char *msg,int len);
	// ---2005.07.11

	virtual BOOL ITcpServer(void);
	virtual int CheckStatus(void);

//	virtual int Wait();
	virtual int Send( char *message);
	virtual int Recv( char *msg);
    virtual int SetPortNo(int PortNo);
	virtual int SetIPAdress(char *IPAdress);
	virtual int SetPcName(char *PcName);
	virtual int SetProgramName(char *ProgramName);
	int Wait( DWORD timeout=5000);		// タイムアウトを指定できるようにした

	// 2008.04.24 追加
	int SendDirect( char *message,int sendlen,DWORD close_wait);
	int RecvDirect( char *msg,int len,int time_out=1);	//タイムアウトを指定できるよう変更 2010.06.08 matsuda
	void GetClientIp( char *ip );		// 2008.11.11 matuda
protected:

};
#endif
