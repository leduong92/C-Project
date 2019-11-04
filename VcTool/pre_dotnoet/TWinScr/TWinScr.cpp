//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TScrWin.h
//	Class		:TScreenWindow
// 				:グリッドウインドウ・ビュークラス
//	作成日		:1999.07.30	Coded by coba
//               2004. 4 .5 縦書きと、クリック処理追加(前田)
//========================================================================================
#include "twinscr.h"
#include "colordef.h"
#include "stdio.h"

#define WM_USER_KEYDOWN WM_USER+100

// おまじない関係--------------------------------------------------------------------------
static TScreenWindow** clsArryWs=NULL;
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
TScreenWindow::TScreenWindow(HINSTANCE inst,HWND hparent,BOOL white)
{
	m_hInst = inst;
	m_hwndParent = hparent;

	m_WindowInit=FALSE;


	// クラス配列

	TScreenWindow** _clsArray = (TScreenWindow **)realloc(clsArryWs, sizeof(TScreenWindow *) * (clsCnt +1));
	(clsArryWs = _clsArray)[clsCnt] = this;
	clsCnt++;

	m_bkWhite = white;
}


//---------------------------------------------------------------------------------------------------------------------
//	TSelectOrder::TSelectOrder()
//	タイプ：Public
//	機能　：デストラクタ
//---------------------------------------------------------------------------------------------------------------------
TScreenWindow::~TScreenWindow(void)
{
	DestroyWindow(m_thisWnd);

	// クラス配列から削除する
	int		index;

	for( index=0; index< clsCnt;index++)
		if(clsArryWs[index]==this)
			break;

	memmove(clsArryWs + index, clsArryWs + index +1, sizeof(TScreenWindow *) * (--clsCnt - index));
	clsArryWs = (TScreenWindow **)realloc(clsArryWs, sizeof(TScreenWindow *) * clsCnt);

}


