
#ifndef TSCREEN_WIN_H
#define TSCREEN_WIN_H
//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TScrWin.h
//	Class		:CGridView
// 				:スクリーンウインドウクラス
//				 ウインドウに張り付けて描画をする等に使用。派生クラスでグラフクラス等を。
//				 
//	作成日		:2000.07.27	Coded by coba
//					※描画関数の単位系は全てピクセルです
//========================================================================================
//	修正		:
//	2003.11.17 coba 枠線描画関数DrawBox()の追加
//	2004. 4. 5 Maeda 縦書き文字とボタン処理追加
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>



class TScreenWindow
{
	private:
		//--------------------------------------------------------------------------------------------------
		// 継承先のクラスで自由にカスタマイズしてください
		// VB風に言えばイベントプロシージャ、MFCではメッセージハンドラと呼ばれるヤツです
		virtual BOOL EvClick( long col,long row);
		//---------------------------------------------------------------------------------------------------

	public:
		TScreenWindow(HINSTANCE inst,HWND hparent,BOOL white=FALSE );
		~TScreenWindow();

		LONG DoCommand(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam );

		void Draw(void);
		HWND m_thisWnd;
		BOOL CreateScreen(LPRECT pRect);

		// 真円
		void DrawCircle( int x,int y,int r, int LineWidth,BOOL paint,COLORREF theColor,COLORREF interColor);

		// 線
		void DrawLine( int sx,int sy,int ex, int ey,int LineType,int LineWidth,COLORREF theColor,BOOL redraw=FALSE);

		void TextOut( int x, int y, int fontsize, char* txt ,COLORREF color,char* font="ＭＳ ゴシック",int charset=SHIFTJIS_CHARSET);
		void TextOutV( int x, int y, int fontsize, char* txt ,COLORREF color,char* font, int charset);
		// 枠（塗りつぶし色指定可能）
		void DrawBox( int sx, int sy, int ex, int ey,int LineType,int LineWidth,COLORREF theColor
																		,BOOL paint,COLORREF bkcolor);

		// リセット（真っ黒けにする）
		void ResetScreen( void );

		// プロットを１ピクセル打つ
		void SetPlot(int x,int y,COLORREF color);

		void Redraw(void);

	protected:


		HWND m_hwndParent;

		HDC m_hdcScrWin;

		// エディットボックスのIME設定
		BOOL SetIme( int mode );

	private:
		BOOL DrawData( void );

		// ハンドル関係
		HINSTANCE m_hInst;

		// リスト作成済みフラグ
		BOOL m_WindowInit;

		// 描画用ﾌｫﾝﾄ
		HFONT m_hFont,m_tmpFont,m_eFont;

		// メッセイジ処理
		BOOL EvInit( HWND hwnd);
		BOOL EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick);

		// スクリーンのサイズ（ピクセルです）
		int m_Xsize, m_Ysize;

		HBITMAP m_hbit,m_hOldbit;

		// 背景色
		BOOL m_bkWhite;

};
#endif
