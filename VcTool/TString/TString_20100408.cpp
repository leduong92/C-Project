//========================================================================================
//
//	FileName	:Strings.cpp
//	Class		:TString
//				: 簡易版可変長文字列操作クラス
//	作成日		:1999.08.19	Coded by Coba
//========================================================================================
#include "TString.h"
#include <mbstring.h>
#include <mbctype.h>

//=======================================================================================================
//	TString::TString()
//	デフォルトコンストラクタ
//========================================================================================================
TString::TString()
{
	// 初期化
	m_len = 0;
	m_str = NULL;

	// 文字列作成
	Create( NULL, m_len );
}


//=======================================================================================================
//	TString::TString(const char* str)
//	コンストラクタ
//	引数１：設定する文字列
//========================================================================================================
TString::TString(const char* str)
{
	m_len = 0;
	m_str = NULL;

	if( str==NULL)
			m_len = 0;

	// 長さ算出
	else{
		m_len = strlen(str)> LENGTH_MAX ? LENGTH_MAX : strlen(str);
	}

	// 文字列作成
	Create( (char*)str, m_len );
}
//=======================================================================================================
//	TString::TString(const char* str)
//	コンストラクタ
//	引数１：設定する文字列
//========================================================================================================
TString::TString(unsigned char* str)
{
	m_len = 0;
	m_str = NULL;

	if( str==NULL)		m_len = 0;

	// 長さ算出
	else m_len = strlen((char*)str)> LENGTH_MAX ? LENGTH_MAX : strlen((char*)str);

	// 文字列作成
	Create( str, m_len );

}

//=======================================================================================================
//	TString::TString(char* str)
//	コンストラクタ
//	引数１：設定する文字列
//========================================================================================================
TString::TString(char* str)
{
	m_len = 0;
	m_str = NULL;

	if( str==NULL)		m_len = 0;

	// 長さ算出
	m_len = strlen(str)> LENGTH_MAX ? LENGTH_MAX : strlen(str);

	// 文字列作成
	Create( str, m_len );

}

//=======================================================================================================
//	TString::~TString()
//	デストラクタ
//========================================================================================================
TString::~TString()
{
	// メモリ解放
	Destroy();
}

//=======================================================================================================
//	TString::operator =(const char* str)
//	変換関数 = TString
//	引数１：設定する文字列
//	＊前に設定されていた文字列は破棄されます
//========================================================================================================
TString::operator =(TString& ts)
{
	// リセット
	Destroy();

	// 文字列作成
	Create( (char*)ts, ts.len() );

	return(0);
}

//=======================================================================================================
//	TString::operator =(const char* str)
//	変換関数 = char*
//	引数１：設定する文字列
//	＊前に設定されていた文字列は破棄されます
//========================================================================================================
TString::operator =(const char* str)
{
	// リセット
	Destroy();

	// 長さ算出
	if( str==NULL)	m_len = 0;
	else m_len = strlen(str)> LENGTH_MAX ? LENGTH_MAX : strlen(str);

	// 文字列作成
	Create( (char*)str, m_len );

	return 0;
}

//=======================================================================================================
//	TString::operator =(const char* str)
//	変換関数 = char*
//	引数１：設定する文字列
//	＊前に設定されていた文字列は破棄されます
//========================================================================================================
TString::operator =(const char a)
{
	char str[2];

	// リセット
	Destroy();

	// 長さ算出
	m_len = 1;
	str[0]=a;str[1]=0;

	// 文字列作成
	Create( (char*)str, m_len );

	return 0;
}
//=======================================================================================================
//	TString::operator =(const char* str)
//	変換関数 = char*
//	引数１：設定する文字列
//	＊前に設定されていた文字列は破棄されます
//========================================================================================================
TString::operator =(unsigned char a)
{
	char str[2];

	// リセット
	Destroy();

	// 長さ算出
	m_len = 1;
	str[0]=a;str[1]=0;

	// 文字列作成
	Create( (char*)str, m_len );

	return 0;
}

