// CMelP3E.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
//================================================================================================
//	FileName	:CMelP3E.h
//	Class		:CMelsecP3E
// 				:Melsec プロトコル3E形式通信(QnA互換3Eフレーム)
//	作成日		:2007.02.17 coba
//================================================================================================
#include <stdio.h>
#include <conio.h>
#include "CMelP3E.h"
#include <stdlib.h>

//========================================================================================================
//	CMelsecP3E::CMelsecP3E(void)
//	引数　：なし
//	返値　：
//　機能　：コンストラクタ。デフォルトサイズでデータバッファを作成
//========================================================================================================
CMelsecP3E::CMelsecP3E()
{
	m_fOpen = FALSE;

	m_fBinMode = FALSE;	//初期状態はアスキーモードとする	//2014.02.03 sanno

}

//========================================================================================================
//	CMelsecP3E::~CMelsecP3E()
//	デストラクタ
//	引数　：なし
//	返値　：
//　機能　：
//========================================================================================================
CMelsecP3E::~CMelsecP3E()
{
	WSACleanup();
}

//========================================================================================================
//	int CMelsecP3E::SetMode(BOOL binary_mode)
//	type　：private
//　機能　：WPLCのモードをセット
//========================================================================================================
void CMelsecP3E::SetMode(BOOL binary_mode)
{
	if (binary_mode == TRUE)
		m_fBinMode = TRUE;
	else
		m_fBinMode = FALSE;

	return;
}


//========================================================================================================
//	int CMelsecP3E::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type　：private
//　機能　：WinSock初期化
//========================================================================================================
BOOL CMelsecP3E::Init()
{
	// ｵｰﾌﾟﾝ中はOK
	if (m_fOpen) return(TRUE);

	WSADATA			WSAData;

	// ソケット初期化
	if (WSAStartup(MAKEWORD(2, 2), &WSAData)) return(-1);

	m_fOpen = TRUE;
	return(TRUE);
}


//========================================================================================================
//	int CMelsecP3E::P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
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
int CMelsecP3E::P3ERead(char* ipaddr, unsigned short port, char* buf, int bufsize
	, int devtype, char* devaddr, short counts)
{
	int ret;

	if (m_fBinMode == TRUE)
		ret = this->P3ERead(ipaddr, port, buf, bufsize, devtype, devaddr, counts, TRUE);
	else
		ret = this->P3ERead(ipaddr, port, buf, bufsize, devtype, devaddr, counts, FALSE);

	return ret;
}

//========================================================================================================
//	int CMelsecP3E::P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
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
int CMelsecP3E::P3ERead(char* ipaddr, unsigned short port, char* buf, int bufsize
	, int devtype, char* devaddr, short counts, BOOL binary)
{
	int ret;

	if (binary == TRUE)
		ret = this->P3EReadB(ipaddr, port, buf, bufsize, devtype, devaddr, counts);
	else
		ret = this->P3EReadA(ipaddr, port, buf, bufsize, devtype, devaddr, counts);

	return ret;
}


//========================================================================================================
//	int CMelsecP3E::P3EReadA( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
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
int CMelsecP3E::P3EReadA(char* ipaddr, unsigned short port, char* buf, int bufsize, int devtype, char* devaddr, short counts)
{

	// 送信バッファ
	char send[5120];
	int ret;

	SOCKET fSocket;
	WSAEVENT fEvent;
	//DWORD		cEvents;		//hEventsに含まれるイベントハンドルの数
	unsigned long laddr;
	WSABUF		wsBuf;		//WSABUF構造体配列
	DWORD		dwBufferCount;	//lpwsBufの示す配列に入っているWSABUF構造体の数
	DWORD		cnt;//送信バイト数を受け取るDWORD変数へのポインタ
	DWORD		dwFlags;		//送信方法を制御するビットマスク
	struct sockaddr_in	addr;

	WSANETWORKEVENTS events;
	int			iFromlen;//lpToの示すsockaddr構造体の長さを指定する整数
	char rbuf[4096];
	char rcv[100];

	// 未オープンエラー
	memset(rcv, 0, sizeof(rcv));
	//if (m_fOpen == FALSE) return(-1);
	if (counts <0 || counts > 960) return(-2);

	// ｱﾄﾞﾚｽ(デバイスコード(2)+先頭デバイス(6))
	if (strlen(devaddr) != 8) return(-3);

	// IPアドレス
	if (INADDR_NONE == inet_addr(ipaddr)) return(-4);

	// ポート番号
	if (port <1024 || port >65535) return(-5);

	memset(send, 0, sizeof(send));

	// ①固定部
	// サブヘッダ"5000"+NW番号"00"+PC番号"FF"+要求先Unit#"03FF"+要求局番"00"
	strcpy(send, "500000FF03FF00");

	// ②送信データ長を算出する=読み出しなので固定長(監視タイマ～読み出し点数)
	sprintf(&send[strlen(send)], "%04X", P3E_FIX_DATALEN);


	// ③CPU監視タイマ(4)+コマンド(4)+サブコマンド(4)
	if (devtype == DEV_WORD)
		strcat(send, "001004010000");	// ワードデバイス
	else
		strcat(send, "001004010001");	// ビットデバイス

										// ④読み出しｱﾄﾞﾚｽ(8)
	strcat(send, devaddr);

	// ⑤読み出し点数
	sprintf(&send[strlen(send)], "%04X", counts);
	// 送信する

	//ret = UdpSendA(ipaddr, port, send, strlen(send), buf, bufsize);


	// IPｱﾄﾞﾚｽ変換(to 32bit)
	if (INADDR_NONE == (laddr = inet_addr(ipaddr))) return(-101);

	fSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//	fSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (fSocket == INVALID_SOCKET) {
		printf("socket()Err");
		return(-102);
	}
	fEvent = WSACreateEvent();
	if (fEvent == WSA_INVALID_EVENT) {
		printf("\nWSACreateEvent()Err: %d\n", fEvent);
		closesocket(fSocket);
		return(-103);
	}

	ret = WSAEventSelect(fSocket, fEvent, FD_READ);
	if (ret == SOCKET_ERROR) {
		printf("\nWSAEventSelect()Err: %d\n", ret);
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-104);
	}

	// 送信データ作成
	wsBuf.len = strlen(send);
	wsBuf.buf = send;
	dwBufferCount = 1;
	dwFlags = MSG_DONTROUTE;//MSG_PARTIAL;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = laddr;
	addr.sin_port = htons(port);

	// 送信
	ret = WSASendTo(fSocket, &wsBuf, dwBufferCount, &cnt, dwFlags,
		(LPSOCKADDR)&addr, sizeof(addr), NULL, NULL);
	if (ret == SOCKET_ERROR) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-105);
	}

	// 受信待ち
	ret = WSAWaitForMultipleEvents(1, &fEvent, TRUE, 10000, TRUE);
	if (ret == WSA_WAIT_TIMEOUT) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-106);
	}

	ret = WSAEnumNetworkEvents(fSocket, fEvent, &events);
	if (ret == SOCKET_ERROR) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-107);
	}

	// 受信イベント以外
	if ((events.lNetworkEvents & FD_READ) == 0) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-108);
	}

	// 受信する
	//printf("Event Err FD=%d\n",FD_READ);
	memset(rbuf, 0, sizeof(rbuf));
	wsBuf.len = sizeof(rbuf);
	wsBuf.buf = rbuf;
	dwBufferCount = 1;
	dwFlags = 0;//MSG_PEEK;//MSG_PARTIAL;
	memset(&addr, 0, sizeof(addr));
	iFromlen = sizeof(addr);

	ret = WSARecvFrom(fSocket, &wsBuf, dwBufferCount, &cnt, &dwFlags,
		(LPSOCKADDR)&addr, &iFromlen, NULL, NULL);


	WSACloseEvent(fEvent);
	closesocket(fSocket);


	// 終了コードをチェック(先頭から18バイト)
	if (memcmp(rbuf + 18, "0000", 4)) {
		//printf("\nrcv=%s",rbuf);
		// 終了コードを出力して終了
		memcpy(rcv, rbuf + 18, 4);
		return(1);
	}

	// 受信データ長(先頭から14バイト目)、終了コード(4バイト)を含んでいるので引く
	ret = this->htoi(rbuf + 14, 4) - 4;

	// 受信データコピー
	if (ret>0) {
		memset(rcv, 0, sizeof(buf));
		memcpy(rcv, rbuf + 22, ret> sizeof(buf) ? sizeof(buf) : ret);
	}

	return atoi(rcv);
}


