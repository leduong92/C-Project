#ifndef DB_ODBC_BASE_H
#define DB_ODBC_BASE_H

//========================================================================================
//
//	FileName	:TOdbcBase.h
//	Class		:TOdbcBase
//				: ODBC 3.X ��DB��{����N���X
//	�쐬��		:2008.12.23	Coded by matsuda
//========================================================================================
// 2015.01.14 coba ���{���printf()���p�ꉻ
//========================================================================================
#include <windows.h>		// sqltypes.h ����HWND��typedef�����邽��
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

class TOdbcBase
{
	public:
		TOdbcBase();
		~TOdbcBase();

		// �ڑ�
		int OpenConnect(char* dsn,char* user,char* pass);
		void CloseConnect(void);
		int CheckConnect(void);

		// �g�����U�N�V����
//		int Commit(int mode);

		SQLHANDLE* GetDb(void){return(&m_hConn);};

	private:
		SQLHANDLE m_hEnv;	//���n���h��
		SQLHANDLE m_hConn;	//�R�l�N�V�����n���h��
		BOOL m_Connect;		//�ڑ��t���O

 	protected:
		int IsSuccess( SQLRETURN code );
		void SQLSetLastError(SQLSMALLINT hType,SQLHANDLE hSQL ,char* errpoint);
};

#endif
