//================================================================================================
//	FileName	:TXlsx.H
//	Class		:TXlsx 
// 				:TXls�N���X�݊���xlsx�`������N���X
//	�쐬��		:2013.12.04	Coded by coba
//================================================================================================
#include "TXlsx.h"
#include <stdio.h>

//========================================================================================================
//	TXls::TXlsx(void)
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F�R���X�g���N�^�B���ɉ�������
//========================================================================================================
TXls::TXls(void)
{
	// �e�p�����[�^�̃C�j�V�����C�Y
	m_Open=FALSE;
	m_Err = 0;
	m_xid = NULL;
}

//========================================================================================================
//	TXls::~TXlsx()
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

	// �n���h���擾
	if(FALSE==XlsxCreate(&m_xid)){
		m_Err = -3;		return(FALSE);
	}

	// �V�K�쐬���[�h
	if( overwrite ){
		if( !XlsxCreateBook(m_xid,filename, ver2007, sheetMax, NULL)){
			SetErrCode();return(FALSE);
		}
	}

	// �����t�@�C���I�[�v�����[�h
	else{
		if(!XlsxOpenBook(m_xid,filename,NULL, NULL)){
			SetErrCode();return(FALSE);
		}
	}


	// ����
	m_Open=TRUE;
	return(TRUE);
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

	// �n���h���擾
	if(FALSE==XlsxCreate(&m_xid)){
		m_Err = -3;		return(FALSE);
	}

	// �I�[�v��
	if(!XlsxOpenBook(m_xid,filename,templatefile, NULL)){
		SetErrCode();return(FALSE);
	}

	// ����
	m_Open=TRUE;
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
	// �n���h���擾
	if(FALSE==XlsxCreate(&m_xid)){
		m_Err = -3;		return(FALSE);
	}

	if(!XlsxReadBook(m_xid,filename, NULL)){
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
	if(m_xid==NULL) return;

	// ����
	XlsxCloseBook(m_xid,save);
	XlsxRelease(m_xid);

	m_Open=FALSE;
	m_Err=0;
	m_xid=NULL;
}


