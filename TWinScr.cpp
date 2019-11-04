//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TScrWin.h
//	Class		:TScreenWindow
// 				:�O���b�h�E�C���h�E�E�r���[�N���X
//	�쐬��		:1999.07.30	Coded by coba
//               2004. 4 .5 �c�����ƁA�N���b�N�����ǉ�(�O�c)
//========================================================================================
#include "twinscr.h"
#include "colordef.h"
#include "stdio.h"

#define WM_USER_KEYDOWN WM_USER+100

// ���܂��Ȃ��֌W--------------------------------------------------------------------------
static TScreenWindow** clsArryWs=NULL;
static int clsCnt=0;
static LRESULT CALLBACK ScrWinProc(HWND, UINT, WPARAM, LPARAM);
//------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------------------------
//	TSelectOrder::TSelectOrder()
//	�^�C�v�FPublic
//	�@�\�@�F�f�t�H���g�R���X�g���N�^
//	�����P�F�C���X�^���X
//	�����Q�F�e�E�C���h�E�n���h��
//	�����R�F�s�̃u���b�N�T�C�Y�i���̒P�ʂŃ��������A���P�[�g����j
//	�����S�F�񐔂̏����l
//---------------------------------------------------------------------------------------------------------------------
TScreenWindow::TScreenWindow(HINSTANCE inst,HWND hparent,BOOL white)
{
	m_hInst = inst;
	m_hwndParent = hparent;

	m_WindowInit=FALSE;


	// �N���X�z��

	TScreenWindow** _clsArray = (TScreenWindow **)realloc(clsArryWs, sizeof(TScreenWindow *) * (clsCnt +1));
	(clsArryWs = _clsArray)[clsCnt] = this;
	clsCnt++;

	m_bkWhite = white;
}


//---------------------------------------------------------------------------------------------------------------------
//	TSelectOrder::TSelectOrder()
//	�^�C�v�FPublic
//	�@�\�@�F�f�X�g���N�^
//---------------------------------------------------------------------------------------------------------------------
TScreenWindow::~TScreenWindow(void)
{
	DestroyWindow(m_thisWnd);

	// �N���X�z�񂩂�폜����
	int		index;

	for( index=0; index< clsCnt;index++)
		if(clsArryWs[index]==this)
			break;

	memmove(clsArryWs + index, clsArryWs + index +1, sizeof(TScreenWindow *) * (--clsCnt - index));
	clsArryWs = (TScreenWindow **)realloc(clsArryWs, sizeof(TScreenWindow *) * clsCnt);

}


//---------------------------------------------------------------------------------------------------------------------
//	TScreenWindow::CreateGrid(WNDPROC wproc, LPRECT pRect)
//	�^�C�v�FPublic
//	�@�\�@�F�E�C���h�E�쐬
//	�����Q�F�e�E�C���h�E�ɑ΂��鍶����W�ƁA�E�C���h�E�̍����Ƌ�
//---------------------------------------------------------------------------------------------------------------------
BOOL TScreenWindow::CreateScreen(LPRECT pRect)
{

	// �X�N���[���w�肪����ꍇ�̃t���O
	DWORD winStyle;

	m_Xsize = pRect->right;
	m_Ysize = pRect->bottom;

	// WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|WS_HSCROLL,
	winStyle = 	WS_VISIBLE|WS_CHILD|WS_BORDER;

	WNDCLASS wcl;
	char className[20];
	wsprintf(className,"cobascreen");

	// �o�^�ς݂��H
	if( !GetClassInfo(m_hInst, className, &wcl) ){
		// �N���X�̓o�^
		memset(&wcl, 0, sizeof(WNDCLASS) );
		wcl.hInstance = m_hInst;
		wcl.lpszClassName =className;
		wcl.lpfnWndProc = ScrWinProc;
		wcl.style = (CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW | CS_DBLCLKS | CS_GLOBALCLASS | CS_OWNDC);
		wcl.hIcon = NULL;
		wcl.hCursor =  LoadCursor(NULL, IDC_ARROW);
		wcl.lpszMenuName = 0;
		wcl.cbClsExtra = 0; 
		wcl.cbWndExtra = 0;
		if( m_bkWhite)
			wcl.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));	/* Make the window background white. */
		else
			wcl.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));	/* Make the window background white. */

		/* Register the window class. */
		if(!RegisterClass(&wcl)){
			MessageBox(NULL,"RegistarClass() err","NOK Fanatic class libraly",MB_OK);
			return(FALSE) ;
		}
	}

	//==============�E�C���h�쐬==================
	m_thisWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		"nok Fanatic class libraly scwin",
		winStyle,
		pRect->left,		pRect->top,
		pRect->right,			// �X�N���[���̕�
		pRect->bottom,
		m_hwndParent,
		NULL,
		m_hInst,
		NULL 
	);

	EvInit(m_thisWnd);

	ShowWindow(m_thisWnd, SW_SHOW);
	UpdateWindow(m_thisWnd);
	 m_WindowInit=TRUE;


	return(TRUE);
}


