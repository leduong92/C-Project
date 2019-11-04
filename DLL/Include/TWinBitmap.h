#ifndef TWIN_BITMAP_H
#define TWIN_BITMAP_H
//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TWinBitmap.h
//	Class		:TWinBitmap
// 				:�r�b�g�}�b�v�E�C���h�E�N���X
//				 
//				 
//	�쐬��		:2003.12.3	Coded by N.Maeda
//	�C��		:
//					���`��֐��̒P�ʌn�͑S�ăs�N�Z���ł�
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>



class TWinBitmap
{
	private:
		//--------------------------------------------------------------------------------------------------
		// �p����̃N���X�Ŏ��R�ɃJ�X�^�}�C�Y���Ă�������
		// VB���Ɍ����΃C�x���g�v���V�[�W���AMFC�ł̓��b�Z�[�W�n���h���ƌĂ΂�郄�c�ł�
		virtual BOOL EvClick( long col,long row);
		//---------------------------------------------------------------------------------------------------

	public:
		TWinBitmap(HINSTANCE inst,HWND hparent,BOOL white=FALSE );
		~TWinBitmap();

		LONG DoCommand(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam );

		void Draw(void);
		HWND m_thisWnd;
		BOOL CreateScreen(LPRECT pRect);

		// ���Z�b�g�i�^�������ɂ���j
		void ResetScreen( void );

		void Redraw(void);

		// Bitmap�摜�֌W
		BOOL LoadBitMap( char* fname );

	protected:


		HWND m_hwndParent;

		HDC m_hdcScrWin;

	private:
		BOOL DrawData( void );

		// �n���h���֌W
		HINSTANCE m_hInst;

		// ���X�g�쐬�ς݃t���O
		BOOL m_WindowInit;

//		// �`��p̫��
//		HFONT m_hFont,m_tmpFont,m_eFont;

		// ���b�Z�C�W����
		BOOL EvInit( HWND hwnd);
		BOOL EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick);

		// �X�N���[���̃T�C�Y�i�s�N�Z���ł��j
		int m_Xsize, m_Ysize;

		HBITMAP m_hbit,m_hOldbit;

		// �w�i�F
		BOOL m_bkWhite;

		// �摜�t�@�C��
		BITMAPINFO m_bInfo;
		char* m_bitData;
};
#endif
