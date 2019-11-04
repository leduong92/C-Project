#ifndef EXTSTRING_H
#define EXTSTRING_H
//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:Strings.H
//	Class		:TString
//				: 簡易版可変長文字列操作クラス
//	作成日		:1999.08.19	Coded by Coba
//					!最大６４Ｋまで
//========================================================================================
//	修正
//	2003.06.02 coba LTrim()のバグ取り
//========================================================================================

#include	<windows.h>
#include	<string.h>


#define LENGTH_MAX 65536
class TString
{
	public:
		TString();
		TString(const char* str);
		TString(char* str);
		TString(unsigned char* str);
		~TString();

		// 現在の文字列長
		int len(){return (int)m_len;}

		// 変換関数
		operator char*(){return(char*)m_str;}
		operator unsigned char*(){return (unsigned char*)m_str;}

		// 代入関数
		operator =(const char*);
		operator =(unsigned char*);
		operator =(unsigned char);
		operator =(const char);
		operator =(TString&);

		// 文字セット
		Set(char* str,int len);

		void RTrim();						// 右側のスペースをとる
		void LTrim();						// 右側のスペースをとる
		void Dash(void);				// シングルコーテイションの有無をチェックと処置を施す

		void Dash(TString& ret);				// シングルコーテイションの有無をチェックと処置を施す

	private:
		unsigned char *m_str;				// 文字列へのポインタ
		unsigned int m_len;					// 現在のバッファ長

		BOOL Destroy();						// メモリクリア
		void Create(void* str,int leng);

};


#endif
