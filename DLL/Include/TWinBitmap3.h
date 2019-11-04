#ifndef TWIN_BITMAP3_H
#define TWIN_BITMAP3_H
//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TWinBitmap3.h
//	Class		:TWinBitmap3
// 				:Bitmap Window class
//				 
//				 
//	Date		:2003.12.3	Coded by N.Maeda
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>



class TWinBitmap3
{
	private:

	public:
		TWinBitmap3(HINSTANCE inst,HWND hparent,BOOL white=FALSE );
		~TWinBitmap3();

		LONG DoCommand(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam );

		HWND m_thisWnd;

		BOOL CreateScreen3(LPRECT pRect,char* fname);
		BOOL LoadScreen3(char* fname);


	protected:


		HWND m_hwndParent;

		HDC m_hdcScrWin;

	private:
		BOOL DrawData( void );

		HINSTANCE m_hInst;

		BOOL m_WindowInit;

		BOOL EvInit( HWND hwnd);
		BOOL EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick);

		// Screen Size(Pixel)
		int m_Xsize, m_Ysize;

		HBITMAP m_hbit,m_hOldbit;

		// 
		BOOL m_bkWhite;

		// 
		char* m_bitData;

		char m_clsName[256];



		BITMAPFILEHEADER bmpFileHeader;
		BITMAPINFO		 bmpInfo;
		BYTE * bPixelBits;


		RECT m_Rect;

};
#endif
