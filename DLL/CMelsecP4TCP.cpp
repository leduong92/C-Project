//================================================================================================
//	FileName	:CMelsecP4TCP.cpp
//	Class		:CMelsecP4TCP
// 				:Melsec プロトコル４形式通信onTCP/IP
//				 Serial-Ethernetコンバータのサーバモード用に、通信内容をTCPに乗せて送る
//	sumチェック有り、xon/off制御なし。
//	作成日		:2013.09.20 coba
//================================================================================================
#include "CMelsecP4TCP.h"
#include <stdio.h>

//========================================================================================================
//	CMelsecP4TCP::CMelsecP4TCP(void)
//	引数　：なし
//	返値　：
//　機能　：コンストラクタ。デフォルトサイズでデータバッファを作成
//========================================================================================================
CMelsecP4TCP::CMelsecP4TCP(void)
{
	m_fOpen=FALSE;
}

//========================================================================================================
//	CMelsecP4TCP::~CMelsecP4TCP()
//	デストラクタ
//	引数　：なし
//	返値　：
//　機能　：
//========================================================================================================
CMelsecP4TCP::~CMelsecP4TCP()
{
	WSACleanup();
}


//========================================================================================================
//	BOOL CMelsecP4TCP::Init(void)
//	type　：private
//	機能　：初期化
//	返値　：TRUE or FALSE
//========================================================================================================
BOOL CMelsecP4TCP::Init(int wait)
{
	// ｵｰﾌﾟﾝ中はOK
	if( m_fOpen ) return(TRUE);

  	WSADATA			WSAData;

	m_Wait = wait;

	// ソケット初期化
  	if(WSAStartup( MAKEWORD(2,2), &WSAData)) return(-1);


	m_fOpen=TRUE;

	return(TRUE);
}

//========================================================================================================
//	int CMelsecP4TCP::P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
//　機能　：プロトコル４形式でワードデータをブロック受信
//	引数１：受信バッファ
//	引数２：受信バッファサイズ
//	引数３：局番
//	引数４：PC局番
//	引数５：受信先頭アドレス
//	引数６：受信ブロック数(max64)
//	type　：public
//========================================================================================================
int CMelsecP4TCP::P4ReadW( char* ipaddr,unsigned short port,char* buf,int bufsize
												,char* station,char* pcstation,char* addr, short counts)
{
	int ret,len;
	int retry=0,sum;
	char data[10],txt[10],rcv[1024];
	char initcom[5];

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

//printf("\n");
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 64 ) return(-2);

	memset( data, 0, sizeof(data));
	// 送信ループ
	while( retry < 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}		
		//----送信文字列の作成-----
		// 通常コマンド
		if(strlen(addr) == 5 )
			MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "WR",addr,counts,"");
		// 2013.02.17 coba 拡張コマンド(D1023以降対応)
		else MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "QR",addr,counts,"");

		//-----データ送受信------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),rcv,sizeof(rcv));
		if( len < 1){
			printf("len=%d",len);
			return(-3);
		}
		// 残りの処理数をコマンド別に決める
		//-----STXの場合(data到着)------
		if( rcv[0] == STX ){
			// 1レジスタ読み出しでも最低14バイトある
			if( len < 14 ){
				retry++;										// ﾘﾄﾗｲする
				continue;
			}

			// 最初のSTXと、最後4つ(sumチェックcrlf)を覗いてコピ-
			memset( m_buf, 0, sizeof(m_buf) );
			memcpy( m_buf,rcv+1,len-5);

			// サムチェック部をコピー
			memcpy( data,rcv+len-4,2);

			// サムチェックを比較
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			// OKの場合はACKを返して正常終了
			if( !memcmp( txt, data, 2 ) ){
				Answer(ipaddr,port,ACK,station,pcstation);
				break;
			}
			// NGの時はNAKを返す
			Answer(ipaddr,port,NAK,station,pcstation);
			retry++;										// ﾘﾄﾗｲする
			continue;
		}

		//-----NAKの場合------
		else if( rcv[0] == NAK ){
			retry++;										// ﾘﾄﾗｲする
			continue;
		}

		//----- エラーだ。-------
		else return(-8);
	}
	if(retry >= 8) return(-9);

	// ETXを外して受信データをバッファにｺﾋﾟｰする
	memcpy( buf, m_buf+4,(int)strlen(m_buf+4)-1 > bufsize ? bufsize:strlen(m_buf+4)-1);

	return(0);
}

