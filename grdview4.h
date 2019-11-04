#ifndef GRIDVIEW_H
#define GRIDVIEW_H
//========================================================================================
//
//	FileName	:GridView.h
//	Class		:CGridView
// 				:�O���b�h�E�C���h�E�E�r���[�N���Xver3
//	�쐬��		:2000.11.30	Coded by coba
//========================================================================================
//	�C��
//	2001.01.10 coba	bitmap�̉�����Y��̃o�O���
//	2002.06.12 coba	�t�H���g�w��@�\�ǉ�(CreateGrid());
//					�F���A�E�C���h�E�Y���g�p���Ă���F���g���悤�ɂ���
//	2002.06.18 TAKE �w�b�_�O���b�h�N���b�N�ʒm�֐��ǉ�
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>
#include <imm.h>
#include "grddata4.h"


// �O���b�h�ݒ�p�̍\����
typedef struct{
	int fontsize;					// �t�H���g�T�C�Y
	BOOL rowtitle;					// �s���o���g�p�E�s�g�p
	int  rowtitleWidth;				// �s���o����
	BOOL hScroll;					// �����X�N���[���L��
	BOOL vScroll;
	int selecttype;					// �I������@�O���Z���P�ʁA�P���s�P��
	long fixRows;					// �Œ�s��
	BOOL readonly;					// �G�f�B�b�g�{�b�N�X�̎g�p�E�s�g�p
} GRIDINFO;

// �Z�����ɂ����镶���̈ʒu
#define POS_LEFT	0
#define POS_CENTER	1
#define POS_RIGHT	2

// �㉺���E
#define MV_UP 0
#define MV_DOWN 1
#define MV_LEFT 2
#define MV_RIGHT 3

// EndEdit()���̈ړ���w��萔
#define NOT_MOVE 0
#define MOVE_WITH_EDIT 1
#define MOVE_CELONLY 2



class CGridViewCtrl : public TGridData
{
	private:
		//--------------------------------------------------------------------------------------------------
		// �p����̃N���X�Ŏ��R�ɃJ�X�^�}�C�Y���Ă�������
		// VB���Ɍ����΃C�x���g�v���V�[�W���AMFC�ł̓��b�Z�[�W�n���h���ƌĂ΂�郄�c�ł�
		virtual BOOL EvEndEdit( long col, long row ,char *txt ,short *move ,long *nextCol,long *nextRow,char *color);
		virtual BOOL EvClick( long col,long row);
		virtual BOOL EvDblClick( long col,long row);
		virtual BOOL EvStartEdit( long col, long row );
		virtual BOOL EvKeyDown( WPARAM keycode );
		virtual BOOL EvMoveCelEnd( long col, long row);
		virtual BOOL EvEditBoxMove( long col, long row);
		virtual BOOL EvMove( long col, long row){return(TRUE);};

		virtual BOOL EvHeaderClick( long col );	// 2002.06.17 TAKE

		virtual void EvHScrollEnd(HWND hWnd,WORD event,WORD lastpos,long left); 
					// 2003.06.18 Y.Kogai �����X�N���[���o�[�̈ړ��I����ɁA���[��Col�ԍ���Ԃ��܂��B

		//---------------------------------------------------------------------------------------------------

		// �Z���R�s�[�o�b�t�@
		typedef struct{
			int  col;
			int row;
			long tag;
			char data[100+1];
			COLORREF ncolor;
			COLORREF nbkcolor;
		} CELLCOPYBUF;
		CELLCOPYBUF m_CellBuf;

	public:
		CGridViewCtrl(HINSTANCE inst,HWND hparent,long allocunit, long totalcol);
		~CGridViewCtrl();
		LONG DoCommand(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam );
		LONG EditProc(HWND hwnd, UINT message,WPARAM wParam, LPARAM lParam );
		LONG KeyProc( int nCode,WPARAM wParam, LPARAM lParam );

		BOOL DrawHeadder(BOOL redraw);

		BOOL SetCel(int col,int row, char* data,long cTag,BOOL draw,char color,char bkcolor);
		BOOL SetCelEx(int col,int row, char* data,long cTag,BOOL draw,COLORREF color,COLORREF bkcolor);
		BOOL SetCel(int col,int row, char* data,BOOL draw);

		void Draw(void);
		void ShowGrid(int);

		void GetActiveCel( long *col, long *row ){
			*col = m_ActCel.col;
			*row = m_ActCel.row;
		};

		BOOL MoveCel(int mbMode);
		BOOL SetLeftX(long newX);


		BOOL ColsCopy(long targetCol);
		BOOL ColsPaste(long targetCol,BOOL insert,BOOL redraw);
		BOOL ColsCut(long targetCol ,BOOL redraw);

		BOOL AddNewCol(long targetCol,long width,short pos,BOOL redraw);

