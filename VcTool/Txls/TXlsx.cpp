//================================================================================================
//	FileName	:TXlsx.H
//	Class		:TXlsx 
// 				:TXlsクラス互換のxlsx形式操作クラス
//	作成日		:2013.12.04	Coded by coba
//================================================================================================
#include "TXlsx.h"
#include <stdio.h>

//========================================================================================================
//	TXls::TXlsx(void)
//	引数　：なし
//	返値　：
//　機能　：コンストラクタ。特に何もせず
//========================================================================================================
TXls::TXls(void)
{
	// 各パラメータのイニシャライズ
	m_Open=FALSE;
	m_Err = 0;
	m_xid = NULL;
}

//========================================================================================================
//	TXls::~TXlsx()
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

	// ハンドル取得
	if(FALSE==XlsxCreate(&m_xid)){
		m_Err = -3;		return(FALSE);
	}

	// 新規作成モード
	if( overwrite ){
		if( !XlsxCreateBook(m_xid,filename, ver2007, sheetMax, NULL)){
			SetErrCode();return(FALSE);
		}
	}

	// 既存ファイルオープンモード
	else{
		if(!XlsxOpenBook(m_xid,filename,NULL, NULL)){
			SetErrCode();return(FALSE);
		}
	}


	// 成功
	m_Open=TRUE;
	return(TRUE);
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

	// ハンドル取得
	if(FALSE==XlsxCreate(&m_xid)){
		m_Err = -3;		return(FALSE);
	}

	// オープン
	if(!XlsxOpenBook(m_xid,filename,templatefile, NULL)){
		SetErrCode();return(FALSE);
	}

	// 成功
	m_Open=TRUE;
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
	// ハンドル取得
	if(FALSE==XlsxCreate(&m_xid)){
		m_Err = -3;		return(FALSE);
	}

	if(!XlsxReadBook(m_xid,filename, NULL)){
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
	if(m_xid==NULL) return;

	// 閉じる
	XlsxCloseBook(m_xid,save);
	XlsxRelease(m_xid);

	m_Open=FALSE;
	m_Err=0;
	m_xid=NULL;
}


