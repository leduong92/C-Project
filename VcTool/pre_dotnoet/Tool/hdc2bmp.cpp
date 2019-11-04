//===================================================================================================================
//
//	FileName	: hdc2bmp.c
//				BMPファイルのセーブ
//	作成日		:2001.10.22 
//===================================================================================================================
#include "hdc2bmp.h"



//===========================================================================================================
//	関数名：BOOL Hdc2BmpFile(HWND appwnd,char* savefile)
//	機能　：指定されたウインドウ(主にアプリケーションウインドウ)をﾋﾞｯﾄﾏｯﾌﾟに変換して保存する
//			アプリケーションからはこの関数を呼べばOK
//	引数１：変換元のウインドウハンドル
//	引数２：保存するﾌｧｲﾙ名
//	戻り値：TRUE or FALSE
//==========================================================================================================
BOOL Hwnd2BmpFile(HWND appwnd,char* savefile)
{
	// 引数チェック
	if( NULL==appwnd || savefile==NULL || strlen(savefile)< 1 )return(FALSE);

	HBITMAP happbit;
	HANDLE hdib;
	HFILE fh;

    BITMAPFILEHEADER    hdr;
    LPBITMAPINFOHEADER  lpbi;
    BITMAPINFOHEADER    bi;
    DWORD               dwSize;


	// ウインドウのDDBﾋﾞｯﾄﾏｯﾌﾟを作成
	if( CreateAppBitmap( appwnd, &happbit)==FALSE) return(FALSE);

	hdib = DibFromBitmap( happbit);
	DeleteObject( happbit );

    //	DIBの全サイズを計算
    DIBInfo(hdib, &bi);
    dwSize = bi.biSize + PaletteSize(&bi) + bi.biSizeImage;

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

    hdr.bfType          = 0x4d42;		// 'BM'
    hdr.bfSize          = dwSize + sizeof(BITMAPFILEHEADER);
    hdr.bfReserved1     = 0;
    hdr.bfReserved2     = 0;
    hdr.bfOffBits       = (DWORD)sizeof(BITMAPFILEHEADER) + lpbi->biSize +
                          PaletteSize(lpbi);

	// 書き込みﾌｧｲﾙをオープン
	if( NULL==(fh	= _lcreat(savefile, NULL))){
	    GlobalUnlock(hdib);
		return(FALSE);
	}

	// BMPヘッダとﾃﾞｰﾀの書き込み
    _lwrite(fh, (LPCSTR)&hdr, sizeof(BITMAPFILEHEADER));
    _hwrite(fh, (LPCSTR)lpbi, dwSize);

    GlobalUnlock(hdib);

	_lclose(fh);

	return(TRUE);
}

//===========================================================================================================
//	関数名：HBITMAP	CreateAppBitmap(HWND hwnd)
//	機能　：指定されたウインドウのDDBﾋﾞｯﾄﾏｯﾌﾟを作成する。
//	引数１：変換元のウインドウハンドル
//	戻り値：TRUE or FALSE
//==========================================================================================================
BOOL	CreateAppBitmap(HWND hwnd,HBITMAP* bmp)
{
	HDC memdc;
	RECT rect;

	// 指定されたウインドウと同じサイズのﾋﾞｯﾄﾏｯﾌﾟを作成
	GetWindowRect(hwnd,&rect);
	*bmp = CreateCompatibleBitmap( GetDC(hwnd),rect.right-rect.left,rect.bottom-rect.top);
	if( *bmp==NULL ) return(FALSE);
	memdc=CreateCompatibleDC(GetDC(hwnd));
	if( memdc==NULL){
		DeleteObject(*bmp);
		return(FALSE);
	}
	*bmp = (HBITMAP)SelectObject(memdc,*bmp);

	// デスクトップ領域から、指定されたウインドウの場所をｺﾋﾟｰする
	BitBlt( memdc,0,0,rect.right-rect.left,rect.bottom-rect.top,
				GetDC(NULL),rect.left,rect.top,SRCCOPY);

	*bmp = (HBITMAP)SelectObject(memdc,*bmp);
	DeleteDC(memdc);
	return(TRUE);

}

BOOL DIBInfo(HANDLE hdib, LPBITMAPINFOHEADER lpbi)
{
    if(!hdib)
        return FALSE;

    *lpbi = *(LPBITMAPINFOHEADER)GlobalLock(hdib);

    GlobalUnlock(hdib);

	if (lpbi->biSizeImage == 0L)
		lpbi->biSizeImage = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount) * lpbi->biHeight;

    if (lpbi->biClrUsed == 0L)
        lpbi->biClrUsed = DIBNumColors(lpbi);

    return TRUE;
}


HANDLE DibFromBitmap(HBITMAP hbitmap)
{
	BITMAP				bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER	lpbi;
	DWORD				dwLen;
	WORD				biBits;
	HANDLE				dib;
	HDC					hdc;

	HPALETTE hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	GetObject(hbitmap, sizeof(bm), (LPSTR)&bm);
	biBits = (unsigned short)bm.bmPlanes * (unsigned short)bm.bmBitsPixel;

	bi.biSize           = sizeof(BITMAPINFOHEADER);
	bi.biWidth          = bm.bmWidth;
	bi.biHeight         = bm.bmHeight;
	bi.biPlanes         = 1;
//	bi.biBitCount       = biBits;
	bi.biBitCount       = 8;		// ※coba注：ここを８にすると、256色bmpになる
	bi.biCompression    = BI_RGB;
	bi.biSizeImage		= WIDTHBYTES((DWORD)bm.bmWidth * biBits) * bm.bmHeight;
	bi.biXPelsPerMeter  = 0;
	bi.biYPelsPerMeter  = 0;
	bi.biClrUsed        = 0;
	bi.biClrImportant   = 0;

	dwLen	= bi.biSize + PaletteSize(&bi) + bi.biSizeImage;
	hdc		= GetDC(0);
	hpal	= SelectPalette(hdc, hpal, FALSE);
	dib		= GlobalAlloc(GMEM_MOVEABLE, dwLen);
	lpbi	= (LPBITMAPINFOHEADER)GlobalLock(dib);

	RealizePalette(hdc);
	*lpbi = bi;

	GetDIBits(hdc,	hbitmap, 0,	(UINT)bi.biHeight,
			(LPSTR)lpbi + lpbi->biSize + PaletteSize(lpbi), (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	GlobalUnlock(dib);
	SelectPalette(hdc, hpal, FALSE);
	ReleaseDC(0, hdc);
	return dib;
}
WORD DIBNumColors(LPBITMAPINFOHEADER lpbi)
{
    int nBitCount;

	if(lpbi->biClrUsed != 0)
		return (WORD)lpbi->biClrUsed;

	nBitCount = lpbi->biBitCount;

    switch (nBitCount)
    {
        case 1:
            return 2;

        case 4:
            return 16;

        case 8:
            return 256;

        default:
            return 0;
    }
}
WORD PaletteSize(LPBITMAPINFOHEADER lpbi)
{
    WORD    NumColors;
    NumColors = DIBNumColors(lpbi);
	return NumColors * (unsigned short)sizeof(RGBQUAD);
}
