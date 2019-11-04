//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:GridView.cpp
//	Class		:CGridViewCtrl
// 				:�O���b�h�E�C���h�E�E�r���[�N���X
//	�쐬��		:1999.07.30	Coded by coba
//
//========================================================================================
#include ".\grdview3.h"
#include "colordef.h"
#include "stdio.h"

#define WM_USER_KEYDOWN WM_USER+100

// ���܂��Ȃ��֌W--------------------------------------------------------------------------
static CGridViewCtrl** clsArray=NULL;
static int clsCnt=0;
static LRESULT CALLBACK GridProc(HWND, UINT, WPARAM, LPARAM);
static LPARAM CALLBACK GridEditProc(HWND, UINT, WPARAM, LPARAM);
static LRESULT CALLBACK KeyHookProc(int, WPARAM, LPARAM);
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
CGridViewCtrl::CGridViewCtrl(HINSTANCE inst,HWND hparent,long rowblock,long totalcol):TGridData(rowblock,totalcol,FALSE)
{
	m_hInst = inst;
	m_hwndParent = hparent;

	m_WindowInit=FALSE;

	m_Top=0;					// �\�����̍ŏ㕔�̍s�ԍ�
	m_Bottom=0;				// �\�����̍ŉ����̍s�ԍ�
	m_Left=0;
	m_Right=0;

	m_EditMode = FALSE;

	// �f�t�H���g�ݒ�
	memset( &m_info, 0, sizeof(m_info) );
	m_info.fontsize=12;				// �t�H���g�T�C�Y
	m_info.rowtitle=FALSE;			// �s�w�b�_
	m_info.rowtitleWidth=20;			// �s�w�b�_�̕�( �w��Ȃ��̎��ׂ̈Ƀf�t�H���g��)
	m_info.hScroll=FALSE;			// �����X�N���[���Ȃ�
	m_info.vScroll=TRUE;			// �����X�N���[������	m_info.readonly=FALSE;			// �ҏW�\


	// �f�t�H���g�̃T�C�Y
	m_info.fontsize = 12;
	m_ColHeight = m_info.fontsize +4;
	m_DispRows = 0;

	// �f�t�H���g�ŌŒ�s�͗��̂�
	m_FixRows=0;				// �Œ�s�i�c�X�N���[���Ȃ��j�̍s��
	m_FixCols=0;				// �Œ�� 2004.06.08

	m_ColInfo = (COLUMNPOSITIONINFO*)malloc(sizeof(COLUMNPOSITIONINFO) * (totalcol+1));
	memset(m_ColInfo, 0, sizeof(COLUMNPOSITIONINFO)*(totalcol+1));

	// �A�N�e�B�u�Z��
	memset( &m_ActCel, 0, sizeof(m_ActCel) );
	m_ActCel.col = -1;

	// �N���X�z��

	CGridViewCtrl** _clsArray = (CGridViewCtrl **)realloc(clsArray, sizeof(CGridViewCtrl *) * (clsCnt +1));
	(clsArray = _clsArray)[clsCnt] = this;
	clsCnt++;

	memset( &m_CellBuf, 0, sizeof(CELLCOPYBUF));
	m_CellBuf.col = -1;
	m_CellBuf.row = -1;



}


//---------------------------------------------------------------------------------------------------------------------
//	TSelectOrder::TSelectOrder()
//	�^�C�v�FPublic
//	�@�\�@�F�f�X�g���N�^
//---------------------------------------------------------------------------------------------------------------------
CGridViewCtrl::~CGridViewCtrl(void)
{
	free(m_ColInfo);

	SetWindowLong(m_EditWnd,GWL_WNDPROC,m_oldEditProc);
	UnhookWindowsHookEx(m_KeyHook);
	DestroyWindow(m_ListWnd);

	// �N���X�z�񂩂�폜����
	int		index;

	for( index=0; index< clsCnt;index++)
		if(clsArray[index]==this)
			break;


	memmove(clsArray + index, clsArray + index +1, sizeof(CGridViewCtrl *) * (--clsCnt - index));
	clsArray = (CGridViewCtrl **)realloc(clsArray, sizeof(CGridViewCtrl *) * clsCnt);

}


