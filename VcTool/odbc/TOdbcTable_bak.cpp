//=====================================================================================================================
//
//	FileName	:TOdbcTable.cpp
//	Class		:TOdbcTable
//				odbc�e�[�u��������N���X
//	�쐬��		:2008.12.23	Coded by matsuda
//========================================================================================
//	�C��
//
//========================================================================================
#include "todbctable.h"
#include "stdio.h"
#include "getnow.h"
//---------------------------------------------------------------------------------------------------------------------
//	TOdbcTable::TOdbcTable()
//	�^�C�v�FPublic
//	�@�\�@�F�f�t�H���g�R���X�g���N�^�B�����o�ϐ��̏�����
//---------------------------------------------------------------------------------------------------------------------
TOdbcTable::TOdbcTable()
{
	m_scrollEnable=FALSE;
	m_ColmnCount = 0;
	m_hStmt = NULL;
	m_selOK=FALSE;
	m_LogMode=FALSE;
}
//---------------------------------------------------------------------------------------------------------------------
//	TOdbcTable::~TOdbcTable()
//	�^�C�v�FPublic
//	�@�\�@�F�f�X�g���N�^�B�I�u�W�F�N�g�⃁�����̉��
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
TOdbcTable::~TOdbcTable()
{
	if( m_hStmt != NULL ) SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt );
}
//---------------------------------------------------------------------------------------------------------------------
//	void TOdbcTable::Reset(void)
//	�^�C�v�FPublic
//	�@�\�@�F�X�e�[�g�����g�n���h����Delete
//	�����P�F�Ȃ�
//	�߂�l�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
void TOdbcTable::Reset(void)
{
	if( m_hStmt != NULL )SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt );
	
	m_hStmt=NULL;
	
	m_selOK=FALSE;
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::Init(void)
//	�^�C�v�FPublic
//	�@�\�@�F�X�e�[�g�����g�n���h���̎擾
//	�����P�F�Ȃ�
//	�߂�l�F0�F���� ����ȊO�F���s
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::Init(void)
{
	SQLRETURN ret;
	//�X�e�[�g�����g�n���h���̎擾
	ret = SQLAllocHandle( SQL_HANDLE_STMT, m_hConn , &m_hStmt );
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,m_hStmt, "Init00");
		m_hStmt = NULL;
		return -1;
	}

	//���J�[�\����Scrollable�ɐݒ肵�Ȃ���Next�����g���Ȃ��悤�ł��B
	//���ݒ肷��ƒx���Ȃ�̂Œ��ӂ��ĉ�����
	if( m_scrollEnable == TRUE )
		SQLSetStmtAttr(m_hStmt,SQL_ATTR_CURSOR_SCROLLABLE,(SQLPOINTER)SQL_SCROLLABLE ,0);
	else
		SQLSetStmtAttr(m_hStmt,SQL_ATTR_CURSOR_SCROLLABLE,(SQLPOINTER)SQL_NONSCROLLABLE ,0);

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	TOdbcTable::SetDB( SQLHANDLE *hconn )
//	�^�C�v�FPublic
//	�@�\�@�F�R�l�N�V�����n���h����Ⴄ���X�e�[�g�����g�n���h�����쐬����
//	�����P�F�R�l�N�V�����n���h��(DB�ڑ��ς݂ł��邱��)
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::SetDB( SQLHANDLE *hconn )
{
	m_hConn = *hconn;
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::SetScrollEnabe(void)
//	�^�C�v�FPublic
//	�@�\�@�FMovePrev�Ȃǂ��g�����߂ɃJ�[�\����Scrollable�ɂ���
//	�����P�FTRUE;�ݒ肷�� FALSE�F�ݒ��������
//	���J�[�\����Scrollable�ɐݒ肵�Ȃ���Next�����g���Ȃ��悤�ł��B�ݒ肷��ƒx���Ȃ�̂Œ��ӂ��ĉ�����
//	
//	���ӁI�I�@Select��Insert�Ȃǂ̃X�e�[�g�����g�쐬�O�ɐݒ肵�ĉ�����
//---------------------------------------------------------------------------------------------------------------------
void TOdbcTable::SetScrollEnable(BOOL on)
{
	m_scrollEnable = on;
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::Next(void)
//	�^�C�v�FPublic
//	�@�\�@�F�J�����g���R�[�h�̈ړ�
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::MoveNext(void)
{
	if(m_hStmt == NULL) return(-100);

	if( m_selOK==FALSE )return(-1);

	int ret;

	ret = SQLFetchScroll( m_hStmt,SQL_FETCH_NEXT,0 );
	if( ret == SQL_ERROR || ret == SQL_INVALID_HANDLE || ret == SQL_NO_DATA ){
		Reset();
		return -2;
	}
	Attach();
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::Next(void)
//	�^�C�v�FPublic
//	�@�\�@�F�J�����g���R�[�h�̈ړ�
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::MovePrev(void)
{
	if(m_hStmt == NULL) return(-100);

	if( m_selOK==FALSE )return(-1);

	int ret;
	ret = SQLFetchScroll( m_hStmt,SQL_FETCH_PRIOR,0 );
	if( ret == SQL_ERROR || ret == SQL_INVALID_HANDLE || ret == SQL_NO_DATA ){
		Reset();
		return -2;
	}
	
	Attach();
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::Next(void)
//	�^�C�v�FPublic
//	�@�\�@�F�J�����g���R�[�h�̈ړ�
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::MoveFirst(void)
{
	if(m_hStmt == NULL) return(-100);

	if( m_selOK==FALSE )return(-1);

	int ret;
	ret = SQLFetchScroll( m_hStmt,SQL_FETCH_FIRST,0 );
	if( ret == SQL_ERROR || ret == SQL_INVALID_HANDLE || ret == SQL_NO_DATA ){
		Reset();
		return -2;
	}
	
	Attach();
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::Next(void)
//	�^�C�v�FPublic
//	�@�\�@�F�J�����g���R�[�h�̈ړ�
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::MoveLast(void)
{
	if(m_hStmt == NULL) return(-100);

	if( m_selOK==FALSE )return(-1);

	int ret;
	ret = SQLFetchScroll( m_hStmt,SQL_FETCH_LAST,0 );
	if( ret == SQL_ERROR || ret == SQL_INVALID_HANDLE || ret == SQL_NO_DATA ){
		Reset();
		return -2;
	}
	
	Attach();
	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::Sel(char* sql)
//	�^�C�v�Fprotected
//	�@�\�@�Fselect���s
//	�����P�F���s����select��
//	�߂�l�F0=���� 
//			����ȊO�F���s
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::Sel(char* sql)
{
	SQLRETURN ret;

	m_bindErr=0;
	m_selOK=FALSE;
	Reset();

	if( Init() ) return -1;

	//SQL���̏���
	ret = SQLPrepare( m_hStmt, (SQLCHAR *)sql, SQL_NTS );
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,m_hStmt, "SEL01",sql);
		Reset();
		return -1;
	}

	ret = SQLExecute( m_hStmt );
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,m_hStmt, "SEL02");
		Reset();
		//�������s
		return -1;
	}
	
	//SQL���s���ċA���Ă����J�������̎擾
	m_ColmnCount = 0;
	SQLNumResultCols(m_hStmt,&m_ColmnCount);
	if( m_ColmnCount <= 0){
		Reset();
		return -2;		//�J�����Ȃ�(���̃P�[�X�͂Ȃ�����)
	}

	//virtual�֐�
	Bind();
	if( m_bindErr ){
		printf("�o�C���h�G���[�̂���Fetch���~\n");
		Reset();
		return -3;
	}

	//Attach�I�Ȏg����
//	ret = SQLFetchScroll( m_hStmt,SQL_FETCH_NEXT,0 );
	// Sel�̎���SQLFetch���g�� 2009.02.02 matuda
	ret = SQLFetch(m_hStmt);
	if( IsSuccess(ret) ){
		Reset();
		return -1;	//�f�[�^�Ȃ�
	}
	
	m_selOK=TRUE;

	Attach();
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::Exec(char* sql)
//	�^�C�v�Fprotected
//	�@�\�@�Fsql�̎��s(insert,select delete)
//	�����P�F���s����sql��
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::Exec(char* sql)
{
	SQLRETURN ret;
	SQLHANDLE stmt;
	
	//�X�e�[�g�����g�n���h���̎擾
	ret = SQLAllocHandle( SQL_HANDLE_STMT, m_hConn , &stmt);
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,stmt, "Exec 00");
		return -1;
	}

	ret = SQLExecDirect( stmt,(SQLCHAR*)sql,strlen(sql) );
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,stmt, "EXEC02");
		//���s���s
		SQLFreeHandle( SQL_HANDLE_STMT, stmt );
		return ret;
	}

	// 2009.03.24 ���� ExecDirect�ɕύX
