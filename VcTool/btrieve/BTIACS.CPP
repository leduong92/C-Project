//#define TEST
//========================================================================================
//
//	Copyright 1996-1996 NOK Cororation All Rights Reserved
//	FileName	:BTIACS.CPP
//	Class		:TBtiAccess ( SubClass of "Btrieve" )
//				:Btrieve.cppのファイル操作機能拡張機能
//	作成日		:1996.10.10	Coded By Koba
//========================================================================================
#include <stdio.h>
#include "btiacs.h"

//#include "noktoolw.h"
//========================================================================================
// TBtiAccess::TBtiAccess()
//	コンストラクタ
//	引数：なし
//	返値：なし
//========================================================================================
TBtiAccess::TBtiAccess()
{
	itsFileOpen=1;
}

//========================================================================================
// TBtiAccess::TBtiAccess()
//	デストラクタ
//	引数：なし
//	返値：なし
//========================================================================================
TBtiAccess::~TBtiAccess()
{
}

//========================================================================================
//	void TBtiAccess::SetDataPointer(void *DataPtr);
//	データレコード格納ポインタを決定する。
//	引数：実際にデータ部を格納するポインタ(SubClassのデータ構造体部のポインタ)
//	返値：なし
//========================================================================================
void TBtiAccess::SetDataPointer(void *DataPtr)
{
	itsDataPtr=(char*)DataPtr;
}

//========================================================================================
//	void TBtiAccess::SetKeyPointer(void *KeyPtr);
//	データレコード格納ポインタを決定する。
//	引数：実際にデータ部を格納するポインタ(SubClassのデータ構造体部のポインタ)
//	返値：なし
//========================================================================================
void TBtiAccess::SetKeyPointer(void *KeyPtr)
{
	itsKeyPtr=(char*)KeyPtr;
}

//========================================================================================
//	void TBtiAccess::SetCreateSpec(BTI_BYTE* fcreate,BTI_WORD fcreatelen)
//	2006.08.22 追加 by coba
//	ﾌｧｲﾙ作成用のﾊﾟﾗﾒｰﾀをセットする
//	引数１：作成するファイルのspecへのポインタ
//	引数２：作成するファイルのspecのサイズ
//========================================================================================
void TBtiAccess::SetCreateSpec(BTI_BYTE* fcreate,BTI_WORD fcreatelen)
{
	Btrieve::fileCreatePtr=fcreate;
	Btrieve::fileCreateLen=fcreatelen;
}

//========================================================================================
//	int TBtiAccess::Open(void)
//	Btrieve ファイルオ－プン
//	Btrieveメンバ関数のオーバーライド
//　引数：なし
//　返値：btrのerrorコード
//========================================================================================
int TBtiAccess::Open(void)
{
	itsFileOpen=Btrieve::Open();

	return(itsFileOpen);
}


//========================================================================================
//	int TBtiAccess::Close(void)
//	Btrieve ファイルオ－プン
//	Btrieveメンバ関数のオーバーライド
//　引数：なし
//　返値：btrのerrorコード
//========================================================================================
int TBtiAccess::Close(void)
{
	if(itsFileOpen==0){
		Btrieve::Close();
		itsFileOpen=1;
	}
	return(0);
}

//========================================================================================
// int TBtiAccess::Add()
//	レコ－ドの挿入を行う
//
//　 引数：なし
//　 返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::Add()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = Insert(itsDataPtr);

//	WinPrintf( "AddR Status = %d \n",theFstat);
#ifdef TEST
#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::Del(void *key)
//	レコ－ドの削除を行う
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：削除するレコードのキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::Del(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = Delete( (char*)key, itsDataPtr);

#ifdef TEST
	printf( "Delete Status = %d \n",theFstat);

#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::Del(void)
//	レコ－ドの削除を行う
//		（＊実行する前には、SubClassのSetKey?Data(,,,)を実行していないと
//			 正常動作は期待できない。）
//　引数：なし
//	入力：キーデータ(itsKeyPtr)
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::Del(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = Delete( itsKeyPtr, itsDataPtr);

#ifdef TEST
	printf( "Delete Status = %d \n",theFstat);

#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::Upd(void *key)
//	レコ－ドの更新を行う
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：char* key	変更前キーデータ
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::Upd(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = Update( (char*)key, itsDataPtr);

#ifdef TEST
	printf( "Update Status = %d \n",theFstat);

#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::Upd(void)
//	レコ－ドの更新を行う
//		（＊実行する前には、SubClassのSetKey?Data(,,,)を実行していないと
//			 正常動作は期待できない。）
//　引数：なし
//	入力：キーデータ(itsKeyPtr)
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::Upd(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = Update( itsKeyPtr, itsDataPtr);

#ifdef TEST
	printf( "Update Status = %d \n",theFstat);

#endif

	return(theFstat);
}

//========================================================================================
// short TBtiAccess::First()
//	最初のレコ－ドを読み込む
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：なし
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::First()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);
	theFstat= GetFirst(itsDataPtr);
#ifdef TEST
	printf( "Read First Status = %d \n",theFstat);
#endif
	return(theFstat);
}


//========================================================================================
// short TBtiAccess::Last()
//	最後のレコ－ドを読み込む
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：なし
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::Last()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);
	theFstat= GetLast(itsDataPtr);
