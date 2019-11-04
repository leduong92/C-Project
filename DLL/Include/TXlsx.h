// *verion 0.00
#ifndef TXLSX_H
#define TXLSX_H
//================================================================================================
//	FileName	:TXlsx.H
//	Class		:TXlsx 
// 				:TXlsクラス互換のxlsx形式操作クラス
//	作成日		:2013.12.04	Coded by coba
//------------------------------------------------------------------------------------------------
// Dinamic Link Library	:xlsxcrt.dll
// inport library		:xlsxcrt.lib	<-リンクに必要
//================================================================================================

#include "windows.h"
#include "xlsxCrt.h"

class TXls
{

	private:
		DWORD  m_xid;						// ファイルハンドル(ハンドルか？)
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
		BOOL SetActiveSheet(long page);
		BOOL CopySheet( long posSource, long posInsert, LPCTSTR pszSheetName );	//2007.03.08 matsuda

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

		BOOL SetColWidthEx(double w ,long x);	// 列幅の設定 2008.09.22


		BOOL SetFontColor( long color,long x,long y);	// フォントの色　2003.04.09　Y.kogai
		BOOL SetCellColor( long color,long x,long y);	// セルのバックカラー　2005.01.26　Y.kogai

		BOOL SetPageProperty(char *Size , BOOL Portrait=TRUE,double HdrMergin=-1,double FrtMergin=-1);	// 用紙設定
		BOOL SetMargin(short Left,short Right,short Top,short Bottom);			// マージンの設定
		int SetPrnArea(int sx, int sy, int ex,int ey);

		BOOL DrawLine(long LineStyle,long bt,long sx, long sy, long ex,long ey,int color=_xclBlack);

		BOOL SetDefFont(char* fname,double fsize=0.0);
		BOOL SetFontStyle( long style,long x,long y,long ex=-1,long ey=-1);

		// エラーの取得
		long GetError(void){return(m_Err);};

		// 文字の位置指定
		BOOL PutPosHorz(long horz,long x,long y,long ex=-1,long ey=-1);
		BOOL PutPosVert(long vert,long x,long y,long ex=-1,long ey=-1);

		// フォントを縮小して全体表示
		BOOL PutFit(BOOL onoff,long x,long y,long ex=-1,long ey=-1);

		// 行・列の操作
		BOOL RowDelete(long sy, long lCount);
		BOOL RowClear(long sy, long lCount);
		BOOL RowInsert(long sy, long lCount);
		BOOL RowCopy(long sy, long dy);
		BOOL RowPaste(long dy);
		BOOL ColumnDelete(long sx, long lCount);
		BOOL ColumnClear(long sx, long lCount);
		BOOL ColumnInsert(long sx, long lCount);
		BOOL ColumnCopy(long sx, long dx);
		BOOL ColumnPaste(long dx);

		//印刷
		void XlsViewPrint(BOOL default_pr,char *filename,int startsheet,int endsheet);
	protected:


};

// xls2互換のためのマクロ定義 coab追加
// フォント下線
#define fuDefault				_xfuNone				// なし
#define fuNone					_xfuNone				// なし
#define fuNormal				_xfuNormal				// 下線
#define fuDouble				_xfuDouble				// 二重線

// カラーパレット
#define xcDefault				_xclAuto				// 自動
#define xcBlack					_xclBlack				// 黒
#define xcWhite					_xclWhite				// 白
#define xcRed					_xclRed					// 赤
#define xcGreen					_xclGreen				// 緑
#define xcBlue					_xclBlue				// 青
#define xcYellow				_xclYellow				// 黄
#define xcPink					_xclPink				// ピンク
#define xcCyan					_xclCyan				// 水

// 罫線の線種
#define lsNone          _xbsNone           		// 罫線 なし
#define lsDefault       _xbsNone           		//      なし
#define lsNormal        _xbsThin  	       		//      実線
#define lsThick         _xbsMedium         		//      太線
#define lsBroken        _xbsDashed         		//      破線
#define lsDot           _xbsDotted         		//      点線
#define lsThick2        _xbsThick          		//      極太線
#define lsDouble        _xbsDouble        		//      二重線
#define lsSlender       _xbsHair       			//      細実線
#define lsMidBroken     _xbsMediumDashed    	//      中破線
#define lsSlnChain1     _xbsDashDot      		//      細一点鎖線
#define lsMidChain1     _xbsMediumDashDot   	//      中一点鎖線
#define lsSlnChain2     _xbsDashDotDot     		//      細二点鎖線
#define lsMidChain2     _xbsMediumDashDotDot    //      中二点鎖線
#define lsMidChains     _xbsSlantDashDot		//      中一点斜鎖線

// ボックスの形状
#define btBox           _xbtBox            // ボックスの形状 箱線
#define btLtc           _xbtLtc            //                格子線
#define btOver          _xbtOver           //                上横線
#define btUnder         _xbtUnder          //                下横線
#define btLeft          _xbtLeft           //                左縦線
#define btRight         _xbtRight          //                右縦線

// 水平位置
#define phNormal        _xphGeneral         // 水平位置 標準
#define phLeft          _xphLeft           	//          左詰
#define phCenter        _xphCenter         	//          中央揃え
#define phRight         _xphRight          	//          右詰
#define phLoop          _xphFill           	//          繰り返し
#define phBothEven      _xphJustify       	//          両端揃え
#define phSelCenter     _xphCenterContinuous  //        選択範囲内で中央
#define phEven          _xphDistributed     //          均等

// 垂直位置
#define pvUp            _xpvTop             // 垂直位置 上詰め
#define pvCenter        _xpvCenter         	//          中央揃え
#define pvDown          _xpvBottom          //          下詰め
#define pvBothEven      _xpvJustify      	//          両端揃え
#define pvEven          _xpvDistributed		//          均等

// フォントスタイル
#define xsDefault       _xfsDefault         // フォントスタイル ノーマル
#define xsNormal        _xfsNormal         //                  ノーマル
#define xsBold          _xfsBold           //                  ボールド
#define xsItalic        _xfsItalic         //                  イタリック
#define xsStrike        _xfsStrike         //                  取消し線
#define xsUp            _xfsUp             //                  上付き
#define xsDown          _xfsDown           //                  下付き

#endif

