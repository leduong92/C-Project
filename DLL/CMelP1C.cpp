
#include <stdio.h>
#include <conio.h>

CMelsecP1C::CMelsecP1C(void)
{
	m_fOpen=FALSE;
}

//========================================================================================================
//	CMelsecP1C::~CMelsecP1C()
//	デストラクタ
//	引数　：なし
//	返値　：
//　機能　：
//========================================================================================================
CMelsecP1C::~CMelsecP1C()
{
	TRS232C::SetRTS(FALSE);
	TRS232C::SetDTR(FALSE);
	TRS232C::Close();
}

//========================================================================================================
//	int CMelsecP1C::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type　：private
//	引数１：ポート№
//	引数２：ボーレート 1200,2400,4800,9600,19200
//	引数２：データビット 7,8
//	引数２：ｽﾄｯﾌﾟﾋﾞｯﾄ 1,2
//	引数２：パリティ 0=なし、 1=奇数、2=偶数
//	引数２：ウエイト
//	返値　：TRUE or FALSE
//　機能　：ポートを開く
//========================================================================================================
BOOL CMelsecP1C::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait)
{
	// ｵｰﾌﾟﾝ中はOK
	if( m_fOpen ) return(TRUE);

	m_Wait = wait;

	PORTSETTINGEX ps;
	memset( &ps, 0, sizeof(PORTSETTINGEX));

	switch( baudrate ){
		case 1200:
			ps.BauRate=CBR_1200;				//1200bps
			break;
		case 2400:
			ps.BauRate=CBR_2400;				//1200bps
			break;
		case 4800:
			ps.BauRate=CBR_4800;				//1200bps
			break;
		case 19200:
			ps.BauRate=CBR_19200;				//1200bps
			break;
		case 38400:
			ps.BauRate=CBR_38400;				//1200bps
			break;
		case 9600:
		default:
			ps.BauRate=CBR_9600;				//1200bps
			break;
	}

	// DataBit
	switch( databit ){
		case 7:
			ps.DByte=7;						// データﾋﾞｯﾄ=8
			break;
		case 8:
		default:
			ps.DByte=8;						// データﾋﾞｯﾄ=8
			break;
	}

	// STOP BIT
	switch( stopbit){
		case 1:
			ps.StopBits=ONESTOPBIT;			// ｽﾄｯﾌﾟﾋﾞｯﾄ=1
			break;
		case 2:
		default:
			ps.StopBits=TWOSTOPBITS;			// ｽﾄｯﾌﾟﾋﾞｯﾄ=2
			break;
	}

	// PARITY
	switch( parity){
		case 1:
			ps.Parity=ODDPARITY;				// 奇数パリティ
			break;
		case 2:									// 
			ps.Parity=EVENPARITY;				// 偶数パリティ
			break;
		case 0:
		default:
			ps.Parity=NOPARITY;				// パリティなし
			break;
	}
	ps.fBinary = TRUE;
	ps.fOutxCtsFlow = TRUE;		// TRUE or FALSE
	ps.fOutxDsrFlow = TRUE;		// TRUE or FALSE
	ps.fDtrControl = DTR_CONTROL_HANDSHAKE;		// DTR_CONTROL_DISABLE or DTR_CONTROL_ENABLE or DTR_CONTROL_HANDSHAKE
	ps.fRtsControl = RTS_CONTROL_HANDSHAKE;		// RTS_CONTROL_ENABLE or RTS_CONTROL_DISABLEE or RTS_CONTROL_HANDSHAKE or RTS_CONTROL_TOGGLE


	if(!TRS232C::CommOpenEx((long)portNo,&ps)){
		return(FALSE);
	}

	TRS232C::SetRTS(TRUE);
	TRS232C::SetDTR(TRUE);

	// モデムのポートをクリア
	TRS232C::ClearRcvBuffer();
	m_fOpen=TRUE;

	return(TRUE);
}

