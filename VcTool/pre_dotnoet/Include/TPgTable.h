#ifndef POSTGRES_TABLE_CLASS_H
#define POSTGRES_TABLE_CLASS_H
//========================================================================================
//
//	FileName	:tpgtable.H
//	Class		:TPgTable
//				postgres�e�[�u��������N���X
//	�쐬��		:2002.05.07	Coded by coba
//========================================================================================
//	�C��
//
//========================================================================================
#include <pslib.h>
#include <windows.h>

class TPgTable
{
	protected:

		// DB�I�u�W�F�N�g
		PGconn** m_pgDB;
		PGresult* m_pgRes;

		// �J�[�\������
		void Reset(void);
		int m_rowNo;
		int m_rowMax;
		BOOL m_selOK;

		int ChkConnect(void);

		int Sel(char* sql);
		int Exec(char* sql);
		char* Columns(char* fldname);

	public:
		TPgTable();
		~TPgTable();
		int SetDB(PGconn** pg);

		// select�̃q�b�g����
		int GetCounts(void){return(m_rowMax);};

		int MoveNext(void);
		int MovePrev(void);
		int MoveFirst(void);
		int MoveLast(void);

		// �f�[�^�W�J
		virtual void Attach(void);

	private:
};

#endif
