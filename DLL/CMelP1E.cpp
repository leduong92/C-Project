//================================================================================================
//	FileName	:CMelP3E.h
//	Class		:CMelsecP1E
// 				:Melsec プロトコル3E形式通信(QnA互換3Eフレーム)
//	作成日		:2007.02.17 coba
//================================================================================================
#include <stdio.h>
#include <conio.h>
#include "CMelP1E.h"

//========================================================================================================
//	CMelsecP1E::CMelsecP1E(void)
//	引数　：なし
//	返値　：
//　機能　：コンストラクタ。デフォルトサイズでデータバッファを作成
//========================================================================================================
CMelsecP1E::CMelsecP1E(void)
{
	m_fOpen=FALSE;
	m_tcp=FALSE;	// 2012.12.21
}

//========================================================================================================
//	CMelsecP1E::~CMelsecP1E()
//	デストラクタ
//	引数　：なし
//	返値　：
//　機能　：
//========================================================================================================
CMelsecP1E::~CMelsecP1E()
{
	WSACleanup();
}

//========================================================================================================
//	int CMelsecP1E::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type　：private
//　機能　：WinSock初期化
//	引数１：TRUE=TCP通信,FALSE(デフォルト)=UDP
//========================================================================================================
BOOL CMelsecP1E::Init(BOOL tcp /*=FALSE*/)
{
	// ｵｰﾌﾟﾝ中はOK
	if( m_fOpen ) return(TRUE);

  	WSADATA			WSAData;

	// ソケット初期化
  	if(WSAStartup( MAKEWORD(2,2), &WSAData)) return(-1);

	m_fOpen=TRUE;

	// 2012.12.21
	if(tcp) m_tcp=TRUE;
	else m_tcp=FALSE;

	return(TRUE);
}


//========================================================================================================
//	int CMelsecP1E::P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
//	type　：public
//　機能　：プロトコル3E形式でデバイスデータをブロック受信
//	引数１：ターゲットのIPアドレス
//	引数２：ターゲットのUDPポート番号
//	引数３：受信バッファ
//	引数４：受信バッファサイズ
//	引数５：デバイスの種類(0=ワードデバイス、1=ビットデバイス)
//	引数５：読み出し先頭アドレス(8桁)
//	引数６：受信ブロック数(max256)
//========================================================================================================
int CMelsecP1E::P1ERead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devname,int devaddr, short counts)
{

	// 送信バッファ
	char send[4096];
	int ret;
	char buf1[30];
	unsigned char buf2[30];
	unsigned long ldat;


	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 256 ) return(-2);

	// IPアドレス
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// ポート番号
	if(port <1024 || port >65535) return(-5);

	memset( send, 0, sizeof(send) );

	// ①サブヘッダ(コマンド番号)+PC番号(FF固定)
	if( devtype==DEV_WORD) strcpy( send,"01FF");
	else strcpy( send,"00FF");

	// ②監視タイマ(000Aを使用)
	memcpy( send+4,"000A",4);

	// ③先頭デバイス名
	memset( buf1,0x20,2);
	memcpy( buf1,devname,strlen(devname) > 2 ? 2:strlen(devname));
	sprintf( &send[strlen(send)],"%02X%02X",buf1[0],buf1[1]);

	// ④先頭デバイス long型に変換し、1バイトづつ文字列化する。順序に注意
	ldat=(unsigned long)devaddr;	// ldatの格納順 → LL LH HL HH (L:Low,H:High)
	memcpy( buf2,&ldat,4);
	sprintf( &send[strlen(send)],"%02X%02X%02X%02X",buf2[3],buf2[2],buf2[1],buf2[0]);

	// ⑤データ点数
	sprintf( &send[strlen(send)],"%02X",counts);
	// ⑥ターミネータ
	memcpy( &send[strlen(send)],"00",2);
	// 2012.12.21 coba TCP通信を追加
	if(m_tcp)ret=TcpSend(ipaddr,port,send,strlen(send),buf,bufsize);
	else ret=UdpSend(ipaddr,port,send,strlen(send),buf,bufsize);

	return(ret);
}

