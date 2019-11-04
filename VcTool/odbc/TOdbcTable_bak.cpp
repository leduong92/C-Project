//=====================================================================================================================
//
//	FileName	:TOdbcTable.cpp
//	Class		:TOdbcTable
//				odbcテーブル操作基底クラス
//	作成日		:2008.12.23	Coded by matsuda
//========================================================================================
//	修正
//
//========================================================================================
#include "todbctable.h"
#include "stdio.h"
#include "getnow.h"
//---------------------------------------------------------------------------------------------------------------------
//	TOdbcTable::TOdbcTable()
//	タイプ：Public
//	機能　：デフォルトコンストラクタ。メンバ変数の初期化
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
//	タイプ：Public
//	機能　：デストラクタ。オブジェクトやメモリの解放
//	引数１：なし
//---------------------------------------------------------------------------------------------------------------------
TOdbcTable::~TOdbcTable()
{
	if( m_hStmt != NULL ) SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt );
}
//---------------------------------------------------------------------------------------------------------------------
//	void TOdbcTable::Reset(void)
//	タイプ：Public
//	機能　：ステートメントハンドルのDelete
//	引数１：なし
//	戻り値：なし
//---------------------------------------------------------------------------------------------------------------------
void TOdbcTable::Reset(void)
{
	if( m_hStmt != NULL )SQLFreeHandle( SQL_HANDLE_STMT, m_hStmt );
	
	m_hStmt=NULL;
	
	m_selOK=FALSE;
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::Init(void)
//	タイプ：Public
//	機能　：ステートメントハンドルの取得
//	引数１：なし
//	戻り値：0：成功 それ以外：失敗
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::Init(void)
{
	SQLRETURN ret;
	//ステートメントハンドルの取得
	ret = SQLAllocHandle( SQL_HANDLE_STMT, m_hConn , &m_hStmt );
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,m_hStmt, "Init00");
		m_hStmt = NULL;
		return -1;
	}

	//※カーソルはScrollableに設定しないとNextしか使えないようです。
	//※設定すると遅くなるので注意して下さい
	if( m_scrollEnable == TRUE )
		SQLSetStmtAttr(m_hStmt,SQL_ATTR_CURSOR_SCROLLABLE,(SQLPOINTER)SQL_SCROLLABLE ,0);
	else
		SQLSetStmtAttr(m_hStmt,SQL_ATTR_CURSOR_SCROLLABLE,(SQLPOINTER)SQL_NONSCROLLABLE ,0);

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	TOdbcTable::SetDB( SQLHANDLE *hconn )
//	タイプ：Public
//	機能　：コネクションハンドルを貰う＆ステートメントハンドルを作成する
//	引数１：コネクションハンドル(DB接続済みであること)
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::SetDB( SQLHANDLE *hconn )
{
	m_hConn = *hconn;
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::SetScrollEnabe(void)
//	タイプ：Public
//	機能　：MovePrevなどを使うためにカーソルをScrollableにする
//	引数１：TRUE;設定する FALSE：設定解除する
//	※カーソルはScrollableに設定しないとNextしか使えないようです。設定すると遅くなるので注意して下さい
//	
//	注意！！　SelectやInsertなどのステートメント作成前に設定して下さい
//---------------------------------------------------------------------------------------------------------------------
void TOdbcTable::SetScrollEnable(BOOL on)
{
	m_scrollEnable = on;
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::Next(void)
//	タイプ：Public
//	機能　：カレントレコードの移動
//	引数１：なし
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
//	タイプ：Public
//	機能　：カレントレコードの移動
//	引数１：なし
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
//	タイプ：Public
//	機能　：カレントレコードの移動
//	引数１：なし
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
//	タイプ：Public
//	機能　：カレントレコードの移動
//	引数１：なし
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
//	タイプ：protected
//	機能　：select実行
//	引数１：実行するselect文
//	戻り値：0=成功 
//			それ以外：失敗
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::Sel(char* sql)
{
	SQLRETURN ret;

	m_bindErr=0;
	m_selOK=FALSE;
	Reset();

	if( Init() ) return -1;

	//SQL文の準備
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
		//検索失敗
		return -1;
	}
	
	//SQL実行して帰ってきたカラム数の取得
	m_ColmnCount = 0;
	SQLNumResultCols(m_hStmt,&m_ColmnCount);
	if( m_ColmnCount <= 0){
		Reset();
		return -2;		//カラムなし(このケースはないかも)
	}

	//virtual関数
	Bind();
	if( m_bindErr ){
		printf("バインドエラーのためFetch中止\n");
		Reset();
		return -3;
	}

	//Attach的な使い方
//	ret = SQLFetchScroll( m_hStmt,SQL_FETCH_NEXT,0 );
	// Selの時はSQLFetchを使う 2009.02.02 matuda
	ret = SQLFetch(m_hStmt);
	if( IsSuccess(ret) ){
		Reset();
		return -1;	//データなし
	}
	
	m_selOK=TRUE;

	Attach();
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcTable::Exec(char* sql)
//	タイプ：protected
//	機能　：sqlの実行(insert,select delete)
//	引数１：実行するsql文
//---------------------------------------------------------------------------------------------------------------------
int TOdbcTable::Exec(char* sql)
{
	SQLRETURN ret;
	SQLHANDLE stmt;
	
	//ステートメントハンドルの取得
	ret = SQLAllocHandle( SQL_HANDLE_STMT, m_hConn , &stmt);
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,stmt, "Exec 00");
		return -1;
	}

	ret = SQLExecDirect( stmt,(SQLCHAR*)sql,strlen(sql) );
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,stmt, "EXEC02");
		//実行失敗
		SQLFreeHandle( SQL_HANDLE_STMT, stmt );
		return ret;
	}

	// 2009.03.24 ﾏﾂﾀﾞ ExecDirectに変更
