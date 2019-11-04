//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:GridView.cpp
//	Class		:CGridViewCtrl
// 				:グリッドウインドウ・ビュークラス
//	作成日		:1999.07.30	Coded by coba
//
//========================================================================================
#include ".\grdview3.h"
#include "colordef.h"
#include "stdio.h"

#define WM_USER_KEYDOWN WM_USER+100

// おまじない関係--------------------------------------------------------------------------
static CGridViewCtrl** clsArray=NULL;
static int clsCnt=0;
static LRESULT CALLBACK GridProc(HWND, UINT, WPARAM, LPARAM);
static LPARAM CALLBACK GridEditProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK KeyHookProc(int, WPARAM, LPARAM);
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
CGridViewCtrl::CGridViewCtrl(HINSTANCE inst,HWND hparent,long rowblock,long totalcol):TGridData(rowblock,totalcol,FALSE)
{
	m_hInst = inst;
	m_hwndParent = hparent;

	m_WindowInit=FALSE;

	m_Top=0;					// 表示中の最上部の行番号
	m_Bottom=0;				// 表示中の最下部の行番号
	m_Left=0;
	m_Right=0;

	m_EditMode = FALSE;

	// デフォルト設定
	memset( &m_info, 0, sizeof(m_info) );
	m_info.fontsize=12;				// フォントサイズ
	m_info.rowtitle=FALSE;			// 行ヘッダ
	m_info.rowtitleWidth=20;			// 行ヘッダの幅( 指定なしの時の為にデフォルト幅)
	m_info.hScroll=FALSE;			// 水平スクロールなし
	m_info.vScroll=TRUE;			// 垂直スクロールあり	m_info.readonly=FALSE;			// 編集可能


	// デフォルトのサイズ
	m_info.fontsize = 12;
	m_ColHeight = m_info.fontsize +4;
	m_DispRows = 0;

	// デフォルトで固定行は列題のみ
	m_FixRows=0;				// 固定行（縦スクロールなし）の行数
	m_FixCols=0;				// 固定列 2004.06.08

	m_ColInfo = (COLUMNPOSITIONINFO*)malloc(sizeof(COLUMNPOSITIONINFO) * (totalcol+1));
	memset(m_ColInfo, 0, sizeof(COLUMNPOSITIONINFO)*(totalcol+1));

	// アクティブセル
	memset( &m_ActCel, 0, sizeof(m_ActCel) );
	m_ActCel.col = -1;

	// クラス配列

	CGridViewCtrl** _clsArray = (CGridViewCtrl **)realloc(clsArray, sizeof(CGridViewCtrl *) * (clsCnt +1));
	(clsArray = _clsArray)[clsCnt] = this;
	clsCnt++;

	memset( &m_CellBuf, 0, sizeof(CELLCOPYBUF));
	m_CellBuf.col = -1;
	m_CellBuf.row = -1;



}


//---------------------------------------------------------------------------------------------------------------------
//	TSelectOrder::TSelectOrder()
//	タイプ：Public
//	機能　：デストラクタ
//---------------------------------------------------------------------------------------------------------------------
CGridViewCtrl::~CGridViewCtrl(void)
{
	free(m_ColInfo);

	SetWindowLong(m_EditWnd,GWL_WNDPROC,m_oldEditProc);
	UnhookWindowsHookEx(m_KeyHook);
	DestroyWindow(m_ListWnd);

	// クラス配列から削除する
	int		index;

	for( index=0; index< clsCnt;index++)
		if(clsArray[index]==this)
			break;


	memmove(clsArray + index, clsArray + index +1, sizeof(CGridViewCtrl *) * (--clsCnt - index));
	clsArray = (CGridViewCtrl **)realloc(clsArray, sizeof(CGridViewCtrl *) * clsCnt);

}


//---------------------------------------------------------------------------------------------------------------------
//	CGridViewCtrl::CreateGrid(WNDPROC wproc, LPRECT pRect)
//	タイプ：Public
//	機能　：ウインドウ作成
//	引数２：親ウインドウに対する左上座標と、ウインドウの高さと巾
//	引数３：グリッド設定情報構造体
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::CreateGrid(LPRECT pRect,GRIDINFO* gr,char* fontname,DWORD charset)
{
	// グリッド設定
	memset( &m_info, 0, sizeof(m_info) );
	memcpy( &m_info, gr, sizeof(m_info) );

	// サイズ
	m_ColHeight = m_info.fontsize +4;

	// 行ヘッダの幅をチェック
	if( !m_info.rowtitle)			// 行ヘッダなし
		m_info.rowtitleWidth=20;		// 念のため
	else if( m_info.rowtitle && !m_info.rowtitleWidth )
		m_info.rowtitleWidth = 20;

	TGridData::m_RowTitleEnable=m_info.rowtitle;

	memcpy( &m_ListRect, pRect, sizeof(RECT) );

	// スクロール指定がある場合のフラグ
	DWORD winStyle;
	int hscrollsize=0;
	int vscrollsize=0;

	// WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|WS_HSCROLL,
	winStyle = 	WS_VISIBLE|WS_CHILD|WS_BORDER;


	// スクロールバーの分だけクライアント領域を調整
	if( m_info.vScroll ){
		vscrollsize= 22;			// スクロールの分
		winStyle |= WS_VSCROLL;
	}

	if( m_info.hScroll ){
		winStyle |= WS_HSCROLL;
		hscrollsize= 20;			// スクロールの分
	}

	WNDCLASS wcl;
	char className[20];
	wsprintf(className,"cobalist");

	// 登録済みか？
	if( !GetClassInfo(m_hInst, className, &wcl) ){
		// クラスの登録
		memset(&wcl, 0, sizeof(WNDCLASS) );
		wcl.hInstance = m_hInst;
		wcl.lpszClassName =className;
		wcl.lpfnWndProc = GridProc;
		wcl.style = (CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW | CS_DBLCLKS | CS_GLOBALCLASS | CS_OWNDC);
		wcl.hIcon = NULL;
		wcl.hCursor =  LoadCursor(NULL, IDC_ARROW);
		wcl.lpszMenuName = 0;
		wcl.cbClsExtra = 0; 
		wcl.cbWndExtra = 0;
		wcl.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));	/* Make the window background white. */

		/* Register the window class. */
		if(!RegisterClass(&wcl)){
			MessageBox(NULL,"RegistarClass() err","coba's Fanatic class libraly",MB_OK);
			return(FALSE) ;
		}
	}

	//==============お手製リストコントロール==================
	m_ListWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		"coba Fanatic class libraly",
		winStyle,
		pRect->left,		pRect->top,
		pRect->right+vscrollsize,			// スクロールの分
		pRect->bottom + hscrollsize,
		m_hwndParent,
		NULL,
		m_hInst,
		NULL 
	);

	// 表示行数の決定(WindowHeight / colheightから行ヘッダを引いた数)
	m_DispRows = (short)((pRect->bottom / m_ColHeight ) -1);
	if( m_DispRows < 0 ) m_DispRows = 0;

	//================入力用エディットコントロール====================
	m_EditWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"EDIT","",
		WS_CHILD|ES_LEFT|ES_AUTOHSCROLL|ES_WANTRETURN|ES_MULTILINE,
		0,0,100,m_ColHeight,
		m_ListWnd,(HMENU)(100),	m_hInst,NULL
	);

	// フォントをコピー
	memset( m_FontName, 0, sizeof(m_FontName));
	if( fontname == NULL || !strlen(fontname) ){
		strcpy( m_FontName,"ＭＳ ゴシック");
		m_CharSet = SHIFTJIS_CHARSET;
	}
	else{
		memcpy( m_FontName,fontname,strlen(fontname)>100 ? 100:strlen(fontname));
		m_CharSet = charset;
	}

	// サブクラス化
	m_oldEditProc = SetWindowLong(m_EditWnd,GWL_WNDPROC, (LONG)GridEditProc);

	m_eFont = CreateFont(	
			m_info.fontsize,
			0,				// フォントサイズ
			0,					// Escapement
			0,					// Orientation
			FW_NORMAL,				// Weight(フォントの太さ)
			0,					// Ital(イタリック体)
			0,					// Underline(下線)
			0,					// StrikeThru(打ち消し線)
//			SHIFTJIS_CHARSET,	// Charset(文字セット)
			m_CharSet,			// Charset(文字セット)
			OUT_DEFAULT_PRECIS,	// Precision(出力精度)
			CLIP_DEFAULT_PRECIS,// ClipPrecision(クリッピング精度)
			DEFAULT_QUALITY,	// Quality(フォントの出力品質)
			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(フォントのピッチとファミリ)
			m_FontName
// 				"ＭＳ ゴシック"
	);


	EvInit(m_ListWnd);

	SendMessage( m_EditWnd, WM_SETFONT,(WPARAM)m_eFont,(LPARAM)MAKELONG((WORD)TRUE,0));

	ShowWindow(m_ListWnd, SW_SHOW);
	UpdateWindow(m_ListWnd);
	 m_WindowInit=TRUE;

	// スクロール指定がある場合
	if( m_info.vScroll )
		EnableScrollBar(m_ListWnd,SB_VERT,ESB_DISABLE_BOTH);

	if( m_info.hScroll ){
		EnableScrollBar(m_ListWnd,SB_HORZ,ESB_DISABLE_BOTH);
		SetHScroll();
	}

	// 固定行関係
	m_FixRows = m_info.fixRows;
	m_Top=m_FixRows;					// 表示中の最上部の行番号
	m_ActCel.row = m_FixRows;

	// 固定列関係	2004.06.08
	m_FixCols = m_info.fixCols;
	m_Left=m_FixCols;					// 表示中の最上部の列番号
	m_ActCel.col = m_FixCols;

	// keyHook
	m_KeyHook = SetWindowsHookEx(WH_GETMESSAGE,(HOOKPROC)KeyHookProc,NULL,GetWindowThreadProcessId( m_ListWnd,NULL));



	return(TRUE);
}