		BOOL DataClear( BOOL redraw );

		HWND m_ListWnd;
		HWND m_EditWnd;

		// �s���o���̕ύX
		BOOL ChangeRowTitle(long rowNo, char* newtitle);
		BOOL ChangeRowTitle2(long rowNo, char* newtitle);	//2003.05.11 Y.Kogai

		BOOL CellCopy(void);
		BOOL CellPaste(int chkmode);


		// Cell �̏�Ԏ擾(�O�c�ǉ�)
		LONG GetCellWidth( long col );
		void SetCellWidth( long col,long width );

		// ���[��col�ʒu��ύX���� //2003.06.18 Y.Kogai
		void SetLeftCell( long col );

		// ���[��col�ʒu���擾���� //2003.06.19 Y.Kogai
		void GetLeftCell( long *col );

		// ��[��col�ʒu��ύX���� // 2004.05.31 N.Maeda
		void SetTopCell( long row );
		// ��[��col�ʒu���擾���� // 2004.05.31 N.Maeda
		void GetTopCell( long *row );


	protected:
		BOOL CreateGrid(LPRECT pRect,GRIDINFO *gr,char* fontname=NULL,DWORD charset=0);
		BOOL SetColumnHeadder( long col, char *titlestr,long width,short position);
		BOOL EndEdit(int action);

		HWND m_hwndParent;
		// �A�N�e�B�u�Z���̈ړ�
		BOOL MoveActiveCel(long newCol,long newRow, BOOL internal=FALSE);
		// �Z���w��O���b�h�`��
		BOOL DrawCel(int col, int row,BOOL act =FALSE);



		// �G�f�B�b�g�{�b�N�X��IME�ݒ�
		BOOL SetIme( int mode );
	private:
		BOOL DrawData( void );

		// �n���h���֌W
		HINSTANCE m_hInst;
		HHOOK m_KeyHook;
		HBITMAP m_hBit,m_hOldBit;

		// ���X�g�E�C���h�E�̃N���C�A���g�̈�
		RECT m_ListRect;

		// ���X�g�쐬�ς݃t���O
		BOOL m_WindowInit;

		// �O���b�hinfo
		GRIDINFO m_info;

		// �J�����g�Z��
		struct ACT{
			long col;
			long row;
		}m_ActCel;

		// �\���T�C�Y�֌W
		short m_DispRows;			// �\���\�ȍs��
		short m_ColHeight;			// 1�s������̍���

		// �\�����̈ʒu�ۑ�
		long m_Top;					// �\�����̍ŏ㕔�̍s�ԍ�
		long m_Bottom;				// �\�����̍ŉ����̍s�ԍ�
		long m_Left;				// �\�����̈�ԍ��̗�ԍ�
		long m_Right;				// 			�� �E�̗�ԍ�
		long m_FixRows;				// �Œ�s�i�c�X�N���[���Ȃ��j�̍s��


		// �`��p̫��
		HFONT m_hFont,m_tmpFont,m_eFont;
		HDC m_hdcList;

		// ���͗p�̃G�f�B�b�g�R���g���[��
		LONG m_oldEditProc,m_oldButtonProc;

		// ���b�Z�C�W����
		BOOL EvInit( HWND hwnd);
		BOOL EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick);
		void EvVScroll(HWND hWnd,WORD event,WORD lastpos);
		void EvHScroll(HWND hWnd,WORD event,WORD lastpos);

		// �Z���̕ҏW
		DWORD m_editX,m_editY;					// �ҏW���̉�ʏ�̈ʒu
		BOOL m_EditMode;						// �ҏW��=TRUE,
		BOOL StartEdit(DWORD x,DWORD y,char asc=NULL,BOOL first=FALSE);
		BOOL MoveEditBox( long col, long row);


		// �t�H�[�J�X���������A�N�e�B�u�Z���̕`��
		BOOL LostFocusCel(int col,int row);

		// �X�N���[���o�[
		BOOL SetVScroll(void);
		BOOL SetHScroll(void);
		BOOL AutoScroll(long newCol,long newRow);

		// ��̋Џ��
		typedef struct{
			long width;
			short position;
		} COLUMNPOSITIONINFO;

		COLUMNPOSITIONINFO *m_ColInfo;

		// �����ꂽ�L�[�R�[�h�����l�֘A�����ׂ�
		BOOL IsKeyNumeric( char* theAsci, UINT key, UINT scanCd);

		BOOL GetImeStatus(void);

		// �t�H���g 2002.06.12 coba
		char m_FontName[100+1];
		DWORD m_CharSet;

	// 2002.10 �ǉ�
	public:
		BOOL PushActiveCell( void );
		BOOL PopActiveCell( void );
	private:
		long pushx,pushy;

};
#endif