//---------------------------------------------------------------------------------------------------------------------
//	CGridViewCtrl::CreateGrid(WNDPROC wproc, LPRECT pRect)
//	�^�C�v�FPublic
//	�@�\�@�F�E�C���h�E�쐬
//	�����Q�F�e�E�C���h�E�ɑ΂��鍶����W�ƁA�E�C���h�E�̍����Ƌ�
//	�����R�F�O���b�h�ݒ���\����
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::CreateGrid(LPRECT pRect,GRIDINFO* gr,char* fontname,DWORD charset)
{
	// �O���b�h�ݒ�
	memset( &m_info, 0, sizeof(m_info) );
	memcpy( &m_info, gr, sizeof(m_info) );

	// �T�C�Y
	m_ColHeight = m_info.fontsize +4;

	// �s�w�b�_�̕����`�F�b�N
	if( !m_info.rowtitle)			// �s�w�b�_�Ȃ�
		m_info.rowtitleWidth=20;		// �O�̂���
	else if( m_info.rowtitle && !m_info.rowtitleWidth )
		m_info.rowtitleWidth = 20;

	TGridData::m_RowTitleEnable=m_info.rowtitle;

	memcpy( &m_ListRect, pRect, sizeof(RECT) );

	// �X�N���[���w�肪����ꍇ�̃t���O
	DWORD winStyle;
	int hscrollsize=0;
	int vscrollsize=0;

	// WS_VISIBLE|WS_CHILD|WS_BORDER|WS_VSCROLL|WS_HSCROLL,
	winStyle = 	WS_VISIBLE|WS_CHILD|WS_BORDER;


	// �X�N���[���o�[�̕������N���C�A���g�̈�𒲐�
	if( m_info.vScroll ){
		vscrollsize= 22;			// �X�N���[���̕�
		winStyle |= WS_VSCROLL;
	}

	if( m_info.hScroll ){
		winStyle |= WS_HSCROLL;
		hscrollsize= 20;			// �X�N���[���̕�
	}

	WNDCLASS wcl;
	char className[20];
	wsprintf(className,"cobalist");

	// �o�^�ς݂��H
	if( !GetClassInfo(m_hInst, className, &wcl) ){
		// �N���X�̓o�^
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

	//==============���萻���X�g�R���g���[��==================
	m_ListWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		"coba Fanatic class libraly",
		winStyle,
		pRect->left,		pRect->top,
		pRect->right+vscrollsize,			// �X�N���[���̕�
		pRect->bottom + hscrollsize,
		m_hwndParent,
		NULL,
		m_hInst,
		NULL 
	);

	// �\���s���̌���(WindowHeight / colheight����s�w�b�_����������)
	m_DispRows = (short)((pRect->bottom / m_ColHeight ) -1);
	if( m_DispRows < 0 ) m_DispRows = 0;

	//================���͗p�G�f�B�b�g�R���g���[��====================
	m_EditWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"EDIT","",
		WS_CHILD|ES_LEFT|ES_AUTOHSCROLL|ES_WANTRETURN|ES_MULTILINE,
		0,0,100,m_ColHeight,
		m_ListWnd,(HMENU)(100),	m_hInst,NULL
	);

	// �t�H���g���R�s�[
	memset( m_FontName, 0, sizeof(m_FontName));
	if( fontname == NULL || !strlen(fontname) ){
		strcpy( m_FontName,"�l�r �S�V�b�N");
		m_CharSet = SHIFTJIS_CHARSET;
	}
	else{
		memcpy( m_FontName,fontname,strlen(fontname)>100 ? 100:strlen(fontname));
		m_CharSet = charset;
	}

	// �T�u�N���X��
	m_oldEditProc = SetWindowLong(m_EditWnd,GWL_WNDPROC, (LONG)GridEditProc);

	m_eFont = CreateFont(	
			m_info.fontsize,
			0,				// �t�H���g�T�C�Y
			0,					// Escapement
			0,					// Orientation
			FW_NORMAL,				// Weight(�t�H���g�̑���)
			0,					// Ital(�C�^���b�N��)
			0,					// Underline(����)
			0,					// StrikeThru(�ł�������)
//			SHIFTJIS_CHARSET,	// Charset(�����Z�b�g)
			m_CharSet,			// Charset(�����Z�b�g)
			OUT_DEFAULT_PRECIS,	// Precision(�o�͐��x)
			CLIP_DEFAULT_PRECIS,// ClipPrecision(�N���b�s���O���x)
			DEFAULT_QUALITY,	// Quality(�t�H���g�̏o�͕i��)
			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(�t�H���g�̃s�b�`�ƃt�@�~��)
			m_FontName
// 				"�l�r �S�V�b�N"
	);


	EvInit(m_ListWnd);

	SendMessage( m_EditWnd, WM_SETFONT,(WPARAM)m_eFont,(LPARAM)MAKELONG((WORD)TRUE,0));

	ShowWindow(m_ListWnd, SW_SHOW);
	UpdateWindow(m_ListWnd);
	 m_WindowInit=TRUE;

	// �X�N���[���w�肪����ꍇ
	if( m_info.vScroll )
		EnableScrollBar(m_ListWnd,SB_VERT,ESB_DISABLE_BOTH);

	if( m_info.hScroll ){
		EnableScrollBar(m_ListWnd,SB_HORZ,ESB_DISABLE_BOTH);
		SetHScroll();
	}

	// �Œ�s�֌W
	m_FixRows = m_info.fixRows;
	m_Top=m_FixRows;					// �\�����̍ŏ㕔�̍s�ԍ�
	m_ActCel.row = m_FixRows;

	// �Œ��֌W	2004.06.08
	m_FixCols = m_info.fixCols;
	m_Left=m_FixCols;					// �\�����̍ŏ㕔�̗�ԍ�
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
//	�^�C�v�Fprivate
//	�@�\�@�F�E�C���h�E�������B�t�H���g�A��ʓ��̃C�j�V�����C�Y
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvInit(HWND hwnd)
{
	// �`��p���z�f�o�C�X�R���e�L�X�g�쐬
	HDC tmpHdc;

	tmpHdc=GetDC(hwnd);
	SelectObject( tmpHdc,GetStockObject(WHITE_BRUSH));
	m_hdcList=CreateCompatibleDC(tmpHdc);
	m_hBit=CreateCompatibleBitmap(tmpHdc, m_ListRect.right, m_ListRect.bottom);
	SelectObject( m_hdcList,GetStockObject(WHITE_BRUSH));
	m_hOldBit = (HBITMAP)SelectObject(m_hdcList,m_hBit);
	PatBlt(m_hdcList,0,0,m_ListRect.right,m_ListRect.bottom,PATCOPY);
	ReleaseDC( hwnd,tmpHdc );

	// �t�H���g�̐ݒ�
	m_hFont = CreateFont(	
			m_info.fontsize,
			0,				// �t�H���g�T�C�Y
			0,					// Escapement
			0,					// Orientation
			FW_NORMAL,				// Weight(�t�H���g�̑���)
			0,					// Ital(�C�^���b�N��)
			0,					// Underline(����)
			0,					// StrikeThru(�ł�������)
//			SHIFTJIS_CHARSET,	// Charset(�����Z�b�g)
			m_CharSet,			// Charset(�����Z�b�g)
			OUT_DEFAULT_PRECIS,	// Precision(�o�͐��x)
			CLIP_DEFAULT_PRECIS,// ClipPrecision(�N���b�s15���O���x)
			DEFAULT_QUALITY,	// Quality(�t�H���g�̏o�͕i��)
			DEFAULT_PITCH | FF_DONTCARE,	// Pitch(�t�H���g�̃s�b�`�ƃt�@�~��)
			m_FontName
//			"�l�r �S�V�b�N"
	);

	m_tmpFont = (HFONT)SelectObject( m_hdcList , m_hFont );

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL EvClickPrivate(HWND hwnd,LPARAM lparam)
//	�^�C�v�Fprivate
//	�@�\�@�F�}�E�X�N���b�N���̃C�x���g�B�����̃Z�����N���b�N���ꂽ�����ׂāA���z�֐����Ăяo��
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvClickPrivate(HWND hwnd,LPARAM lparam,BOOL dblClick)
{
	WORD xPos,yPos,yCel,xCel;
	int i;
	long xColPos=0;
	long currentRow;

	SetFocus(hwnd);

	// ���X�g�̓x�̗̈悪�N���b�N���ꂽ�����ׂ�
	xPos = LOWORD(lparam);
	yPos = HIWORD(lparam);

	// �N���b�N�ʒu����A�i�\����́j�����̃Z�����𒲂ׂ�
	// y����
	// �w�b�_�̈�Ȃ�Ζ���
	yCel = yPos / m_ColHeight;

	// x����
	BOOL xOk=FALSE;
/// �Œ����l�� 2004.06.08
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
	// �N���b�N�ʒu���s�w�b�_���H
	if(TGridData::m_RowTitleEnable==TRUE){
		if( xPos < m_info.rowtitleWidth ) xOk = FALSE;
	}


	// �N���b�N�s����w�b�_�܂��͌Œ�s�̏ꍇ
//	if( !yCel || yCel <= m_FixRows){	// yCel<FixRows ...�s�w�b�_or�Œ�s
//		return(FALSE);
//	}
	// �N���b�N�s����w�b�_�̏ꍇ
	if( !yCel){	// �s�w�b�_
		EvHeaderClick(xCel);
	}
	// �N���b�N�s���Œ�s�̏ꍇ
	if( yCel <= m_FixRows){	// yCel<FixRows ...�Œ�s
		return(FALSE);
	}
	// �N���b�N�����Z�����L���f�[�^�̈悩�H
	if( m_Top + yCel-m_FixRows-1 > TGridData::m_LastRowNo || !xOk){
		return(FALSE);
	}

	// �N���b�N�ʒu����A�f�[�^�s���v�Z
	currentRow=(long)yCel + m_Top - m_FixRows - 1;

	if(currentRow < m_FixRows ) return(FALSE);

	// ���z�֐��Ăяo��
	if(!dblClick){
		if( EvClick(xCel,currentRow) ){
			// �N���b�N�����Z�����A�N�e�B�u��
			MoveActiveCel(xCel,currentRow,TRUE);
		}
	}
	// �_�u���N���b�N
	else{
		EvDblClick(xCel,currentRow);
	}
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::StartEdit(DWORD x,DWORD y)
//	�^�C�v�Fprivate
//	�@�\�@�F�Z���̕ҏW�J�n�B�ҏW�Ώۂ̃Z���ɃG�f�B�b�g�R���g���[����\�����ē��͂���B
//	�����P�F�ΏۃZ���̗�
//	�����Q�F�s
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::StartEdit(DWORD x,DWORD y,char keycode,BOOL first)
{

	int  editX,editY,i;
	char theData[256];
	int xColPos=0;
	// �ҏW���Ȃ�ΏI��点��
	EndEdit(1);

	LostFocusCel(m_ActCel.col,m_ActCel.row);

	// ��ʏ�̗�̍��W�����
	xColPos = m_info.rowtitleWidth;
	for( i= 0; (WORD)i < m_FixCols; i++){
		xColPos += m_ColInfo[i].width;
	}
	for( i= m_Left; (WORD)i < x; i++){
		xColPos += m_ColInfo[i].width;
	}

	// �G�f�B�b�g�E�C���h�E�̈ʒu������
	editX = xColPos+1;
	editY = (y - m_Top +1+m_FixRows) * m_ColHeight;

	// �Z���f�[�^���擾
	memset( theData, 0, sizeof(theData) );
	if( !GetCel( theData, sizeof(theData)-1, x,y ) ) return(FALSE);

	m_EditMode = TRUE;

	// �G�f�B�b�g�E�C���h�E���ړ�
	SetWindowText( m_EditWnd, NULL );
	SetWindowText( m_EditWnd, theData );
	MoveWindow( m_EditWnd, editX, editY,m_ColInfo[x].width,m_ColHeight+1,TRUE);
	SendMessage( m_EditWnd, EM_SETSEL,(WPARAM)0,(LPARAM)strlen(theData));
	ShowWindow(m_EditWnd,SW_SHOW);
	SetFocus(m_EditWnd);

	// �ړ������֐�
	EvEditBoxMove( x, y);


	// ���p���[�h�̏ꍇ�ŁA���͊J�n�̂Ƃ��́A�����ꂽ�L�[���_�C���N�g�ɔ��f
	if( (!GetImeStatus() && keycode !=NULL) ||
		(!GetImeStatus() && keycode !=NULL && first) ){
		if( keycode ==VK_BACK)
			memset( theData, 0, sizeof(theData) );
		else
			wsprintf(theData,"%c",keycode);
		SetWindowText( m_EditWnd, theData);
		SendMessage( m_EditWnd, EM_SETSEL,(WPARAM)1,(LPARAM)1);
	}
	// IMEon�̎��ɃL�[���ނ𑗂�
//	else if(first){
	else if( keycode !=NULL && GetImeStatus() && first){
		HIMC imc;
		char aaa[20];
		imc=ImmGetContext(m_EditWnd);
		wsprintf(aaa,"%c",keycode);
		ImmSetCompositionString(imc,SCS_SETSTR,NULL,0,aaa,1);
		ImmReleaseContext(m_EditWnd,imc);
	}

	// �ҏW���̃Z�����
	m_editX=x,m_editY=y;
	return(TRUE);

}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::EndEdit(void)
//	�^�C�v�Fprivate
//	�@�\�@�F�Z���̕ҏW�I���B�f�[�^�X�V
//	�����@�F�I���^�C�v�@0 �`�F�b�N�Ȃ�
//						1 �`�F�b�N�̂�
//						2 �`�F�b�N&�ړ�
//						3 �L�����Z��
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EndEdit(int action)
{

	// �ҏW���ł͂Ȃ��Ƃ�(�ċA�I�ɌĂ΂��̂�h������)
	if(!m_EditMode) return(FALSE);

	char theData[256];
	char color = 0;

	// �L�����Z���̎�
	if( action==3){
		// �ҏW���[�hoff
		m_EditMode = FALSE;

		// �E�C���h�E�B��
		ShowWindow(m_EditWnd,SW_HIDE);
		SetWindowText( m_EditWnd, NULL );
		DrawCel(m_ActCel.col,m_ActCel.row,TRUE);
		return(TRUE);
	}

	// �ҏW�f�[�^���E�C���h�E����擾
	memset( theData, 0, sizeof(theData) );
	GetWindowText( m_EditWnd, theData,sizeof(theData)-1 );


	short movenext=NOT_MOVE;
	long nextX=-1,nextY=-1;

	// �ҏW���[�hoff
	m_EditMode = FALSE;

	// �e�L�X�g�`�F�b�N
	if( action > 0 ){
		// �`�F�b�N�֐��i���z�j���Ăяo��
		if( !EvEndEdit( m_editX, m_editY,theData, &movenext, &nextX,&nextY, &color)){
			// �s���i�̎��̓e�L�X�g�N���A
			SetWindowText( m_EditWnd, NULL );
/*			if(action ==0)
				ShowWindow(m_EditWnd,SW_HIDE);
			return(FALSE);
*/
		}
	}


	// �E�C���h�E�B��
	ShowWindow(m_EditWnd,SW_HIDE);
	SetWindowText( m_EditWnd, NULL );


	if(movenext && action == 2 ){
		// �܂��̓f�[�^���Z�b�g
		if( !SetCel( m_editX,m_editY,theData,TGridData::GetCelTag(m_editX,m_editY), TRUE ,color,-1)) return(FALSE);
		// �ړ��悪�L���ȏꍇ�͂����ɔ��
		if( nextX < TGridData::m_TotalCols && nextY <= TGridData::m_LastRowNo ){
			m_ActCel.col=nextX;m_ActCel.row=nextY;
			// ���̕ҏW�悪�w�肳��Ă����炻���ɔ��
			MoveActiveCel(nextX,nextY,TRUE);
			// �e�L�X�g�{�b�N�X�I�[�v���̂܂܈ړ�����ꍇ
			if( movenext == MOVE_WITH_EDIT ) MoveEditBox( nextX, nextY);
		}
		else{
			DrawCel(m_ActCel.col,m_ActCel.row,TRUE);
			// �h���N���X���ł͈ړ�������҂��Ă邩������Ȃ̂ŌĂяo��
			EvMoveCelEnd( m_ActCel.col, m_ActCel.row);
		}
	}
	// �f�[�^�̈�ɐݒ�
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
//�@BOOL CGridViewCtrl::EvEditBoxMove( long col, long row)
//	�^�C�v�Fprivate
//	�@�\�@�F�G�f�B�b�g�R���g���[���̈ړ�����
//	�����P�F�ړ���̃f�[�^��
//	�����Q�F�ٓ���̃f�[�^�s
//	�߂�l�FTRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvEditBoxMove( long col, long row)
{
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//�@BOOL CGridViewCtrl::CellCopy(void)
//	�^�C�v�Fprivate
//	�@�\�@�F�A�N�e�B�u�Z���̓��e���o�b�t�@�ɃR�s�[
//	�߂�l�FTRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::CellCopy(void)
{
	// �R�s�[���̃Z���͗L�����H
	if( m_ActCel.col > TGridData::m_TotalCols || m_ActCel.col < m_FixCols ) return(FALSE);
//	if( m_ActCel.col > TGridData::m_TotalCols || m_ActCel.col < 0 ) return(FALSE);

	if( m_ActCel.row > TGridData::m_LastRowNo || m_ActCel.row < m_FixRows ) return(FALSE);

	// ���e���o�b�t�@�ɃR�s�[����
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
//�@BOOL CGridViewCtrl::CellPaste(int chkmode)
//	�^�C�v�Fprivate
//	�@�\�@�F�o�b�t�@�̓��e���A�N�e�B�u�Z���ɃR�s�[
//	�����@�F1=�s�������ꍇ�̂݃R�s�[ 2=�񂪓����ꍇ�̂݃R�s�[ ���̑����`�F�b�N����
//	�߂�l�FTRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::CellPaste(int chkmode)
{
	// �R�s�[��̃Z���͗L�����H
/// �Œ�s 2004.06.08
	if( m_ActCel.col > TGridData::m_TotalCols || m_ActCel.col < m_FixCols ) return(FALSE);
//	if( m_ActCel.col > TGridData::m_TotalCols || m_ActCel.col < 0 ) return(FALSE);
///

	if( m_ActCel.row > TGridData::m_LastRowNo || m_ActCel.row < m_FixRows ) return(FALSE);

	// �R�s�[�o�b�t�@�͐���H
	if( m_ActCel.col< 0 ||  m_ActCel.row < 0 ) return(FALSE);

	switch(chkmode){
		// �s��v���[�h
		case 1:
			if(m_ActCel.row != m_CellBuf.row) return(FALSE);
			break;

		case 2:
			if(m_ActCel.col != m_CellBuf.col) return(FALSE);
			break;

		default:
			break;
	}

	// ���e���o�b�t�@����R�s�[����
	if( !TGridData::SetCelData(  m_ActCel.col, m_ActCel.row, m_CellBuf.color,m_CellBuf.tag,m_CellBuf.data,m_CellBuf.bkcolor)) return(FALSE);

	// ���X�g�ɏ�������
	if( !DrawCel( m_ActCel.col,m_ActCel.row,TRUE) ) return(FALSE);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::MoveEditBox( long col, long row)
//	�^�C�v�Fprivate
//	�@�\�@�F�G�f�B�b�g�R���g���[���̎����ړ�
//	�����P�F�ړ���̃f�[�^��
//	�����Q�F�ٓ���̃f�[�^�s
//	�߂�l�FTRUE=�ړ� FALSE=�ړ�����
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::MoveEditBox( long col, long row)
{

	// �ړ���̃Z���͗L�����H
/// �Œ�s 2004.06.08
	if( col > TGridData::m_TotalCols || col < m_FixCols ) return(FALSE);
//	if( col > TGridData::m_TotalCols || col < 0 ) return(FALSE);
///

	if( row > TGridData::m_LastRowNo || row < m_FixRows ) return(FALSE);

	// �c�ړ�
	// �X�N���[���\�Ŋ���
	// �ړ��悪���݂̕\���̈���Ɏ��܂��ĂȂ��Ƃ��̓X�N���[������
	if( ( TGridData::m_LastRowNo > m_DispRows ) &&
		( row < m_Top || row >= m_Top + m_DispRows-m_FixRows )){
		// �c�X�N���[���s�̎��͉������Ȃ�
		if( !m_info.vScroll ) return(FALSE);


		if( row < m_Top ) m_Top = row;
		else m_Top = row - m_DispRows + m_FixRows + 1;

		// �f�[�^�ĕ`��
		DrawData();

		// �X�N���[���o�[���ړ�
		SCROLLINFO sinfo;

		// �X�N���[�������擾
		sinfo.cbSize=sizeof(SCROLLINFO);
		sinfo.fMask = SIF_POS;
		sinfo.nPos = m_Top;
		SetScrollInfo( m_ListWnd, SB_VERT,&sinfo ,TRUE);
	}

	// �G�f�B�b�g�R���g���[�����ړ�����
	int  editX,editY,len,xColPos,i;
	char txtBuf[256];

	// ��ʏ�̗�̍��W�����
/// �Œ�s
	xColPos = m_info.rowtitleWidth;
	for( i= 0; i < m_FixCols; i++)
		xColPos += m_ColInfo[i].width;
	for( i= m_Left; i < col; i++){
		xColPos += m_ColInfo[i].width;
	}

	// �G�f�B�b�g�E�C���h�E�̈ʒu������
	editX = xColPos+1;
	editY = m_ColHeight*( row - m_Top +1+m_FixRows);		// �w�b�_�ƌŒ�s�̕�pulus

	m_EditMode = TRUE;

	// �Z���f�[�^���擾
	wsprintf( txtBuf,"%s",TGridData::m_StrData[row].d[col] );
	len = strlen(txtBuf);

	SetWindowText( m_EditWnd, txtBuf );
	MoveWindow( m_EditWnd, editX, editY,m_ColInfo[col].width,m_ColHeight,TRUE);		// �ړ�
	ShowWindow(m_EditWnd,SW_SHOW);
	SendMessage( m_EditWnd, EM_SETSEL,(WPARAM)0,(LPARAM)len);			// �e�L�X�g�I��
	SetFocus(m_EditWnd);

	// �ҏW���̃Z�����
	m_editX=col,m_editY=row;

	// �ړ������֐�
	EvEditBoxMove( col, row);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::SetIme( int mode )
//	�^�C�v�Fprotected
//	�@�\�@�F���̓G�f�B�b�g�{�b�N�X��ime�̐ݒ�
//	�����P�F�ݒ肷�郂�[�h
//			off	0
//			�S�p	1
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetIme( int mode )
{
	HIMC imc;
	imc=ImmGetContext(m_EditWnd);

	// ���p�p��
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
		// �ϊ����[�h�ݒ�
		dwMode =IME_CMODE_ROMAN|IME_CMODE_NATIVE;
		ImmSetConversionStatus(imc,dwMode,dwSent);
		// ��open�Ȃ�ime���N��
		if( !ImmGetOpenStatus(imc))
			ImmSetOpenStatus(imc,TRUE);
	}
	ImmReleaseContext(m_EditWnd,imc);
	return(TRUE);

}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::GetImeStatus(void)
//	�^�C�v�Fprotected
//	�@�\�@�F���̓G�f�B�b�g�{�b�N�X��ime��OnOff�𒲂ׂ�
//	�߂�l�FTRUE=ON,FALSE=OFF
//			�S�p	1
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::GetImeStatus(void)
{

	HIMC imc;
	imc=ImmGetContext(m_EditWnd);

	// open status
	if( !ImmGetOpenStatus(imc)) return(FALSE);

	// ���p�p��
	DWORD dwMode=0,dwSent=0;
	if(!ImmGetConversionStatus(imc,&dwMode,&dwSent)) return(FALSE);

	if(dwMode==IME_CMODE_NOCONVERSION ) return(FALSE);

	 ImmReleaseContext(m_EditWnd,imc);
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::SetColumnHeadder( long col, char *titlestr,long width,short pos)
//	�^�C�v�Fprivate
//	�@�\�@�F��f�[�^�̏�����
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F��̋�
//	�����R�F�����̕\���ʒu
//			POS_LEFT	0
//			POS_CENTER	1
//			POS_RIGHT	2
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetColumnHeadder( long col, char *titlestr,long width,short pos)
{
	if( col >= TGridData::m_TotalCols || col < 0) return(FALSE);

	// �f�[�^�̑��
	if( !TGridData::SetTitle(col,titlestr)) return(FALSE);

	m_ColInfo[col].width=width;

	m_ColInfo[col].position=pos;


	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::DrawHeadder(void)
//	�^�C�v�Fprivate
//	�@�\�@�F��w�b�_�̕\��
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


	// �h��Ԃ�
	// �h��Ԃ�
	// �f�[�^�̈�
	SetRect(&rect ,m_info.rowtitleWidth, m_ColHeight ,m_ListRect.right, m_ListRect.bottom);
	FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));
	// ���̈�
	SetRect(&rect,0,0, m_ListRect.right, m_ColHeight);
//	FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	hbr=CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	FillRect( m_hdcList,&rect,hbr);
	// �s�w�b�_
	SetRect(&rect,0,0, m_info.rowtitleWidth, m_ListRect.bottom);
//	FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	FillRect( m_hdcList,&rect,hbr);
	DeleteObject(hbr);

	SetTextColor( m_hdcList, CLR_BLACK);

	//-----------------------��w�b�_------------------------------------
	// �g���i���j
	hPen   = CreatePen( PS_SOLID ,1, CLR_WHITE);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	MoveToEx( m_hdcList , 0,  0 , NULL );
	LineTo(   m_hdcList , m_ListRect.right, 0 );
	hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
	DeleteObject( hPen );

	// �g���i���j
	hPen   = CreatePen( PS_SOLID ,1, COLOR_BTNSHADOW);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	MoveToEx( m_hdcList , 0 , m_ColHeight , NULL );
	LineTo(   m_hdcList , m_ListRect.right, m_ColHeight );
	hPen   = (HPEN)SelectObject(m_hdcList,hOldpen);
	DeleteObject( hPen );

	//-----------------------�s�w�b�_------------------------------------

	// �f�[�^�s�Ƃ̋�؂�i�R�c���j
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

	// ���r��
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

	//-----------------------��w�b�_------------------------------------
	// ���؂�i�R�c���j�Œ��
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

		// �\���\����z������break
		if( xPos + m_info.rowtitleWidth >= m_ListRect.right ){
			break;
		}
	}
	// ���؂�i�R�c���j
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


		// �\���\����z������break
		if( xPos + m_info.rowtitleWidth >= m_ListRect.right ){
			break;
		}

		// �����Ō��݂̉E����擾����
		m_Right = i;
	}

	//-----------------------�f�[�^�̈�------------------------------------
	// �f�[�^���̉��r��
	hPen   = CreatePen( PS_SOLID ,1, CLR_LGRAY);
	hOldpen   = (HPEN)SelectObject(m_hdcList,hPen);
	for( i=1	; i<m_DispRows+1; i++){
		MoveToEx( m_hdcList , 0 + m_info.rowtitleWidth, m_ColHeight*(i+1), NULL );
		LineTo(   m_hdcList , m_ListRect.right, m_ColHeight*(i+1) );
	}

	// �f�[�^���̏c�r��
	xPos = 0;

	// �Œ��
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


	// �w�i�F
//	SetTextColor( m_hdcList, CLR_BLACK);
	colOld = SetBkMode( m_hdcList,CLR_BLUE );

	// �^�C�g��
	char txtBuf[256];

	xPos = m_info.rowtitleWidth;
	// �Œ��
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


	// �����т傤���w��
	if(redraw){
		SetRect(&rect,0,0,m_ListRect.right,m_ListRect.bottom);
		InvalidateRect(m_ListWnd,&rect,TRUE);
	}
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::ChangeRowTitle(long rowNo, char* newtitle)
//	�^�C�v�Fpublic
//	�@�\�@�F�s��̕ύX
//	�����P�F�ύX����s
//	�����Q�F�ύX��̍s��
//	�߂�l�F������TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::ChangeRowTitle(long rowNo, char* newtitle)
{
	if( !TGridData::ChangeRowTitle(rowNo, newtitle)) return(FALSE);

	if( !m_info.rowtitle) return(FALSE);

	// �Ώۂ̃f�[�^�s���\���Ώۗ̈���ɓ����Ă�����ĕ`�悷��
	if( rowNo >= m_Top + m_DispRows-m_FixRows || (rowNo < m_Top && rowNo >=m_FixRows) )return(TRUE);// �ΏۊO

	// �`�悷��
	int disprow;
	RECT rect;
	char txtBuf[256];

	// ��ʏ�̂ǂ̍s���H
	disprow = rowNo-m_Top+m_FixRows;
	// �Œ�s
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
//	�^�C�v�Fpublic
//	�@�\�@�F�Z���Ƀf�[�^��ݒ�
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F�ݒ肷��s�ԍ�
//	�����R�F�ݒ�f�[�^
//	�����S�F�ݒ�^�O�f�[�^
//	�����T�F��ʂɔ��f��TRUE,���f�Ȃ�=FALSE
//	�����U�F�ݒ�
//	�����V�F�Z���̔w�i�F 0=���A1=�����F -1=�ύX�Ȃ��i�O�̐F���g�p����j
//	�߂�l�F������TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetCel(int col,int row, char* data,long cTag,BOOL draw,char color,char bkcolor)
{
	// �f�[�^���Z�b�g
	if(!TGridData::SetCelData(col,row,color,cTag,data,bkcolor)) return(FALSE);

	// ���X�g�ɏ�������
	if(draw)
		if( !DrawCel(col,row,FALSE) ) return(FALSE);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::DrawCel(int col, int row,BOOL act)
//	�^�C�v�Fprivate
//	�@�\�@�F�Z���Ƀf�[�^��`��
//	�����P�F�ݒ肷��f�[�^��ԍ�
//	�����Q�F�ݒ肷��f�[�^�s�ԍ�
//	�����R�F�A�N�e�B�u�Z����TRUE ����ȊO=FALSE(�f�t�H���g�j
//	�߂�l�F������TRUE
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

	// �`��ł��Ȃ�
	if( col < 0 || row < 0 || TGridData::m_LastRowNo < 0) return(FALSE);

	// �Ώۂ̃f�[�^�s���\���Ώۗ̈���ɓ����Ă��邩�H
	if( row >= m_Top + m_DispRows-m_FixRows || (row < m_Top && row >=m_FixRows) )return(FALSE);
	if( col >= m_FixCols && (col < m_Left || col > m_Right+1) ) return(FALSE);

	// �`�悷��
	// ��ʏ�̂ǂ̍s���H
	disprow = row-m_Top+m_FixRows;
	// �Œ�s
	if( row < m_FixRows) disprow = row;

	// ��ʏ�̗�̍��W�����
	xPos = m_info.rowtitleWidth;
/// �Œ��
	if( col < m_FixCols ){
		for( i= 0; i < col; i++)
			xPos += m_ColInfo[i].width;
	}
///	�X�N���[����
	else{
		for( i= 0; i < m_FixCols; i++)
			xPos += m_ColInfo[i].width;
		for( i= m_Left; i < col; i++)
			xPos += m_ColInfo[i].width;
	}
///


	// �O�̃f�[�^��h��Ԃ�(���͏����Ȃ�)
	SetRect(&rect	,xPos+1			// �s�w�b�_�̕�offset
					,m_ColHeight*(disprow+1)+1
					,m_ColInfo[col].width+xPos
					,m_ColHeight*(disprow+2));

	// �A�N�e�B�u�Z��
	if( act ){
		HBRUSH hbrush=CreateSolidBrush(CLR_BLUE);
		FillRect( m_hdcList,&rect,hbrush);
		DeleteObject(hbrush);
	}
	// �ʏ�̃Z��
	else if(!TGridData::m_StrData[row].bkcolor[col]) FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));

	// �Z���D�F
	else FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(GRAY_BRUSH));

	// �̈���Ɏ��܂镶���������o��
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

	// �A�N�e�B�u�Z��
	if( act ){
		old = SetTextColor( m_hdcList, CLR_WHITE );
		DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);
		SetTextColor( m_hdcList, old );
	}
	else{
	// �F��
		if(TGridData::m_StrData[row].color[col]==1){
			old = SetTextColor( m_hdcList, CLR_RED );
		}
		DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);

		if(TGridData::m_StrData[row].color[col]==1){
			SetTextColor( m_hdcList, old );
		}
	}
	SetRect(&rect	,xPos+1			// �s�w�b�_�̕�offset
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
	// �X�V
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

	// �`��ł��Ȃ�
	if( col < 0 || row < 0 || TGridData::m_LastRowNo < 0) return(FALSE);


	// �Ώۂ̃f�[�^�s���\���Ώۗ̈���ɓ����Ă��邩�H
	if( row >= m_Top + m_DispRows-m_FixRows || (row < m_Top && row >=m_FixRows)) return(FALSE);
/// �Œ�s
	if( col >= m_FixCols && (col < m_Left || col > m_Right+1) ) return(FALSE);
///	if( col < m_Left || col > m_Right+1 ) return(FALSE);
///

	// �`�悷��
	// ��ʏ�̂ǂ̍s���H
	disprow = row-m_Top+m_FixRows;
	// �Œ�s
	if( row < m_FixRows) disprow = row;

	// ��ʏ�̗�̍��W�����
	xPos = m_info.rowtitleWidth;
/// �Œ��
	if( col < m_FixCols ){
		for( i= 0; i < col; i++)
			xPos += m_ColInfo[i].width;
	}
/// �X�N���[����
	else{
		for( i= 0; i < m_FixCols; i++)
			xPos += m_ColInfo[i].width;
		for( i= m_Left; i < col; i++)
			xPos += m_ColInfo[i].width;
	}
///


	// �O�̃f�[�^��h��Ԃ�(���͏����Ȃ�)
	SetRect(&rect	,xPos+1			// �s�w�b�_�̕�offset
					,m_ColHeight*(disprow+1)+1
					,m_ColInfo[col].width+xPos
					,m_ColHeight*(disprow+2));

	if(!TGridData::m_StrData[row].bkcolor[col]) FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(WHITE_BRUSH));
	else FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(GRAY_BRUSH));


	// �̈���Ɏ��܂镶���������o��
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

	// �F��
	if(TGridData::m_StrData[row].color[col]==1){
		old = SetTextColor( m_hdcList, CLR_RED );
	}

	DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);

	// �F��
	if(TGridData::m_StrData[row].color[col]==1){
		old = SetTextColor( m_hdcList, CLR_RED );
	}

	SetRect(&rect	,xPos+1			// �s�w�b�_�̕�offset
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

	// �X�V
	InvalidateRect(m_ListWnd,&rect,TRUE);
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::DrawCel(int col, int row,BOOL act)
//	�^�C�v�Fprivate
//	�@�\�@�F�N���C�A���g�̈�S�Z���`��
//	�߂�l�F������TRUE
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

	// ���Ȃǂ��ĕ`��
	DrawHeadder(FALSE);

	// �\���擪�s����f�[�^��\������
	// 1���ڂ͌Œ�s�A�Q���ڂ͉ύs
	for(count=0;count<2; count++ ){
		// �Œ�
		if(count == 0 ){
			st=0;
			ed= m_FixRows;
			datrow = 0;
		}
		// ��
		else{
			datrow = m_Top;
			st = m_FixRows;
			ed = m_DispRows;
		}
		for( disprow=st; disprow< ed; disprow++){
			
			// �f�[�^���Ȃ��Ƃ�
			if( TGridData::m_LastRowNo < 0 ) break;
			// �s�w�b�_
			if( m_info.rowtitle){
				SetRect(&rect	,3
								,m_ColHeight*(disprow+1)+3
								,m_info.rowtitleWidth-2
								,m_ColHeight*(disprow+2)-2);
				wsprintf(txtBuf,"%s",TGridData::m_StrData[datrow].colTitle);
				DrawText(m_hdcList,txtBuf,-1,&rect, DT_CENTER|DT_SINGLELINE);
			}

			// �Œ��
			xPos = 0;
			for( col = 0; col <m_FixCols; col++ ){
				if( col > m_Right+1) break;
				// �O�̃f�[�^��h��Ԃ�(���͏����Ȃ�)
				SetRect(&rect	,xPos+1+m_info.rowtitleWidth
								,m_ColHeight*(disprow+1)+1
								,xPos+m_ColInfo[col].width+m_info.rowtitleWidth
								,m_ColHeight*(disprow+2));
	
				// �A�N�e�B�u�Z���̎������ʏ���(�I�����[�h���Z���̏ꍇ�j
				if( (col == m_ActCel.col && datrow ==m_ActCel.row && !m_info.selecttype) ||
				// �A�N�e�B�u�Z���̎������ʏ���(�I�����[�h���s�̏ꍇ�j
					(datrow ==m_ActCel.row && m_info.selecttype && m_ActCel.col >=0) )
				{
					HBRUSH hbrush=CreateSolidBrush(CLR_BLUE);
					FillRect( m_hdcList,&rect,hbrush);
					DeleteObject(hbrush);
				}
//				else if(!TGridData::m_StrData[datrow].bkcolor[col]) FillRect( m_hdcList,&rect,GetStockObject(WHITE_BRUSH));
				else if(!TGridData::m_StrData[datrow].bkcolor[col]);
				else FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(GRAY_BRUSH));

				// �̈���Ɏ��܂镶���������o��
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

				// �A�N�e�B�u�Z���̎������ʏ���(�I�����[�h���Z���̏ꍇ�j
				if( (col == m_ActCel.col && datrow ==m_ActCel.row && !m_info.selecttype) ||
				// �A�N�e�B�u�Z���̎������ʏ���(�I�����[�h���s�̏ꍇ�j
					(datrow ==m_ActCel.row && m_info.selecttype && m_ActCel.col >=0) )
				{
					old = SetTextColor( m_hdcList, CLR_WHITE );
					DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);
					SetTextColor( m_hdcList, old );
				}
				// �ʏ�̃Z��
				else{
					// �F��
					if(TGridData::m_StrData[datrow].color[col]==1){
						old = SetTextColor( m_hdcList, CLR_RED );
					}
					DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);
					if(TGridData::m_StrData[datrow].color[col]==1){
						SetTextColor( m_hdcList, old );
					}
				}
				//����x���W
				xPos += m_ColInfo[col].width;
			}
			for( col = m_Left; col <TGridData::m_TotalCols; col++ ){
				if( col > m_Right+1) break;
				// �O�̃f�[�^��h��Ԃ�(���͏����Ȃ�)
				SetRect(&rect	,xPos+1+m_info.rowtitleWidth
								,m_ColHeight*(disprow+1)+1
								,xPos+m_ColInfo[col].width+m_info.rowtitleWidth
								,m_ColHeight*(disprow+2));
	
				// �A�N�e�B�u�Z���̎������ʏ���(�I�����[�h���Z���̏ꍇ�j
				if( (col == m_ActCel.col && datrow ==m_ActCel.row && !m_info.selecttype) ||
				// �A�N�e�B�u�Z���̎������ʏ���(�I�����[�h���s�̏ꍇ�j
					(datrow ==m_ActCel.row && m_info.selecttype && m_ActCel.col >=0) )
				{
					HBRUSH hbrush=CreateSolidBrush(CLR_BLUE);
					FillRect( m_hdcList,&rect,hbrush);
					DeleteObject(hbrush);
				}
//				else if(!TGridData::m_StrData[datrow].bkcolor[col]) FillRect( m_hdcList,&rect,GetStockObject(WHITE_BRUSH));
				else if(!TGridData::m_StrData[datrow].bkcolor[col]);
				else FillRect( m_hdcList,&rect,(HBRUSH)GetStockObject(GRAY_BRUSH));

				// �̈���Ɏ��܂镶���������o��
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

				// �A�N�e�B�u�Z���̎������ʏ���(�I�����[�h���Z���̏ꍇ�j
				if( (col == m_ActCel.col && datrow ==m_ActCel.row && !m_info.selecttype) ||
				// �A�N�e�B�u�Z���̎������ʏ���(�I�����[�h���s�̏ꍇ�j
					(datrow ==m_ActCel.row && m_info.selecttype && m_ActCel.col >=0) )
				{
					old = SetTextColor( m_hdcList, CLR_WHITE );
					DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);
					SetTextColor( m_hdcList, old );
				}
				// �ʏ�̃Z��
				else{
					// �F��
					if(TGridData::m_StrData[datrow].color[col]==1){
						old = SetTextColor( m_hdcList, CLR_RED );
					}
					DrawText(m_hdcList,txtBuf,-1,&rect,uFlags);
					if(TGridData::m_StrData[datrow].color[col]==1){
						SetTextColor( m_hdcList, old );
					}
				}
				//����x���W
				xPos += m_ColInfo[col].width;
			}
			// �f�[�^�ŏI�s�܂ł������炨���
			if( datrow >= TGridData::m_LastRowNo ) break;
			// �f�[�^�s�𑝂₷
			datrow++;
		}
	}
	if( m_info.vScroll )
		SetVScroll();
	if( m_info.hScroll )
		SetHScroll();

	// �S�X�V
	SetRect(&rect,0,0,m_ListRect.right,m_ListRect.bottom);
	InvalidateRect(m_ListWnd,&rect,TRUE);
