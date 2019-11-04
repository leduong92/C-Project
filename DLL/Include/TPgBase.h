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
//	2006.10.10 ������DB��������悤�ɁASetDB��public�����o�Ɉڂ�
//	2010.05.19 coba �N���C�A���g�G���R�[�f�B���O�ݒ�֐�SetEncoding()�ǉ�
//	2010.09.03 ���� OpenConnet()�̈�����port��ǉ�
//	2010.12.17 coba Close()��ǉ�
//	2014.07.02 coba �T�[�o�����擾�֐�GetSrvDateTime()��ǉ�
//========================================================================================
#include <pslib.h>

enum commit_arg_id{
	PGCMT_START,
	PGCMT_COMMIT,
	PGCMT_ROLLBACK,
};

// 2014.07.03 �T�[�o�����̎擾
typedef struct
{
	short y;	// �N
	short m;	// ��
	short d;	// ��
	short h;	// ��
	short mi;	// ��
	short s;	// �b
	short dow;	// �j�� 0=���j
	char dt[14+1];	// YYYYMMDD
}SRV_DATETIME;

class TPgBase
{
	public:
		TPgBase();
		~TPgBase();

		// �ڑ�
		// 2010.09.03 matuda ������port��ǉ�
		int OpenConnect(char* host,char* db,char* user,char* pass,int port=5432);

		// �g�����U�N�V����
		int Commit(int mode);

		PGconn** GetDb(void){return(&m_pgConn);};

		// 2010.05.19 coba �G���R�[�h�ݒ�
		int SetEncoding(char* encoding);

		// 2010.12.17 coba �ؒf�֐�
		void CloseDB(void);

		// 2014.07.03 �T�[�o�����̎擾
		int GetSrvDateTime(SRV_DATETIME* dat);

	private:
		PGconn* m_pgConn;
		int ChkConnect(void);

 	protected:
};

#endif
