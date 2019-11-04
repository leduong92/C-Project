#ifndef GRIDTDATA_H
#define GRIDTDATA_H

//========================================================================================
//
//	FileName	:GrdData3.h
//	Class		:TGridData
// 				:グリッドウインドウ・データクラスversion 3
//	修正		:2000.11.14 coba	メモリ使用量の削減の為の大改造でバージョンアップ
//									列数無制限
//	修正		:2002.10.24 coba	行の挿入・削除機能を追加
//	修正		:2003.05.23 coba	行の削除時に、最終行が削除されないバグ取り
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>

#define STRLEN_MAX 100
#define TITLELEN_MAX 40
#define ROW_DEFAULT 256

class TGridData
{
	public:
		TGridData(long rowblock,long totalcol,BOOL title=FALSE);
		~TGridData();

		// タイトル描画
		BOOL SetTitle(int col,char* titleString);
		BOOL GetTitle(int col,char* buf,int bufsize);

		// データ代入 
		BOOL GetCel(char* buf, unsigned int bufsize,int col,int row );
		char *GetCelEx(int col,int row);

		long GetActiveRow(void);
		// 行追加
		long AddRow(long uTag=0);
		long AddRow(char* colTitle, long uTag=0);


		// タグ情報取得
		long GetTag( long row){return(m_StrData[row].Tag);};
		long GetCelTag(long col,long row){return( m_StrData[row].celTag[col]);}
		BOOL SetCelTag(int col,int row, long data);
		BOOL SearchCelTag(long* col,long* row,long searchVal,long satrtcol,long startrow,long direction);

		// 現在の行の合計を返す
		long GetTotalRows(void){return(m_LastRowNo+1);}
		long GetTotalCols(void){return(m_TotalCols);}

		long m_AllocBlock;				// アロケートする単位

		// 指定した行が有効かを判定
		BOOL IsCelEnable( int col, int row);

	protected:
		// 行ヘッダのあるなし
		BOOL m_RowTitleEnable;

		// Viewより参照
		BOOL SetCelData(int col,int row, char color,long cTag,char* data,char bkcolor);

		long m_LastRowNo;			// 保持している行の合計-1
		long m_TotalCols;

		// 見出し格納
//		char m_Title[COL_LIMIT+1][TITLELEN_MAX+1];		// +1 はコピー用バッファ
		char **m_Title;

		// データ格納領域(列単位)
		typedef struct{
			char** d;
			long *celTag;
			long Tag;								// ユーザ用
			char colTitle[TITLELEN_MAX+1];			// 行題（名前と一致してない！！ごめんなさい）
			char *color;				// moji色	0=黒、1=赤 2= 青
			char *bkcolor;				// 背景色色	0=白、1=無効色
		}LISTSTRDATA;


		LISTSTRDATA *m_StrData;
		long m_AllocRows;				// アロケート済み行数


		// 新規列の挿入
		long AddCol(long targetCol);

		// 列コピー,ペースト
		BOOL ColCopy( long targetCol ,BOOL cut);
		long ColPaste( long targetCol,BOOL insert);

		BOOL m_CopyBufFull;

		BOOL DataClear(void);

		// セル背景色の変更
		BOOL SetCelBkColor(int col,int row, char color);

		// 行見出しの変更
		BOOL ChangeRowTitle(long rowNo, char* newtitle);

		// 行の挿入・削除
		long RowDelete(long row);
		long InsertNewRow(long row,long tag=0,char* title=NULL);

	private:
		// GetCelEx()用のバッファ
		char *m_retBuf;

};
#endif

