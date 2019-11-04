//================================================================================================
//	FileName	:CKeyL2AE.h
//	Class		:CKeyL20AE
// 				:Keyence KV-LE20A 形式通信(上位リンク通信
//	作成日		:2007.02.17 coba
//================================================================================================
#include <stdio.h>
#include <conio.h>
#include "CKeyL2AE.h"

#include <string>
using namespace std;
//========================================================================================================
//	CKeyL2AE::CKeyL2AE(void)
//	引数　：なし
//	返値　：
//　機能　：コンストラクタ。デフォルトサイズでデータバッファを作成
//========================================================================================================
CKeyL2AE::CKeyL2AE(void)
{
	m_fOpen=FALSE;
}
//========================================================================================================
//	CKeyL2AE::~CKeyL2AE()
//	デストラクタ
//	引数　：なし
//	返値　：
//　機能　：
//========================================================================================================
CKeyL2AE::~CKeyL2AE()
{
	WSACleanup();
}
//========================================================================================================
//	int CKeyL2AE::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type　：private
//　機能　：WinSock初期化
//========================================================================================================
BOOL CKeyL2AE::Init(void)
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
//	int CKeyL2AE::L2ARead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
//	type　：public
//　機能　：Keyence　上位互換通信
//	引数１：ターゲットのIPアドレス
//	引数２：ターゲットのUDPポート番号
//	引数３：受信バッファ
//	引数４：受信バッファサイズ
//	引数５：デバイスの種類(0=ワードデバイス、1=ビットデバイス)
//	引数５：読み出し先頭アドレス　Device + address 固定の桁数なし  
//	引数６：受信ブロック数(Max：120)
//========================================================================================================
int CKeyL2AE::L2ARead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
{

	// 送信バッファ
	char send[5120];
	int ret;

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 120 ) return(-2);	//一回の読み出しは120

	// IPアドレス
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// ポート番号
	if(port <1024 || port >65535) return(-5);

	memset( send, 0, sizeof(send) );

	// ①固定部
	// 連続読み出しコマンド"RDE"
	strcpy( send,"RDE ");
	
	// ②デバイス　+ そのアドレス
	//DM(データメモリ),MR(内部リレー)
	strcat( send, devaddr );
	
	// ③suffix
	if( devtype==DEV_BIT)
		strcat( send,".U");	// ビットデバイス
	strcat( send, " " );	

	// ③読み出し個数
	sprintf( &send[strlen(send)],"%d",counts);

	//終了 cr
	strcat( send, "\r" );	

	// 送信する
	ret=UdpSend(ipaddr,port,send,strlen(send),buf,bufsize);
	
	char buf2[500]={0};
	char buf3[20]={0};
	for(int i=0 ; i< counts ; i++){
		memcpy(buf3, buf+(6*i),5 );
		buf3[5]=0;
		sprintf( buf2+strlen(buf2), "%04X", atoi(buf3) );
	}

	strcpy(buf,buf2);

	return(ret);
}

//========================================================================================================
//	int CKeyL2AE::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
//	type　：public
//　機能　：上位互換通信でデバイスデータをブロック送信
//	引数１：ターゲットのIPアドレス
//	引数２：ターゲットのUDPポート番号
//	引数３：デバイスの種類(0=ワードデバイス、1=ビットデバイス)
//	引数４：書き込み先頭アドレス(8桁)
//	引数５：送信ブロック数(Max：120)　受信バッファ
//	引数６：書き込みデータ(文字の場合はASCIIコード 2文字分（2×2で4桁）、数値の場合はBCD)
//========================================================================================================
int CKeyL2AE::L2AWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
{
	// 送信バッファ
	char send[5120];
	int ret;
	char buf[40+1];

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);

	if( counts <0 || counts > 120 ) return(-2);	//一回の読み出しは120

	// IPアドレス
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// ポート番号
	if(port <1024 || port >65535) return(-5);

	// 送信データ長
	if( devtype==DEV_WORD){
		if( (short)strlen(data) !=counts*4) return(-6);
	}
	else{
		if( (short)strlen(data) !=counts) return(-7);
	}
	memset( send, 0, sizeof(send) );

	// ①固定部
	// 連続書き込みコマンド"WDE"
	strcpy( send,"WRE ");
	
	// ②デバイス　+ そのアドレス
	//DM(データメモリ),MR(内部リレー)
	strcat( send, devaddr );
	
	// ③suffix
	if( devtype==DEV_BIT)
		strcat( send,".U");	// ビットデバイス
	strcat( send, " " );	

	// ③読み出し個数
	sprintf( &send[strlen(send)],"%d",counts);
	
	// ④書き込みデータ
	for(int i=0; i< counts;i++){
		strcat( send, " " );	
/*	
		if(i%2 == 0)
			strcat( send, " " );	

		memset( buf,0,sizeof(buf) );
		memcpy( buf, data+i, 1 );
		char ch;
		ch = buf[0]; 
		sprintf(buf,"%d",ch);  //ASCII code

		memcpy( send+strlen(send), buf, 2 );	
*/

		memset( buf,0,sizeof(buf) );
		memcpy( buf, data+(i*4), 4 );

		int data =0 ;
				
		data = htoi( buf , 4 );
		sprintf(buf,"%d",data);
		memcpy( send+strlen(send), buf, strlen(buf) );	

	}

	//終了 cr
	strcat( send, "\r" );	

	// 送信する
	ret=UdpSend(ipaddr,port,send,strlen(send),buf,sizeof(buf));

	return(ret);
}


