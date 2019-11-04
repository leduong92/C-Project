#ifndef TBINARYDB_NOK_H
#define TBINARYDB_NOK_H
//=======================================================================================================
//	可変長バイナリファイル操作クラスのヘッダ
//	Class Name TBinaryTable
//	File Name TBinaryTable.h
//	作成者：coba
//	作成日：2002.02.01
//	修正日：
//	ファイルの最大サイズ 4,294,967,295バイト
//	レコードの最大長	65,535-16(ポジションブロック)=65,519 バイト
//	レコードの先頭にポジション情報（レコード位置やレコード長）を持ってる
//=======================================================================================================

#include "windows.h"
#include "winuser.h"
#include "winbase.h"

#include <stdio.h>
#include <time.h>
#include <share.h>
#include <io.h>

#pragma pack(push,1)

// エラーコード(Btrieveのパクリ)
#define BTB_TOF	0
#define BTB_OK			0
#define BTB_OPE_ERR		1
#define BTB_IO_ERR		2
#define BTB_NOTFIND		4
#define BTB_DUP			5
#define BTB_KEY_ERR		6
#define BTB_POS_ERR		8
#define BTB_EOF			9
#define BTB_NAME_FAULT	11
#define BTB_NOFILE	 	12
#define BTB_NOINIT	 	20
#define BTB_KEYLEN_ERR	 	21
#define BTB_DBUFLEN_ERR	 	22
#define BTB_CANT_CREATE	 	25
#define BTB_RECLEN_ERR	 	28
// 互換性の無いファイル
#define BTB_DIFF_FILE	 	30
#define BTB_LOCKED	 	85


// windowsをやめたときの対策
#ifndef MAX_PATH
#define MAX_PATH 255
#endif

#define NO_POSITON 0
#define MAX_RECORDLENGTH 65519
#define MAX_KEY_LENGTH 100

// 定義名
static const char* BF_NOK_NAME="BF_NOK";

// バージョン
static const char* BF_NOK_VERSION="--.--";

// ファイル構造定義
typedef struct{
	unsigned short keylen;					// キー長(最大100)
	unsigned short offset;					// キーの先頭からのﾊﾞｲﾄオフセット
} BF_SPEC;

// ファイルヘッダ(この構造に一致しないファイルはエラー)
typedef struct{
	char code[6];			//"BF_NOK"
	char date[8];			// YYYYMMDD		// 作成日
	char ver[5];			// **.**
	unsigned short keylen;					// キー長(最大100)
	unsigned short offset;					// キーの先頭からのﾊﾞｲﾄオフセット
	unsigned long first_pos;			// 最初のレコードのファイル先頭からの位置
	unsigned long last_pos;				// 最終レコードのファイル先頭からの位置
	char crlf[2];
}BF_HEAD;


class TBinaryTable
{
	public:
		TBinaryTable(void);
		~TBinaryTable(void);

	protected:
		int Open(char* filename);

		int Del( void *key );
		int Add( void* data, unsigned short reclen);
		int Upd( void* data, unsigned short reclen);


		int First( unsigned short* reclen);
		int Last( unsigned short* reclen);
		int Next( unsigned short* reclen);
		int Prev( unsigned short* reclen);
		int Create(char* fname,BF_SPEC *filespec);

		int Find( void *key ,unsigned short* reclen);
		int GE( void *key ,unsigned short* reclen);
		int GT( void *key ,unsigned short* reclen);
		int LE( void *key ,unsigned short* reclen);
		int LT( void *key ,unsigned short* reclen);

		int ReadRecord(void* buf, unsigned short bufsize);


//		int GetFileInfo(BF_HEAD* buf,char* filename);

	private:
		char m_fName[MAX_PATH+1];		// 操作中のファイル名

		// ポジションブロック
		struct BF_POSBLOCK{
			unsigned short del_sign;		// 有効フラグ
			unsigned long rec_pos;			// ファイル先頭からの位置（ポジションブロックを含む）
			unsigned short rec_len;			// （ポジションブロックを除いた）レコード長
			unsigned long next_pos;			// 次のレコードのファイル先頭からの位置
			unsigned long prev_pos;			// 前のレコードのファイル先頭からの位置
		} m_Pblock;

		int Move(unsigned short* reclen,int direction);

		int SearchFile(char* fname);
		int SearchKey( void *key ,int operation ,unsigned short* reclen);

};
#pragma pack(pop)

#endif