//========================================================================================================
//	int CMelsecP4TCP::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//　機能　：プロトコル４形式でワードをブロック送信
//	type　：private
//	引数１：局番
//	引数２：PC局番
//	引数３：送信先頭アドレス
//	引数４：送信ブロック数(max64)
//	引数５：送信データ
//========================================================================================================
int CMelsecP4TCP::P4WriteW( char* ipaddr,unsigned short port,char* station, char* pcstation,char* addr, short counts, char* msg)
{
	int ret,len;
	int retry=0;
	char data[128];
	char initcom[5];

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 64 ) return(-2);

	// 長さオーバー
	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);

	if( counts*4 != (int)strlen(msg) ) return(-11);

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;


	// 送信文字列の作成
	// 通常コマンド
	if(strlen(addr) == 5 )
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "WW",addr,counts,msg);
	// 2013.02.17 coba 拡張コマンド(D1023以降対応)
	else
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "QW",addr,counts,msg);

	// semd-loop
	while( retry < 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}		
		//-----データ送受信------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),data,sizeof(data));
		if( len < 1){
			if( retry++ < 8 )continue;
			return(-3);
		}
		// 残りのバイト数をコマンド別に決める
		if( data[0] == ACK )  break;		// ACKの場合
		else{
			if( retry++ < 8 )continue;
			return(-4);						// エラーだ。
		}
	}
	// 2013.04.11 coba リトライ回数overでも0で返していた
	if(retry >= 8) return(-9);
	return(0);
}
//========================================================================================================
//	int CMelsecP4TCP::P4ReadB( char* station,char* pcstation,char* addr)
//	type　：public
//　機能　：プロトコル４形式でビットデータを１デバイス受信
//	引数３：局番
//	引数４：PC局番
//	引数５：受信先頭アドレス
//  戻り値：0=OFF,1=ON,0未満=エラー
//========================================================================================================
int CMelsecP4TCP::P4ReadB( char* ipaddr,unsigned short port,char* station,char* pcstation,char* addr)
{
	int ret,buf;
	char data[20];

	memset(data,0,sizeof(data));
	ret=P4ReadBB(ipaddr,port,data,sizeof(data),station,pcstation,addr,1);
	if(ret) return(ret);

	// 受信データをバッファにｺﾋﾟｰする
	buf=data[0]-0x30;

	return(buf);
}
//========================================================================================================
//	int CMelsecP4TCP::P4ReadB( char* station,char* pcstation,char* addr)
//	type　：public
//　機能　：プロトコル４形式でビットデータを１デバイス受信
//	引数３：局番
//	引数４：PC局番
//	引数５：受信先頭アドレス
//  戻り値：0=OFF,1=ON,0未満=エラー
//========================================================================================================
int CMelsecP4TCP::P4ReadBB(  char* ipaddr,unsigned short port,char* buf,int bufsize
											,char* station,char* pcstation,char* addr,short cnt)
{
	int ret,len;
	int retry=0,sum;
	char data[10],txt[10],rcv[1024];
	char initcom[5];

	if( m_fOpen == FALSE ) return(-1);

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	memset( data, 0, sizeof(data));
	// 送信ループ
	while( retry < 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}

		//----送信文字列の作成-----
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "BR",addr,cnt,"");

		//-----データ送受信------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),rcv,sizeof(rcv));
		if( len < 1) return(-3);

		// 残りの処理数をコマンド別に決める
		//-----STXの場合(data到着)------
		if( rcv[0] == STX ){
			// 1bit読み出しでも最低11バイトある
			if( len < 11){
				retry++;										// ﾘﾄﾗｲする
				continue;
			}
			// 最初のSTXと、最後4つ(sumチェックcrlf)を覗いてコピ-
			memset( m_buf, 0, sizeof(m_buf) );
			memcpy( m_buf,rcv+1,len-5);

			// サムチェック部をコピー
			memcpy( data,rcv+len-4,2);

			// サムチェックを比較
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			// OKの場合はACKを返して正常終了
			if( !memcmp( txt, data, 2 ) ){
				Answer(ipaddr,port,ACK,station,pcstation);
				break;
			}
			// NGの時はNAKを返す
			Answer(ipaddr,port,NAK,station,pcstation);
			retry++;										// ﾘﾄﾗｲする
			continue;
		}

		//-----NAKの場合------
		else if( rcv[0] == NAK ){
			retry++;										// ﾘﾄﾗｲする
			continue;
		}

		//----- エラーだ。-------
		else return(-8);
	}

	// 2013.04.11 coba リトライ回数overでも0で返していた
	if(retry >= 8) return(-9);

	// ETXカットして受信データをバッファにｺﾋﾟｰする
	memcpy( buf, m_buf+4,(int)(strlen(m_buf+4)-1) > bufsize ? bufsize:strlen(m_buf+4)-1);

	return(0);
}

