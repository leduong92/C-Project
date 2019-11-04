#ifndef TTEXTDB_H
#define TTEXTDB_H
//=======================================================================================================
//	テキストDBAccessクラスのヘッダ
//	Class Name TTextTable
//	File Name TTextTable.h
//	作成者：Coba
//	作成日：1999.05.10
//=======================================================================================================
//	修正
//	2003.03.04 coba GetLast()関数を追加
//	2003.05.21 coba path名長さを、MAX_PATHに変更
//					key退避バッファが足りなくてオーバーフローするバグを修正
//	2014.04.10 coba ファイル消去関数Drop()を追加
//  2015.02.26 sanno	ファイル名の長さを拡張
//=======================================================================================================

#include "windows.h"
#include "winuser.h"
#include "winbase.h"

// エラーコード
#define TXDB_NOFILE	72
#define DEF_BUFSIZE 4096
#define TXDB_MAXKEYLEN	128
#define TXDB_WORKING 144

class TTextTable
{
	public:
		TTextTable(void);
		~TTextTable(void);
		int GetEqual( void *key );
		int GetEqual2( void *key );
		int FindFirst( void *key );
		int Upd( void *key);
		int Upd2( void *key);
		int Del( void *key );
		int Del2( void *key );		//2004.11.09 ﾏﾂﾀﾞ
		int Insert( void *key );
		int Insert2( void *key );	//2004.11.09 ﾏﾂﾀﾞ
		int GetFirst(void);
		int GetLast( void  );
		int Next(void);
		int Prev(void);
		int Add( void );

		int GetCounts(void);

		int Drop(void);		// 2014.4.10 coba

	protected:

		typedef struct{
//			char filename[30+1];
			//char filename[50+1];	//2009.01.22 ﾏﾂﾀﾞ サイズ変更
			char filename[80+1];	//2015.02.26 sanno サイズ変更
			short reclen;
			short keylen;					// キー長（最大50byteにして下さい）
			short offset;					// キーの先頭からのﾊﾞｲﾄオフセット
			char *datptr;					// レコード格納ポインタ
			char dir[MAX_PATH+1];					// ファイルのパス
			char tmpdir[MAX_PATH+1];				// 作業領域のパス
		} FSPEC;

		int Initialize(FSPEC *filespec,int bufsize);
		FSPEC m_fSpec;					// ファイルスペック

	private:

		long m_PrevRecNo;				// 直前に操作したレコード番号
		void Clear(void);

		int m_BufSize;					// ファイル操作時のバッファ長

		int Open( HANDLE& hFile ,BOOL readonly);
		void Close(HANDLE& hFile);
		
		int CheckFile(void);			// 2004.11.09 ﾏﾂﾀﾞ

		int MoveRecord( HANDLE hfile ,void *key , long *recNo );		// キーに該当するﾚｺｰﾄﾞに移動する
		int MoveRecord2( HANDLE hfile ,void *key , long *recNo );		// キーに該当するﾚｺｰﾄﾞに移動する(旧)


};

#endif
