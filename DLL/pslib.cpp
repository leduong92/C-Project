//===================================================================================================================
//
//	FileName	: pslib.c
//				Postgres用のユーティリティ
//	作成日		:2001.10.22 coba
//	更新日		: 
//===================================================================================================================

//#include "stdafx.h"
#include "pslib.h"

// Postgres用関数変数
//--------------------------------------------------------------------------------------------------------------------
//	int PS_Connect( PGconn** pg)
// 説明　:	PostgreSQLに接続
// 引数１:　データベース接続変数
// 戻り値: 0=成功
//--------------------------------------------------------------------------------------------------------------------
int PS_Connect( PGconn** pg,char* constr)
{
	if( constr==NULL || strlen(constr) < 1 ) return(-2);

	*pg = PQconnectdb(constr); /* データベースへの接続 */

	/* データベースへの接続失敗? */
	if (PQstatus(*pg) == CONNECTION_BAD) return(-1);

	return(0);
}
//--------------------------------------------------------------------------------------------------------------------
//	void PS_DisConnect( PGconn* pg,PGresult* res)
// 説明　:DB接続の開放
// 引数１:　データベース接続変数
// 引数２:　結果セット受取変数
//--------------------------------------------------------------------------------------------------------------------
void PS_DisConnect( PGconn* pg)
{
	PQfinish(pg);
}

//--------------------------------------------------------------------------------------------------------------------
//	int IsConnect(PGconn* pg)
// 説明　:DB接続が有効かどうか調べる
// 引数１:　データベース接続変数
// 戻り値:0=接続OK
//--------------------------------------------------------------------------------------------------------------------
int PS_IsConnect(PGconn* pg)
{
	if (PQstatus(pg) == CONNECTION_BAD) return(-1);
	return(0);
}

//--------------------------------------------------------------------------------------------------------------------
//	int ReConnect(PGconn** pg)
// 説明　:DBに再接続する
// 引数１:　データベース接続変数
//--------------------------------------------------------------------------------------------------------------------
int PS_ReConnect(PGconn** pg)
{
	PQreset(*pg); /* データベースへの接続 */

	/* データベースへの接続失敗? */
	if (PQstatus(*pg) == CONNECTION_BAD) return(-1);
	return(0);
}

//--------------------------------------------------------------------------------------------------------------------
//	int PS_Select( PGconn* pg,PGresult* res, char* sql)
// 説明　:selectを実行
// 引数１:　データベース接続変数
// 引数２:　結果セット受取変数
// 引数３:　発行するSELECT文
// 戻り値: 0未満＝失敗 それ以外＝レコード数
//--------------------------------------------------------------------------------------------------------------------
int PS_Select( PGconn* pg,PGresult** res, char* sql)
{
	*res = PQexec(pg, sql);/* 問い合わせの送信 */
	if (PQresultStatus(*res) != PGRES_TUPLES_OK) {
		return(-101);
	}

	return(PQntuples(*res));        /* 戻ってきた行数 */
}

//--------------------------------------------------------------------------------------------------------------------
// void PS_Close( PGconn* pg,PGresult* res)
// 説明　:結果セットのクローズ
// 引数１:　結果セット受取変数
//--------------------------------------------------------------------------------------------------------------------
void PS_Close( PGresult* res)
{
	PQclear(res);
}

//--------------------------------------------------------------------------------------------------------------------
//	int PS_GetColCounts(PGresult* res)
// 説明　:検索結果のフィールド数を返す
// 引数１:　結果セット
// 戻り値:フィールド数
//--------------------------------------------------------------------------------------------------------------------
int PS_GetColCounts(PGresult* res)
{
	return(PQnfields(res));
}

//--------------------------------------------------------------------------------------------------------------------
//	int PS_GetColName( PGresult* res, char* fieldname,int bufsize)
// 説明　:検索結果のフィールド番号を指定して、そのフィールド名をバッファに取得する
// 引数１:　結果セット
// 引数２:　フィールド（列）番号
// 引数４:	取得バッファのポインタ
// 引数５:	バッファのサイズ
// 戻り値:成功=0,それ以外=エラー
//--------------------------------------------------------------------------------------------------------------------
int PS_GetColName( PGresult* res, int colno,char* fieldname,int bufsize)
{
	int len;

	// 列は有効か?
	if( colno >= PQnfields(res) ) return(-1);

	len=strlen(PQfname( res, colno));
	if( len <1 ) return(-2);

	// 列名コピー
	memcpy( fieldname, PQfname( res, colno), len>bufsize ? bufsize : len);
	return(0);
}

