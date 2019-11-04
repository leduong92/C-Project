#ifndef TWIN_BITMAP_H
#define TWIN_BITMAP_H
//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TWinBitmap.h
//	Class		:TWinBitmap
// 				:ビットマップウインドウクラス
//				 
//				 
//	作成日		:2003.12.3	Coded by N.Maeda
//	修正		:
//					※描画関数の単位系は全てピクセルです
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>



class TWinBitmap
{
	private:
		//--------------------------------------------------------------------------------------------------
		// 継承先のクラスで自由にカスタマイズしてください
		// VB風に言えばイベントプロシージャ、MFCではメッセージハンドラと呼ばれるヤツです
		virtual BOOL EvClick( long col,long row);
		//---------------------------------------------------------------------------------------------------

	public:
		TWinBitmap(HINSTANCE inst,HWND hparent,BOOL white=FALSE );
		~TWinBitmap();

		LONG DoCommand(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam );

		void Draw(void);
		HWND m_thisWnd;
		BOOL CreateScreen(LPRECT pRect);

		// リセット（真っ黒けにする）
		void ResetScreen( void );

		void Redraw(void);

		// Bitmap画像関係
		BOOL LoadBitMap( char* fname );

	protected:


		HWND m_hwndParent;

		HDC m_hdcScrWin;

	private:
		BOOL DrawData( void );

		// ハンドル関係
		HINSTANCE m_hInst;

		// リスト作成済みフラグ
		BOOL m_WindowInit;

//		// 描画用ﾌｫﾝﾄ
//		HFONT m_hFont,m_tmpFont,m_eFont;

		// メッセイジ処理
		BOOL EvInit( HWND hwnd);
		BOOL EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick);

		// スクリーンのサイズ（ピクセルです）
		int m_Xsize, m_Ysize;

		HBITMAP m_hbit,m_hOldbit;

		// 背景色
		BOOL m_bkWhite;

		// 画像ファイル
		BITMAPINFO m_bInfo;
		char* m_bitData;
};
#endif
