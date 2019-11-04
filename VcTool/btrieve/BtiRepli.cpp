//========================================================================================
//	Btrieve.cppのファイル操作機能クラス(拡張版:ファイルミラーリング機能付き)
//	ファイル操作クラスを作成するときはこれを継承して下さい！
//	FileName	:BtiRepli.cpp
//	Class		:TBtiReplication
//	作成日		:2006.08.21	Coded By coba
//========================================================================================
#include <stdio.h>
#include "btirepli.h"

//========================================================================================
// TBtiReplication::TBtiReplication()
//	コンストラクタ
//	引数：なし
//	返値：なし
//========================================================================================
TBtiReplication::TBtiReplication()
{
	itsFileOpen=1;
	itsFileOpenBk=1;
}

//========================================================================================
// TBtiReplication::TBtiReplication()
//	デストラクタ
//	引数：なし
//	返値：なし
//========================================================================================
TBtiReplication::~TBtiReplication()
{
}

//========================================================================================
//	int TBtiReplication::SetFileName( char *FileName, char *ProfileName
//					, LPCTSTR SectionName ,LPCTSTR KeyName)
// 機能：ファイル名のセット。ﾚﾌﾟﾘｹｰｼｮﾝ用のﾌｧｲﾙ名は、引数と同じiniファイルからパスを取得
//		  但し、セクション名は[BTIBACK]から取得する。
// 引数１：btrieveﾌｧｲﾙ名
// 引数２：パスを取得するiniﾌｧｲﾙ名
// 引数３：パスを取得するiniﾌｧｲﾙ内のセクション名
// 引数４：パスを取得するiniﾌｧｲﾙ内のキー名
//========================================================================================
int TBtiReplication::SetFileName( char *FileName, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	// ファイル本体
	Btrieve::SetFileName( FileName,ProfileName,SectionName,KeyName);

	// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ
	m_bkFile.SetFileName( FileName,ProfileName,"BTIBACK",KeyName);
	return(0);
}
//========================================================================================
// int TBtiReplication::SetDataLen(short Len)
// 機能　：レコード長のセット。本体＆ﾚﾌﾟﾘｹｰｼｮﾝﾌｧｲﾙの両方に設定する
// 引数１：btiﾌｧｲﾙのレコード長
//========================================================================================
int TBtiReplication::SetDataLen(short Len)
{
	// ファイル本体
	TBtiAccess::SetDataLen(Len);
	// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ
	m_bkFile.SetDataLen(Len);
	return(0);
}

//========================================================================================
// int TBtiReplication::SetKey(short Keyno)
// 機能　：操作する時のキー番号をセット。本体＆ﾚﾌﾟﾘｹｰｼｮﾝﾌｧｲﾙの両方に設定する
//========================================================================================
int TBtiReplication::SetKey(short Keyno)
{
	// ファイル本体
	Btrieve::SetKey(Keyno);
	// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ
	m_bkFile.SetKey(Keyno);
	return(0);
}
//========================================================================================
//	void TBtiReplication::SetDataPointer(void *DataPtr);
//	データレコード格納ポインタを決定する。本体＆ﾚﾌﾟﾘｹｰｼｮﾝﾌｧｲﾙの両方に設定。
//	引数：実際にデータ部を格納するポインタ(SubClassのデータ構造体部のポインタ)
//	返値：なし
//========================================================================================
void TBtiReplication::SetDataPointer(void *DataPtr)
{
	// ファイル本体
	TBtiAccess::SetDataPointer(DataPtr);	//itsDataPtr=(char*)DataPtr;
	// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ
	m_bkFile.SetDataPointer(DataPtr);
}

//========================================================================================
// int TBtiReplication::Create(void)
// 機能　：空のbtiﾌｧｲﾙを作成する。ﾏｽﾀの作成に成功したときのみﾚﾌﾟﾘｹｰｼｮﾝﾌｧｲﾙ作成する
//========================================================================================
int TBtiReplication::Create(void)
{
	int ret;

	// ﾌｧｲﾙ本体を作成
	Btrieve::fileCreatePtr = this->fileCreatePtr;
	Btrieve::fileCreateLen = this->fileCreateLen;

	ret=Btrieve::Create();

	// 本体(ﾏｽﾀ)の作成に成功したら、ﾚﾌﾟﾘｹｰｼｮﾝ(ｽﾚｰﾌﾞ)を作成
	if(!ret){
		m_bkFile.SetCreateSpec(this->fileCreatePtr,this->fileCreateLen);
		m_bkFile.Create();
	}

	return(ret);

}

//========================================================================================
//	int TBtiReplication::Open(void)
//	Btrieve ファイルオ－プン。本体＆ﾚﾌﾟﾘｹｰｼｮﾝﾌｧｲﾙの両方を開く
//	Btrieveメンバ関数のオーバーライド
//　引数：なし
//　返値：btrのerrorコード
//========================================================================================
int TBtiReplication::Open(void)
{
	// ファイル本体
	itsFileOpen=TBtiAccess::Open();//	itsFileOpen=Btrieve::Open();

	// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ(本体がOKの場合のみ)
	if(itsFileOpen==0)	itsFileOpenBk=m_bkFile.Open();

	return(itsFileOpen);
}


//========================================================================================
//	int TBtiReplication::Close(void)
//	Btrieve ファイルオ－プン
//	Btrieveメンバ関数のオーバーライド
//　引数：なし
//　返値：btrのerrorコード
//========================================================================================
int TBtiReplication::Close(void)
{
	if(itsFileOpen==0){
		TBtiAccess::Close();//		Btrieve::Close();
		itsFileOpen=1;
	}

	// ﾊﾞｯｸｱｯﾌﾟﾌｧｲﾙ
	if(itsFileOpenBk==0){
		m_bkFile.Close();
		itsFileOpenBk=1;
	}
	return(0);
}

//========================================================================================
// int TBtiReplication::Add()
//	レコ－ドの挿入を行う
//
//　 引数：なし
//　 返値：Btrieve Error Code
//========================================================================================
int TBtiReplication::Add()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = TBtiAccess::Add();//	theFstat = Insert(itsDataPtr);

	// 本体が成功したらﾊﾞｯｸｱｯﾌﾟのほうも更新する
	if( !theFstat && !itsFileOpenBk) m_bkFile.Add();

	return(theFstat);
}

//========================================================================================
// int TBtiReplication::Del(void *key)
//	レコ－ドの削除を行う
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：削除するレコードのキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int TBtiReplication::Del(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = TBtiAccess::Del( (char*)key);	//	theFstat = Delete( (char*)key, itsDataPtr);

	// 本体が成功したらﾊﾞｯｸｱｯﾌﾟのほうも更新する
	if( !theFstat && !itsFileOpenBk) m_bkFile.Del((char*)key);

#ifdef TEST
	printf( "Delete Status = %d \n",theFstat);

#endif

	return(theFstat);
}


//========================================================================================
// int TBtiReplication::Upd(void *key)
//	レコ－ドの更新を行う
//		（＊実行する前にBtrieve::SetKey(short keyno)を実行し、
//			 キー番号を指定をしておくこと。）
//　引数：char* key	変更前キーデータ
//　返値：Btrieve Error Code
//========================================================================================
int TBtiReplication::Upd(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = TBtiAccess::Upd( (char*)key);//	theFstat = Update( (char*)key, itsDataPtr);

	// 本体が成功したらﾊﾞｯｸｱｯﾌﾟのほうも更新する
	if( !theFstat && !itsFileOpenBk) m_bkFile.Upd((char*)key);

#ifdef TEST
	printf( "Update Status = %d \n",theFstat);

#endif

	return(theFstat);
}