//========================================================================================================
//	int CMelsecP3E::P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
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
int CMelsecP3E::P3EReadB(char* ipaddr, unsigned short port, char* buf, int bufsize, int devtype, char* devaddr, short counts)
{

	// 送信バッファ
	unsigned char send[120];
	int ret;
	unsigned short len;
	char txt[30];
	short hex_sign;	//読み出しアドレスの指定方法が16進数かどうかのサイン	0:10進	1:16進
	unsigned int top_addres;

	// 未オープンエラー
	if (m_fOpen == FALSE) return(-1);
	if (counts <0 || counts > 960) return(-2);

	// ｱﾄﾞﾚｽ(デバイスコード(2)+先頭デバイス(6))
	if (strlen(devaddr) != 8) return(-3);

	// IPアドレス
	if (INADDR_NONE == inet_addr(ipaddr)) return(-4);

	// ポート番号
	if (port <1024 || port >65535) return(-5);


	// ①固定部
	memset(send, 0, sizeof(send));
	// サブヘッダ"5000"+NW番号"00"+PC番号"FF"+要求先Unit#"03FF"+要求局番"00"
	send[0] = 0x50;//サブヘッダー
	send[1] = 0x00;//サブヘッダー
	send[2] = 0x00;//ネットワーク番号
	send[3] = 0xFF;//PC番号
	send[4] = 0xFF;//要求先ユニット
	send[5] = 0x03;//I/O番号
	send[6] = 0x00;//局番

				   // ②送信データ長を算出する=読み出しなので固定長(監視タイマ～読み出し点数)
				   //		sprintf( &send[strlen(send)],"%04X",P3E_FIX_DATALEN);
	len = P3E_FIX_DATALEN / 2;
	memcpy(&send[7], &len, 2);//要求データ長0x0c


							  // ③CPU監視タイマ(4)+コマンド(4)+サブコマンド(4)
	if (devtype == DEV_WORD)
	{
		//			strcat( send,"001004010000");	// ワードデバイス
		send[9] = 0x10;//CPU監視タイマーL
		send[10] = 0x00;//CPU監視タイマーH
		send[11] = 0x01;//コマンドL
		send[12] = 0x04;//コマンドH
		send[13] = 0x00;//サブコマンドL
		send[14] = 0x00;//サブコマンドH
	}
	else
	{
		//			strcat( send,"001004010001");	// ビットデバイス
		send[9] = 0x10;//CPU監視タイマーL
		send[10] = 0x00;//CPU監視タイマーH
		send[11] = 0x01;//コマンドL
		send[12] = 0x04;//コマンドH
		send[13] = 0x01;//サブコマンドL
		send[14] = 0x00;//サブコマンドH
	}

	// デバイスコードに変換
	hex_sign = 0;

	if (!memcmp(devaddr, "D*", 2)) send[18] = 0xa8;			//データレジスタ
	else if (!memcmp(devaddr, "M*", 2)) send[18] = 0x90;		//内部リレー
	else if (!memcmp(devaddr, "SM", 2)) send[18] = 0x91;		//特殊リレー
	else if (!memcmp(devaddr, "SD", 2)) send[18] = 0xA9;		//特殊レジスタ
	else if (!memcmp(devaddr, "X*", 2)) {					//入力リレー
		send[18] = 0x9C;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "Y*", 2)) {					//出力リレー
		send[18] = 0x9D;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "L*", 2)) send[18] = 0x92;		//ラッチリレー
	else if (!memcmp(devaddr, "F*", 2)) send[18] = 0x93;		//アナンシェータ
	else if (!memcmp(devaddr, "V*", 2)) send[18] = 0x94;		//エッジリレー
	else if (!memcmp(devaddr, "B*", 2)) {					//リンクリレー
		send[18] = 0xA0;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "W*", 2)) {					//リンクレジスタ
		send[18] = 0xB4;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "TS", 2)) send[18] = 0xC1;		//タイマ　接点
	else if (!memcmp(devaddr, "TC", 2)) send[18] = 0xC0;		//タイマ　コイル
	else if (!memcmp(devaddr, "TN", 2)) send[18] = 0xC2;		//タイマ　現在地
	else if (!memcmp(devaddr, "SS", 2)) send[18] = 0xC7;		//積算タイマ　接点
	else if (!memcmp(devaddr, "SC", 2)) send[18] = 0xC6;		//積算タイマ　コイル
	else if (!memcmp(devaddr, "SN", 2)) send[18] = 0xC8;		//積算タイマ　現在地
	else if (!memcmp(devaddr, "CS", 2)) send[18] = 0xC4;		//カウンタ　接点
	else if (!memcmp(devaddr, "CC", 2)) send[18] = 0xC3;		//カウンタ　コイル
	else if (!memcmp(devaddr, "CN", 2)) send[18] = 0xC5;		//カウンタ　現在地
	else if (!memcmp(devaddr, "SB", 2)) {					//特殊リンクリレー
		send[18] = 0xA1;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "SW", 2)) {					//特殊リンクレジスタ
		send[18] = 0xB5;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "S*", 2)) send[18] = 0x98;		//ステップリレー
	else if (!memcmp(devaddr, "DX", 2)) {					//ダイレクト入力
		send[18] = 0xA2;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "DY", 2)) {					//ダイレクト出力
		send[18] = 0xA3;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "Z*", 2)) send[18] = 0xCC;		//インデックスレジスタ
	else if (!memcmp(devaddr, "R*", 2)) send[18] = 0xAF;		//ファイレジスタ
	else if (!memcmp(devaddr, "ZR", 2)) {					//ファイルレジスタ
		send[18] = 0xB0;
		hex_sign = 1;
	}
	else                     return(-100);

	// ④読み出しｱﾄﾞﾚｽ(8)
	if (hex_sign == 0) {
		memset(txt, 0, sizeof(txt));
		memcpy(txt, devaddr + 2, 6);
		top_addres = atoi(txt);
		memcpy(&send[15], (unsigned int*)&top_addres, 3);
	}
	else {
		memset(txt, 0, sizeof(txt));
		memcpy(txt, devaddr + 6, 2);
		send[15] = (unsigned char)htoi(txt, 2);//ディバイス点数  L
		memcpy(txt, devaddr + 4, 2);
		send[16] = (unsigned char)htoi(txt, 2);//ディバイス点数  -
		memcpy(txt, devaddr + 2, 2);
		send[17] = (unsigned char)htoi(txt, 2);//ディバイス点数  H	
	}


	// ⑤読み出し点数
	memcpy(&send[19], (unsigned short*)&counts, 2);
	/*
	int i;
	i = 0;
	while(i<25){
	printf("%02d:%02x\n",i,send[i]);
	i++;
	}
	*/
	// 送信する
	if (devtype == DEV_WORD)
		ret = UdpSendB(ipaddr, port, (char*)send, 21, buf, bufsize);
	else
		ret = UdpSendBit(ipaddr, port, (char*)send, 21, buf, bufsize, counts);

	return(ret);
}

