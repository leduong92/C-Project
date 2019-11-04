//=======================================================================================================================================
//
//  	プリンターの制御基本クラス
//	
//		FileName : TPrinter.cpp
//		ＮＯＫ株式会社 技術本部 生産技術部
//		作成日：2000.05.15 coba 分散した過去のソースを合併して作り直し
//		修正日：
//
//===========================================================================
#include	<stdio.h>
#include	"Tprinter.h"



// ASCII文字をバーコードテーブルの番号へ変換するテーブル
static char BarcodeConvTbl[256]={
	//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//0
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//1
		38,41,38,38,40,43,38,38,38,38,39,42,38,36,37,38,	//2
		 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,38,38,38,38,38,	//3
		38,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,	//4
		25,26,27,28,29,30,31,32,33,34,35,38,38,38,38,38,	//5
		38,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,	//6
		25,26,27,28,29,30,31,32,33,34,35,38,38,38,38,38,	//7
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//8
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//9
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//A
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//B
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//C
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//D
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//E
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38};	//F



// バーコードのテーブル
// BarCode(Code-39) BASR(5)+SPACE(4) 幅の広い方が１狭い方が０
//順番は黒、白、黒の順番に並んでいる
static char BarcodeTbl[44][11]={
						{1,0,0,1,0,0,0,0,1,0,'1'},	// 00 ■■□■□□■□■□■■□
						{0,0,1,1,0,0,0,0,1,0,'2'},	// 01 ■□■■□□■□■□■■□
						{1,0,1,1,0,0,0,0,0,0,'3'},	// 02 ■■□■■□□■□■□■□
						{0,0,0,1,1,0,0,0,1,0,'4'},	// 03 ■□■□□■■□■□■■□
						{1,0,0,1,1,0,0,0,0,0,'5'},	// 04 ■■□■□□■■□■□■□
						{0,0,1,1,1,0,0,0,0,0,'6'},	// 05
						{0,0,0,1,0,0,1,0,1,0,'7'},	// 06
						{1,0,0,1,0,0,1,0,0,0,'8'},	// 07
						{0,0,1,1,0,0,1,0,0,0,'9'},	// 08
						{0,0,0,1,1,0,1,0,0,0,'0'},	// 09
						{1,0,0,0,0,1,0,0,1,0,'A'},	// 10
						{0,0,1,0,0,1,0,0,1,0,'B'},	// 11
						{1,0,1,0,0,1,0,0,0,0,'C'},	// 12
						{0,0,0,0,1,1,0,0,1,0,'D'},	// 13
						{1,0,0,0,1,1,0,0,0,0,'E'},	// 14
						{0,0,1,0,1,1,0,0,0,0,'F'},	// 15
						{0,0,0,0,0,1,1,0,1,0,'G'},	// 16
						{1,0,0,0,0,1,1,0,0,0,'H'},	// 17
						{0,0,1,0,0,1,1,0,0,0,'I'},	// 18
						{0,0,0,0,1,1,1,0,0,0,'J'},	// 19
						{1,0,0,0,0,0,0,1,1,0,'K'},	// 20
						{0,0,1,0,0,0,0,1,1,0,'L'},	// 21
						{1,0,1,0,0,0,0,1,0,0,'M'},	// 22
						{0,0,0,0,1,0,0,1,1,0,'N'},	// 23
						{1,0,0,0,1,0,0,1,0,0,'O'},	// 24
						{0,0,1,0,1,0,0,1,0,0,'P'},	// 25
						{0,0,0,0,0,0,1,1,1,0,'Q'},	// 26
						{1,0,0,0,0,0,1,1,0,0,'R'},	// 27
						{0,0,1,0,0,0,1,1,0,0,'S'},	// 28
						{0,0,0,0,1,0,1,1,0,0,'T'},	// 29
						{1,1,0,0,0,0,0,0,1,0,'U'},	// 30
						{0,1,1,0,0,0,0,0,1,0,'V'},	// 31
						{1,1,1,0,0,0,0,0,0,0,'W'},	// 32
						{0,1,0,0,1,0,0,0,1,0,'X'},	// 33
						{1,1,0,0,1,0,0,0,0,0,'Y'},	// 34
						{0,1,1,0,1,0,0,0,0,0,'Z'},	// 35
						{0,1,0,0,0,0,1,0,1,0,'_'},	// 36
						{1,1,0,0,0,0,1,0,0,0,'.'},	// 37
						{0,1,1,0,0,0,1,0,0,0,' '},	// 38
						{0,1,0,0,1,0,1,0,0,0,'*'},	// 39
						{0,1,0,1,0,1,0,0,0,0,'$'},	// 40
						{0,1,0,1,0,0,0,1,0,0,'!'},	// 41
						{0,1,0,0,0,1,0,1,0,0,'+'},	// 42
						{0,0,0,1,0,1,0,1,0,0,'%'}};	// 43

static char BarcodeConvTbl128A[256]={
	//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,	//0
		80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,	//1
		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,	//2
		16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,	//3
		32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,	//4
		48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,	//5
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//6
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//7
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//8
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//9
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//A
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//B
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//C
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//D
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//E
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38};	//F

static char BarcodeConvTbl128B[256]={
	//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,	//0
		80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,	//1
		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,	//2
		16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,	//3
		32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,	//4
		48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,	//5
		64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,	//6
		80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,	//7
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//8
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//9
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//A
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//B
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//C
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//D
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//E
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38};	//F

