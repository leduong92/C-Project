
#ifndef TSCREEN_WIN_H
#define TSCREEN_WIN_H
//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TScrWin.h
//	Class		:CGridView
// 				:�X�N���[���E�C���h�E�N���X
//				 �E�C���h�E�ɒ���t���ĕ`������铙�Ɏg�p�B�h���N���X�ŃO���t�N���X�����B
//				 
//	�쐬��		:2000.07.27	Coded by coba
//					���`��֐��̒P�ʌn�͑S�ăs�N�Z���ł�
//========================================================================================
//	�C��		:
//	2003.11.17 coba �g���`��֐�DrawBox()�̒ǉ�
//	2004. 4. 5 Maeda �c���������ƃ{�^�������ǉ�
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>



class TScreenWindow
{
	private:
		//--------------------------------------------------------------------------------------------------
		// �p����̃N���X�Ŏ��R�ɃJ�X�^�}�C�Y���Ă�������
		// VB���Ɍ����΃C�x���g�v���V�[�W���AMFC�ł̓��b�Z�[�W�n���h���ƌĂ΂�郄�c�ł�
		virtual BOOL EvClick( long col,long row);
		//---------------------------------------------------------------------------------------------------

	public:
		TScreenWindow(HINSTANCE inst,HWND hparent,BOOL white=FALSE );
		~TScreenWindow();

		LONG DoCommand(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam );

		void Draw(void);
		HWND m_thisWnd;
		BOOL CreateScreen(LPRECT pRect);

		// �^�~
		void DrawCircle( int x,int y,int r, int LineWidth,BOOL paint,COLORREF theColor,COLORREF interColor);

		// ��
		void DrawLine( int sx,int sy,int ex, int ey,int LineType,int LineWidth,COLORREF theColor,BOOL redraw=FALSE);

		void TextOut( int x, int y, int fontsize, char* txt ,COLORREF color,char* font="�l�r �S�V�b�N",int charset=SHIFTJIS_CHARSET);
		void TextOutV( int x, int y, int fontsize, char* txt ,COLORREF color,char* font, int charset);
		// �g�i�h��Ԃ��F�w��\�j
		void DrawBox( int sx, int sy, int ex, int ey,int LineType,int LineWidth,COLORREF theColor
																		,BOOL paint,COLORREF bkcolor);

		// ���Z�b�g�i�^�������ɂ���j
		void ResetScreen( void );

		// �v���b�g���P�s�N�Z���ł�
		void SetPlot(int x,int y,COLORREF color);

		void Redraw(void);

	protected:


		HWND m_hwndParent;

		HDC m_hdcScrWin;

		// �G�f�B�b�g�{�b�N�X��IME�ݒ�
		BOOL SetIme( int mode );

	private:
		BOOL DrawData( void );

		// �n���h���֌W
		HINSTANCE m_hInst;

		// ���X�g�쐬�ς݃t���O
		BOOL m_WindowInit;

		// �`��p̫��
		HFONT m_hFont,m_tmpFont,m_eFont;

		// ���b�Z�C�W����
		BOOL EvInit( HWND hwnd);
		BOOL EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick);

		// �X�N���[���̃T�C�Y�i�s�N�Z���ł��j
		int m_Xsize, m_Ysize;

		HBITMAP m_hbit,m_hOldbit;

		// �w�i�F
		BOOL m_bkWhite;

};
#endif