//========================================================================================================
//	int CMelsecP3E::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
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
int CMelsecP3E::P3EWrite(char* ipaddr, unsigned short port, int devtype, char* devaddr, short counts, char* data)
{
	int ret;

	if (m_fBinMode == TRUE)
		ret = this->P3EWrite(ipaddr, port, devtype, devaddr, counts, data, TRUE);
	else
		ret = this->P3EWrite(ipaddr, port, devtype, devaddr, counts, data, FALSE);

	return ret;
}

//========================================================================================================
//	int CMelsecP3E::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
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
//	引数８：書き込みモード（アスキーorバイナリ）
//========================================================================================================
int CMelsecP3E::P3EWrite(char* ipaddr, unsigned short port, int devtype, char* devaddr, short counts, char* data, BOOL binary)
{
	int ret;

	if (binary == TRUE)
		ret = this->P3EWriteB(ipaddr, port, devtype, devaddr, counts, data);
	else
		ret = this->P3EWriteA(ipaddr, port, devtype, devaddr, counts, data);

	return ret;
}


//========================================================================================================
//	int CMelsecP3E::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
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
int CMelsecP3E::P3EWriteA(char* ipaddr, unsigned short port, int devtype, char* devaddr, short counts, char* data)
{
	// 送信バッファ
	char send[5120];
	int ret;
	char buf[40 + 1];

	SOCKET fSocket;
	WSAEVENT fEvent;
	//DWORD		cEvents;		//hEventsに含まれるイベントハンドルの数
	unsigned long laddr;
	WSABUF		wsBuf;		//WSABUF構造体配列
	DWORD		dwBufferCount;	//lpwsBufの示す配列に入っているWSABUF構造体の数
	DWORD		cnt;//送信バイト数を受け取るDWORD変数へのポインタ
	DWORD		dwFlags;		//送信方法を制御するビットマスク
	struct sockaddr_in	addr;
	WSANETWORKEVENTS events;
	int			iFromlen;//lpToの示すsockaddr構造体の長さを指定する整数
	char rbuf[4096];
	char rcv[1028];

	// 未オープンエラー
	memset(rcv, 0, sizeof(rcv));
	//if (m_fOpen == FALSE) return(-1);
	if (counts <0 || counts > 960) return(-2);

	// ｱﾄﾞﾚｽ(デバイスコード(2)+先頭デバイス(6))
	if (strlen(devaddr) != 8) return(-3);

	// IPアドレス
	if (INADDR_NONE == inet_addr(ipaddr)) return(-4);

	// ポート番号
	if (port <1024 || port >65535) return(-5);

	// 送信データ長
	if (devtype == DEV_WORD) {
		if ((short)strlen(data) != counts * 4) return(-6);
	}
	else {
		if ((short)strlen(data) != counts) return(-7);
	}
	memset(send, 0, sizeof(send));

	// ①固定部
	// サブヘッダ"5000"+NW番号"00"+PC番号"FF"+要求先Unit#"03FF"+要求局番"00"
	strcpy(send, "500000FF03FF00");


	// ワードデバイス
	if (devtype == DEV_WORD) {
		// ②送信データ長を算出する=(固定:監視タイマ～読み出し点数)+デバイス点数×4(バイト)
		sprintf(&send[strlen(send)], "%04X", P3E_FIX_DATALEN + counts * 4);
		// ③CPU監視タイマ(4)+コマンド(4)+サブコマンド(4)
		strcat(send, "001014010000");	// ワードデバイス
	}
	// ビットデバイス
	else {
		// ②送信データ長を算出する=(固定:監視タイマ～読み出し点数)+デバイス点数
		sprintf(&send[strlen(send)], "%04X", P3E_FIX_DATALEN + counts);
		// ③CPU監視タイマ(4)+コマンド(4)+サブコマンド(4)
		strcat(send, "001014010001");	// ビットデバイス
	}
	// ④読み出しｱﾄﾞﾚｽ(8)
	strcat(send, devaddr);

	// ⑤読み出し点数
	sprintf(&send[strlen(send)], "%04X", counts);

	// ⑥送信データ
	if (devtype == DEV_WORD)
		memcpy(&send[strlen(send)], data, counts * 4);
	else
		memcpy(&send[strlen(send)], data, counts);


	// 送信する
	//ret = UdpSendA(ipaddr, port, send, strlen(send), buf, sizeof(buf));


	// IPｱﾄﾞﾚｽ変換(to 32bit)
	if (INADDR_NONE == (laddr = inet_addr(ipaddr))) return(-101);

	fSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//	fSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (fSocket == INVALID_SOCKET) {
		printf("socket()Err");
		return(-102);
	}
	fEvent = WSACreateEvent();
	if (fEvent == WSA_INVALID_EVENT) {
		printf("\nWSACreateEvent()Err: %d\n", fEvent);
		closesocket(fSocket);
		return(-103);
	}

	ret = WSAEventSelect(fSocket, fEvent, FD_READ);
	if (ret == SOCKET_ERROR) {
		printf("\nWSAEventSelect()Err: %d\n", ret);
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-104);
	}

	// 送信データ作成
	wsBuf.len = sizeof(send);
	wsBuf.buf = send;
	dwBufferCount = 1;
	dwFlags = MSG_DONTROUTE;//MSG_PARTIAL;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = laddr;
	addr.sin_port = htons(port);

	// 送信
	ret = WSASendTo(fSocket, &wsBuf, dwBufferCount, &cnt, dwFlags,
		(LPSOCKADDR)&addr, sizeof(addr), NULL, NULL);
	if (ret == SOCKET_ERROR) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-105);
	}

	// 受信待ち
	ret = WSAWaitForMultipleEvents(1, &fEvent, TRUE, 10000, TRUE);
	if (ret == WSA_WAIT_TIMEOUT) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-106);
	}

	ret = WSAEnumNetworkEvents(fSocket, fEvent, &events);
	if (ret == SOCKET_ERROR) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-107);
	}

	// 受信イベント以外
	if ((events.lNetworkEvents & FD_READ) == 0) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-108);
	}

	// 受信する
	//printf("Event Err FD=%d\n",FD_READ);
	memset(rbuf, 0, sizeof(rbuf));
	wsBuf.len = sizeof(rbuf);
	wsBuf.buf = rbuf;
	dwBufferCount = 1;
	dwFlags = 0;//MSG_PEEK;//MSG_PARTIAL;
	memset(&addr, 0, sizeof(addr));
	iFromlen = sizeof(addr);

	ret = WSARecvFrom(fSocket, &wsBuf, dwBufferCount, &cnt, &dwFlags,
		(LPSOCKADDR)&addr, &iFromlen, NULL, NULL);


	WSACloseEvent(fEvent);
	closesocket(fSocket);


	// 終了コードをチェック(先頭から18バイト)
	if (memcmp(rbuf + 18, "0000", 4)) {
		//printf("\nrcv=%s",rbuf);
		// 終了コードを出力して終了
		memcpy(rcv, rbuf + 18, 4);
		return(-109);
	}

	// 受信データ長(先頭から14バイト目)、終了コード(4バイト)を含んでいるので引く
	ret = this->htoi(rbuf + 14, 4) - 4;

	// 受信データコピー
	if (ret>0) {
		memset(rcv, 0, sizeof(buf));
		memcpy(rcv, rbuf + 22, ret> sizeof(buf) ? sizeof(buf) : ret);
	}

	return(ret);
}


