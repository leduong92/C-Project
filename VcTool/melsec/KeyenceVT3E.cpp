//================================================================================================
//	FileName	:KeyenceVT3e.cpp
//	Class		:KeyenceVT3e
// 				:KEYENCEタッチパネルVT3通信
//	作成日		:2013.05.29 coba
//================================================================================================
#include <stdio.h>
#include <conio.h>
#include "KeyenceVT3e.h"

//========================================================================================================
//	KeyenceVT3e::KeyenceVT3e(void)
//	引数　：なし
//	返値　：
//　機能　：コンストラクタ
//========================================================================================================
KeyenceVT3e::KeyenceVT3e(void)
{
	m_fOpen=FALSE;
}

//========================================================================================================
//	KeyenceVT3e::~KeyenceVT3e()
//	デストラクタ
//	引数　：なし
//	返値　：
//　機能　：
//========================================================================================================
KeyenceVT3e::~KeyenceVT3e()
{
	WSACleanup();
}

//========================================================================================================
//	int KeyenceVT3e::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type　：private
//　機能　：WinSock初期化
//	引数１：TRUE=TCP通信,FALSE(デフォルト)=UDP
//========================================================================================================
BOOL KeyenceVT3e::Init(void)
{
	// ｵｰﾌﾟﾝ中はOK
	if( m_fOpen ) return(TRUE);

  	WSADATA			WSAData;

	// ソケット初期化
  	if(WSAStartup( MAKEWORD(2,2), &WSAData)) return(-1);

	m_fOpen=TRUE;

	return(TRUE);
}

//========================================================================================================
// int KeyenceVT3e::ReadW(char* dev,int cnt,char* buf,unsigned int bufsize)
// 機能：TPの内部ワードデバイスを読み出す
// 読み出し後は、Melsecと同じ1ワード=4バイト文字列形式で返します
// 引数１：IPアドレス
// 引数２：ポート番号
// 引数１：デバイスアドレス
// 引数２：デバイス点数(1-64)
// 引数３：受信バッファ
// 引数４：受信バッファサイズ
//========================================================================================================
int KeyenceVT3e::ReadW(const char* ipaddr ,const int port
				,const int  dev_no,const int cnt,char* buf,unsigned int bufsize)
{
	char send[256],rcv[256];
	int ret;
	unsigned short* rcvdat;

	// 引数チェック
	if( cnt <1 || cnt > 64 ) return(-1);
	if( (int)bufsize < cnt*4) return(-2);


	// ヘッダを準備
	VT_UDP_SEND* vt=(VT_UDP_SEND*)send;
	memset(send,0,sizeof(send));
	memcpy( vt->cmd,"ERW",3);		// コマンド
	vt->siki='M';					// 識別子 M:内部デバイス固定
	vt->dev_no=dev_no;				// デバイスアドレス
	vt->dev_cnt=cnt;				// データ数
	vt->len=7;						// 送信データ長(読み出しなのでヘッダ部のみ)

	// 受信する
	ret=UdpSend((char*)ipaddr,port,send,sizeof(VT_UDP_SEND),rcv,sizeof(rcv));
	if(ret) return(ret);

	// 結果チェック
	VT_UDP_RCV* vtr=(VT_UDP_RCV*)rcv;
	if( vtr->result ) return((int)vtr->result);

	// 受信データの位置をセット
	rcvdat=(unsigned short*)(rcv+sizeof(VT_UDP_RCV));

	// データコピー
	memset( buf,0,bufsize);
	for(ret=0;ret<cnt;ret++,rcvdat++){
		sprintf( &buf[ret*4],"%04X",*rcvdat);
	}

	return(0);
}
//========================================================================================================
// int KeyenceVT3e::WriteW(char* dev,int cnt,char* buf,unsigned int bufsize)
// 機能：TPの内部ワードデバイスを書き出す
// 引数１：IPアドレス
// 引数２：ポート番号
// 引数１：デバイスアドレス
// 引数２：デバイス点数(1-64)
// 引数３：送信データ(Melsecと同じく1ワード=4バイト文字列形式)
//========================================================================================================
int KeyenceVT3e::WriteW(const char* ipaddr ,const int port,const int  dev_no,const int cnt,char* dat)
{
	char send[256],rcv[256];
	int ret;
	unsigned short* senddat;

	// 引数チェック
	if( cnt <1 || cnt > 64 ) return(-1);
	if( (int)strlen(dat) < cnt*4) return(-2);


	// ヘッダを準備
	VT_UDP_SEND* vt=(VT_UDP_SEND*)send;
	memset(send,0,sizeof(send));
	memcpy( vt->cmd,"EWW",3);		// コマンド
	vt->siki='M';					// 識別子 M:内部デバイス固定
	vt->dev_no=dev_no;				// デバイスアドレス
	vt->dev_cnt=cnt;				// データ数
	vt->len=7+cnt*2;				// 送信データ長(ヘッダ部+データ)

	// データ部セット:文字列を数値に変換
	senddat=(unsigned short*)(send+sizeof(VT_UDP_SEND));
	for(ret=0;ret<cnt;ret++,senddat++){
		*senddat=this->htous(&dat[ret*4],4);
	}

	// 送信する
	ret=UdpSend((char*)ipaddr,port,send,sizeof(VT_UDP_SEND)+cnt*2,rcv,sizeof(rcv));
	if(ret) return(ret);

	// 結果チェック
	VT_UDP_RCV* vtr=(VT_UDP_RCV*)rcv;
	if( vtr->result ) return((int)vtr->result);


	return(0);
}


