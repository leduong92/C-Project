//================================================================================================
//	NOK Fanatic Class Libraly
//	FileName	:TXls.cpp
//	Class		:TXls 
// 				:MS-Excel97(bif80)形式ファイル/書き込みクラス
//	作成日		:2000.01.08	Coded by coba
//				2000.07.4 TAKE シート追加ならびにシート名セット関数を追加
//	修正日		:2007.03.08 matsuda CopySheetを追加 エクセルクリエータVer3の関数!!
//================================================================================================
#include "TXls.h"
#include <stdio.h>

//========================================================================================================
//	TXls::TXls(void)
//	引数　：なし
//	返値　：
//　機能　：コンストラクタ。特に何もせず
//========================================================================================================
TXls::TXls(void)
{
	// 各パラメータのイニシャライズ
	m_Open=FALSE;
	m_Err = 0;

}

//========================================================================================================
//	TXls::~TXls()
//　機能　：ファイルハンドルの解放。
//	引数　：なし
//	返値　：
//========================================================================================================
TXls::~TXls()
{
	Close(FALSE);
}


//========================================================================================================
//	BOOL OpenXls(char* filename, BOOL overwrite);
//  public
//　機能　：書き込みようにエクセルファイルを開く
//	引数１：オープンするエクセルブック名(NULLで終わる拡張子付き。)
//	引数２：ファイルオープンモード 
//			TRUE=filenameが存在する場合は一度クリアしてから開き、無いは新規に開く 
//			FALSE=filenameが存在する場合はそのまま開き、無い場合は開かない
//	引数３：作成するシート数（省略値=3）
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::OpenXls(char* filename, BOOL overwrite, long sheetMax)
{

	m_Err = 0;

	// すでにオープンしてある
	if( m_Open ){
		m_Err = -1;		return(FALSE);
	}

	// ファイル名のチェック
	if( filename==NULL ){
		m_Err = -2;		return(FALSE);
	}

	// ファイル名のチェックその２
	if( !strlen(filename)){
		m_Err = -2;		return(FALSE);
	}

	// 新規作成モード
	if( overwrite ){
		if( !XlsCreateBook(filename, ver97, sheetMax, &m_XlsID)){
			SetErrCode();return(FALSE);
		}
	}

	// 既存ファイルオープンモード
	else{
		if(!XlsOpenBook(filename,NULL, &m_XlsID)){
			SetErrCode();return(FALSE);
		}
	}


	// 成功
	m_Open=TRUE;
	return(TRUE);
}