//=======================================================================================================
//	TString::Set(char* str,int len)
//	文字代入
//	引数１：設定する文字列
//	＊前に設定されていた文字列は破棄されます
//========================================================================================================
int TString::Set(char* str,int len)
{
	// リセット
	Destroy();

	// 長さ算出
	if( str==NULL)	m_len = 0;
	else{
		m_len = len > LENGTH_MAX ? LENGTH_MAX : len;
		if( strlen(str) < m_len ) m_len = strlen(str);
	}
	// 文字列作成
	Create( str, m_len );

	return 0;
}

//=======================================================================================================
//	TString::operator =(const char* str)
//	変換関数 =
//	引数１：設定する文字列
//	＊前に設定されていた文字列は破棄されます
//========================================================================================================
TString::operator =(unsigned char* str)
{
	// リセット
	Destroy();

	if( str==NULL)	m_len = 0;

	// 長さ算出
	else m_len = strlen((char*)str)> LENGTH_MAX ? LENGTH_MAX : strlen((char*)str);

	// 文字列作成
	Create( str, m_len );

	return 0;
}

//=======================================================================================================
//	TString::operator ==(TString& target)
//	比較演算 ==
//	引数１：比較する文字列
//	戻り値  1:左辺と右辺は等しい
//========================================================================================================
TString::operator ==(TString& target)
{
	if( m_str ==NULL )	return(0);

	if( (int)m_len != target.len()) return(0);

	if( !m_len ) return(1);		// 両方とも空

	if( memcmp(m_str,(char*)target,m_len)) return(0);

	return 1;
}

//=======================================================================================================
//	TString::operator !=(TString& target)
//	比較演算 !=
//	引数１：比較する文字列
//	戻り値  1:左辺と右辺は等しくない
//========================================================================================================
TString::operator !=(TString& target)
{

	if( !m_len && !target.len()) return(0);		// 両方とも空=同じ

	// 同じ文字列
	if( (int)m_len == target.len() && !memcmp(m_str,(char*)target,m_len)) return(0);

	return 1;
}

//=======================================================================================================
//	TString::operator ==(char* target)
//	比較演算 ==
//	引数１：比較する文字列
//	戻り値  1:左辺と右辺は等しい
//========================================================================================================
TString::operator ==(char* target)
{
	if( m_str  ==NULL )	return(0);
	if( target ==NULL )	return(0);

	if( (int)m_len != strlen(target)) return(0);

	if( !m_len ) return(1);		// 両方とも空

	if( memcmp(m_str,(char*)target,m_len)) return(0);

	return 1;
}

//=======================================================================================================
//	TString::operator !=(TString& target)
//	比較演算 !=
//	引数１：比較する文字列
//	戻り値  1:左辺と右辺は等しくない
//========================================================================================================
TString::operator !=(char* target)
{

	if( !m_len && ( target==NULL || !strlen(target))) return(0);		// 両方とも空=同じ

	// 同じ文字列
	if( (int)m_len == strlen(target) && !memcmp(m_str,target,m_len)) return(0);

	return 1;
}



//=======================================================================================================
//	BOOL TString::Destroy()
//	文字列領域のクリア
//========================================================================================================
BOOL TString::Destroy()
{
	if( m_str ==NULL )	return(FALSE);

	delete m_str;
	m_str = NULL;
	m_len =0;

	return(0);
}

//=======================================================================================================
//	void TString::Create()
//	文字列領域の作成
//========================================================================================================
void TString::Create(void* str, int leng)
{

	if( m_str !=NULL ) delete m_str;

	// メモリ確保
	m_str = new unsigned char[leng+1];
	memset( m_str, 0,leng+1);

	if( str !=NULL)
		memcpy( m_str, (char*)str, leng);

	m_len = leng;
	return;

}


