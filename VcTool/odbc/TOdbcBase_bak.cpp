//=====================================================================================================================
//
//	FileName	:TOdbcBase.CPP
//	Class		:TOdbcBase
//				: ODBC 3.X のDB基本操作クラス
//	作成日		:2008.12.23	Coded by matsuda
//
//=====================================================================================================================
//	修正
//=====================================================================================================================
#include "todbcbase.h"
#include "stdio.h"
//---------------------------------------------------------------------------------------------------------------------
//	TOdbcBase::TOdbcBase()
//	タイプ：public
//	機能　：デフォルトコンストラクタ。メンバ変数の初期化
//	引数１：なし
//---------------------------------------------------------------------------------------------------------------------
TOdbcBase::TOdbcBase()
{
	m_hEnv = NULL;
	m_hConn = NULL;
}

//---------------------------------------------------------------------------------------------------------------------
//	TOdbcBase::~TOdbcBase()
//	タイプ：public
//	機能　：デストラクタ。オブジェクトやメモリの解放
//	引数１：なし
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
//	タイプ：public
//	機能　：接続切断処理
//	引数１：なし
//---------------------------------------------------------------------------------------------------------------------
void TOdbcBase::CloseConnect(void)
{
	int ret;
	// 各ハンドルの解放
	if(	m_hConn != NULL ){
		ret = SQLDisconnect(m_hConn);
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_DBC, m_hConn ,"CLOSE CON 00");
		}
	}
	//2009.03.25 ﾏﾂﾀﾞ この関数では接続を切るだけにする
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
//	タイプ：public
//	機能　：接続ハンドルでの接続有無確認機能
//	引数１：なし
//---------------------------------------------------------------------------------------------------------------------
int TOdbcBase::CheckConnect(void)
{
	if( m_hConn == NULL ){
		printf("ODBC接続:切断\n");
		return -1;	//未接続
	}
	//それ以外は接続されている
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL TOdbcBase::Connect(void)
//	タイプ：public
//	機能　：データベースに接続
//	引数１：なし
//---------------------------------------------------------------------------------------------------------------------
int TOdbcBase::OpenConnect(char* dsn,char* user,char* pass)
{
	if( NULL==dsn || NULL==user || NULL == pass ) return(-1);
//	if( m_hConn != NULL ) return 0;	//接続中はＯＫ
	SQLRETURN ret;
	printf("ODBC接続中...\n");
	
	//2009.03.25 ﾏﾂﾀﾞ 両方のハンドルがNULLだったら新規。それ以外は再接続扱いにする
	if( m_hEnv==NULL && m_hConn==NULL ){	//新規接続(この条件以外は再接続)
		// ①環境ハンドルのアロケート
		ret = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv );
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"BASE01");
			m_hEnv = NULL;
			return -2;	//失敗
		}

		// ②環境属性の設定(ODBC 3.0)
		ret =  SQLSetEnvAttr( m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0 );
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"BASE02");
			// 環境ハンドルの解放
			SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
			m_hEnv = NULL;
			return -3;	//失敗
		}

		// ③コネクトハンドルのアロケート
		ret = SQLAllocHandle( SQL_HANDLE_DBC, m_hEnv, &m_hConn );
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_DBC, m_hConn ,"BASE03");
			// 環境ハンドルの解放
			SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
			m_hEnv = NULL;
			m_hConn=NULL;
			return -4;	//失敗
		}

		// ④コネクトのタイムアウト設定
		SQLPOINTER timeOut;
		int i = 10;	//タイムアウト10秒
		timeOut = &i;
		ret = SQLSetConnectAttr(m_hConn, SQL_LOGIN_TIMEOUT, (SQLPOINTER)(timeOut), 0);
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"BASE04");
			// 各ハンドルの解放
			SQLFreeHandle( SQL_HANDLE_DBC, m_hConn );
			SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
			m_hConn=NULL;
			m_hEnv = NULL;
			return -5;	//失敗
		}

		// ⑤オートコミットモードの設定
		ret = SQLSetConnectAttr(m_hConn, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);
		if( IsSuccess( ret ) ){
			SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"BASE05");
			// 各ハンドルの解放
			SQLFreeHandle( SQL_HANDLE_DBC, m_hConn );
			SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
			m_hConn=NULL;
			m_hEnv = NULL;
			return -6;	//失敗
		}
	}
	
	// ⑥コネクト実行
	ret = SQLConnect(m_hConn, (SQLCHAR *)dsn,SQL_NTS,(SQLCHAR *)user,SQL_NTS,(SQLCHAR *)pass,SQL_NTS );
	if( IsSuccess( ret ) ){
		SQLSetLastError( SQL_HANDLE_ENV, m_hEnv ,"BASE06");
		// 各ハンドルの解放
		SQLFreeHandle( SQL_HANDLE_DBC, m_hConn );
		SQLFreeHandle( SQL_HANDLE_ENV, m_hEnv );
		m_hConn=NULL;
		m_hEnv = NULL;
		return -7;	//失敗
	}
	printf("ODBC接続完了\n");
	
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcBase::IsSuccess( SQLRETURN code )
//	タイプ：public
//	機能　：SQL関数が成功したかどうかのチェック
//	引数１：SQLRETURN型
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
//	タイプ：public
//	機能　：エラー文字列の取得
//	引数１：SQLハンドルタイプ
//	引数２：SQLハンドル
//---------------------------------------------------------------------------------------------------------------------
void TOdbcBase::SQLSetLastError(SQLSMALLINT hType,SQLHANDLE hSQL ,char* errpoint)
{ 
	char szState[5+1];		// SQLSTATE取得用
	int errno;
	char errMsg[1024];
	SQLSMALLINT	nRetLength = 0;		// エラーメッセージ長

	memset(szState,0,sizeof(szState));
	SQLGetDiagRec( hType, hSQL, 1, (SQLCHAR*)szState, (SQLINTEGER*)&errno, (SQLCHAR*)errMsg, 1024, &nRetLength );

	printf("point = %s ,SQL Err=%d\nmsg=%s",errpoint,errno,errMsg);
}

