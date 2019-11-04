#ifndef GRIDVIEW_H
#define GRIDVIEW_H
//========================================================================================
//
//	FileName	:GridView.h
//	Class		:CGridView
// 				:グリッドウインドウ・ビュークラスver3
//	作成日		:2000.11.30	Coded by coba
//========================================================================================
//	修正
//	2001.01.10 coba	bitmapの解放し忘れのバグ取り
//	2002.06.12 coba	フォント指定機能追加(CreateGrid());
//					色を、ウインドウズが使用している色を使うようにした
//	2002.06.18 TAKE ヘッダグリッドクリック通知関数追加
//	2003.06.06 coba 入力終了時、フォーカスを失う機能(EndEdit())
//	2003.06.26 coba アクティブセルの表示を強制的に非表示にする関数追加 KillActCel()
//	2003.11.08 coba 表示中の左端のセル番号を取得する関数を追加
//	2004.06.09 coba 表示固定列の機能を追加 GRIDINFOのfixColsメンバで指定する
//	2004.10.15 coba vctoolにマージ
//	2006.08.03 ﾏﾂﾀﾞ 文字色指定（color=2 青）を追加 
//	2006.12.14 coba 文字の色設定を定数化
					/*	GVW_CLR_BLACK,
						GVW_CLR_RED,
						GVW_CLR_BLUE,
						GVW_CLR_LBLUE,
						GVW_CLR_GREEN,
						GVW_CLR_LGREEN,
						GVW_CLR_YELLOW,
						GVW_CLR_ORANGE,*/
// 2007.02.22 coba IBMの機種でホイールマウス操作時に不正な処理で飛ぶバグを修正
//					(本社小田D修正を反映)EvVscroll()
// 2013.06.19 coba ホイールマウス対応
// 2014.10.04 coba フォント太字対応
// 2015.01.15 coba 背景色を指定できるように修正
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>
#include <imm.h>
#include "grddata3.h"


// グリッド設定用の構造体
typedef struct{
	int fontsize;					// フォントサイズ
	BOOL rowtitle;					// 行見出し使用・不使用
	int  rowtitleWidth;				// 行見出し幅
	BOOL hScroll;					// 水平スクロール有無
	BOOL vScroll;
	int selecttype;					// 選択方式　０＝セル単位、１＝行単位
	long fixRows;					// 固定行数
	long fixCols;					// 固定列数 2004.06.09
	BOOL readonly;					// エディットボックスの使用・不使用
	BOOL fontBold;
} GRIDINFO;

// セル内における文字の位置
#define POS_LEFT	0
#define POS_CENTER	1
#define POS_RIGHT	2

// 上下左右
#define MV_UP 0
#define MV_DOWN 1
#define MV_LEFT 2
#define MV_RIGHT 3

// EndEdit()時の移動先指定定数
#define NOT_MOVE 0
#define MOVE_WITH_EDIT 1
#define MOVE_CELONLY 2
#define MOVE_LOSTFOCUS 3

// 文字色の設定
enum grdvw_font_color_id{
	GVW_CLR_BLACK,
	GVW_CLR_RED,
	GVW_CLR_BLUE,
	GVW_CLR_LBLUE,
	GVW_CLR_GREEN,
	GVW_CLR_DGREEN,
	GVW_CLR_YELLOW,
	GVW_CLR_ORANGE,
	GVW_CLR_DGRAY,
	GVW_CLR_LGRAY,
	GVW_CLR_WHITE,
};

// 文字色の設定 2015.01.15 coba
enum grdvw_bk_color_id{
	GVW_BKC_NORMAL,
	GVW_BKC_GRAY,
	GVW_BKC_BLUE,
	GVW_BKC_LBLUE,
	GVW_BKC_GREEN,
	GVW_BKC_DGREEN,
	GVW_BKC_YELLOW,
	GVW_BKC_ORANGE,
	GVW_BKC_DGRAY,
	GVW_BKC_LGRAY,
	GVW_BKC_RED,
	GVW_BKC_MAX,
};
class CGridViewCtrl : public TGridData
{
	private:
		//--------------------------------------------------------------------------------------------------
		// 継承先のクラスで自由にカスタマイズしてください
		// VB風に言えばイベントプロシージャ、MFCではメッセージハンドラと呼ばれるヤツです
		virtual BOOL EvEndEdit( long col, long row ,char *txt ,short *move ,long *nextCol,long *nextRow,char *color);
		virtual BOOL EvClick( long col,long row);
		virtual BOOL EvDblClick( long col,long row);
		virtual BOOL EvStartEdit( long col, long row );
		virtual BOOL EvKeyDown( WPARAM keycode );
		virtual BOOL EvMoveCelEnd( long col, long row);
		virtual BOOL EvEditBoxMove( long col, long row);
		virtual BOOL EvMove( long col, long row){return(TRUE);};

		virtual BOOL EvHeaderClick( long col );	// 2002.06.17 TAKE
		//---------------------------------------------------------------------------------------------------

