//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TWinBitmap2.cpp
//	Class		:TWinBitmap2
// 				:�r�b�g�}�b�v�E�C���h�E�N���X
//	�쐬��		:2003.12.3	Coded by N.Maeda
//
//========================================================================================
#include "TWinBitmap2.h"
#include "colordef.h"
#include "stdio.h"

//#define WM_USER_KEYDOWN WM_USER+100

// ���܂��Ȃ��֌W--------------------------------------------------------------------------
static TWinBitmap2** clsArryWs=NULL;
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
TWinBitmap2::TWinBitmap2(HINSTANCE inst,HWND hparent,BOOL white)
{
	m_hInst = inst;
	m_hwndParent = hparent;

	m_WindowInit=FALSE;


	// �N���X�z��

	TWinBitmap2** _clsArray = (TWinBitmap2 **)realloc(clsArryWs, sizeof(TWinBitmap2 *) * (clsCnt +1));
	(clsArryWs = _clsArray)[clsCnt] = this;
	clsCnt++;

	m_bkWhite = white;

	m_bitData=NULL;
	m_thisWnd=NULL;
	memset(m_clsName,0,sizeof(m_clsName));
}


//---------------------------------------------------------------------------------------------------------------------
//	TSelectOrder::TSelectOrder()
//	�^�C�v�FPublic
//	�@�\�@�F�f�X�g���N�^
//---------------------------------------------------------------------------------------------------------------------
TWinBitmap2::~TWinBitmap2(void)
{
	DestroyWindow(m_thisWnd);

	// �N���X�z�񂩂�폜����
	int		index;

	for( index=0; index< clsCnt;index++)
		if(clsArryWs[index]==this)
			break;

	memmove(clsArryWs + index, clsArryWs + index +1, sizeof(TWinBitmap2 *) * (--clsCnt - index));
	clsArryWs = (TWinBitmap2 **)realloc(clsArryWs, sizeof(TWinBitmap2 *) * clsCnt);

	if( m_bitData!=NULL) free(m_bitData);m_bitData=NULL;

}


BOOL TWinBitmap2::CreateScreen2(LPRECT pRect,char* fname)
{
//	HWND hWnd;
//	MSG msg;
	WNDCLASS winc;
	HANDLE hFile;
	DWORD dwBytes;

	int i;



	hFile = CreateFile(fname , GENERIC_READ , 0 , NULL ,
		OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL);
	if (hFile == INVALID_HANDLE_VALUE) return 1;

	ReadFile(hFile , &bmpFileHeader , sizeof (BITMAPFILEHEADER) , &dwBytes , NULL);
	if (bmpFileHeader.bfType != 0x4D42) {
		MessageBox(NULL , TEXT("This is not a bitmap file") , NULL , MB_OK);
		return 1;
	}

	ReadFile(hFile , &bmpInfo , sizeof (BITMAPINFOHEADER) , &dwBytes , NULL);
	bPixelBits = (BYTE *) malloc (bmpFileHeader.bfSize - bmpFileHeader.bfOffBits);
	ReadFile(hFile ,bPixelBits ,
		bmpFileHeader.bfSize - bmpFileHeader.bfOffBits , &dwBytes , NULL);
	CloseHandle(hFile);


	// Cls Name 
	if( strlen(m_clsName)!=0 ){
		if (!UnregisterClass(m_clsName,m_hInst)) return 1;
	}
	
	memset(m_clsName,0,sizeof(m_clsName));
	memcpy(m_clsName,fname,sizeof(m_clsName)-1<strlen(fname)?sizeof(m_clsName)-1:strlen(fname));
	for(i=0;i<sizeof(m_clsName);i++){
		if( m_clsName[i]=='\\' || m_clsName[i]==':' ) m_clsName[i]='_';
	}
	


	winc.style 			= CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc	= ScrWinProc;
	winc.cbClsExtra		= winc.cbWndExtra	= 0;
	winc.hInstance		= m_hInst;
	winc.hIcon			= LoadIcon(NULL , IDI_APPLICATION);
	winc.hCursor		= LoadCursor(NULL , IDC_ARROW);
	winc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName	= NULL;
//	winc.lpszClassName	= TEXT("BMP_CL");
	winc.lpszClassName	= TEXT(m_clsName);
	if (!RegisterClass(&winc)) return 1;


//	m_thisWnd = CreateWindow(
//			TEXT("KITTY") , TEXT("Kitty on your lap") ,
//			WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
//			CW_USEDEFAULT , CW_USEDEFAULT ,
//			CW_USEDEFAULT , CW_USEDEFAULT ,
//			NULL , NULL , m_hInst , NULL
//	);


	WNDCLASS wcl;
	char className[256];
	DWORD winStyle;
	winStyle = 	WS_VISIBLE|WS_CHILD|WS_BORDER;
	wsprintf(className,"NokBitmapScreen");

	m_Xsize = pRect->right;
	m_Ysize = pRect->bottom;


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
			wcl.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));	// Make the window background white.
		else
			wcl.hbrBackground = (HBRUSH)(GetStockObject(BLACK_BRUSH));	// Make the window background white.

		// Register the window class. 
		if(!RegisterClass(&wcl)){
			MessageBox(NULL,"RegistarClass() err","NOK Fanatic class libraly",MB_OK);
			return(FALSE) ;
		}
	}

	if( m_thisWnd==NULL ){
		m_thisWnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			className,
			"nok Fanatic class libraly scwinbitmap2",
			winStyle,
			pRect->left,
			pRect->top,
			pRect->right,			// �X�N���[���̕�
			pRect->bottom,
			m_hwndParent,
			NULL,
			m_hInst,
			NULL 
		);
		if (m_thisWnd == NULL) return 1;

		EvInit(m_thisWnd);

		ShowWindow(m_thisWnd, SW_SHOW);
	}else{
		// ���ɓo�^�ς݂̏ꍇ
//		SetWindowPos(m_thisWnd,HWND_TOP,
//			pRect->left,
//			pRect->top,
//			pRect->right,			// �X�N���[���̕�
//			pRect->bottom,
//			SWP_SHOWWINDOW);
		EvInit(m_thisWnd);
		ShowWindow(m_thisWnd, SW_SHOW);
	}

	UpdateWindow(m_thisWnd);
	m_WindowInit=TRUE;


	return TRUE;

}






