#ifndef BTIACS_EXT_H
#define BTIACS_EXT_H
//========================================================================================
//	Btrieve.cppのファイル操作機能クラス(拡張版:ファイルミラーリング機能付き)
//	ファイル操作クラスを作成するときはこれを継承して下さい！
//	FileName	:BtiRepli.H
//	Class		:TBtiReplication
//	作成日		:2006.08.21	Coded By coba
//========================================================================================
//	Modefied
//========================================================================================
#include "btiacs.h"

class TBtiReplication :public TBtiAccess
{
	private:
		int itsFileOpen;			// ファイルオープンステータス
		int itsFileOpenBk;			// ファイルオープンステータス

		TBtiAccess m_bkFile;		// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ操作用のｵﾌﾞｼﾞｪｸﾄ

	protected:
		int SetFileName( char *FileName, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName);
		int SetDataLen(short Len);
		void SetDataPointer(void *DataPtr);

		BTI_BYTE *fileCreatePtr;
		BTI_WORD	fileCreateLen;

	public:
		int SetKey(short Keyno);				// キー番号をセット

		TBtiReplication();
		~TBtiReplication();

		int Create(void);		// ファイルの作成

		int Close(void);		// Btrieve Close
		int Open(void);			// Btrive Open

		int Add(void);			// レコードの追加

		int Del(void *key);		// レコードの削除

		int Upd(void *key);		// レコードの更新

 };

#endif
