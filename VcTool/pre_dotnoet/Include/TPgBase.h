#ifndef DB_POSTGRES_BASE_H
#define DB_POSTGRES_BASE_H

//========================================================================================
//
//	FileName	:TPgBase.h
//	Class		:TPgBase
//				: postgres��DB��{����N���X
//	�쐬��		:2002.05.07	Coded by Coba
//========================================================================================
//	�C��
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

		// �ڑ�
		int OpenConnect(char* host,char* db,char* user,char* pass);

		// �g�����U�N�V����
		int Commit(int mode);

	private:
		PGconn* m_pgConn;
		int ChkConnect(void);

 	protected:
		PGconn** GetDb(void){return(&m_pgConn);};
};

#endif
