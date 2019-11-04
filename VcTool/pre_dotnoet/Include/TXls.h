// *verion 0.00
#ifndef TXLS_H
#define TXLS_H
//================================================================================================
//	NOK Fanatic Class Libraly
//	FileName	:TXls.H
//	Class		:TXls 
// 				:MS-Excel97(bif80)形式ファイル/書き込みクラス
//	作成日		:2000.01.08	Coded by coba
//				2000.07.4 TAKE シート追加ならびにシート名セット関数を追加
//				2001.06.12 coba 書式設定関数SetFormat()を追加
//				2001.09.12 TAKE 行高さ設定SetRowHeight()を追加
//				2001.09.12 TAKE 列幅設定SetColWidth()を追加
//				2001.09.12 TAKE セル結合解除CellJoint()を追加
//				2001.09.12 TAKE 用紙設定SetPage()を追加
//				2001.09.12 TAKE 用紙設定SetMargin()を追加
//				2001.10.02 TAKE 罫線DrawLine()を追加
//				2002.10.15 coba 文字の位置指定を追加
//				2003.04.23 coba フォントのサイズ指定
//				2003.06.04 coba フォント指定
//				2003.11.10 coba open時にシート数を指定できるように変更 OpenXls()
//				2004.05.08 coba フォントアンダーライン指定
//				2004.05.24 coba SetFont()とSetFontSize()とPutPosHorz()を範囲指定できるように修正
//------------------------------------------------------------------------------------------------
// Dinamic Link Library	:xlscrt2.dll
// inport library		:xlscrt2.lib	<-リンクに必要
//================================================================================================

#include "windows.h"
#include "xls2.h"

class TXls
{

	private:
		long  m_XlsID;						// ファイルハンドル(ハンドルか？)
		BOOL m_Open;
		long m_Err;
		void SetErrCode(void);

	public:
		TXls( void );
		~TXls( void );

		// ブックのオープンクローズ
		BOOL OpenXls(char* filename, BOOL overwrite, long sheetMax=3);
		BOOL OpenXlsByTemplate(char* filename, char* templatefile,BOOL overwrite);
		BOOL ReadXls(char* filename);
		void Close(BOOL save);
		BOOL AddSheet(long Page);
		BOOL DelSheet(long SheetNo,long Page);
		BOOL SetSheetName(char* SheetName);
		BOOL GetSheetName(long SheetNo,char* SheetName,int size);
		BOOL SetSheetNo(long Page);

		// セルへのデータ書き込み
		BOOL WriteStr(char* data, long x,long y);
		BOOL WriteLong(long data, long x,long y);
		BOOL WriteDbl(double data, long x,long y);
		BOOL WriteVariant(char* data, long x,long y);

		// セルのデータ読み込み
		BOOL ReadStr(char* data, int size, long x,long y);
		BOOL ReadLong(long* data, long x,long y);
		BOOL ReadDbl(double* data, long x,long y);

		// セルの書式設定
		BOOL SetFormat( char* format,long x,long y);
		BOOL SetRowHeight(double h,long y);	// 行高さの設定
		BOOL SetColWidth(long w ,long x);	// 列幅の設定
		BOOL CellJoint( BOOL onoff ,long sx, long sy, long ex,long ey);	// セルの結合
		BOOL SetFontSize( double fsize,long x,long y,long ex=-1,long ey=-1);
		BOOL SetFont( char* fontname,long x,long y,long ex=-1,long ey=-1);
		BOOL SetFontLine( long ltype,long x,long y);
	
		BOOL SetFontColor( long color,long x,long y);	// フォントの色　2003.04.09　Y.kogai
		BOOL SetCellColor( long color,long x,long y);	// セルのバックカラー　2005.01.26　Y.kogai

		BOOL SetPageProperty(char *Size , BOOL Portrait=TRUE,double HdrMergin=-1,double FrtMergin=-1);	// 用紙設定
		BOOL SetMargin(short Left,short Right,short Top,short Bottom);			// マージンの設定
		BOOL DrawLine(long LineStyle,long bt,long sx, long sy, long ex,long ey);
		
		// エラーの取得
		long GetError(void){return(m_Err);};

		// 文字の位置指定
		BOOL PutPosHorz(long horz,long x,long y,long ex=-1,long ey=-1);

	protected:


};
#endif

