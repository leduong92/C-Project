
#ifndef HDC2BMP_H
#define HDC2BMP_H
//===================================================================================================================
//
//	FileName	: hdc2bmp.c
//				BMP�t�@�C���̃Z�[�u
//	�쐬��		:2001.10.22 
//===================================================================================================================

#include <windows.h>
#include <winuser.h>
#include <wingdi.h>


#define WIDTHBYTES(i)   ((i+31)/32*4)


 // �A�v���P�[�V��������͂��̊֐����Ă�
BOOL	Hwnd2BmpFile(HWND appwnd,char* savefile);


BOOL	CreateAppBitmap(HWND hwnd,HBITMAP* bitmap);
// �ȉ��̓p�N��from ASCII Windows95 �v���O���~���O�R��
HANDLE	DibFromBitmap (HBITMAP hbitmap);
BOOL	DIBInfo (HANDLE hdib, LPBITMAPINFOHEADER lpbi);
WORD	PaletteSize (LPBITMAPINFOHEADER lpbi);
WORD    DIBNumColors (LPBITMAPINFOHEADER lpbi);

#endif