//========================================================================================================
//	int CMelsecP3E::P3EWriteB(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
//	type　：public
//　機能　：プロトコル3E形式でデバイスデータをブロック送信(バイナリモード)
//	引数１：ターゲットのIPアドレス
//	引数２：ターゲットのUDPポート番号
//	引数３：受信バッファ
//	引数４：受信バッファサイズ
//	引数５：デバイスの種類(0=ワードデバイス、1=ビットデバイス)
//	引数５：書き込み先頭アドレス(8桁)
//	引数６：送信ブロック数(max256)
//	引数７：書き込みデータ
//========================================================================================================
int CMelsecP3E::P3EWriteB(char* ipaddr, unsigned short port, int devtype, char* devaddr, short counts, char* data)
{
	// 送信バッファ
	unsigned char send[5120];
	int ret, i;
	char buf[40 + 1];
	unsigned short len;
	char txt[30];
	short hex_sign;
	unsigned int top_addres;
	int send_bytes;

	// 未オープンエラー
	if (m_fOpen == FALSE) return(-1);
	if (counts <0 || counts > 960) return(-2);

	// ｱﾄﾞﾚｽ(デバイスコード(2)+先頭デバイス(6))
	if (strlen(devaddr) != 8) return(-3);

	// IPアドレス
	if (INADDR_NONE == inet_addr(ipaddr)) return(-4);

	// ポート番号
	if (port <1024 || port >65535) return(-5);

	// 送信データ長
	if (devtype == DEV_WORD) {
		if ((short)strlen(data) != counts * 4) return(-6);
	}
	else
		if ((short)strlen(data) != counts) return(-7);

	memset(send, 0, sizeof(send));


	// ①固定部
	// サブヘッダ"5000"+NW番号"00"+PC番号"FF"+要求先Unit#"03FF"+要求局番"00"
	send[0] = 0x50;//サブヘッダー
	send[1] = 0x00;//サブヘッダー
	send[2] = 0x00;//ネットワーク番号
	send[3] = 0xFF;//PC番号
	send[4] = 0xFF;//I/O番号
	send[5] = 0x03;//要求先ユニット
	send[6] = 0x00;//局番



				   // ワードデバイス
	if (devtype == DEV_WORD) {
		// ②送信データ長を算出する=(固定:監視タイマ～読み出し点数)+デバイス点数×4(バイト)
		//send[7]=(P3E_FIX_DATALEN+counts*4)/2;//要求データ長
		len = (P3E_FIX_DATALEN + counts * 4) / 2;
		memcpy(&send[7], &len, 2);//要求データ長

								  // ③CPU監視タイマ(4)+コマンド(4)+サブコマンド(4)
								  //			strcat( send,"001014010000");	// ワードデバイス
		send[9] = 0x10;//CPU監視タイマーL
		send[10] = 0x00;//CPU監視タイマーH
		send[11] = 0x01;//コマンドL
		send[12] = 0x14;//コマンドH
		send[13] = 0x00;//サブコマンドL
		send[14] = 0x00;//サブコマンドH
	}
	// ビットデバイス
	else {
		// ②送信データ長を算出する=(固定:監視タイマ～読み出し点数)+デバイス点数×4(バイト)
		if (counts % 2 == 0)
			len = (P3E_FIX_DATALEN + counts) / 2;
		else
			len = (P3E_FIX_DATALEN + counts) / 2 + 1;
		memcpy(&send[7], &len, 2);//要求データ長

								  // ③CPU監視タイマ(4)+コマンド(4)+サブコマンド(4)
								  //strcat( send,"001014010001");	// ビットデバイス

		send[9] = 0x10;//CPU監視タイマーL
		send[10] = 0x00;//CPU監視タイマーH
		send[11] = 0x01;//コマンドL
		send[12] = 0x14;//コマンドH
		send[13] = 0x01;//サブコマンドL
		send[14] = 0x00;//サブコマンドH

	}

	// デバイスコードに変換
	hex_sign = 0;

	if (!memcmp(devaddr, "D*", 2)) send[18] = 0xa8;			//データレジスタ
	else if (!memcmp(devaddr, "M*", 2)) send[18] = 0x90;		//内部リレー
	else if (!memcmp(devaddr, "SM", 2)) send[18] = 0x91;		//特殊リレー
	else if (!memcmp(devaddr, "SD", 2)) send[18] = 0xA9;		//特殊レジスタ
	else if (!memcmp(devaddr, "X*", 2)) {					//入力リレー
		send[18] = 0x9C;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "Y*", 2)) {					//出力リレー
		send[18] = 0x9D;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "L*", 2)) send[18] = 0x92;		//ラッチリレー
	else if (!memcmp(devaddr, "F*", 2)) send[18] = 0x93;		//アナンシェータ
	else if (!memcmp(devaddr, "V*", 2)) send[18] = 0x94;		//エッジリレー
	else if (!memcmp(devaddr, "B*", 2)) {					//リンクリレー
		send[18] = 0xA0;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "W*", 2)) {					//リンクレジスタ
		send[18] = 0xB4;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "TS", 2)) send[18] = 0xC1;		//タイマ　接点
	else if (!memcmp(devaddr, "TC", 2)) send[18] = 0xC0;		//タイマ　コイル
	else if (!memcmp(devaddr, "TN", 2)) send[18] = 0xC2;		//タイマ　現在地
	else if (!memcmp(devaddr, "SS", 2)) send[18] = 0xC7;		//積算タイマ　接点
	else if (!memcmp(devaddr, "SC", 2)) send[18] = 0xC6;		//積算タイマ　コイル
	else if (!memcmp(devaddr, "SN", 2)) send[18] = 0xC8;		//積算タイマ　現在地
	else if (!memcmp(devaddr, "CS", 2)) send[18] = 0xC4;		//カウンタ　接点
	else if (!memcmp(devaddr, "CC", 2)) send[18] = 0xC3;		//カウンタ　コイル
	else if (!memcmp(devaddr, "CN", 2)) send[18] = 0xC5;		//カウンタ　現在地
	else if (!memcmp(devaddr, "SB", 2)) {					//特殊リンクリレー
		send[18] = 0xA1;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "SW", 2)) {					//特殊リンクレジスタ
		send[18] = 0xB5;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "S*", 2)) send[18] = 0x98;		//ステップリレー
	else if (!memcmp(devaddr, "DX", 2)) {					//ダイレクト入力
		send[18] = 0xA2;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "DY", 2)) {					//ダイレクト出力
		send[18] = 0xA3;
		hex_sign = 1;
	}
	else if (!memcmp(devaddr, "Z*", 2)) send[18] = 0xCC;		//インデックスレジスタ
	else if (!memcmp(devaddr, "R*", 2)) send[18] = 0xAF;		//ファイレジスタ
	else if (!memcmp(devaddr, "ZR", 2)) {					//ファイルレジスタ
		send[18] = 0xB0;
		hex_sign = 1;
	}
	else                     return(-100);

	// ④読み出しｱﾄﾞﾚｽ(8)
	if (hex_sign == 0) {
		memset(txt, 0, sizeof(txt));
		memcpy(txt, devaddr + 2, 6);
		top_addres = atoi(txt);
		memcpy(&send[15], (unsigned int*)&top_addres, 3);
	}
	else {
		memset(txt, 0, sizeof(txt));
		memcpy(txt, devaddr + 6, 2);
		send[15] = (unsigned char)htoi(txt, 2);//ディバイス点数  L
		memcpy(txt, devaddr + 4, 2);
		send[16] = (unsigned char)htoi(txt, 2);//ディバイス点数  -
		memcpy(txt, devaddr + 2, 2);
		send[17] = (unsigned char)htoi(txt, 2);//ディバイス点数  H	
	}

	// ⑤読み出し点数
	memcpy(&send[19], (unsigned short*)&counts, 2);//要求データ長


												   // ⑥送信データ
												   // ワードデバイス
	if (devtype == DEV_WORD) {
		// 1ワード=2バイトで表現
		for (i = 0; i<counts; i++) {
			// 2バイトに分ける(HL=>LH入れ替え)
			// 送信データ(data)は、文字列で4バイト=1ワードで入っている
			send[21 + (i * 2)] = (unsigned char)htoi(data + (i * 4) + 2, 2);	// L
			send[21 + (i * 2) + 1] = (unsigned char)htoi(data + (i * 4), 2);	// H
		}
		// 送信バイト数(1ワード=2バイト)
		send_bytes = 21 + counts * 2;
		/*
		// 1ワード=2バイトで表現
		for(i=0,j=21;i<counts*4;i=i+4,j=j+2){
		//2バイトに分ける
		send[j] = (unsigned char)htoi(data+(i+2),2);
		send[j+1] = (unsigned char)htoi(data+i,2);
		}
		*/
	}
	// BITデバイス
	else {
		// 2ビットを1バイトで表現
		for (i = 0; i<counts; i++) {
			if (data[i] == '1') {
				if (i % 2 == 0) send[21 + i / 2] += 0x10;
				else send[21 + i / 2] += 0x1;
			}
		}
		// 送信バイト数(2ビット=1バイト)
		send_bytes = 21 + counts / 2;
		if (counts % 2) send_bytes++;		// 奇数送信の場合は+1する
											/*
											for(i=0,j=21;i<counts;i++)
											{
											if(i % 2 == 0){
											if(data[i] == '1')
											send[j] = (unsigned short)16;
											else
											send[j] = (unsigned short)0;
											}
											else{
											if(data[i] == '1')
											send[j] += (unsigned short)1;
											else
											send[j] += (unsigned short)0;
											}

											//2ビット分を1バイトのデータに入れたらsend[]をひとつ進める
											if(i % 2 == 1)
											j++;
											}
											*/
	}
	/* いらない
	if(counts % 2 == 1)
	j++;
	send[j]=0;
	*/
	// 送信する
	ret = UdpSendB(ipaddr, port, (char*)send, send_bytes, buf, sizeof(buf));

	return(ret);
}