// バーコードのテーブル
static char BarcodeTbl128[106][6]={
						{2,1,2,2,2,2},	// 00 ■■□■□□■□■□■■□
						{2,2,2,1,2,2},	// 01 ■□■■□□■□■□■■□
						{2,2,2,2,2,1},
						{1,2,1,2,2,3},
						{1,2,1,3,2,2},
						{1,3,1,2,2,2},
						{1,2,2,2,1,3},
						{1,2,2,3,1,2},
						{1,3,2,2,1,2},
						{2,2,1,2,1,3},
						{2,2,1,3,1,2},
						{2,3,1,2,1,2},	// 01 ■□■■□□■□■□■■□
						{1,1,2,2,3,2},
						{1,2,2,1,3,2},
						{1,2,2,2,3,1},
						{1,1,3,2,2,2},
						{1,2,3,1,2,2},
						{1,2,3,2,2,1},
						{2,2,3,2,1,1},
						{2,2,1,1,3,2},
						{2,2,1,2,3,1},
						{2,1,3,2,1,2},	// 01 ■□■■□□■□■□■■□
						{2,2,3,1,1,2},
						{3,1,2,1,3,1},
						{3,1,1,2,2,2},
						{3,2,1,1,2,2},
						{3,2,1,2,2,1},
						{3,1,2,2,1,2},
						{3,2,2,1,1,2},
						{3,2,2,2,1,1},
						{2,1,2,1,2,3},
						{2,1,2,3,2,1},	// 01 ■□■■□□■□■□■■□
						{2,3,2,1,2,1},
						{1,1,1,3,2,3},
						{1,3,1,1,2,3},
						{1,3,1,3,2,1},
						{1,1,2,3,1,3},
						{1,3,2,1,1,3},
						{1,3,2,3,1,1},
						{2,1,1,3,1,3},
						{2,3,1,1,1,3},
						{2,3,1,3,1,1},	// 01 ■□■■□□■□■□■■□
						{1,1,2,1,3,3},
						{1,1,2,3,3,1},
						{1,3,2,1,3,1},
						{1,1,3,1,2,3},
						{1,1,3,3,2,1},
						{1,3,3,1,2,1},
						{3,1,3,1,2,1},
						{2,1,1,3,3,1},
						{2,3,1,1,3,1},
						{2,1,3,1,1,3},	// 01 ■□■■□□■□■□■■□
						{2,1,3,3,1,1},
						{2,1,3,1,3,1},
						{3,1,1,1,2,3},
						{3,1,1,3,2,1},
						{3,3,1,1,2,1},
						{3,1,2,1,1,3},
						{3,1,2,3,1,1},
						{3,3,2,1,1,1},
						{3,1,4,1,1,1},
						{2,2,1,4,1,1},	// 01 ■□■■□□■□■□■■□
						{4,3,1,1,1,1},
						{1,1,1,2,2,4},
						{1,1,1,4,2,2},
						{1,2,1,1,2,4},
						{1,2,1,4,2,1},
						{1,4,1,1,2,2},
						{1,4,1,2,2,1},
						{1,1,2,2,1,4},
						{1,1,2,4,1,2},
						{1,2,2,1,1,4},	// 01 ■□■■□□■□■□■■□
						{1,2,2,4,1,1},
						{1,4,2,1,1,2},
						{1,4,2,2,1,1},
						{2,4,1,2,1,1},
						{2,2,1,1,1,4},
						{4,1,3,1,1,1},
						{2,4,1,1,1,2},
						{1,3,4,1,1,1},
						{1,1,1,2,4,2},
						{1,2,1,1,4,2},	// 01 ■□■■□□■□■□■■□
						{1,2,1,2,4,1},
						{1,1,4,2,1,2},
						{1,2,4,1,1,2},
						{1,2,4,2,1,1},
						{4,1,1,2,1,2},
						{4,2,1,1,1,2},
						{4,2,1,2,1,1},
						{2,1,2,1,4,1},
						{2,1,4,1,2,1},
						{4,1,2,1,2,1},	// 01 ■□■■□□■□■□■■□
						{1,1,1,1,4,3},
						{1,1,1,3,4,1},
						{1,3,1,1,4,1},
						{1,1,4,1,1,3},
						{1,1,4,3,1,1},
						{4,1,1,1,1,3},
						{4,1,1,3,1,1},
						{1,1,3,1,4,1},
						{1,1,4,1,3,1},
						{3,1,1,1,4,1},	// 01 ■□■■□□■□■□■■□
						{4,1,1,1,3,1},
						{2,1,1,4,1,2},		// 103=STOP-A
						{2,1,1,2,1,4},
						{2,1,1,2,3,2}};



static char BarcodeTblStop[7]={2,3,3,1,1,1,2};

// 印刷する文字フォントのテーブル
static char *FontName[]={{"Default"},					// 0
						{"ＭＳ 明朝"},					// 1
						{"ＭＳ ゴシック"},				// 2
						{"Century Gothic"},				// 3
						{"Courier New"},				// 4
						{"Courier New Bold"},			// 5
						{"Courier New Bold Italic"},	// 6
						{"Courier New Italic"},			// 7
					//	{"ＨＧｺﾞｼｯｸE-PRO"},				// 8
						{"Arial"},				// 8
						{"Impact"}};					// 9



//================================================================================================
//	TPrinter::TPrinter(void)
//	デフォルトコンストラクター
//	説明：これを使う場合は、この後に明示的に InitPageSetting()を呼ぶこと!!
//	説明：使用するプリンタを指定してオブジェクト作成。用紙はプリンタのデフォルトを使う
//	引数　なし
//================================================================================================
TPrinter::TPrinter(void)
{
	flgIPrinter = 0;		// プリンターの初期化フラグ(未初期化)
}

//================================================================================================
// TPrinter::TPrinter(int *initStat)
//	コンストラクター(デフォルト用紙、デフォルトプリンタを使用)
//	説明：これを使う場合は、この後に明示的に InitPageSetting()を呼ぶこと!!
//	説明：使用するプリンタを指定してオブジェクト作成。用紙はプリンタのデフォルトを使う
//	引数　なし
//================================================================================================
TPrinter::TPrinter(int *initStat)
{
	int stat;
	char csDrv[256], csDev[256], csPort[256];

	flgIPrinter = 0;		// プリンターの初期化フラグ(未初期化)

	// 接続プリンタを取得
	stat = GetDefaultPrinter(csDrv,csDev,csPort);

	// 取得失敗
	if( stat ){
		*initStat = stat;
		return;
	}

	// デバイスコンテキストの作成
	hdcPrint = CreateDC( csDrv, csDev, csPort,NULL);
	//hdcPrint = CreateDC(lpPrtDriver,lpPrtDevice,lpPrtPort, NULL);
	if( hdcPrint == NULL ){
		*initStat = -2;
		return ;
	}

	// 用紙サイズの取得(ピクセル数)
	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	// 
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	// 


	// マッピングモードの取得
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		// これによりプリンタの座標系が画面の座標系と上下反転します
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );


	IPrinter();
	*initStat = 0;
}

