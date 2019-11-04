//================================================================================================
//	NOK Fanatic Class Libraly
//	FileName	:TXls.cpp
//	Class		:TXls 
// 				:MS-Excel97(bif80)�`���t�@�C��/�������݃N���X
//	�쐬��		:2000.01.08	Coded by coba
//				2000.07.4 TAKE �V�[�g�ǉ��Ȃ�тɃV�[�g���Z�b�g�֐���ǉ�
//================================================================================================
#include "TXls.h"
#include <stdio.h>

//========================================================================================================
//	TXls::TXls(void)
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F�R���X�g���N�^�B���ɉ�������
//========================================================================================================
TXls::TXls(void)
{
	// �e�p�����[�^�̃C�j�V�����C�Y
	m_Open=FALSE;
	m_Err = 0;

}

//========================================================================================================
//	TXls::~TXls()
//�@�@�\�@�F�t�@�C���n���h���̉���B
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//========================================================================================================
TXls::~TXls()
{
	Close(FALSE);
}


//========================================================================================================
//	BOOL OpenXls(char* filename, BOOL overwrite);
//  public
//�@�@�\�@�F�������݂悤�ɃG�N�Z���t�@�C�����J��
//	�����P�F�I�[�v������G�N�Z���u�b�N��(NULL�ŏI���g���q�t���B)
//	�����Q�F�t�@�C���I�[�v�����[�h 
//			TRUE=filename�����݂���ꍇ�͈�x�N���A���Ă���J���A�����͐V�K�ɊJ�� 
//			FALSE=filename�����݂���ꍇ�͂��̂܂܊J���A�����ꍇ�͊J���Ȃ�
//	�����R�F�쐬����V�[�g���i�ȗ��l=3�j
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::OpenXls(char* filename, BOOL overwrite, long sheetMax)
{

	m_Err = 0;

	// ���łɃI�[�v�����Ă���
	if( m_Open ){
		m_Err = -1;		return(FALSE);
	}

	// �t�@�C�����̃`�F�b�N
	if( filename==NULL ){
		m_Err = -2;		return(FALSE);
	}

	// �t�@�C�����̃`�F�b�N���̂Q
	if( !strlen(filename)){
		m_Err = -2;		return(FALSE);
	}

	// �V�K�쐬���[�h
	if( overwrite ){
		if( !XlsCreateBook(filename, ver97, sheetMax, &m_XlsID)){
			SetErrCode();return(FALSE);
		}
	}

	// �����t�@�C���I�[�v�����[�h
	else{
		if(!XlsOpenBook(filename,NULL, &m_XlsID)){
			SetErrCode();return(FALSE);
		}
	}


	// ����
	m_Open=TRUE;
	return(TRUE);
}

