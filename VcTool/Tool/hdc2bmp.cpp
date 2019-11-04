//===================================================================================================================
//
//	FileName	: hdc2bmp.c
//				BMP�t�@�C���̃Z�[�u
//	�쐬��		:2001.10.22 
//===================================================================================================================
#include "hdc2bmp.h"



//===========================================================================================================
//	�֐����FBOOL Hdc2BmpFile(HWND appwnd,char* savefile)
//	�@�\�@�F�w�肳�ꂽ�E�C���h�E(��ɃA�v���P�[�V�����E�C���h�E)���ޯ�ϯ�߂ɕϊ����ĕۑ�����
//			�A�v���P�[�V��������͂��̊֐����Ăׂ�OK
//	�����P�F�ϊ����̃E�C���h�E�n���h��
//	�����Q�F�ۑ�����̧�ٖ�
//	�߂�l�FTRUE or FALSE
//==========================================================================================================
BOOL Hwnd2BmpFile(HWND appwnd,char* savefile)
{
	// �����`�F�b�N
	if( NULL==appwnd || savefile==NULL || strlen(savefile)< 1 )return(FALSE);

	HBITMAP happbit;
	HANDLE hdib;
	HFILE fh;

    BITMAPFILEHEADER    hdr;
    LPBITMAPINFOHEADER  lpbi;
    BITMAPINFOHEADER    bi;
    DWORD               dwSize;


	// �E�C���h�E��DDB�ޯ�ϯ�߂��쐬
	if( CreateAppBitmap( appwnd, &happbit)==FALSE) return(FALSE);

	hdib = DibFromBitmap( happbit);
	DeleteObject( happbit );

    //	DIB�̑S�T�C�Y���v�Z
    DIBInfo(hdib, &bi);
    dwSize = bi.biSize + PaletteSize(&bi) + bi.biSizeImage;

    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);

    hdr.bfType          = 0x4d42;		// 'BM'
    hdr.bfSize          = dwSize + sizeof(BITMAPFILEHEADER);
    hdr.bfReserved1     = 0;
    hdr.bfReserved2     = 0;
    hdr.bfOffBits       = (DWORD)sizeof(BITMAPFILEHEADER) + lpbi->biSize +
                          PaletteSize(lpbi);

	// ��������̧�ق��I�[�v��
	if( NULL==(fh	= _lcreat(savefile, NULL))){
	    GlobalUnlock(hdib);
		return(FALSE);
	}

	// BMP�w�b�_���ް��̏�������
    _lwrite(fh, (LPCSTR)&hdr, sizeof(BITMAPFILEHEADER));
    _hwrite(fh, (LPCSTR)lpbi, dwSize);

    GlobalUnlock(hdib);

	_lclose(fh);

	return(TRUE);
}

//===========================================================================================================
//	�֐����FHBITMAP	CreateAppBitmap(HWND hwnd)
//	�@�\�@�F�w�肳�ꂽ�E�C���h�E��DDB�ޯ�ϯ�߂��쐬����B
//	�����P�F�ϊ����̃E�C���h�E�n���h��
//	�߂�l�FTRUE or FALSE
//==========================================================================================================
BOOL	CreateAppBitmap(HWND hwnd,HBITMAP* bmp)
{
	HDC memdc;
	RECT rect;

	// �w�肳�ꂽ�E�C���h�E�Ɠ����T�C�Y���ޯ�ϯ�߂��쐬
	GetWindowRect(hwnd,&rect);
	*bmp = CreateCompatibleBitmap( GetDC(hwnd),rect.right-rect.left,rect.bottom-rect.top);
	if( *bmp==NULL ) return(FALSE);
	memdc=CreateCompatibleDC(GetDC(hwnd));
	if( memdc==NULL){
		DeleteObject(*bmp);
		return(FALSE);
	}
	*bmp = (HBITMAP)SelectObject(memdc,*bmp);

	// �f�X�N�g�b�v�̈悩��A�w�肳�ꂽ�E�C���h�E�̏ꏊ���߰����
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
	bi.biBitCount       = 8;		// ��coba���F�������W�ɂ���ƁA256�Fbmp�ɂȂ�
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
