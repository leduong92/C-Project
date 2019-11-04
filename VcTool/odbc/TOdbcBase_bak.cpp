//=====================================================================================================================
//
//	FileName	:TOdbcBase.CPP
//	Class		:TOdbcBase
//				: ODBC 3.X ��DB��{����N���X
//	�쐬��		:2008.12.23	Coded by matsuda
//
//=====================================================================================================================
//	�C��
//=====================================================================================================================
#include "todbcbase.h"
#include "stdio.h"
//---------------------------------------------------------------------------------------------------------------------
//	TOdbcBase::TOdbcBase()
//	�^�C�v�Fpublic
//	�@�\�@�F�f�t�H���g�R���X�g���N�^�B�����o�ϐ��̏�����
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
TOdbcBase::TOdbcBase()
{
	m_hEnv = NULL;
	m_hConn = NULL;
}

//---------------------------------------------------------------------------------------------------------------------
//	TOdbcBase::~TOdbcBase()
//	�^�C�v�Fpublic
//	�@�\�@�F�f�X�g���N�^�B�I�u�W�F�N�g�⃁�����̉��
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
TOdbcBase::~TOdbcBase()
{
	int ret;
	
	CloseConnect();
	
	if(	m_hConn != NULL ){
		ret = SQLFreeHandle( SQL_HANDLE_DBC,m_hConn);
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_DBC, m_hConn ,"CLOSE CON 01");
		}
	}
	m_hConn=NULL;

	if( m_hEnv != NULL ){
		ret = SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"CLOSE CON 02");
		}
	}
	
	m_hEnv = NULL;
	
}
//---------------------------------------------------------------------------------------------------------------------
//	void TOdbcBase::CloseConnect(void)
//	�^�C�v�Fpublic
//	�@�\�@�F�ڑ��ؒf����
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
void TOdbcBase::CloseConnect(void)
{
	int ret;
	// �e�n���h���̉��
	if(	m_hConn != NULL ){
		ret = SQLDisconnect(m_hConn);
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_DBC, m_hConn ,"CLOSE CON 00");
		}
	}
	//2009.03.25 ���� ���̊֐��ł͐ڑ���؂邾���ɂ���
/*	
	if(	m_hConn != NULL ){
		ret = SQLDisconnect(m_hConn);
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_DBC, m_hConn ,"CLOSE CON 00");
		}
		ret = SQLFreeHandle( SQL_HANDLE_DBC,m_hConn);
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_DBC, m_hConn ,"CLOSE CON 01");
		}
	}
	
	m_hConn=NULL;
	if( m_hEnv != NULL ){
		ret = SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"CLOSE CON 02");
		}
	}
	m_hEnv = NULL;
*/
}
//---------------------------------------------------------------------------------------------------------------------
//	void TOdbcBase::CloseConnect(void)
//	�^�C�v�Fpublic
//	�@�\�@�F�ڑ��n���h���ł̐ڑ��L���m�F�@�\
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TOdbcBase::CheckConnect(void)
{
	if( m_hConn == NULL ){
		printf("ODBC�ڑ�:�ؒf\n");
		return -1;	//���ڑ�
	}
	//����ȊO�͐ڑ�����Ă���
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL TOdbcBase::Connect(void)
//	�^�C�v�Fpublic
//	�@�\�@�F�f�[�^�x�[�X�ɐڑ�
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TOdbcBase::OpenConnect(char* dsn,char* user,char* pass)
{
	if( NULL==dsn || NULL==user || NULL == pass ) return(-1);
//	if( m_hConn != NULL ) return 0;	//�ڑ����͂n�j
	SQLRETURN ret;
	printf("ODBC�ڑ���...\n");
	
	//2009.03.25 ���� �����̃n���h����NULL��������V�K�B����ȊO�͍Đڑ������ɂ���
	if( m_hEnv==NULL && m_hConn==NULL ){	//�V�K�ڑ�(���̏����ȊO�͍Đڑ�)
		// �@���n���h���̃A���P�[�g
		ret = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv );
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"BASE01");
			m_hEnv = NULL;
			return -2;	//���s
		}

		// �A�������̐ݒ�(ODBC 3.0)
		ret =  SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0 );
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"BASE02");
			// ���n���h���̉��
			SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
			m_hEnv = NULL;
			return -3;	//���s
		}

		// �B�R�l�N�g�n���h���̃A���P�[�g
		ret = SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hConn );
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_DBC, m_hConn ,"BASE03");
			// ���n���h���̉��
			SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
			m_hEnv = NULL;
			m_hConn=NULL;
			return -4;	//���s
		}

		// �C�R�l�N�g�̃^�C���A�E�g�ݒ�
		SQLPOINTER timeOut;
		int i = 10;	//�^�C���A�E�g10�b
		timeOut = &i;
		ret = SQLSetConnectAttr(m_hConn, SQL_LOGIN_TIMEOUT, (SQLPOINTER)(timeOut), 0);
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"BASE04");
			// �e�n���h���̉��
			SQLFreeHandle( SQL_HANDLE_DBC, m_hConn );
			SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
			m_hConn=NULL;
			m_hEnv = NULL;
			return -5;	//���s
		}

		// �D�I�[�g�R�~�b�g���[�h�̐ݒ�
		ret = SQLSetConnectAttr(m_hConn, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"BASE05");
			// �e�n���h���̉��
			SQLFreeHandle( SQL_HANDLE_DBC, m_hConn );
			SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
			m_hConn=NULL;
			m_hEnv = NULL;
			return -6;	//���s
		}
	}
	
	// �E�R�l�N�g���s
	ret = SQLConnect(m_hConn, (SQLCHAR *)dsn,SQL_NTS,(SQLCHAR *)user,SQL_NTS,(SQLCHAR *)pass,SQL_NTS );
	if( IsSuccess( ret ) ){
		SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"BASE06");
		// �e�n���h���̉��
		SQLFreeHandle( SQL_HANDLE_DBC, m_hConn );
		SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
		m_hConn=NULL;
		m_hEnv = NULL;
		return -7;	//���s
	}
	printf("ODBC�ڑ�����\n");
	
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcBase::IsSuccess( SQLRETURN code )
//	�^�C�v�Fpublic
//	�@�\�@�FSQL�֐��������������ǂ����̃`�F�b�N
//	�����P�FSQLRETURN�^
//---------------------------------------------------------------------------------------------------------------------
int TOdbcBase::IsSuccess( SQLRETURN code )
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
//---------------------------------------------------------------------------------------------------------------------
void TOdbcBase::SQLSetLastError(SQLSMALLINT hType,SQLHANDLE hSQL ,char* errpoint)
{ 
	char szState[5+1];		// SQLSTATE�擾�p
	int errno;
	char errMsg[1024];
	SQLSMALLINT	nRetLength = 0;		// �G���[���b�Z�[�W��

	memset(szState,0,sizeof(szState));
	SQLGetDiagRec( hType, hSQL, 1, (SQLCHAR*)szState, (SQLINTEGER*)&errno, (SQLCHAR*)errMsg, 1024, &nRetLength );

	printf("point = %s ,SQL Err=%d\nmsg=%s",errpoint,errno,errMsg);
}

