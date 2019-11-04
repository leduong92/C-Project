//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TWinBitmap.cpp
//	Class		:TWinBitmap
// 				:�r�b�g�}�b�v�E�C���h�E�N���X
//	�쐬��		:2003.12.3	Coded by N.Maeda
//
//========================================================================================
#include "TWinBitmap.h"
#include "colordef.h"
#include "stdio.h"

//#define WM_USER_KEYDOWN WM_USER+100

// ���܂��Ȃ��֌W--------------------------------------------------------------------------
static TWinBitmap** clsArryWs=NULL;
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
TWinBitmap::TWinBitmap(HINSTANCE inst,HWND hparent,BOOL white)
{
	m_hInst = inst;
	m_hwndParent = hparent;

	m_WindowInit=FALSE;


	// �N���X�z��

	TWinBitmap** _clsArray = (TWinBitmap **)realloc(clsArryWs, sizeof(TWinBitmap *) * (clsCnt +1));
	(clsArryWs = _clsArray)[clsCnt] = this;
	clsCnt++;

	m_bkWhite = white;

	m_bitData=NULL;
}


//---------------------------------------------------------------------------------------------------------------------
//	TSelectOrder::TSelectOrder()
//	�^�C�v�FPublic
//	�@�\�@�F�f�X�g���N�^
//---------------------------------------------------------------------------------------------------------------------
TWinBitmap::~TWinBitmap(void)
{
	DestroyWindow(m_thisWnd);

	// �N���X�z�񂩂�폜����
	int		index;

	for( index=0; index< clsCnt;index++)
		if(clsArryWs[index]==this)
			break;

	memmove(clsArryWs + index, clsArryWs + index +1, sizeof(TWinBitmap *) * (--clsCnt - index));
	clsArryWs = (TWinBitmap **)realloc(clsArryWs, sizeof(TWinBitmap *) * clsCnt);

	if( m_bitData!=NULL) free(m_bitData);m_bitData=NULL;

}


//---------------------------------------------------------------------------------------------------------------------
//	TWinBitmap::CreateGrid(WNDPROC wproc, LPRECT pRect)
//	�^�C�v�FPublic
//	�@�\�@�F�E�C���h�E�쐬
//	�����Q�F�e�E�C���h�E�ɑ΂��鍶����W�ƁA�E�C���h�E�̍����Ƌ�
//---------------------------------------------------------------------------------------------------------------------
BOOL TWinBitmap::CreateScreen(LPRECT pRect)
{

	// �X�N���[���w�肪����ꍇ�̃t���O
	DWORD winStyle;

	m_Xsize = pRect->right;
	m_Ysize = pRect->bottom;

	winStyle = 	WS_VISIBLE|WS_CHILD|WS_BORDER;

	WNDCLASS wcl;
	char className[256];

	wsprintf(className,"NokBitmapScreen");

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
		"nok Fanatic class libraly scwinbitmap",
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


void TWinBitmap::Draw(void)
{

}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL	TSelectOrder::InitDialog(HWND hDlg)
//	�^�C�v�Fprivate
//	�@�\�@�F�E�C���h�E�������B�t�H���g�A��ʓ��̃C�j�V�����C�Y
//---------------------------------------------------------------------------------------------------------------------
BOOL TWinBitmap::EvInit(HWND hwnd)
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

//	// �t�H���g�̐ݒ�
//	m_hFont = CreateFont(	
//			10,
//			0,				// �t�H���g�T�C�Y
//			0,					// Escapement
//			0,					// Orientation
//			FW_NORMAL,				// Weight(�t�H���g�̑���)
//			0,					// Ital(�C�^���b�N��)
//			0,					// Underline(����)
//			0,					// StrikeThru(�ł�������)
//			SHIFTJIS_CHARSET,	// Charset(�����Z�b�g)
//			OUT_DEFAULT_PRECIS,	// Precision(�o�͐��x)
//			CLIP_DEFAULT_PRECIS,// ClipPrecision(�N���b�s15���O���x)
//			DEFAULT_QUALITY,	// Quality(�t�H���g�̏o�͕i��)
//			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(�t�H���g�̃s�b�`�ƃt�@�~��)
//				"�l�r �S�V�b�N"
//	);
//
//	m_tmpFont = (HFONT)SelectObject( m_hdcScrWin , m_hFont );


	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	LONG TWinBitmap::DoCommand(HWND hDlg ,UINT message, WPARAM wParam, LPARAM lParam)
//	�^�C�v�FPublic
//	�@�\�@�F���b�Z�[�W���[�v
//---------------------------------------------------------------------------------------------------------------------
LONG TWinBitmap::DoCommand(HWND hWind ,UINT message, WPARAM wParam, LPARAM lParam)
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

//				BitBlt(tmpHdc,ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right,ps.rcPaint.bottom,m_hdcScrWin,ps.rcPaint.left,ps.rcPaint.top,SRCCOPY);

				StretchDIBits( tmpHdc
						,0,0
						,ps.rcPaint.right,ps.rcPaint.bottom
						,0,0
						,m_bInfo.bmiHeader.biWidth,m_bInfo.bmiHeader.biHeight
						,m_bitData
						,&m_bInfo
						,DIB_RGB_COLORS
						,SRCCOPY);
				EndPaint(hWind,&ps);

				break;
		case WM_CREATE:
				break;

		case WM_DESTROY:
			// release the GDI objects
//			m_hFont = (HFONT)SelectObject( GetDC(hWind) , m_tmpFont );
//			DeleteObject( m_hFont );
				m_hbit=(HBITMAP)SelectObject(m_hdcScrWin,m_hOldbit);
				DeleteObject( m_hbit );
				DeleteDC(m_hdcScrWin);
				if( m_bitData!=NULL) free(m_bitData);m_bitData=NULL;
				break;

		case WM_LBUTTONDOWN:
				EvClickPrivate(hWind,lParam,FALSE);
				break;

		default:
				return DefWindowProc(hWind, message, wParam, lParam);
	}
	return(0);

}