//================================================================================================
//	TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat)
//	コンストラクター(デフォルト用紙使用ﾊﾞｰｼﾞｮﾝ)
//	説明：使用するプリンタを指定してオブジェクト作成。用紙はプリンタのデフォルトを使う
//	引数１：theDrv	ドライバ名
//	引数２：theDev	デバイス名
//	引数３：thePort	ポート名
//	引数４：initStat	初期化ステータス（出力）
//================================================================================================
TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat)
{
	flgIPrinter = 0;		// プリンターの初期化フラグ(未初期化)
	int stat;

	// デバイスコンテキストの作成
	hdcPrint = CreateDC( theDrv, theDev, thePort,NULL);
	//hdcPrint = CreateDC(lpPrtDriver,lpPrtDevice,lpPrtPort, NULL);
	if( hdcPrint == NULL ){
		*initStat = -2;
		return ;
	}

	// 用紙サイズの取得(ピクセル数)
	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	// 
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	// 


	// マッピングモードの取得
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		// これによりプリンタの座標系が画面の座標系と上下反転します
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );


	IPrinter();
	*initStat = 0;
}


//==========================================================================================================
//	コンストラクター(用紙指定ﾊﾞｰｼﾞｮﾝ)
//	説明：使用するプリンタと用紙サイズを指定してオブジェクト作成。
//	引数１：theDrv		ドライバ名
//	引数２：theDev		デバイス名
//	引数３：thePort		ポート名
//	引数４：theLength	用紙縦サイズ(0.1mm)
//	引数５：theLength	用紙横サイズ(0.1mm)
//	引数６：initStat	初期化ステータス（出力）
//==========================================================================================================
TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth, int *initStat)
{
	flgIPrinter = 0;		// プリンターの初期化フラグ(未初期化)

	// デバイスコンテキストを作成
	*initStat = InitPageSetting(theDrv, theDev, thePort, theLength,theWidth);

	return;
}



//================================================================================================
//	デストラクター
//================================================================================================
TPrinter::~TPrinter()
{
	// プリンターの初期化フラグ
	if( flgIPrinter != 0 ){
		EndDoc(hdcPrint);		// 印刷終了

		DeleteDC(hdcPrint);		// 印刷用コンテキスト削除
	}
}


//============================================================================
//	概要 : ﾌﾟﾘﾝﾀｰ関係の初期化(ﾊｰﾄﾞ､ｿﾌﾄ)を行う関数
//
//	Return: int ( 0:OK or 1:NG )
//
//============================================================================
int TPrinter::IPrinter( void )
{
	DocStart();
	flgIPrinter = 1;						// 印刷Ready(プリンタ指定済み)のフラグ

	return 0;
}

//===========================================================================================================
//	int	TPrinter::InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth)
//	説明：使用するプリンタと用紙サイズを指定してデバイスコンテキストを作成。
//	引数１：theDrv		ドライバ名
//	引数２：theDev		デバイス名
//	引数３：thePort		ポート名
//	引数４：theLength	用紙縦サイズ(0.1mm)
//	引数５：theLength	用紙横サイズ(0.1mm)
//	引数６：initStat	初期化ステータス（出力）
//===========================================================================================================
int	TPrinter::InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth)
{

	int		stat;				// 設定値読み出しステータス
	HANDLE	phPrinter;			// プリンタハンドル
	DWORD	dwDmSize;				// プリンターのインフォメーション情報データサイズ
	DEVMODE	*devNew;			// 初期値－＞変更する
	char *pszIn;
	DWORD	newdmVal;


	// プリンターのハンドル取得して、DEVMODE構造体に必要なサイズを取得
	if( OpenPrinter( theDev , &phPrinter , NULL ) == FALSE ){
		return(-1);
	}


	// (プリンターの設定情報"DEVMODE"に必要なデータサイズの取得)
	dwDmSize = DocumentProperties(	NULL ,			// handle to window that displays dialog box
									phPrinter,			// handle to printer object
									theDrv,				// pointer to device name (おそらくDriver nameのことでしょう)
								 	NULL ,				// pointer to modified device mode structure
									NULL ,				// pointer to original device mode structure
 									0 );				// mode flg ( 0:Get DEVMODE structure size )

 
	// DEVMODE構造体用にメモリアロケート
	pszIn = new char[dwDmSize+1];
	devNew = (DEVMODE *)pszIn;

	// DEVMODE構造体にデータ取得
	stat = DocumentProperties(	NULL,phPrinter,theDrv,devNew ,NULL ,	DM_OUT_BUFFER );	// mode flg (設定値取得)
	if( stat != IDOK ){
		ClosePrinter( phPrinter );
		delete( pszIn );
		return(-2);
	}

	// プリンタハンドルクローズ
	ClosePrinter( phPrinter );

	// 用紙サイズ関連の設定を変更
	devNew -> dmPaperSize = DMPAPER_USER;			// ユーザー指定サイズ
	devNew -> dmPaperLength = theLength;			// 用紙長さ
	devNew -> dmPaperWidth = theWidth;				// 用紙長さ

	// dmFieldsメンバのフラグを設定
	newdmVal = devNew -> dmFields;
	devNew -> dmFields = newdmVal|DM_PAPERSIZE|DM_PAPERLENGTH|DM_PAPERWIDTH;


	// デバイスコンテキストの作成
	hdcPrint = CreateDC( theDrv, theDev, thePort, devNew );
	if( hdcPrint == NULL ){
		delete( pszIn );
		return(-3);
	}

	// 用紙サイズの取得(ピクセル数)
	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	// 
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	// 
	// マッピングモードの取得
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		// これによりプリンタの座標系が画面の座標系と上下反転します
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );

	flgIPrinter = 1;						// 印刷Ready(プリンタ指定済み)のフラグ

	IPrinter();

	return(0);
}

//================================================================================================
// TPrinter::TPrinter(int *initStat)
//	コンストラクター(デフォルトプリンタを使用)
//	説明 ：使用するプリンタを指定してオブジェクト作成。用紙と向きも指定
//	引数1：ステータス出力用
//	引数2：頁サイズ（DEVMODE構造体のdmPaperSize参照）
//	引数3:印刷部数
//================================================================================================
TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat,short pageSize,short orient)
{

	flgIPrinter = 0;		// プリンターの初期化フラグ(未初期化)


	// デバイスコンテキストの作成
	*initStat = InitPageSettingEx(theDrv, theDev, thePort, pageSize, orient);
	return;
}