//========================================================================================================
//	int CMelsecP1E::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
//	type　：public
//　機能　：プロトコル3E形式でデバイスデータをブロック送信
//	引数１：ターゲットのIPアドレス
//	引数２：ターゲットのUDPポート番号
//	引数３：受信バッファ
//	引数４：受信バッファサイズ
//	引数５：デバイスの種類(0=ワードデバイス、1=ビットデバイス)
//	引数５：書き込み先頭アドレス(8桁)
//	引数６：送信ブロック数(max256)
//	引数７：書き込みデータ
//========================================================================================================
int CMelsecP1E::P1EWrite(char* ipaddr,unsigned short port,int devtype,char* devname,int devaddr, short counts, char* data)
{
	// 送信バッファ
	char send[4096];
	int ret;
	char buf[40+1];
	unsigned char buf2[30];
	unsigned long ldat;

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 256 ) return(-2);

	// IPアドレス
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// ポート番号
	if(port <1024 || port >65535) return(-5);

	memset( send, 0, sizeof(send) );

	// ①サブヘッダ(コマンド番号)+PC番号(FF固定)
	if( devtype==DEV_WORD) strcpy( send,"03FF");
	else strcpy( send,"02FF");

	// ②監視タイマ(000Aを使用)
	memcpy( send+4,"000A",4);

	// ③先頭デバイス名
	memset( buf,0x20,2);
	memcpy( buf,devname,strlen(devname) > 2 ? 2:strlen(devname));
	sprintf( &send[strlen(send)],"%02X%02X",buf[0],buf[1]);

	// ④先頭デバイス long型に変換し、1バイトづつ文字列化する。順序に注意
	ldat=(unsigned long)devaddr;	// ldatの格納順 → LL LH HL HH (L:Low,H:High)
	memcpy( buf2,&ldat,4);
	sprintf( &send[strlen(send)],"%02X%02X%02X%02X",buf2[3],buf2[2],buf2[1],buf2[0]);

	// ⑤データ点数
	sprintf( &send[strlen(send)],"%02X",counts);
	// ⑥ターミネータ
	memcpy( &send[strlen(send)],"00",2);

	// ⑦送信データ
	if( devtype==DEV_WORD){
		memcpy( &send[strlen(send)],data,counts*4);
	}
	else{
		memcpy( &send[strlen(send)],data,counts);
		// 奇数の場合はダミーを付加
		if( counts % 2 )
			memcpy( &send[strlen(send)],"0",1);
	}
//printf("\nsend=%s",send);

	// 送信する
	// 2012.12.21 coba TCP通信を追加
	if(m_tcp)ret=TcpSend(ipaddr,port,send,strlen(send),buf,sizeof(buf));
	else ret=UdpSend(ipaddr,port,send,strlen(send),buf,sizeof(buf));

	return(ret);
}