void CGridViewCtrl::Draw(void)
{

	DrawData();
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL	TSelectOrder::InitDialog(HWND hDlg)
//	タイプ：private
//	機能　：ウインドウ初期化。フォント、画面等のイニシャライズ
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvInit(HWND hwnd)
{
	// 描画用仮想デバイスコンテキスト作成
	HDC tmpHdc;

	tmpHdc=GetDC(hwnd);
	SelectObject( tmpHdc,GetStockObject(WHITE_BRUSH));
	m_hdcList=CreateCompatibleDC(tmpHdc);
	m_hBit=CreateCompatibleBitmap(tmpHdc, m_ListRect.right, m_ListRect.bottom);
	SelectObject( m_hdcList,GetStockObject(WHITE_BRUSH));
	m_hOldBit = (HBITMAP)SelectObject(m_hdcList,m_hBit);
	PatBlt(m_hdcList,0,0,m_ListRect.right,m_ListRect.bottom,PATCOPY);
	ReleaseDC( hwnd,tmpHdc );

	// フォントの設定
	m_hFont = CreateFont(	
			m_info.fontsize,
			0,				// フォントサイズ
			0,					// Escapement
			0,					// Orientation
			FW_NORMAL,				// Weight(フォントの太さ)
			0,					// Ital(イタリック体)
			0,					// Underline(下線)
			0,					// StrikeThru(打ち消し線)
//			SHIFTJIS_CHARSET,	// Charset(文字セット)
			m_CharSet,			// Charset(文字セット)
			OUT_DEFAULT_PRECIS,	// Precision(出力精度)
			CLIP_DEFAULT_PRECIS,// ClipPrecision(クリッピ15ング精度)
			DEFAULT_QUALITY,	// Quality(フォントの出力品質)
			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(フォントのピッチとファミリ)
			m_FontName
//			"ＭＳ ゴシック"
	);

	m_tmpFont = (HFONT)SelectObject( m_hdcList , m_hFont );

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL EvClickPrivate(HWND hwnd,LPARAM lparam)
//	タイプ：private
//	機能　：マウスクリック時のイベント。何処のセルがクリックされたか調べて、仮想関数を呼び出す
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick)
{
	WORD xPos,yPos,yCel,xCel;
	int i;
	long xColPos=0;
	long currentRow;

	SetFocus(hwnd);

	// リストの度の領域がクリックされたか調べる
	xPos = LOWORD(lparam);
	yPos = HIWORD(lparam);

	// クリック位置から、（表示上の）何処のセルかを調べる
	// y方向
	// ヘッダ領域ならば無視
	yCel = yPos / m_ColHeight;

	// x方向
	BOOL xOk=FALSE;
/// 固定列も考慮 2004.06.08
	xColPos = m_info.rowtitleWidth;
	for( i=0; i< m_FixCols;i++){
		xColPos += m_ColInfo[i].width;
		if( xPos < xColPos){
			xCel = i;
			xOk=TRUE;
			break;
		}
	}
///	xColPos = m_info.rowtitleWidth;
	if( xOk == FALSE){
		for( i=m_Left; i<= m_Right+1;i++){
			xColPos += m_ColInfo[i].width;
			if( xPos < xColPos){
				xCel = i;
				xOk=TRUE;
				break;
			}
		}
	}
	// クリック位置が行ヘッダか？
	if(TGridData::m_RowTitleEnable==TRUE){
		if( xPos < m_info.rowtitleWidth ) xOk = FALSE;
	}


	// クリック行が列ヘッダまたは固定行の場合
//	if( !yCel || yCel <= m_FixRows){	// yCel<FixRows ...行ヘッダor固定行
//		return(FALSE);
//	}
	// クリック行が列ヘッダの場合
	if( !yCel){	// 行ヘッダ
		EvHeaderClick(xCel);
	}
	// クリック行が固定行の場合
	if( yCel <= m_FixRows){	// yCel<FixRows ...固定行
		return(FALSE);
	}
	// クリックしたセルが有効データ領域か？
	if( m_Top + yCel-m_FixRows-1 > TGridData::m_LastRowNo || !xOk){
		return(FALSE);
	}

	// クリック位置から、データ行を計算
	currentRow=(long)yCel + m_Top - m_FixRows - 1;

	if(currentRow < m_FixRows ) return(FALSE);

	// 仮想関数呼び出し
	if(!dblClick){
		if( EvClick(xCel,currentRow) ){
			// クリックしたセルをアクティブに
			MoveActiveCel(xCel,currentRow,TRUE);
		}
	}
	// ダブルクリック
	else{
		EvDblClick(xCel,currentRow);
	}
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::StartEdit(DWORD x,DWORD y)
//	タイプ：private
//	機能　：セルの編集開始。編集対象のセルにエディットコントロールを表示して入力する。
//	引数１：対象セルの列
//	引数２：行
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::StartEdit(DWORD x,DWORD y,char keycode,BOOL first)
{

	int  editX,editY,i;
	char theData[256];
	int xColPos=0;
	// 編集中ならば終わらせる
	EndEdit(1);

	LostFocusCel(m_ActCel.col,m_ActCel.row);

	// 画面上の列の座標を取る
	xColPos = m_info.rowtitleWidth;
	for( i= 0; (WORD)i < m_FixCols; i++){
		xColPos += m_ColInfo[i].width;
	}
	for( i= m_Left; (WORD)i < x; i++){
		xColPos += m_ColInfo[i].width;
	}

	// エディットウインドウの位置を決定
	editX = xColPos+1;
	editY = (y - m_Top +1+m_FixRows) * m_ColHeight;

	// セルデータを取得
	memset( theData, 0, sizeof(theData) );
	if( !GetCel( theData, sizeof(theData)-1, x,y ) ) return(FALSE);

	m_EditMode = TRUE;

	// エディットウインドウを移動
	SetWindowText( m_EditWnd, NULL );
	SetWindowText( m_EditWnd, theData );
	MoveWindow( m_EditWnd, editX, editY,m_ColInfo[x].width,m_ColHeight+1,TRUE);
	SendMessage( m_EditWnd, EM_SETSEL,(WPARAM)0,(LPARAM)strlen(theData));
	ShowWindow(m_EditWnd,SW_SHOW);
	SetFocus(m_EditWnd);

	// 移動完了関数
	EvEditBoxMove( x, y);


	// 半角モードの場合で、入力開始のときは、押されたキーをダイレクトに反映
	if( (!GetImeStatus() && keycode !=NULL) ||
		(!GetImeStatus() && keycode !=NULL && first) ){
		if( keycode ==VK_BACK)
			memset( theData, 0, sizeof(theData) );
		else
			wsprintf(theData,"%c",keycode);
		SetWindowText( m_EditWnd, theData);
		SendMessage( m_EditWnd, EM_SETSEL,(WPARAM)1,(LPARAM)1);
	}
	// IMEonの時にキーｺｰﾄﾞを送る
//	else if(first){
	else if( keycode !=NULL && GetImeStatus() && first){
		HIMC imc;
		char aaa[20];
		imc=ImmGetContext(m_EditWnd);
		wsprintf(aaa,"%c",keycode);
		ImmSetCompositionString(imc,SCS_SETSTR,NULL,0,aaa,1);
		ImmReleaseContext(m_EditWnd,imc);
	}

	// 編集中のセル代入
	m_editX=x,m_editY=y;
	return(TRUE);

}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::EndEdit(void)
//	タイプ：private
//	機能　：セルの編集終了。データ更新
//	引数　：終了タイプ　0 チェックなし
//						1 チェックのみ
//						2 チェック&移動
//						3 キャンセル
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EndEdit(int action)
{

	// 編集中ではないとき(再帰的に呼ばれるのを防ぐため)
	if(!m_EditMode) return(FALSE);

	char theData[256];
	char color = 0;

	// キャンセルの時
	if( action==3){
		// 編集モードoff
		m_EditMode = FALSE;

		// ウインドウ隠す
		ShowWindow(m_EditWnd,SW_HIDE);
		SetWindowText( m_EditWnd, NULL );
		DrawCel(m_ActCel.col,m_ActCel.row,TRUE);
		return(TRUE);
	}

	// 編集データをウインドウから取得
	memset( theData, 0, sizeof(theData) );
	GetWindowText( m_EditWnd, theData,sizeof(theData)-1 );


	short movenext=NOT_MOVE;
	long nextX=-1,nextY=-1;

	// 編集モードoff
	m_EditMode = FALSE;

	// テキストチェック
	if( action > 0 ){
		// チェック関数（仮想）を呼び出し
		if( !EvEndEdit( m_editX, m_editY,theData, &movenext, &nextX,&nextY, &color)){
			// 不合格の時はテキストクリア
			SetWindowText( m_EditWnd, NULL );
/*			if(action ==0)
				ShowWindow(m_EditWnd,SW_HIDE);
			return(FALSE);
*/
		}
	}


	// ウインドウ隠す
	ShowWindow(m_EditWnd,SW_HIDE);
	SetWindowText( m_EditWnd, NULL );


	if(movenext && action == 2 ){
		// まずはデータをセット
		if( !SetCel( m_editX,m_editY,theData,TGridData::GetCelTag(m_editX,m_editY), TRUE ,color,-1)) return(FALSE);
		// 移動先が有効な場合はそこに飛ぶ
		if( nextX < TGridData::m_TotalCols && nextY <= TGridData::m_LastRowNo ){
			m_ActCel.col=nextX;m_ActCel.row=nextY;
			// 次の編集先が指定されていたらそこに飛ぶ
			MoveActiveCel(nextX,nextY,TRUE);
			// テキストボックスオープンのまま移動する場合
			if( movenext == MOVE_WITH_EDIT ) MoveEditBox( nextX, nextY);
		}
		else{
			DrawCel(m_ActCel.col,m_ActCel.row,TRUE);
			// 派生クラス側では移動完了を待ってるかもしれなので呼び出し
			EvMoveCelEnd( m_ActCel.col, m_ActCel.row);
		}
	}
	// データ領域に設定
	else{
		if( !SetCel( m_editX,m_editY,theData,TGridData::GetCelTag(m_editX,m_editY), TRUE ,color,-1)) return(FALSE);
			DrawCel(m_ActCel.col,m_ActCel.row,TRUE);
	}
	if(movenext== MOVE_LOSTFOCUS ){
		LostFocusCel(m_ActCel.col,m_ActCel.row);
	}
	return(TRUE);

}
//---------------------------------------------------------------------------------------------------------------------
//　BOOL CGridViewCtrl::EvEditBoxMove( long col, long row)
//	タイプ：private
//	機能　：エディットコントロールの移動完了
//	引数１：移動先のデータ列
//	引数２：異動先のデータ行
//	戻り値：TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvEditBoxMove( long col, long row)
{
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//　BOOL CGridViewCtrl::CellCopy(void)
//	タイプ：private
//	機能　：アクティブセルの内容をバッファにコピー
//	戻り値：TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::CellCopy(void)
{
	// コピー元のセルは有効か？
	if( m_ActCel.col > TGridData::m_TotalCols || m_ActCel.col < m_FixCols ) return(FALSE);
//	if( m_ActCel.col > TGridData::m_TotalCols || m_ActCel.col < 0 ) return(FALSE);

	if( m_ActCel.row > TGridData::m_LastRowNo || m_ActCel.row < m_FixRows ) return(FALSE);

	// 内容をバッファにコピーする
	m_CellBuf.data[sizeof(m_CellBuf.data)-1]=0;
	memcpy( m_CellBuf.data, TGridData::m_StrData[ m_ActCel.row].d[ m_ActCel.col], 100);
	m_CellBuf.col=m_ActCel.col;
	m_CellBuf.row=m_ActCel.row;
	m_CellBuf.tag=TGridData::m_StrData[ m_ActCel.row].celTag[ m_ActCel.col];
	m_CellBuf.color=TGridData::m_StrData[ m_ActCel.row].color[ m_ActCel.col];
	m_CellBuf.bkcolor=TGridData::m_StrData[ m_ActCel.row].bkcolor[ m_ActCel.col];
	return(TRUE);

}

//---------------------------------------------------------------------------------------------------------------------
//　BOOL CGridViewCtrl::CellPaste(int chkmode)
//	タイプ：private
//	機能　：バッファの内容をアクティブセルにコピー
//	引数　：1=行が同じ場合のみコピー 2=列が同じ場合のみコピー その他＝チェック無し
//	戻り値：TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::CellPaste(int chkmode)
{
	// コピー先のセルは有効か？
/// 固定行 2004.06.08
	if( m_ActCel.col > TGridData::m_TotalCols || m_ActCel.col < m_FixCols ) return(FALSE);
//	if( m_ActCel.col > TGridData::m_TotalCols || m_ActCel.col < 0 ) return(FALSE);
///

	if( m_ActCel.row > TGridData::m_LastRowNo || m_ActCel.row < m_FixRows ) return(FALSE);

	// コピーバッファは正常？
	if( m_ActCel.col< 0 ||  m_ActCel.row < 0 ) return(FALSE);

	switch(chkmode){
		// 行一致モード
		case 1:
			if(m_ActCel.row != m_CellBuf.row) return(FALSE);
			break;

		case 2:
			if(m_ActCel.col != m_CellBuf.col) return(FALSE);
			break;

		default:
			break;
	}

	// 内容をバッファからコピーする
	if( !TGridData::SetCelData(  m_ActCel.col, m_ActCel.row, m_CellBuf.color,m_CellBuf.tag,m_CellBuf.data,m_CellBuf.bkcolor)) return(FALSE);

	// リストに書き込む
	if( !DrawCel( m_ActCel.col,m_ActCel.row,TRUE) ) return(FALSE);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::MoveEditBox( long col, long row)
//	タイプ：private
//	機能　：エディットコントロールの自動移動
//	引数１：移動先のデータ列
//	引数２：異動先のデータ行
//	戻り値：TRUE=移動 FALSE=移動せず
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::MoveEditBox( long col, long row)
{

	// 移動先のセルは有効か？
/// 固定行 2004.06.08
	if( col > TGridData::m_TotalCols || col < m_FixCols ) return(FALSE);
//	if( col > TGridData::m_TotalCols || col < 0 ) return(FALSE);
///

	if( row > TGridData::m_LastRowNo || row < m_FixRows ) return(FALSE);

	// 縦移動
	// スクロール可能で且つ
	// 移動先が現在の表示領域内に収まってないときはスクロールする
	if( ( TGridData::m_LastRowNo > m_DispRows ) &&
		( row < m_Top || row >= m_Top + m_DispRows-m_FixRows )){
		// 縦スクロール不可の時は何もしない
		if( !m_info.vScroll ) return(FALSE);


		if( row < m_Top ) m_Top = row;
		else m_Top = row - m_DispRows + m_FixRows + 1;

		// データ再描画
		DrawData();

		// スクロールバーを移動
		SCROLLINFO sinfo;

		// スクロール情報を取得
		sinfo.cbSize=sizeof(SCROLLINFO);
		sinfo.fMask = SIF_POS;
		sinfo.nPos = m_Top;
		SetScrollInfo( m_ListWnd, SB_VERT,&sinfo ,TRUE);
	}

	// エディットコントロールを移動する
	int  editX,editY,len,xColPos,i;
	char txtBuf[256];

	// 画面上の列の座標を取る
/// 固定行
	xColPos = m_info.rowtitleWidth;
	for( i= 0; i < m_FixCols; i++)
		xColPos += m_ColInfo[i].width;
	for( i= m_Left; i < col; i++){
		xColPos += m_ColInfo[i].width;
	}

	// エディットウインドウの位置を決定
	editX = xColPos+1;
	editY = m_ColHeight*( row - m_Top +1+m_FixRows);		// ヘッダと固定行の分pulus

	m_EditMode = TRUE;

	// セルデータを取得
	wsprintf( txtBuf,"%s",TGridData::m_StrData[row].d[col] );
	len = strlen(txtBuf);

	SetWindowText( m_EditWnd, txtBuf );
	MoveWindow( m_EditWnd, editX, editY,m_ColInfo[col].width,m_ColHeight,TRUE);		// 移動
	ShowWindow(m_EditWnd,SW_SHOW);
	SendMessage( m_EditWnd, EM_SETSEL,(WPARAM)0,(LPARAM)len);			// テキスト選択
	SetFocus(m_EditWnd);

	// 編集中のセル代入
	m_editX=col,m_editY=row;

	// 移動完了関数
	EvEditBoxMove( col, row);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::SetIme( int mode )
//	タイプ：protected
//	機能　：入力エディットボックスのimeの設定
//	引数１：設定するモード
//			off	0
//			全角	1
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetIme( int mode )
{
	HIMC imc;
	imc=ImmGetContext(m_EditWnd);

	// 半角英数
	DWORD dwMode=0,dwSent=0;
	ImmGetConversionStatus(imc,&dwMode,&dwSent);
	dwMode=0;
	// OFF
	if(!mode){
		dwMode =IME_CMODE_NOCONVERSION;
		ImmSetConversionStatus(imc,dwMode,dwSent);
		if( ImmGetOpenStatus(imc))
			ImmSetOpenStatus(imc,FALSE);
	}
	else{
		// 変換モード設定
		dwMode =IME_CMODE_ROMAN|IME_CMODE_NATIVE;
		ImmSetConversionStatus(imc,dwMode,dwSent);
		// 非openならimeを起動
		if( !ImmGetOpenStatus(imc))
			ImmSetOpenStatus(imc,TRUE);
	}
	ImmReleaseContext(m_EditWnd,imc);
	return(TRUE);

}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::GetImeStatus(void)
//	タイプ：protected
//	機能　：入力エディットボックスのimeのOnOffを調べる
//	戻り値：TRUE=ON,FALSE=OFF
//			全角	1
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::GetImeStatus(void)
{

	HIMC imc;
	imc=ImmGetContext(m_EditWnd);

	// open status
	if( !ImmGetOpenStatus(imc)) return(FALSE);

	// 半角英数
	DWORD dwMode=0,dwSent=0;
	if(!ImmGetConversionStatus(imc,&dwMode,&dwSent)) return(FALSE);

	if(dwMode==IME_CMODE_NOCONVERSION ) return(FALSE);

	 ImmReleaseContext(m_EditWnd,imc);
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::SetColumnHeadder( long col, char *titlestr,long width,short pos)
//	タイプ：private
//	機能　：列データの初期化
//	引数１：設定する列番号
//	引数２：列の巾
//	引数３：文字の表示位置
//			POS_LEFT	0
//			POS_CENTER	1
//			POS_RIGHT	2
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetColumnHeadder( long col, char *titlestr,long width,short pos)
{
	if( col >= TGridData::m_TotalCols || col < 0) return(FALSE);

	// データの代入
	if( !TGridData::SetTitle(col,titlestr)) return(FALSE);

	m_ColInfo[col].width=width;

	m_ColInfo[col].position=pos;


	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::DrawHeadder(void)
//	タイプ：private
//	機能　：列ヘッダの表示
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::DrawHeadder(BOOL redraw)
{
	if( !m_WindowInit ) return(FALSE);

	RECT rect;
	COLORREF colOld;
	HPEN	hOldpen,hPen;				// Handle of pen
	int i;
	int xPos = 0;
	HBRUSH hbr;


	// 塗りつぶし
	// 塗りつぶし
	// データ領域
	SetRect(&rect ,m_info.rowtitleWidth, m_ColHeight ,m_ListRect.right, m_ListRect.bottom);
	FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));
	// 列題領域
	SetRect(&rect,0,0, m_ListRect.right, m_ColHeight);
//	FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	hbr=CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	FillRect( m_hdcList,&rect,hbr);
	// 行ヘッダ
	SetRect(&rect,0,0, m_info.rowtitleWidth, m_ListRect.bottom);
//	FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	FillRect( m_hdcList,&rect,hbr);
	DeleteObject(hbr);

	SetTextColor( m_hdcList, CLR_BLACK);

	//-----------------------列ヘッダ------------------------------------
	// 枠線（下）
	hPen   = CreatePen( PS_SOLID ,1, CLR_WHITE);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	MoveToEx( m_hdcList , 0,  0 , NULL );
	LineTo(   m_hdcList , m_ListRect.right, 0 );
	hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
	DeleteObject( hPen );

	// 枠線（下）
	hPen   = CreatePen( PS_SOLID ,1, COLOR_BTNSHADOW);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	MoveToEx( m_hdcList , 0 , m_ColHeight , NULL );
	LineTo(   m_hdcList , m_ListRect.right, m_ColHeight );
	hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
	DeleteObject( hPen );

	//-----------------------行ヘッダ------------------------------------

	// データ行との区切り（３Ｄ風）
	hPen   = CreatePen( PS_SOLID ,1, COLOR_3DDKSHADOW);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	MoveToEx( m_hdcList , m_info.rowtitleWidth, 0 , NULL );
	LineTo(   m_hdcList , m_info.rowtitleWidth, m_ListRect.bottom );
	hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
	DeleteObject( hPen );

	hPen   = CreatePen( PS_SOLID ,1, CLR_DGRAY);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	MoveToEx( m_hdcList , m_info.rowtitleWidth-1, 0 , NULL );
	LineTo(   m_hdcList , m_info.rowtitleWidth-1, m_ListRect.bottom );
	hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
	DeleteObject( hPen );

	hPen   = CreatePen( PS_SOLID ,1, CLR_WHITE);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	MoveToEx( m_hdcList , m_info.rowtitleWidth+1, 0 , NULL );
	LineTo(   m_hdcList , m_info.rowtitleWidth+1, m_ListRect.bottom );
	hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
	DeleteObject( hPen );

	// 横罫線
	for( i=0; i<=m_DispRows; i++){
		hPen   = CreatePen( PS_SOLID ,1, CLR_WHITE);
		hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
		MoveToEx( m_hdcList , 0, m_ColHeight*(i) +1 , NULL );
		LineTo(   m_hdcList , 0+m_info.rowtitleWidth, m_ColHeight*(i)+1 );
		hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
		DeleteObject( hPen );

		hPen   = CreatePen( PS_SOLID ,1, COLOR_BTNSHADOW);
		hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
		MoveToEx( m_hdcList , 0, m_ColHeight*(i+1), NULL );
		LineTo(   m_hdcList , 0+m_info.rowtitleWidth, m_ColHeight*(i+1) );
		hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
		DeleteObject( hPen );
	}

	//-----------------------列ヘッダ------------------------------------
	// 列区切り（３Ｄ風）固定列
	for( i = 0; i < m_FixCols; i++ ){
		hPen   = CreatePen( PS_SOLID ,1, COLOR_3DDKSHADOW);
		hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
		MoveToEx( m_hdcList , xPos +m_ColInfo[i].width+m_info.rowtitleWidth, 0 , NULL );
		LineTo(   m_hdcList , xPos  +m_ColInfo[i].width+m_info.rowtitleWidth, m_ColHeight );
		hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
		DeleteObject( hPen );

		hPen   = CreatePen( PS_SOLID ,1, CLR_DGRAY);
		hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
		MoveToEx( m_hdcList , xPos+m_ColInfo[i].width-1+m_info.rowtitleWidth , 0 , NULL );
		LineTo(   m_hdcList , xPos+m_ColInfo[i].width-1+m_info.rowtitleWidth  , m_ColHeight );
		hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
		DeleteObject( hPen );

		hPen   = CreatePen( PS_SOLID ,1, CLR_WHITE);
		hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
		MoveToEx( m_hdcList , xPos+m_ColInfo[i].width+1+m_info.rowtitleWidth , 0 , NULL );
		LineTo(   m_hdcList , xPos+m_ColInfo[i].width+1+m_info.rowtitleWidth  , m_ColHeight );
		hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
		DeleteObject( hPen );

		xPos += m_ColInfo[i].width;

		// 表示可能域を越えたらbreak
		if( xPos + m_info.rowtitleWidth >= m_ListRect.right ){
			break;
		}
	}
	// 列区切り（３Ｄ風）
	for( i = m_Left; i < TGridData::m_TotalCols; i++ ){
		hPen   = CreatePen( PS_SOLID ,1, COLOR_3DDKSHADOW);
		hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
		MoveToEx( m_hdcList , xPos +m_ColInfo[i].width+m_info.rowtitleWidth, 0 , NULL );
		LineTo(   m_hdcList , xPos  +m_ColInfo[i].width+m_info.rowtitleWidth, m_ColHeight );
		hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
		DeleteObject( hPen );

		hPen   = CreatePen( PS_SOLID ,1, CLR_DGRAY);
		hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
		MoveToEx( m_hdcList , xPos+m_ColInfo[i].width-1+m_info.rowtitleWidth , 0 , NULL );
		LineTo(   m_hdcList , xPos+m_ColInfo[i].width-1+m_info.rowtitleWidth  , m_ColHeight );
		hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
		DeleteObject( hPen );

		hPen   = CreatePen( PS_SOLID ,1, CLR_WHITE);
		hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
		MoveToEx( m_hdcList , xPos+m_ColInfo[i].width+1+m_info.rowtitleWidth , 0 , NULL );
		LineTo(   m_hdcList , xPos+m_ColInfo[i].width+1+m_info.rowtitleWidth  , m_ColHeight );
		hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
		DeleteObject( hPen );


		xPos += m_ColInfo[i].width;


		// 表示可能域を越えたらbreak
		if( xPos + m_info.rowtitleWidth >= m_ListRect.right ){
			break;
		}

		// ここで現在の右列を取得する
		m_Right = i;
	}

	//-----------------------データ領域------------------------------------
	// データ部の横罫線
	hPen   = CreatePen( PS_SOLID ,1, CLR_LGRAY);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	for( i=1	; i<m_DispRows+1; i++){
		MoveToEx( m_hdcList , 0 + m_info.rowtitleWidth, m_ColHeight*(i+1), NULL );
		LineTo(   m_hdcList , m_ListRect.right, m_ColHeight*(i+1) );
	}

	// データ部の縦罫線
	xPos = 0;

	// 固定列
	for( i = 0; i <m_FixCols; i++ ){
		MoveToEx( m_hdcList , xPos+m_ColInfo[i].width+m_info.rowtitleWidth, m_ColHeight , NULL );
		LineTo(   m_hdcList , xPos+m_ColInfo[i].width+m_info.rowtitleWidth, m_ListRect.bottom );
		xPos += m_ColInfo[i].width;
	}

	for( i = m_Left; i <m_Right+2; i++ ){
		MoveToEx( m_hdcList , xPos+m_ColInfo[i].width+m_info.rowtitleWidth, m_ColHeight , NULL );
		LineTo(   m_hdcList , xPos+m_ColInfo[i].width+m_info.rowtitleWidth, m_ListRect.bottom );
		xPos += m_ColInfo[i].width;
	}

	hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
	DeleteObject( hPen );


	// 背景色
//	SetTextColor( m_hdcList, CLR_BLACK);
	colOld = SetBkMode( m_hdcList,CLR_BLUE );

	// タイトル
	char txtBuf[256];

	xPos = m_info.rowtitleWidth;
	// 固定列
	for( i = 0; i < m_FixCols; i++ ){
		wsprintf(txtBuf,"%s",TGridData::m_Title[i]);
		SetRect(&rect,(xPos + 2 ),3,xPos+m_ColInfo[i].width-2,m_ColHeight-2);
		DrawText(m_hdcList,txtBuf,-1,&rect,DT_CENTER|DT_SINGLELINE);
		xPos += m_ColInfo[i].width;
	}

	for( i = m_Left; i < m_Right+2; i++ ){
		wsprintf(txtBuf,"%s",TGridData::m_Title[i]);
		SetRect(&rect,(xPos + 2 ),3,xPos+m_ColInfo[i].width-2,m_ColHeight-2);
		DrawText(m_hdcList,txtBuf,-1,&rect,DT_CENTER|DT_SINGLELINE);
		xPos += m_ColInfo[i].width;
	}

	SetBkColor( m_hdcList,colOld );


	// さいびょうが指定
	if(redraw){
		SetRect(&rect,0,0,m_ListRect.right,m_ListRect.bottom);
		InvalidateRect(m_ListWnd,&rect,TRUE);
	}
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::ChangeRowTitle(long rowNo, char* newtitle)
//	タイプ：public
//	機能　：行題の変更
//	引数１：変更する行
//	引数２：変更後の行題
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::ChangeRowTitle(long rowNo, char* newtitle)
{
	if( !TGridData::ChangeRowTitle(rowNo, newtitle)) return(FALSE);

	if( !m_info.rowtitle) return(FALSE);

	// 対象のデータ行が表示対象領域内に入っていたら再描画する
	if( rowNo >= m_Top + m_DispRows-m_FixRows || (rowNo < m_Top && rowNo >=m_FixRows) )return(TRUE);// 対象外

	// 描画する
	int disprow;
	RECT rect;
	char txtBuf[256];

	// 画面上のどの行か？
	disprow = rowNo-m_Top+m_FixRows;
	// 固定行
	if( rowNo < m_FixRows) disprow = rowNo;


	SetRect(&rect	,3
			,m_ColHeight*(disprow+1)+3
			,m_info.rowtitleWidth-2
			,m_ColHeight*(disprow+2)-2);
	wsprintf(txtBuf,"%s",TGridData::m_StrData[rowNo].colTitle);

	FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	SetTextColor( m_hdcList, CLR_BLACK);
	DrawText(m_hdcList,txtBuf,-1,&rect, DT_CENTER|DT_SINGLELINE);

	InvalidateRect(m_ListWnd,&rect,TRUE);

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::SetCel(int col,int row, char* data,long cTag,BOOL draw,char color,char bkcolor)
//	タイプ：public
//	機能　：セルにデータを設定
//	引数１：設定する列番号
//	引数２：設定する行番号
//	引数３：設定データ
//	引数４：設定タグデータ
//	引数５：画面に反映＝TRUE,反映なし=FALSE
//	引数６：設定
//	引数７：セルの背景色 0=白、1=無効色 -1=変更なし（前の色を使用する）
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetCel(int col,int row, char* data,long cTag,BOOL draw,char color,char bkcolor)
{
	// データをセット
	if(!TGridData::SetCelData(col,row,color,cTag,data,bkcolor)) return(FALSE);

	// リストに書き込む
	if(draw)
		if( !DrawCel(col,row,FALSE) ) return(FALSE);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::DrawCel(int col, int row,BOOL act)
//	タイプ：private
//	機能　：セルにデータを描画
//	引数１：設定するデータ列番号
//	引数２：設定するデータ行番号
//	引数３：アクティブセル＝TRUE それ以外=FALSE(デフォルト）
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::DrawCel(int col, int row,BOOL act)
{
	int disprow;
	RECT rect;
	char txtBuf[256];
	UINT uFlags;
	COLORREF old;
	int xPos,i;

	SetTextColor( m_hdcList, CLR_BLACK);

	// 描画できない
	if( col < 0 || row < 0 || TGridData::m_LastRowNo < 0) return(FALSE);

	// 対象のデータ行が表示対象領域内に入っているか？
	if( row >= m_Top + m_DispRows-m_FixRows || (row < m_Top && row >=m_FixRows) )return(FALSE);
	if( col >= m_FixCols && (col < m_Left || col > m_Right+1) ) return(FALSE);

	// 描画する
	// 画面上のどの行か？
	disprow = row-m_Top+m_FixRows;
	// 固定行
	if( row < m_FixRows) disprow = row;

	// 画面上の列の座標を取る
	xPos = m_info.rowtitleWidth;
/// 固定列
	if( col < m_FixCols ){
		for( i= 0; i < col; i++)
			xPos += m_ColInfo[i].width;
	}
///	スクロール列
	else{
		for( i= 0; i < m_FixCols; i++)
			xPos += m_ColInfo[i].width;
		for( i= m_Left; i < col; i++)
			xPos += m_ColInfo[i].width;
	}
///


	// 前のデータを塗りつぶす(線は消さない)
	SetRect(&rect	,xPos+1			// 行ヘッダの幅offset
					,m_ColHeight*(disprow+1)+1
					,m_ColInfo[col].width+xPos
					,m_ColHeight*(disprow+2));

	// アクティブセル
	if( act ){
		HBRUSH hbrush=CreateSolidBrush(CLR_BLUE);
		FillRect( m_hdcList,&rect,hbrush);
		DeleteObject(hbrush);
	}
	// 通常のセル
	else if(!TGridData::m_StrData[row].bkcolor[col]) FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));

	// 濃い灰色
	else FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(GRAY_BRUSH));

	// 領域内に収まる文字数だけ出力
	SetRect(&rect	,xPos+2
					,m_ColHeight*(disprow+1)+2
					,xPos+m_ColInfo[col].width-2
					,m_ColHeight*(disprow+2)-2);
	wsprintf( txtBuf,"%s",TGridData::m_StrData[row].d[col] );

	switch(m_ColInfo[col].position){
		case POS_CENTER:
//			uFlags = DT_CENTER|DT_SINGLELINE;
			uFlags = DT_CENTER|DT_WORDBREAK;
			break;
		case POS_RIGHT:
			uFlags = DT_RIGHT|DT_SINGLELINE;
			break;
		default:
			uFlags = DT_LEFT|DT_SINGLELINE;
			break;
	}

	// アクティブセル
	if( act ){
		old = SetTextColor( m_hdcList, CLR_WHITE );
		DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);
		SetTextColor( m_hdcList, old );
	}
	else{
	// 色つき
		if(TGridData::m_StrData[row].color[col]==1){
			old = SetTextColor( m_hdcList, CLR_RED );
		}
		DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);

		if(TGridData::m_StrData[row].color[col]==1){
			SetTextColor( m_hdcList, old );
		}
	}
	SetRect(&rect	,xPos+1			// 行ヘッダの幅offset
					,m_ColHeight*(disprow+1)+1
					,m_ColInfo[col].width+xPos+1
					,m_ColHeight*(disprow+2));

	HPEN	hOldpen,hPen;				// Handle of pen
	hPen   = CreatePen( PS_SOLID ,1, CLR_LGRAY);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	MoveToEx( m_hdcList , xPos+m_ColInfo[col].width, m_ColHeight*(disprow+1) , NULL );
	LineTo(   m_hdcList , xPos+m_ColInfo[col].width, m_ColHeight*(disprow+2));
	hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
	DeleteObject( hPen );
	// 更新
	InvalidateRect(m_ListWnd,&rect,TRUE);
	return(TRUE);
}

BOOL CGridViewCtrl::LostFocusCel(int col, int row )
{
	int disprow;
	RECT rect;
	char txtBuf[256];
	UINT uFlags;
	int xPos,i;
	HPEN	hOldpen,hPen;				// Handle of pen
	COLORREF old;

	SetTextColor( m_hdcList, CLR_BLACK);

	// 描画できない
	if( col < 0 || row < 0 || TGridData::m_LastRowNo < 0) return(FALSE);


	// 対象のデータ行が表示対象領域内に入っているか？
	if( row >= m_Top + m_DispRows-m_FixRows || (row < m_Top && row >=m_FixRows)) return(FALSE);
/// 固定行
	if( col >= m_FixCols && (col < m_Left || col > m_Right+1) ) return(FALSE);
///	if( col < m_Left || col > m_Right+1 ) return(FALSE);
///

	// 描画する
	// 画面上のどの行か？
	disprow = row-m_Top+m_FixRows;
	// 固定行
	if( row < m_FixRows) disprow = row;

	// 画面上の列の座標を取る
	xPos = m_info.rowtitleWidth;
/// 固定列
	if( col < m_FixCols ){
		for( i= 0; i < col; i++)
			xPos += m_ColInfo[i].width;
	}
/// スクロール列
	else{
		for( i= 0; i < m_FixCols; i++)
			xPos += m_ColInfo[i].width;
		for( i= m_Left; i < col; i++)
			xPos += m_ColInfo[i].width;
	}
///


	// 前のデータを塗りつぶす(線は消さない)
	SetRect(&rect	,xPos+1			// 行ヘッダの幅offset
					,m_ColHeight*(disprow+1)+1
					,m_ColInfo[col].width+xPos
					,m_ColHeight*(disprow+2));

	if(!TGridData::m_StrData[row].bkcolor[col]) FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));
	else FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(GRAY_BRUSH));


	// 領域内に収まる文字数だけ出力
	SetRect(&rect	,xPos+2
					,m_ColHeight*(disprow+1)+2
					,xPos+m_ColInfo[col].width-2
					,m_ColHeight*(disprow+2)-2);
	wsprintf( txtBuf,"%s",TGridData::m_StrData[row].d[col] );

	switch(m_ColInfo[col].position){
		case POS_CENTER:
//			uFlags = DT_CENTER|DT_SINGLELINE;
			uFlags = DT_CENTER|DT_WORDBREAK;
			break;
		case POS_RIGHT:
			uFlags = DT_RIGHT|DT_SINGLELINE;
			break;
		default:
			uFlags = DT_LEFT|DT_SINGLELINE;
			break;
	}

	// 色つき
	if(TGridData::m_StrData[row].color[col]==1){
		old = SetTextColor( m_hdcList, CLR_RED );
	}

	DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);

	// 色つき
	if(TGridData::m_StrData[row].color[col]==1){
		old = SetTextColor( m_hdcList, CLR_RED );
	}

	SetRect(&rect	,xPos+1			// 行ヘッダの幅offset
					,m_ColHeight*(disprow+1)+1
					,m_ColInfo[col].width+xPos
					,m_ColHeight*(disprow+2));


	hPen   = CreatePen( PS_SOLID ,1, CLR_BLUE);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	MoveToEx( m_hdcList , rect.left,  rect.top , NULL );
	LineTo(   m_hdcList , rect.right-1,rect.top);
	MoveToEx( m_hdcList , rect.left,  rect.top , NULL );
	LineTo(   m_hdcList , rect.left,rect.bottom-1 );

	MoveToEx( m_hdcList , rect.right-1,  rect.top , NULL );
	LineTo(   m_hdcList , rect.right-1,	rect.bottom-1 );
	MoveToEx( m_hdcList , rect.left,  	rect.bottom-1 , NULL );
	LineTo(   m_hdcList , rect.right-1,	rect.bottom-1 );

	hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
	DeleteObject( hPen );

	// 更新
	InvalidateRect(m_ListWnd,&rect,TRUE);
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::DrawCel(int col, int row,BOOL act)
//	タイプ：private
//	機能　：クライアント領域全セル描画
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::DrawData( void )
{
	COLORREF old;
	int col ,datrow,disprow;
	RECT rect;
	char txtBuf[256];
	UINT uFlags;
	int xPos;

	int count,st,ed;

	SetTextColor( m_hdcList, CLR_BLACK);

	// 線などを再描画
	DrawHeadder(FALSE);

	// 表示先頭行からデータを表示する
	// 1順目は固定行、２順目は可変行
	for(count=0;count<2; count++ ){
		// 固定
		if(count == 0 ){
			st=0;
			ed= m_FixRows;
			datrow = 0;
		}
		// 可変
		else{
			datrow = m_Top;
			st = m_FixRows;
			ed = m_DispRows;
		}
		for( disprow=st; disprow< ed; disprow++){
			
			// データがないとき
			if( TGridData::m_LastRowNo < 0 ) break;
			// 行ヘッダ
			if( m_info.rowtitle){
				SetRect(&rect	,3
								,m_ColHeight*(disprow+1)+3
								,m_info.rowtitleWidth-2
								,m_ColHeight*(disprow+2)-2);
				wsprintf(txtBuf,"%s",TGridData::m_StrData[datrow].colTitle);
				DrawText(m_hdcList,txtBuf,-1,&rect, DT_CENTER|DT_SINGLELINE);
			}

			// 固定列
			xPos = 0;
			for( col = 0; col <m_FixCols; col++ ){
				if( col > m_Right+1) break;
				// 前のデータを塗りつぶす(線は消さない)
				SetRect(&rect	,xPos+1+m_info.rowtitleWidth
								,m_ColHeight*(disprow+1)+1
								,xPos+m_ColInfo[col].width+m_info.rowtitleWidth
								,m_ColHeight*(disprow+2));
	
				// アクティブセルの時だけ別処理(選択モード＝セルの場合）
				if( (col == m_ActCel.col && datrow ==m_ActCel.row && !m_info.selecttype) ||
				// アクティブセルの時だけ別処理(選択モード＝行の場合）
					(datrow ==m_ActCel.row && m_info.selecttype && m_ActCel.col >=0) )
				{
					HBRUSH hbrush=CreateSolidBrush(CLR_BLUE);
					FillRect( m_hdcList,&rect,hbrush);
					DeleteObject(hbrush);
				}
//				else if(!TGridData::m_StrData[datrow].bkcolor[col]) FillRect( m_hdcList,&rect,GetStockObject(WHITE_BRUSH));
				else if(!TGridData::m_StrData[datrow].bkcolor[col]);
				else FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(GRAY_BRUSH));

				// 領域内に収まる文字数だけ出力
				SetRect(&rect	,xPos+2+m_info.rowtitleWidth
								,m_ColHeight*(disprow+1)+2
								,xPos+m_ColInfo[col].width-2+m_info.rowtitleWidth
								,m_ColHeight*(disprow+2)-2);
				wsprintf( txtBuf,"%s",TGridData::m_StrData[datrow].d[col] );

				switch(m_ColInfo[col].position){
					case POS_CENTER:
//						uFlags = DT_CENTER|DT_SINGLELINE|DT_WORDBREAK;
						uFlags = DT_CENTER|DT_WORDBREAK;
						break;
					case POS_RIGHT:
						uFlags = DT_RIGHT|DT_SINGLELINE;
						break;
					default:
						uFlags = DT_LEFT|DT_SINGLELINE;
						break;
				}

				// アクティブセルの時だけ別処理(選択モード＝セルの場合）
				if( (col == m_ActCel.col && datrow ==m_ActCel.row && !m_info.selecttype) ||
				// アクティブセルの時だけ別処理(選択モード＝行の場合）
					(datrow ==m_ActCel.row && m_info.selecttype && m_ActCel.col >=0) )
				{
					old = SetTextColor( m_hdcList, CLR_WHITE );
					DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);
					SetTextColor( m_hdcList, old );
				}
				// 通常のセル
				else{
					// 色つき
					if(TGridData::m_StrData[datrow].color[col]==1){
						old = SetTextColor( m_hdcList, CLR_RED );
					}
					DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);
					if(TGridData::m_StrData[datrow].color[col]==1){
						SetTextColor( m_hdcList, old );
					}
				}
				//次のx座標
				xPos += m_ColInfo[col].width;
			}
			for( col = m_Left; col <TGridData::m_TotalCols; col++ ){
				if( col > m_Right+1) break;
				// 前のデータを塗りつぶす(線は消さない)
				SetRect(&rect	,xPos+1+m_info.rowtitleWidth
								,m_ColHeight*(disprow+1)+1
								,xPos+m_ColInfo[col].width+m_info.rowtitleWidth
								,m_ColHeight*(disprow+2));
	
				// アクティブセルの時だけ別処理(選択モード＝セルの場合）
				if( (col == m_ActCel.col && datrow ==m_ActCel.row && !m_info.selecttype) ||
				// アクティブセルの時だけ別処理(選択モード＝行の場合）
					(datrow ==m_ActCel.row && m_info.selecttype && m_ActCel.col >=0) )
				{
					HBRUSH hbrush=CreateSolidBrush(CLR_BLUE);
					FillRect( m_hdcList,&rect,hbrush);
					DeleteObject(hbrush);
				}
//				else if(!TGridData::m_StrData[datrow].bkcolor[col]) FillRect( m_hdcList,&rect,GetStockObject(WHITE_BRUSH));
				else if(!TGridData::m_StrData[datrow].bkcolor[col]);
				else FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(GRAY_BRUSH));

				// 領域内に収まる文字数だけ出力
				SetRect(&rect	,xPos+2+m_info.rowtitleWidth
								,m_ColHeight*(disprow+1)+2
								,xPos+m_ColInfo[col].width-2+m_info.rowtitleWidth
								,m_ColHeight*(disprow+2)-2);
				wsprintf( txtBuf,"%s",TGridData::m_StrData[datrow].d[col] );

				switch(m_ColInfo[col].position){
					case POS_CENTER:
//						uFlags = DT_CENTER|DT_SINGLELINE|DT_WORDBREAK;
						uFlags = DT_CENTER|DT_WORDBREAK;
						break;
					case POS_RIGHT:
						uFlags = DT_RIGHT|DT_SINGLELINE;
						break;
					default:
						uFlags = DT_LEFT|DT_SINGLELINE;
						break;
				}

				// アクティブセルの時だけ別処理(選択モード＝セルの場合）
				if( (col == m_ActCel.col && datrow ==m_ActCel.row && !m_info.selecttype) ||
				// アクティブセルの時だけ別処理(選択モード＝行の場合）
					(datrow ==m_ActCel.row && m_info.selecttype && m_ActCel.col >=0) )
				{
					old = SetTextColor( m_hdcList, CLR_WHITE );
					DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);
					SetTextColor( m_hdcList, old );
				}
				// 通常のセル
				else{
					// 色つき
					if(TGridData::m_StrData[datrow].color[col]==1){
						old = SetTextColor( m_hdcList, CLR_RED );
					}
					DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);
					if(TGridData::m_StrData[datrow].color[col]==1){
						SetTextColor( m_hdcList, old );
					}
				}
				//次のx座標
				xPos += m_ColInfo[col].width;
			}
			// データ最終行までいったらおわり
			if( datrow >= TGridData::m_LastRowNo ) break;
			// データ行を増やす
			datrow++;
		}
	}
	if( m_info.vScroll )
		SetVScroll();
	if( m_info.hScroll )
		SetHScroll();

	// 全更新
	SetRect(&rect,0,0,m_ListRect.right,m_ListRect.bottom);
	InvalidateRect(m_ListWnd,&rect,TRUE);
