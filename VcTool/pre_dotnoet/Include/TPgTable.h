#ifndef POSTGRES_TABLE_CLASS_H
#define POSTGRES_TABLE_CLASS_H
//========================================================================================
//
//	FileName	:tpgtable.H
//	Class		:TPgTable
//				postgresテーブル操作基底クラス
//	作成日		:2002.05.07	Coded by coba
//========================================================================================
//	修正
//
//========================================================================================
#include <pslib.h>
#include <windows.h>

class TPgTable
{
	protected:

		// DBオブジェクト
		PGconn** m_pgDB;
		PGresult* m_pgRes;

		// カーソル操作
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

		// selectのヒット件数
		int GetCounts(void){return(m_rowMax);};

		int MoveNext(void);
		int MovePrev(void);
		int MoveFirst(void);
		int MoveLast(void);

		// データ展開
		virtual void Attach(void);

	private:
};

#endif
