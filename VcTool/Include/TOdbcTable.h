#ifndef ODBC_TABLE_CLASS_H
#define ODBC_TABLE_CLASS_H
//========================================================================================
//
//	FileName	:todbctable.H
//	Class		:TOdbcTable
//				ODBCテーブル操作基底クラス
//	作成日		:2008.12.23	Coded by matuda
//========================================================================================
//	2009.11.05 matuda Sel時のFetch(),FetchScroll()どちらを使うかをiSeries のドライバのバージョンで切り替えるよう変更
//	2010.02.04 matuda Exec()の引数に影響行数を返すためのポインタを追加
//	2012.02.15 matsuda MoveNext前にBindされているフィールドをクリアするためのvirtual関数ClearBindedDataを追加
//========================================================================================
#include <windows.h>		// sqltypes.h 内でHWNDのtypedefがあるため
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

class TOdbcTable
{
	private:
		BOOL m_LogMode;//ログファイル記録モード


	protected:
		int Exec(char* sql);
		int Exec(char* sql,int *affect_rows);
		// データフィールドのバインド
		virtual void Bind(void);
		//Attach
		virtual void Attach(void);
		// 2012.02.15 matsuda 追加
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

		//ログファイル記録モード
		void SetLogMode( BOOL on ){	m_LogMode = on;};
		
		//ステートメントのリセット
		void Reset(void);
		void EnableFethcScroll(BOOL on);	//Enable or disable fetch scroll function. 2013.03.25 matsuda 
		
	private:
		SQLHANDLE m_hConn;	//コネクションハンドル
		SQLHANDLE m_hStmt;
		short m_ColmnCount;
		BOOL m_selOK;
		int m_bindErr;
		BOOL m_scrollEnable;
		BOOL m_fetchScroll;		//FetchScrollを使用する場合はTRUE 2009.11.05 matuda

		void SQLSetLastError(SQLSMALLINT hType,SQLHANDLE hSQL ,char* errpoint,char *msg=NULL);
		int IsSuccess( SQLRETURN code );

		int Init(void);

		
};

#endif