//========================================================================================================
//	BOOL AddSheet(long Page);
//  public
//　機能　：ｵｰﾌﾟﾝ中のエクセルファイルにシートを追加する
//	引数１：エクセルシート数
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::AddSheet(long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsAddSheet(Page);
	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL DelSheet(long SheetNo,long Page);
//  public
//　機能　：ｵｰﾌﾟﾝ中のエクセルファイルのシートを削除する
//	引数１：エクセルシートNo.
//	引数２：エクセルシート数
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::DelSheet(long SheetNo,long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsDelSheet(SheetNo,Page);

//char aaa[100];
//memset(aaa,0,sizeof(aaa));
//sprintf(aaa,"ret=%ld  s=%ld   p=%ld",m_Err,SheetNo,Page);
//MessageBox(NULL,aaa,"",MB_OK);

	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL SetSheetName(char* Sheetname);
//  public
//　機能　：エクセルファイルのシートに名前を付ける
//	引数１：エクセルシート名(NULLで終わる拡張子付き。)
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::SetSheetName(char* SheetName)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsPutSheetName(SheetName);
	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL GetSheetName(long SheetNo,char* SheetName);
//  public
//　機能　：エクセルファイルのシートの名前を取得する
//	引数１：エクセルシートNo.
//	引数２：エクセルシート名(NULLで終わる拡張子付き。)
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::GetSheetName(long SheetNo,char* SheetName,int size)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsGetSheetName(SheetNo,SheetName,size);

	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL SetSheetNo(long Page);
//  public
//　機能　：エクセルファイルブック内シートの選択
//	引数１：シートのNo.-1 (Sheet４だったら３)
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::SetSheetNo(long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsSheetNo(Page);
	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}
//========================================================================================================
//	BOOL SetActiveSheet(long Page);
//  public
//　機能　：アクティブなシートの選択(データをセットするためのシート選択ではなく、エクセル開いた時のアクティブなシート）
//	引数１：シートのNo.-1 (Sheet４だったら３)
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::SetActiveSheet(long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsSetActiveSheet(Page);
	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}
//========================================================================================================
//	BOOL TXls::OpenXlsByTemplate(char* filename, char* templatefile,BOOL overwrite)
//  public
//　機能　：既存のファイルをテンプレートにして、書き込み用にエクセルファイルを開く
//	引数１：オープンするエクセルブック名(NULLで終わる拡張子付き。)
//	引数２：テンプレートとなるエクセルブック名(NULLで終わる拡張子付き。)
//	引数３：ファイルオープンモード 
//			TRUE=filenameが存在する場合は一度クリアして開き、無いは新規に開く 
//			FALSE=filenameが存在する場合は開かない
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::OpenXlsByTemplate(char* filename, char* templatefile,BOOL overwrite)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind;

	m_Err = 0;

	// すでにオープンしてある
	if( m_Open ){
		m_Err = -1;		return(FALSE);
	}

	// ファイル名のチェック
	if( filename==NULL || templatefile==NULL){
		m_Err = -2;		return(FALSE);
	}

	// ファイル名のチェックその２
	if( !strlen(filename) || !strlen(templatefile)){
		m_Err = -2;		return(FALSE);
	}

	// overwriteしない場合は、filenameの有無チェック
	if(!overwrite){
		memset( &fd, 0, sizeof(fd) );
		fd.dwFileAttributes = FILE_ATTRIBUTE_ARCHIVE;

		// すでに存在する場合はエラーとする
		hFind=FindFirstFile(filename,&fd);
		if( hFind != INVALID_HANDLE_VALUE){
			FindClose(hFind);
			m_Err = -3;		return(FALSE);
		}
		FindClose(hFind);
	}

	// オープン
	if(!XlsOpenBook(filename,templatefile, &m_XlsID)){
		SetErrCode();return(FALSE);
	}

	// 成功
	m_Open=TRUE;
	return(TRUE);
}

//========================================================================================================
//	void TXls::Close(BOOL save);
//  public
//	引数１：TRUE=セーブしてクローズ FALSE=セーブせずにクローズ
//　機能　：開いているファイルをクローズする。
//========================================================================================================
void TXls::Close(BOOL save)
{
	// 未オープン
	if(!m_Open) return;

	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return;
	}


	// 閉じる
	XlsCloseBook(TRUE);

	m_Open=FALSE;
	m_Err=0;
}


