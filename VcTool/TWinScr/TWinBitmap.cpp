//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TWinBitmap.cpp
//	Class		:TWinBitmap
// 				:ビットマップウインドウクラス
//	作成日		:2003.12.3	Coded by N.Maeda
//
//========================================================================================
#include "TWinBitmap.h"
#include "colordef.h"
#include "stdio.h"

//#define WM_USER_KEYDOWN WM_USER+100

// おまじない関係--------------------------------------------------------------------------
static TWinBitmap** clsArryWs=NULL;
static int clsCnt=0;
static LRESULT CALLBACK ScrWinProc(HWND, UINT, WPARAM, LPARAM);
//------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------
//	TSelectOrder::TSelectOrder()
//	タイプ：Public
//	機能　：デフォルトコンストラクタ
//	引数１：インスタンス
//	引数２：親ウインドウハンドル
//	引数３：行のブロックサイズ（この単位でメモリをアロケートする）
//	引数４：列数の初期値
//---------------------------------------------------------------------------------------------------------------------
TWinBitmap::TWinBitmap(HINSTANCE inst,HWND hparent,BOOL white)
{
	m_hInst = inst;
	m_hwndParent = hparent;

	m_WindowInit=FALSE;


	// クラス配列

	TWinBitmap** _clsArray = (TWinBitmap **)realloc(clsArryWs, sizeof(TWinBitmap *) * (clsCnt +1));
	(clsArryWs = _clsArray)[clsCnt] = this;
	clsCnt++;

	m_bkWhite = white;

	m_bitData=NULL;
}


//---------------------------------------------------------------------------------------------------------------------
//	TSelectOrder::TSelectOrder()
//	タイプ：Public
//	機能　：デストラクタ
//---------------------------------------------------------------------------------------------------------------------
TWinBitmap::~TWinBitmap(void)
{
	DestroyWindow(m_thisWnd);

	// クラス配列から削除する
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
//	タイプ：Public
//	機能　：ウインドウ作成
//	引数２：親ウインドウに対する左上座標と、ウインドウの高さと巾
//---------------------------------------------------------------------------------------------------------------------
BOOL TWinBitmap::CreateScreen(LPRECT pRect)
{

	// スクロール指定がある場合のフラグ
	DWORD winStyle;

	m_Xsize = pRect->right;
	m_Ysize = pRect->bottom;

	winStyle = 	WS_VISIBLE|WS_CHILD|WS_BORDER;

	WNDCLASS wcl;
	char className[256];

	wsprintf(className,"NokBitmapScreen");

	// 登録済みか？
	if( !GetClassInfo(m_hInst, className, &wcl) ){
		// クラスの登録
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

	//==============ウインド作成==================
	m_thisWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		"nok Fanatic class libraly scwinbitmap",
		winStyle,
		pRect->left,		pRect->top,
		pRect->right,			// スクロールの分
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
//	タイプ：private
//	機能　：ウインドウ初期化。フォント、画面等のイニシャライズ
//---------------------------------------------------------------------------------------------------------------------
BOOL TWinBitmap::EvInit(HWND hwnd)
{
	// 描画用仮想デバイスコンテキスト作成
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

//	// フォントの設定
//	m_hFont = CreateFont(	
//			10,
//			0,				// フォントサイズ
//			0,					// Escapement
//			0,					// Orientation
//			FW_NORMAL,				// Weight(フォントの太さ)
//			0,					// Ital(イタリック体)
//			0,					// Underline(下線)
//			0,					// StrikeThru(打ち消し線)
//			SHIFTJIS_CHARSET,	// Charset(文字セット)
//			OUT_DEFAULT_PRECIS,	// Precision(出力精度)
//			CLIP_DEFAULT_PRECIS,// ClipPrecision(クリッピ15ング精度)
//			DEFAULT_QUALITY,	// Quality(フォントの出力品質)
//			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(フォントのピッチとファミリ)
//				"ＭＳ ゴシック"
//	);
//
//	m_tmpFont = (HFONT)SelectObject( m_hdcScrWin , m_hFont );


	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	LONG TWinBitmap::DoCommand(HWND hDlg ,UINT message, WPARAM wParam, LPARAM lParam)
//	タイプ：Public
//	機能　：メッセージループ
//---------------------------------------------------------------------------------------------------------------------
LONG TWinBitmap::DoCommand(HWND hWind ,UINT message, WPARAM wParam, LPARAM lParam)
{

	switch(message)
	{

		// ウィンドウ再描画(仮装デバイスコンテキストの内容をコピーする)
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
//	タイプ：virtual	(private)
//	機能　：クリックされた時に呼ばれる関数
//	引数１：クリックされた列番号
//	引数２：クリックされた行番号
//	戻り値：デフォルト処理をする＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TWinBitmap::EvClick( long col,long row)
{
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL EvClickPrivate(HWND hwnd,LPARAM lparam)
//	タイプ：private
//	機能　：マウスクリック時のイベント。何処のセルがクリックされたか調べて、仮想関数を呼び出す
//---------------------------------------------------------------------------------------------------------------------
BOOL TWinBitmap::EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick)
{
	WORD xPos,yPos;
	long xColPos=0;

	SetFocus(hwnd);

	// リストの度の領域がクリックされたか調べる
	xPos = LOWORD(lparam);
	yPos = HIWORD(lparam);



	// 仮想関数呼び出し
	if(!dblClick){
		if( EvClick(xPos,yPos) ){
		}
	}
	// ダブルクリック
	else{
		//EvDblClick(xCel,currentRow);
	}
	return(TRUE);
}

// Bitmap画像関係
BOOL TWinBitmap::LoadBitMap( char* fname )
{
	FILE *fp;
	int ret;
	BITMAPFILEHEADER bHead;
	size_t dLen;
	int bitWidth;

	fp=fopen( fname , "rb" );
	if( fp==NULL ) return FALSE;

	// ヘッダーの読み込み
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

	// 読み込むデータ長
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

  // 画像が表示サイズよりあまりにも大きいとメモリーを消費するので、縮小する
	// (面倒なので、整数倍で縮小する)
	if( m_bInfo.bmiHeader.biWidth > m_Xsize*2 ){
		bai=m_bInfo.bmiHeader.biWidth/m_Xsize;  //倍率
		nx=m_bInfo.bmiHeader.biWidth/bai;		// 新しいサイズ

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
		bai=m_bInfo.bmiHeader.biHeight/m_Ysize;  //倍率
		ny=m_bInfo.bmiHeader.biHeight/bai;		// 新しいサイズ

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
//******************************************   以下はおまじないです   *******************************************
//*******************************************                        ********************************************
//********************************************                      *********************************************
//***************************************************************************************************************
//***************************************************************************************************************

// グリッドのWindowProcのインスタンス
static LRESULT CALLBACK ScrWinProc(HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam)
{
	int i;

	// ウインドウ検索
	for( i=0; i<clsCnt;i++)
		if(clsArryWs[i]->m_thisWnd == hwnd ) break;

	// クラスのメンバ関数に処理をわたす
	return( clsArryWs[i]->DoCommand(hwnd, message, wParam, lParam) );
}


