#ifndef DB_POSTGRES_BASE_H
#define DB_POSTGRES_BASE_H

//========================================================================================
//	FileName	:TPgBase.h
//	Class		:TPgBase
//========================================================================================

#include "pslib.h"


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


		int OpenConnect(char* host,char* db,char* user,char* pass);
		
		int Commit(int mode);

		PGconn** GetDb(void){return(&m_pgConn);};
	private:
		PGconn* m_pgConn;
		int ChkConnect(void);

 	protected:
};

#endif