//========================================================================================================
//	int CMelsecP3E::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
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
int CMelsecP3E::UdpSendA(char* ipaddr, unsigned short port, char* data, int size, char* rcv, int rsize)
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
	if (INADDR_NONE == (laddr = inet_addr(ipaddr))) return(-101);

	fSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//	fSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (fSocket == INVALID_SOCKET) {
		printf("socket()Err");
		return(-102);
	}
	fEvent = WSACreateEvent();
	if (fEvent == WSA_INVALID_EVENT) {
		printf("\nWSACreateEvent()Err: %d\n", fEvent);
		closesocket(fSocket);
		return(-103);
	}

	ret = WSAEventSelect(fSocket, fEvent, FD_READ);
	if (ret == SOCKET_ERROR) {
		printf("\nWSAEventSelect()Err: %d\n", ret);
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-104);
	}

	// 送信データ作成
	wsBuf.len = size;
	wsBuf.buf = data;
	dwBufferCount = 1;
	dwFlags = MSG_DONTROUTE;//MSG_PARTIAL;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = laddr;
	addr.sin_port = htons(port);

	// 送信
	ret = WSASendTo(fSocket, &wsBuf, dwBufferCount, &cnt, dwFlags,
		(LPSOCKADDR)&addr, sizeof(addr), NULL, NULL);
	if (ret == SOCKET_ERROR) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-105);
	}

	// 受信待ち
	ret = WSAWaitForMultipleEvents(1, &fEvent, TRUE, 10000, TRUE);
	if (ret == WSA_WAIT_TIMEOUT) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-106);
	}

	ret = WSAEnumNetworkEvents(fSocket, fEvent, &events);
	if (ret == SOCKET_ERROR) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-107);
	}

	// 受信イベント以外
	if ((events.lNetworkEvents & FD_READ) == 0) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-108);
	}

	// 受信する
	//printf("Event Err FD=%d\n",FD_READ);
	memset(rbuf, 0, sizeof(rbuf));
	wsBuf.len = sizeof(rbuf);
	wsBuf.buf = rbuf;
	dwBufferCount = 1;
	dwFlags = 0;//MSG_PEEK;//MSG_PARTIAL;
	memset(&addr, 0, sizeof(addr));
	iFromlen = sizeof(addr);

	ret = WSARecvFrom(fSocket, &wsBuf, dwBufferCount, &cnt, &dwFlags,
		(LPSOCKADDR)&addr, &iFromlen, NULL, NULL);


	WSACloseEvent(fEvent);
	closesocket(fSocket);


	// 終了コードをチェック(先頭から18バイト)
	if (memcmp(rbuf + 18, "0000", 4)) {
		//printf("\nrcv=%s",rbuf);
		// 終了コードを出力して終了
		memcpy(rcv, rbuf + 18, 4);
		return(1);
	}

	// 受信データ長(先頭から14バイト目)、終了コード(4バイト)を含んでいるので引く
	ret = this->htoi(rbuf + 14, 4) - 4;

	// 受信データコピー
	if (ret>0) {
		memset(rcv, 0, rsize);
		memcpy(rcv, rbuf + 22, ret> rsize ? rsize : ret);
	}

	return ret;
}