//========================================================================================================
//	BOOL AddSheet(long Page);
//  public
//　機能　：ｵｰﾌﾟﾝ中のエクセルファイルにシートを追加する
//	引数１：追加するシート数
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::AddSheet(long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsxAddSheet(m_xid,Page,-1);
	if(m_Err==FALSE){
		SetErrCode();return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL DelSheet(long SheetNo,long Page);
//  public
//　機能　：ｵｰﾌﾟﾝ中のエクセルファイルのシートを削除する
//	引数１：削除するシートNo.(0～)
//	引数２：シート数
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::DelSheet(long SheetNo,long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsxDelSheet(m_xid,SheetNo,Page);

	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL SetSheetName(char* Sheetname);
//  public
//　機能　：現在カレントのシートに名前を付ける
//	引数１：エクセルシート名(NULLで終わる拡張子付き。)
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::SetSheetName(char* SheetName)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsxPutSheetName(m_xid,SheetName);
	if(m_Err==FALSE){
		SetErrCode();return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL GetSheetName(long SheetNo,char* SheetName);
//  public
//　機能　：エクセルファイルのシートの名前を取得する
//	引数１：エクセルシートNo.
//	引数２：エクセルシート名バッファ(NULLで終わる拡張子付き。64バイト以上確保)
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::GetSheetName(long SheetNo,char* SheetName,int size)
{
	int preNo;
	char preSheet[100];

	if( m_Open!=TRUE ) 	return(FALSE);

	// 現在のシートを取得
	memset( preSheet,0,sizeof(preSheet));
	m_Err = XlsxGetSheetName(m_xid,preSheet);
	if(m_Err==FALSE){
		SetErrCode();return(FALSE);
	}
	// 現在のシート№を取得
	preNo=XlsxGetSheetNo2(m_xid,preSheet);
	if( preNo < 0 ){
		SetErrCode();return(FALSE);
	}

	// 現在のシートと違う場合は切り替える
	if( preNo != SheetNo ){
		m_Err = XlsxPutSheetNo(m_xid,SheetNo);
		if(m_Err==FALSE){
			SetErrCode();return(FALSE);
		}
	}

	// 指定されたシート名を取得
	m_Err = XlsxGetSheetName(m_xid,SheetName);
	if(m_Err==FALSE){
		SetErrCode();return(FALSE);
	}

	// 現在のシートと違う場合は戻す
	if( preNo != SheetNo ){
		m_Err = XlsxPutSheetNo(m_xid,preNo);
		if(m_Err==FALSE){
			SetErrCode();return(FALSE);
		}
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

	m_Err = XlsxPutSheetNo(m_xid,Page);
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

	m_Err = XlsxPutActiveSheet(m_xid,Page);
	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
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

	// 文字列のチェック
	if( data==NULL ){
		m_Err = -5;		return(FALSE);
	}

	// 文字列のチェックその２
	if( !strlen(data)){
		m_Err = -5;		return(FALSE);
	}

	// セル移動
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxPutString(m_xid,data)){
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
	// セル移動
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}
	// 書き込み
	if(!XlsxPutLong(m_xid,data)){
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
	// セル移動
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}
	// 書き込み
	if(!XlsxPutDouble(m_xid,data)){
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
	// XlsxPutValue()の実装が違うので、そのまま文字列として書き込む 2013.12.04 coba
	return(this->WriteStr(data,x,y));
/*
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// ねんのため
	if(!XlsSelectBook(m_xid)){
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
	// セル移動
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxPutValue(data,POS(x,y,x,y))){
		SetErrCode();//	return(FALSE);
		XlsGetErrorNo(&err);
		return FALSE;
	}

	return(TRUE);
*/
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

	// 文字列のチェック
	if( data==NULL ){
		m_Err = -5;		return(FALSE);
	}

	// セル移動
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// 読み込み
	if(!XlsxGetString(m_xid,data)){
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
	long val;

	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// セル移動
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// 読み込み
	val = XlsxGetLong(m_xid);
	*data = val;

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
	double val;

	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// セル移動
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// 読み込み
	val=XlsxGetDouble(m_xid);
	*data=val;
	return(TRUE);
}

void TXls::SetErrCode(void)
{
	// DLLのエラーコードを取得して、このクラスのerr用のﾒﾝﾊﾞ変数にセットする
	m_Err=XlsxGetErrorNo(m_xid);

//char aaa[100];
//memset(aaa,0,sizeof(aaa));
//sprintf(aaa,"%ld",m_Err);
//MessageBox(NULL,aaa,"",MB_OK);

}

//========================================================================================================
// BOOL TXls::SetFormat( char* format,long x,long y)
// 表示書式設定
// 引数１:書式(エクセルの)
// 引数２：設定するセルX位置
// 引数２：設定するセルY位置
//========================================================================================================
BOOL TXls::SetFormat( char* format,long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// セル移動
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutFormat(m_xid,format)){
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

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// セル移動
	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutFontPoint(m_xid,fsize)){
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

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// セル移動
	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutFontName(m_xid,font)){
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
//	引数２：フォントサイズ(1.0～409.0)。1.0以下を指定すると、デフォルト(11pt)を設定。※省略可
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
		if(!XlsxPutDefFontPoint( m_xid,fsize)){
			SetErrCode();
			return(FALSE);
		}
	}

	// フォントの設定
	if(!XlsxPutDefFontName( m_xid,fname)){
		SetErrCode();
//		return(FALSE);		// 失敗してもエラーにしない
	}

	return(TRUE);
}


//========================================================================================================
// BOOL TXls::SetFormat( char* format,long x,long y)
// フォント下線指定
// 引数１:下線スタイル
//	xlsxcrt.h
//		xfuNone  0  下線なし(デフォルト値)  
//		xfuNormal  1  一重下線  
//		xfuDouble  2  二重下線  
// 	TXls互換
//		fuNone  0  下線なし(デフォルト値)  
//		fuNormal  1  一重下線  
//		fuDouble  2  二重下線  
// 引数２：設定するセルX位置
// 引数２：設定するセルY位置
//========================================================================================================
BOOL TXls::SetFontLine( long ltype,long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// セル移動
	if( !XlsxPos(m_xid,x,y,x,y)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutFontULine(m_xid,ltype)){
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

	// セル移動
	if( !XlsxPos(m_xid,0,y,0,y)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxPutRowHeight(m_xid,h)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

// セル幅
BOOL TXls::SetColWidth( long w ,long x)
{
	return(this->SetColWidthEx((double)w,x));

}

// セル幅
BOOL TXls::SetColWidthEx( double w ,long x)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// セル移動
	if( !XlsxPos(m_xid,x,0,x,0)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxPutColWidth(m_xid,w)){
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

	// 範囲指定
	if( !XlsxPos(m_xid,sx,sy,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutJoint(m_xid,onoff)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}


//========================================================================================================
// BOOL TXls::SetFontColor(long color, long x,long y)
// フォント色の設定
// 引数１:色(xlsxcrt.h参照)
//		以下はXls互換用
//		#define xcDefault				_xclAuto				// 自動
//		#define xcBlack					_xclBlack				// 黒
//		#define xcWhite					_xclWhite				// 白
//		#define xcRed					_xclRed					// 赤
//		#define xcGreen					_xclGreen				// 緑
//		#define xcBlue					_xclBlue				// 青
//		#define xcYellow				_xclYellow				// 黄
//		#define xcPink					_xclPink				// ピンク
//		#define xcCyan					_xclCyan				// 水
//========================================================================================================
BOOL TXls::SetFontColor(long color, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// 範囲指定
	if( !XlsxPos(m_xid,x,y,x,y)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutFontColor(m_xid,color)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
// BOOL TXls::SetCellColor(long color, long x,long y)
// セル背景色の設定
// 引数１:色(xlsxcrt.h参照)
//		以下はXls互換用
//		#define xcDefault				_xclAuto				// 自動
//		#define xcBlack					_xclBlack				// 黒
//		#define xcWhite					_xclWhite				// 白
//		#define xcRed					_xclRed					// 赤
//		#define xcGreen					_xclGreen				// 緑
//		#define xcBlue					_xclBlue				// 青
//		#define xcYellow				_xclYellow				// 黄
//		#define xcPink					_xclPink				// ピンク
//		#define xcCyan					_xclCyan				// 水
//========================================================================================================
BOOL TXls::SetCellColor(long color, long x,long y)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// 範囲指定
	if( !XlsxPos(m_xid,x,y,x,y)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutBackColor(m_xid,color)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}


//========================================================================================================
// ページ設定
// 引数１:ページサイス "A3","A4","A5","B5"
// 引数２:用紙の向き TRUE=縦
// 引数３：ヘッダ余白
// 引数４：フッダ余白
//========================================================================================================
BOOL TXls::SetPageProperty(char *Size,BOOL Portrait,double HdrMergin,double FrtMergin)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
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

	if(!XlsxPageMode(m_xid, PageSize, -1, Orientation , (int)HdrMergin, (int)FrtMergin)){
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

	// 書き込み
	if(!XlsxSetMargin( m_xid,Left, Right, Top, Bottom)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
//========================================================================================================
//	BOOL  TXls::DrawLine(long LineStyle,long bt,long sx, long sy, long ex,long ey)
//	Cell	対象となるセル範囲を示すChar型のポインタを設定します。カレントセルを対象にする場合は、NULLを設定して下さい。
//	引数 LineStyle xlsxcrt.h参照
//	以下は互換用
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
BOOL  TXls::DrawLine(long LineStyle,long bt,long sx, long sy, long ex,long ey,int color/*=_xclBlack*/)
{
	// 未オープン
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// 範囲指定
	if( !XlsxPos(m_xid,sx,sy,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutBox( m_xid,LineStyle, bt,color)){
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

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// 範囲指定
	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutPosHorz( m_xid,horz)){
		SetErrCode();	return(FALSE);
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

	// 範囲指定
	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;
	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}


	// 書き込み
	if(!XlsxAttrPutPosVert( m_xid,vert)){
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

	// 範囲指定
	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;
	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutFit( m_xid,onoff)){
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

	// 範囲指定
	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// 書き込み
	if(!XlsxAttrPutFontStyle( m_xid,style)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
// BOOL TXls::CopySheet( long posSource, long posInsert, LPCTSTR pszSheetName )
//  public 2007.03.07 matsuda追加
//　機能　：シートのコピー機能
//	引数１：コピー元シート番号
//	引数２：コピー先シート番号
//	引数３：シート名
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::CopySheet( long posSource, long posInsert, LPCTSTR pszSheetName )
{
	if( posSource < 0 ) return FALSE;
	if( posInsert < 0 ) return FALSE;
	
	if( XlsxCopySheet( m_xid,posSource,posInsert,pszSheetName ) == FALSE ) return FALSE;
	return TRUE;
}

//========================================================================================================
//	行の操作に関する関数
//	引数  ：sy 開始行 or コピー元行
//	引数  ：dy コピー先行
//	引数  ：sy 行数
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::RowDelete(long sy, long lCount)
{
	int ret=0;
	ret=XlsxRowDelete(m_xid,sy,lCount);
	return(ret);
}
BOOL TXls::RowClear(long sy, long lCount)
{
	int ret=0;
	ret=XlsxRowClear(m_xid,sy,lCount);
	return(ret);
}
BOOL TXls::RowInsert(long sy, long lCount)
{
	int ret=0;
	ret=XlsxRowInsert(m_xid,sy,lCount);
	return(ret);
}
BOOL TXls::RowCopy(long sy, long dy)
{
	int ret=0;
	ret=XlsxRowCopy(m_xid,sy,dy);
	return(ret);
}
BOOL TXls::RowPaste(long dy)
{
	int ret=0;
	ret=XlsxRowPaste(m_xid,dy);
	return(ret);
}

//========================================================================================================
//	列の操作に関する関数
//	引数  ：sy 開始列 or コピー元列
//	引数  ：dy コピー先列
//	引数  ：sy 列数
//	返値　：TRUE=成功、FALSE=失敗
//========================================================================================================
BOOL TXls::ColumnDelete(long sx, long lCount)
{
	int ret=0;
	ret=XlsxColumnDelete(m_xid,sx,lCount);
	return(ret);
}
BOOL TXls::ColumnClear(long sx, long lCount)
{
	int ret=0;
	ret=XlsxColumnClear(m_xid,sx,lCount);
	return(ret);
}
BOOL TXls::ColumnInsert(long sx, long lCount)
{
	int ret=0;
	ret=XlsxColumnInsert(m_xid,sx,lCount);
	return(ret);
}
BOOL TXls::ColumnCopy(long sx, long dx)
{
	int ret=0;
	ret=XlsxColumnCopy(m_xid,sx,dx);
	return(ret);
}
BOOL TXls::ColumnPaste(long dx)
{
	int ret=0;
	ret=XlsxColumnPaste(m_xid,dx);
	return(ret);
}
// 印刷範囲の指定 2008.12.10追加
int TXls::SetPrnArea(int sx, int sy, int ex,int ey)
{
	return(XlsxPrintArea(m_xid,sx,sy,ex,ey));
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
	// ビューワがないのでできません。。。。。
/*
	// デフォルトプリンタを使用しない場合 = プリンタ選択する場合
	if(default_pr==FALSE){
		if( ViewSetPrint(NULL) == FALSE ) return;
	}
	// ファイル名指定
	ViewPutUrlName(NULL, filename);
	// 印刷実行
	ViewPrint(NULL ,startsheet ,endsheet);
*/
}