//========================================================================================================
// int KeyenceVT3e::ReadB(char* dev,int cnt,char* buf,unsigned int bufsize)
// 機能：TPの内部ワードデバイスを読み出す
// 引数１：IPアドレス
// 引数２：ポート番号
// 引数３：デバイスアドレス
// 戻り値 0=OFF,1=ON,それ以外=エラー
//========================================================================================================
int KeyenceVT3e::ReadB(const char* ipaddr ,const int port,const int  dev_no)
{
	char send[256],rcv[256];
	int ret;
	unsigned char* rcvdat;


	// ヘッダを準備
	VT_UDP_SEND_BIT* vt=(VT_UDP_SEND_BIT*)send;
	memset(send,0,sizeof(send));
	memcpy( vt->cmd,"ERB",3);		// コマンド
	vt->siki='M';					// 識別子 M:内部デバイス固定
	vt->dev_no=dev_no;				// デバイスアドレス
	vt->len=6;						// 送信データ長(読み出しなのでヘッダ部のみ)


	// 受信する
	ret=UdpSend((char*)ipaddr,port,send,sizeof(VT_UDP_SEND_BIT),rcv,sizeof(rcv));
	if(ret) return(ret);

	// 結果チェック
	VT_UDP_RCV* vtr=(VT_UDP_RCV*)rcv;
	if( vtr->result ) return(-1);

	// 受信データの位置をセット
	rcvdat=(unsigned char*)(rcv+sizeof(VT_UDP_RCV));

	ret=(int)(*rcvdat);

	return(ret);
}

//========================================================================================================
// int KeyenceVT3e::ReadB(char* dev,int cnt,char* buf,unsigned int bufsize)
// 機能：TPの内部ワードデバイスを読み出す
// 引数１：IPアドレス
// 引数２：ポート番号
// 引数３：デバイスアドレス
// 引数４：TRUE=on,FALSE=OFF
// 戻り値 0=OK,それ以外=エラー
//========================================================================================================
int KeyenceVT3e::WriteB(const char* ipaddr ,const int port,const int  dev_no,const BOOL on)
{
	char send[256],rcv[256];
	int ret;
	unsigned char* senddat;


	// ヘッダを準備
	VT_UDP_SEND_BIT* vt=(VT_UDP_SEND_BIT*)send;
	memset(send,0,sizeof(send));
	memcpy( vt->cmd,"EWB",3);		// コマンド
	vt->siki='M';					// 識別子 M:内部デバイス固定
	vt->dev_no=dev_no;				// デバイスアドレス
	vt->len=6+1;					// 送信データ長(読み出しなのでヘッダ+データ

	// データセット
	senddat=(unsigned char*)(send+sizeof(VT_UDP_SEND_BIT));
	if( on ) *senddat=1;

	// 送信する
	ret=UdpSend((char*)ipaddr,port,send,sizeof(VT_UDP_SEND_BIT)+1,rcv,sizeof(rcv));
	if(ret) return(ret);

	// 結果チェック
	VT_UDP_RCV* vtr=(VT_UDP_RCV*)rcv;
	if( vtr->result ) return(vtr->result);

	return(ret);
}