void TWinBitmap::ResetScreen( void )
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
void TWinBitmap::Redraw(void)
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
BOOL TWinBitmap::EvClick( long col,long row)
{
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL EvClickPrivate(HWND hwnd,LPARAM lparam)
//	�^�C�v�Fprivate
//	�@�\�@�F�}�E�X�N���b�N���̃C�x���g�B�����̃Z�����N���b�N���ꂽ�����ׂāA���z�֐����Ăяo��
//---------------------------------------------------------------------------------------------------------------------
BOOL TWinBitmap::EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick)
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

// Bitmap�摜�֌W
BOOL TWinBitmap::LoadBitMap( char* fname )
{
	FILE *fp;
	int ret;
	BITMAPFILEHEADER bHead;
	size_t dLen;
	int bitWidth;

	fp=fopen( fname , "rb" );
	if( fp==NULL ) return FALSE;

	// �w�b�_�[�̓ǂݍ���
	ret=fread( (void*)&bHead , sizeof( bHead ) , 1 , fp );
	if( ret !=1 || bHead.bfType!=0x4D42 || bHead.bfOffBits!=54 ){
		fclose( fp );
		return FALSE;	
	}

	// Infomation
	BITMAPINFOHEADER aa;
	ret=fread( (void*)&aa , sizeof( aa ) , 1 , fp );
	memset( &m_bInfo , 0 , sizeof( m_bInfo ));
	m_bInfo.bmiHeader = aa;
//	ret=fread( (void*)&m_bInfo.bmiHeader , sizeof( m_bInfo.bmiHeader ) , 1 , fp );
	if( ret !=1 || m_bInfo.bmiHeader.biSize!=40 || m_bInfo.bmiHeader.biPlanes !=1 || m_bInfo.bmiHeader.biBitCount !=24 ){
		fclose( fp );
		return FALSE;	
	}
	
	// 
	if( m_bitData !=NULL ){
		free( m_bitData );
		m_bitData=NULL;
	}

	// �ǂݍ��ރf�[�^��
	bitWidth=(m_bInfo.bmiHeader.biWidth*3+3)/4*4;
	dLen=bitWidth*m_bInfo.bmiHeader.biHeight;

	m_bitData = (char*)malloc(dLen);

	// Load Bitmapdata
	ret=fread( m_bitData , dLen , 1, fp );
	if( ret !=1 ){
		fclose( fp );
		free( m_bitData );
		m_bitData=NULL;
		return FALSE;	
	}
/*
	double r,g,b;
	int bitWidthNew;
	char* m_bitDataNew;
	unsigned char *p1,*p2;
	int bai,n;
	int x,y,nx,ny;

  // �摜���\���T�C�Y��肠�܂�ɂ��傫���ƃ������[�������̂ŁA�k������
	// (�ʓ|�Ȃ̂ŁA�����{�ŏk������)
	if( m_bInfo.bmiHeader.biWidth > m_Xsize*2 ){
		bai=m_bInfo.bmiHeader.biWidth/m_Xsize;  //�{��
		nx=m_bInfo.bmiHeader.biWidth/bai;		// �V�����T�C�Y

		bitWidthNew=(nx*3+3)/4*4;

		dLen=bitWidthNew*m_bInfo.bmiHeader.biHeight;
		m_bitDataNew = (char*)malloc(dLen);
		memset( m_bitDataNew , 0 , dLen );

		for( y=0;y<m_bInfo.bmiHeader.biHeight;y++){
			p1=(unsigned char*)&m_bitData[bitWidth*y];
			p2=(unsigned char*)&m_bitDataNew[bitWidthNew*y];
			for( x=0;x<nx;x++){
				r=0;
				g=0;
				b=0;
				for( n=0;n<bai;n++){
					r+=*p1++;
					g+=*p1++;
					b+=*p1++;
				}
				*p2++=(unsigned char)(r/n);
				*p2++=(unsigned char)(g/n);
				*p2++=(unsigned char)(b/n);
			}
		}
		free( m_bitData );
		m_bitData=m_bitDataNew;
		m_bInfo.bmiHeader.biWidth=nx;
	}
	// 
	if( m_bInfo.bmiHeader.biHeight > m_Ysize*2 ){
		bai=m_bInfo.bmiHeader.biHeight/m_Ysize;  //�{��
		ny=m_bInfo.bmiHeader.biHeight/bai;		// �V�����T�C�Y

		bitWidth=(m_bInfo.bmiHeader.biWidth*3+3)/4*4;

		dLen=bitWidth*ny;
		m_bitDataNew = (char*)malloc(dLen);
		memset( m_bitDataNew , 0 , dLen );

		for( x=0;x<m_bInfo.bmiHeader.biWidth;x++){
			p1=(unsigned char*)&m_bitData[x*3];
			for( y=0;y<ny;y++){
				r=0;
				g=0;
				b=0;
				p2=(unsigned char*)&m_bitDataNew[bitWidthNew*y+x*3];
				for( n=0;n<bai;n++){
					r+=p1[0];
					g+=p1[1];
					b+=p1[2];
					p1+=bitWidth;
				}
				*p2++=(unsigned char)(r/n);
				*p2++=(unsigned char)(g/n);
				*p2++=(unsigned char)(b/n);
			}
		}
		free( m_bitData );
		m_bitData=m_bitDataNew;
		m_bInfo.bmiHeader.biHeight=ny;
	}
*/
	


	fclose( fp );

//	UpdateWindow(m_thisWnd);
	Redraw();

	return TRUE;
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