/*
	//SQL���̏���
	ret = SQLPrepare( stmt, (SQLCHAR *)sql, SQL_NTS );
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,stmt, "EXEC01");
		SQLFreeHandle( SQL_HANDLE_STMT, stmt );
		return -1;
	}
	//���s
	ret = SQLExecute( stmt );
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,stmt, "EXEC02");
		//���s���s
		SQLFreeHandle( SQL_HANDLE_STMT, stmt );
		return ret;
	}
*/
	SQLFreeHandle( SQL_HANDLE_STMT, stmt );
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcBase::IsSuccess( SQLRETURN code )
//	�^�C�v�Fpublic
//	�@�\�@�FSQL�֐��������������ǂ����̃`�F�b�N
//	�����P�FSQLRETURN�^
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::IsSuccess( SQLRETURN code )
{
	if( (code == SQL_SUCCESS ) || ( code == SQL_SUCCESS_WITH_INFO ) ){
		return 0;
	}
	return -1;
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL TOdbcBase::SQLSetLastError(SQLSMALLINT hType,SQLHANDLE hSQL )
//	�^�C�v�Fpublic
//	�@�\�@�F�G���[������̎擾
//	�����P�FSQL�n���h���^�C�v
//	�����Q�FSQL�n���h��
//	�����R�F���b�Z�[�W
//---------------------------------------------------------------------------------------------------------------------
void TOdbcTable::SQLSetLastError(SQLSMALLINT hType,SQLHANDLE hSQL ,char* errpoint,char *msg/*=NULL*/)
{ 
	char szState[5+1];		// SQLSTATE�擾�p
	int errno;
	char errMsg[1024];
	SQLSMALLINT	nRetLength = 0;		// �G���[���b�Z�[�W��

	memset(szState,0,sizeof(szState));
	memset(errMsg,0,sizeof(errMsg));
	SQLGetDiagRec( hType, hSQL, 1, (SQLCHAR*)szState, (SQLINTEGER*)&errno, (SQLCHAR*)errMsg, 1024, &nRetLength );

	printf("point = %s ,SQL Err=%d\nmsg=%s",errpoint,errno,errMsg);
	if( m_LogMode == TRUE ){
		FILE *fp;
		char now[14+1],buf[50],fname[100];
		get_nowtime(now,14,0);now[14]=0;
		memcpy(buf,now,8);buf[8]=0;
		sprintf(fname, "%sodbc.err",buf);
		fp = fopen( fname,"ab");
		if( fp != NULL){
			if( msg==NULL ) fprintf(fp,"%s:point=%s,SQL Err=%d Err Msg=%s\r\n",now,errpoint,errno,errMsg);
			else fprintf(fp,"%s:point=%s,SQL Err=%d Err Msg=%s msg=%s\r\n",now,errpoint,errno,errMsg,msg);
			fclose(fp);
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::BindField( char *fldname, void *data,int ,dsize,int dtype )
//	�^�C�v�Fpublic
//	�@�\�@�FODBC�̃t�B�[���h�ƃf�[�^�x�[�X�̃t�B�[���h�̃o�C���h
//	�����P�F��������t�B�[���h��
//	�����Q�F�f�[�^�̃|�C���^
//	�����R�F�t�B�[���h�T�C�Y
//	�����S�F�f�[�^�̃^�C�v SQL_C_SLONG,SQL_C_CHAR�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::BindField( char *fldname, void *data,int fldsize,int datatype )
{
	if( m_ColmnCount <= 0 )return -1;
	if( !strlen(fldname) ) return -2;

	if( m_hStmt==NULL ){
		m_bindErr = -1;
		return -1;
	}

	short namelen,decdigit;
	SQLSMALLINT dtype;
	unsigned long colsize;
	unsigned char colname[100];
	char txt[100],buf[100];
	SDWORD	dwTypeLen=0;

	//�J�������̎擾
	for(short i=1; i<=m_ColmnCount ;i++){
		SQLDescribeCol(m_hStmt,i,colname,sizeof(colname)-1,&namelen,&dtype,&colsize,&decdigit,NULL);
		sprintf(txt,"%s",colname);
		strupr(txt);
		sprintf(buf,"%s",fldname);
		strupr(buf);
		if( !memcmp(txt,buf,strlen(buf)) ){
			SQLBindCol(m_hStmt,i,(SQLSMALLINT)datatype ,(SQLPOINTER)data, fldsize ,&dwTypeLen);
			return 0; //Bind����
		}
	}

	m_bindErr++;
	printf("�w�肳�ꂽ�t�B�[���h %s ��Fetch�Ώۂɑ��݂��Ȃ�\n",fldname);
	return -2;		//�w�肳�ꂽ�t�B�[���h��������Ȃ�
}
//---------------------------------------------------------------------------------------------------------------------
//	void TOdbcTable::Attach(void)
//	�^�C�v�Fpublic
//	�@�\�@�FDOBC�f�[�^�ƃZ�b�g����f�[�^�t�B�[���h��Attach
//	Virtual�̃_�~�[�֐�
//---------------------------------------------------------------------------------------------------------------------
void TOdbcTable::Attach(void)
{
}
//---------------------------------------------------------------------------------------------------------------------
//	void TOdbcTable::Bind(void)
//	�^�C�v�Fpublic
//	�@�\�@�FDOBC�f�[�^�ƃZ�b�g����f�[�^�t�B�[���h���o�C���h������
//	Virtual�̃_�~�[�֐�
//---------------------------------------------------------------------------------------------------------------------
void TOdbcTable::Bind(void)
{
}

