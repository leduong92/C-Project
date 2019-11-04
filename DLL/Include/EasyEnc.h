#ifndef EASY_ENC_H
#define EASY_ENC_H
//=======================================================================================================
//	簡易版暗号化処理クラス
//	Class Name EasyEnc.h
//	File Name CEasyEnc.h
//	Date : 2006.10.12 N.Maeda
//=======================================================================================================

#include "windows.h"

#define ENC_REG_KEY "Software\\NokEasyEnc"
#define ENC_REG_DATA "EASY_ENC_KEY1"	// キー
#define ENC_REG_DATE "EASY_ENC_DATE"	// キーを登録した日

// 2006.10.12
// 簡易暗号化レジストリ照合コード
// MACアドレスをＸＯＲで見えなくする
#define ENC_REG_CODE "ISHIDA"

#define L_MAC_STRING_MAX 10

class CEasyEnc
{
	public:
		CEasyEnc(void);
		~CEasyEnc(void);

		// プログラムの最初にレジストリの値が正しいか？チェックする
		// レジストリの登録が正しくない場合は、値を削除する。
		// （以降のファイルオープンのチェックで引っかかる）
		BOOL	CheckRegEncKey( char*EncCode );

		// ファイル操作クラスでレジストリに値が登録されているか？チェックする
		// TText2からCALL
		BOOL	CheckEnc(void);

		// レジストキー登録
		BOOL	SetRegEncKey( char*EncCode );

		// レジストキー削除
		BOOL	DelRegEncKey(void);


		//--------------------------------------------------------
		//GetReg
		BOOL	GetReg( char*szKeyName,char*szDataName,char*buf,int buf_len);

		//PutReg
		BOOL	PutReg( char*szKeyName,char*szDataName,char*buf);

		//DelReg
		BOOL	DelReg( char*szKeyName,char*szDataName);

	protected:

		//GetMac
		BOOL	GetMac( unsigned char*buf );
		int	GetMacAddress();	// 

	private:

		long htol( char *hexstr , short len );


		char MacString[L_MAC_STRING_MAX][6+1];

};



#endif