//===========================================================================================================
//	int	TPrinter::InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth)
//	説明：使用するプリンタと用紙サイズと方向を指定してデバイスコンテキストを作成。
//	引数１：theDrv		ドライバ名
//	引数２：theDev		デバイス名
//	引数３：thePort		ポート名
//	引数４：thePage		用紙縦サイズ(DEVMODE構造体)
//	引数５：theOrient	用紙方向
//===========================================================================================================
int	TPrinter::InitPageSettingEx(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short thePage ,short theOrient)
{

	int		stat;				// 設定値読み出しステータス
	HANDLE	phPrinter;			// プリンタハンドル
	DWORD	dwDmSize;				// プリンターのインフォメーション情報データサイズ
	DEVMODE	*devNew;			// 初期値－＞変更する
	char *pszIn;
	DWORD	newdmVal;


	// プリンターのハンドル取得して、DEVMODE構造体に必要なサイズを取得
	if( OpenPrinter( theDev , &phPrinter , NULL ) == FALSE ){
		return(-1);
	}


	// (プリンターの設定情報"DEVMODE"に必要なデータサイズの取得)
	dwDmSize = DocumentProperties(	NULL ,			// handle to window that displays dialog box
									phPrinter,			// handle to printer object
									theDrv,				// pointer to device name (おそらくDriver nameのことでしょう)
								 	NULL ,				// pointer to modified device mode structure
									NULL ,				// pointer to original device mode structure
 									0 );				// mode flg ( 0:Get DEVMODE structure size )

 
	// DEVMODE構造体用にメモリアロケート
	pszIn = new char[dwDmSize+1];
	devNew = (DEVMODE *)pszIn;

	// DEVMODE構造体にデータ取得
	stat = DocumentProperties(	NULL,phPrinter,theDrv,devNew ,NULL ,	DM_OUT_BUFFER );	// mode flg (設定値取得)
	if( stat != IDOK ){
		ClosePrinter( phPrinter );
		delete( pszIn );
		return(-2);
	}

	// プリンタハンドルクローズ
	ClosePrinter( phPrinter );

	// 用紙サイズ関連の設定を変更
	devNew -> dmPaperSize = thePage;			// 用紙サイズ
	devNew -> dmCopies = 1;			// 用紙サイズ
	devNew -> dmOrientation = theOrient;			// 用紙方向
	devNew ->dmColor=DMCOLOR_COLOR;

	// dmFieldsメンバのフラグを設定
	newdmVal = devNew -> dmFields;
	devNew -> dmFields = newdmVal|DM_PAPERSIZE|DM_COPIES|DM_ORIENTATION|DM_COLOR;


	// デバイスコンテキストの作成
	hdcPrint = CreateDC( theDrv, theDev, thePort, devNew );
	if( hdcPrint == NULL ){
		delete( pszIn );
		return(-3);
	}

	// 用紙サイズの取得(ピクセル数)
	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	// 
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	// 
	// マッピングモードの取得
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		// これによりプリンタの座標系が画面の座標系と上下反転します
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );

	flgIPrinter = 1;						// 印刷Ready(プリンタ指定済み)のフラグ

	IPrinter();

	return(0);
}



//============================================================================
//	概要 : 印刷の実行(オーバーライドされる)
//	
//	Return:

//	
//============================================================================
int TPrinter::Print(void)
{
	char	prMes[] = "印刷フォームPrint();が設定(オーバーライド)されていません！！";
	// プリンタの選択が行われているかのチェック
	if( flgIPrinter == 0 ){
		return 1;	// エラー
	}
	return 0;
}

//============================================================================
//			概  要 : 印刷ページの開始
//	Input:	
//	Return:
//============================================================================
int	TPrinter::PageStart( void )
{
	int stat;

	if( flgIPrinter == 0 ) return 1;		// エラー

	stat = StartPage( hdcPrint );


	if( stat != 0 ){
		return GetLastError();
	}
	return 0;

}

//============================================================================
//			概  要 : 印刷ページの終了
//	Input:
//	Return:
//============================================================================
int	TPrinter::PageEnd( void )
{
	int stat;

	if( flgIPrinter == 0 ) return 1;		// エラー
	stat = EndPage( hdcPrint );

	if( stat != 0 ){
		return GetLastError();
	}
	return 0;
}


//============================================================================
//	int TPrinter::DocStart( void )
//	public
//	外部からプリンターにStartDoc()をかける
//
//============================================================================
int TPrinter::DocStart( void )
{
	DOCINFO docinfo;				// 印刷用デバイスコンテキスト

	// プリンタ設定
	docinfo.cbSize = sizeof(docinfo);		// 
	docinfo.lpszDocName= "NOK Printer";	// 印刷中にプリンタに出るメッセージ
	docinfo.lpszOutput = (LPSTR)NULL;		// 
	docinfo.lpszDatatype = NULL;			// 
	docinfo.fwType = 0;						// 
	StartDoc(hdcPrint,&docinfo);			// 印刷準備


	return 0;
}

//============================================================================
//	int	TPrinter::DocEnd( void )
//	public
//	外部からプリンターにEndDoc()をかける
//
//============================================================================
int	TPrinter::DocEnd( void )
{
	int stat;

	if( flgIPrinter == 0 ) return 1;		// エラー
		stat = EndDoc( hdcPrint );

	if( stat != 0 ){
		return GetLastError();
	}
	return 0;
}

