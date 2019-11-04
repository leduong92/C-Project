//===========================================================================
//
//  	さとーバーコードプリンターの制御基本クラス
//	
//		FileName : tsatoprt.cpp
//		ＮＯＫ株式会社 技術本部 生産技術部
//		2001.6.15 coba
//===========================================================================


#include "tsatoprtcom.h"
#include <stdio.h>

//---------------------------------------------------------------------------------------------------------------------
//	TSatoPrinter::TSatoPrinter( char* ptrname, int ptrtype, BOOL* status )
//	タイプ：public
//	機能　：ポート指定コンストラクタ
//	引数１：プリンタタイプ 400系=0,それ以外=410
//	引数２：使用するポート番号
//	引数３：ボーレート
//	引数４：データビット
//	引数５：ストップビット
//	引数６：パリティ
//	引数７：（出力）初期化ｽﾃｰﾀｽ True or False
//---------------------------------------------------------------------------------------------------------------------
TSatoPrinter::TSatoPrinter( int ptrtype,int portNo, int speed,int data,int stop,int parity, BOOL* status )
{
	*status = FALSE;
	m_Com = NULL;

	// 232C開く
	if( InitPrinter(ptrtype, portNo,speed,data,stop,parity)){
		return;
	}

	*status = TRUE;

}
//---------------------------------------------------------------------------------------------------------------------
//	TSatoPrinter::TSatoPrinter( char* ptrname, int ptrtype, BOOL* status )
//	タイプ：public
//	機能　：デフォルトコンストラクタ
//		この後、InitPrinter()で初期化が必要
//---------------------------------------------------------------------------------------------------------------------
TSatoPrinter::TSatoPrinter()
{
	m_Com = NULL;
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TSatoPrinter::InitPrinter( int portNo, PORTSETTING* pset,int prtype)
//	タイプ：public
//	機能　：ポート指定コンストラクタ
//	引数１：使用するポート番号
//	引数２：ポートの設定
//	引数３：プリンタタイプ 400系=0,それ以外=410
//	戻り値：初期化ｽﾃｰﾀｽ True or False
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::InitPrinter( int ptrtype,int portNo, int speed,int data,int stop,int parity)
{
	PORTSETTING ps;

	// 初期化済み
	if( m_Com != NULL ) return(-1);

	// メンバ変数の初期化
	m_prtType = ptrtype;


	memset( &ps, 0, sizeof(PORTSETTING));
	switch( speed ){
		case 300:
			ps.BauRate=CBR_300;				//1200bps
			break;
		case 600:
			ps.BauRate=CBR_600;				//1200bps
			break;
		case 1200:
			ps.BauRate=CBR_1200;				//1200bps
			break;
		case 2400:
			ps.BauRate=CBR_2400;				//bps
			break;
		case 4800:
			ps.BauRate=CBR_4800;				//bps
			break;
		case 14400:
			ps.BauRate=CBR_14400;				//bps
			break;
		case 19200:
			ps.BauRate=CBR_19200;				//bps
			break;
		case 28800:
			ps.BauRate=28800;				//bps
			break;
		case 38400:
			ps.BauRate=CBR_38400;				//bps
			break;
		case 57600:
			ps.BauRate=CBR_57600;				//bps
			break;
		case 115200:
			ps.BauRate=CBR_115200;				//bps
			break;
		case 9600:
		default:
			ps.BauRate=CBR_9600;				//1200bps
			break;
	}

	// DataBit
	switch( data ){
		case 7:
			ps.DByte=7;						// データﾋﾞｯﾄ=8
			break;
		case 8:
		default:
			ps.DByte=8;						// データﾋﾞｯﾄ=8
			break;
	}

	// STOP BIT
	switch( stop){
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

	// 232C開く
	m_Com = NULL;
	m_Com = new TRS232C;
	if( NULL == m_Com ) return(-2);

	if(portNo<1) portNo=1;

	// TRs232Cクラスのインスタンスを作成
	if(!m_Com->CommOpen(portNo,&ps)){
		delete m_Com;
		m_Com = NULL;
		return(-3);
	}

	// 0.1mmあたりのドット数を算出
	m_dpm = 0.0;
	// 400系(203dpi 1ドット=0.125mm)
	if( ptrtype==SATO_ER400)m_dpm = 0.1/0.125;
	// 410系(305dpi 1ドット=0.083mm)
	else m_dpm = 0.1/0.083;

	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	TSatoPrinter::~TSatoPrinter()
//	タイプ：public
//	機能　：デストラクタ
//---------------------------------------------------------------------------------------------------------------------
TSatoPrinter::~TSatoPrinter()
{
	if( m_Com !=NULL){
		m_Com->Close();
		delete m_Com;
		m_Com = NULL;
	}
}


//---------------------------------------------------------------------------------------------------------------------
//	TSatoPrinter::TSatoPrinter( char* ptrname, int ptrtype, BOOL* status )
//	タイプ：public
//	機能　：印刷スタート
//			232C版では、互換を保つ為のダミー
//---------------------------------------------------------------------------------------------------------------------
BOOL TSatoPrinter::PrintStart(void)
{
	if( m_Com == NULL ) return(FALSE);

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	TSatoPrinter::TSatoPrinter( char* ptrname, int ptrtype, BOOL* status )
//	タイプ：public
//	機能　：印刷終了
//---------------------------------------------------------------------------------------------------------------------
BOOL TSatoPrinter::PrintEnd(void)
{
	if( m_Com == NULL ) return(TRUE);
	m_Com->Close();
	delete m_Com;
	m_Com = NULL;

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::StartLabel(int mode)
//	タイプ：public
//	機能　：いわゆるStartPage()
//	引数１：回転 コマンドESC+%に対応
//			（プリンタ正面からみて）垂直印刷=0
//			1=90度回転
//			2=180度回転(逆さ)
//			3=270度回転(逆さ)
//	引数２：印字速度 2～6
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::StartLabel(int mode,char speed)
{
	if( m_Com == NULL ) return(-1);

	char cmd[20];
	int ret;

	// ﾃﾞｰﾀ送信開始 STX+ESCA
	cmd[0] = S_STX;
	cmd[1] = S_ESC;
	cmd[2] = 'A';
	cmd[3] = 0;
	if( mode >0 && mode <4){
		cmd[3] = S_ESC;
		cmd[4] = '%';
		cmd[5] = '0'+(char)mode;
		cmd[6] = 0;
	}
	ret = SendData(cmd,(int)strlen(cmd));
	if(ret)return(ret);

	// S-JISコード
	// ﾃﾞｰﾀ送信開始 ESCKC
	cmd[0] = S_ESC;
	cmd[1] = 'K';
	cmd[2] = 'C';
	cmd[3] = '1';					// 1=S-JIS
	cmd[4] = 0;
	ret = SendData(cmd,(int)strlen(cmd));
	if(ret)return(ret);

	// 連続動作offset指定 ESCPO
	cmd[0] = S_ESC;
	cmd[1] = 'P';
	cmd[2] = 'O';
	cmd[3] = '3';
	cmd[4] = '+';
	cmd[5] = '0';
	cmd[6] = '0';
	cmd[7] = 0;
	ret = SendData(cmd,(int)strlen(cmd));
	if(ret)return(ret);

	// TearOff動作offset指定 ESCPO
	cmd[0] = S_ESC;
	cmd[1] = 'P';
	cmd[2] = 'O';
	cmd[3] = '2';
	cmd[4] = '+';
	cmd[5] = '0';
	cmd[6] = '0';
	cmd[7] = 0;
	ret = SendData(cmd,(int)strlen(cmd));
	if(ret)return(ret);

	// 印字速度
	if( speed < 2 ) speed = 2;		// 最低値
	if( speed > 6 ) speed = 6;

	// 410系は４迄
	if( m_prtType==SATO_ER410 && speed > 4 ) speed=4;
	cmd[0] = S_ESC;
	cmd[1] = 'C';
	cmd[2] = 'S';
	cmd[3] =  speed+0x30;
	cmd[4] = 0;
	ret = SendData(cmd,(int)strlen(cmd));


	return(ret);

}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::EndLabel(short page)
//	タイプ：public
//	機能　：いわゆるEndPage()
//	引数　：ｺﾋﾟｰ枚数
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::EndLabel(long page)
{
	if( m_Com == NULL ) return(-1);

	// 発行枚数
	if( page < 1 || page > 999999 ) page=1;

	char cmd[20];

	// ﾃﾞｰﾀ送信終了ESCQ+maisu+ESCZ+ETX
	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	cmd[1] = 'Q';
	wsprintf( &cmd[2],"%ld", page);
	cmd[strlen(cmd)]=S_ESC;
	cmd[strlen(cmd)]='Z';
	cmd[strlen(cmd)]=S_ETX;

	return( SendData(cmd,(int)strlen(cmd)) );

}

//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::DrawStringOutline( int x,int y,char* str, int len,short type,short yoko,short tate,short propaty);
//	タイプ：private
//	機能　：Ascii文字の印刷(漢字はだめ)
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：文字列
//	引数４：文字数
//	引数５：文字種類の指定 'A'=ヘルベチカボールド(プロポーショナル)
//                         'B'=ヘルベチカボールド(文字間ピッチ固定)
//	引数６：文字幅(50-999)
//	引数７：文字高さ(50-999)
//	引数８：フォント形状(0-9)
//          0=標準(黒）
//          1=白抜き
//          2=灰色文字(パターン1）
//          3=灰色文字(パターン2）
//          4=灰色文字(パターン3）
//          5=影付き文字
//          6=白抜き影付き文字
//          7=鏡反転文字
//          8=標準斜体文字
//          9=白抜き影付き斜体文字
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::DrawStringOutline( int x,int y,char* str, int len,short type,short yoko,short tate,short propaty)
{
	if( m_Com == NULL ) return(-1);

	char cmd[50];

	SetPosition(x,y);

	// 縦横倍率
	if( yoko < 50 || yoko >999) yoko=50;
	if( tate < 50 || tate >999) tate=50;
	if( type!='A' && type!='B' ) type='A'; 
	if( propaty<0 || propaty>9 ) propaty=0;

	// 文字セット
	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%c$%c,%03d,%03d,%d",S_ESC,type,yoko,tate,propaty);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));
	cmd[strlen(cmd)] = S_ESC;
	cmd[strlen(cmd)] = '$';
	cmd[strlen(cmd)] = '=';
	SendData(cmd,(int)strlen(cmd) );
	return( SendData(str, len) );
}


//---------------------------------------------------------------------------------------------------------------------
//	int  TSatoPrinter::SendData(char *data)
//	タイプ：private
//	機能　：Ascii文字の印刷(漢字はだめ)
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：文字列
//	引数４：文字数
//	引数５：文字サイズ(0-4)
//	引数６：横倍率(1-12)
//	引数７：縦倍率(1-12)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::DrawString( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch)
{
	if( m_Com == NULL ) return(-1);

	char cmd[50];

	SetPosition(x,y);

	// 縦横倍率
	if( yoko < 1 || yoko >12 ) yoko=1;
	if( tate < 1 || tate >12 ) tate=1;

	if( pitch < 0 || pitch > 99 ) pitch=2;

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP%02d",S_ESC,pitch);
//	wsprintf( &cmd[strlen(cmd)],"%cP%2d",S_ESC,pitch);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));
	cmd[strlen(cmd)] = S_ESC;
	cmd[strlen(cmd)] = 'L';
	wsprintf( &cmd[strlen(cmd)],"%02d%02d",yoko,tate);

	SendData(cmd,(int)strlen(cmd) );


	// 文字サイズ
	if( size < 0 || size > 4 ) size=0;
	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	// スムージング機能なし
	if( size < 3 )
		wsprintf( &cmd[strlen(cmd)],"X2%d,",size);
	else
		wsprintf( &cmd[strlen(cmd)],"X2%d,1",size);

	SendData(cmd,(int)strlen(cmd) );

	return( SendData(str, len) );
}


//---------------------------------------------------------------------------------------------------------------------
//	int	 DrawKanji( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch)
//	タイプ：private
//	機能　：全半混合漢字文字列印刷（プリンタの設定をS-JISにしておくこと）
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：文字列
//	引数４：文字数
//	引数５：文字サイズ(0(16*16)-1(24*24))
//	引数６：横倍率(1-12)
//	引数７：縦倍率(1-12)
//	引数７：文字間ピッチ(0-99)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::DrawKanji( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch)
{
	if( m_Com == NULL ) return(-1);

	char cmd[50];

	SetPosition(x,y);

	// 縦横倍率
	if( yoko < 1 || yoko >12 ) yoko=1;
	if( tate < 1 || tate >12 ) tate=1;

	if( pitch < 0 || pitch > 99 ) pitch=2;

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP%02d",S_ESC,pitch);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));
	cmd[strlen(cmd)] = S_ESC;
	cmd[strlen(cmd)] = 'L';
	wsprintf( &cmd[strlen(cmd)],"%02d%02d",yoko,tate);

	SendData(cmd,(int)strlen(cmd) );


	// 文字サイズ
	if( !size  ) size=5;		// 16*16
	else size=6;				// 24*24
	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	wsprintf( &cmd[strlen(cmd)],"K%dC",size);

	SendData(cmd,(int)strlen(cmd) );

	// 漢字の上位ビットと下位ビットの入れ替え
	unsigned short a,b,i;
	for( i=0; i< len; i++){
		// 次の文字は漢字か？
		if( IsDBCSLeadByteEx(CP_ACP, str[i])==TRUE){
			b=(unsigned short)str[i];
			b= b << 8;
			b= b >> 8;
			a=(unsigned short)str[++i];
			a= a << 8;
			b= a|b;
			SendData((char*)&b,sizeof(short) );
		}
		// 半角の場合はそのまま送る
		else{
			SendData(&str[i],1 );
		}
	}

	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::Bar39( int x,int y,int h,char* bcr,BOOL guide,int hiritu,int kakudai);
//	タイプ：public
//	機能　：code39ﾊﾞｰｺｰﾄﾞの印字
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：ﾊﾞｰｺｰﾄﾞ高さ(0.1mm)
//	引数４：ﾊﾞｰｺｰﾄﾞ文字列
//	引数５：文字数
//	引数６：ガイド文字有無
//	引数７：文字サイズ(0-4)
//	引数８：比率 0=1,2(ﾃﾞﾌｫﾙﾄ) 1=1:3, 2=2:5
//	引数９：横1倍率(1-12)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::Bar39( int x,int y,short h,char* bcr, int len, BOOL guide,int hiritu,short kakudai)
{
	if( m_Com == NULL ) return(-1);

	int pos;
	char cmd[30];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP00",S_ESC);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));


	// 比率
	switch(hiritu){
		case 1:
			wsprintf( cmd,"%cB1",S_ESC);
			break;
		case 2:
			wsprintf( cmd,"%cBD1",S_ESC);
			break;
		default:
			wsprintf( cmd,"%cD1",S_ESC);
	}
	// 横拡大率
	if( kakudai < 1 || kakudai >12 ) kakudai =1;
	// バー高さ
	pos = (int)((double)h*m_dpm);
	if( pos<1 || pos > 600 ) pos=20;
	wsprintf( &cmd[strlen(cmd)], "%02d%03d", kakudai,pos);

	SendData(cmd,(int)strlen(cmd) );
	return( SendData(bcr, len) );

}
//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::Bar128( int x,int y,int h,char* bcr,BOOL guide,int hiritu,int kakudai);
//	タイプ：public
//	機能　：code128ﾊﾞｰｺｰﾄﾞの印字
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：ﾊﾞｰｺｰﾄﾞ高さ(0.1mm)
//	引数４：ﾊﾞｰｺｰﾄﾞ文字列
//	引数５：文字数
//	引数６：ガイド文字有無
//	引数７：文字サイズ(0-4)
//	引数９：横1倍率(1-12)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::Bar128( int x,int y,short h,char* bcr, int len, BOOL guide,short kakudai)
{
	if( m_Com == NULL ) return(-1);

	int pos;
	char cmd[30];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP00",S_ESC);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));


	wsprintf( cmd,"%cBG",S_ESC);

	// 横拡大率
	if( kakudai < 1 || kakudai >12 ) kakudai =1;
	// バー高さ
	pos = (int)((double)h*m_dpm);
	if( pos<1 || pos > 600 ) pos=20;
	wsprintf( &cmd[strlen(cmd)], "%02d%03d", kakudai,pos);

	SendData(cmd,(int)strlen(cmd) );
	return( SendData(bcr, len) );

}

//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::BarQR2( int x,int y,char err,short size,short mode,short joint,char* bcr, int len,short c1,short c2,char* c3)
//	タイプ：public
//	機能　：QR2次元ﾊﾞｰｺｰﾄﾞの印字 (Model2)
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：エラーコレクション(誤り訂正）レベル('L'=7%、'M'=15%、'Q'=25%、'H'=30%)
//  引数４：セル一片のサイズ指定(1-32 Dot)
//  引数５：入力モード(0=自動設定、1=数字、2=英数字、3=漢字:SJIS)
//	引数６：連結 (0=通常モード、1=連結モード)
//	引数７：ﾊﾞｰｺｰﾄﾞ文字列
//	引数８：文字数
//  以下連結モードの時指定--------------------
//	引数９：連結モード分割数 (1-16)
//	引数10：連結モード分割した通番(1-16)
//	引数11：連結モードパリティデータ(00-FF)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::BarQR2( int x,int y,char err,short size,short mode,short joint,char* bcr, int len,short c1,short c2,short c3)
{
	if( m_Com == NULL ) return(-1);

	char cmd[64];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP00",S_ESC);
	SendData(cmd,(int)strlen(cmd) );

	// コマンド（１／２）
	memset( cmd, 0, sizeof(cmd));
	wsprintf( cmd,"%c2D30",S_ESC);
	//Mode
	if( err=='L' || err=='l' ) wsprintf( &cmd[strlen(cmd)], ",L");
	else if( err=='M' || err=='m' ) wsprintf( &cmd[strlen(cmd)], ",M");
	else if( err=='H' || err=='h' ) wsprintf( &cmd[strlen(cmd)], ",H");
	else if( err=='Q' || err=='q' ) wsprintf( &cmd[strlen(cmd)], ",Q");
	else wsprintf( &cmd[strlen(cmd)], ",L");
	//Size
	if( size<1 || size > 32 ) size=10;
	wsprintf( &cmd[strlen(cmd)], ",%02d", size);
	//Data Mode
	if( mode<0 || mode > 3 ) mode=0;
	if( mode==0 )
		wsprintf( &cmd[strlen(cmd)], ",1");	// Auto
	else
		wsprintf( &cmd[strlen(cmd)], ",0");	// Manual
	//Joint
	if( joint<0 || joint > 1 ) joint=0;
	wsprintf( &cmd[strlen(cmd)], ",%1d", joint);
	if( joint==1 ){
		if( c1<1 || c1 > 16 ) c1=1;
		if( c2<1 || c2 > 16 ) c2=1;
		if( c3<0 || c3 > 255 ) c3=0;
		wsprintf( &cmd[strlen(cmd)], ",%02d,%02d,%02X", c1,c2,c3);
	}

	SendData(cmd,(int)strlen(cmd) );



	// コマンド（２／２）マニュアルモード
	if( mode!=0 ){
		memset( cmd, 0, sizeof(cmd));
		wsprintf( cmd,"%cDS",S_ESC);
		wsprintf( &cmd[strlen(cmd)], "%1d,", mode);
		SendData(cmd,(int)strlen(cmd) );
		return( SendData(bcr, len) );
	}else
	// コマンド（２／２）オートモード
	{
		memset( cmd, 0, sizeof(cmd));
		wsprintf( cmd,"%cDN",S_ESC);
		wsprintf( &cmd[strlen(cmd)], "%04d,", len);
		SendData(cmd,(int)strlen(cmd) );
		return( SendData(bcr, len) );
	}
}
//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::BarPDF417( int x,int y,int h,char* bcr,BOOL guide,int hiritu,int kakudai);
//	タイプ：public
//	機能　：PDF417 2次元ﾊﾞｰｺｰﾄﾞの印字
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：最小モジュール幅(1-9dot)
//	引数４：最小モジュールピッチ寸法(1-24dot)
//	引数５：セキュリティ（誤り訂正）レベル(0-8)
//	引数６：１行当たりのデータコードワード(1-30)
//	引数７：シンボル当たりの行数(3-90)
//	引数８：コード種(0=通常,1=切り詰め)
//	引数９：ﾊﾞｰｺｰﾄﾞ文字列
//	引数10：文字数
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::BarPDF417( int x,int y,short module,short pitch,short ayamari,short data_w,short symbol_row,short type,char* bcr, int len)
{
	if( m_Com == NULL ) return(-1);

//	int pos;
	char cmd[30];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP00",S_ESC);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));

	if( module < 1 || module >9 ) module =1;
	if( pitch < 1 || pitch >24 ) pitch =1;
	if( ayamari < 0 || ayamari >8 ) ayamari =0;
	if( data_w < 1 || data_w >30 ) data_w =1;
	if( symbol_row < 3 || symbol_row >90 ) symbol_row =3;
	if( type < 0 || type >1 ) type =0;



	wsprintf( cmd,"%c2D10,%02d,%02d,%d,%02d,%02d",S_ESC,module,pitch,ayamari,data_w,symbol_row);
	if( type )
		wsprintf( &cmd[strlen(cmd)], ",%1d", type);
	SendData(cmd,(int)strlen(cmd) );

	// データ部
	wsprintf( cmd,"%cDN%04d,",S_ESC,len);
	SendData(cmd,(int)strlen(cmd) );
	return( SendData(bcr, len) );

}
//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::BarMAXI( int x,int y,short mode,char* bcr,int len,short service,short country,short post)
//	タイプ：public
//	機能　：MAXI  2次元ﾊﾞｰｺｰﾄﾞの印字
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：モード 2=配送専用　　(印字データ最大 数字のみ=123/英数混在=84)
//                 3=配送専用　　(印字データ最大 数字のみ=123/英数混在=84)
//                 4=標準記号　　(印字データ最大 数字のみ=138/英数混在=93)
//                 6=読み取り機専用　　(印字データ最大 数字のみ=138/英数混在=93)
//	引数４：ﾊﾞｰｺｰﾄﾞ文字列
//	引数５：文字数
//	引数６：サービスクラス (1-999) モード4,6は省略
//	引数７：国コード(1-999) モード4,6は省略
//	引数９：郵便コード(0-999999) モード4,6は省略
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::BarMAXI( int x,int y,short mode,char* bcr,int len,short service,short country,char* post)
{
	if( m_Com == NULL ) return(-1);

	char cmd[30];
	char tmp[30];
	long postV;
	int maxDatLen;

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP00",S_ESC);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));

	if( mode!=2 && mode!=3 && mode!=4 && mode!=6 ) mode=6;
	if( service < 1 || service > 999 ) service=1;
	if( country < 1 || country > 999 ) country=1;

	wsprintf( cmd,"%c2D20,",S_ESC);

	// モード

	if( mode==2 ){
		// Mode 2
		postV=atoi(post);
		if( postV < 0 || postV > 999999 ) postV=0;
		wsprintf( &cmd[strlen(cmd)], "%01d,%03d,%03d,%06ld", mode,service,country,postV);
		maxDatLen=84;
	}else if( mode==3 ){
		// Mode 3
		memset(tmp,0,sizeof(tmp));
		memset(tmp,' ' , 6 );
		memcpy(tmp,post,strlen(post)<6?strlen(post):6);
		wsprintf( &cmd[strlen(cmd)], "%01d,%03d,%03d,%s", mode,service,country,tmp);
		maxDatLen=84;
	}else{
		// Mode 4,5
		wsprintf( &cmd[strlen(cmd)], "%01d", mode);
		maxDatLen=93;
	}
	SendData(cmd,(int)strlen(cmd) );

	// データ部
	wsprintf( cmd,"%cDN%",S_ESC);
	if( len > maxDatLen )len=maxDatLen;
	wsprintf( &cmd[strlen(cmd)], "%04d,", len);
	SendData(cmd,(int)strlen(cmd) );
	return( SendData(bcr, len) );

}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::DrawFrame(int x,int y,int xlen,int ylen,short linewidth )
//	タイプ：public
//	機能　：枠線を引く（回転させている場合は、座標及び長さに注意）
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：x方向長さ(0.1mm)
//	引数４：y方向長さ(0.1mm)
//	引数５：線の太さ(0.1mm)
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::DrawFrame(int x,int y,int xlen,int ylen,short linewidth )
{
	if( m_Com == NULL ) return(-1);

	int pos,xl,yl;
	char cmd[50];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	pos = (int)((double) linewidth *m_dpm);
	xl = (int)((double) xlen *m_dpm);
	yl = (int)((double) ylen *m_dpm);
	if( pos < 2 || pos >99 ) pos=2;
	wsprintf( cmd,"%cFW%02d%02dV%04ldH%04ld",S_ESC,pos ,pos,yl,xl );
	return(SendData(cmd,(int)strlen(cmd) ) );
}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::DrawLine(int x,int y,int len,char direct,short linewidth )
//	タイプ：public
//	機能　：罫線を引く（回転させている場合は、座標及び長さに注意）
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：長さ(0.1mm)
//	引数４：方向 'H' or 'V'
//	引数５：線の太さ(0.1mm)
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::DrawLine(int x,int y,int len,char direct,short linewidth )
{
	if( m_Com == NULL ) return(-1);

	int pos,l;
	char cmd[50];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	memset( cmd, 0, sizeof(cmd));

	pos = (int)((double) linewidth *m_dpm);
	l = (int)((double) len *m_dpm);
	if( pos < 2 || pos >99 ) pos=2;
	wsprintf( cmd,"%cFW%02d%c%04ld",S_ESC,pos ,direct,l );
	return(SendData(cmd,(int)strlen(cmd) ) );
}

//---------------------------------------------------------------------------------------------------------------------
//	void TSatoPrinter::SetPosition( int x, int y)
//	タイプ：private
//	機能　：カレント座標の移動
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//---------------------------------------------------------------------------------------------------------------------
void TSatoPrinter::SetPosition( int x, int y)
{

	int pos;
	char cmd[50];

	// 印字位置送信（垂直）
	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	cmd[1] = 'V';
	pos = (int)( (double)y * m_dpm);
	wsprintf( &cmd[2],"%ld",pos);

	// 印字位置送信（水平）
	cmd[strlen(cmd)] = S_ESC;
	cmd[strlen(cmd)] = 'H';
	pos = (int)( (double)x * m_dpm);
	wsprintf( &cmd[strlen(cmd)],"%ld",pos);

	SendData(cmd,(int)strlen(cmd) );

}

//---------------------------------------------------------------------------------------------------------------------
//	int  TSatoPrinter::SendData(char *data)
//	タイプ：private
//	機能　：ﾃﾞｰﾀをプリンタに送信
//	引数　：送信文字列
//---------------------------------------------------------------------------------------------------------------------
int  TSatoPrinter::SendData(char *data,int len)
{
	int stat=0;

	if( m_Com == NULL ) return(-1);

	stat = m_Com->Write((BYTE*)data,(DWORD)len);

	// 書き込み結果 !=書き込み数
	if(stat !=len ){
		return 1;
	}

	return 0;
}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::SetPageSize( int x, int y );
//	タイプ：public
//	機能　：用紙サイズ指定
//	引数１：xサイズ(0.1mm)
//	引数２：yサイズ(0.1mm)
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::SetPageSize( int x, int y )
{
	char cmd[256];

	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	wsprintf( &cmd[1],"A1%04d%04d",(long)(m_dpm*y),(long)(m_dpm*x));
	return(SendData(cmd,(int)strlen(cmd) ) );

}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::SetLabelSize(int sx,int sy,int lx,int ly,int slx,int sly,int nx,int ny)
//	タイプ：public
//	機能　：ラベルサイズ指定
//  詳細はsatoマニュアル ESC+RI を参照
//  指定は全て ０．１ｍｍ
//	引数１：「横シートサイズ（用紙領域幅）」 ＝ 有効範囲 ： １５００～３６００（ドット）
//	引数２：「縦シートサイズ（用紙領域高さ）」 ＝ 有効範囲 ： ４８０～３６００（ドット）
//	引数３：「横ラベル間サイズ」 ＝ 有効範囲 ： ０～６０（ドット）
//	引数４：「縦ラベル間サイズ」 ＝ 有効範囲 ： ００～６０（ドット）
//	引数５：「小ラベル横用紙サイズ（用紙サイズ幅）」 ＝ 有効範囲 ： ４８０～３２００（ドット）
//	引数６：「小ラベル縦用紙サイズ（用紙サイズ高さ）」 ＝ 有効範囲 ： ４８０～３６００（ドット）
//	引数７：「横ラベル枚数」 ＝ 有効範囲 ： ０１～０６（枚）
//	引数８：「縦ラベル枚数」 ＝ 有効範囲 ： ０１～０７（枚）
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::SetLabelSize(int sx,int sy,int lx,int ly,int slx,int sly,int nx,int ny)
{
 	char cmd[256];

	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
 	wsprintf( &cmd[1],"RI%04d,%04d,%02d,%02d,%04d,%04d,%02d,%02d"
		,(long)(m_dpm*sx)
		,(long)(m_dpm*sy)
		,(long)(m_dpm*lx)
		,(long)(m_dpm*ly)
		,(long)(m_dpm*slx)
		,(long)(m_dpm*sly)
		,nx
		,ny
		);
	return(SendData(cmd,(int)strlen(cmd) ) );

}
//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::SetOffset( int x, int y );
//	タイプ：public
//	機能　：基点補正指定
//	引数１：xサイズ(0.1mm)
//	引数２：yサイズ(0.1mm)
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::SetOffset( int x, int y )
{
	char cmd[256];

	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	if( y >=0 )
		wsprintf( &cmd[1],"A3V+%d%",(long)(m_dpm*y));
	else
		wsprintf( &cmd[1],"A3V%d%",(long)(m_dpm*y));

	if( x >=0 )
		wsprintf( &cmd[strlen(cmd)],"H+%d%",(long)(m_dpm*x));
	else
		wsprintf( &cmd[strlen(cmd)],"H%d%",(long)(m_dpm*x));

	return(SendData(cmd,(int)strlen(cmd) ) );

}

//int TSatoPrinter::SetPrinter( SATO_LESPRIT_SETTING *setting );
int TSatoPrinter::SetPrinter( SATO_LESPRIT_SETTING *setting )
{
	char cmd[1024];

	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_STX;
	cmd[1] = S_ESC;
	cmd[2] = 'A';
	cmd[3] = 0;
	cmd[4] = S_ESC;
	cmd[5] = 'P';
	cmd[6] = 'G';
	wsprintf( cmd+7,"0000%02X00%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%04X%04X%04X%04X%02X%02X%02X%02X%02X%02X%02X"
		,setting->PrintSpeed
		,setting->Cutter
		,setting->Hakuri
		,setting->NonSepa
		,setting->ConcentrationSetup
		,setting->ConcentrationLevel
		,setting->Sensor
		,setting->Zero
		,setting->Kanji
		,setting->Paper
		,setting->Feed
		,setting->Pitch
		,setting->VDot
		,setting->HDot
		,setting->VOffset
		,setting->HOffset
		,setting->PitchOffset
		,setting->TearOffset
		,setting->CutterOffset
		,setting->HakuriOffset
		,setting->Comatibility
		,setting->Gap
		,setting->Buzzer
		);
	// ﾃﾞｰﾀ送信開始 STX+ESCA
	cmd[strlen(cmd)]=S_ESC;
	cmd[strlen(cmd)]='Z';
	cmd[strlen(cmd)]=S_ETX;

	return(SendData(cmd,(int)strlen(cmd) ) );

}
//---------------------------------------------------------------------------------------------------------------------
//	int	 DrawKanjiPT( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch)
//	タイプ：private
//	機能　：プチラパン(PT-200e)用・全半混合漢字文字列印刷（プリンタの設定をS-JISにしておくこと）
//	引数１：x座標(0.1mm)
//	引数２：y座標(0.1mm)
//	引数３：文字列
//	引数４：文字数
//	引数５：文字サイズ(1(16*16),2(24*24),3(22*22))
//	引数６：横倍率(1-12)
//	引数７：縦倍率(1-12)
//	引数７：文字間ピッチ(0-99)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::DrawKanjiPT( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch)
{
	if( m_Com == NULL ) return(-1);

	char cmd[50];

	SetPosition(x,y);

	// 縦横倍率
	if( yoko < 1 || yoko >12 ) yoko=1;
	if( tate < 1 || tate >12 ) tate=1;

	if( pitch < 0 || pitch > 99 ) pitch=2;

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP%02d",S_ESC,pitch);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));
	cmd[strlen(cmd)] = S_ESC;
	cmd[strlen(cmd)] = 'L';
	wsprintf( &cmd[strlen(cmd)],"%02d%02d",yoko,tate);

	SendData(cmd,(int)strlen(cmd) );


	// 文字サイズ
	if( size !=1 && size !=2 && size !=3 ) size=1;		// 16*16
	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	wsprintf( &cmd[strlen(cmd)],"K%dH",size);
	SendData(cmd,(int)strlen(cmd) );

	// 漢字コードを文字変換して送る
	int i;
	for( i=0; i< len; i++){
		// 次の文字は漢字か？
		if( IsDBCSLeadByteEx(CP_ACP, str[i])==TRUE){
			sprintf( cmd,"%02X%02X",(unsigned char)str[i],(unsigned char)str[i+1]);
			SendData(cmd,(int)strlen(cmd) );
			i++;
		}
		// 半角の場合はそのまま送る
		else{
			sprintf( cmd,"%02X",(unsigned short)str[i]);
			SendData(cmd,2 );
		}

	}

	return(0);
}

