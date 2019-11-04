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
//	2007.12.11 coba IsConnect()関数が機能していなかったので、再接続処理を見直した
//	2007.12.13 coba 再接続確認を毎回実行していたのを、エラー時のみ実行するように変更
//	2009.03.31 ﾏﾂﾀﾞ ChkConnect()関数でPS_Select後にカーソルを解放していないバグ修正
//	2011.02.01 matsuda フィールド名からフィールド番号を取得できないケースが発生したためchar* Columns(int colNo)を追加
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

		int Exec(char* sql);

	public:
		TPgTable();
		~TPgTable();
		int SetDB(PGconn** pg);

		int Sel(char* sql);
		char* Columns(char* fldname);
		char* Columns(int colNo);		//2011.02.01 matuda

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