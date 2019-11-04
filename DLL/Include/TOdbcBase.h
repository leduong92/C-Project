#ifndef DB_ODBC_BASE_H
#define DB_ODBC_BASE_H

//========================================================================================
//
//	FileName	:TOdbcBase.h
//	Class		:TOdbcBase
//				: ODBC 3.X のDB基本操作クラス
//	作成日		:2008.12.23	Coded by matsuda
//========================================================================================
// 2015.01.14 coba 日本語のprintf()を英語化
//========================================================================================
#include <windows.h>		// sqltypes.h 内でHWNDのtypedefがあるため
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

class TOdbcBase
{
	public:
		TOdbcBase();
		~TOdbcBase();

		// 接続
		int OpenConnect(char* dsn,char* user,char* pass);
		void CloseConnect(void);
		int CheckConnect(void);

		// トランザクション
//		int Commit(int mode);

		SQLHANDLE* GetDb(void){return(&m_hConn);};

	private:
		SQLHANDLE m_hEnv;	//環境ハンドル
		SQLHANDLE m_hConn;	//コネクションハンドル
		BOOL m_Connect;		//接続フラグ

 	protected:
		int IsSuccess( SQLRETURN code );
		void SQLSetLastError(SQLSMALLINT hType,SQLHANDLE hSQL ,char* errpoint);
};

#endif
