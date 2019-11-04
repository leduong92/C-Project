#ifndef BTIACS_H
#define BTIACS_H
//========================================================================================
//
//	Copyright 1996-1996 NOK Cororation All Rights Reserved
//	FileName	:BTIACS.H
//	Class		:TBtiAccess
//	作成日		:1996.2.16	Coded By Koba
//	Btrieve.cppのファイル操作機能拡張クラス
//	ファイル操作クラスを作成するときはこれを継承して下さい！
//
//========================================================================================
// Modefied
// 1996.10.10 メンバ関数Last(),GT(),LT()を追加 By Koba
// 2006.08.21 coba レプリケーション用派生ｸﾗｽ作成のため、いくつかのメンバをpublicにした
// 2006.08.22 coba レプリケーション用派生ｸﾗｽ作成のため、関数を追加
//========================================================================================
#include "Btrieve.h"
//#include <noktool.h>

class TBtiAccess :public Btrieve
{
	private:
		int itsFileOpen;			// ファイルオープンステータス
		char *itsDataPtr;			// データ代入ポインタ
		char *itsKeyPtr;			// キーデータポインタ


	protected:
		void SetKeyPointer(void *KeyPtr);

	public:
		TBtiAccess();
		~TBtiAccess();

		void SetDataPointer(void *DataPtr);	// 2006.08.21 publicメンバに移動
		void SetCreateSpec(BTI_BYTE* fcreate,BTI_WORD fcreatelen);	// 2006.08.22

		int Close(void);		// Btrieve Close
		int Open(void);			// Btrive Open

		int Add(void);			// レコードの追加

		int Del(void *key);		// レコードの削除
		int Del(void );			// レコードの削除その２

		int Upd(void *key);		// レコードの更新
		int Upd(void );			// レコードの更新その２

		int First(void);		// １番目のレコードを読み込む

		int Last(void);			// 最終レコードを読み込む

		int Find(void *key);	// 指定したキーのレコードを読み込む
		int Find(void);			// 指定したキーのレコードを読み込むその２

		int Next(void);			// 次のレコードを読み込む
		int Prev(void);			// 前のレコードを読み込む

		int GE(void *key);		// 指定したキー以上のレコードを読み込む
		int GE(void);			// 指定したキー以上のレコードを読み込むその２

		int LE(void *key);		// f_le()
		int LE(void);			// f_le()その２

		int GT(void *key);		// 指定したキーより大きいレコードを読み込む
		int GT(void);			// 指定したキーより大きいレコードを読み込むその２

		int LT(void *key);		// 指定したキーより小さいレコードを読み込む
		int LT(void);			// 指定したキーより小さいレコードを読み込むその２
		int IsOpen(void){return itsFileOpen;};	//ファイルのオープン状態のチェック関数 2013.10.28 matsuda

 };

#endif