//========================================================================================================
//	int KeyenceVT3e::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
//	type　：private
//	機能　：UDP送受信
//	引数１：ターゲットのIPアドレス
//	引数２：ターゲットのUDPポート番号
//	引数３：送信データ
//	引数４：送信データサイズ
//	引数５：受信バッファ
//	引数６：受信バッファサイズ
//	戻り値：0=OK,0未満=ネットワークエラー、1=受信データエラー
//========================================================================================================
int KeyenceVT3e::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,unsigned int rsize)
{
	SOCKET fSocket;
	WSAEVENT fEvent;
	//DWORD		cEvents;		//hEventsに含まれるイベントハンドルの数
	unsigned long laddr;
	WSABUF		wsBuf;		//WSABUF構造体配列
	DWORD		dwBufferCount;	//lpwsBufの示す配列に入っているWSABUF構造体の数
	DWORD		cnt;//送信バイト数を受け取るDWORD変数へのポインタ
	DWORD		dwFlags;		//送信方法を制御するビットマスク
	struct sockaddr_in	addr;
	int ret;
	WSANETWORKEVENTS events;
	int			iFromlen;//lpToの示すsockaddr構造体の長さを指定する整数
	char rbuf[4096];

	// IPｱﾄﾞﾚｽ変換(to 32bit)
	if( INADDR_NONE ==( laddr=inet_addr(ipaddr)) ) return(-101);

	fSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	fSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (fSocket == INVALID_SOCKET){
		printf("socket()Err"); 
		return(-102);
	}
	fEvent = WSACreateEvent();
	if(fEvent == WSA_INVALID_EVENT){
		printf("\nWSACreateEvent()Err: %d\n", fEvent);
		closesocket(fSocket);
		return(-103);
	}

	ret = WSAEventSelect(fSocket,fEvent,FD_READ);
	if(ret == SOCKET_ERROR){
		printf("\nWSAEventSelect()Err: %d\n", ret);
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-104);
	}

	// 送信データ作成
	wsBuf.len=size;
	wsBuf.buf=data;
	dwBufferCount=1;
	dwFlags=MSG_DONTROUTE;//MSG_PARTIAL;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= laddr;
	addr.sin_port			= htons(port);

	// 送信
	ret=WSASendTo(fSocket,&wsBuf,dwBufferCount, &cnt,dwFlags,
					(LPSOCKADDR)&addr,sizeof(addr),NULL,NULL);
	if (ret == SOCKET_ERROR){
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-105);
	}

	// 受信待ち
	//ret = WSAWaitForMultipleEvents(1,&fEvent,FALSE,10000,FALSE);
	//ret = WSAWaitForMultipleEvents(2,&fEvent,TRUE,10000,TRUE);
	ret = WSAWaitForMultipleEvents(1,&fEvent,TRUE,10000,TRUE);	//2013.01.11 kobashi
	if(ret == WSA_WAIT_TIMEOUT){
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-106);
	}

	ret = WSAEnumNetworkEvents(fSocket,fEvent,&events);
	if(ret == SOCKET_ERROR){
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-107);
	}

	// 受信イベント以外
	if((events.lNetworkEvents & FD_READ)==0){
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-108);
	}

	// 受信する
	//printf("Event Err FD=%d\n",FD_READ);
	memset( rbuf,0,sizeof(rbuf));
	wsBuf.len=sizeof(rbuf);
	wsBuf.buf=rbuf;
	dwBufferCount=1;
	dwFlags=0;//MSG_PEEK;//MSG_PARTIAL;
	memset(&addr, 0, sizeof(addr));
	iFromlen=sizeof(addr);

	ret=WSARecvFrom(fSocket,&wsBuf,dwBufferCount, &cnt,&dwFlags,
							(LPSOCKADDR)&addr, &iFromlen,NULL,NULL);
	WSACloseEvent(fEvent);
	closesocket(fSocket);

	if(ret == SOCKET_ERROR){
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-108);
	}

	memset( rcv,0,rsize);
	memcpy( rcv, rbuf, cnt> rsize ? rsize:cnt);

	return(0);
}

//========================================================================================================
//	unsigned short KeyenceVT3e::htous( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
unsigned short KeyenceVT3e::htous( char *hexstr , short len )
{
	unsigned short ret = 0;
	unsigned short i;

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