#ifdef TEST
	printf( "Read Last Status = %d \n",theFstat);
#endif
	return(theFstat);
}


//========================================================================================
// int TBtiAccess::Find(void *key)
//	指定したキーのレコ－ドを読み込む
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：char* key	取得したいレコードのキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::Find(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat= GetEqual(itsDataPtr, (char*)key);
#ifdef TEST
	printf( "Read Find Status = %d \n",theFstat);
#endif
	// 未オープン時は再度開いてトライ
	if( theFstat == 3 ){
		theFstat = Open();
		if( !theFstat ){
			theFstat =Btrieve::Open();
			theFstat= GetEqual(itsDataPtr, (char*)key);
		}
	}
	return(theFstat);
}


//========================================================================================
// int TBtiAccess::Find(void)
//	指定したキーのレコ－ドを読み込む
//		（＊実行する前には、SubClassのSetKey?Data(,,,)を実行していないと
//			 正常動作は期待できない。）
//　引数：なし
//	入力：キーデータ(itsKeyPtr)
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::Find(void)
{
	int theFstat;



	if(itsFileOpen != 0 ) return(-1);

	
	theFstat= GetEqual(itsDataPtr, itsKeyPtr);
#ifdef TEST
	printf( "Read Find Status = %d \n",theFstat);
#endif
	return(theFstat);
}


//========================================================================================
// int TBtiAccess::Next()
//	次のレコ－ドを読み込む
//
//　引数：なし
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::Next()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetNext(itsDataPtr);

#ifdef TEST
	printf( "Read Next Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::Prev()
//	 前のレコ－ドを読み込む
//
//　引数：
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::Prev()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetPrev(itsDataPtr);

#ifdef TEST
	printf( "Read Prev Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::GE(void *key)
//	指定キ－以上のレコ－ドを読み込む
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：条件となるキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::GE(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetGE(itsDataPtr,(char*)key);

#ifdef TEST
	printf( "Read GE Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::GE(void)
//	指定キ－以上のレコ－ドを読み込む
//		（＊実行する前には、SubClassのSetKey?Data(,,,)を実行していないと
//			 正常動作は期待できない。）
//　引数：なし
//	入力：キーデータ(itsKeyPtr)
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::GE(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetGE(itsDataPtr,itsKeyPtr);

#ifdef TEST
	printf( "Read GE Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::LE(void *key)
//	指定キー以下のレコ－ドを読み込む
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：条件となるキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::LE(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetLE(itsDataPtr,(char*)key);

#ifdef TEST
	printf( "Read LE Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::LE(void)
//	指定キー以下のレコ－ドを読み込む
//		（＊実行する前には、SubClassのSetKey?Data(,,,)を実行していないと
//			 正常動作は期待できない。）
//　引数：なし
//	入力：キーデータ(itsKeyPtr)
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::LE(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetLE(itsDataPtr,itsKeyPtr);

#ifdef TEST
	printf( "Read LE Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::GT(void *key)
//	指定キ－より大きいレコ－ドを読み込む
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：条件となるキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::GT(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetGT(itsDataPtr,(char*)key);

#ifdef TEST
	printf( "Read GT Status = %d \n",theFstat);
#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::GT(void)
//	指定キ－より大きいレコ－ドを読み込む
//		（＊実行する前には、SubClassのSetKey?Data(,,,)を実行していないと
//			 正常動作は期待できない。）
//　引数：なし
//	入力：キーデータ(itsKeyPtr)
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::GT(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetGT(itsDataPtr,itsKeyPtr);

#ifdef TEST
	printf( "Read GT Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::LT(void *key)
//	指定キーより小さいレコ－ドを読み込む
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：条件となるキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::LT(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetLT(itsDataPtr,(char*)key);

#ifdef TEST
	printf( "Read LT Status = %d \n",theFstat);
#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::LT(void)
//	指定キーより小さいレコ－ドを読み込む
//		（＊実行する前には、SubClassのSetKey?Data(,,,)を実行していないと
//			 正常動作は期待できない。）
//　引数：なし
//	入力：キーデータ(itsKeyPtr)
//　返値：Btrieve Error Code
//========================================================================================
int TBtiAccess::LT(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetLT(itsDataPtr,itsKeyPtr);

#ifdef TEST
	printf( "Read LT Status = %d \n",theFstat);
#endif

	return(theFstat);
}