//========================================================================================================
//	int CMelsecP4TCP::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//　機能　：プロトコル４形式でビットデバイス(1点)をON/OFF
//	type　：private
//	引数１：局番
//	引数２：PC局番
//	引数３：送信アドレス
//	引数５：送信データ 0=OFF,それ以外=ON
//	戻り値：0.=成功 それ以外=エラー
//========================================================================================================
int CMelsecP4TCP::P4WriteB( char* ipaddr,unsigned short port,char* station, char* pcstation,char* addr, int onoff)
{
	int ret;
	char send[12];

	// 送信文字列の作成
	if( !onoff ) strcpy(send,"0\0");
	else strcpy(send,"1\0");				// off

	ret=P4WriteBB(ipaddr,port,station,pcstation,addr,1,send);
	return(ret);
}
//========================================================================================================
// int CMelsecP4TCP::P4WriteBB( char* station, char* pcstation,char* addr, int cnt,char* dat)
//	2012.06.20 coba追加
//　機能　：プロトコル４形式でビットデバイス複数点を一括ON/OFF
//	type　：private
//	引数１：局番
//	引数２：PC局番
//	引数３：送信アドレス
//	引数５：送信データ 0=OFF,1=ONの配列
//	戻り値：0.=成功 それ以外=エラー
//========================================================================================================
int CMelsecP4TCP::P4WriteBB( char* ipaddr,unsigned short port,char* station, char* pcstation,char* addr, int cnt,char* dat)
{
	int ret,len;
	int retry=0;
	char data[128];
	char initcom[5];

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);

	// セットするレジスタ数=データバイト数なのでチェック
	if( (int)strlen(dat) != cnt ) return(-2);

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "BW",addr,cnt,dat);

	// semd-loop
	while( retry < 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}		

		//-----データ送受信------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),data,sizeof(data));
		if( len < 1){
			if( retry++ < 8 )continue;
			return(-3);
		}

		// 残りのバイト数をコマンド別に決める
		if( data[0] == ACK ) break;		// ACKの場合
		else retry++;
	}

	// 2013.04.11 coba リトライ回数overでも0で返していた
	if(retry >= 8) return(-9);
	return(0);
}



//========================================================================================================
//	int CMelsecP4TCP::P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
//　機能　：プロトコル４形式でワードデータをブロック受信
//	引数１：受信バッファ
//	引数２：受信バッファサイズ
//	引数３：局番
//	引数４：PC局番
//	引数５：受信先頭アドレス
//	引数６：受信ブロック数(max64)
//	type　：public
//========================================================================================================
int CMelsecP4TCP::P2C4ReadW( char* ipaddr,unsigned short port,char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
{
	int ret,len;
	int retry=0,sum;
	char rcv[4096],data[10],txt[10];
	char initcom[5];

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 960 ) return(-2);

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	// semd-loop
	while( retry <= 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}

		// 送信文字列の作成(2C)
		MakeCmd2C( m_buf, sizeof(m_buf), station,pcstation, "2",addr,counts,"");

		//-----データ送受信------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),rcv,sizeof(rcv));
		if( len < 1) return(-3);

		// 残りの処理数をコマンド別に決める
		//-----STXの場合(data到着)------
		if( rcv[0] == STX ){
			// 1レジスタ読み出しでも最低16バイトある
			if( len < 16 ){
				retry++;										// ﾘﾄﾗｲする
				continue;
			}

			// 最初のSTXと、最後4つ(sumチェックcrlf)を覗いてコピ-
			memset( m_buf, 0, sizeof(m_buf) );
			memcpy( m_buf,rcv+1,len-5);

			// サムチェック部をコピー
			memcpy( data,rcv+len-4,2);

			// サムチェックを比較
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			if( !memcmp( txt, data, 2 ) ) break;
		}
		else retry++;
	}
	// 2013.04.11 coba リトライ回数overでも0で返していた
	if(retry >= 8) return(-9);

	// 受信データをバッファにｺﾋﾟｰする
	memcpy( buf, m_buf+6,(int)(strlen(m_buf+6)-1) > bufsize ? bufsize:strlen(m_buf+6)-1);

	return(0);

	
}