//	SetFocus(m_ListWnd);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::SetVScroll(void)
//	タイプ：private
//	機能　：垂直スクロールバーの設定
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetVScroll(void)
{
	 if(!m_WindowInit) return(FALSE);

	SCROLLINFO sinfo;

	// データ行数が表示可能行数以下の時はスクロールバーを使用不可にする
	if( TGridData::m_LastRowNo < m_DispRows ){
		EnableScrollBar(m_ListWnd,SB_VERT,ESB_DISABLE_BOTH);
		return(TRUE);
	}


	// 現在の設定を読み込む
	sinfo.cbSize=sizeof(SCROLLINFO);
	sinfo.fMask = SIF_PAGE|SIF_RANGE;
	GetScrollInfo( m_ListWnd, SB_VERT,&sinfo );

	// 設定する
	sinfo.nMin = 0;
	sinfo.nMax = TGridData::m_LastRowNo-m_DispRows+1;
	sinfo.nPage = 1;
	SetScrollInfo( m_ListWnd, SB_VERT,&sinfo ,TRUE);
	EnableScrollBar(m_ListWnd,SB_VERT,ESB_ENABLE_BOTH);
	return(TRUE);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::SetVScroll(void)
//	タイプ：private
//	機能　：垂直スクロールバーの設定
//	戻り値：成功＝TRUE
//		*Create時又は列数を変化させたとき以外からの呼び出しはしてはいけません
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetHScroll(void)
{
	 if(!m_WindowInit) return(FALSE);

	int i;
	SCROLLINFO sinfo;
	long xTotal=0;


	// 列幅が表示可能幅より少ない場合はスクロールバーを使用不可にする
	// 但し左端の列が０番の時のみチェック
	if( m_Left == m_FixCols ){
//	if( m_Left == 0 ){
		for(i=0;i<TGridData::m_TotalCols;i++){
			xTotal += m_ColInfo[i].width;
		}
		xTotal+= m_info.rowtitleWidth;

		//  幅合計 <= クライアント領域合計
		if( xTotal <= m_ListRect.right ){
			EnableScrollBar(m_ListWnd,SB_HORZ,ESB_DISABLE_BOTH);
			return(TRUE);
		}
	}


	// 現在の設定を読み込む
	sinfo.cbSize=sizeof(SCROLLINFO);
	sinfo.fMask = SIF_PAGE|SIF_RANGE;
	GetScrollInfo( m_ListWnd, SB_HORZ,&sinfo );

	// 設定する
	sinfo.nMin = 0;
	sinfo.nMax = TGridData::m_TotalCols-1;
	sinfo.nPage = 1;
	SetScrollInfo( m_ListWnd, SB_HORZ,&sinfo ,TRUE);
	EnableScrollBar(m_ListWnd,SB_HORZ,ESB_ENABLE_BOTH);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	void CGridViewCtrl::EvVScroll(HWND hWnd,WORD event,WORD lastPos)
//	タイプ：private
//	機能　：垂直スクロールバーの制御
//	引数１：ウインドウハンドル
//	引数２：発生したイベント
//	引数３：つまみをはなした時のつまみの位置
//---------------------------------------------------------------------------------------------------------------------
void CGridViewCtrl::EvVScroll(HWND hWnd,WORD event,WORD lastPos)
{
	long nextTopRow;			// スクロール後に先頭に来る行

	nextTopRow = m_Top-m_FixRows;


	switch(event){
		case SB_LINEDOWN:
		case SB_PAGEDOWN:
			nextTopRow++;
			break;
		case SB_LINEUP:
		case SB_PAGEUP:
			nextTopRow--;
			break;
		case SB_THUMBPOSITION:
			nextTopRow = lastPos;
			break;
		default:
			return;
	}

	// スクロールが必要か？
	if( nextTopRow < 0)		return;


	if(nextTopRow+m_DispRows-1 >TGridData::m_LastRowNo )
		nextTopRow = TGridData::m_LastRowNo -m_DispRows+1;

	SCROLLINFO sinfo;

	// スクロール情報を取得
	sinfo.cbSize=sizeof(SCROLLINFO);
	sinfo.fMask = SIF_POS;
	GetScrollInfo(hWnd, SB_VERT,&sinfo );

	// スクロール位置が変更ない場合
	if( sinfo.nPos == nextTopRow && event !=SB_THUMBPOSITION) return;

	// 編集中の時は終わらせる
	EndEdit(1);

	sinfo.nPos = nextTopRow;
	SetScrollInfo( hWnd, SB_VERT,&sinfo ,TRUE);
	m_Top = nextTopRow + m_FixRows;


	// 縦スクロールすることによって、アクティブセルが表示できなくなったら、アクティブセルの行を変更
	if( m_ActCel.row < m_Top ) m_ActCel.row = m_Top;
	else if( m_ActCel.row > m_Top+m_DispRows-1-m_FixRows ) m_ActCel.row = m_Top+m_DispRows-m_FixRows-1;
	DrawData();
	SetFocus(m_ListWnd);


}

//---------------------------------------------------------------------------------------------------------------------
//	void CGridViewCtrl::EvHScroll(HWND hWnd,WORD event,WORD lastPos)
//	タイプ：private
//	機能　：水平スクロールバーの制御
//	引数１：ウインドウハンドル
//	引数２：発生したイベント
//	引数３：つまみをはなした時のつまみの位置
//---------------------------------------------------------------------------------------------------------------------
void CGridViewCtrl::EvHScroll(HWND hWnd,WORD event,WORD lastPos)
{
	SetFocus(m_ListWnd);


	SCROLLINFO sinfo;
	long nextPos;
	nextPos = m_Left;
	int i;

	switch(event){
		case SB_LINELEFT:
		case SB_PAGELEFT:
			nextPos--;
			break;
		case SB_LINERIGHT:
		case SB_PAGERIGHT:
			nextPos++;
			break;
		case SB_THUMBPOSITION:
			nextPos = lastPos;
			break;
		default:
			return;
	}

	if( nextPos <m_FixCols ) nextPos = m_FixCols;
//	if( nextPos <0 ) nextPos = 0;
	if( nextPos >=TGridData::m_TotalCols ) nextPos = TGridData::m_TotalCols-1;

	// スクロール情報を取得
	sinfo.cbSize=sizeof(SCROLLINFO);
	sinfo.fMask = SIF_POS;
	GetScrollInfo(hWnd, SB_HORZ,&sinfo );


	// スクロール位置が変更ない場合
	if( sinfo.nPos == nextPos && event !=SB_THUMBPOSITION) return;

	// 編集中の時は終わらせる
	EndEdit(1);

	sinfo.nPos = nextPos;
	SetScrollInfo( hWnd, SB_HORZ,&sinfo ,TRUE);
	m_Left = nextPos;

	// 右の列番号を取得
	nextPos =0;
	for( i = 0; i < m_FixCols; i++ )
		nextPos += m_ColInfo[i].width;
	for( i = m_Left; i < TGridData::m_TotalCols; i++ ){
		nextPos += m_ColInfo[i].width;

		// 表示可能域を越えたらbreak
		if( nextPos + m_info.rowtitleWidth >= m_ListRect.right ){
			break;
		}
		// ここで現在の右列を取得する
		m_Right = i;
	}

	// 横スクロールすることによって、アクティブセルが表示できなくなったら、アクティブセルの行を変更
	if( m_ActCel.col < m_Left ) m_ActCel.col = m_Left;
	else if( m_ActCel.col > m_Right ) m_ActCel.col = m_Right;
	DrawData();


	switch(event){
		case SB_LINELEFT:
		case SB_PAGELEFT:
		case SB_LINERIGHT:
		case SB_PAGERIGHT:
		case SB_THUMBPOSITION:
			EvMove(m_Left,m_Top);
			break;
		default:
			break;
	}

	SetFocus(m_ListWnd);

}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::MoveActiveCel(long newCol,long newRow)
//	タイプ：private
//	機能　：アクティブなセルを変更する
//	引数１：アクティブにするセルの列
//	引数２：アクティブにするセルの行
//	引数３：内部呼び出しｻｲﾝ(省略可),FALSEで使用してね
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::MoveActiveCel(long newCol,long newRow,BOOL internal)
{
	long i;

	// 移動先のセルは有効か？
	if( newRow > TGridData::m_LastRowNo || newCol >=TGridData::m_TotalCols ||
		newRow < m_FixRows )
//		newRow < m_FixRows || newCol < m_FixCols )
		return(FALSE);

	// 移動先が画面に入ってい無い場合はスクロール
	if( ( newCol >=m_FixCols && newCol < m_Left) || newCol > m_Right || 
		(( TGridData::m_LastRowNo >= m_DispRows ) && ( newRow < m_Top || newRow >= m_Top + m_DispRows-m_FixRows )) )
	{
		// 移動失敗
		if( !AutoScroll(newCol,newRow) ) return(FALSE);
		// 成功したら、再描画
		// アクティブセル用のメンバ変数を更新
		if( newCol >= m_FixCols ) m_ActCel.col = newCol;		// 固定列 04.06.21
		m_ActCel.row = newRow;
		DrawData();
		if( internal) EvMove(m_Left,m_Top);
	}

	// スクロールしない場合
	else{
		// 直前のアクティブセルの色を元に戻す
		// セル選択モード
		if( !m_info.selecttype )
			DrawCel(m_ActCel.col,m_ActCel.row);

		// 行選択モード
		else{
			for(i=0; i< m_FixCols;i++)
				DrawCel(i,m_ActCel.row);
			for(i=m_Left; i<=m_Right+2;i++)
				DrawCel(i,m_ActCel.row);
		}

		// アクティブセル用のメンバ変数を更新
		if( newCol >= m_FixCols ) m_ActCel.col = newCol;		// 固定列 04.06.21
//		m_ActCel.col = newCol;
		m_ActCel.row = newRow;

		// 反転で表示
		// セル選択モード
		if( !m_info.selecttype )
			DrawCel(m_ActCel.col,m_ActCel.row,TRUE);
		// 行選択モード
		else{
			for(i=0; i<m_FixCols;i++)	DrawCel(i,m_ActCel.row,TRUE);
			for(i=m_Left; i<=m_Right+2;i++)	DrawCel(i,m_ActCel.row,TRUE);
		}
	}

	// 移動完了関数
	EvMoveCelEnd( m_ActCel.col, m_ActCel.row);

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	void CGridViewCtrl::KillActCel(void)
//	タイプ：public
//	機能　：現在アクティブなセルの表示を、フォーカスを失った状態に変更
//---------------------------------------------------------------------------------------------------------------------
void CGridViewCtrl::KillActCel(void)
{
	int x;

	if( m_info.selecttype){					// 選択方式　０＝セル単位、１＝行単位
		for(x=0;x<TGridData::m_TotalCols; x++)
			LostFocusCel(x,m_ActCel.row);
	}
	else LostFocusCel(m_ActCel.col,m_ActCel.row);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVieCtrl::SetLeftX(long newX)
//	タイプ：public
//	機能　：左のセル番号を、指定したセルに変更する
//	引数１：アクティブにするセルの列
//	引数２：アクティブにするセルの行
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetLeftX(long newCol)
{
	// 移動先のセルは有効か？
	if( newCol >=TGridData::m_TotalCols || newCol < m_FixCols )
		return(FALSE);

	if( newCol == m_Left ) return(TRUE);

	// 編集中の時は終わらせる
	EndEdit(1);

	SCROLLINFO sinfo;
	BOOL redraw=FALSE;					// 再描画サイン

	// ---------------------------------------------水平位置--------------------------------------
	// 再描画時の、左セル番号を算出
	if( !m_info.selecttype){
		// 横スクロールが使用不可の場合は移動させない
		if( !m_info.hScroll ) return(FALSE);

		// 現在の左位置を変更
		m_Left = newCol;

		// スクロールバーの設定
		sinfo.cbSize=sizeof(SCROLLINFO);
		sinfo.fMask = SIF_POS;
		sinfo.nPos = m_Left;
		SetScrollInfo( m_ListWnd, SB_HORZ,&sinfo ,TRUE);

	}

	if( m_ActCel.col <m_Left || m_ActCel.col > m_Right)
		m_ActCel.col = newCol;

	DrawData();
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::AutoScroll(long newCol,long newRow)
//	タイプ：private
//	機能　：指定のセルが画面に出るように自動スクロール
//	引数１：アクティブにするセルの列
//	引数２：アクティブにするセルの行
// この後にDrawData()を実行しないと、再描画はされません
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::AutoScroll(long newCol,long newRow)
{
	// 移動先のセルは有効か？
	if( newRow > TGridData::m_LastRowNo || newCol >=TGridData::m_TotalCols ||
		newRow < m_FixRows || newCol < m_FixCols )
		return(FALSE);

	long xTotal;
	int i;
	SCROLLINFO sinfo;
	BOOL redraw=FALSE;					// 再描画サイン
	// ---------------------------------------------水平位置--------------------------------------
	// 再描画時の、左セル番号を算出
	if( (newCol < m_Left || newCol > m_Right ) &&  !m_info.selecttype){
		// 横スクロールが使用不可の場合は移動させない
		if( !m_info.hScroll ) return(FALSE);

		// 現在の左位置より小さい
		if( newCol < m_Left ) m_Left = newCol;
		if( newCol < m_FixCols ) m_Left = m_FixCols;

		// 現在の右位置より大きい
		if( newCol > m_Right ){
			m_Right = newCol;
			xTotal = m_info.rowtitleWidth;
			// 固定行
			for( i = 0; i <m_FixCols; i++ )
				xTotal += m_ColInfo[i].width;
			for( i = m_Right; i >=0; i-- ){
				xTotal += m_ColInfo[i].width;
				if(xTotal >=m_ListRect.right) break;
			}
			m_Left = i+1;

		}
		// スクロールバーの設定
		sinfo.cbSize=sizeof(SCROLLINFO);
		sinfo.fMask = SIF_POS;
		sinfo.nPos = m_Left;
		SetScrollInfo( m_ListWnd, SB_HORZ,&sinfo ,TRUE);

	}

	// ---------------------------------------------垂直位置--------------------------------------
	if( ( TGridData::m_LastRowNo >= m_DispRows ) &&
		( (newRow < m_Top && newRow >=m_FixRows) || newRow >= m_Top + m_DispRows - m_FixRows)){
		// 縦スクロール不可の時は何もしない
		if( !m_info.vScroll ) return(FALSE);

		if( newRow < m_Top ) m_Top = newRow;
		else m_Top = newRow - m_DispRows + m_FixRows + 1;

		// スクロールバーを移動
		SCROLLINFO sinfo;

		// スクロール情報を取得
		sinfo.cbSize=sizeof(SCROLLINFO);
		sinfo.fMask = SIF_POS;
		sinfo.nPos = m_Top;
		SetScrollInfo( m_ListWnd, SB_VERT,&sinfo ,TRUE);
	}

	// スクロールOK
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::MoveCel(int mvMode)
//	タイプ：public
//	機能　：アクティブセルの一こま移動
//	引数１：移動モード
//			 MV_UP 0
//			 MV_DOWN 1
//			 MV_LEFT 2
//			 MV_RIGHT 3
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::MoveCel(int mvMode)
{
	long nextCol,nextRow;

	GetActiveCel(&nextCol,&nextRow);

	switch(mvMode){
			 case MV_UP:
				nextRow--;
				// 先頭より前の行になったら、前の列の最後尾にする
				if( nextRow < m_FixRows ){
					nextRow = TGridData::m_LastRowNo;
					nextCol --;
					if( nextCol < m_FixCols )	nextCol = m_FixCols;
//					if( nextCol < 0 )	nextCol = 0;
				}
				break;

			case MV_DOWN:
				nextRow++;
				// 先頭より後の行になったら、次の列の先頭にする
				if( nextRow > TGridData::m_LastRowNo ){
					nextRow = m_FixRows;
					nextCol++;
					if( nextCol >= TGridData::m_TotalCols )	nextCol = TGridData::m_TotalCols-1;
				}
				break;

			 case MV_LEFT:
				nextCol--;
				if( nextCol < m_FixCols ) return(FALSE);
				break;

			 case MV_RIGHT:
				nextCol++;
				if( nextCol >= TGridData::m_TotalCols )	nextCol = TGridData::m_TotalCols-1;
				break;
		default:
			return(FALSE);

	}
	return(MoveActiveCel(nextCol,nextRow,TRUE));
}



//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::ColsCopy(long targetCol)
//	タイプ：public
//	機能　：指定列のデータを全てバッファにコピーする
//	引数１：コピー対象の列番号
//	戻り値：成功＝TRUE、失敗＝FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::ColsCopy(long targetCol)
{
	// データ域のコピー
	if( !TGridData::ColCopy(targetCol,FALSE) ) return(FALSE);

	// 列情報をコピー
	m_ColInfo[TGridData::m_TotalCols].width=m_ColInfo[targetCol].width;
	m_ColInfo[TGridData::m_TotalCols].position=m_ColInfo[targetCol].position;
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::ColsPaste(long targetCol,BOOL insert,BOOL redraw)
//	タイプ：public
//	機能　：指定列のデータを全てバッファにコピーする
//	引数１：コピー対象の列番号
//	引数２：挿入=TRUE 上書き=FALSE
//	引数３：再描画指定 TRUE=再描画する
//	戻り値：成功＝TRUE、失敗＝FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::ColsPaste(long targetCol,BOOL insert,BOOL redraw)
{
	long ret;
	long prevCols;
	int offset,i;
	COLUMNPOSITIONINFO *info;

	// 張り付け前の列数
	prevCols = TGridData::m_TotalCols;
	// データ域の張り付け
	if( ( ret =TGridData::ColPaste(targetCol,insert)) <0 ) return(FALSE);

	// 列がないときの処理
	if( targetCol < 0 ) targetCol = 0;
	if( prevCols <= 0){
		prevCols = 1;
		targetCol = 0;
		insert = FALSE;
	}

	// 列情報
	// 上書きorずらす必要なし
	if( targetCol > prevCols || !insert){
		if( targetCol > prevCols )
				targetCol = prevCols;

		// コピーする
		m_ColInfo[targetCol].width=m_ColInfo[TGridData::m_TotalCols].width;
		m_ColInfo[targetCol].position=m_ColInfo[TGridData::m_TotalCols].position;
	}

	// 挿入
	else{
		// ずらす場合（列MAX）
		if( prevCols == TGridData::m_TotalCols ) offset =1;

		// !列max
		else	offset = 0;

		info = (COLUMNPOSITIONINFO*)realloc(m_ColInfo,sizeof(COLUMNPOSITIONINFO) * (m_TotalCols+1));
		m_ColInfo=info;
		for(i=TGridData::m_TotalCols;i>targetCol;i--){
			m_ColInfo[i].width=m_ColInfo[i-1].width;
			m_ColInfo[i].position=m_ColInfo[i-1].position;
		}
	//	memmove( &m_ColInfo[targetCol+1], &m_ColInfo[targetCol],	( prevCols-targetCol-offset) * sizeof(COLUMNPOSITIONINFO) );
//		memcpy( &m_ColInfo[targetCol], &m_ColInfo[TGridData::m_TotalCols],    sizeof(COLUMNPOSITIONINFO) );
		m_ColInfo[targetCol].width=m_ColInfo[TGridData::m_TotalCols].width;
		m_ColInfo[targetCol].position=m_ColInfo[TGridData::m_TotalCols].position;
	}

	// 列数に変化があったときはスクロールバーを更新
	if( TGridData::m_TotalCols != prevCols )
		SetHScroll();

	// 再描画指定あり
	if( redraw)
		DrawData();

	return(TRUE);

}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::ColsCut(long targetCol ,BOOL redraw)
//	タイプ：public
//	機能　：指定列のデータを削除し、コピーバッファに保存する
//	引数１：削除対象の列番号
//	引数３：再描画指定 TRUE=再描画する
//	戻り値：成功＝TRUE、失敗＝FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::ColsCut(long targetCol ,BOOL redraw)
{
	long prevCols;
	COLUMNPOSITIONINFO *info;
	int i;

	// 張り付け前の列数
	prevCols = TGridData::m_TotalCols;

	// データ域のカット
	if( !TGridData::ColCopy(targetCol,TRUE) ) return(FALSE);

	// 列情報をコピー
	m_ColInfo[prevCols].width=m_ColInfo[targetCol].width;
	m_ColInfo[prevCols].position=m_ColInfo[targetCol].position;

	// カット

	for(i=targetCol;i<TGridData::m_TotalCols-1;i++){
		m_ColInfo[i].width=m_ColInfo[i+1].width;
		m_ColInfo[i].position=m_ColInfo[i+1].position;
	}

//	memmove( &m_ColInfo[targetCol], &m_ColInfo[targetCol+1] ,( prevCols-targetCol-1) * sizeof(COLUMNPOSITIONINFO) );
	info = (COLUMNPOSITIONINFO*)realloc(m_ColInfo,sizeof(COLUMNPOSITIONINFO) * (m_TotalCols+1));
	m_ColInfo=info;

	// アクティブセルに対する処置
	if( m_ActCel.col >= TGridData::m_TotalCols )
		m_ActCel.col=TGridData::m_TotalCols-1;

	// スクロールbar
	SetHScroll();

	// 再描画指定あり
	if( redraw)
		DrawData();


	return(TRUE);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::DataClear( BOOL redraw )
//	タイプ：public
//	機能　：現在保持している全データをクリアする
//	引数１：再描画指定 TRUE=再描画する
//	戻り値：TRUE=クリア実施
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::DataClear( BOOL redraw )
{
	if( !TGridData::DataClear() ) return(FALSE);

	m_Top = 0;
/*
	m_ActCel.col = -1;
	m_ActCel.row = m_FixRows;
*/
	SCROLLINFO sinfo;

	// スクロール情報を取得
	sinfo.cbSize=sizeof(SCROLLINFO);
	sinfo.fMask = SIF_POS;
	GetScrollInfo(m_ListWnd , SB_VERT,&sinfo );

	sinfo.nPos = 0;
	SetScrollInfo( m_ListWnd , SB_VERT,&sinfo ,TRUE);
	m_Top = m_FixRows;
	if(redraw)
		DrawData();

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::AddNewCol(long targetCol,long width, short pos,BOOL redraw)
//	タイプ：public
//	機能　：新規に列を挿入する
//	引数１：挿入する列番号
//	引数１：挿入する列の幅
//	引数１：挿入する列の文字の位置
//	引数３：再描画指定 TRUE=再描画する
//	戻り値：成功＝TRUE、失敗＝FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::AddNewCol(long targetCol,long width, short pos,BOOL redraw)
{
	long ret;
	long prevCols;
	int offset,i;
	BOOL insert = TRUE;
	COLUMNPOSITIONINFO colinfo,*info;


	// 張り付け前の列数
	prevCols = TGridData::m_TotalCols;
	// データ域の張り付け
	if( ( ret =TGridData::AddCol(targetCol)) <0 ) return(FALSE);

	// 挿入した列のデフォルト値
	memset( &colinfo, 0, sizeof(colinfo) );
	colinfo.width = width;
	colinfo.position = pos;

	// 列がないときの処理
	if( targetCol < 0 ) targetCol = 0;
	if( prevCols <= 0){
		prevCols = 1;
		targetCol = 0;
		insert = FALSE;
	}

	// 列情報
	// 上書きorずらす必要なし
	if( targetCol > prevCols || !insert){
		if( targetCol > prevCols )		targetCol = prevCols;

		// コピーする
		memcpy( &m_ColInfo[targetCol], &colinfo, sizeof(COLUMNPOSITIONINFO) );
	}

	// 挿入
	else{
		// ずらす場合（列MAX）
		if( prevCols == TGridData::m_TotalCols ) offset =1;

		// !列max
		else	offset = 0;

		info = (COLUMNPOSITIONINFO*)realloc(m_ColInfo,sizeof(COLUMNPOSITIONINFO) * (m_TotalCols+1));
		m_ColInfo=info;

		for(i=TGridData::m_TotalCols;i>targetCol;i--){
			m_ColInfo[i].width=m_ColInfo[i-1].width;
			m_ColInfo[i].position=m_ColInfo[i-1].position;
		}
//		memmove( &m_ColInfo[targetCol+1], &m_ColInfo[targetCol],	( prevCols-targetCol-offset) * sizeof(COLUMNPOSITIONINFO) );
		memcpy( &m_ColInfo[targetCol], &colinfo,    sizeof(COLUMNPOSITIONINFO) );
	}

	// 列数に変化があったときはスクロールバーを更新
	if( TGridData::m_TotalCols != prevCols )	SetHScroll();

	// 再描画指定あり
	if( redraw)		DrawData();


	return(TRUE);
}

///**********************************************************************************************************************
//										virtual メンバ関数
///**********************************************************************************************************************

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvEndEdit( long col, long row ,char *txt ,short *move ,long *nextCol,long *nextRow)
//	タイプ：virtual	(private)
//	機能　：編集が終わった時に呼ばれる関数
//	引数１：編集中のの列番号
//	引数２：編集中のの行番号
//	引数３：入力されたテキスト
//	引数４：１＝終了後、テキストボックスを開いたままセルを移動させる場合（移動先セルは引数の５と６）
//			２＝終了後、テキストボックスを閉じてセルを移動する場合（移動先セルは引数の５と６）
//			０＝終了後、テキストボックスを閉じて移動しない場合
//	引数５：移動先の列番号
//	引数６：移動先の行番号
//	引数７：色
//	戻り値：編集を終了させる＝TRUE、編集を継続させる＝FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvEndEdit( long col, long row ,char *txt ,short *move ,long *nextCol,long *nextRow,char *color)
{
	*color=0;
	*move=NOT_MOVE;
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvClick( long col,long row)
//	タイプ：virtual	(private)
//	機能　：クリックされた時に呼ばれる関数
//	引数１：クリックされた列番号
//	引数２：クリックされた行番号
//	戻り値：デフォルト処理をする＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvClick( long col,long row)
{
	return(TRUE);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvHeaderClick( long col)
//	タイプ：virtual	(private)
//	機能　：行ヘッダをクリックされた時に呼ばれる関数
//	引数１：行ヘッダをクリックされた列番号
//	戻り値：デフォルト処理をする＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvHeaderClick( long col)
{
	return(TRUE);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvDblClick( long col,long row)
//	タイプ：virtual	(private)
//	機能　：ダブルクリックされた時に呼ばれる関数
//	引数１：クリックされた列番号
//	引数２：クリックされた行番号
//	戻り値：デフォルト処理をする＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvDblClick( long col,long row)
{
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvClick( long col,long row)
//	タイプ：virtual	(private)
//	機能　：編集開始直前に呼ばれる関数
//	引数１：列番号
//	引数２：行番号
//	戻り値：編集を許可する＝TRUE　許可しない=FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvStartEdit( long col, long row )
{
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvClick( long col,long row)
//	タイプ：virtual	(private)
//	機能　：キーが押されたときに呼ばれる関数
//	引数１：列番号
//	引数２：行番号
//	戻り値：デフォルト処理をさせる＝TRUEしない=FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvKeyDown( WPARAM keycode )
{
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::BOOL EvMoveCelEnd( long col, long row)
//	タイプ：virtual	(private)
//	機能　：アクティブセルが移動完了時に呼ばれる関数
//	引数１：移動後の列番号
//	引数２：移動後行番号
//	戻り値：デフォルト処理をさせる＝TRUEしない=FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvMoveCelEnd( long col, long row)
{
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//LONG CGridViewCtrl::DoCommand(HWND hDlg ,UINT message, WPARAM wParam, LPARAM lParam)
//	タイプ：Public
//	機能　：メッセージループ
//---------------------------------------------------------------------------------------------------------------------
LONG CGridViewCtrl::DoCommand(HWND hWind ,UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL ret;
	int i;
	char asc;

	switch(message)
	{

		case WM_KILLFOCUS:			// LOST FOCUS
			if( !m_info.selecttype )
				LostFocusCel(m_ActCel.col,m_ActCel.row);

			// 行選択モード
			else{
				for(i=0;i<m_FixCols;i++)
					LostFocusCel(i,m_ActCel.row);
				for(i=m_Left; i<=m_Right+1;i++)
					LostFocusCel(i,m_ActCel.row);
			}
			break;
		case WM_SETFOCUS:			// LOST FOCUS
			if( m_ActCel.col < 0 ) m_ActCel.col =0;
			if( !m_info.selecttype ){
				DrawCel(m_ActCel.col,m_ActCel.row,TRUE);
				EvMoveCelEnd(m_ActCel.col,m_ActCel.row);
			}
			// 行選択モード
			else{
				for(i=0;i<m_FixCols;i++)
					DrawCel(i,m_ActCel.row,TRUE);
				for(i=m_Left; i<=m_Right+1;i++)
					DrawCel(i,m_ActCel.row,TRUE);
			}
//			SetIme(0);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_USER_KEYDOWN:
			// IMEの切換キーはデフォルトのwindprocへ
			if(wParam==0xe5){
				UINT a,b;
				// スキャンコードを取得
				a=lParam << 8;
				b=a >> 24;
				switch(b){
					case 0x39:
					message=WM_KEYDOWN;
					wParam=VK_SPACE;
					break;
					case 0x79:
					case 0x70:
					case 0x7B:
						message=WM_KEYDOWN;
						return DefWindowProc(hWind, message, wParam, lParam);
					default:break;
				}
			}

//		case WM_IME_KEYDOWN:
			// 仮想関数を呼び出す
			ret = EvKeyDown(wParam);
			// デフォ処理をする
			if( ret ){
				switch(wParam){
					// 左右移動はセル選択モード時のみ有効
					case VK_LEFT:
						if(!m_info.selecttype) MoveCel(MV_LEFT);
						break;
					case VK_RIGHT:
						if(!m_info.selecttype) MoveCel(MV_RIGHT);
						break;
					case VK_UP:
						MoveCel(MV_UP);
						break;
					case VK_DOWN:
						MoveCel(MV_DOWN);
						break;
					case VK_RETURN:
					case VK_SPACE:
						// 編集許可されてる時
						if( m_info.readonly ==FALSE ){
							// 仮想関数を呼び出す
							ret = EvStartEdit(m_ActCel.col,m_ActCel.row);
							// デフォ処理をする
							if(ret){
								m_EditMode = FALSE;
								StartEdit(m_ActCel.col,m_ActCel.row);
							}
						}	
						break;

					// IME関係全／半角キー
					case 0x00f4:
					case 0x00f3:
					case 0x0019:
						message=WM_KEYDOWN;
						return DefWindowProc(hWind, message, wParam, lParam);

					// 編集を無視するキー
					case VK_MENU:
					case VK_HOME:
					case VK_END:
					case VK_NEXT:
					case VK_PRIOR:
					case VK_TAB:
					case VK_DELETE:
					case VK_INSERT:
					case VK_SCROLL:
						break;

					default:
						// 編集許可されてる時
						if( m_info.readonly ==FALSE ){
							// 数値関係のキーだった場合は入力に移る
							if(IsKeyNumeric(&asc,wParam,lParam)){
								// 仮想関数を呼び出す
								ret = EvStartEdit(m_ActCel.col,m_ActCel.row);
								// デフォ処理をする
								if(ret){
									m_EditMode = FALSE;

									// ImeがOffの時は変換したのを送る
									StartEdit(m_ActCel.col,m_ActCel.row,asc,TRUE);
								}
							}
						}	
						break;
				}
			}
			break;
		// ウィンドウ再描画(仮装デバイスコンテキストの内容をコピーする)
		case WM_PAINT:
				HDC tmpHdc;
			PAINTSTRUCT ps;
			ps.fErase=FALSE;
			tmpHdc=BeginPaint(hWind,&ps);
			ps.fErase=FALSE;

			BitBlt(tmpHdc,ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right,ps.rcPaint.bottom,m_hdcList,ps.rcPaint.left,ps.rcPaint.top,
					SRCCOPY);
			EndPaint(hWind,&ps);
			break;

		case WM_CREATE:
			break;

		case WM_LBUTTONDOWN:
			EvClickPrivate(hWind,lParam,FALSE);
			break;

		case WM_LBUTTONDBLCLK:
			EvClickPrivate(hWind,lParam,TRUE);
			break;

		case WM_VSCROLL:
			EvVScroll(hWind,LOWORD(wParam),HIWORD(wParam));
			break;

		case WM_HSCROLL:
			EvHScroll(hWind,LOWORD(wParam),HIWORD(wParam));
			break;

		case WM_DESTROY:
			// release the GDI objects
			m_hFont = (HFONT)SelectObject( GetDC(hWind) , m_tmpFont );
			m_hBit=(HBITMAP)SelectObject(m_hdcList,m_hOldBit);
			DeleteObject( m_hFont );
			DeleteObject( m_hBit );
			DeleteObject( m_eFont );
			DeleteDC(m_hdcList);
			break;

		default:

			return DefWindowProc(hWind, message, wParam, lParam);
	}
	return(0);

}
//---------------------------------------------------------------------------------------------------------------------
//	BO1OL CGridViewCtrl::IsKeyNumeric( char* theAsci, UINT key, UINT scanCd)
//	タイプ：private
//	引数１：アスキー代入バッファ（出力）
//	引数１：チェックするキーコード
//	引数１：チェックするキーのスキャンコード
//	機能　：押されたキーコードが数値関連('0'-'9',','等)か調べ,ASCII変換する
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::IsKeyNumeric( char* theAsci, UINT key, UINT scanCd)
{
	BYTE keys[256];
	WORD wRet;
	char asci;
	int ret;
	UINT a,b;

	GetKeyboardState(keys);

	ret=ToAscii( key, (scanCd >> 16 ) && 0xFF,keys, &wRet,0 );

	asci=(char)wRet & 0xFF;
ret=0;
	// IMEかもしれないので、チェックする
	if(ret==0){

		// スキャンコードを取得
		a=scanCd << 8;
		b=a >> 24;

		// スキャンコードから仮想キーコードを取得 
		a = MapVirtualKey(b,1);

		// IME制御のキー
		switch(b){
			case 0x70:
			case 0x7b:
			case 0x79:
				return(FALSE);
			// 10keypad( MapVirtualKey()で変換してくれない)
			case 0x52:
				a=VK_NUMPAD0;break;
			case 0x4F:
				a=VK_NUMPAD1;break;
			case 0x50:
				a=VK_NUMPAD2;break;
			case 0x51:
				a=VK_NUMPAD3;break;
			case 0x4B:
				a=VK_NUMPAD4;break;
			case 0x4C:
				a=VK_NUMPAD5;break;
			case 0x4d:
				a=VK_NUMPAD6;break;
			case 0x47:
				a=VK_NUMPAD7;break;
			case 0x48:
				a=VK_NUMPAD8;break;
			case 0x49:
				a=VK_NUMPAD9;break;
			case 0x53:
				a=VK_DECIMAL;break;		// "."
			case 0x35:
				a=VK_DIVIDE;break;		// "/"
			case 0x37:
				a=VK_MULTIPLY;break;		// "*"
			case 0x4A:
				a=VK_SUBTRACT;break;	// "-"
			case 0x4E:
				a=VK_ADD;break;			// "+"
			case 0x1C:
				a=VK_RETURN;break;		// "Enter"
			default:break;
		}

		// 再度ascii変換
		ret=ToAscii( a,  b && 0xFF,keys, &wRet,0 );
		if( !ret || ret == 2 ) return(FALSE);
		asci=(char)wRet & 0xFF;
		*theAsci=asci;
		// 2ﾊﾞｲﾄ文字はチェックなし
		return(TRUE);
	}
/*
	// ascii変換した文字が、数値関連かチェック
	if( asci >= '0' && asci <='9' ){
		*theAsci=asci;
		return(TRUE);
	}
*/

	if(IsCharAlphaNumeric(asci)==TRUE){
		*theAsci=asci;
		return(TRUE);
	}

	switch(asci){
		case '-':
		case '.':
		case '+':
			break;

		// その他はNG
		default:
			return(FALSE);
	}

	*theAsci=asci;
	return(TRUE);

}


//---------------------------------------------------------------------------------------------------------------------
//LONG CGridViewCtrl::EditProc(HWND hDlg ,UINT message, WPARAM wParam, LPARAM lParam)
//	タイプ：Public
//	機能　：editコントロール用のメッセージループ
//---------------------------------------------------------------------------------------------------------------------
LONG CGridViewCtrl::EditProc(HWND hWind ,UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message){
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			switch(wParam){
				case VK_RETURN:
//					EndEdit(2);
					return(TRUE);

				case VK_ESCAPE:
					EndEdit(3);
					return(TRUE);
			default:break;
			}
			break;

		case WM_CHAR:
			switch(wParam){
				case 0x0d:
					EndEdit(2);
					return(TRUE);
			}


		case WM_COMMAND:
//			switch(wParam){
//				case VK_RETURN:

			break;

		case WM_KILLFOCUS:			// LOST FOCUS
			SendMessage( m_EditWnd, EM_SETSEL,(WPARAM)0,(LPARAM)0);
			EndEdit(1);
			break;

		default:break;
	}

	return( CallWindowProc( (WNDPROC)m_oldEditProc,hWind,message,wParam,lParam) );
}

//---------------------------------------------------------------------------------------------------------------------
//	LONG CGridViewCtrl::KeyProc( UINT nCode, WPARAM wParam, LPARAM lParam)
//	タイプ：Public
//	機能　：キーフック
//---------------------------------------------------------------------------------------------------------------------
LONG CGridViewCtrl::KeyProc( int nCode, WPARAM wParam, LPARAM lParam)
{
	LPMSG msg=(LPMSG)lParam;

	if( nCode <0 )
		return( CallNextHookEx(m_KeyHook,nCode,wParam,lParam));

	if( msg->message == WM_SYSKEYDOWN || msg->message==WM_KEYDOWN ){
		msg->message = WM_USER_KEYDOWN;				// メッセージ書き換え
	}
	return(0);
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
static LRESULT CALLBACK GridProc(HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam)
{
	int i;

	// ウインドウ検索
	for( i=0; i<clsCnt;i++)
		if(clsArray[i]->m_ListWnd == hwnd ) break;

	// クラスのメンバ関数に処理をわたす
	return( clsArray[i]->DoCommand(hwnd, message, wParam, lParam) );
}

// セル編集用入力コントロールのインスタンス
static LPARAM CALLBACK GridEditProc(HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam)
{
	int i;

	// ウインドウ検索
	for( i=0; i<clsCnt;i++)
		if(clsArray[i]->m_EditWnd == hwnd ) break;


	// クラスのメンバ関数に処理をわたす
	return( clsArray[i]->EditProc(hwnd, message, wParam, lParam) );
}

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
/*
	CWPSTRUCT *cwp;
	cwp=(CWPSTRUCT*)lParam;
*/
	LPMSG msg=(LPMSG)lParam;

	int i;

	// ウインドウ検索
	for( i=0; i<clsCnt;i++){
		if(clsArray[i]->m_ListWnd == msg->hwnd ){
			// クラスのメンバ関数に処理をわたす
			clsArray[i]->KeyProc(nCode, wParam, lParam);
			break;
		}
	}
	return(0);
}