//=======================================================================================================
//	void TString::Create()
//	文字列の右側のスペースをはずす
//========================================================================================================
void TString::RTrim()
{
	if( !m_len)	return;

	int i;
	unsigned int newlen;

	for(i=m_len;i>0;i--){
		if( m_str[i-1]!=0x20) break;
		if( m_str[i-1]==0x20) m_str[i-1] = 0;
	}

	// はずしたのか？
	newlen = strlen((char*)m_str);
	if( m_len == newlen ) return;

	// メモリ入れ替え
	char *tmp = new char[newlen+1];
	memset( tmp, 0, newlen+1);
	memcpy( tmp, m_str, newlen);

	Destroy();
	Create( tmp, newlen );

	delete tmp;

}
//=======================================================================================================
//	void TString::Create()
//	文字列の左側のスペースをはずす
//========================================================================================================
void TString::LTrim()
{
	if( !m_len)	return;

	unsigned int newlen,i;

	for(i=0;i<m_len;i++){
		if( m_str[i]!=0x20) break;
	}

	// 先頭にｽﾍﾟｰｽ無し
	if( i==0 || i==m_len) return;

	// メモリ入れ替え
	char *tmp = new char[m_len+1];
	memset( tmp, 0, m_len+1);
	memcpy( tmp, &m_str[i], m_len-i+1);
	newlen=strlen(tmp);

	Destroy();
	Create( tmp, newlen );

	delete tmp;

}
//=======================================================================================================
//	void TString::Dash()
//	シングルコーテイションの有無をチェックと処置を施す
//========================================================================================================
void TString::Dash(void)
{
	if( !m_len)	return;

	unsigned int i,j,count;
	unsigned int newlen;

	// シングルクート'0x27があるか？
	count=0;
	for(i=0 ;i<m_len;i++){
		if( m_str[i]==0x27) count++;
		// エスケープシーケンス \\ はあるか？ 2010.02.26 ﾏﾂﾀﾞ
		if( m_str[i]==0x5c && _mbsbtype(m_str,i) == _MBC_SINGLE  ) count++;
	}

	// シングルクオートなし
	if( !count ){
//		ret = m_str;
		return;
	}
	// シングルクオートを重ねる
	newlen = m_len+count;
	char *tmp = new char[newlen+1];
	memset( tmp, 0, newlen+1);
	// 2006.11.25 コピー長はnewlenではなくm_len! システム部小田の指摘
	memcpy( tmp, m_str, m_len);	//memcpy( tmp, m_str, newlen);

	for(i=0,j=0 ;i<m_len;i++,j++){
		tmp[j] = m_str[i];

		if( m_str[i]==0x27) tmp[++j]=0x27;
		//エスケープシーケンスを追加する 2010.02.26 ﾏﾂﾀﾞ
		if( m_str[i]==0x5c && _mbsbtype(m_str,i) == _MBC_SINGLE ) tmp[++j]=0x5c;
	}

	Destroy();
	Create( tmp, newlen );

	delete tmp;

}


void TString::Dash(TString& ret)
{
	if( !m_len){
		ret="";
		return;
	}

	unsigned int i,j;
	unsigned int newlen,count;

	// シングルクート'0x27があるか？
	count=0;
	for(i=0 ;i<m_len;i++){
		if( m_str[i]==0x27) count++;
		// エスケープシーケンス \\ はあるか？ 2010.02.26 ﾏﾂﾀﾞ
		if( m_str[i]==0x5c && _mbsbtype(m_str,i) == _MBC_SINGLE  ) count++;
	}

	// シングルクオートなし
	if( !count ){
		ret = m_str;
		return;
	}
	// シングルクオートを重ねる
	newlen = m_len+count;
	char *tmp = new char[newlen+1];
	memset( tmp, 0, newlen+1);
	// 2006.11.25 コピー長はnewlenではなくm_len! システム部小田の指摘
	memcpy( tmp, m_str, m_len);	//memcpy( tmp, m_str, newlen);

	for(i=0,j=0 ;i<m_len;i++,j++){
		tmp[j] = m_str[i];
		if( m_str[i]==0x27) tmp[++j]=0x27;
		//エスケープシーケンスを追加する 2010.02.26 ﾏﾂﾀﾞ
		if( m_str[i]==0x5c && _mbsbtype(m_str,i) == _MBC_SINGLE ) tmp[++j]=0x5c;
	}

	ret = (char*)tmp;

	delete tmp;

}