//========================================================================================================
//	int CMelsecP4TCP::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//　機能　：プロトコル４形式でワードをブロック送信
//	type　：private
//	引数１：局番
//	引数２：PC局番
//	引数３：送信先頭アドレス
//	引数４：送信ブロック数(max64)
//	引数５：送信データ
//========================================================================================================
int CMelsecP4TCP::P2C4WriteW( char* ipaddr,unsigned short port,char* station, char* pcstation,char* addr, short counts, char* msg)
{
	int ret,len;
	int retry=0;
	char data[20];
	char initcom[5];

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 960 ) return(-2);

	// 長さオーバー
	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);

	if( counts*4 != (int)strlen(msg) ) return(-11);

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	// 送信文字列の作成(2C)
	MakeCmd2C( m_buf, sizeof(m_buf), station,pcstation, "4",addr,counts,msg);

	// semd-loop
	while( retry <= 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}		

		//-----データ送受信------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),data,sizeof(data));
		if( len < 1){
			if( retry++ < 8 )continue;
			return(-3);
		}
		// 残りのバイト数をコマンド別に決める
		if( data[0] == ACK )  break;		// ACKの場合
		else{
			if( retry++ < 8 )continue;
			return(-4);						// エラーだ。
		}
	}
	// 2013.04.11 coba リトライ回数overでも0で返していた
	if(retry >= 8) return(-9);
	return(0);
}



//========================================================================================================
//	int CMelsecP4TCP::Answer( char flg, char* station,char* pc )
//　機能　：PLCに返事を返す
//	type　：private
//	引数１：ACK or NAK
//	引数２：局番
//	引数３：ＰＣ局番
//========================================================================================================
int CMelsecP4TCP::Answer( char* ip ,unsigned short port,char flg, char* station,char* pc )
{
	char buf[10];
	int ret;

	memset( buf, 0, sizeof(buf));

	buf[0]= flg;
	memcpy( buf+1, station, 2 );
	memcpy( buf+3, pc, 2 );
	buf[5]=CR;
	buf[6]=LF;

	ret=TcpSend( ip,port,buf,strlen(buf),NULL,0);
	return(0);
}

//========================================================================================================
//	void CMelsecP4TCP::MakeCmd(char* buf,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
//　機能　：プロトコル４形式での送信データを作成
//	引数１：作成バッファ
//	引数２：作成バッファサイズ
//	引数３：局番
//	引数４：PC局番
//	引数５：コマンド
//	引数６：先頭アドレス
//	引数７：デバイスブロック数(max64)
//	type　：public
//========================================================================================================
void CMelsecP4TCP::MakeCmd(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
{
	char txt[10];
	int sum;

	memset( buf, 0, bufsize );
	buf[0] = ENQ;

	// 局番、PC局番
	strcat( buf, station );
	strcat( buf, pc );

	// コマンド
	strcat( buf, cmd );

	// ウエイト
	wsprintf( txt, "%d", m_Wait );
	memcpy( &buf[strlen(buf)], txt, 1 );

	// 先頭デバイス
	strcat( buf, addr );

	// デバイス点数
	wsprintf( txt, "%02X", counts );
	memcpy( &buf[strlen(buf)], txt, 2 );

	// 送信メッセージ
	strcat( buf, msg );

	// さむチェック
	sum = GetCheckSum(&buf[1]);
	wsprintf( txt, "%02X", sum );
	memcpy( &buf[strlen(buf)], txt, 2 );

	buf[strlen (buf)] = CR;
	buf[strlen (buf)] = LF;
}

//========================================================================================================
//	void CMelsecP4TCP::MakeCmd(char* buf,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
//　機能　：２Ｃ プロトコル４形式での送信データを作成
//	引数１：作成バッファ
//	引数２：作成バッファサイズ
//	引数３：局番
//	引数４：PC局番
//	引数５：コマンド
//	引数６：先頭アドレス
//	引数７：デバイスブロック数(max64)
//	type　：public
//========================================================================================================
void CMelsecP4TCP::MakeCmd2C(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
{
	char txt[10];
	int sum;

	memset( buf, 0, bufsize );
	buf[0] = ENQ;
	buf[1] = 'F';
	buf[2] = 'B';


	// 局番、PC局番
	strcat( buf, station );
	strcat( buf, pc );

	// コマンド
	strcat( buf, cmd );


	// 先頭デバイス (8) Ex:ZR123456
	strcat( buf, addr );

	// デバイス点数 (4)
	wsprintf( txt, "%04X", counts );
	memcpy( &buf[strlen(buf)], txt, 4 );

	// 送信メッセージ
	strcat( buf, msg );

	// さむチェック
	sum = GetCheckSum(&buf[1]);
	wsprintf( txt, "%02X", sum );
	memcpy( &buf[strlen(buf)], txt, 2 );

	buf[strlen (buf)] = CR;
	buf[strlen (buf)] = LF;
}


//========================================================================================================
//	int CMelsecP4TCP::GetCheckSum(char* str )
//	type　：private
//	引数　：計算する文字列
//	返値　：算出したsamチェック
//　機能　：sumチェックの計算
//========================================================================================================
int CMelsecP4TCP::GetCheckSum(char* str )
{
	int data=0;
	int i,len;

	if( str==NULL ) return(0);

	len = strlen(str);
	if( len <=0 ) return(0);

	for( i=0; i< len; i++ )	data += (int)str[i];

	data = data & 0x000000ff;

	return(data);
}
//========================================================================================================
//	int CMelsecP4TCP::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	機能　：文字列を、Melsec送信形式に変換する
//			ex) "A1234"->"31413433"
//	引数１：出力バッファ
//	引数２：バッファサイズ（変換元データ長の２倍+1は必要）
//	引数３：変換元文字列データ(NULL-terminated)
//	返値　：0=OK
//========================================================================================================
int CMelsecP4TCP::Str2Mel(char* buf, unsigned int bufsize, char* str)
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
			wsprintf( txt,"%02X",(unsigned char)str[i+1]);		// 2011.03.11 coba
//			wsprintf( txt,"%02X",str[i+1]);
		memcpy( &buf[i*2], txt, 2 );

		wsprintf( txt,"%02X",(unsigned char)str[i]);		// 2011.03.11
//		wsprintf( txt,"%02X",str[i]);
		i++;
		memcpy( &buf[i*2], txt, 2 );
	}

	return(0);

}

