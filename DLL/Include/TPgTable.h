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
//	2007.12.11 coba IsConnect()�֐����@�\���Ă��Ȃ������̂ŁA�Đڑ���������������
//	2007.12.13 coba �Đڑ��m�F�𖈉���s���Ă����̂��A�G���[���̂ݎ��s����悤�ɕύX
//	2009.03.31 ���� ChkConnect()�֐���PS_Select��ɃJ�[�\����������Ă��Ȃ��o�O�C��
//	2011.02.01 matsuda �t�B�[���h������t�B�[���h�ԍ����擾�ł��Ȃ��P�[�X��������������char* Columns(int colNo)��ǉ�
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

		int Exec(char* sql);

	public:
		TPgTable();
		~TPgTable();
		int SetDB(PGconn** pg);

		int Sel(char* sql);
		char* Columns(char* fldname);
		char* Columns(int colNo);		//2011.02.01 matuda

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