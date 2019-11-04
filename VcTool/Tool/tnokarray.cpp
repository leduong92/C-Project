//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:TNokArray.cpp
//	Class		:TNokArray
//				: 簡易版可変配列操作
//	作成日		:2002.02.26	Coded by coba
//========================================================================================
#include "TNokArray.h"

#pragma pack(push,1)

//=======================================================================================================
//	TNokArray::TNokArray(void)
//	デフォルトコンストラクタ
//========================================================================================================
TNokArray::TNokArray(void)
{
	m_len = 0;
	m_allocsize = 0;
	m_counts = 0;
	m_data = NULL;
}

//=======================================================================================================
//	TNokArray::TNokArray(void)
//	デストラクタ
//	機能　：メモリの解放
//========================================================================================================
TNokArray::~TNokArray(void)
{
	if( m_data != NULL ) free(m_data);
}

//=======================================================================================================
//	TNokArray::TNokArray(void)
//	機能　：データ領域のクリア
//========================================================================================================
void TNokArray::Reset(void)
{
	if( m_data != NULL ) free(m_data);
	m_allocsize = 0;
	m_counts = 0;
	m_data = NULL;

}

int TNokArray::SetElementLen(unsigned short len)
{
	if( m_len > 0 ) return(-1);
	if( len <0 ) return(-2);
	m_len = len;
	return(0);
}
int TNokArray::SetArraySize(unsigned short size)
{
	this->Reset();

	if( m_len <= 0 ) return(-1);
	if( size <=0) return(-2);
	m_data = (char*)malloc(size*m_len);
	if(m_data==NULL) return(-3);
	m_allocsize=size;
	return(0);
}

int TNokArray::Add(void* data)
{
	char* _data;

	if( m_len <= 0 ) return(-1);
	if(m_allocsize==m_counts){
		_data=(char*)realloc(m_data,(m_allocsize+1)*m_len);
		m_allocsize ++;
		m_data = _data;
	}

	if( m_data==NULL){
		this->Reset();
		return(-2);
	}

	memcpy( m_data+(m_counts*m_len), (char*)data, m_len);
	m_counts++;
	return(0);

}

int TNokArray::Get(void* buf, int index)
{
	if( m_len <= 0 ) return(-1);
	if( m_counts < index ) return(-2);
	if( m_data==NULL) return(-3);
	memcpy( buf, m_data+(index*m_len), m_len );
	return(0);

}
int TNokArray::Set(void* buf, int index)
{
	if( m_len <= 0 ) return(-1);
	if( m_counts < index ) return(-2);
	if( m_data==NULL) return(-3);
	memcpy( m_data+(index*m_len), buf,m_len );
	return(0);

}
#pragma pack(pop)