//========================================================================================================
//	int CKeyL2AE::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
//	type　：public
//　機能　：上位互換通信でデバイスデータをブロック送信
//	引数１：ターゲットのIPアドレス
//	引数２：ターゲットのUDPポート番号
//	引数３：デバイスの種類(0=ワードデバイス、1=ビットデバイス)
//	引数４：書き込み先頭アドレス(8桁)
//	引数５：送信ブロック数(Max：120)　受信バッファ
//	引数６：書き込みデータ(文字の場合はASCIIコード 2文字分（2×2で4桁）、数値の場合はBCD)
//========================================================================================================
int CKeyL2AE::L2ASetBit(char* ipaddr,unsigned short port,char* devaddr,BOOL on )
{
	// 送信バッファ
	char send[128];
	int ret;
	char buf[64];

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);

	// IPアドレス
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// ポート番号
	if(port <1024 || port >65535) return(-5);

	memset( send, 0, sizeof(send) );

	// ①固定部
	// SET or RESET
	if( on ) strcpy( send,"ST ");
	else strcpy( send,"RS ");

	// ②デバイス　+ そのアドレス
	strcat( send, devaddr );

	//終了 cr
	strcat( send, "\r" );	

	// 送信する
	ret=UdpSend(ipaddr,port,send,strlen(send),buf,sizeof(buf));
	if(ret) return(-1);
	if( memcmp( buf,"OK",2)) return(1);

	return(ret);
}

int  CKeyL2AE::GetPlcType(char* ipaddr,unsigned short port,char* recvBuf)
{
	string stbuf;
	int ret=0;

	//"?K"で型式ゲット
	stbuf = "?K\r";

	// 送信する
	ret=UdpSend(ipaddr,port,(char*)stbuf.c_str() ,stbuf.length() ,recvBuf,3);

	return ret;
}
//========================================================================================================
//	int CKeyL2AE::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
//	type　：private
//　機能　：UDP送受信（データフレームにdataの内容送るだけ）
//	引数１：ターゲットのIPアドレス
//	引数２：ターゲットのUDPポート番号
//	引数３：送信データ
//	引数４：送信データサイズ
//	引数５：受信バッファ
//	引数６：受信バッファサイズ
//	戻り値：0=OK,0未満=ネットワークエラー、1=受信データエラー
//========================================================================================================
int CKeyL2AE::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
{
	SOCKET fSocket;
	WSAEVENT fEvent;
	//DWORD		cEvents;		//hEventsに含まれるイベントハンドルの数
	unsigned long laddr;
	WSABUF		wsBuf;			//WSABUF構造体配列
	DWORD		dwBufferCount;	//lpwsBufの示す配列に入っているWSABUF構造体の数
	DWORD		cnt;			//送信バイト数を受け取るDWORD変数へのポインタ
	DWORD		dwFlags;		//送信方法を制御するビットマスク
	struct sockaddr_in	addr;
	int ret;
	WSANETWORKEVENTS events;
	int			iFromlen;		//lpToの示すsockaddr構造体の長さを指定する整数
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
	ret = WSAWaitForMultipleEvents(1,&fEvent,TRUE,10000,TRUE);
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

	// 受信データコピー
	if( strlen(rbuf) > 0 ){
		if( rsize  > strlen(rbuf)+1 )
			strcpy(rcv,rbuf);
		else
			memcpy(rcv,rbuf,rsize);
	}

	return(0);
}

//========================================================================================================
//	int CKeyL2AE::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	機能　：文字列を、Melsec送信形式に変換する
//			ex) "A1234"->"31413433"
//	引数１：出力バッファ
//	引数２：バッファサイズ（変換元データ長の２倍+1は必要）
//	引数３：変換元文字列データ(NULL-terminated)
//	返値　：0=OK
//========================================================================================================
int CKeyL2AE::Str2Mel(char* buf, unsigned int bufsize, char* str)
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
//	unsigned char CKeyL2AE::htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
int CKeyL2AE::Mel2Str(char* buf, unsigned int bufsize, char* melstr)
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
			buf[strlen(buf)]=(char)htoi( &melstr[i+2],2);

		buf[strlen(buf)]=(char)htoi( &melstr[i],2);
		i+=2;
	}
	return(0);
}

//========================================================================================================
//	unsigned char CKeyL2AE::htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
unsigned short CKeyL2AE::htoi( char *hexstr , short len )
{
	unsigned short ret = 0;
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


BOOL CKeyL2AE::ULChg( char* buf )
{
	char tmp[8];

	if( strlen(buf)<8 )
		return FALSE;

	memcpy( tmp , buf , 8);
	memcpy( buf+0 , tmp+4 , 4 );
	memcpy( buf+4 , tmp+0 , 4 );
	
	return TRUE;
}
