#ifndef GRIDTDATA_H
#define GRIDTDATA_H

//========================================================================================
//
//	FileName	:GridData.h
//	Class		:TGridData
// 				:グリッドウインドウ・データクラス
//	作成日		:1999.07.30	Coded by coba
//	修正		:1999.09.06 coba	行数を無制限に（今までは最大５０）
//									行ヘッダ機能追加
//									横スクロール機能追加
//	修正		:1999.09.09 coba	方向キーでのセル間移動機能追加
//									列単位のコピー・張り付け機能追加
//	修正		:1999.09.10 coba	行一括選択モードを追加
//									行数の制限を廃止
//	修正		:1999.09.10 coba	１行単位の行アロケートをブロック単位へ（遅いから）
//	修正		:2000.06.07 coba	行題の変更機能を追加
//
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>

#define STRLEN_MAX 100
#define TITLELEN_MAX 40
#define COL_LIMIT 301
//#define COL_LIMIT 256
#define ROW_DEFAULT 256

class TGridData
{
	public:
		TGridData(long rowblock,long totalcol,BOOL title=FALSE);
		~TGridData();

		// タイトル描画
		BOOL SetTitle(int col,char* titleString);

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

		long m_AllocBlock;				// アロケートする単位

	protected:
		// 行ヘッダのあるなし
		BOOL m_RowTitleEnable;

		// Viewより参照
		BOOL SetCelData(int col,int row, char color,long cTag,char* data,char bkcolor);

		long m_LastRowNo;			// 保持している行の合計-1
		long m_TotalCols;

		// 見出し格納
		char m_Title[COL_LIMIT+1][TITLELEN_MAX+1];		// +1 はコピー用バッファ

		// データ格納領域(列単位)
		typedef struct{
			char d[COL_LIMIT+1][STRLEN_MAX+1];
			long celTag[COL_LIMIT+1];
			long Tag;								// ユーザ用
			char colTitle[TITLELEN_MAX+1];
			char color[COL_LIMIT+1];				// moji色	0=黒、1=赤 2= 青
			char bkcolor[COL_LIMIT+1];				// 背景色色	0=白、1=無効色
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

	private:
		// GetCelEx()用のバッファ
		char m_retBuf[STRLEN_MAX+1];

};
#endif