//========================================================================================================
//	int CMelsecP3E::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
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
int CMelsecP3E::UdpSendB(char* ipaddr, unsigned short port, char* data, int size, char* rcv, int rsize)
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
	int i;
	unsigned short dcnt, *dat;

	// IPｱﾄﾞﾚｽ変換(to 32bit)
	if (INADDR_NONE == (laddr = inet_addr(ipaddr))) return(-101);

	fSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//	fSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (fSocket == INVALID_SOCKET) {
		printf("socket()Err");
		return(-102);
	}
	fEvent = WSACreateEvent();
	if (fEvent == WSA_INVALID_EVENT) {
		printf("\nWSACreateEvent()Err: %d\n", fEvent);
		closesocket(fSocket);
		return(-103);
	}

	ret = WSAEventSelect(fSocket, fEvent, FD_READ);
	if (ret == SOCKET_ERROR) {
		printf("\nWSAEventSelect()Err: %d\n", ret);
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-104);
	}

	// 送信データ作成
	wsBuf.len = size;
	wsBuf.buf = data;
	dwBufferCount = 1;
	dwFlags = MSG_DONTROUTE;//MSG_PARTIAL;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = laddr;
	addr.sin_port = htons(port);

	// 送信
	ret = WSASendTo(fSocket, &wsBuf, dwBufferCount, &cnt, dwFlags,
		(LPSOCKADDR)&addr, sizeof(addr), NULL, NULL);
	if (ret == SOCKET_ERROR) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-105);
	}

	// 受信待ち
	ret = WSAWaitForMultipleEvents(1, &fEvent, TRUE, 10000, TRUE);
	if (ret == WSA_WAIT_TIMEOUT) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-106);
	}

	ret = WSAEnumNetworkEvents(fSocket, fEvent, &events);
	if (ret == SOCKET_ERROR) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-107);
	}

	// 受信イベント以外
	if ((events.lNetworkEvents & FD_READ) == 0) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-108);
	}

	// 受信する
	//printf("Event Err FD=%d\n",FD_READ);
	memset(rbuf, 0, sizeof(rbuf));
	wsBuf.len = sizeof(rbuf);
	wsBuf.buf = rbuf;
	dwBufferCount = 1;
	dwFlags = 0;//MSG_PEEK;//MSG_PARTIAL;
	memset(&addr, 0, sizeof(addr));
	iFromlen = sizeof(addr);

	ret = WSARecvFrom(fSocket, &wsBuf, dwBufferCount, &cnt, &dwFlags,
		(LPSOCKADDR)&addr, &iFromlen, NULL, NULL);


	WSACloseEvent(fEvent);
	closesocket(fSocket);


	// 終了コードをチェック
	if (rbuf[9] != 0x00 || rbuf[10] != 0x00) {
		printf("\nrcv=%d%d", rbuf[9], rbuf[10]);
		// 終了コードを出力して終了
		//			memcpy( rcv,rbuf+18,4);
		sprintf(rcv, "%02d%02d", rbuf[9], rbuf[10]);
		return(1);
	}

	// 受信データ長(先頭から7,8バイト目)、終了コード(2バイト)を含んでいるので引く
	//2バイトで4文字分表せるので、2で割る
	memcpy(&dcnt, &rbuf[7], 2);
	dcnt = (dcnt - 2) / 2;

	//ret=rbuf[8]*100+rbuf[7]-2;

	// 受信データコピー
	if (dcnt>0)
	{
		memset(rcv, 0, rsize);
		for (i = 0; i<dcnt; i++)
		{
			// データ位置(受信データは11番目から)
			dat = (unsigned short*)(rbuf + 11 + i * 2);
			sprintf(&rcv[i * 4], "%04X", *dat);
		}
	}

	return(0);
}
//========================================================================================================
//	int CMelsecP3E::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
//	type　：private
//　機能　：UDP送受信(BITデバイス読み出し,BINARYモード専用)
//	引数１：ターゲットのIPアドレス
//	引数２：ターゲットのUDPポート番号
//	引数３：送信データ
//	引数４：送信データサイズ
//	引数５：受信バッファ
//	引数６：受信バッファサイズ
//	引数７：受信データ点数
//	戻り値：0=OK,0未満=ネットワークエラー、1=受信データエラー
//========================================================================================================
int CMelsecP3E::UdpSendBit(char* ipaddr, unsigned short port, char* data, int size, char* rcv, int rsize, short counts)
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
	int i;
	unsigned short dcnt;

	// IPｱﾄﾞﾚｽ変換(to 32bit)
	if (INADDR_NONE == (laddr = inet_addr(ipaddr))) return(-101);

	fSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//	fSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (fSocket == INVALID_SOCKET) {
		printf("socket()Err");
		return(-102);
	}
	fEvent = WSACreateEvent();
	if (fEvent == WSA_INVALID_EVENT) {
		printf("\nWSACreateEvent()Err: %d\n", fEvent);
		closesocket(fSocket);
		return(-103);
	}

	ret = WSAEventSelect(fSocket, fEvent, FD_READ);
	if (ret == SOCKET_ERROR) {
		printf("\nWSAEventSelect()Err: %d\n", ret);
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-104);
	}

	// 送信データ作成
	wsBuf.len = size;
	wsBuf.buf = data;
	dwBufferCount = 1;
	dwFlags = MSG_DONTROUTE;//MSG_PARTIAL;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = laddr;
	addr.sin_port = htons(port);

	// 送信
	ret = WSASendTo(fSocket, &wsBuf, dwBufferCount, &cnt, dwFlags,
		(LPSOCKADDR)&addr, sizeof(addr), NULL, NULL);
	if (ret == SOCKET_ERROR) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-105);
	}

	// 受信待ち
	ret = WSAWaitForMultipleEvents(1, &fEvent, TRUE, 10000, TRUE);
	if (ret == WSA_WAIT_TIMEOUT) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-106);
	}

	ret = WSAEnumNetworkEvents(fSocket, fEvent, &events);
	if (ret == SOCKET_ERROR) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-107);
	}

	// 受信イベント以外
	if ((events.lNetworkEvents & FD_READ) == 0) {
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-108);
	}

	// 受信する
	//printf("Event Err FD=%d\n",FD_READ);
	memset(rbuf, 0, sizeof(rbuf));
	wsBuf.len = sizeof(rbuf);
	wsBuf.buf = rbuf;
	dwBufferCount = 1;
	dwFlags = 0;//MSG_PEEK;//MSG_PARTIAL;
	memset(&addr, 0, sizeof(addr));
	iFromlen = sizeof(addr);

	ret = WSARecvFrom(fSocket, &wsBuf, dwBufferCount, &cnt, &dwFlags,
		(LPSOCKADDR)&addr, &iFromlen, NULL, NULL);


	WSACloseEvent(fEvent);
	closesocket(fSocket);

	// 終了コードをチェック
	if (rbuf[9] != 0x00 || rbuf[10] != 0x00) {
		printf("\nrcv=%d%d", rbuf[9], rbuf[10]);
		// 終了コードを出力して終了
		//			memcpy( rcv,rbuf+18,4);
		sprintf(rcv, "%02d%02d", rbuf[9], rbuf[10]);
		return(1);
	}

	// 受信データ長(先頭から7,8バイト目)、終了コード(2バイト)を含んでいるので引く
	memcpy(&dcnt, &rbuf[7], 2);
	dcnt = dcnt - 2;

	// 受信データコピー
	if (dcnt>0)
	{
		memset(rcv, 0, rsize);
		for (i = 0; i<dcnt; i++)
		{
			// データ位置(受信データは11番目から)
			sprintf(&rcv[i * 2], "%02X", rbuf[11 + i]);
		}
	}

	//データ読み込み点数が奇数の場合は、最後の1桁はダミーデータなので消す
	if (counts % 2 == 1)	rcv[counts] = 0;
	return(0);
}