//	SetFocus(m_ListWnd);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::SetVScroll(void)
//	�^�C�v�Fprivate
//	�@�\�@�F�����X�N���[���o�[�̐ݒ�
//	�߂�l�F������TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetVScroll(void)
{
	 if(!m_WindowInit) return(FALSE);

	SCROLLINFO sinfo;

	// �f�[�^�s�����\���\�s���ȉ��̎��̓X�N���[���o�[���g�p�s�ɂ���
	if( TGridData::m_LastRowNo < m_DispRows ){
		EnableScrollBar(m_ListWnd,SB_VERT,ESB_DISABLE_BOTH);
		return(TRUE);
	}


	// ���݂̐ݒ��ǂݍ���
	sinfo.cbSize=sizeof(SCROLLINFO);
	sinfo.fMask = SIF_PAGE|SIF_RANGE;
	GetScrollInfo( m_ListWnd, SB_VERT,&sinfo );

	// �ݒ肷��
	sinfo.nMin = 0;
	sinfo.nMax = TGridData::m_LastRowNo-m_DispRows+1;
	sinfo.nPage = 1;
	SetScrollInfo( m_ListWnd, SB_VERT,&sinfo ,TRUE);
	EnableScrollBar(m_ListWnd,SB_VERT,ESB_ENABLE_BOTH);
	return(TRUE);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::SetVScroll(void)
//	�^�C�v�Fprivate
//	�@�\�@�F�����X�N���[���o�[�̐ݒ�
//	�߂�l�F������TRUE
//		*Create�����͗񐔂�ω��������Ƃ��ȊO����̌Ăяo���͂��Ă͂����܂���
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetHScroll(void)
{
	 if(!m_WindowInit) return(FALSE);

	int i;
	SCROLLINFO sinfo;
	long xTotal=0;


	// �񕝂��\���\����菭�Ȃ��ꍇ�̓X�N���[���o�[���g�p�s�ɂ���
	// �A�����[�̗񂪂O�Ԃ̎��̂݃`�F�b�N
	if( m_Left == m_FixCols ){
//	if( m_Left == 0 ){
		for(i=0;i<TGridData::m_TotalCols;i++){
			xTotal += m_ColInfo[i].width;
		}
		xTotal+= m_info.rowtitleWidth;

		//  �����v <= �N���C�A���g�̈捇�v
		if( xTotal <= m_ListRect.right ){
			EnableScrollBar(m_ListWnd,SB_HORZ,ESB_DISABLE_BOTH);
			return(TRUE);
		}
	}


	// ���݂̐ݒ��ǂݍ���
	sinfo.cbSize=sizeof(SCROLLINFO);
	sinfo.fMask = SIF_PAGE|SIF_RANGE;
	GetScrollInfo( m_ListWnd, SB_HORZ,&sinfo );

	// �ݒ肷��
	sinfo.nMin = 0;
	sinfo.nMax = TGridData::m_TotalCols-1;
	sinfo.nPage = 1;
	SetScrollInfo( m_ListWnd, SB_HORZ,&sinfo ,TRUE);
	EnableScrollBar(m_ListWnd,SB_HORZ,ESB_ENABLE_BOTH);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	void CGridViewCtrl::EvVScroll(HWND hWnd,WORD event,WORD lastPos)
//	�^�C�v�Fprivate
//	�@�\�@�F�����X�N���[���o�[�̐���
//	�����P�F�E�C���h�E�n���h��
//	�����Q�F���������C�x���g
//	�����R�F�܂݂��͂Ȃ������̂܂݂̈ʒu
//---------------------------------------------------------------------------------------------------------------------
void CGridViewCtrl::EvVScroll(HWND hWnd,WORD event,WORD lastPos)
{
	long nextTopRow;			// �X�N���[����ɐ擪�ɗ���s

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

	// �X�N���[�����K�v���H
	if( nextTopRow < 0)		return;


	if(nextTopRow+m_DispRows-1 >TGridData::m_LastRowNo )
		nextTopRow = TGridData::m_LastRowNo -m_DispRows+1;

	SCROLLINFO sinfo;

	// �X�N���[�������擾
	sinfo.cbSize=sizeof(SCROLLINFO);
	sinfo.fMask = SIF_POS;
	GetScrollInfo(hWnd, SB_VERT,&sinfo );

	// �X�N���[���ʒu���ύX�Ȃ��ꍇ
	if( sinfo.nPos == nextTopRow && event !=SB_THUMBPOSITION) return;

	// �ҏW���̎��͏I��点��
	EndEdit(1);

	sinfo.nPos = nextTopRow;
	SetScrollInfo( hWnd, SB_VERT,&sinfo ,TRUE);
	m_Top = nextTopRow + m_FixRows;


	// �c�X�N���[�����邱�Ƃɂ���āA�A�N�e�B�u�Z�����\���ł��Ȃ��Ȃ�����A�A�N�e�B�u�Z���̍s��ύX
	if( m_ActCel.row < m_Top ) m_ActCel.row = m_Top;
	else if( m_ActCel.row > m_Top+m_DispRows-1-m_FixRows ) m_ActCel.row = m_Top+m_DispRows-m_FixRows-1;
	DrawData();
	SetFocus(m_ListWnd);


}

//---------------------------------------------------------------------------------------------------------------------
//	void CGridViewCtrl::EvHScroll(HWND hWnd,WORD event,WORD lastPos)
//	�^�C�v�Fprivate
//	�@�\�@�F�����X�N���[���o�[�̐���
//	�����P�F�E�C���h�E�n���h��
//	�����Q�F���������C�x���g
//	�����R�F�܂݂��͂Ȃ������̂܂݂̈ʒu
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

	// �X�N���[�������擾
	sinfo.cbSize=sizeof(SCROLLINFO);
	sinfo.fMask = SIF_POS;
	GetScrollInfo(hWnd, SB_HORZ,&sinfo );


	// �X�N���[���ʒu���ύX�Ȃ��ꍇ
	if( sinfo.nPos == nextPos && event !=SB_THUMBPOSITION) return;

	// �ҏW���̎��͏I��点��
	EndEdit(1);

	sinfo.nPos = nextPos;
	SetScrollInfo( hWnd, SB_HORZ,&sinfo ,TRUE);
	m_Left = nextPos;

	// �E�̗�ԍ����擾
	nextPos =0;
	for( i = 0; i < m_FixCols; i++ )
		nextPos += m_ColInfo[i].width;
	for( i = m_Left; i < TGridData::m_TotalCols; i++ ){
		nextPos += m_ColInfo[i].width;

		// �\���\����z������break
		if( nextPos + m_info.rowtitleWidth >= m_ListRect.right ){
			break;
		}
		// �����Ō��݂̉E����擾����
		m_Right = i;
	}

	// ���X�N���[�����邱�Ƃɂ���āA�A�N�e�B�u�Z�����\���ł��Ȃ��Ȃ�����A�A�N�e�B�u�Z���̍s��ύX
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
//	�^�C�v�Fprivate
//	�@�\�@�F�A�N�e�B�u�ȃZ����ύX����
//	�����P�F�A�N�e�B�u�ɂ���Z���̗�
//	�����Q�F�A�N�e�B�u�ɂ���Z���̍s
//	�����R�F�����Ăяo�����(�ȗ���),FALSE�Ŏg�p���Ă�
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::MoveActiveCel(long newCol,long newRow,BOOL internal)
{
	long i;

	// �ړ���̃Z���͗L�����H
	if( newRow > TGridData::m_LastRowNo || newCol >=TGridData::m_TotalCols ||
		newRow < m_FixRows )
//		newRow < m_FixRows || newCol < m_FixCols )
		return(FALSE);

	// �ړ��悪��ʂɓ����Ă������ꍇ�̓X�N���[��
	if( ( newCol >=m_FixCols && newCol < m_Left) || newCol > m_Right || 
		(( TGridData::m_LastRowNo >= m_DispRows ) && ( newRow < m_Top || newRow >= m_Top + m_DispRows-m_FixRows )) )
	{
		// �ړ����s
		if( !AutoScroll(newCol,newRow) ) return(FALSE);
		// ����������A�ĕ`��
		// �A�N�e�B�u�Z���p�̃����o�ϐ����X�V
		if( newCol >= m_FixCols ) m_ActCel.col = newCol;		// �Œ�� 04.06.21
		m_ActCel.row = newRow;
		DrawData();
		if( internal) EvMove(m_Left,m_Top);
	}

	// �X�N���[�����Ȃ��ꍇ
	else{
		// ���O�̃A�N�e�B�u�Z���̐F�����ɖ߂�
		// �Z���I�����[�h
		if( !m_info.selecttype )
			DrawCel(m_ActCel.col,m_ActCel.row);

		// �s�I�����[�h
		else{
			for(i=0; i< m_FixCols;i++)
				DrawCel(i,m_ActCel.row);
			for(i=m_Left; i<=m_Right+2;i++)
				DrawCel(i,m_ActCel.row);
		}

		// �A�N�e�B�u�Z���p�̃����o�ϐ����X�V
		if( newCol >= m_FixCols ) m_ActCel.col = newCol;		// �Œ�� 04.06.21
//		m_ActCel.col = newCol;
		m_ActCel.row = newRow;

		// ���]�ŕ\��
		// �Z���I�����[�h
		if( !m_info.selecttype )
			DrawCel(m_ActCel.col,m_ActCel.row,TRUE);
		// �s�I�����[�h
		else{
			for(i=0; i<m_FixCols;i++)	DrawCel(i,m_ActCel.row,TRUE);
			for(i=m_Left; i<=m_Right+2;i++)	DrawCel(i,m_ActCel.row,TRUE);
		}
	}

	// �ړ������֐�
	EvMoveCelEnd( m_ActCel.col, m_ActCel.row);

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	void CGridViewCtrl::KillActCel(void)
//	�^�C�v�Fpublic
//	�@�\�@�F���݃A�N�e�B�u�ȃZ���̕\�����A�t�H�[�J�X����������ԂɕύX
//---------------------------------------------------------------------------------------------------------------------
void CGridViewCtrl::KillActCel(void)
{
	int x;

	if( m_info.selecttype){					// �I������@�O���Z���P�ʁA�P���s�P��
		for(x=0;x<TGridData::m_TotalCols; x++)
			LostFocusCel(x,m_ActCel.row);
	}
	else LostFocusCel(m_ActCel.col,m_ActCel.row);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVieCtrl::SetLeftX(long newX)
//	�^�C�v�Fpublic
//	�@�\�@�F���̃Z���ԍ����A�w�肵���Z���ɕύX����
//	�����P�F�A�N�e�B�u�ɂ���Z���̗�
//	�����Q�F�A�N�e�B�u�ɂ���Z���̍s
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::SetLeftX(long newCol)
{
	// �ړ���̃Z���͗L�����H
	if( newCol >=TGridData::m_TotalCols || newCol < m_FixCols )
		return(FALSE);

	if( newCol == m_Left ) return(TRUE);

	// �ҏW���̎��͏I��点��
	EndEdit(1);

	SCROLLINFO sinfo;
	BOOL redraw=FALSE;					// �ĕ`��T�C��

	// ---------------------------------------------�����ʒu--------------------------------------
	// �ĕ`�掞�́A���Z���ԍ����Z�o
	if( !m_info.selecttype){
		// ���X�N���[�����g�p�s�̏ꍇ�͈ړ������Ȃ�
		if( !m_info.hScroll ) return(FALSE);

		// ���݂̍��ʒu��ύX
		m_Left = newCol;

		// �X�N���[���o�[�̐ݒ�
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
//	�^�C�v�Fprivate
//	�@�\�@�F�w��̃Z������ʂɏo��悤�Ɏ����X�N���[��
//	�����P�F�A�N�e�B�u�ɂ���Z���̗�
//	�����Q�F�A�N�e�B�u�ɂ���Z���̍s
// ���̌��DrawData()�����s���Ȃ��ƁA�ĕ`��͂���܂���
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::AutoScroll(long newCol,long newRow)
{
	// �ړ���̃Z���͗L�����H
	if( newRow > TGridData::m_LastRowNo || newCol >=TGridData::m_TotalCols ||
		newRow < m_FixRows || newCol < m_FixCols )
		return(FALSE);

	long xTotal;
	int i;
	SCROLLINFO sinfo;
	BOOL redraw=FALSE;					// �ĕ`��T�C��
	// ---------------------------------------------�����ʒu--------------------------------------
	// �ĕ`�掞�́A���Z���ԍ����Z�o
	if( (newCol < m_Left || newCol > m_Right ) &&  !m_info.selecttype){
		// ���X�N���[�����g�p�s�̏ꍇ�͈ړ������Ȃ�
		if( !m_info.hScroll ) return(FALSE);

		// ���݂̍��ʒu��菬����
		if( newCol < m_Left ) m_Left = newCol;
		if( newCol < m_FixCols ) m_Left = m_FixCols;

		// ���݂̉E�ʒu���傫��
		if( newCol > m_Right ){
			m_Right = newCol;
			xTotal = m_info.rowtitleWidth;
			// �Œ�s
			for( i = 0; i <m_FixCols; i++ )
				xTotal += m_ColInfo[i].width;
			for( i = m_Right; i >=0; i-- ){
				xTotal += m_ColInfo[i].width;
				if(xTotal >=m_ListRect.right) break;
			}
			m_Left = i+1;

		}
		// �X�N���[���o�[�̐ݒ�
		sinfo.cbSize=sizeof(SCROLLINFO);
		sinfo.fMask = SIF_POS;
		sinfo.nPos = m_Left;
		SetScrollInfo( m_ListWnd, SB_HORZ,&sinfo ,TRUE);

	}

	// ---------------------------------------------�����ʒu--------------------------------------
	if( ( TGridData::m_LastRowNo >= m_DispRows ) &&
		( (newRow < m_Top && newRow >=m_FixRows) || newRow >= m_Top + m_DispRows - m_FixRows)){
		// �c�X�N���[���s�̎��͉������Ȃ�
		if( !m_info.vScroll ) return(FALSE);

		if( newRow < m_Top ) m_Top = newRow;
		else m_Top = newRow - m_DispRows + m_FixRows + 1;

		// �X�N���[���o�[���ړ�
		SCROLLINFO sinfo;

		// �X�N���[�������擾
		sinfo.cbSize=sizeof(SCROLLINFO);
		sinfo.fMask = SIF_POS;
		sinfo.nPos = m_Top;
		SetScrollInfo( m_ListWnd, SB_VERT,&sinfo ,TRUE);
	}

	// �X�N���[��OK
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::MoveCel(int mvMode)
//	�^�C�v�Fpublic
//	�@�\�@�F�A�N�e�B�u�Z���̈ꂱ�܈ړ�
//	�����P�F�ړ����[�h
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
				// �擪���O�̍s�ɂȂ�����A�O�̗�̍Ō���ɂ���
				if( nextRow < m_FixRows ){
					nextRow = TGridData::m_LastRowNo;
					nextCol --;
					if( nextCol < m_FixCols )	nextCol = m_FixCols;
//					if( nextCol < 0 )	nextCol = 0;
				}
				break;

			case MV_DOWN:
				nextRow++;
				// �擪����̍s�ɂȂ�����A���̗�̐擪�ɂ���
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
//	�^�C�v�Fpublic
//	�@�\�@�F�w���̃f�[�^��S�ăo�b�t�@�ɃR�s�[����
//	�����P�F�R�s�[�Ώۂ̗�ԍ�
//	�߂�l�F������TRUE�A���s��FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::ColsCopy(long targetCol)
{
	// �f�[�^��̃R�s�[
	if( !TGridData::ColCopy(targetCol,FALSE) ) return(FALSE);

	// ������R�s�[
	m_ColInfo[TGridData::m_TotalCols].width=m_ColInfo[targetCol].width;
	m_ColInfo[TGridData::m_TotalCols].position=m_ColInfo[targetCol].position;
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::ColsPaste(long targetCol,BOOL insert,BOOL redraw)
//	�^�C�v�Fpublic
//	�@�\�@�F�w���̃f�[�^��S�ăo�b�t�@�ɃR�s�[����
//	�����P�F�R�s�[�Ώۂ̗�ԍ�
//	�����Q�F�}��=TRUE �㏑��=FALSE
//	�����R�F�ĕ`��w�� TRUE=�ĕ`�悷��
//	�߂�l�F������TRUE�A���s��FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::ColsPaste(long targetCol,BOOL insert,BOOL redraw)
{
	long ret;
	long prevCols;
	int offset,i;
	COLUMNPOSITIONINFO *info;

	// ����t���O�̗�
	prevCols = TGridData::m_TotalCols;
	// �f�[�^��̒���t��
	if( ( ret =TGridData::ColPaste(targetCol,insert)) <0 ) return(FALSE);

	// �񂪂Ȃ��Ƃ��̏���
	if( targetCol < 0 ) targetCol = 0;
	if( prevCols <= 0){
		prevCols = 1;
		targetCol = 0;
		insert = FALSE;
	}

	// ����
	// �㏑��or���炷�K�v�Ȃ�
	if( targetCol > prevCols || !insert){
		if( targetCol > prevCols )
				targetCol = prevCols;

		// �R�s�[����
		m_ColInfo[targetCol].width=m_ColInfo[TGridData::m_TotalCols].width;
		m_ColInfo[targetCol].position=m_ColInfo[TGridData::m_TotalCols].position;
	}

	// �}��
	else{
		// ���炷�ꍇ�i��MAX�j
		if( prevCols == TGridData::m_TotalCols ) offset =1;

		// !��max
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

	// �񐔂ɕω����������Ƃ��̓X�N���[���o�[���X�V
	if( TGridData::m_TotalCols != prevCols )
		SetHScroll();

	// �ĕ`��w�肠��
	if( redraw)
		DrawData();

	return(TRUE);

}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::ColsCut(long targetCol ,BOOL redraw)
//	�^�C�v�Fpublic
//	�@�\�@�F�w���̃f�[�^���폜���A�R�s�[�o�b�t�@�ɕۑ�����
//	�����P�F�폜�Ώۂ̗�ԍ�
//	�����R�F�ĕ`��w�� TRUE=�ĕ`�悷��
//	�߂�l�F������TRUE�A���s��FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::ColsCut(long targetCol ,BOOL redraw)
{
	long prevCols;
	COLUMNPOSITIONINFO *info;
	int i;

	// ����t���O�̗�
	prevCols = TGridData::m_TotalCols;

	// �f�[�^��̃J�b�g
	if( !TGridData::ColCopy(targetCol,TRUE) ) return(FALSE);

	// ������R�s�[
	m_ColInfo[prevCols].width=m_ColInfo[targetCol].width;
	m_ColInfo[prevCols].position=m_ColInfo[targetCol].position;

	// �J�b�g

	for(i=targetCol;i<TGridData::m_TotalCols-1;i++){
		m_ColInfo[i].width=m_ColInfo[i+1].width;
		m_ColInfo[i].position=m_ColInfo[i+1].position;
	}

//	memmove( &m_ColInfo[targetCol], &m_ColInfo[targetCol+1] ,( prevCols-targetCol-1) * sizeof(COLUMNPOSITIONINFO) );
	info = (COLUMNPOSITIONINFO*)realloc(m_ColInfo,sizeof(COLUMNPOSITIONINFO) * (m_TotalCols+1));
	m_ColInfo=info;

	// �A�N�e�B�u�Z���ɑ΂��鏈�u
	if( m_ActCel.col >= TGridData::m_TotalCols )
		m_ActCel.col=TGridData::m_TotalCols-1;

	// �X�N���[��bar
	SetHScroll();

	// �ĕ`��w�肠��
	if( redraw)
		DrawData();


	return(TRUE);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::DataClear( BOOL redraw )
//	�^�C�v�Fpublic
//	�@�\�@�F���ݕێ����Ă���S�f�[�^���N���A����
//	�����P�F�ĕ`��w�� TRUE=�ĕ`�悷��
//	�߂�l�FTRUE=�N���A���{
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

	// �X�N���[�������擾
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
//	�^�C�v�Fpublic
//	�@�\�@�F�V�K�ɗ��}������
//	�����P�F�}�������ԍ�
//	�����P�F�}�������̕�
//	�����P�F�}�������̕����̈ʒu
//	�����R�F�ĕ`��w�� TRUE=�ĕ`�悷��
//	�߂�l�F������TRUE�A���s��FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::AddNewCol(long targetCol,long width, short pos,BOOL redraw)
{
	long ret;
	long prevCols;
	int offset,i;
	BOOL insert = TRUE;
	COLUMNPOSITIONINFO colinfo,*info;


	// ����t���O�̗�
	prevCols = TGridData::m_TotalCols;
	// �f�[�^��̒���t��
	if( ( ret =TGridData::AddCol(targetCol)) <0 ) return(FALSE);

	// �}��������̃f�t�H���g�l
	memset( &colinfo, 0, sizeof(colinfo) );
	colinfo.width = width;
	colinfo.position = pos;

	// �񂪂Ȃ��Ƃ��̏���
	if( targetCol < 0 ) targetCol = 0;
	if( prevCols <= 0){
		prevCols = 1;
		targetCol = 0;
		insert = FALSE;
	}

	// ����
	// �㏑��or���炷�K�v�Ȃ�
	if( targetCol > prevCols || !insert){
		if( targetCol > prevCols )		targetCol = prevCols;

		// �R�s�[����
		memcpy( &m_ColInfo[targetCol], &colinfo, sizeof(COLUMNPOSITIONINFO) );
	}

	// �}��
	else{
		// ���炷�ꍇ�i��MAX�j
		if( prevCols == TGridData::m_TotalCols ) offset =1;

		// !��max
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

	// �񐔂ɕω����������Ƃ��̓X�N���[���o�[���X�V
	if( TGridData::m_TotalCols != prevCols )	SetHScroll();

	// �ĕ`��w�肠��
	if( redraw)		DrawData();


	return(TRUE);
}

///**********************************************************************************************************************
//										virtual �����o�֐�
///**********************************************************************************************************************

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvEndEdit( long col, long row ,char *txt ,short *move ,long *nextCol,long *nextRow)
//	�^�C�v�Fvirtual	(private)
//	�@�\�@�F�ҏW���I��������ɌĂ΂��֐�
//	�����P�F�ҏW���̗̂�ԍ�
//	�����Q�F�ҏW���̂̍s�ԍ�
//	�����R�F���͂��ꂽ�e�L�X�g
//	�����S�F�P���I����A�e�L�X�g�{�b�N�X���J�����܂܃Z�����ړ�������ꍇ�i�ړ���Z���͈����̂T�ƂU�j
//			�Q���I����A�e�L�X�g�{�b�N�X����ăZ�����ړ�����ꍇ�i�ړ���Z���͈����̂T�ƂU�j
//			�O���I����A�e�L�X�g�{�b�N�X����Ĉړ����Ȃ��ꍇ
//	�����T�F�ړ���̗�ԍ�
//	�����U�F�ړ���̍s�ԍ�
//	�����V�F�F
//	�߂�l�F�ҏW���I�������遁TRUE�A�ҏW���p�������遁FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvEndEdit( long col, long row ,char *txt ,short *move ,long *nextCol,long *nextRow,char *color)
{
	*color=0;
	*move=NOT_MOVE;
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvClick( long col,long row)
//	�^�C�v�Fvirtual	(private)
//	�@�\�@�F�N���b�N���ꂽ���ɌĂ΂��֐�
//	�����P�F�N���b�N���ꂽ��ԍ�
//	�����Q�F�N���b�N���ꂽ�s�ԍ�
//	�߂�l�F�f�t�H���g���������遁TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvClick( long col,long row)
{
	return(TRUE);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvHeaderClick( long col)
//	�^�C�v�Fvirtual	(private)
//	�@�\�@�F�s�w�b�_���N���b�N���ꂽ���ɌĂ΂��֐�
//	�����P�F�s�w�b�_���N���b�N���ꂽ��ԍ�
//	�߂�l�F�f�t�H���g���������遁TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvHeaderClick( long col)
{
	return(TRUE);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvDblClick( long col,long row)
//	�^�C�v�Fvirtual	(private)
//	�@�\�@�F�_�u���N���b�N���ꂽ���ɌĂ΂��֐�
//	�����P�F�N���b�N���ꂽ��ԍ�
//	�����Q�F�N���b�N���ꂽ�s�ԍ�
//	�߂�l�F�f�t�H���g���������遁TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvDblClick( long col,long row)
{
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvClick( long col,long row)
//	�^�C�v�Fvirtual	(private)
//	�@�\�@�F�ҏW�J�n���O�ɌĂ΂��֐�
//	�����P�F��ԍ�
//	�����Q�F�s�ԍ�
//	�߂�l�F�ҏW�������遁TRUE�@�����Ȃ�=FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvStartEdit( long col, long row )
{
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridVeiwCtrl::EvClick( long col,long row)
//	�^�C�v�Fvirtual	(private)
//	�@�\�@�F�L�[�������ꂽ�Ƃ��ɌĂ΂��֐�
//	�����P�F��ԍ�
//	�����Q�F�s�ԍ�
//	�߂�l�F�f�t�H���g�����������遁TRUE���Ȃ�=FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvKeyDown( WPARAM keycode )
{
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CGridViewCtrl::BOOL EvMoveCelEnd( long col, long row)
//	�^�C�v�Fvirtual	(private)
//	�@�\�@�F�A�N�e�B�u�Z�����ړ��������ɌĂ΂��֐�
//	�����P�F�ړ���̗�ԍ�
//	�����Q�F�ړ���s�ԍ�
//	�߂�l�F�f�t�H���g�����������遁TRUE���Ȃ�=FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL CGridViewCtrl::EvMoveCelEnd( long col, long row)
{
	return(TRUE);
}


//---------------------------------------------------------------------------------------------------------------------
//LONG CGridViewCtrl::DoCommand(HWND hDlg ,UINT message, WPARAM wParam, LPARAM lParam)
//	�^�C�v�FPublic
//	�@�\�@�F���b�Z�[�W���[�v
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

			// �s�I�����[�h
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
			// �s�I�����[�h
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
			// IME�̐؊��L�[�̓f�t�H���g��windproc��
			if(wParam==0xe5){
				UINT a,b;
				// �X�L�����R�[�h���擾
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
			// ���z�֐����Ăяo��
			ret = EvKeyDown(wParam);
			// �f�t�H����������
			if( ret ){
				switch(wParam){
					// ���E�ړ��̓Z���I�����[�h���̂ݗL��
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
						// �ҏW������Ă鎞
						if( m_info.readonly ==FALSE ){
							// ���z�֐����Ăяo��
							ret = EvStartEdit(m_ActCel.col,m_ActCel.row);
							// �f�t�H����������
							if(ret){
								m_EditMode = FALSE;
								StartEdit(m_ActCel.col,m_ActCel.row);
							}
						}	
						break;

					// IME�֌W�S�^���p�L�[
					case 0x00f4:
					case 0x00f3:
					case 0x0019:
						message=WM_KEYDOWN;
						return DefWindowProc(hWind, message, wParam, lParam);

					// �ҏW�𖳎�����L�[
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
						// �ҏW������Ă鎞
						if( m_info.readonly ==FALSE ){
							// ���l�֌W�̃L�[�������ꍇ�͓��͂Ɉڂ�
							if(IsKeyNumeric(&asc,wParam,lParam)){
								// ���z�֐����Ăяo��
								ret = EvStartEdit(m_ActCel.col,m_ActCel.row);
								// �f�t�H����������
								if(ret){
									m_EditMode = FALSE;

									// Ime��Off�̎��͕ϊ������̂𑗂�
									StartEdit(m_ActCel.col,m_ActCel.row,asc,TRUE);
								}
							}
						}	
						break;
				}
			}
			break;
		// �E�B���h�E�ĕ`��(�����f�o�C�X�R���e�L�X�g�̓��e���R�s�[����)
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
//	�^�C�v�Fprivate
//	�����P�F�A�X�L�[����o�b�t�@�i�o�́j
//	�����P�F�`�F�b�N����L�[�R�[�h
//	�����P�F�`�F�b�N����L�[�̃X�L�����R�[�h
//	�@�\�@�F�����ꂽ�L�[�R�[�h�����l�֘A('0'-'9',','��)������,ASCII�ϊ�����
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
	// IME��������Ȃ��̂ŁA�`�F�b�N����
	if(ret==0){

		// �X�L�����R�[�h���擾
		a=scanCd << 8;
		b=a >> 24;

		// �X�L�����R�[�h���牼�z�L�[�R�[�h���擾 
		a = MapVirtualKey(b,1);

		// IME����̃L�[
		switch(b){
			case 0x70:
			case 0x7b:
			case 0x79:
				return(FALSE);
			// 10keypad( MapVirtualKey()�ŕϊ����Ă���Ȃ�)
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

		// �ēxascii�ϊ�
		ret=ToAscii( a,  b && 0xFF,keys, &wRet,0 );
		if( !ret || ret == 2 ) return(FALSE);
		asci=(char)wRet & 0xFF;
		*theAsci=asci;
		// 2�޲ĕ����̓`�F�b�N�Ȃ�
		return(TRUE);
	}
/*
	// ascii�ϊ������������A���l�֘A���`�F�b�N
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

		// ���̑���NG
		default:
			return(FALSE);
	}

	*theAsci=asci;
	return(TRUE);

}


//---------------------------------------------------------------------------------------------------------------------
//LONG CGridViewCtrl::EditProc(HWND hDlg ,UINT message, WPARAM wParam, LPARAM lParam)
//	�^�C�v�FPublic
//	�@�\�@�Fedit�R���g���[���p�̃��b�Z�[�W���[�v
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
//	�^�C�v�FPublic
//	�@�\�@�F�L�[�t�b�N
//---------------------------------------------------------------------------------------------------------------------
LONG CGridViewCtrl::KeyProc( int nCode, WPARAM wParam, LPARAM lParam)
{
	LPMSG msg=(LPMSG)lParam;

	if( nCode <0 )
		return( CallNextHookEx(m_KeyHook,nCode,wParam,lParam));

	if( msg->message == WM_SYSKEYDOWN || msg->message==WM_KEYDOWN ){
		msg->message = WM_USER_KEYDOWN;				// ���b�Z�[�W��������
	}
	return(0);
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
static LRESULT CALLBACK GridProc(HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam)
{
	int i;

	// �E�C���h�E����
	for( i=0; i<clsCnt;i++)
		if(clsArray[i]->m_ListWnd == hwnd ) break;

	// �N���X�̃����o�֐��ɏ������킽��
	return( clsArray[i]->DoCommand(hwnd, message, wParam, lParam) );
}

// �Z���ҏW�p���̓R���g���[���̃C���X�^���X
static LPARAM CALLBACK GridEditProc(HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam)
{
	int i;

	// �E�C���h�E����
	for( i=0; i<clsCnt;i++)
		if(clsArray[i]->m_EditWnd == hwnd ) break;


	// �N���X�̃����o�֐��ɏ������킽��
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

	// �E�C���h�E����
	for( i=0; i<clsCnt;i++){
		if(clsArray[i]->m_ListWnd == msg->hwnd ){
			// �N���X�̃����o�֐��ɏ������킽��
			clsArray[i]->KeyProc(nCode, wParam, lParam);
			break;
		}
	}
	return(0);
}