//========================================================================================================
//	2012.12.21 coba追加
//	int CMelsecP1E::TcpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
//	type　：private
//　機能　：UDP送受信
//	引数１：ターゲットのIPアドレス
//	引数２：ターゲットのUDPポート番号
//	引数３：送信データ
//	引数４：送信データサイズ
//	引数５：受信バッファ
//	引数６：受信バッファサイズ
//	戻り値：0=OK,0未満=ネットワークエラー、1=受信データエラー
//========================================================================================================
int CMelsecP1E::TcpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
{
	unsigned long laddr;
	SOCKET soc;
	struct sockaddr_in rmthost;
	int ret;
	unsigned long arg;
	int len,len2,dlen;
	int wait_cnt,chk;
	fd_set r_socket;
  	struct timeval      time;
	char rbuf[4096];

	//2008.04.25 ﾏﾂﾀﾞ 正しい形で開始するよう追加
//	WSADATA WSAData;
//	WSAStartup( MAKEWORD(2,2), &WSAData);

	// IPｱﾄﾞﾚｽ変換(to 32bit)
	if( INADDR_NONE ==( laddr=inet_addr(ipaddr)) )return(-101);

	// 送信サイズをチェック
	if( size < 1 ) return(-103);

	soc = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if(soc ==INVALID_SOCKET ) return(-104);

	// ホストソケット構造体入力データ作成
	// 接続先（リモートホスト）のアドレスをsockaddr構造体に格納
	memset( &rmthost, 0, sizeof(rmthost) );			// ゼロクリア
	rmthost.sin_family = AF_INET;						// PF_INET
	rmthost.sin_port = htons((unsigned short)port);		// ポート番号
//	rmthost.sin_addr.s_addr = htonl(laddr);				// IPアドレス
	rmthost.sin_addr.s_addr = laddr;				// IPアドレス

	ret = connect( soc, (struct sockaddr*)&rmthost, sizeof(struct sockaddr) );
	if(ret==SOCKET_ERROR){
		closesocket(soc);
//	swprintf( cData,TEXT("WLAN=port=%d"),WSAGetLastError());
		Sleep(30);	// ウエイト
		return -105;										// コネクト失敗
	}


	arg = 0;
	if( (ioctlsocket( soc, FIONBIO, &arg) ) != 0 ) {
		closesocket(soc);
		Sleep(30);	// ウエイト
		return -106;										// ioctl失敗
	}


   	FD_ZERO( &r_socket );
	FD_SET( soc, &r_socket );
//    time.tv_sec  = 3;  /* ﾀｲﾑｱｳﾄ1秒 */
    time.tv_sec  = 2000;	//送信タイムアウトを設定できるよう変更 2008.07.01 ﾏﾂﾀﾞ
    time.tv_usec = 0;
    ret = select( 1, NULL,&r_socket, NULL, &time );
	if(ret==SOCKET_ERROR){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// ウエイト
		return -107;										// データ送信失敗
	}
   	else if( ret == 0 ){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// ウエイト
		return -108;										// データ送信失敗
   	}
  	else if( FD_ISSET( soc, &r_socket ) == 0 ){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// ウエイト
		return -109;										// データ送信失敗
	}


	len = size;
	len2 = 0;
	wait_cnt=0;
	do{
		ret = send( soc, &data[len2], len-len2, 0 );
		if( ret ==SOCKET_ERROR ) {
			shutdown(soc, 2);
			closesocket(soc);
			Sleep(30);	// ウエイト
			return -110;										// データ送信失敗
		}
		// 送信済みバイト数
		len2 += ret;
		if( len !=len2 ){
			if( ++wait_cnt > 4000 ){
				shutdown(soc, 2);
				closesocket(soc);
				Sleep(30);	// ウエイト
				return -111;										// データ送信失敗
			}
			Sleep(5);
		}
		// wait timerをリセット 2005.11.03
		else wait_cnt=0;
	}while( len !=len2);				// 送りきるまで


   	FD_ZERO( &r_socket );
	FD_SET( soc, &r_socket );
//    time.tv_sec  = 7;  /* ﾀｲﾑｱｳﾄ1秒 */
//    time.tv_sec  = 15;  // ﾀｲﾑｱｳﾄ15秒に変更 2006.09.19 matsuda
    time.tv_sec  = 2000;  // 受信タイムアウトを設定できるよう変更 2008.07.01 ﾏﾂﾀﾞ
    time.tv_usec = 0;
    ret = select( 1, &r_socket, NULL, NULL, &time );
	if(ret==SOCKET_ERROR){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// ウエイト
		return -112;										// データ送信失敗
	}
   	else if( ret == 0 ){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// ウエイト
		return -113;										// データ送信失敗
   	}
  	else if( FD_ISSET( soc, &r_socket ) == 0 ){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// ウエイト
		return -114;										// データ送信失敗
	}

	// 2011.08.30 coba やはりループ必要
	len =0,len2=0,wait_cnt=0,dlen=0,chk=0;
	memset(rbuf,0,sizeof(rbuf));
	while(1){
		len = recv(soc, &rbuf[len2], rsize-len2, 0);
		if(len == SOCKET_ERROR){
		//if(len < 1 || len == SOCKET_ERROR){
			shutdown(soc, 2);
			closesocket(soc);
			return -115;										// データ送信失敗
		}
		if(len > 0 ){
			len2 +=len;
			break;
		}

		//３秒待ってダメならタイムｱｳﾄ
		if(len < 1) wait_cnt++;
		else wait_cnt=0;
		if(wait_cnt > 2000){
			shutdown(soc, 2);						//
			closesocket(soc);
			Sleep(30);	// ウエイト
			return(-116);
		}
		Sleep(5);
	}

	shutdown(soc, 2);							// 2:SHUT_RDWR
	closesocket(soc);
	//2008.04.25 ﾏﾂﾀﾞ 正しい形で終了するよう追加
	//WSACleanup();

	// 終了コードをチェック(先頭から2バイト)
	memset( rcv,0,rsize);
	if( memcmp( rbuf+2,"00",2)){
//		printf("\nlen=%d,rcv=%s",strlen(rbuf),rbuf);
		// 終了コードを出力して終了
		memcpy( rcv,rbuf+2,4);
		Sleep(30);	// ウエイト
		return(1);
	}

	ret=strlen(rbuf+4);

	// 受信データコピー
	if(ret>0){
		memcpy( rcv, rbuf+4, ret> rsize ? rsize:ret);
	}
	Sleep(30);	// ウエイト
	return(0);
}
int CMelsecP1E::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
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
//printf("\nrbuf=%s",rbuf);
	// 終了コードをチェック(先頭から2バイト)
	if( memcmp( rbuf+2,"00",2)){
//		printf("\nlen=%d,rcv=%s",strlen(rbuf),rbuf);
		// 終了コードを出力して終了
		memcpy( rcv,rbuf+2,4);
		return(1);
	}

	ret=strlen(rbuf+4);

	// 受信データコピー
	if(ret>0){
		memset( rcv,0,rsize);
		memcpy( rcv, rbuf+4, ret> rsize ? rsize:ret);
	}

	return(0);
}