		// セルコピーバッファ
		typedef struct{
			int  col;
			int row;
			long tag;
			char data[100+1];
			char color;
			char bkcolor;
		} CELLCOPYBUF;
		CELLCOPYBUF m_CellBuf;

	public:
		CGridViewCtrl(HINSTANCE inst,HWND hparent,long allocunit, long totalcol);
		~CGridViewCtrl();
		LONG DoCommand(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam );
		LONG EditProc(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam );
		LONG KeyProc( int nCode,WPARAM wParam, LPARAM lParam );

		BOOL DrawHeadder(BOOL redraw);

		BOOL SetCel(int col,int row, char* data,long cTag,BOOL draw,char color,char bkcolor);

		void Draw(void);

		void GetActiveCel( long *col, long *row ){
			*col = m_ActCel.col;
			*row = m_ActCel.row;
		};

		void KillActCel(void);

		BOOL MoveCel(int mbMode);
		BOOL SetLeftX(long newX);
		BOOL GetLeftX(void){
			return(m_Left);
		};


		BOOL ColsCopy(long targetCol);
		BOOL ColsPaste(long targetCol,BOOL insert,BOOL redraw);
		BOOL ColsCut(long targetCol ,BOOL redraw);

		BOOL AddNewCol(long targetCol,long width,short pos,BOOL redraw);

		BOOL DataClear( BOOL redraw );

		HWND m_ListWnd;
		HWND m_EditWnd;

		// 行見出しの変更
		BOOL ChangeRowTitle(long rowNo, char* newtitle);

		BOOL CellCopy(void);
		BOOL CellPaste(int chkmode);

	protected:
		BOOL CreateGrid(LPRECT pRect,GRIDINFO *gr,char* fontname=NULL,DWORD charset=0);
		BOOL SetColumnHeadder( long col, char *titlestr,long width,short position);
		BOOL EndEdit(int action);

		HWND m_hwndParent;
		// アクティブセルの移動
		BOOL MoveActiveCel(long newCol,long newRow, BOOL internal=FALSE);
		// セル指定グリッド描画
		BOOL DrawCel(int col, int row,BOOL act =FALSE);



		// エディットボックスのIME設定
		BOOL SetIme( int mode );
	private:
		BOOL DrawData( void );

		// ハンドル関係
		HINSTANCE m_hInst;
		HHOOK m_KeyHook;
		HBITMAP m_hBit,m_hOldBit;

		// リストウインドウのクライアント領域
		RECT m_ListRect;

		// リスト作成済みフラグ
		BOOL m_WindowInit;

		// グリッドinfo
		GRIDINFO m_info;

		// カレントセル
		struct ACT{
			long col;
			long row;
		}m_ActCel;

		// 表示サイズ関係
		short m_DispRows;			// 表示可能な行数
		short m_ColHeight;			// 1行あたりの高さ

		// 表示中の位置保存
		long m_Top;					// 表示中の最上部の行番号
		long m_Bottom;				// 表示中の最下部の行番号
		long m_Left;				// 表示中の一番左の列番号
		long m_Right;				// 			↑ 右の列番号
		long m_FixRows;				// 固定行（縦スクロールなし）の行数
		long m_FixCols;				// 固定列（横スクロールなし）の列数

		// フォーカスを失ったアクティブセルの描画
		BOOL LostFocusCel(int col,int row);

		// 描画用ﾌｫﾝﾄ
		HFONT m_hFont,m_tmpFont,m_eFont;
		HDC m_hdcList;

		// 入力用のエディットコントロール
		LONG m_oldEditProc,m_oldButtonProc;

		// メッセイジ処理
		BOOL EvInit( HWND hwnd);
		BOOL EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick);
		void EvVScroll(HWND hWnd,WORD event,WORD lastpos);
		void EvHScroll(HWND hWnd,WORD event,WORD lastpos);
		BOOL AutoScroll(long newCol,long newRow);

		// セルの編集
		DWORD m_editX,m_editY;					// 編集中の画面上の位置
		BOOL m_EditMode;						// 編集中=TRUE,
//		BOOL StartEdit(DWORD x,DWORD y,char asc=NULL);
		BOOL StartEdit(DWORD x,DWORD y,char asc=NULL,BOOL first=FALSE);
		BOOL MoveEditBox( long col, long row);



		// スクロールバー
		BOOL SetVScroll(void);
		BOOL SetHScroll(void);

		// 列の巾情報
		typedef struct{
			long width;
			short position;
		} COLUMNPOSITIONINFO;

		COLUMNPOSITIONINFO *m_ColInfo;

		// 押されたキーコードが数値関連か調べる
		BOOL IsKeyNumeric( char* theAsci, UINT key, UINT scanCd);

		BOOL GetImeStatus(void);

		// フォント 2002.06.12 coba
		char m_FontName[100+1];
		DWORD m_CharSet;

		// 定数からRGBを引く
		COLORREF GetColorFromId(int id);


};
#endif