//========================================================================================================
//	int CMelsecP1C::P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
//　機能　：プロトコル 形式1でワードデータをブロック受信
//	引数１：受信バッファ
//	引数２：受信バッファサイズ
//	引数３：局番
//	引数４：PC局番
//	引数５：受信先頭アドレス
//	引数６：受信ブロック数(max64)
//	type　：public
//========================================================================================================
int CMelsecP1C::P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
{
	int ret,len;
	int retry=0,sum;
	char data[20],txt[10];

	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 64 ) return(-2);

	memset( data, 0, sizeof(data));
	// 送信ループ
	while( retry < 8 ){
		//----送信文字列の作成-----
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "WR",addr,counts,"");

		TRS232C::ClearRcvBuffer();				// モデムのポートをクリア
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );

		//-----結果受信------
		ret = Receive( data, sizeof(data), 1 );		// 1バイト受信してSTX or NAKを調べる
		if( ret ) return(-3);

		// 残りの処理数をコマンド別に決める
		//-----STXの場合(data到着)------
		if( data[0] == STX ){
			// ETXになるまで受信を続ける
			len =0;
			memset( m_buf, 0, sizeof(m_buf) );
			while( data[0] != ETX ){
				ret = Receive( data, sizeof(data), 1 );			// 残りのバイト数を受信する
				if( ret ) return(-4);							// エラー
				m_buf[len] = data[0];							// バッファにため込む
				if( ++len == sizeof(m_buf) ) return(-5); 						// バッファオーバーフロー
			}

			// 残り(2byte)を受信
			ret = Receive( data, sizeof(data), 2 );
			if( ret ) return(-6);							// エラー

			// サムチェックを比較
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			// OKの場合はACKを返して正常終了
			if( !memcmp( txt, data, 2 ) ){
				Answer(ACK,station,pcstation);
				break;
			}
			// NGの時はNAKを返す
			Answer(NAK,station,pcstation);
			retry++;										// ﾘﾄﾗｲする
			continue;
		}

		//-----NAKの場合------
		else if( data[0] == NAK ){
			ret = Receive( data, sizeof(data), 6 );			// 残りのバイト数を受信する
			if( ret ) return(-7);
			retry++;										// ﾘﾄﾗｲする
			continue;
		}

		//----- エラーだ。-------
		else return(-8);
	}

	// 受信データをバッファにｺﾋﾟｰする
	memcpy( buf, m_buf+4,(int)(strlen(m_buf+4)-1) > bufsize ? bufsize:strlen(m_buf+4)-1);

	return(0);
}

//========================================================================================================
//	int CMelsecP1C::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//　機能　：プロトコル 形式1でワードをブロック送信
//	type　：private
//	引数１：局番
//	引数２：PC局番
//	引数３：送信先頭アドレス
//	引数４：送信ブロック数(max64)
//	引数５：送信データ
//========================================================================================================
int CMelsecP1C::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
{
	int ret,len;
	int retry=0;
	char data[20];
	char initcom[5];

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 64 ) return(-2);

	// 長さオーバー
	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);

	if( counts*4 != (int)strlen(msg) ) return(-11);

	// 送信文字列の作成
	MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "WW",addr,counts,msg);

	// semd-loop
	while( retry < 8 ){
		if( retry ){
			TRS232C::ClearRcvBuffer();				// モデムのポートをクリア
			ret = TRS232C::Write( (BYTE*)initcom, 3 );
		}		
		// 送信
		TRS232C::ClearRcvBuffer();				// モデムのポートをクリア
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// 結果受信
		ret = Receive( data, sizeof(data), 1 );		// 1バイト受信してACK or NAKを調べる
		if( ret ){
			if( retry++ < 8 )continue;
			return(-3);
		}
		// 残りのバイト数をコマンド別に決める
		if( data[0] == ACK )  len = 4 ;		// ACKの場合
		else if( data[0] == NAK )  len = 6;	// NAKの場合
		else{
			if( retry++ < 8 )continue;
			return(-4);						// エラーだ。
		}
		// 残りのバイト数を受信する
		ret = Receive( data, sizeof(data), len );
		if( ret ){
			if( retry++ < 8 )continue;
			return(-5);
		}

		// NAKの場合はエラーとしてﾘﾄﾗｲする
		if( len == 6 ) retry++;

		// ACKの場合はOK
		else break;
	}
	return(0);
}

//========================================================================================================
//	int CMelsecP1C::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//　機能　：プロトコル 形式1でワードをブロック送信
//	type　：private
//	引数１：局番
//	引数２：PC局番
//	引数３：送信先頭アドレス
//	引数４：送信ブロック数(max64)
//	引数５：送信データ
//========================================================================================================
int CMelsecP1C::P4Send( char* station, char* pcstation,char* command,char* addr, short counts, char* msg)
{
	int ret,len;
	int retry=0;
	char data[20];
	char initcom[5];

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);
//	if( counts <0 || counts > 64 ) return(-2);
	if( counts <0 ) return(-2);

	// 長さオーバー
	if( strlen(addr)+strlen(msg) + P4_HEADLEN+2 >= sizeof(m_buf) ) return(-10);