//========================================================================================================
//	int CMelsecP3E::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	機能　：文字列を、Melsec送信形式に変換する
//			ex) "A1234"->"31413433"
//	引数１：出力バッファ
//	引数２：バッファサイズ（変換元データ長の２倍+1は必要）
//	引数３：変換元文字列データ(NULL-terminated)
//	返値　：0=OK
//========================================================================================================
int CMelsecP3E::Str2Mel(char* buf, unsigned int bufsize, char* str)
{
	unsigned int i, len;
	char txt[10];

	len = strlen(str);
	if (len * 2 >= bufsize) return(-1);

	memset(buf, 0, bufsize);

	for (i = 0; i<len; i++) {
		// 上下を入れ替える
		if (i + 1 == len)
			wsprintf((LPWSTR)txt, (LPCWSTR)"00");
		else
			wsprintf((LPWSTR)txt, (LPCWSTR)"%02X", (unsigned char)str[i + 1]);	// 2011.03.11 coba
																//			wsprintf( txt,"%02X",str[i+1]);
		memcpy(&buf[i * 2], txt, 2);

		wsprintf((LPWSTR)txt, (LPCWSTR)"%02X", (unsigned char)str[i]);		// 2011.03.11 coba
															//		wsprintf( txt,"%02X",str[i]);
		i++;
		memcpy(&buf[i * 2], txt, 2);
	}

	return(0);

}

