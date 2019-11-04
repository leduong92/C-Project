#ifndef PS_LIB_H
#define PS_LIB_H
//===================================================================================================================
//
//	FileName	: pslib.c
//				Postgres用のユーティリティ
//	作成日		:2001.10.22 
//	更新日		:2002.04.18 coba ちょっとしたてなおし
//===================================================================================================================
// 2010.05.19 coba クライアントのエンコーディングを設定する関数を追加
// 2011.02.01 matsuda フィールド名からフィールド番号を取得できないケースが発生したためフィールド番号直接指定の関数を追加(PS_ColbyNo)
//===================================================================================================================
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libpq-fe.h" /* libpq ヘッダファイル */

// DB接続
int PS_Connect( PGconn** pg, char* constr);
void PS_DisConnect( PGconn* pg);

int PS_IsConnect(PGconn* pg);
int PS_ReConnect(PGconn** pg);

// selectの発行および結果の開放
int PS_Select( PGconn* pg,PGresult** res, char* sql);
void PS_Close( PGresult* res);

// 検索結果情報の取得
int PS_GetColCounts(PGresult* res);
int PS_GetColName( PGresult* res, int colno,char* fieldname,int bufsize);

// 検索結果のフィールドデータの参照
int PS_GetCol( PGresult* res, int rowno,int colno,char* buf,int bufsize);
int PS_GetByField( PGresult* res, int rowno,char* fieldname,char* buf,int bufsize);
char *PS_Col( PGresult* res, int rowno,char* fieldname);
char *PS_ColEx( PGresult* res, int rowno,int colno,char* fieldname,int bufsize);
char *PS_ColbyNo( PGresult* res, int rowno,int fieldno);		//2011.02.01 matsuda 

// SQLの発行
int PS_Exec( PGconn* pg, char* sql);

// 2010.05.19 coba エンコーディングの設定
int PS_Encoding(PGconn* pg,char* encoding);

#ifdef __cplusplus
}
#endif

#endif