//	if( counts*4 != (int)strlen(msg) ) return(-11);

	// 送信文字列の作成
	MakeCmd( m_buf, sizeof(m_buf), station,pcstation, command,addr,counts,msg);

	// semd-loop
	while( retry < 8 ){
		if( retry ){
			TRS232C::ClearRcvBuffer();				// モデムのポートをクリア
			ret = TRS232C::Write( (BYTE*)initcom, 3 );
		}		
		// 送信
		TRS232C::ClearRcvBuffer();				// モデムのポートをクリア
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// 結果受信
		ret = Receive( data, sizeof(data), 1 );		// 1バイト受信してACK or NAKを調べる
		if( ret ){
			if( retry++ < 8 )continue;
			return(-3);
		}
		// 残りのバイト数をコマンド別に決める
		if( data[0] == ACK )  len = 6 ;		// ACKの場合
		else if( data[0] == NAK )  len = 8;	// NAKの場合
		else{
			if( retry++ < 8 )continue;
			return(-4);						// エラーだ。
		}
		// 残りのバイト数を受信する
		ret = Receive( data, sizeof(data), len );
		if( ret ){
			if( retry++ < 8 )continue;
			return(-5);
		}
		// NAKの場合はエラーとしてﾘﾄﾗｲする
		if( len == 8 ) retry++;

		// ACKの場合はOK
		else break;
	}
	return(0);
}


//========================================================================================================
//	int CMelsecP1C::P4ReadB( char* station,char* pcstation,char* addr)
//	type　：public
//　機能　：プロトコル 形式1でビットデータを１デバイス受信
//	引数３：局番
//	引数４：PC局番
//	引数５：受信先頭アドレス
//  戻り値：0=OFF,1=ON,0未満=エラー
//========================================================================================================
int CMelsecP1C::P4ReadB( char* station,char* pcstation,char* addr)
{
	int ret,len;
	int retry=0,sum;
	char data[20],txt[10];
	char buf;

	if( m_fOpen == FALSE ) return(-1);

	memset( data, 0, sizeof(data));
	// 送信ループ
	while( retry < 8 ){
		//----送信文字列の作成-----
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "BR",addr,8,"");

		TRS232C::ClearRcvBuffer();				// モデムのポートをクリア
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );

		//-----結果受信------
		ret = Receive( data, sizeof(data), 1 );		// 1バイト受信してSTX or NAKを調べる
		if( ret ) return(-3);

		// 残りの処理数をコマンド別に決める
		//-----STXの場合(data到着)------
		if( data[0] == STX ){
			// ETXになるまで受信を続ける
			len =0;
			memset( m_buf, 0, sizeof(m_buf) );
			while( data[0] != ETX ){
				ret = Receive( data, sizeof(data), 1 );			// 残りのバイト数を受信する
				if( ret ) return(-4);							// エラー
				m_buf[len] = data[0];							// バッファにため込む
				if( ++len == sizeof(m_buf) ) return(-5); 						// バッファオーバーフロー
			}

			// 残り(4byte)を受信
			ret = Receive( data, sizeof(data), 2 );
			if( ret ) return(-6);							// エラー

			// サムチェックを比較
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			// OKの場合はACKを返して正常終了
			if( !memcmp( txt, data, 2 ) ){
				Answer(ACK,station,pcstation);
				break;
			}
			// NGの時はNAKを返す
			Answer(NAK,station,pcstation);
			retry++;										// ﾘﾄﾗｲする
			continue;
		}

		//-----NAKの場合------
		else if( data[0] == NAK ){
			ret = Receive( data, sizeof(data), 6 );			// 残りのバイト数を受信する
			if( ret ) return(-7);
			retry++;										// ﾘﾄﾗｲする
			continue;
		}

		//----- エラーだ。-------
		else return(-8);
	}

	// 受信データをバッファにｺﾋﾟｰする
	buf=m_buf[4];

	return(buf-0x30);
}
//========================================================================================================
//	int CMelsecP1C::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//　機能　：プロトコル 形式1でビットデバイス(1点)をON/OFF
//	type　：private
//	引数１：局番
//	引数２：PC局番
//	引数３：送信アドレス
//	引数５：送信データ 0=OFF,それ以外=ON
//	戻り値：0.=成功 それ以外=エラー
//========================================================================================================
int CMelsecP1C::P4WriteB( char* station, char* pcstation,char* addr, int onoff)
{
	int ret,len;
	int retry=0;
	char data[20];
	char send[10];

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);

	// 送信文字列の作成
	if( !onoff ) strcpy(send,"0\0");
	else strcpy(send,"1\0");				// off

	MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "BW",addr,1,send);

	// semd-loop
	while( retry < 8 ){
		// 送信
		TRS232C::ClearRcvBuffer();				// モデムのポートをクリア
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// 結果受信
		ret = Receive( data, sizeof(data), 1 );		// 1バイト受信してACK or NAKを調べる
		if( ret ) return(-3);

		// 残りのバイト数をコマンド別に決める
		if( data[0] == ACK )  len = 4 ;		// ACKの場合
		else if( data[0] == NAK )  len = 6;	// NAKの場合
		else return(-4);						// エラーだ。

		// 残りのバイト数を受信する
		ret = Receive( data, sizeof(data), len );
		if( ret ) return(-5);

		// NAKの場合はエラーとしてﾘﾄﾗｲする
		if( len == 6 ) retry++;

		// ACKの場合はOK
		else break;
	}
	return(0);
}