//============================================================================
//		概要 : 文字印刷
//		Input:	int x;			Ｘ座標
//				int y;			Ｙ座標
//				int nFont;		フォントの番号デフォルトは０
//				int nBold:		字の太さ（CreateFontの定数）
//				int nPoint1;	フォントのサイズ(X)
//				int nPoint2;	フォントのサイズ(Y)
//				LPSTR Message;	文字列
//		Return:
//============================================================================
void TPrinter::PrMessage( int x, int y, int nFont, int nBold,int nPoint1, int nPoint2, LPSTR Message )
{
	HFONT	holdf,hnewf;
	HDC		hdc;
	int		sx,sy;

	TEXTMETRIC	textmetric;

	hdc = hdcPrint;


	sx = ConvertMmToPixcel( x , 0 );	// 印字Ｘ座標
	sy = ConvertMmToPixcel( y , 1 );	// 印字Ｙ座標

	GetTextMetrics(hdc,&textmetric);

	if( nPoint2 == 0 ) nPoint2 = nPoint1;

	nPoint1 = ConvertMmToPixcel( nPoint1 , 0 );
	nPoint2 = ConvertMmToPixcel( nPoint2 , 1 );

	if( nFont != 0 && nFont <= 8 ){		// フォントの個数分
		hnewf = CreateFont(	nPoint2,nPoint1,				// フォントサイズ
							0,					// Escapement
							0,					// Orientation
							nBold,				// Weight(フォントの太さ)
							0,					// Ital(イタリック体)
							0,					// Underline(下線)
							0,					// StrikeThru(打ち消し線)
							SHIFTJIS_CHARSET,	// Charset(文字セット)
							OUT_DEFAULT_PRECIS,	// Precision(出力精度)
							CLIP_DEFAULT_PRECIS,// ClipPrecision(クリッピング精度)
							DEFAULT_QUALITY,	// Quality(フォントの出力品質)
							DEFAULT_PITCH | FF_DONTCARE,	// Pitch(フォントのピッチとファミリ)
//							DEFAULT_PITCH | FF_ROMAN,	// Pitch(フォントのピッチとファミリ)
							FontName[nFont] );	// FontName
		holdf = SelectObject( hdc , hnewf );	// フォントの切り替え

		TextOut(hdc,sx,sy,Message,strlen(Message));

		hnewf = SelectObject( hdc , holdf );
		DeleteObject( hnewf );

	}else{
		TextOut(hdc,sx,sy,Message,strlen(Message));
	}

}
// 互換性の為
void TPrinter::PrMessage( int x, int y, int nFont,int nPoint1, int nPoint2, LPSTR Message )
{
	PrMessage( x,  y, nFont, FW_SEMIBOLD, nPoint1, nPoint2, Message );
}
//============================================================================
//		概要 : 文字印刷(中央よせ)
//	x,endX間で文字を中央よせで印字
//		Input:	int x;			Ｘ座標
//				int y;			Ｙ座標
//				int endX;		X座標
//				int nFont;		フォントの番号デフォルトは０
//				int nBold:		字の太さ（CreateFontの定数）
//				int nPoint1;	フォントのサイズ(X)
//				int nPoint2;	フォントのサイズ(Y)
//				LPSTR Message;	文字列
//		Return:
//============================================================================
void TPrinter::CenterText( int x, int y, int endX,int nFont, int nBold,int nPoint1, int nPoint2, LPSTR Message )
{
	HFONT	holdf,hnewf;
	HDC		hdc;
	int		sx,sy,ex;
	RECT rect;

	TEXTMETRIC	textmetric;

	hdc = hdcPrint;


	sx = ConvertMmToPixcel( x , 0 );	// 印字Ｘ座標
	ex = ConvertMmToPixcel( endX , 0 );	// 印字Ｘ座標
	sy = ConvertMmToPixcel( y , 1 );	// 印字Ｙ座標

	GetTextMetrics(hdc,&textmetric);

	if( nPoint2 == 0 ) nPoint2 = nPoint1;

	nPoint1 = ConvertMmToPixcel( nPoint1 , 0 );
	nPoint2 = ConvertMmToPixcel( nPoint2 , 1 );

	SetRect(&rect	,x
					,sy
					,ex
					,sy+nPoint2);

	if( nFont != 0 && nFont <= 8 ){		// フォントの個数分
		hnewf = CreateFont(	nPoint2,nPoint1,				// フォントサイズ
							0,					// Escapement
							0,					// Orientation
							nBold,				// Weight(フォントの太さ)
							0,					// Ital(イタリック体)
							0,					// Underline(下線)
							0,					// StrikeThru(打ち消し線)
							SHIFTJIS_CHARSET,	// Charset(文字セット)
							OUT_DEFAULT_PRECIS,	// Precision(出力精度)
							CLIP_DEFAULT_PRECIS,// ClipPrecision(クリッピング精度)
							DEFAULT_QUALITY,	// Quality(フォントの出力品質)
							DEFAULT_PITCH | FF_DONTCARE,	// Pitch(フォントのピッチとファミリ)
//							DEFAULT_PITCH | FF_ROMAN,	// Pitch(フォントのピッチとファミリ)
							FontName[nFont] );	// FontName
		holdf = SelectObject( hdc , hnewf );	// フォントの切り替え

//		TextOut(hdc,sx,sy,Message,strlen(Message));
		DrawText(hdc, Message,-1,&rect,DT_CENTER|DT_SINGLELINE);

		hnewf = SelectObject( hdc , holdf );
		DeleteObject( hnewf );

	}else{
		DrawText(hdc, Message,-1,&rect,DT_CENTER|DT_SINGLELINE);
	}

}

//=======================================================================================================================================
// void TPrinter::PrMessage( int x, int y, int nFont, int nBold,int nPoint1, int nPoint2, COLORREF colFont,COLORREF colBack,LPSTR Message )
//	public
//	前背景色を指定した文字印刷
//	引数:		int x;			Ｘ座標
//				int y;			Ｙ座標
//				int nFont;		フォントの番号デフォルトは０
//				int nBold:		字の太さ（CreateFontの定数）
//				int nPoint1;	フォントのサイズ(X)
//				int nPoint2;	フォントのサイズ(Y)
//				COLORREF colFont
//				COLORREF colBack
//				LPSTR Message;	文字列
//=======================================================================================================================================
void TPrinter::PrMessage( int x, int y, int nFont, int nBold,int nPoint1, int nPoint2, COLORREF colFont,COLORREF colBack,LPSTR Message )
{
	COLORREF oldFontCol,oldBackCol;

	// フォント色
	oldFontCol = SetTextColor( hdcPrint,colFont);

	// 背景色
	oldBackCol = SetBkColor(hdcPrint,colBack );

	PrMessage( x, y, nFont, nBold,nPoint1, nPoint2, Message );

	// フォント色
	SetTextColor( hdcPrint,oldFontCol);

	// 背景色
	SetBkColor(hdcPrint,oldBackCol );
}