//========================================================================================================
//	BOOL AddSheet(long Page);
//  public
//�@�@�\�@�F����ݒ��̃G�N�Z���t�@�C���ɃV�[�g��ǉ�����
//	�����P�F�ǉ�����V�[�g��
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::AddSheet(long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsxAddSheet(m_xid,Page,-1);
	if(m_Err==FALSE){
		SetErrCode();return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL DelSheet(long SheetNo,long Page);
//  public
//�@�@�\�@�F����ݒ��̃G�N�Z���t�@�C���̃V�[�g���폜����
//	�����P�F�폜����V�[�gNo.(0�`)
//	�����Q�F�V�[�g��
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::DelSheet(long SheetNo,long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsxDelSheet(m_xid,SheetNo,Page);

	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL SetSheetName(char* Sheetname);
//  public
//�@�@�\�@�F���݃J�����g�̃V�[�g�ɖ��O��t����
//	�����P�F�G�N�Z���V�[�g��(NULL�ŏI���g���q�t���B)
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::SetSheetName(char* SheetName)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsxPutSheetName(m_xid,SheetName);
	if(m_Err==FALSE){
		SetErrCode();return(FALSE);
	}
	return TRUE;
}

//========================================================================================================
//	BOOL GetSheetName(long SheetNo,char* SheetName);
//  public
//�@�@�\�@�F�G�N�Z���t�@�C���̃V�[�g�̖��O���擾����
//	�����P�F�G�N�Z���V�[�gNo.
//	�����Q�F�G�N�Z���V�[�g���o�b�t�@(NULL�ŏI���g���q�t���B64�o�C�g�ȏ�m��)
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::GetSheetName(long SheetNo,char* SheetName,int size)
{
	int preNo;
	char preSheet[100];

	if( m_Open!=TRUE ) 	return(FALSE);

	// ���݂̃V�[�g���擾
	memset( preSheet,0,sizeof(preSheet));
	m_Err = XlsxGetSheetName(m_xid,preSheet);
	if(m_Err==FALSE){
		SetErrCode();return(FALSE);
	}
	// ���݂̃V�[�g�����擾
	preNo=XlsxGetSheetNo2(m_xid,preSheet);
	if( preNo < 0 ){
		SetErrCode();return(FALSE);
	}

	// ���݂̃V�[�g�ƈႤ�ꍇ�͐؂�ւ���
	if( preNo != SheetNo ){
		m_Err = XlsxPutSheetNo(m_xid,SheetNo);
		if(m_Err==FALSE){
			SetErrCode();return(FALSE);
		}
	}

	// �w�肳�ꂽ�V�[�g�����擾
	m_Err = XlsxGetSheetName(m_xid,SheetName);
	if(m_Err==FALSE){
		SetErrCode();return(FALSE);
	}

	// ���݂̃V�[�g�ƈႤ�ꍇ�͖߂�
	if( preNo != SheetNo ){
		m_Err = XlsxPutSheetNo(m_xid,preNo);
		if(m_Err==FALSE){
			SetErrCode();return(FALSE);
		}
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

	m_Err = XlsxPutSheetNo(m_xid,Page);
	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
}
//========================================================================================================
//	BOOL SetActiveSheet(long Page);
//  public
//�@�@�\�@�F�A�N�e�B�u�ȃV�[�g�̑I��(�f�[�^���Z�b�g���邽�߂̃V�[�g�I���ł͂Ȃ��A�G�N�Z���J�������̃A�N�e�B�u�ȃV�[�g�j
//	�����P�F�V�[�g��No.-1 (Sheet�S��������R)
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::SetActiveSheet(long Page)
{
	if( m_Open!=TRUE ) 	return(FALSE);

	m_Err = XlsxPutActiveSheet(m_xid,Page);
	if(m_Err==FALSE){
		SetErrCode();
		return(FALSE);
	}
	return TRUE;
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

	// ������̃`�F�b�N
	if( data==NULL ){
		m_Err = -5;		return(FALSE);
	}

	// ������̃`�F�b�N���̂Q
	if( !strlen(data)){
		m_Err = -5;		return(FALSE);
	}

	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxPutString(m_xid,data)){
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
	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}
	// ��������
	if(!XlsxPutLong(m_xid,data)){
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
	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}
	// ��������
	if(!XlsxPutDouble(m_xid,data)){
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
	// XlsxPutValue()�̎������Ⴄ�̂ŁA���̂܂ܕ�����Ƃ��ď������� 2013.12.04 coba
	return(this->WriteStr(data,x,y));
/*
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}
	// �˂�̂���
	if(!XlsSelectBook(m_xid)){
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
	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxPutValue(data,POS(x,y,x,y))){
		SetErrCode();//	return(FALSE);
		XlsGetErrorNo(&err);
		return FALSE;
	}

	return(TRUE);
*/
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

	// ������̃`�F�b�N
	if( data==NULL ){
		m_Err = -5;		return(FALSE);
	}

	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// �ǂݍ���
	if(!XlsxGetString(m_xid,data)){
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
	long val;

	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// �ǂݍ���
	val = XlsxGetLong(m_xid);
	*data = val;

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
	double val;

	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// �ǂݍ���
	val=XlsxGetDouble(m_xid);
	*data=val;
	return(TRUE);
}

void TXls::SetErrCode(void)
{
	// DLL�̃G���[�R�[�h���擾���āA���̃N���X��err�p�����ޕϐ��ɃZ�b�g����
	m_Err=XlsxGetErrorNo(m_xid);

//char aaa[100];
//memset(aaa,0,sizeof(aaa));
//sprintf(aaa,"%ld",m_Err);
//MessageBox(NULL,aaa,"",MB_OK);

}

//========================================================================================================
// BOOL TXls::SetFormat( char* format,long x,long y)
// �\�������ݒ�
// �����P:����(�G�N�Z����)
// �����Q�F�ݒ肷��Z��X�ʒu
// �����Q�F�ݒ肷��Z��Y�ʒu
//========================================================================================================
BOOL TXls::SetFormat( char* format,long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,-1,-1)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutFormat(m_xid,format)){
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

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutFontPoint(m_xid,fsize)){
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

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutFontName(m_xid,font)){
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
		if(!XlsxPutDefFontPoint( m_xid,fsize)){
			SetErrCode();
			return(FALSE);
		}
	}

	// �t�H���g�̐ݒ�
	if(!XlsxPutDefFontName( m_xid,fname)){
		SetErrCode();
//		return(FALSE);		// ���s���Ă��G���[�ɂ��Ȃ�
	}

	return(TRUE);
}


//========================================================================================================
// BOOL TXls::SetFormat( char* format,long x,long y)
// �t�H���g�����w��
// �����P:�����X�^�C��
//	xlsxcrt.h
//		xfuNone  0  �����Ȃ�(�f�t�H���g�l)  
//		xfuNormal  1  ��d����  
//		xfuDouble  2  ��d����  
// 	TXls�݊�
//		fuNone  0  �����Ȃ�(�f�t�H���g�l)  
//		fuNormal  1  ��d����  
//		fuDouble  2  ��d����  
// �����Q�F�ݒ肷��Z��X�ʒu
// �����Q�F�ݒ肷��Z��Y�ʒu
//========================================================================================================
BOOL TXls::SetFontLine( long ltype,long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �Z���ړ�
	if( !XlsxPos(m_xid,x,y,x,y)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutFontULine(m_xid,ltype)){
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

	// �Z���ړ�
	if( !XlsxPos(m_xid,0,y,0,y)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxPutRowHeight(m_xid,h)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

// �Z����
BOOL TXls::SetColWidth( long w ,long x)
{
	return(this->SetColWidthEx((double)w,x));

}

// �Z����
BOOL TXls::SetColWidthEx( double w ,long x)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �Z���ړ�
	if( !XlsxPos(m_xid,x,0,x,0)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxPutColWidth(m_xid,w)){
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

	// �͈͎w��
	if( !XlsxPos(m_xid,sx,sy,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutJoint(m_xid,onoff)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}


//========================================================================================================
// BOOL TXls::SetFontColor(long color, long x,long y)
// �t�H���g�F�̐ݒ�
// �����P:�F(xlsxcrt.h�Q��)
//		�ȉ���Xls�݊��p
//		#define xcDefault				_xclAuto				// ����
//		#define xcBlack					_xclBlack				// ��
//		#define xcWhite					_xclWhite				// ��
//		#define xcRed					_xclRed					// ��
//		#define xcGreen					_xclGreen				// ��
//		#define xcBlue					_xclBlue				// ��
//		#define xcYellow				_xclYellow				// ��
//		#define xcPink					_xclPink				// �s���N
//		#define xcCyan					_xclCyan				// ��
//========================================================================================================
BOOL TXls::SetFontColor(long color, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �͈͎w��
	if( !XlsxPos(m_xid,x,y,x,y)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutFontColor(m_xid,color)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
// BOOL TXls::SetCellColor(long color, long x,long y)
// �Z���w�i�F�̐ݒ�
// �����P:�F(xlsxcrt.h�Q��)
//		�ȉ���Xls�݊��p
//		#define xcDefault				_xclAuto				// ����
//		#define xcBlack					_xclBlack				// ��
//		#define xcWhite					_xclWhite				// ��
//		#define xcRed					_xclRed					// ��
//		#define xcGreen					_xclGreen				// ��
//		#define xcBlue					_xclBlue				// ��
//		#define xcYellow				_xclYellow				// ��
//		#define xcPink					_xclPink				// �s���N
//		#define xcCyan					_xclCyan				// ��
//========================================================================================================
BOOL TXls::SetCellColor(long color, long x,long y)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �͈͎w��
	if( !XlsxPos(m_xid,x,y,x,y)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutBackColor(m_xid,color)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}


//========================================================================================================
// �y�[�W�ݒ�
// �����P:�y�[�W�T�C�X "A3","A4","A5","B5"
// �����Q:�p���̌��� TRUE=�c
// �����R�F�w�b�_�]��
// �����S�F�t�b�_�]��
//========================================================================================================
BOOL TXls::SetPageProperty(char *Size,BOOL Portrait,double HdrMergin,double FrtMergin)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
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

	if(!XlsxPageMode(m_xid, PageSize, -1, Orientation , (int)HdrMergin, (int)FrtMergin)){
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

	// ��������
	if(!XlsxSetMargin( m_xid,Left, Right, Top, Bottom)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
//========================================================================================================
//	BOOL  TXls::DrawLine(long LineStyle,long bt,long sx, long sy, long ex,long ey)
//	Cell	�ΏۂƂȂ�Z���͈͂�����Char�^�̃|�C���^��ݒ肵�܂��B�J�����g�Z����Ώۂɂ���ꍇ�́ANULL��ݒ肵�ĉ������B
//	���� LineStyle xlsxcrt.h�Q��
//	�ȉ��͌݊��p
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
BOOL  TXls::DrawLine(long LineStyle,long bt,long sx, long sy, long ex,long ey,int color/*=_xclBlack*/)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �͈͎w��
	if( !XlsxPos(m_xid,sx,sy,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutBox( m_xid,LineStyle, bt,color)){
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

	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	// �͈͎w��
	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutPosHorz( m_xid,horz)){
		SetErrCode();	return(FALSE);
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

	// �͈͎w��
	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;
	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}


	// ��������
	if(!XlsxAttrPutPosVert( m_xid,vert)){
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

	// �͈͎w��
	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;
	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutFit( m_xid,onoff)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}
//				2013.04.18 coba �����̂��X�^�C���ǉ�
//========================================================================================================
//	BOOL TXls::SetFontStyle(long horz,long x,long y,long ex,long ey)
//  public
//�@�@�\�@�F�Z�����t�H���g�X�^�C���w�肷��B��S�E�T�������ȗ�����ƁA�Q�E�R�Ŏw�肵���Z���݂̂�ݒ�
//	�����P�F�X�^�C��
//	#define XL_XSNORMAL         0x00            // �t�H���g�X�^�C�� �m�[�}��
//	#define XL_XSBOLD           0x01            //                  �{�[���h
//	#define XL_XSITALIC         0x02            //                  �C�^���b�N
//	#define XL_XSSTRIKE         0x04            //                  �������
//	#define XL_XSUP             0x08            //                  ��t��
//	#define XL_XSDOWN           0x10            //                  ���t��
//	�����Q�F�ݒ肷��Z���̂w�ʒu(�J�n)
//	�����R�F�ݒ肷��Z���̂x�ʒu(�J�n)
//	�����S�F�ݒ肷��Z���̂w�ʒu(�I��)���ȗ���
//	�����T�F�ݒ肷��Z���̂x�ʒu(�I��)���ȗ���
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::SetFontStyle(long style,long x,long y,long ex,long ey)
{
	// ���I�[�v��
	if(!m_Open){
		m_Err = -4;
		return(FALSE);
	}

	// �͈͎w��
	if( ex == -1 ) ex=x;
	if( ey == -1 ) ey=y;

	if( !XlsxPos(m_xid,x,y,ex,ey)){
		SetErrCode();	return(FALSE);
	}

	// ��������
	if(!XlsxAttrPutFontStyle( m_xid,style)){
		SetErrCode();	return(FALSE);
	}

	return(TRUE);
}

//========================================================================================================
// BOOL TXls::CopySheet( long posSource, long posInsert, LPCTSTR pszSheetName )
//  public 2007.03.07 matsuda�ǉ�
//�@�@�\�@�F�V�[�g�̃R�s�[�@�\
//	�����P�F�R�s�[���V�[�g�ԍ�
//	�����Q�F�R�s�[��V�[�g�ԍ�
//	�����R�F�V�[�g��
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::CopySheet( long posSource, long posInsert, LPCTSTR pszSheetName )
{
	if( posSource < 0 ) return FALSE;
	if( posInsert < 0 ) return FALSE;
	
	if( XlsxCopySheet( m_xid,posSource,posInsert,pszSheetName ) == FALSE ) return FALSE;
	return TRUE;
}

//========================================================================================================
//	�s�̑���Ɋւ���֐�
//	����  �Fsy �J�n�s or �R�s�[���s
//	����  �Fdy �R�s�[��s
//	����  �Fsy �s��
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::RowDelete(long sy, long lCount)
{
	int ret=0;
	ret=XlsxRowDelete(m_xid,sy,lCount);
	return(ret);
}
BOOL TXls::RowClear(long sy, long lCount)
{
	int ret=0;
	ret=XlsxRowClear(m_xid,sy,lCount);
	return(ret);
}
BOOL TXls::RowInsert(long sy, long lCount)
{
	int ret=0;
	ret=XlsxRowInsert(m_xid,sy,lCount);
	return(ret);
}
BOOL TXls::RowCopy(long sy, long dy)
{
	int ret=0;
	ret=XlsxRowCopy(m_xid,sy,dy);
	return(ret);
}
BOOL TXls::RowPaste(long dy)
{
	int ret=0;
	ret=XlsxRowPaste(m_xid,dy);
	return(ret);
}

//========================================================================================================
//	��̑���Ɋւ���֐�
//	����  �Fsy �J�n�� or �R�s�[����
//	����  �Fdy �R�s�[���
//	����  �Fsy ��
//	�Ԓl�@�FTRUE=�����AFALSE=���s
//========================================================================================================
BOOL TXls::ColumnDelete(long sx, long lCount)
{
	int ret=0;
	ret=XlsxColumnDelete(m_xid,sx,lCount);
	return(ret);
}
BOOL TXls::ColumnClear(long sx, long lCount)
{
	int ret=0;
	ret=XlsxColumnClear(m_xid,sx,lCount);
	return(ret);
}
BOOL TXls::ColumnInsert(long sx, long lCount)
{
	int ret=0;
	ret=XlsxColumnInsert(m_xid,sx,lCount);
	return(ret);
}
BOOL TXls::ColumnCopy(long sx, long dx)
{
	int ret=0;
	ret=XlsxColumnCopy(m_xid,sx,dx);
	return(ret);
}
BOOL TXls::ColumnPaste(long dx)
{
	int ret=0;
	ret=XlsxColumnPaste(m_xid,dx);
	return(ret);
}
// ����͈͂̎w�� 2008.12.10�ǉ�
int TXls::SetPrnArea(int sx, int sy, int ex,int ey)
{
	return(XlsxPrintArea(m_xid,sx,sy,ex,ey));
}
//====================================================================
//	�֐����Fvoid TXls::XlsViewPrint(BOOL default_pr,char *filename,int startsheet,int endsheet)
//	�����P�F�f�t�H���g�v�����^���g�p����ꍇ:TRUE
//	�����Q�F�������t�@�C����
//	�����R�F����J�n�V�[�g(0����J�n) 
//	�����S�F����I���V�[�g
//	2010.12.23 ���� �ǉ�
//====================================================================
void TXls::XlsViewPrint(BOOL default_pr,char *filename,int startsheet,int endsheet)
{
	// �r���[�����Ȃ��̂łł��܂���B�B�B�B�B
/*
	// �f�t�H���g�v�����^���g�p���Ȃ��ꍇ = �v�����^�I������ꍇ
	if(default_pr==FALSE){
		if( ViewSetPrint(NULL) == FALSE ) return;
	}
	// �t�@�C�����w��
	ViewPutUrlName(NULL, filename);
	// ������s
	ViewPrint(NULL ,startsheet ,endsheet);
*/
}