/*
	//SQL文の準備
	ret = SQLPrepare( stmt, (SQLCHAR *)sql, SQL_NTS );
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,stmt, "EXEC01");
		SQLFreeHandle( SQL_HANDLE_STMT, stmt );
		return -1;
	}
	//実行
	ret = SQLExecute( stmt );
	if( IsSuccess(ret) ){
		SQLSetLastError(SQL_HANDLE_STMT,stmt, "EXEC02");
		//実行失敗
		SQLFreeHandle( SQL_HANDLE_STMT, stmt );
		return ret;
	}
*/
	SQLFreeHandle( SQL_HANDLE_STMT, stmt );
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	int TOdbcBase::IsSuccess( SQLRETURN code )
//	タイプ：public
//	機能　：SQL関数が成功したかどうかのチェック
//	引数１：SQLRETURN型
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
//	タイプ：public
//	機能　：エラー文字列の取得
//	引数１：SQLハンドルタイプ
//	引数２：SQLハンドル
//	引数３：メッセージ
//---------------------------------------------------------------------------------------------------------------------
void TOdbcTable::SQLSetLastError(SQLSMALLINT hType,SQLHANDLE hSQL ,char* errpoint,char *msg/*=NULL*/)
{ 
	char szState[5+1];		// SQLSTATE取得用
	int errno;
	char errMsg[1024];
	SQLSMALLINT	nRetLength = 0;		// エラーメッセージ長

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
//	タイプ：public
//	機能　：ODBCのフィールドとデータベースのフィールドのバインド
//	引数１：検索するフィールド名
//	引数２：データのポインタ
//	引数３：フィールドサイズ
//	引数４：データのタイプ SQL_C_SLONG,SQL_C_CHARなど
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

	//カラム名の取得
	for(short i=1; i<=m_ColmnCount ;i++){
		SQLDescribeCol(m_hStmt,i,colname,sizeof(colname)-1,&namelen,&dtype,&colsize,&decdigit,NULL);
		sprintf(txt,"%s",colname);
		strupr(txt);
		sprintf(buf,"%s",fldname);
		strupr(buf);
		if( !memcmp(txt,buf,strlen(buf)) ){
			SQLBindCol(m_hStmt,i,(SQLSMALLINT)datatype ,(SQLPOINTER)data, fldsize ,&dwTypeLen);
			return 0; //Bind成功
		}
	}

	m_bindErr++;
	printf("指定されたフィールド %s はFetch対象に存在しない\n",fldname);
	return -2;		//指定されたフィールドが見つからない
}
//---------------------------------------------------------------------------------------------------------------------
//	void TOdbcTable::Attach(void)
//	タイプ：public
//	機能　：DOBCデータとセットするデータフィールドをAttach
//	Virtualのダミー関数
//---------------------------------------------------------------------------------------------------------------------
void TOdbcTable::Attach(void)
{
}
//---------------------------------------------------------------------------------------------------------------------
//	void TOdbcTable::Bind(void)
//	タイプ：public
//	機能　：DOBCデータとセットするデータフィールドをバインドさせる
//	Virtualのダミー関数
//---------------------------------------------------------------------------------------------------------------------
void TOdbcTable::Bind(void)
{
}