//============================================================================
//			概  要 : バーコード印刷
//	Input:	int x;			印刷するＸ座標(0.1mm)
//			int y;			印刷するＹ座標(0.1mm)
//			int nWidth;		印刷するバーコードの幅(一番細い線をドット単位で指示する)
//			int nHeight;	印刷するバーコードの高さ(0.1mm) 文字の部分は含まない
//			int strON		文字列印刷ありなし（０以外＝印刷する）
//			LPSTR Message;	文字列
//	Return:
//============================================================================
void TPrinter::PrBCode( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message )
{

	HDC	hdc;
	char	mojiBarcode[MAX_BARCODE+1];		// バーコードの下に表示する文字列
	char	dspBarcode[MAX_BARCODE];		// BarcodeTbl[no]に変換した文字列
	int i;

	int		mojisuu;					// 文字数
	int		sx,ox1,ox2;					// 印刷するＸ座標(開始、相対位置)
	int		sy,ey;						// 印刷するＹ座標(開始、終了)
	int		l;							// バーコードのラインNo.
	double		bigBar,smallBar;			// バーコードの幅

	// 塗りつぶし
	RECT rect;

	// 印刷データが存在するか？
	hdc = hdcPrint;

	// 文字列チェック
	memset( dspBarcode , 0 , sizeof( dspBarcode ));
	mojisuu = strlen(Message);
	if( mojisuu > MAX_BARCODE-2 )mojisuu = MAX_BARCODE-2;	// 両端*が付属するため


	// 印刷する文字の両端に '*' を付加する
	memset( mojiBarcode , 0 , sizeof( mojiBarcode ));
	mojiBarcode[0]='*';
	memcpy( &mojiBarcode[1] , Message , mojisuu );
	mojiBarcode[mojisuu+1] = '*';

	// バーコード表の並びに変換する
	for( i = 0 ; i < mojisuu+2 ; i++ ){
		dspBarcode[i] = BarcodeConvTbl[mojiBarcode[i]];
	}

	// 枠を書く(DEBUG用)
	//	Rectangle( hdc , x-5 , y-5 , x+nWidth*((mojisuu)*13-1)+5 , y+nHeight+5);

//	sx = x;
	sx = ConvertMmToPixcel( x , 0 );	// 開始Ｘ座標
	sy = ConvertMmToPixcel( y , 1 );	// 開始Ｙ座標
	ey = ConvertMmToPixcel((y+nHeight),1);	// 終了Ｙ座標



	smallBar = ConvertMmToPixcel(nWidth,0);
	bigBar = 2*smallBar;
//	bigBar = 2.5*smallBar;




	// バーを黒く見せるために２ドット以上のバーは別処理
	if( smallBar == 1 ){
	
		ox1 = 0;						// 線を書き始める位置
		for( i = 0 ; i < mojisuu+2 ; i++ ){	// 文字
			for( l = 0 ; l < 10 ; l++ ){		// ライン
				// 線を書き終わる位置を算出する
				ox2 = ox1 + (int)(BarcodeTbl[dspBarcode[i]][l] ? bigBar : smallBar);
				if( l % 2 == 0 ){			// 黒いバー
					SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
					FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
				}
				ox1 = ox2;		// 次の開始位置
			}
		}
	}else{
	
		ox1 = 0;						// 線を書き始める位置
		for( i = 0 ; i < mojisuu+2 ; i++ ){	// 文字
			for( l = 0 ; l < 10 ; l++ ){		// ライン
				// 線を書き終わる位置を算出する
				ox2 = ox1 + (int)(BarcodeTbl[dspBarcode[i]][l] ? bigBar : smallBar);
				if( l % 2 == 0 ){			// 黒いバー
					SetRect( &rect,sx+ox1,sy,sx+ox2-1,ey );
					FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
				}
				ox1 = ox2;		// 次の開始位置
			}
		}
	}



	// バーコードの下に文字を表示する
	if(strON)	PrBCodeString( x , y+nHeight+1 , sx+ox2 , mojiBarcode );


}



// 互換性の為
void TPrinter::PrBCode( int x, int y, int nWidth, int nHeight,LPSTR Message )
{
	PrBCode( x, y, nWidth, nHeight,1 , Message );
}

