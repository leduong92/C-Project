#ifndef TTEXTDB2_H
#define TTEXTDB2_H
//=======================================================================================================
//	テキストDBAccessクラスのヘッダ
//	Class Name TTextTable2
//	File Name TTextTable2.h
//	作成者：Coba
//	作成日：1999.05.10
//=======================================================================================================
//	修正
//	2003.03.04 coba GetLast()関数を追加
//	2003.05.21 coba path名長さを、MAX_PATHに変更
//					key退避バッファが足りなくてオーバーフローするバグを修正
//	2006.10.11	Maeda	XOR 暗号化処理追加(暫定版)
//=======================================================================================================

#include "windows.h"
#include "winuser.h"
#include "winbase.h"

#include "EasyEnc.h"

// エラーコード
#define TXDB_NOFILE	72
#define DEF_BUFSIZE 4096
#define TXDB_MAXKEYLEN	128
#define TXDB_WORKING 144

//----------------------------
// 簡易暗号化コード
#define EASY_ENC_CODE 0xA5

class TTextTable2
{
	public:
		TTextTable2(void);
		~TTextTable2(void);
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

		int SetEncryption( int EncryptionCode );	// 2006.10.11 Maeda

	protected:

		typedef struct{
			char filename[30+1];
			short reclen;
			short keylen;					// キー長（最大50byteにして下さい）
			short offset;					// キーの先頭からのﾊﾞｲﾄオフセット
			char *datptr;					// レコード格納ポインタ
			char dir[MAX_PATH+1];					// ファイルのパス
			char tmpdir[MAX_PATH+1];				// 作業領域のパス
		} FSPEC;

		int Initialize(FSPEC *filespec,int bufsize);	//2006.10.11 Maeda
//		int InitializeEnc(FSPEC *filespec,int bufsize);
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

		// 2006.10.11 管理暗号化処理  Maeda
		long	m_EncryptionEnableFlg;	// PC のレジストリチェック結果
										// 0=レジストリ登録無し
										// 1=レジストリ登録あり（暗号化で、Add可能)
		long	m_Encryption;	// 暗号化フラグ
								// 0==Normal
								// 0!=XorCode
		BOOL ReadFileEnc(  HANDLE hFile,  LPVOID lpBuffer,  DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead,   LPOVERLAPPED lpOverlapped );
		BOOL WriteFileEnc( HANDLE hFile,  LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,LPOVERLAPPED lpOverlapped );

};

#endif