//========================================================================================================
//	int CMelsecP1C::Receive( char* buf, int bufsize, int waitlen )
//　機能　：受信バッファからデータを読み出す
//	type　：private
//	引数１：バッファ
//	引数２：バッファサイズ
//	引数３：受信待ちするデータバイト数
//========================================================================================================
int CMelsecP1C::Receive( char* buf, int bufsize, int waitlen )
{
	int theLen,prevLen=0;
	int counter = 0;

	// 未オープンエラー
	if( m_fOpen == FALSE ) return(-1);
	if( waitlen <1 ) return(-2);
	if( bufsize < waitlen ) return(-2);

	memset( buf, 0, bufsize );

	while(1){
		// 受信バッファにデータが来ているか調べる
		theLen = TRS232C::ReadLength();
		// 指定したバイト数に達した
		if( theLen >= waitlen ){
			break;
		}
		// 指定したバイト数に満たない
		else{
			Sleep(15);
			if( theLen != prevLen ){	// 前より増えていたらカウンタリセット
				theLen = prevLen;
				counter=0;
			}
			else counter++;				// 受信バイト数が変わらない
		}
		// タイムアウト
		if( counter > 40 ) return(-3);
	}

	// 正常終了
	TRS232C::Read( (BYTE*)buf, waitlen );
	return(0);
}

//========================================================================================================
//	int CMelsecP1C::Answer( char flg, char* station,char* pc )
//　機能　：PLCに返事を返す
//	type　：private
//	引数１：ACK or NAK
//	引数２：局番
//	引数３：ＰＣ局番
//========================================================================================================
int CMelsecP1C::Answer( char flg, char* station,char* pc )
{
	char buf[10];
	int ret;

	memset( buf, 0, sizeof(buf));

	buf[0]= flg;
	memcpy( buf+1, station, 2 );
	memcpy( buf+3, pc, 2 );

// 1Cは不要
//	buf[5]=CR;
//	buf[6]=LF;

	ret = TRS232C::Write( (BYTE*)buf, strlen(buf) );
	return(0);
}

//========================================================================================================
//	void CMelsecP1C::MakeCmd(char* buf,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
//　機能　：プロトコル 形式1での送信データを作成
//	引数１：作成バッファ
//	引数２：作成バッファサイズ
//	引数３：局番
//	引数４：PC局番
//	引数５：コマンド
//	引数６：先頭アドレス
//	引数７：デバイスブロック数(max64)
//	type　：public
//========================================================================================================
void CMelsecP1C::MakeCmd(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
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

// 1Cは不要
//	buf[strlen (buf)] = CR;
//	buf[strlen (buf)] = LF;
}


//========================================================================================================
//	int CMelsecP1C::GetCheckSum(char* str )
//	type　：private
//	引数　：計算する文字列
//	返値　：算出したsamチェック
//　機能　：sumチェックの計算
//========================================================================================================
int CMelsecP1C::GetCheckSum(char* str )
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
//	int CMelsecP1C::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	機能　：文字列を、Melsec送信形式に変換する
//			ex) "A1234"->"31413433"
//	引数１：出力バッファ
//	引数２：バッファサイズ（変換元データ長の２倍+1は必要）
//	引数３：変換元文字列データ(NULL-terminated)
//	返値　：0=OK
//========================================================================================================
int CMelsecP1C::Str2Mel(char* buf, unsigned int bufsize, char* str)
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
			//wsprintf( txt,"%02X",str[i+1]);
		memcpy( &buf[i*2], txt, 2 );

		wsprintf( txt,"%02X",(unsigned char)str[i]);		// 2011.03.11 coba
		//wsprintf( txt,"%02X",str[i]);
		i++;
		memcpy( &buf[i*2], txt, 2 );
	}

	return(0);

}

//========================================================================================================
//	unsigned char CMelsecP1C::htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
int CMelsecP1C::Mel2Str(char* buf, unsigned int bufsize, char* melstr)
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
//	unsigned char CMelsecP1C::htoi( char *hexstr , short len )
//	機能　：16進（文字列２文字のみ）を整数値(unsigned char型)に変換する
//	引数　：変換元データ格納ポインタ
//	返値　：変換後データ
//========================================================================================================
unsigned char CMelsecP1C::htoi( char *hexstr , short len )
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


BOOL CMelsecP1C::ULChg( char* buf )
{
	char tmp[8];

	if( strlen(buf)<8 )
		return FALSE;

	memcpy( tmp , buf , 8);
	memcpy( buf+0 , tmp+4 , 4 );
	memcpy( buf+4 , tmp+0 , 4 );
	
	return TRUE;
}