//---------------------------------------------------------------------------------------------------------------------
//	TScreenWindow::CreateGrid(WNDPROC wproc, LPRECT pRect)
//	タイプ：Public
//	機能　：ウインドウ作成
//	引数２：親ウインドウに対する左上座標と、ウインドウの高さと巾
//---------------------------------------------------------------------------------------------------------------------
BOOL TScreenWindow::CreateScreen(LPRECT pRect)
{

	// スクロール指定がある場合のフラグ
	DWORD winStyle;

	m_Xsize = pRect->right;
	m_Ysize = pRect->bottom;

	// WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|WS_HSCROLL,
	winStyle = 	WS_VISIBLE|WS_CHILD|WS_BORDER;

	WNDCLASS wcl;
	char className[20];
	wsprintf(className,"cobascreen");

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
		"nok Fanatic class libraly scwin",
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


void TScreenWindow::Draw(void)
{

}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL	TSelectOrder::InitDialog(HWND hDlg)
//	タイプ：private
//	機能　：ウインドウ初期化。フォント、画面等のイニシャライズ
//---------------------------------------------------------------------------------------------------------------------
BOOL TScreenWindow::EvInit(HWND hwnd)
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

	// フォントの設定
	m_hFont = CreateFont(	
			10,
			0,				// フォントサイズ
			0,					// Escapement
			0,					// Orientation
			FW_NORMAL,				// Weight(フォントの太さ)
			0,					// Ital(イタリック体)
			0,					// Underline(下線)
			0,					// StrikeThru(打ち消し線)
			SHIFTJIS_CHARSET,	// Charset(文字セット)
			OUT_DEFAULT_PRECIS,	// Precision(出力精度)
			CLIP_DEFAULT_PRECIS,// ClipPrecision(クリッピ15ング精度)
			DEFAULT_QUALITY,	// Quality(フォントの出力品質)
			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(フォントのピッチとファミリ)
 				"ＭＳ ゴシック"
	);

	m_tmpFont = (HFONT)SelectObject( m_hdcScrWin , m_hFont );


	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	LONG TScreenWindow::DoCommand(HWND hDlg ,UINT message, WPARAM wParam, LPARAM lParam)
//	タイプ：Public
//	機能　：メッセージループ
//---------------------------------------------------------------------------------------------------------------------
LONG TScreenWindow::DoCommand(HWND hWind ,UINT message, WPARAM wParam, LPARAM lParam)
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
//	タイプ：public
//	機能　：線を引く
//	Input:	int sx;			開始Ｘ座標(0
//			int sy;			開始Ｙ座標(0.1mm)
//			int ex;
//			int ey;
//			int LineType 線の種類
//					0:実線
//					1:破線
//					2:点線
//			int LineType 線のはば（実践指定時のみ有効）
//---------------------------------------------------------------------------------------------------------------------
void TScreenWindow::DrawLine( int sx,int sy,int ex, int ey,int LineType ,int LineWidth,COLORREF theColor,BOOL redraw)
{
	HPEN	hOldpen,hPen;				// Handle of pen
	int theStyle,theWidth;

	switch( LineType ){
		case 1:						// 破線
			theStyle = PS_DASH;
			theWidth = 1;
			break;
		case 2:						// 点線
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

	// 線を引く
	MoveToEx( m_hdcScrWin , sx , sy , NULL );
	LineTo(   m_hdcScrWin , ex , ey );


	// GDIもどし
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

	// 塗りつぶし
	if( paint ){
		Ellipse( m_hdcScrWin , x-r ,y-r,x+r,y+r);
	}
	// からの○
	else{
		Arc( m_hdcScrWin , x-r ,y-r,x+r,y+r,x,y-r,x,y-r);
	}
	hPen   = (HPEN)SelectObject(m_hdcScrWin,hOldpen);
	hbrush   = (HBRUSH)SelectObject(m_hdcScrWin,hold);
	DeleteObject( hPen );
	DeleteObject( hbrush );
}

//============================================================================
//		概要 : 文字印刷
//		Input:	int x;			Ｘ座標
//				int y;			Ｙ座標
//				int nFont;		フォントの番号デフォルトは０
//				int nBold:		字の太さ（CreateFontの定数）
//				int nPoint1;	フォントのサイズ(X)
//				int nPoint2;	フォントのサイズ(Y)
//				LPSTR Message;	文字列
//		Return:
//============================================================================
void TScreenWindow::TextOut( int x, int y, int fontsize, char* txt ,COLORREF color,char* font, int charset)
{
	if( NULL==txt ) return;

	HFONT	holdf,hnewf;

	// フォントの設定
	hnewf = CreateFont(	
			fontsize,
			0,				// フォントサイズ
			0,					// Escapement
			0,					// Orientation
			FW_NORMAL,				// Weight(フォントの太さ)
			0,					// Ital(イタリック体)
			0,					// Underline(下線)
			0,					// StrikeThru(打ち消し線)
			charset,//SHIFTJIS_CHARSET,	// Charset(文字セット)
			OUT_DEFAULT_PRECIS,	// Precision(出力精度)
			CLIP_DEFAULT_PRECIS,// ClipPrecision(クリッピ15ング精度)
			DEFAULT_QUALITY,	// Quality(フォントの出力品質)
			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(フォントのピッチとファミリ)
 			font
	);

	holdf = (HFONT)SelectObject(m_hdcScrWin , hnewf );	// フォントの切り替え

	// フォント色
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
//		概要 : 文字印刷
//		Input:	int x;			Ｘ座標
//				int y;			Ｙ座標
//				int nFont;		フォントの番号デフォルトは０
//				int nBold:		字の太さ（CreateFontの定数）
//				int nPoint1;	フォントのサイズ(X)
//				int nPoint2;	フォントのサイズ(Y)
//				LPSTR Message;	文字列
//		Return:
//============================================================================
void TScreenWindow::TextOutV( int x, int y, int fontsize, char* txt ,COLORREF color,char* font, int charset)
{
	if( NULL==txt ) return;

	HFONT	holdf,hnewf;

	// フォントの設定
	hnewf = CreateFont(	
			fontsize,
			0,				// フォントサイズ
			900,					// Escapement
			900,					// Orientation
			FW_NORMAL,				// Weight(フォントの太さ)
			0,					// Ital(イタリック体)
			0,					// Underline(下線)
			0,					// StrikeThru(打ち消し線)
			charset,//SHIFTJIS_CHARSET,	// Charset(文字セット)
			OUT_DEFAULT_PRECIS,	// Precision(出力精度)
			CLIP_DEFAULT_PRECIS,// ClipPrecision(クリッピ15ング精度)
			DEFAULT_QUALITY,	// Quality(フォントの出力品質)
			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(フォントのピッチとファミリ)
 			font
	);

	holdf = (HFONT)SelectObject(m_hdcScrWin , hnewf );	// フォントの切り替え

	// フォント色
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
		case 1:						// 破線
			theStyle = PS_DASH;
			theWidth = 1;
			break;
		case 2:						// 点線
			theStyle = PS_DOT;
			theWidth = 1;
			break;

		default:
			theStyle = PS_SOLID;
			theWidth = LineWidth;
			break;
	}

	// 塗りつぶし
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
//	タイプ：virtual	(private)
//	機能　：クリックされた時に呼ばれる関数
//	引数１：クリックされた列番号
//	引数２：クリックされた行番号
//	戻り値：デフォルト処理をする＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TScreenWindow::EvClick( long col,long row)
{
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL EvClickPrivate(HWND hwnd,LPARAM lparam)
//	タイプ：private
//	機能　：マウスクリック時のイベント。何処のセルがクリックされたか調べて、仮想関数を呼び出す
//---------------------------------------------------------------------------------------------------------------------
BOOL TScreenWindow::EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick)
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