//========================================================================================================
//	BOOL AddSheet(long Page);
//  public
//�@�@�\�@�F����ݒ��̃G�N�Z���t�@�C���ɃV�[�g��ǉ�����
//	�����P�F�G�N�Z���V�[�g��
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::AddSheet(long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsAddSheet(Page);
	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL DelSheet(long SheetNo,long Page);
//  public
//�@�@�\�@�F����ݒ��̃G�N�Z���t�@�C���̃V�[�g���폜����
//	�����P�F�G�N�Z���V�[�gNo.
//	�����Q�F�G�N�Z���V�[�g��
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::DelSheet(long SheetNo,long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsDelSheet(SheetNo,Page);

//char aaa[100];
//memset(aaa,0,sizeof(aaa));
//sprintf(aaa,"ret=%ld  s=%ld   p=%ld",m_Err,SheetNo,Page);
//MessageBox(NULL,aaa,"",MB_OK);

	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL SetSheetName(char* Sheetname);
//  public
//�@�@�\�@�F�G�N�Z���t�@�C���̃V�[�g�ɖ��O��t����
//	�����P�F�G�N�Z���V�[�g��(NULL�ŏI���g���q�t���B)
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::SetSheetName(char* SheetName)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsPutSheetName(SheetName);
	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL GetSheetName(long SheetNo,char* SheetName);
//  public
//�@�@�\�@�F�G�N�Z���t�@�C���̃V�[�g�̖��O���擾����
//	�����P�F�G�N�Z���V�[�gNo.
//	�����Q�F�G�N�Z���V�[�g��(NULL�ŏI���g���q�t���B)
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::GetSheetName(long SheetNo,char* SheetName,int size)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsGetSheetName(SheetNo,SheetName,size);

	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL SetSheetNo(long Page);
//  public
//�@�@�\�@�F�G�N�Z���t�@�C���u�b�N���V�[�g�̑I��
//	�����P�F�V�[�g��No.-1 (Sheet�S��������R)
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::SetSheetNo(long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsSheetNo(Page);
	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}
//========================================================================================================
//	BOOL TXls::OpenXlsByTemplate(char* filename, char* templatefile,BOOL overwrite)
//  public
//�@�@�\�@�F�����̃t�@�C�����e���v���[�g�ɂ��āA�������ݗp�ɃG�N�Z���t�@�C�����J��
//	�����P�F�I�[�v������G�N�Z���u�b�N��(NULL�ŏI���g���q�t���B)
//	�����Q�F�e���v���[�g�ƂȂ�G�N�Z���u�b�N��(NULL�ŏI���g���q�t���B)
//	�����R�F�t�@�C���I�[�v�����[�h 
//			TRUE=filename�����݂���ꍇ�͈�x�N���A���ĊJ���A�����͐V�K�ɊJ�� 
//			FALSE=filename�����݂���ꍇ�͊J���Ȃ�
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::OpenXlsByTemplate(char* filename, char* templatefile,BOOL overwrite)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind;

	m_Err = 0;

	// ���łɃI�[�v�����Ă���
	if( m_Open ){
		m_Err = -1;		return(FALSE);
	}

	// �t�@�C�����̃`�F�b�N
	if( filename==NULL || templatefile==NULL){
		m_Err = -2;		return(FALSE);
	}

	// �t�@�C�����̃`�F�b�N���̂Q
	if( !strlen(filename) || !strlen(templatefile)){
		m_Err = -2;		return(FALSE);
	}

	// overwrite���Ȃ��ꍇ�́Afilename�̗L���`�F�b�N
	if(!overwrite){
		memset( &fd, 0, sizeof(fd) );
		fd.dwFileAttributes = FILE_ATTRIBUTE_ARCHIVE;

		// ���łɑ��݂���ꍇ�̓G���[�Ƃ���
		hFind=FindFirstFile(filename,&fd);
		if( hFind != INVALID_HANDLE_VALUE){
			FindClose(hFind);
			m_Err = -3;		return(FALSE);
		}
		FindClose(hFind);
	}

	// �I�[�v��
	if(!XlsOpenBook(filename,templatefile, &m_XlsID)){
		SetErrCode();return(FALSE);
	}

	// ����
	m_Open=TRUE;
	return(TRUE);
}

//========================================================================================================
//	void TXls::Close(BOOL save);
//  public
//	�����P�FTRUE=�Z�[�u���ăN���[�Y FALSE=�Z�[�u�����ɃN���[�Y
//�@�@�\�@�F�J���Ă���t�@�C�����N���[�Y����B
//========================================================================================================
void TXls::Close(BOOL save)
{
	// ���I�[�v��
	if(!m_Open) return;

	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return;
	}


	// ����
	XlsCloseBook(TRUE);

	m_Open=FALSE;
	m_Err=0;
}