//========================================================================================================
//	unsigned char CMelsecP3E::htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
int CMelsecP3E::Mel2Str(char* buf, unsigned int bufsize, char* melstr)
{
	unsigned int i, len;

	len = strlen(melstr);
	if (bufsize <= len / 2) return(-1);

	memset(buf, 0, bufsize);

	for (i = 0; i<len; i += 2) {
		// 上下を入れ替える
		if (i + 2 >= len)
			buf[strlen(buf)] = 0x20;
		else
			buf[strlen(buf)] = (char)htoi(&melstr[i + 2], 2);

		buf[strlen(buf)] = (char)htoi(&melstr[i], 2);
		i += 2;
	}
	return(0);
}

//========================================================================================================
//	unsigned char CMelsecP3E::htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
unsigned short CMelsecP3E::htoi(char *hexstr, short len)
{
	unsigned short ret = 0;
	short i;

	if (len > 4) return 0;
	for (i = 0; i < len; i++) {
		ret *= 16;
		if (hexstr[i] < 'A') ret += hexstr[i] - '0';
		else
			if (hexstr[i] < 'a') ret += hexstr[i] - 'A' + 10;
			else
				ret += hexstr[i] - 'a' + 10;
	}
	return(ret);
}


BOOL CMelsecP3E::ULChg(char* buf)
{
	char tmp[8];

	if (strlen(buf)<8)
		return FALSE;

	memcpy(tmp, buf, 8);
	memcpy(buf + 0, tmp + 4, 4);
	memcpy(buf + 4, tmp + 0, 4);

	return TRUE;
}




