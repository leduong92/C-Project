#ifndef DB_POSTGRES_BASE_H
#define DB_POSTGRES_BASE_H

//========================================================================================
//
//	FileName	:TPgBase.h
//	Class		:TPgBase
//				: postgresのDB基本操作クラス
//	作成日		:2002.05.07	Coded by Coba
//========================================================================================
//	修正
//	2006.10.10 複数のDBを扱えるように、SetDBをpublicメンバに移す
//	2010.05.19 coba クライアントエンコーディング設定関数SetEncoding()追加
//	2010.09.03 ﾏﾂﾀﾞ OpenConnet()の引数にportを追加
//	2010.12.17 coba Close()を追加
//	2014.07.02 coba サーバ時刻取得関数GetSrvDateTime()を追加
//========================================================================================
#include <pslib.h>

enum commit_arg_id{
	PGCMT_START,
	PGCMT_COMMIT,
	PGCMT_ROLLBACK,
};

// 2014.07.03 サーバ時刻の取得
typedef struct
{
	short y;	// 年
	short m;	// 月
	short d;	// 日
	short h;	// 時
	short mi;	// 分
	short s;	// 秒
	short dow;	// 曜日 0=日曜
	char dt[14+1];	// YYYYMMDD
}SRV_DATETIME;

class TPgBase
{
	public:
		TPgBase();
		~TPgBase();

		// 接続
		// 2010.09.03 matuda 引数にportを追加
		int OpenConnect(char* host,char* db,char* user,char* pass,int port=5432);

		// トランザクション
		int Commit(int mode);

		PGconn** GetDb(void){return(&m_pgConn);};

		// 2010.05.19 coba エンコード設定
		int SetEncoding(char* encoding);

		// 2010.12.17 coba 切断関数
		void CloseDB(void);

		// 2014.07.03 サーバ時刻の取得
		int GetSrvDateTime(SRV_DATETIME* dat);

	private:
		PGconn* m_pgConn;
		int ChkConnect(void);

 	protected:
};

#endif