//========================================================================================================
//	unsigned char CMelsecP4TCP::htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
int CMelsecP4TCP::Mel2Str(char* buf, unsigned int bufsize, char* melstr)
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
//	unsigned char CMelsecP4TCP::htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
unsigned char CMelsecP4TCP::htoi( char *hexstr , short len )
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
//========================================================================================================
//	int CMelsecP4TCP::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	機能　：文字列を、Melsec送信形式に変換する
//			ex) "A1234"->"31413433"
//	引数１：出力バッファ
//	引数２：バッファサイズ（変換元データ長の２倍+1は必要）
//	引数３：変換元文字列データ(NULL-terminated)
//	返値　：0=OK
//========================================================================================================
int CMelsecP4TCP::UStr2Mel(char* buf, unsigned int bufsize, unsigned char* str)
{
	unsigned int i,len;
	char txt[10];

	len = strlen((char*)str);
	if( len*2 >= bufsize ) return(-1);

	memset( buf, 0, bufsize );

	for( i=0;i<len;i++){
		// 上下を入れ替える
		if( i+1==len )
			wsprintf( txt,"00");
		else
			wsprintf( txt,"%02X",(unsigned)str[i+1] & 0xFF);
		memcpy( &buf[i*2], txt, 2 );

		wsprintf( txt,"%02X",(unsigned)str[i] & 0xFF);
		i++;
		memcpy( &buf[i*2], txt, 2 );
	}

	return(0);

}


BOOL CMelsecP4TCP::ULChg( char* buf )
{
	char tmp[8];

	if( strlen(buf)<8 )
		return FALSE;

	memcpy( tmp , buf , 8);
	memcpy( buf+0 , tmp+4 , 4 );
	memcpy( buf+4 , tmp+0 , 4 );
	
	return TRUE;
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
int CMelsecP4TCP::TcpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
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
	printf( "WLAN=port=%d\n",WSAGetLastError());
		closesocket(soc);
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

	// 受信なし
	if( rcv==NULL){
		shutdown(soc, 2);							// 2:SHUT_RDWR
		closesocket(soc);
		return(0);
	}


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
			// CR-LF受信
			if( rbuf[len2-2]==CR && rbuf[len2-1]==LF){
				// NAKの場合
				if(rbuf[0]==NAK ) break;
				// ACKの場合
				if(rbuf[0]==ACK ) break;
				// STXの場合
				if(rbuf[0]==STX && len2 > 5){
					if(rbuf[len2-5]==ETX) break;
				}
			}
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

	if(!len2) return(-117);

	memset(rcv,0,rsize);
	memcpy( rcv, rbuf, len2> rsize ? rsize:len2);
	Sleep(10);	// ウエイト
	return(len2);
}