void TScreenWindow::Draw(void)
{

}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL	TSelectOrder::InitDialog(HWND hDlg)
//	�^�C�v�Fprivate
//	�@�\�@�F�E�C���h�E�������B�t�H���g�A��ʓ��̃C�j�V�����C�Y
//---------------------------------------------------------------------------------------------------------------------
BOOL TScreenWindow::EvInit(HWND hwnd)
{
	// �`��p���z�f�o�C�X�R���e�L�X�g�쐬
	HDC tmpHdc;

	tmpHdc=GetDC(hwnd);
	SelectObject( tmpHdc,GetStockObject(WHITE_BRUSH));
	m_hdcScrWin=CreateCompatibleDC(tmpHdc);
	m_hbit=CreateCompatibleBitmap(tmpHdc, m_Xsize, m_Ysize);
	if( m_bkWhite)
		SelectObject( m_hdcScrWin,GetStockObject(WHITE_BRUSH));
	else
		SelectObject( m_hdcScrWin,GetStockObject(BLACK_BRUSH));
	m_hOldbit=(HBITMAP)SelectObject(m_hdcScrWin,m_hbit);
	PatBlt(m_hdcScrWin,0,0,m_Xsize,m_Ysize,PATCOPY);
	ReleaseDC( hwnd,tmpHdc );

	// �t�H���g�̐ݒ�
	m_hFont = CreateFont(	
			10,
			0,				// �t�H���g�T�C�Y
			0,					// Escapement
			0,					// Orientation
			FW_NORMAL,				// Weight(�t�H���g�̑���)
			0,					// Ital(�C�^���b�N��)
			0,					// Underline(����)
			0,					// StrikeThru(�ł�������)
			SHIFTJIS_CHARSET,	// Charset(�����Z�b�g)
			OUT_DEFAULT_PRECIS,	// Precision(�o�͐��x)
			CLIP_DEFAULT_PRECIS,// ClipPrecision(�N���b�s15���O���x)
			DEFAULT_QUALITY,	// Quality(�t�H���g�̏o�͕i��)
			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(�t�H���g�̃s�b�`�ƃt�@�~��)
 				"�l�r �S�V�b�N"
	);

	m_tmpFont = (HFONT)SelectObject( m_hdcScrWin , m_hFont );


	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	LONG TScreenWindow::DoCommand(HWND hDlg ,UINT message, WPARAM wParam, LPARAM lParam)
//	�^�C�v�FPublic
//	�@�\�@�F���b�Z�[�W���[�v
//---------------------------------------------------------------------------------------------------------------------
LONG TScreenWindow::DoCommand(HWND hWind ,UINT message, WPARAM wParam, LPARAM lParam)
{

	switch(message)
	{

		// �E�B���h�E�ĕ`��(�����f�o�C�X�R���e�L�X�g�̓��e���R�s�[����)
		case WM_PAINT:
				HDC tmpHdc;
			PAINTSTRUCT ps;
			ps.fErase=FALSE;
			tmpHdc=BeginPaint(hWind,&ps);
			ps.fErase=FALSE;

			BitBlt(tmpHdc,ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right,ps.rcPaint.bottom,m_hdcScrWin,ps.rcPaint.left,ps.rcPaint.top,
					SRCCOPY);
			EndPaint(hWind,&ps);
			break;

		case WM_CREATE:
			break;

		case WM_DESTROY:
			// release the GDI objects
			m_hFont = (HFONT)SelectObject( GetDC(hWind) , m_tmpFont );
			DeleteObject( m_hFont );
			m_hbit=(HBITMAP)SelectObject(m_hdcScrWin,m_hOldbit);
			DeleteObject( m_hbit );
			DeleteDC(m_hdcScrWin);
			break;

		case WM_LBUTTONDOWN:
			EvClickPrivate(hWind,lParam,FALSE);
			break;

		default:

			return DefWindowProc(hWind, message, wParam, lParam);
	}
	return(0);

}

//---------------------------------------------------------------------------------------------------------------------
//	void TScreenWindow::DrawLine( int sx,int sy,int ex, int ey,int LineType ,int LineWidth)
//	�^�C�v�Fpublic
//	�@�\�@�F��������
//	Input:	int sx;			�J�n�w���W(0
//			int sy;			�J�n�x���W(0.1mm)
//			int ex;
//			int ey;
//			int LineType ���̎��
//					0:����
//					1:�j��
//					2:�_��
//			int LineType ���̂͂΁i���H�w�莞�̂ݗL���j
//---------------------------------------------------------------------------------------------------------------------
void TScreenWindow::DrawLine( int sx,int sy,int ex, int ey,int LineType ,int LineWidth,COLORREF theColor,BOOL redraw)
{
	HPEN	hOldpen,hPen;				// Handle of pen
	int theStyle,theWidth;

	switch( LineType ){
		case 1:						// �j��
			theStyle = PS_DASH;
			theWidth = 1;
			break;
		case 2:						// �_��
			theStyle = PS_DOT;
			theWidth = 1;
			break;

		default:
			theStyle = PS_SOLID;
			theWidth = LineWidth;
			break;
	}


	hPen   = CreatePen( theStyle ,theWidth, theColor);
	hOldpen   = (HPEN)SelectObject(m_hdcScrWin, hPen);

	SetBkColor(m_hdcScrWin, CLR_BLACK );

	// ��������
	MoveToEx( m_hdcScrWin , sx , sy , NULL );
	LineTo(   m_hdcScrWin , ex , ey );


	// GDI���ǂ�
	hPen   = (HPEN)SelectObject( m_hdcScrWin,hOldpen);
	DeleteObject( hPen );

	if(redraw){
		RECT rect;
		SetRect(&rect, sx < ex ? sx-5:ex-5, sy<ey ? sy-5:ey-5, sx<ex ? ex+5:sx+5, sy< ey ? ey+5:sy+5);
		InvalidateRect(m_thisWnd,&rect,TRUE);
		UpdateWindow(m_thisWnd);
	}
}

void TScreenWindow::DrawCircle( int x,int y,int r, int LineWidth,BOOL paint,COLORREF theColor,COLORREF interColor)
{

	HPEN	hOldpen,hPen;				// Handle of pen
	HBRUSH hbrush,hold;

	hPen   = CreatePen( PS_SOLID ,LineWidth, theColor);
	hOldpen   = (HPEN)SelectObject(m_hdcScrWin,hPen);
	hbrush = CreateSolidBrush(interColor);
	hold   = (HBRUSH)SelectObject(m_hdcScrWin,hbrush);

	// �h��Ԃ�
	if( paint ){
		Ellipse( m_hdcScrWin , x-r ,y-r,x+r,y+r);
	}
	// ����́�
	else{
		Arc( m_hdcScrWin , x-r ,y-r,x+r,y+r,x,y-r,x,y-r);
	}
	hPen   = (HPEN)SelectObject(m_hdcScrWin,hOldpen);
	hbrush   = (HBRUSH)SelectObject(m_hdcScrWin,hold);
	DeleteObject( hPen );
	DeleteObject( hbrush );

		RECT rect;
		SetRect(&rect, x-r, y-r,x+r,y+r);
		InvalidateRect(m_thisWnd,&rect,TRUE);
		UpdateWindow(m_thisWnd);
}

//============================================================================
//		�T�v : �������
//		Input:	int x;			�w���W
//				int y;			�x���W
//				int nFont;		�t�H���g�̔ԍ��f�t�H���g�͂O
//				int nBold:		���̑����iCreateFont�̒萔�j
//				int nPoint1;	�t�H���g�̃T�C�Y(X)
//				int nPoint2;	�t�H���g�̃T�C�Y(Y)
//				LPSTR Message;	������
//		Return:
//============================================================================
void TScreenWindow::TextOut( int x, int y, int fontsize, char* txt ,COLORREF color,char* font, int charset)
{
	if( NULL==txt ) return;

	HFONT	holdf,hnewf;

	// �t�H���g�̐ݒ�
	hnewf = CreateFont(	
			fontsize,
			0,				// �t�H���g�T�C�Y
			0,					// Escapement
			0,					// Orientation
			FW_NORMAL,				// Weight(�t�H���g�̑���)
			0,					// Ital(�C�^���b�N��)
			0,					// Underline(����)
			0,					// StrikeThru(�ł�������)
			charset,//SHIFTJIS_CHARSET,	// Charset(�����Z�b�g)
			OUT_DEFAULT_PRECIS,	// Precision(�o�͐��x)
			CLIP_DEFAULT_PRECIS,// ClipPrecision(�N���b�s15���O���x)
			DEFAULT_QUALITY,	// Quality(�t�H���g�̏o�͕i��)
			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(�t�H���g�̃s�b�`�ƃt�@�~��)
 			font
	);

	holdf = (HFONT)SelectObject(m_hdcScrWin , hnewf );	// �t�H���g�̐؂�ւ�

	// �t�H���g�F
	SetTextColor( m_hdcScrWin,color);
//	oldBackCol = SetBkColor(hdcPrint,colBack );
	if( m_bkWhite)
		SetBkColor( m_hdcScrWin,CLR_WHITE );
	else
		SetBkColor( m_hdcScrWin,CLR_BLACK );

	::TextOut(m_hdcScrWin,x,y,txt,strlen(txt));

	hnewf = (HFONT)SelectObject( m_hdcScrWin , holdf );
	DeleteObject( hnewf );

}

void TScreenWindow::SetPlot(int x,int y,COLORREF color)
{
	SetPixelV(m_hdcScrWin, x, y, color );
}
//============================================================================
//		�T�v : �������
//		Input:	int x;			�w���W
//				int y;			�x���W
//				int nFont;		�t�H���g�̔ԍ��f�t�H���g�͂O
//				int nBold:		���̑����iCreateFont�̒萔�j
//				int nPoint1;	�t�H���g�̃T�C�Y(X)
//				int nPoint2;	�t�H���g�̃T�C�Y(Y)
//				LPSTR Message;	������
//		Return:
//============================================================================
void TScreenWindow::TextOutV( int x, int y, int fontsize, char* txt ,COLORREF color,char* font, int charset)
{
	if( NULL==txt ) return;

	HFONT	holdf,hnewf;

	// �t�H���g�̐ݒ�
	hnewf = CreateFont(	
			fontsize,
			0,				// �t�H���g�T�C�Y
			900,					// Escapement
			900,					// Orientation
			FW_NORMAL,				// Weight(�t�H���g�̑���)
			0,					// Ital(�C�^���b�N��)
			0,					// Underline(����)
			0,					// StrikeThru(�ł�������)
			charset,//SHIFTJIS_CHARSET,	// Charset(�����Z�b�g)
			OUT_DEFAULT_PRECIS,	// Precision(�o�͐��x)
			CLIP_DEFAULT_PRECIS,// ClipPrecision(�N���b�s15���O���x)
			DEFAULT_QUALITY,	// Quality(�t�H���g�̏o�͕i��)
			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(�t�H���g�̃s�b�`�ƃt�@�~��)
 			font
	);

	holdf = (HFONT)SelectObject(m_hdcScrWin , hnewf );	// �t�H���g�̐؂�ւ�

	// �t�H���g�F
	SetTextColor( m_hdcScrWin,color);
//	oldBackCol = SetBkColor(hdcPrint,colBack );
	if( m_bkWhite)
		SetBkColor( m_hdcScrWin,CLR_WHITE );
	else
		SetBkColor( m_hdcScrWin,CLR_BLACK );

	::TextOut(m_hdcScrWin,x,y,txt,strlen(txt));

	hnewf = (HFONT)SelectObject( m_hdcScrWin , holdf );
	DeleteObject( hnewf );

}

void TScreenWindow::DrawBox( int sx, int sy, int ex, int ey,int LineType,int LineWidth,COLORREF theColor,BOOL paint,COLORREF interColor)
{
	RECT rect;
	HPEN	hOldpen,hPen;				// Handle of pen
	HBRUSH hbrush,hold;
	int theStyle,theWidth;

	switch( LineType ){
		case 1:						// �j��
			theStyle = PS_DASH;
			theWidth = 1;
			break;
		case 2:						// �_��
			theStyle = PS_DOT;
			theWidth = 1;
			break;

		default:
			theStyle = PS_SOLID;
			theWidth = LineWidth;
			break;
	}

	// �h��Ԃ�
	if(paint){
		SetRect( &rect, sx, sy, sx+ex,sy+ey);
		hbrush = CreateSolidBrush(interColor);
		hold   = (HBRUSH)SelectObject(m_hdcScrWin,hbrush);
		FillRect(m_hdcScrWin,&rect,hbrush);
		hbrush   = (HBRUSH)SelectObject(m_hdcScrWin,hold);
		DeleteObject( hbrush );
	}

	hPen   = CreatePen( theStyle ,theWidth, theColor);
	hOldpen   = (HPEN)SelectObject(m_hdcScrWin,hPen);
	MoveToEx( m_hdcScrWin , sx , sy , NULL );
	LineTo(   m_hdcScrWin , sx+ex , sy );
	LineTo(   m_hdcScrWin , sx+ex , sy+ey );
	LineTo(   m_hdcScrWin , sx , ey+sy );
	LineTo(   m_hdcScrWin , sx , sy );
	hPen   = (HPEN)SelectObject(m_hdcScrWin,hOldpen);
	DeleteObject( hPen );





}

void TScreenWindow::ResetScreen( void )
{
	RECT rect;
	if( m_bkWhite)
		PatBlt(m_hdcScrWin,0,0, m_Xsize, m_Ysize,WHITENESS);
	else
		PatBlt(m_hdcScrWin,0,0, m_Xsize, m_Ysize,BLACKNESS);
	SetRect( &rect, 0, 0, m_Xsize,m_Ysize);

	InvalidateRect(m_thisWnd,&rect,TRUE);
	UpdateWindow(m_thisWnd);
}
void TScreenWindow::Redraw(void)
{
	RECT rect;

	SetRect( &rect, 0, 0, m_Xsize,m_Ysize);

	InvalidateRect(m_thisWnd,&rect,TRUE);
	UpdateWindow(m_thisWnd);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvClick( long col,long row)
//	�^�C�v�Fvirtual	(private)
//	�@�\�@�F�N���b�N���ꂽ���ɌĂ΂��֐�
//	�����P�F�N���b�N���ꂽ��ԍ�
//	�����Q�F�N���b�N���ꂽ�s�ԍ�
//	�߂�l�F�f�t�H���g���������遁TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TScreenWindow::EvClick( long col,long row)
{
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL EvClickPrivate(HWND hwnd,LPARAM lparam)
//	�^�C�v�Fprivate
//	�@�\�@�F�}�E�X�N���b�N���̃C�x���g�B�����̃Z�����N���b�N���ꂽ�����ׂāA���z�֐����Ăяo��
//---------------------------------------------------------------------------------------------------------------------
BOOL TScreenWindow::EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick)
{
	WORD xPos,yPos;
	long xColPos=0;

	SetFocus(hwnd);

	// ���X�g�̓x�̗̈悪�N���b�N���ꂽ�����ׂ�
	xPos = LOWORD(lparam);
	yPos = HIWORD(lparam);



	// ���z�֐��Ăяo��
	if(!dblClick){
		if( EvClick(xPos,yPos) ){
		}
	}
	// �_�u���N���b�N
	else{
		//EvDblClick(xCel,currentRow);
	}
	return(TRUE);
}


//***************************************************************************************************************
//***************************************************************************************************************
//********************************************                      *********************************************
//*******************************************                        ********************************************
//******************************************   �ȉ��͂��܂��Ȃ��ł�   *******************************************
//*******************************************                        ********************************************
//********************************************                      *********************************************
//***************************************************************************************************************
//***************************************************************************************************************

// �O���b�h��WindowProc�̃C���X�^���X
static LRESULT CALLBACK ScrWinProc(HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam)
{
	int i;

	// �E�C���h�E����
	for( i=0; i<clsCnt;i++)
		if(clsArryWs[i]->m_thisWnd == hwnd ) break;

	// �N���X�̃����o�֐��ɏ������킽��
	return( clsArryWs[i]->DoCommand(hwnd, message, wParam, lParam) );
}