//--------------------------------------------------------------------------------------------------------------------
//	int PS_GetCol( PGresult* res, int rowno,int colno,char* buf,int bufsize)
// 説明　:行番号と列番号を指定して、フィールドデータをバッファに取得する
// 引数１:　結果セット
// 引数２:　取得したい行の番号
// 引数３:　取得したい列の番号
// 引数４:	取得バッファのポインタ
// 引数５:	バッファのサイズ
//--------------------------------------------------------------------------------------------------------------------
int PS_GetCol( PGresult* res, int rowno,int colno,char* buf,int bufsize)
{
	int len;
	// 列は有効か?
	if( colno >= PQnfields(res) ) return(-1);

	// 行は有効か?
	if( rowno >= PQntuples(res) ) return(-2);

	// データの長さを取得
	len = PQgetlength(res,rowno,colno);
	if( len < 1 ) return(-4);

	memcpy( buf,PQgetvalue(res, rowno,colno),len>bufsize ? bufsize : len);
	return(0);
}

//--------------------------------------------------------------------------------------------------------------------
//	int PS_GetCol( PGresult* res, int rowno,int colno,char* buf,int bufsize)
// 説明　:行番号とフィールド名を指定して、フィールドデータをバッファに取得する(select * 検索向け)
// 引数１:　結果セット
// 引数２:　取得したい行の番号
// 引数３:　取得したい列の名前
// 引数４:	取得バッファのポインタ
// 引数５:	バッファのサイズ
//--------------------------------------------------------------------------------------------------------------------
int PS_GetByField( PGresult* res, int rowno,char* fieldname,char* buf,int bufsize)
{
	int f_index;
	int len;

	// フィールドのインデックスを探す
	if( (f_index=PQfnumber(res,fieldname))==-1) return(-1);

	// 行は有効か?
	if( rowno >= PQntuples(res) ) return(-2);        /* 戻ってきた行数 */

	// NULL?
	if( PQgetisnull(res,rowno,f_index)==1 )return(-3);

	// データの長さを取得
	len = PQgetlength(res,rowno,f_index);
	if( len < 1 ) return(-4);

	memcpy( buf,PQgetvalue(res, rowno,f_index),len>bufsize ? bufsize : len);
	return(0);
}

//--------------------------------------------------------------------------------------------------------------------
// char *PS_Col( PGresult* res, int rowno,char* fieldname)
// 説明　:行とフィールド名を指定して、フィールドデータの取得(select * 検索向け)
// 引数１:　結果セット
// 引数２:　取得したい行の番号
// 引数３:　取得したいフィールド名
//--------------------------------------------------------------------------------------------------------------------
char *PS_Col( PGresult* res, int rowno,char* fieldname)
{
	int f_index;

	// フィールドのインデックスを探す
	if( (f_index=PQfnumber(res,fieldname))==-1) return("");

	// 行は有効か?
	if( rowno >= PQntuples(res) ) return("");        /* 戻ってきた行数 */

	// NULL?
	if( PQgetisnull(res,rowno,f_index)==1 )return("");

	return( PQgetvalue(res, rowno,f_index));
}

//--------------------------------------------------------------------------------------------------------------------
// char *PS_ColEx( PGresult* res, int rowno,int colno,char* fieldname,int bufsize)
// 説明　:行と列を指定してフィールドデータ及びフィールド名の取得
// 引数１:　結果セット
// 引数２:　取得したい行の番号
// 引数３:　取得したい列の番号
// 引数４:　フィールド名(出力)
// 引数５:　上記バッファサイズ
// 戻り値： 取得した文字列
//--------------------------------------------------------------------------------------------------------------------
char *PS_ColEx( PGresult* res, int rowno,int colno,char* fieldname,int bufsize)
{
	int len;

	// 列は有効か?
	if( colno >= PQnfields(res) ) return("");        /* 戻ってきた行数 */

	// 行は有効か?
	if( rowno >= PQntuples(res) ) return("");        /* 戻ってきた行数 */

	// 列名コピー
	len=strlen(PQfname( res, colno));

	memcpy( fieldname, PQfname( res, colno), len>bufsize ? bufsize : len);


	// NULL?
	if( PQgetisnull(res,rowno,colno)==1 )return("");

	return( PQgetvalue(res, rowno,colno));
}
//--------------------------------------------------------------------------------------------------------------------
//	int PS_Exec( PGconn* pg, char* sql)
// 説明　:update or insertを実行
// 引数１:　データベース接続変数
// 引数３:　発行するinsert or update文
// 戻り値: 0=成功  それ以外＝失敗
//--------------------------------------------------------------------------------------------------------------------
int PS_Exec( PGconn* pg, char* sql)
{
	PGresult* res;

	res = PQexec(pg, sql);/* 問い合わせの送信 */
	if( res == NULL )return(1);
	if (PQresultStatus(res) != PGRES_COMMAND_OK){
		PQclear(res);
		return(2);
	}


	// 1件も影響を受けなかった時はエラーとする
	if ( atoi(PQcmdTuples(res)) <=0 ){
		PQclear(res);
		return(3);
	}
	PQclear(res);
	return(0);
}