//========================================================================================================
//	BOOL TXls::WriteStr(char* data, long x,long y)
//  public
//�@�@�\�@�F�Z���ԍ����w�肵�āA������f�[�^����������
//	�����P�F�������݃f�[�^(NULL�ŏI)
//	�����Q�F�Z���̂w�ʒu
//	�����R�F�Z���̂x�ʒu
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::WriteStr(char* data, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ������̃`�F�b�N
	if( data==NULL ){
		m_Err = -5;		return(FALSE);
	}

	// ������̃`�F�b�N���̂Q
	if( !strlen(data)){
		m_Err = -5;		return(FALSE);
	}

	// ��������
	if(!XlsPutStr(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::WriteLong(long data, long x,long y)
//  public
//�@�@�\�@�F�Z���ԍ����w�肵�āAlong�^�f�[�^����������
//	�����P�F�������݃f�[�^(NULL�ŏI)
//	�����Q�F�Z���̂w�ʒu
//	�����R�F�Z���̂x�ʒu
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::WriteLong(long data, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}
	// ��������
	if(!XlsPutLong(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::WriteDbl(double data, long x,long y)
//  public
//�@�@�\�@�F�Z���ԍ����w�肵�āAdouble�^�f�[�^����������
//	�����P�F�������݃f�[�^(NULL�ŏI)
//	�����Q�F�Z���̂w�ʒu
//	�����R�F�Z���̂x�ʒu
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::WriteDbl(double data, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsPutDouble(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::WriteVariant(char* data, long x,long y)
//  public
//�@�@�\�@�F�Z���ԍ����w�肵�āA������f�[�^����������
//	�����P�F�������݃f�[�^(NULL�ŏI)
//	�����Q�F�Z���̂w�ʒu
//	�����R�F�Z���̂x�ʒu
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::WriteVariant(char* data, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ������̃`�F�b�N
	if( data==NULL ){
		m_Err = -5;		return(FALSE);
	}

	// ������̃`�F�b�N���̂Q
	if( !strlen(data)){
		m_Err = -5;		return(FALSE);
	}

	// ��������
	if(!XlsPutValue(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL ReadXls(char* filename);
//  public
//�@�@�\�@�F�ǂݍ��݂悤�ɃG�N�Z���t�@�C�����J��
//	�����P�F�I�[�v������G�N�Z���u�b�N��(NULL�ŏI���g���q�t���B)
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::ReadXls(char* filename)
{

	m_Err = 0;

	// ���łɃI�[�v�����Ă���
	if( m_Open ){
		m_Err = -1;		return(FALSE);
	}

	// �t�@�C�����̃`�F�b�N
	if( filename==NULL ){
		m_Err = -2;		return(FALSE);
	}

	// �t�@�C�����̃`�F�b�N���̂Q
	if( !strlen(filename)){
		m_Err = -2;		return(FALSE);
	}

	// �����t�@�C���I�[�v�����[�h
	else{
		if(!XlsReadBook(filename, &m_XlsID)){
			SetErrCode();return(FALSE);
		}
	}

	// ����
	m_Open=TRUE;
	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::ReadStr(char* data, int size, long x,long y)
//  public
//�@�@�\�@�F�Z���ԍ����w�肵�āA������f�[�^��ǂݍ���
//	�����P�F�ǂݍ��݃f�[�^(NULL�ŏI)
//	�����Q�F�Z���̂w�ʒu
//	�����R�F�Z���̂x�ʒu
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::ReadStr(char* data, int size, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ������̃`�F�b�N
	if( data==NULL ){
		m_Err = -5;		return(FALSE);
	}

	// ������̃`�F�b�N���̂Q
//	if( !strlen(data)){
//		m_Err = -5;		return(FALSE);
//	}

	// �ǂݍ���
	if(!XlsGetStr(data,size,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::ReadLong(long data, long x,long y)
//  public
//�@�@�\�@�F�Z���ԍ����w�肵�āAlong�^�f�[�^��ǂݍ���
//	�����P�F�ǂݍ��݃f�[�^(NULL�ŏI)
//	�����Q�F�Z���̂w�ʒu
//	�����R�F�Z���̂x�ʒu
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::ReadLong(long* data, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// �ǂݍ���
	if(!XlsGetLong(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
//	BOOL TXls::ReadDbl(double data, long x,long y)
//  public
//�@�@�\�@�F�Z���ԍ����w�肵�āAdouble�^�f�[�^��ǂݍ���
//	�����P�F�ǂݍ��݃f�[�^(NULL�ŏI)
//	�����Q�F�Z���̂w�ʒu
//	�����R�F�Z���̂x�ʒu
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::ReadDbl(double* data, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// �ǂݍ���
	if(!XlsGetDouble(data,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

void TXls::SetErrCode(void)
{
	// DLL�̃G���[�R�[�h���擾���āA���̃N���X��err�p�����ޕϐ��ɃZ�b�g����
	XlsGetErrorNo(&m_Err);

//char aaa[100];
//memset(aaa,0,sizeof(aaa));
//sprintf(aaa,"%ld",m_Err);
//MessageBox(NULL,aaa,"",MB_OK);

}


BOOL TXls::SetFormat( char* format,long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsPutFormat(format,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

// �t�H���g�T�C�Y�̎w�� 03.04.23
BOOL TXls::SetFontSize( double fsize,long x,long y,long ex,long ey)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;
	// ��������
	if(!XlsPutFontPoint(fsize,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
// �t�H���g�̎w�� 03.06.04
BOOL TXls::SetFont( char* font,long x,long y,long ex,long ey)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// ��������
	if(!XlsPutFontName(font,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

// �t�H���g�����w��
BOOL TXls::SetFontLine( long ltype,long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsPutFontULine(ltype,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
BOOL TXls::SetRowHeight( double h ,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsPutRowHeight(h,POS(0,y,-1,-1))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

BOOL TXls::SetColWidth( long w ,long x)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	//if(!XlsPutColWidth(w,POS(x,0,-1,-1))){
	if(!XlsPutColWidth(w,POS(x,0,x,0))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

BOOL TXls::CellJoint( BOOL onoff ,long sx, long sy, long ex,long ey)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsPutJoint(onoff,POS(sx,sy,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

BOOL TXls::SetFontColor(long color, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

//#define xcDefault       XL_XCDEFAULT        // �J���[�p���b�g ����
//#define xcBlack         XL_XCBLACK          //                ��
//#define xcWhite         XL_XCWHITE          //                ��
//#define xcRed           XL_XCRED            //                ��
//#define xcGreen         XL_XCGREEN          //                ��
//#define xcBlue          XL_XCBLUE           //                ��
//#define xcYellow        XL_XCYELLOW         //                ��
//#define xcPink          XL_XCPINK           //                �s���N
//#define xcCyan          XL_XCCYAN           //                ��

//#define XLP_DEFAULT          0  // ����
//#define XLP_BLACK            8  // ��
//#define XLP_WHITE            9  // ��
//#define XLP_RED             10  // ��
//#define XLP_GREEN           11  // ��
//#define XLP_BLUE            12  // ��
//#define XLP_YELLOW          13  // ��
//#define XLP_MAGENTA         14  // ��
//#define XLP_CYAN            15  // ��



	// ��������
	if(!XlsPutFontColor(color,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

BOOL TXls::SetCellColor(long color, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

//#define xcDefault       XL_XCDEFAULT        // �J���[�p���b�g ����
//#define xcBlack         XL_XCBLACK          //                ��
//#define xcWhite         XL_XCWHITE          //                ��
//#define xcRed           XL_XCRED            //                ��
//#define xcGreen         XL_XCGREEN          //                ��
//#define xcBlue          XL_XCBLUE           //                ��
//#define xcYellow        XL_XCYELLOW         //                ��
//#define xcPink          XL_XCPINK           //                �s���N
//#define xcCyan          XL_XCCYAN           //                ��

//#define XLP_DEFAULT          0  // ����
//#define XLP_BLACK            8  // ��
//#define XLP_WHITE            9  // ��
//#define XLP_RED             10  // ��
//#define XLP_GREEN           11  // ��
//#define XLP_BLUE            12  // ��
//#define XLP_YELLOW          13  // ��
//#define XLP_MAGENTA         14  // ��
//#define XLP_CYAN            15  // ��



	// ��������
	if(!XlsPutBackColor(color,POS(x,y,x,y))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

BOOL TXls::SetPageProperty(char *Size,BOOL Portrait,double HdrMergin,double FrtMergin)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	long PageSize,Orientation;

	if(!memcmp(Size,"A3",2)) PageSize = 8;
	else if(!memcmp(Size,"A4",2)) PageSize = 9;
	else if(!memcmp(Size,"A5",2)) PageSize = 11;
	else if(!memcmp(Size,"B4",2)) PageSize = 12;
	else if(!memcmp(Size,"B5",2)) PageSize = 13;
	else  PageSize = 9;

	if( Portrait==TRUE ) Orientation = 2;
	else 	Orientation = 0;

	if(!XlsPageMode( PageSize, -1, Orientation , HdrMergin, FrtMergin)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

// �]���������l��mm�P�ʂŐݒ肵�܂��B�ȗ�����ꍇ�́A-1���w��
BOOL  TXls::SetMargin(short Left,short Right,short Top,short Bottom)
{
	
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsSetMargin( Left, Right, Top, Bottom)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
//========================================================================================================
//	BOOL XlsPutBox(long LineStyle,long bt,char *Cell)
//	Box�r���̏������݂����܂��B
//	Cell	�ΏۂƂȂ�Z���͈͂�����Char�^�̃|�C���^��ݒ肵�܂��B�J�����g�Z����Ώۂɂ���ꍇ�́ANULL��ݒ肵�ĉ������B
//	���� LineStyle�̐ݒ�l�͎��̂Ƃ���ł��B
//
//	�萔	�l	���e
//	lsNone	0x00000000	�r���Ȃ��i�f�t�H���g�l�j
//	lsNormal	0x01000000	����
//	lsThick	0x02000000	����
//	lsBroken	0x03000000	�j��
//	lsDot	0x04000000	�_��
//	lsThick2	0x05000000	�ɑ���
//	lsDouble	0x06000000	��d��
//	lsSlender	0x07000000	�׎���
//	lsMidBroken	0x08000000	���j��*
//	lsSlnChain1	0x09000000	�׈�_����*
//	lsMidChain1	0x0A000000	����_����*
//	lsSlnChain2	0x0B000000	�ד�_����*
//	lsMidChain2	0x0C000000	����_����*
//	lsMidChains	0x0D000000	����_�΍���*
//	xcDefault	0	�����i�f�t�H���g�l�j
//	xcBlack	8	��
//	xcWhite	9	��
//	xcRed	10	��
//	xcGreen	11	��
//	xcBlue	12	��
//	xcYellow	13	���F
//	xcPink	14	�s���N
//	xcCyan	15	���F

//	���� bt�̐ݒ�l�͎��̂Ƃ���ł��B

//	�萔	�l	���e

//	btBox	0	�����i�f�t�H���g�l�j
//	btLtc	1	�i�q��
//	btOver	2	�㉡��
//	btUnder	3	������
//	btLeft	4	���c��
//	btRight	5	�E�c��
//========================================================================================================
BOOL  TXls::DrawLine(long LineStyle,long bt,long sx, long sy, long ex,long ey)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsPutBox( LineStyle, bt, POS(sx,sy,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}



//				2002.10.15 coba �����̈ʒu�w���ǉ�
//========================================================================================================
//	BOOL TXls::PutPosHorz(long horz,long x,long y,long ex,long ey)
//  public
//�@�@�\�@�F�Z���̐����ʒu���w�肷��B��S�E�T�������ȗ�����ƁA�Q�E�R�Ŏw�肵���Z���݂̂�ݒ�
//	�����P�F�����ʒu
//				phNormal	20	�W���i�f�t�H���g�l�j
//				phLeft	21	���l��
//				phCenter	22	��������
//				phRight	23	�E�l��
//				phLoop	24	�J��Ԃ�
//				phBothEven	25	���[����
//				phSelCenter	26	�I��͈͓��Œ���
//				phEven	27	�ϓ����t
//	�����Q�F�ݒ肷��Z���̂w�ʒu(�J�n)
//	�����R�F�ݒ肷��Z���̂x�ʒu(�J�n)
//	�����S�F�ݒ肷��Z���̂w�ʒu(�I��)���ȗ���
//	�����T�F�ݒ肷��Z���̂x�ʒu(�I��)���ȗ���
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::PutPosHorz(long horz,long x,long y,long ex,long ey)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// ��������
	if(!XlsPutPosHorz( horz,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//	2005.04.14 coba �V�[�g�̃f�t�H���gFont�ݒ� SetDefFont()��ǉ�
//========================================================================================================
//	BOOL TXls::SetDefFont(char* fname,double fsize=-1.0);
//  public
//�@�@�\�@�F�u�b�N�̃f�t�H���g�t�H���g��ݒ�
//	�����P�F�t�H���g��(NULL�ŏI)
//	�����Q�F�t�H���g�T�C�Y(1.0�`409.0)�B1.0�ȉ����w�肷��ƁA�f�t�H���g(11pt)��ݒ�B���ȗ���
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::SetDefFont(char* fname,double fsize)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �T�C�Y���L���Ȃ�΁A�Z�b�g����
	if( fsize >=1.0 && fsize <=409.0){
		if(!XlsPutDefFontPoint( fsize)){
			SetErrCode();
			return(FALSE);
		}
	}

	// �t�H���g�̐ݒ�
	if(!XlsPutDefFontName( fname)){
		SetErrCode();
//		return(FALSE);		// ���s���Ă��G���[�ɂ��Ȃ�
	}

	return(TRUE);
}
//				2005.05.19 coba �����ʒu�w��PutPosVert()��ǉ�
//========================================================================================================
//	BOOL TXls::PutPosVert(long vert,long x,long y,long ex,long ey)
//  public
//�@�@�\�@�F�Z���̐����ʒu���w�肷��B��S�E�T�������ȗ�����ƁA�Q�E�R�Ŏw�肵���Z���݂̂�ݒ�
//	�����P�F�����ʒu
//		pvUp	0	��l��
//		pvCenter	1	��������
//		pvDown	2	���l�߁i�f�t�H���g�l�j
//		pvBothEven	3	���[����
//		pvEven	4	�ϓ����t
//	�����Q�F�ݒ肷��Z���̂w�ʒu(�J�n)
//	�����R�F�ݒ肷��Z���̂x�ʒu(�J�n)
//	�����S�F�ݒ肷��Z���̂w�ʒu(�I��)���ȗ���
//	�����T�F�ݒ肷��Z���̂x�ʒu(�I��)���ȗ���
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::PutPosVert(long vert,long x,long y,long ex,long ey)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// ��������
	if(!XlsPutPosVert( vert,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
//========================================================================================================
//	BOOL TXls::PutFit(BOOL onoff,long x,long y,long ex,long ey)
//  public 2006.08.31 coba�ǉ�
//�@�@�\�@�F�u�k�����đS�̂�\���v���w�肷��
//	�����P�FON=�ݒ�,OFF=����
//	�����Q�F�ݒ肷��Z���̂w�ʒu(�J�n)
//	�����R�F�ݒ肷��Z���̂x�ʒu(�J�n)
//	�����S�F�ݒ肷��Z���̂w�ʒu(�I��)���ȗ���
//	�����T�F�ݒ肷��Z���̂x�ʒu(�I��)���ȗ���
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::PutFit(BOOL onoff,long x,long y,long ex,long ey)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_XlsID)){
		SetErrCode();	return(FALSE);
	}

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// ��������
	if(!XlsPutFit( onoff,POS(x,y,ex,ey))){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
