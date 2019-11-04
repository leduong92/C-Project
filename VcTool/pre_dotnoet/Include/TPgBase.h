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
//========================================================================================
#include <pslib.h>

enum commit_arg_id{
	PGCMT_START,
	PGCMT_COMMIT,
	PGCMT_ROLLBACK,
};

class TPgBase
{
	public:
		TPgBase();
		~TPgBase();

		// 接続
		int OpenConnect(char* host,char* db,char* user,char* pass);

		// トランザクション
		int Commit(int mode);

	private:
		PGconn* m_pgConn;
		int ChkConnect(void);

 	protected:
		PGconn** GetDb(void){return(&m_pgConn);};
};

#endif
