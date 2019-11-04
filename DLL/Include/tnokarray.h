#ifndef NOK_ARRY_H
#define NOK_ARRY_H
//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TNokArray.H
//	Class		:TNokArray
//				: 簡易版可変配列操作
//	作成日		:2002.02.26	Coded by coba
//========================================================================================
//	修正履歴
//	2003.06.25 coba 要素上書き関数 Set()を追加
//========================================================================================

#include	<stdio.h>
#include	<string.h>
#include	<malloc.h>

#pragma pack(push,1)

class TNokArray
{
	public:
		TNokArray(void);
		~TNokArray(void);

		int SetElementLen(unsigned short len);

		int SetArraySize(unsigned short size);

		void Reset(void);
		int Add(void* data);
		int Get(void* buf, int index);
		int Set(void* buf, int index);
		unsigned short GetLen(void){return(m_len);};
		int GetTotal(void){return(m_counts);};

	protected:
	private:
		unsigned short m_len;
		unsigned short m_allocsize;
		int m_counts;
		char* m_data;
};
#pragma pack(pop)
#endif
