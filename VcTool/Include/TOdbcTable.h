#ifndef ODBC_TABLE_CLASS_H
#define ODBC_TABLE_CLASS_H
//========================================================================================
//
//	FileName	:todbctable.H
//	Class		:TOdbcTable
//				ODBC�e�[�u��������N���X
//	�쐬��		:2008.12.23	Coded by matuda
//========================================================================================
//	2009.11.05 matuda Sel����Fetch(),FetchScroll()�ǂ�����g������iSeries �̃h���C�o�̃o�[�W�����Ő؂�ւ���悤�ύX
//	2010.02.04 matuda Exec()�̈����ɉe���s����Ԃ����߂̃|�C���^��ǉ�
//	2012.02.15 matsuda MoveNext�O��Bind����Ă���t�B�[���h���N���A���邽�߂�virtual�֐�ClearBindedData��ǉ�
//========================================================================================
#include <windows.h>		// sqltypes.h ����HWND��typedef�����邽��
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

class TOdbcTable
{
	private:
		BOOL m_LogMode;//���O�t�@�C���L�^���[�h


	protected:
		int Exec(char* sql);
		int Exec(char* sql,int *affect_rows);
		// �f�[�^�t�B�[���h�̃o�C���h
		virtual void Bind(void);
		//Attach
		virtual void Attach(void);
		// 2012.02.15 matsuda �ǉ�
		virtual void ClearBindedData(void);

	public:
		TOdbcTable();
		~TOdbcTable();
		int SetDB( SQLHANDLE *hconn );
		void SetScrollEnable(BOOL on);

		int Sel(char* sql);

		int MoveNext(void);
		int MovePrev(void);
		int MoveFirst(void);
		int MoveLast(void);

		int BindField( char *fldname, void *data,int dsize,int dtype );

		//���O�t�@�C���L�^���[�h
		void SetLogMode( BOOL on ){	m_LogMode = on;};
		
		//�X�e�[�g�����g�̃��Z�b�g
		void Reset(void);
		void EnableFethcScroll(BOOL on);	//Enable or disable fetch scroll function. 2013.03.25 matsuda 
		
	private:
		SQLHANDLE m_hConn;	//�R�l�N�V�����n���h��
		SQLHANDLE m_hStmt;
		short m_ColmnCount;
		BOOL m_selOK;
		int m_bindErr;
		BOOL m_scrollEnable;
		BOOL m_fetchScroll;		//FetchScroll���g�p����ꍇ��TRUE 2009.11.05 matuda

		void SQLSetLastError(SQLSMALLINT hType,SQLHANDLE hSQL ,char* errpoint,char *msg=NULL);
		int IsSuccess( SQLRETURN code );

		int Init(void);

		
};

#endif