//---------------------------------------------------------------------------------------------------------------------
//	BOOL	TSelectOrder::InitDialog(HWND hDlg)
//	�^�C�v�Fprivate
//	�@�\�@�F�E�C���h�E�������B�t�H���g�A��ʓ��̃C�j�V�����C�Y
//---------------------------------------------------------------------------------------------------------------------
BOOL TWinBitmap2::EvInit(HWND hwnd)
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

	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	LONG TWinBitmap2::DoCommand(HWND hDlg ,UINT message, WPARAM wParam, LPARAM lParam)
//	�^�C�v�FPublic
//	�@�\�@�F���b�Z�[�W���[�v
//---------------------------------------------------------------------------------------------------------------------
LONG TWinBitmap2::DoCommand(HWND hWind ,UINT message, WPARAM wParam, LPARAM lParam)
{

	switch(message)
	{

		// �E�B���h�E�ĕ`��(�����f�o�C�X�R���e�L�X�g�̓��e���R�s�[����)
		case WM_PAINT:
				HDC hdc;
				PAINTSTRUCT ps;
				hdc = BeginPaint(hWind , &ps);
/*
				SetDIBitsToDevice(
					hdc , 0 , 0 ,
					bmpInfo.bmiHeader.biWidth ,
					bmpInfo.bmiHeader.biHeight ,
					0 , 0 , 0 , 
					bmpInfo.bmiHeader.biHeight ,
					bPixelBits , &bmpInfo , DIB_RGB_COLORS
				);
*/
				StretchDIBits(
					hdc , 0 , 0 ,
					m_Xsize, m_Ysize ,
					0,0,
					bmpInfo.bmiHeader.biWidth ,
					bmpInfo.bmiHeader.biHeight ,
					bPixelBits , &bmpInfo , DIB_RGB_COLORS,SRCCOPY
				);


				EndPaint(hWind , &ps);		
/*
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
*/
				break;
		case WM_CREATE:
				break;

		case WM_DESTROY:
			// release the GDI objects
//				m_hbit=(HBITMAP)SelectObject(m_hdcScrWin,m_hOldbit);
//				DeleteObject( m_hbit );
//				DeleteDC(m_hdcScrWin);
//				if( m_bitData!=NULL) free(m_bitData);m_bitData=NULL;

				free(bPixelBits);
				PostQuitMessage(0);
				break;

//		case WM_LBUTTONDOWN:
//				EvClickPrivate(hWind,lParam,FALSE);
//				break;

		default:
				return DefWindowProc(hWind, message, wParam, lParam);
	}
	return(0);

}

/*
void TWinBitmap2::ResetScreen( void )
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
void TWinBitmap2::Redraw(void)
{
	RECT rect;

	SetRect( &rect, 0, 0, m_Xsize,m_Ysize);

	InvalidateRect(m_thisWnd,&rect,TRUE);
	UpdateWindow(m_thisWnd);
}


// Bitmap�摜�֌W
BOOL TWinBitmap2::LoadBitMap( char* fname )
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
	fclose( fp );

	Redraw();

	return TRUE;
}
*/

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


