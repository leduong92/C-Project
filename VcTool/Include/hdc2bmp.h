
#ifndef HDC2BMP_H
#define HDC2BMP_H
//===================================================================================================================
//
//	FileName	: hdc2bmp.c
//				BMPファイルのセーブ
//	作成日		:2001.10.22 
//===================================================================================================================

#include <windows.h>
#include <winuser.h>
#include <wingdi.h>


#define WIDTHBYTES(i)   ((i+31)/32*4)


 // アプリケーションからはこの関数を呼ぶ
BOOL	Hwnd2BmpFile(HWND appwnd,char* savefile);


BOOL	CreateAppBitmap(HWND hwnd,HBITMAP* bitmap);
// 以下はパクリfrom ASCII Windows95 プログラミング３章
HANDLE	DibFromBitmap (HBITMAP hbitmap);
BOOL	DIBInfo (HANDLE hdib, LPBITMAPINFOHEADER lpbi);
WORD	PaletteSize (LPBITMAPINFOHEADER lpbi);
WORD    DIBNumColors (LPBITMAPINFOHEADER lpbi);

#endif
