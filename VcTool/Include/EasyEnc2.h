#ifndef EASY_ENC_H
#define EASY_ENC_H
//=======================================================================================================
//	簡易版暗号化処理クラス
//	Class Name EasyEnc.h
//	File Name CEasyEnc.h
//	Date : 2006.10.12 N.Maeda
//=======================================================================================================

#include "windows.h"

// 1:DATE , 2:
#define KEY_INFO_MAC_ADR -1
#define KEY_INFO_DATE 1
#define KEY_INFO_MAC  2

// Memo--------------------
// Link lib =  shlwapi.lib NetApi32.lib 
//
// check enc key
// ret=l_EasyEnc->CheckEncKey();		//CheckRegEncKey(ENC_REG_CODE);
//
// Compile mode =  /MLd
// コンパイルモードを間違うと
// 他のライプラリの使用と競合しています。 / NODEFAULTLIB:libraly を使用してください。
//-------------------------


class CEasyEnc
{
	public:
		CEasyEnc(void);
		~CEasyEnc(void);

		// ファイル操作クラスでレジストリに値が登録されているか？チェックする
		// TText2からCALL
		BOOL	CheckEncKey(void);


		// キー登録
		BOOL	SetRegEncKey( char*InstallKey );

		// キー削除
		BOOL	DelRegEncKey(void);

		// キー情報
		// -1:Mac address
		// 1:ENTRY DATE
		// 2:ENTRY MAC address
		BOOL	GetRegEncKeyInfo(int info_no,char*buf,int buf_size);

		// データの変換
		BOOL EncData( unsigned char*data , int data_len );

		// 暗号化しているか？のチェック
		BOOL CheckEncFile( unsigned char * chkData );

		// 暗号キーの作成
		BOOL	MakeInstallKey( int KeyMode , unsigned char* keyCode , unsigned char* mac );

	protected:
		// プログラムの最初にレジストリの値が正しいか？チェックする
		// レジストリの登録が正しくない場合は、値を削除する
		// （以降のファイルオープンのチェックで引っかかる）
		BOOL	CheckRegEncKey( int DeleteMode=FALSE );
		
		//--------------------------------------------------------
		//GetReg
		BOOL	GetReg( char*szKeyName,char*szDataName,char*buf,int buf_len,int keyMode=1);

		//PutReg
		BOOL	PutReg( char*szKeyName,char*szDataName,char*buf);

		//DelReg
		BOOL	DelReg( char*szKeyName,char*szDataName);


	private:
		//GetMac
		BOOL	GetMac( unsigned char*buf );
		int		GetMacAddress();	// 

		long htol( char *hexstr , short len );

		BOOL	m_Enc_Flg;	// TRUE=OK , FALSE=NG
		int		m_EndErrNo;	// 1=LAN
							// 2=No regist key
							// 3=Illegal key


};



#endif