//================================================================================================
//	void TPrinter::PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code)
//			概  要 : code128バーコード印刷(コード固定)
//	Input:	int x;			印刷するＸ座標(0.1mm)
//			int y;			印刷するＹ座標(0.1mm)
//			int nWidth;		印刷するバーコードの幅(一番細い線をドット単位で指示する)
//			int nHeight;	印刷するバーコードの高さ(0.1mm) 文字の部分は含まない
//			int strON		文字列印刷ありなし（０以外＝印刷する）
//			LPSTR Message;	文字列
//			int code		印刷に使うコード 0=A,1=B,2=C
//================================================================================================
void TPrinter::PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code)
{
	HDC	hdc;
	char mojiBarcode[MAX_BARCODE+1];		// バーコードの下に表示する文字列
	char dspBarcode[MAX_BARCODE];		// BarcodeTbl[no]に変換した文字列
	int i,j,mojisuu;
	int	sx,ox1,ox2;					// 印刷するＸ座標(開始、相対位置)
	int	sy,ey;						// 印刷するＹ座標(開始、終了)
	int	chkD;						// code128のﾁｪｯｸﾃﾞｼﾞｯﾄ
	int barWidth;

	// 引数チェック
	if(code < 0 || code > 2 ) code=1;	// 無難にcodeBにする

	// 塗りつぶし
	RECT rect;

	// 印刷データが存在するか？
	hdc = hdcPrint;

	// 文字列チェック
	memset( dspBarcode , 0 , sizeof( dspBarcode ));
	memset( mojiBarcode , 0 , sizeof( mojiBarcode ));
	mojisuu = strlen(Message);
	if( mojisuu > MAX_BARCODE)mojisuu = MAX_BARCODE;
	memcpy( mojiBarcode, Message , mojisuu );

	// バーコード表の並びに変換する
	for( i = 0 ; i < mojisuu ; i++ ){
		switch(code){
			case 2:		// code A
				if(mojiBarcode[i] > 0 )
					dspBarcode[i] = mojiBarcode[i];
				break;

			case 0:
				dspBarcode[i] = BarcodeConvTbl128A[mojiBarcode[i]];
				break;
			default:
				dspBarcode[i] = BarcodeConvTbl128B[mojiBarcode[i]];
				break;
		}
	}

	sx = ConvertMmToPixcel( x , 0 );	// 開始Ｘ座標
	sy = ConvertMmToPixcel( y , 1 );	// 開始Ｙ座標
	ey = ConvertMmToPixcel((y+nHeight),1);	// 終了Ｙ座標
	barWidth =  ConvertMmToPixcel( nWidth , 0 );	// バーの幅

	ox1 = 0;						// 線を書き始める位置
	
	// スタートコード
	for( i=0; i<6; i++ ){ 
		if( i % 2 == 0 ){	// 黒いバー
			ox2=ox1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
		}
		else{				// 白いバー
			ox2=ox1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
		}
		ox1=ox2;
	}
	// データ
	for( i=0; i<mojisuu; i++ ){

		for( j=0; j<6; j++ ){ 
			if( j % 2 == 0 ){	// 黒いバー
				ox2=ox1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
				SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
				FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
			}
			else{				// 白いバー
				ox2=ox1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
			}
			ox1=ox2;
		}
	}

	// ﾁｪｯｸﾃﾞｼﾞｯﾄ
	chkD=0;
	for( i=0; i<mojisuu; i++ ){
		chkD+=( dspBarcode[i]*(i+1) );
	}
	chkD=(chkD+STARTCODE_128+code)%103;
	for( j=0; j<6; j++ ){ 
		if( j % 2 == 0 ){	// 黒いバー
			ox2=ox1+(BarcodeTbl128[chkD][j]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
		}
		else{				// 白いバー
			ox2=ox1+(BarcodeTbl128[chkD][j]*barWidth);
		}
		ox1=ox2;
	}
	
	// stopコード
	for( j=0; j<7; j++ ){ 
		if( j % 2 == 0 ){	// 黒いバー
			ox2=ox1+(BarcodeTblStop[j]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
		}
		else{				// 白いバー
			ox2=ox1+(BarcodeTblStop[j]*barWidth);
		}
		ox1=ox2;
	}

	// バーコードの下に文字を表示する
	if(strON)	
		PrBCodeString( x , y+nHeight+1 , sx+ox2 , mojiBarcode );


}

//============================================================================
//			概  要 : バーコードの文字印刷
//	Input:	int x;			印刷するＸ座標(0.1mm)
//			int y;			印刷するＹ座標(0.1mm)
//			int endX;		印刷終了するX座標(Pixcel)
//			LPSTR Message;	文字列
//	Return:
//============================================================================
void	TPrinter::PrBCodeString( int x, int y, int endX, LPSTR Message )
{
	int i;
	int mojisuu;
	char	moji[4];
	int	 endXmm,txtSpace;

	memset( moji , 0 , sizeof( moji ));
	mojisuu = strlen( Message );

	// 文字間の計算
	// 終了座標をmmに変換
	endXmm = endX * 10 * PrinterSizeX / PrinterResX;
	txtSpace = (endXmm-x-12) / (mojisuu-1);	// 12を引くのはラスト１文字分の幅

	for( i = 0 ; i < mojisuu ; i++ ){
		moji[0] = Message[i];
		PrMessage( x+i*txtSpace , y , 1 ,FW_NORMAL, 12 , 30 , moji );
	}
}

//==================================================================================================
//	void TPrinter::DrawCircle( int x,int y,int r, BOOL paint)
//	public
//	真円の描画
//	引数:	int sx;			中心Ｘ座標(0.1mm)
//			int sy;			中心Ｙ座標(0.1mm)
//			int r;			半径(0.1mm)
//			BOOL paint;		塗りつぶしあり=TRUE
//==================================================================================================
void TPrinter::DrawCircle( int x,int y,int r, BOOL paint)
{
	int px,py,rx,ry;

	px = ConvertMmToPixcel( x , 0 );	// 開始Ｘ座標
	rx = ConvertMmToPixcel( r , 0 );	// 開始Ｘ座標

	py = ConvertMmToPixcel( y , 1 );	// 開始Ｘ座標
	ry = ConvertMmToPixcel( r , 1 );	// 開始Ｘ座標

	HPEN	hOldpen,hPen;				// Handle of pen
	HBRUSH hbrush,hold;

	hPen   = CreatePen( PS_SOLID ,3, RGB(0,0,0));
	hOldpen   = (HPEN)SelectObject(hdcPrint,hPen);
	hbrush = CreateSolidBrush(RGB(0,0,0));
	hold   = (HPEN)SelectObject(hdcPrint,hbrush);

	// 塗りつぶし
	if( paint ){
		Ellipse( hdcPrint , px-rx ,py-ry,px+rx,py+ry);
	}
	// からの○
	else{
		Arc( hdcPrint , px-rx ,py-ry,px+rx,py+ry,px,py-ry,px,py-ry);
	}
	hPen   = (HPEN)SelectObject(hdcPrint,hOldpen);
	hbrush   = (HPEN)SelectObject(hdcPrint,hold);
	DeleteObject( hPen );
	DeleteObject( hbrush );
}
//============================================================================
//			概  要 : 円または楕円を描く
//	Input:	int x;			印刷するＸ座標(0.1mm)
//			int y;			印刷するＹ座標(0.1mm)
//			int rx;			x軸半径
//			int ry;			y軸半径
//			int LineType 線の種類
//					0:実線
//					1:破線
//					2:点線
//			int LineType 線のはば（実践指定時のみ有効）
//	Return:
//============================================================================
void TPrinter::DrawCircle( int x, int y ,int rx ,int ry , int LineType ,int width)
{
	HDC	hdc;
	HPEN	hPen,hOldPen;				// Handle of pen
	int theStyle,theWidth;
	int sx,sy;
	int ex,ey;

	// 印刷データが存在するか？
	hdc = hdcPrint;

	sx = ConvertMmToPixcel( x-rx , 0 );	// 印字Ｘ座標
	sy = ConvertMmToPixcel( y-ry , 1 );	// 印字Ｙ座標
	ex = ConvertMmToPixcel( x+rx , 0 );	// 印字Ｘ座標
	ey = ConvertMmToPixcel( y+ry , 1 );	// 印字Ｙ座標	

	switch( LineType ){
		case 1:						// 破線
			theStyle = PS_DASH;
			theWidth = 1;
			break;
		case 2:						// 点線
			theStyle = PS_DOT;
			theWidth = 1;
			break;

		default:
			theStyle = PS_SOLID;
			theWidth = width;
			break;
	}

	hPen   = CreatePen( theStyle ,width, RGB(0,0,0));
	hOldPen   = (HPEN)SelectObject(hdc,hPen);

	// 楕円を描く
	Arc( hdc , sx , sy , ex , ey, sx+(ex-sx)/2 , sy+(ey-sy)/2 , sx+(ex-sx)/2, sy+(ey-sy)/2);

	SelectObject(hdc,hOldPen);

	DeleteObject( hPen );

}

//============================================================================
//	線を引く
//	Input:	int sx;			印刷するＸ座標(0.1mm)
//			int sy;			印刷するＹ座標(0.1mm)
//			int ex;
//			int ey;
//			int LineType 線の種類
//					0:実線
//					1:破線
//					2:点線
//			int LineType 線のはば（実践指定時のみ有効）
//	Return:
//============================================================================
void TPrinter::DrawLine( int sx,int sy,int ex, int ey,int LineType ,int LineWidth)
{
	HDC	hdc;
	HPEN	hOldpen,hPen;				// Handle of pen
	int theStyle,theWidth;
	int x1,x2,y1,y2;

	switch( LineType ){
		case 1:						// 破線
			theStyle = PS_DASH;
			theWidth = 1;
			break;
		case 2:						// 点線
			theStyle = PS_DOT;
			theWidth = 1;
			break;

		default:
			theStyle = PS_SOLID;
			theWidth = LineWidth;
			break;
	}


	// 印刷データが存在するか？
	hdc = hdcPrint;
	x1 = ConvertMmToPixcel( sx , 0 );	// 開始Ｘ座標
	x2 = ConvertMmToPixcel( ex , 0 );	// 開始Ｘ座標
	y1 = ConvertMmToPixcel( sy , 1 );	// 開始Ｙ座標
	y2 = ConvertMmToPixcel(ey,1);		// 終了Ｙ座標

	hPen   = CreatePen( theStyle ,theWidth, RGB(0,0,0));
	hOldpen   = (HPEN)SelectObject(hdc,hPen);

	// 線を引く
	MoveToEx( hdc , x1 , y1 , NULL );
	LineTo(   hdc , x2 , y2 );


	// GDIもどし
	hPen   = (HPEN)SelectObject(hdc,hOldpen);
	DeleteObject( hPen );
}

//==================================================================================================
//	void TPrinter::DrawRect( int sx, int sy, int ex, int ey,COLORREF theColor)
//	public
//	指定範囲内の塗りつぶし
//	Input:	int sx;			左上Ｘ座標(0.1mm)
//			int sy;			左上Ｙ座標(0.1mm)
//			int ex;			右下X
//			int ey	;		右下Y
//			COLORREF theColor	塗りつぶし色
//==================================================================================================
void TPrinter::DrawRect( int sx, int sy, int ex, int ey,COLORREF theColor)
{

	RECT rect;	// 塗りつぶし領域

	HBRUSH hBrush,hOldBrush;

	SetRect( &rect,ConvertMmToPixcel(sx,0),ConvertMmToPixcel(sy,1),
				   ConvertMmToPixcel(ex,0),ConvertMmToPixcel(ey,1 ));

	// ブラシ作成
	hBrush = CreateSolidBrush( theColor);

	hOldBrush = SelectObject( hdcPrint , hBrush );
	FillRect( hdcPrint,&rect,hBrush );

	// GDI戻し
	hBrush = SelectObject( hdcPrint , hOldBrush );
	DeleteObject( hBrush );
}

//============================================================================
//	概要 :	指定の座標(ｍｍ)をプリンターの座標(ピクセル)に変換する
//			ＶＰ－６００を使用する際に使用する
//	
//	Input : int mm;			指定の座標	０．１ｍｍを１とする
//			int xy;			座標系		０：Ｘ座標
//										１：Ｙ座標
//	Return: 計算結果
//	
//============================================================================
int	TPrinter::ConvertMmToPixcel( int mm , int xy )
{
	double val;

	if( xy == 0 ){			// Ｘ座標の変換
		val =  (double)mm * PrinterResX/(PrinterSizeX*10);
//val=mm;
	}else{					// Ｙ座標の変換
		val =  (double)mm * PrinterResY/(PrinterSizeY*10);

	}


	return (int)val;
}




// デフォルトプリンタに関する情報の取得
int TPrinter::GetDefaultPrinter(char *theDrv, char *theDev, char *thePort)
{
	static PAGESETUPDLG psd;		// ペ－ジ設定ダイアログ初期化構造体
	DEVNAMES *dvn; 					// プリンタ情報構造体
	int size;
	char lpDev[256];
	char lpDrv[256];
	char lpPort[256];

	// PAGESETUP構造体の初期化
	memset( &psd,0, sizeof(psd));
	psd.Flags = PSD_RETURNDEFAULT;		// ダイアログをオープンせずにデフォルトプリンタを取得
 	psd.hDevNames=NULL;
	psd.hDevMode=NULL;
	psd.lStructSize=sizeof(PAGESETUPDLG);
	psd.hwndOwner=NULL;

	// ダイアログを開いてデバイス・ポート・ドライバ名取得
	if(!PageSetupDlg( &psd )) return(-1);

	// DEVNAME構造体のメモリロック＆割り当て
	dvn=(DEVNAMES*)GlobalLock(psd.hDevNames);
	size=dvn->wDeviceOffset;

	// 各種データを取得する
	memset(lpDev,0,sizeof(lpDev) );
	strncpy( lpDev,(char*)dvn+dvn->wDeviceOffset,sizeof(lpDev) );

	memset(lpDrv,0,sizeof(lpDrv) );
	strncpy( lpDrv,(char*)dvn+dvn->wDriverOffset, sizeof(lpDrv) );

	memset(lpPort,0,sizeof(lpPort) );
	strncpy( lpPort,(char*)dvn+dvn->wOutputOffset,sizeof(lpPort) );

	// メモリ解放
	GlobalFree(psd.hDevMode);
	GlobalUnlock(psd.hDevNames);
	GlobalFree(psd.hDevNames);
	GlobalFree(psd.hPageSetupTemplate);
	GlobalUnlock(dvn);
	GlobalFree(dvn);

	// 出力
	strncpy(theDrv,lpDrv,256);
	strncpy(theDev,lpDev,256);
	strncpy(thePort,lpPort,256);
	return(0);

}





//===== End of Text =====