//========================================================================================================
//	BOOL TXls::WriteStr(char* data, long x,long y)
//  public
//　機能　：セル番号を指定して、文字列データを書き込む
//	引数１：書き込みデータ(NULLで終)
//	引数２：セルのＸ位置
//	引数３：セルのＹ位置
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::WriteStr(char* data, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 文字列のチェック
	if( data==NULL ){
		m_Err = -5;		return(FALSE);
	}

	// 文字列のチェックその２
	if( !strlen(data)){
		m_Err = -5;		return(FALSE);
	}

	// 書き込み
	if(!XlsPutStr(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::WriteLong(long data, long x,long y)
//  public
//　機能　：セル番号を指定して、long型データを書き込む
//	引数１：書き込みデータ(NULLで終)
//	引数２：セルのＸ位置
//	引数３：セルのＹ位置
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::WriteLong(long data, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}
	// 書き込み
	if(!XlsPutLong(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::WriteDbl(double data, long x,long y)
//  public
//　機能　：セル番号を指定して、double型データを書き込む
//	引数１：書き込みデータ(NULLで終)
//	引数２：セルのＸ位置
//	引数３：セルのＹ位置
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::WriteDbl(double data, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsPutDouble(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::WriteVariant(char* data, long x,long y)
//  public
//　機能　：セル番号を指定して、文字列データを書き込む
//	引数１：書き込みデータ(NULLで終)
//	引数２：セルのＸ位置
//	引数３：セルのＹ位置
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::WriteVariant(char* data, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 文字列のチェック
	if( data==NULL ){
		m_Err = -5;		return(FALSE);
	}

	// 文字列のチェックその２
	if( !strlen(data)){
		m_Err = -5;		return(FALSE);
	}
long err=0;
	// 書き込み
	if(!XlsPutValue(data,POS(x,y,x,y))){
		SetErrCode();//	return(FALSE);
		XlsGetErrorNo(&err);
		printf("err=%ld\n",err);
		return FALSE;
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL ReadXls(char* filename);
//  public
//　機能　：読み込みようにエクセルファイルを開く
//	引数１：オープンするエクセルブック名(NULLで終わる拡張子付き。)
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::ReadXls(char* filename)
{

	m_Err = 0;

	// すでにオープンしてある
	if( m_Open ){
		m_Err = -1;		return(FALSE);
	}

	// ファイル名のチェック
	if( filename==NULL ){
		m_Err = -2;		return(FALSE);
	}

	// ファイル名のチェックその２
	if( !strlen(filename)){
		m_Err = -2;		return(FALSE);
	}

	// 既存ファイルオープンモード
	else{
		if(!XlsReadBook(filename, &m_XlsID)){
			SetErrCode();return(FALSE);
		}
	}

	// 成功
	m_Open=TRUE;
	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::ReadStr(char* data, int size, long x,long y)
//  public
//　機能　：セル番号を指定して、文字列データを読み込む
//	引数１：読み込みデータ(NULLで終)
//	引数２：セルのＸ位置
//	引数３：セルのＹ位置
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::ReadStr(char* data, int size, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 文字列のチェック
	if( data==NULL ){
		m_Err = -5;		return(FALSE);
	}

	// 文字列のチェックその２
//	if( !strlen(data)){
//		m_Err = -5;		return(FALSE);
//	}

	// 読み込み
	if(!XlsGetStr(data,size,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::ReadLong(long data, long x,long y)
//  public
//　機能　：セル番号を指定して、long型データを読み込む
//	引数１：読み込みデータ(NULLで終)
//	引数２：セルのＸ位置
//	引数３：セルのＹ位置
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::ReadLong(long* data, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 読み込み
	if(!XlsGetLong(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::ReadDbl(double data, long x,long y)
//  public
//　機能　：セル番号を指定して、double型データを読み込む
//	引数１：読み込みデータ(NULLで終)
//	引数２：セルのＸ位置
//	引数３：セルのＹ位置
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::ReadDbl(double* data, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 読み込み
	if(!XlsGetDouble(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

void TXls::SetErrCode(void)
{
	// DLLのエラーコードを取得して、このクラスのerr用のﾒﾝﾊﾞ変数にセットする
	XlsGetErrorNo(&m_Err);

//char aaa[100];
//memset(aaa,0,sizeof(aaa));
//sprintf(aaa,"%ld",m_Err);
//MessageBox(NULL,aaa,"",MB_OK);

}


BOOL TXls::SetFormat( char* format,long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsPutFormat(format,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

// フォントサイズの指定 03.04.23
BOOL TXls::SetFontSize( double fsize,long x,long y,long ex,long ey)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;
	// 書き込み
	if(!XlsPutFontPoint(fsize,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
// フォントの指定 03.06.04
BOOL TXls::SetFont( char* font,long x,long y,long ex,long ey)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// 書き込み
	if(!XlsPutFontName(font,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

// フォント下線指定
BOOL TXls::SetFontLine( long ltype,long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsPutFontULine(ltype,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
BOOL TXls::SetRowHeight( double h ,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsPutRowHeight(h,POS(0,y,-1,-1))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

BOOL TXls::SetColWidth( long w ,long x)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	//if(!XlsPutColWidth(w,POS(x,0,-1,-1))){
	if(!XlsPutColWidth(w,POS(x,0,x,0))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

// 2008.09.20 coba追加 ver3より
BOOL TXls::SetColWidthEx( double w ,long x)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	//if(!XlsPutColWidth(w,POS(x,0,-1,-1))){
	if(!XlsPutColWidthEx(w,POS(x,0,x,0))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

BOOL TXls::CellJoint( BOOL onoff ,long sx, long sy, long ex,long ey)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsPutJoint(onoff,POS(sx,sy,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

BOOL TXls::SetFontColor(long color, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

//#define xcDefault       XL_XCDEFAULT        // カラーパレット 自動
//#define xcBlack         XL_XCBLACK          //                黒
//#define xcWhite         XL_XCWHITE          //                白
//#define xcRed           XL_XCRED            //                赤
//#define xcGreen         XL_XCGREEN          //                緑
//#define xcBlue          XL_XCBLUE           //                青
//#define xcYellow        XL_XCYELLOW         //                黄
//#define xcPink          XL_XCPINK           //                ピンク
//#define xcCyan          XL_XCCYAN           //                水

//#define XLP_DEFAULT          0  // 自動
//#define XLP_BLACK            8  // 黒
//#define XLP_WHITE            9  // 白
//#define XLP_RED             10  // 赤
//#define XLP_GREEN           11  // 緑
//#define XLP_BLUE            12  // 青
//#define XLP_YELLOW          13  // 黄
//#define XLP_MAGENTA         14  // 紫
//#define XLP_CYAN            15  // 水



	// 書き込み
	if(!XlsPutFontColor(color,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

BOOL TXls::SetCellColor(long color, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

//#define xcDefault       XL_XCDEFAULT        // カラーパレット 自動
//#define xcBlack         XL_XCBLACK          //                黒
//#define xcWhite         XL_XCWHITE          //                白
//#define xcRed           XL_XCRED            //                赤
//#define xcGreen         XL_XCGREEN          //                緑
//#define xcBlue          XL_XCBLUE           //                青
//#define xcYellow        XL_XCYELLOW         //                黄
//#define xcPink          XL_XCPINK           //                ピンク
//#define xcCyan          XL_XCCYAN           //                水

//#define XLP_DEFAULT          0  // 自動
//#define XLP_BLACK            8  // 黒
//#define XLP_WHITE            9  // 白
//#define XLP_RED             10  // 赤
//#define XLP_GREEN           11  // 緑
//#define XLP_BLUE            12  // 青
//#define XLP_YELLOW          13  // 黄
//#define XLP_MAGENTA         14  // 紫
//#define XLP_CYAN            15  // 水



	// 書き込み
	if(!XlsPutBackColor(color,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

BOOL TXls::SetPageProperty(char *Size,BOOL Portrait,double HdrMergin,double FrtMergin)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	long PageSize,Orientation;

	if(!memcmp(Size,"A3",2)) PageSize = 8;
	else if(!memcmp(Size,"A4",2)) PageSize = 9;
	else if(!memcmp(Size,"A5",2)) PageSize = 11;
	else if(!memcmp(Size,"B4",2)) PageSize = 12;
	else if(!memcmp(Size,"B5",2)) PageSize = 13;
	else  PageSize = 9;

	if( Portrait==TRUE ) Orientation = 2;
	else 	Orientation = 0;

	if(!XlsPageMode( PageSize, -1, Orientation , HdrMergin, FrtMergin)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

// 余白を示す値をmm単位で設定します。省略する場合は、-1を指定
BOOL  TXls::SetMargin(short Left,short Right,short Top,short Bottom)
{
	
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsSetMargin( Left, Right, Top, Bottom)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
//========================================================================================================
//	BOOL XlsPutBox(long LineStyle,long bt,char *Cell)
//	Box罫線の書き込みをします。
//	Cell	対象となるセル範囲を示すChar型のポインタを設定します。カレントセルを対象にする場合は、NULLを設定して下さい。
//	引数 LineStyleの設定値は次のとおりです。
//
//	定数	値	内容
//	lsNone	0x00000000	罫線なし（デフォルト値）
//	lsNormal	0x01000000	実線
//	lsThick	0x02000000	太線
//	lsBroken	0x03000000	破線
//	lsDot	0x04000000	点線
//	lsThick2	0x05000000	極太線
//	lsDouble	0x06000000	二重線
//	lsSlender	0x07000000	細実線
//	lsMidBroken	0x08000000	中破線*
//	lsSlnChain1	0x09000000	細一点鎖線*
//	lsMidChain1	0x0A000000	中一点鎖線*
//	lsSlnChain2	0x0B000000	細二点鎖線*
//	lsMidChain2	0x0C000000	中二点鎖線*
//	lsMidChains	0x0D000000	中一点斜鎖線*
//	xcDefault	0	自動（デフォルト値）
//	xcBlack	8	黒
//	xcWhite	9	白
//	xcRed	10	赤
//	xcGreen	11	緑
//	xcBlue	12	青
//	xcYellow	13	黄色
//	xcPink	14	ピンク
//	xcCyan	15	水色

//	引数 btの設定値は次のとおりです。

//	定数	値	内容

//	btBox	0	箱線（デフォルト値）
//	btLtc	1	格子線
//	btOver	2	上横線
//	btUnder	3	下横線
//	btLeft	4	左縦線
//	btRight	5	右縦線
//========================================================================================================
BOOL  TXls::DrawLine(long LineStyle,long bt,long sx, long sy, long ex,long ey)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsPutBox( LineStyle, bt, POS(sx,sy,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}



//				2002.10.15 coba 文字の位置指定を追加
//========================================================================================================
//	BOOL TXls::PutPosHorz(long horz,long x,long y,long ex,long ey)
//  public
//　機能　：セルの水平位置を指定する。第４・５引数を省略すると、２・３で指定したセルのみを設定
//	引数１：水平位置
//				phNormal	20	標準（デフォルト値）
//				phLeft	21	左詰め
//				phCenter	22	中央揃え
//				phRight	23	右詰め
//				phLoop	24	繰り返し
//				phBothEven	25	両端揃え
//				phSelCenter	26	選択範囲内で中央
//				phEven	27	均等割付
//	引数２：設定するセルのＸ位置(開始)
//	引数３：設定するセルのＹ位置(開始)
//	引数４：設定するセルのＸ位置(終了)※省略可
//	引数５：設定するセルのＹ位置(終了)※省略可
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::PutPosHorz(long horz,long x,long y,long ex,long ey)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// 書き込み
	if(!XlsPutPosHorz( horz,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//	2005.04.14 coba シートのデフォルトFont設定 SetDefFont()を追加
//========================================================================================================
//	BOOL TXls::SetDefFont(char* fname,double fsize=-1.0);
//  public
//　機能　：ブックのデフォルトフォントを設定
//	引数１：フォント名(NULLで終)
//	引数２：フォントサイズ(1.0〜409.0)。1.0以下を指定すると、デフォルト(11pt)を設定。※省略可
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::SetDefFont(char* fname,double fsize)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// サイズが有効ならば、セットする
	if( fsize >=1.0 && fsize <=409.0){
		if(!XlsPutDefFontPoint( fsize)){
			SetErrCode();
			return(FALSE);
		}
	}

	// フォントの設定
	if(!XlsPutDefFontName( fname)){
		SetErrCode();
//		return(FALSE);		// 失敗してもエラーにしない
	}

	return(TRUE);
}
//				2005.05.19 coba 垂直位置指定PutPosVert()を追加
//========================================================================================================
//	BOOL TXls::PutPosVert(long vert,long x,long y,long ex,long ey)
//  public
//　機能　：セルの垂直位置を指定する。第４・５引数を省略すると、２・３で指定したセルのみを設定
//	引数１：垂直位置
//		pvUp	0	上詰め
//		pvCenter	1	中央揃え
//		pvDown	2	下詰め（デフォルト値）
//		pvBothEven	3	両端揃え
//		pvEven	4	均等割付
//	引数２：設定するセルのＸ位置(開始)
//	引数３：設定するセルのＹ位置(開始)
//	引数４：設定するセルのＸ位置(終了)※省略可
//	引数５：設定するセルのＹ位置(終了)※省略可
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::PutPosVert(long vert,long x,long y,long ex,long ey)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// 書き込み
	if(!XlsPutPosVert( vert,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
//========================================================================================================
//	BOOL TXls::PutFit(BOOL onoff,long x,long y,long ex,long ey)
//  public 2006.08.31 coba追加
//　機能　：「縮小して全体を表示」を指定する
//	引数１：ON=設定,OFF=解除
//	引数２：設定するセルのＸ位置(開始)
//	引数３：設定するセルのＹ位置(開始)
//	引数４：設定するセルのＸ位置(終了)※省略可
//	引数５：設定するセルのＹ位置(終了)※省略可
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::PutFit(BOOL onoff,long x,long y,long ex,long ey)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// 書き込み
	if(!XlsPutFit( onoff,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
//				2013.04.18 coba 文字のをスタイル追加
//========================================================================================================
//	BOOL TXls::SetFontStyle(long horz,long x,long y,long ex,long ey)
//  public
//　機能　：セルをフォントスタイル指定する。第４・５引数を省略すると、２・３で指定したセルのみを設定
//	引数１：スタイル
//	#define XL_XSNORMAL         0x00            // フォントスタイル ノーマル
//	#define XL_XSBOLD           0x01            //                  ボールド
//	#define XL_XSITALIC         0x02            //                  イタリック
//	#define XL_XSSTRIKE         0x04            //                  取消し線
//	#define XL_XSUP             0x08            //                  上付き
//	#define XL_XSDOWN           0x10            //                  下付き
//	引数２：設定するセルのＸ位置(開始)
//	引数３：設定するセルのＹ位置(開始)
//	引数４：設定するセルのＸ位置(終了)※省略可
//	引数５：設定するセルのＹ位置(終了)※省略可
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::SetFontStyle(long style,long x,long y,long ex,long ey)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// 書き込み
	if(!XlsPutFontStyle( style,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::PutFit(BOOL onoff,long x,long y,long ex,long ey)
//  public 2007.03.07 matsuda追加
//　機能　：シートのコピー機能
//	引数１：コピー元シート番号
//	引数２：コピー先シート番号
//	引数３：シート名
//	返値　：TRUE=成功、FALSE=失敗
//	注意！！エクセルクリエータ3関数です。配布DLLはかならずVer3以上を使ってください
//========================================================================================================
BOOL TXls::CopySheet( long posSource, long posInsert, LPCTSTR pszSheetName )
{
	if( posSource < 0 ) return FALSE;
	if( posInsert < 0 ) return FALSE;
	
	if( XlsCopySheet( posSource,posInsert,pszSheetName ) == FALSE ) return FALSE;
	return TRUE;
}

//========================================================================================================
//	行の操作に関する関数
//	引数  ：sy 開始行 or コピー元行
//	引数  ：dy コピー先行
//	引数  ：sy 行数
//	返値　：TRUE=成功、FALSE=失敗
//	注意！！エクセルクリエータ3関数です。配布DLLはかならずVer3以上を使ってください
//========================================================================================================
BOOL TXls::RowDelete(long sy, long lCount)
{
	int ret=0;
	ret=XlsRowDelete(sy,lCount);
	return(ret);
}
BOOL TXls::RowClear(long sy, long lCount)
{
	int ret=0;
	ret=XlsRowClear(sy,lCount);
	return(ret);
}
BOOL TXls::RowInsert(long sy, long lCount)
{
	int ret=0;
	ret=XlsRowInsert(sy,lCount);
	return(ret);
}
BOOL TXls::RowCopy(long sy, long dy)
{
	int ret=0;
	ret=XlsRowCopy(sy,dy);
	return(ret);
}
BOOL TXls::RowPaste(long dy)
{
	int ret=0;
	ret=XlsRowPaste(dy);
	return(ret);
}

//========================================================================================================
//	列の操作に関する関数
//	引数  ：sy 開始列 or コピー元列
//	引数  ：dy コピー先列
//	引数  ：sy 列数
//	返値　：TRUE=成功、FALSE=失敗
//	注意！！エクセルクリエータ3関数です。配布DLLはかならずVer3以上を使ってください
//========================================================================================================
BOOL TXls::ColumnDelete(long sx, long lCount)
{
	int ret=0;
	ret=XlsColumnDelete(sx,lCount);
	return(ret);
}
BOOL TXls::ColumnClear(long sx, long lCount)
{
	int ret=0;
	ret=XlsColumnClear(sx,lCount);
	return(ret);
}
BOOL TXls::ColumnInsert(long sx, long lCount)
{
	int ret=0;
	ret=XlsColumnInsert(sx,lCount);
	return(ret);
}
BOOL TXls::ColumnCopy(long sx, long dx)
{
	int ret=0;
	ret=XlsColumnCopy(sx,dx);
	return(ret);
}
BOOL TXls::ColumnPaste(long dx)
{
	int ret=0;
	ret=XlsColumnPaste(dx);
	return(ret);
}
// 印刷範囲の指定 2008.12.10追加
int TXls::SetPrnArea(int sx, int sy, int ex,int ey)
{
	return(XlsSetPrintArea(sx,sy,ex,ey));
}
//====================================================================
//	関数名：void TXls::XlsViewPrint(BOOL default_pr,char *filename,int startsheet,int endsheet)
//	引数１：デフォルトプリンタを使用する場合:TRUE
//	引数２：印刷するファイル名
//	引数３：印刷開始シート(0から開始) 
//	引数４：印刷終了シート
//	2010.12.23 ﾏﾂﾀﾞ 追加
//====================================================================
void TXls::XlsViewPrint(BOOL default_pr,char *filename,int startsheet,int endsheet)
{
	// デフォルトプリンタを使用しない場合 = プリンタ選択する場合
	if(default_pr==FALSE){
		if( ViewSetPrint(NULL) == FALSE ) return;
	}
	// ファイル名指定
	ViewPutUrlName(NULL, filename);
	// 印刷実行
	ViewPrint(NULL ,startsheet ,endsheet);
}