//========================================================================================================
//	int CMelsecP1E::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	機能　：文字列を、Melsec送信形式に変換する
//			ex) "A1234"->"31413433"
//	引数１：出力バッファ
//	引数２：バッファサイズ（変換元データ長の２倍+1は必要）
//	引数３：変換元文字列データ(NULL-terminated)
//	返値　：0=OK
//========================================================================================================
int CMelsecP1E::Str2Mel(char* buf, unsigned int bufsize, char* str)
{
	unsigned int i,len;
	char txt[10];

	len = strlen(str);
	if( len*2 >= bufsize ) return(-1);

	memset( buf, 0, bufsize );

	for( i=0;i<len;i++){
		// 上下を入れ替える
		if( i+1==len )
			wsprintf( txt,"00");
		else
			wsprintf( txt,"%02X",(unsigned char)str[i+1]);	// 2011.03.11 coba
//			wsprintf( txt,"%02X",str[i+1]);
		memcpy( &buf[i*2], txt, 2 );

		wsprintf( txt,"%02X",(unsigned char)str[i]);		// 2011.03.11 coba
//		wsprintf( txt,"%02X",str[i]);
		i++;
		memcpy( &buf[i*2], txt, 2 );
	}

	return(0);

}

//========================================================================================================
//	unsigned char CMelsecP1E::htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
int CMelsecP1E::Mel2Str(char* buf, unsigned int bufsize, char* melstr)
{
	unsigned int i,len;

	len = strlen(melstr);
	if( bufsize<=len/2 ) return(-1);

	memset( buf, 0, bufsize );

	for( i=0;i<len;i+=2){
		// 上下を入れ替える
		if( i+2>=len )
			buf[strlen(buf)]=0x20;
		else
			buf[strlen(buf)]=htoi( &melstr[i+2],2);

		buf[strlen(buf)]=htoi( &melstr[i],2);
		i+=2;
	}
	return(0);
}

//========================================================================================================
//	unsigned char CMelsecP1E::htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
unsigned char CMelsecP1E::htoi( char *hexstr , short len )
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


BOOL CMelsecP1E::ULChg( char* buf )
{
	char tmp[8];

	if( strlen(buf)<8 )
		return FALSE;

	memcpy( tmp , buf , 8);
	memcpy( buf+0 , tmp+4 , 4 );
	memcpy( buf+4 , tmp+0 , 4 );
	
	return TRUE;
}
