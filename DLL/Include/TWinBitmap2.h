#ifndef TWIN_BITMAP_H
#define TWIN_BITMAP_H
//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TWinBitmap2.h
//	Class		:TWinBitmap2
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



class TWinBitmap2
{
	private:
		//--------------------------------------------------------------------------------------------------
		// 継承先のクラスで自由にカスタマイズしてください
		// VB風に言えばイベントプロシージャ、MFCではメッセージハンドラと呼ばれるヤツです
//		virtual BOOL EvClick( long col,long row);
		//---------------------------------------------------------------------------------------------------

	public:
		TWinBitmap2(HINSTANCE inst,HWND hparent,BOOL white=FALSE );
		~TWinBitmap2();

		LONG DoCommand(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam );

		HWND m_thisWnd;
//		BOOL CreateScreen(LPRECT pRect);
//		// Bitmap画像関係
//		BOOL LoadBitMap( char* fname );

		BOOL CreateScreen2(LPRECT pRect,char* fname);


		// リセット（真っ黒けにする）
//		void ResetScreen( void );
//
//		void Redraw(void);
//

	protected:


		HWND m_hwndParent;

		HDC m_hdcScrWin;

	private:
		BOOL DrawData( void );

		// ハンドル関係
		HINSTANCE m_hInst;

		// リスト作成済みフラグ
		BOOL m_WindowInit;

		// メッセイジ処理
		BOOL EvInit( HWND hwnd);
		BOOL EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick);

		// スクリーンのサイズ（ピクセルです）
		int m_Xsize, m_Ysize;

		HBITMAP m_hbit,m_hOldbit;

		// 背景色
		BOOL m_bkWhite;

		// 画像ファイル
//		BITMAPINFO m_bInfo;
		char* m_bitData;

		char m_clsName[256];



		BITMAPFILEHEADER bmpFileHeader;
		BITMAPINFO		 bmpInfo;
		BYTE * bPixelBits;

};
#endif
