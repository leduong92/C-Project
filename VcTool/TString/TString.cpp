//========================================================================================
//
//	FileName	:Strings.cpp
//	Class		:TString
//				: �ȈՔŉϒ������񑀍�N���X
//	�쐬��		:1999.08.19	Coded by Coba
//========================================================================================
#include "TString.h"
#include <mbstring.h>
#include <mbctype.h>

//=======================================================================================================
//	TString::TString()
//	�f�t�H���g�R���X�g���N�^
//========================================================================================================
TString::TString()
{
	// ������
	m_len = 0;
	m_str = NULL;

	// ������쐬
	Create( NULL, m_len );
}


//=======================================================================================================
//	TString::TString(const char* str)
//	�R���X�g���N�^
//	�����P�F�ݒ肷�镶����
//========================================================================================================
TString::TString(const char* str)
{
	m_len = 0;
	m_str = NULL;

	if( str==NULL)
			m_len = 0;

	// �����Z�o
	else{
		m_len = (unsigned int)strlen(str)> LENGTH_MAX ? LENGTH_MAX :(unsigned int) strlen(str);
	}

	// ������쐬
	Create( (char*)str, m_len );
}
//=======================================================================================================
//	TString::TString(const char* str)
//	�R���X�g���N�^
//	�����P�F�ݒ肷�镶����
//========================================================================================================
TString::TString(unsigned char* str)
{
	m_len = 0;
	m_str = NULL;

	if( str==NULL)		m_len = 0;

	// �����Z�o
	else m_len = (unsigned int)strlen((char*)str)> LENGTH_MAX ? LENGTH_MAX : (unsigned int)strlen((char*)str);

	// ������쐬
	Create( str, m_len );

}

//=======================================================================================================
//	TString::TString(char* str)
//	�R���X�g���N�^
//	�����P�F�ݒ肷�镶����
//========================================================================================================
TString::TString(char* str)
{
	m_len = 0;
	m_str = NULL;

	if( str==NULL)		m_len = 0;

	// �����Z�o
	m_len = (unsigned int)strlen(str)> LENGTH_MAX ? LENGTH_MAX : (unsigned int)strlen(str);

	// ������쐬
	Create( str, m_len );

}

//=======================================================================================================
//	TString::~TString()
//	�f�X�g���N�^
//========================================================================================================
TString::~TString()
{
	// ���������
	Destroy();
}

//=======================================================================================================
//	TString::operator =(const char* str)
//	�ϊ��֐� = TString
//	�����P�F�ݒ肷�镶����
//	���O�ɐݒ肳��Ă���������͔j������܂�
//========================================================================================================
void TString::operator =(TString& ts)
{
	// ���Z�b�g
	Destroy();

	// ������쐬
	Create( (char*)ts, ts.len() );

//	return(0);
}

//=======================================================================================================
//	TString::operator =(const char* str)
//	�ϊ��֐� = char*
//	�����P�F�ݒ肷�镶����
//	���O�ɐݒ肳��Ă���������͔j������܂�
//========================================================================================================
void TString::operator =(const char* str)
{
	// ���Z�b�g
	Destroy();

	// �����Z�o
	if( str==NULL)	m_len = 0;
	else m_len = (unsigned int)strlen(str)> LENGTH_MAX ? LENGTH_MAX : (unsigned int)strlen(str);

	// ������쐬
	Create( (char*)str, m_len );

//	return 0;
}

//=======================================================================================================
//	TString::operator =(const char* str)
//	�ϊ��֐� = char*
//	�����P�F�ݒ肷�镶����
//	���O�ɐݒ肳��Ă���������͔j������܂�
//========================================================================================================
void TString::operator =(const char a)
{
	char str[2];

	// ���Z�b�g
	Destroy();

	// �����Z�o
	m_len = 1;
	str[0]=a;str[1]=0;

	// ������쐬
	Create( (char*)str, m_len );

//	return 0;
}
//=======================================================================================================
//	TString::operator =(const char* str)
//	�ϊ��֐� = char*
//	�����P�F�ݒ肷�镶����
//	���O�ɐݒ肳��Ă���������͔j������܂�
//========================================================================================================
void TString::operator =(unsigned char a)
{
	char str[2];

	// ���Z�b�g
	Destroy();

	// �����Z�o
	m_len = 1;
	str[0]=a;str[1]=0;

	// ������쐬
	Create( (char*)str, m_len );

//	return 0;
}

//=======================================================================================================
//	TString::Set(char* str,int len)
//	�������
//	�����P�F�ݒ肷�镶����
//	���O�ɐݒ肳��Ă���������͔j������܂�
//========================================================================================================
int TString::Set(char* str,int len)
{
	// ���Z�b�g
	Destroy();

	// �����Z�o
	if( str==NULL)	m_len = 0;
	else{
		m_len = len > LENGTH_MAX ? LENGTH_MAX : len;
		if( (unsigned int)strlen(str) < m_len ) m_len = (unsigned int)strlen(str);
	}
	// ������쐬
	Create( str, m_len );

	return 0;
}

//=======================================================================================================
//	TString::operator =(const char* str)
//	�ϊ��֐� =
//	�����P�F�ݒ肷�镶����
//	���O�ɐݒ肳��Ă���������͔j������܂�
//========================================================================================================
void TString::operator =(unsigned char* str)
{
	// ���Z�b�g
	Destroy();

	if( str==NULL)	m_len = 0;

	// �����Z�o
	else m_len = (unsigned int)strlen((char*)str)> LENGTH_MAX ? LENGTH_MAX : (unsigned int)strlen((char*)str);

	// ������쐬
	Create( str, m_len );

//	return 0;
}

//=======================================================================================================
//	TString::operator ==(TString& target)
//	��r���Z ==
//	�����P�F��r���镶����
//	�߂�l  1:���ӂƉE�ӂ͓�����
//========================================================================================================
int TString::operator ==(TString& target)
{
	if( m_str ==NULL )	return(0);

	if( (int)m_len != target.len()) return(0);

	if( !m_len ) return(1);		// �����Ƃ���

	if( memcmp(m_str,(char*)target,m_len)) return(0);

	return 1;
}

//=======================================================================================================
//	TString::operator !=(TString& target)
//	��r���Z !=
//	�����P�F��r���镶����
//	�߂�l  1:���ӂƉE�ӂ͓������Ȃ�
//========================================================================================================
int TString::operator !=(TString& target)
{

	if( !m_len && !target.len()) return(0);		// �����Ƃ���=����

	// ����������
	if( (int)m_len == target.len() && !memcmp(m_str,(char*)target,m_len)) return(0);

	return 1;
}

//=======================================================================================================
//	TString::operator ==(char* target)
//	��r���Z ==
//	�����P�F��r���镶����
//	�߂�l  1:���ӂƉE�ӂ͓�����
//========================================================================================================
int TString::operator ==(char* target)
{
	if( m_str  ==NULL )	return(0);
	if( target ==NULL )	return(0);

	if( (int)m_len != strlen(target)) return(0);

	if( !m_len ) return(1);		// �����Ƃ���

	if( memcmp(m_str,(char*)target,m_len)) return(0);

	return 1;
}

//=======================================================================================================
//	TString::operator !=(TString& target)
//	��r���Z !=
//	�����P�F��r���镶����
//	�߂�l  1:���ӂƉE�ӂ͓������Ȃ�
//========================================================================================================
int TString::operator !=(char* target)
{

	if( !m_len && ( target==NULL || !strlen(target))) return(0);		// �����Ƃ���=����

	// ����������
	if( (int)m_len == strlen(target) && !memcmp(m_str,target,m_len)) return(0);

	return 1;
}



//=======================================================================================================
//	BOOL TString::Destroy()
//	������̈�̃N���A
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
//	������̈�̍쐬
//========================================================================================================
void TString::Create(void* str, int leng)
{

	if( m_str !=NULL ) delete m_str;

	// �������m��
	m_str = new unsigned char[leng+1];
	memset( m_str, 0,leng+1);

	if( str !=NULL)
		memcpy( m_str, (char*)str, leng);

	m_len = leng;
	return;

}


//=======================================================================================================
//	void TString::Create()
//	������̉E���̃X�y�[�X���͂���
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

	// �͂������̂��H
	newlen = (unsigned int)strlen((char*)m_str);
	if( m_len == newlen ) return;

	// ����������ւ�
	char *tmp = new char[newlen+1];
	memset( tmp, 0, newlen+1);
	memcpy( tmp, m_str, newlen);

	Destroy();
	Create( tmp, newlen );

	delete tmp;

}
//=======================================================================================================
//	void TString::Create()
//	������̍����̃X�y�[�X���͂���
//========================================================================================================
void TString::LTrim()
{
	if( !m_len)	return;

	unsigned int newlen,i;

	for(i=0;i<m_len;i++){
		if( m_str[i]!=0x20) break;
	}

	// �擪�ɽ�߰�����
	if( i==0 || i==m_len) return;

	// ����������ւ�
	char *tmp = new char[m_len+1];
	memset( tmp, 0, m_len+1);
	memcpy( tmp, &m_str[i], m_len-i+1);
	newlen=(unsigned int)strlen(tmp);

	Destroy();
	Create( tmp, newlen );

	delete tmp;

}
//=======================================================================================================
//	Function name:char* Trim(char  target)
//	Outline:This function remove 'target' charactor from original character
//	Arg1:The character that was removed.
//========================================================================================================
void TString::Trim(char target)
{
	if( !m_len)	return;

	unsigned int newlen,i,j,count;
	count=0;
	// count the number of target character
	for(i=0;i<m_len;i++){
		if( m_str[i]!=target) count++;
	}
	if( count == m_len ) return;	//There is not target character in source data pointer

	// ����������ւ�
	char *tmp = new char[count+1];
	memset( tmp, 0, count+1);
	j=0;
	for(i=0;i<m_len;i++){
		if( m_str[i] != target) tmp[j++] = m_str[i];
	}
	newlen=(unsigned int)strlen(tmp);

	Destroy();
	Create( tmp, newlen );

	delete tmp;

}

//=======================================================================================================
//	void TString::Dash()
//	�V���O���R�[�e�C�V�����̗L�����`�F�b�N�Ə��u���{��
//========================================================================================================
void TString::Dash(void)
{
	if( !m_len)	return;

	unsigned int i,j,count;
	unsigned int newlen;

	// �V���O���N�[�g'0x27�����邩�H
	count=0;
	for(i=0 ;i<m_len;i++){
		if( m_str[i]==0x27) {
			//���̕������0x27�Ȃ�OK 2010.04.08 ����
			if( i+1<m_len && m_str[i+1]==0x27){
				i++;
				continue;
			}
			count++;
		}
		// �G�X�P�[�v�V�[�P���X \\ �͂��邩�H 2010.02.26 ����
		if( m_str[i]==0x5c && _mbsbtype(m_str,i) == _MBC_SINGLE  ){
			//���̕������0x5c�Ȃ�OK  2010.04.08 ����
			if( i+1<m_len && m_str[i+1]==0x5c){
				i++;
				continue;
			}
			count++;
		}
	}

	// �V���O���N�I�[�g�Ȃ�
	if( !count ){
		return;
	}
	// �V���O���N�I�[�g���d�˂�
	newlen = m_len+count;
	char *tmp = new char[newlen+1];
	memset( tmp, 0, newlen+1);
	// 2006.11.25 �R�s�[����newlen�ł͂Ȃ�m_len! �V�X�e�������c�̎w�E
	memcpy( tmp, m_str, m_len);	//memcpy( tmp, m_str, newlen);

	for(i=0,j=0 ;i<m_len;i++,j++){
		tmp[j] = m_str[i];
		if( m_str[i]==0x27){
			//���̕������0x27�Ȃ�OK  2010.04.08 ����
			if( i+1<m_len && m_str[i+1]==0x27){
				tmp[++j] = m_str[++i];
				continue;
			}
			else tmp[++j]=0x27;
		}
		//�G�X�P�[�v�V�[�P���X��ǉ����� 2010.02.26 ����
		if( m_str[i]==0x5c && _mbsbtype(m_str,i) == _MBC_SINGLE ){
			//���̕������0x5c�Ȃ�OK  2010.04.08 ����
			if( i+1<m_len && m_str[i+1]==0x5c){
				tmp[++j] = m_str[++i];
				continue;
			}
			else tmp[++j]=0x5c;
		}
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

	// �V���O���N�[�g'0x27�����邩�H
	count=0;
	for(i=0 ;i<m_len;i++){
		if( m_str[i]==0x27){
			//���̕������0x27�Ȃ�OK  2010.04.08 ����
			if( i+1<m_len && m_str[i+1]==0x27){
				i++;
				continue;
			}
			count++;
		}
		// �G�X�P�[�v�V�[�P���X \\ �͂��邩�H 2010.02.26 ����
		if( m_str[i]==0x5c && _mbsbtype(m_str,i) == _MBC_SINGLE  ){
			//���̕������0x5c�Ȃ�OK  2010.04.08 ����
			if( i+1<m_len && m_str[i+1]==0x5c){
				i++;
				continue;
			}
			count++;
		}

	}

	// �V���O���N�I�[�g�Ȃ�
	if( !count ){
		ret = m_str;
		return;
	}
	// �V���O���N�I�[�g���d�˂�
	newlen = m_len+count;
	char *tmp = new char[newlen+1];
	memset( tmp, 0, newlen+1);
	// 2006.11.25 �R�s�[����newlen�ł͂Ȃ�m_len! �V�X�e�������c�̎w�E
	memcpy( tmp, m_str, m_len);	//memcpy( tmp, m_str, newlen);

	for(i=0,j=0 ;i<m_len;i++,j++){
		tmp[j] = m_str[i];
		if( m_str[i]==0x27){
			//���̕������0x27�Ȃ�OK  2010.04.08 ����
			if( i+1<m_len && m_str[i+1]==0x27){
				tmp[++j] = m_str[++i];
				continue;
			}
			else tmp[++j]=0x27;
		}
		//�G�X�P�[�v�V�[�P���X��ǉ����� 2010.02.26 ����
		if( m_str[i]==0x5c && _mbsbtype(m_str,i) == _MBC_SINGLE ){
			//���̕������0x5c�Ȃ�OK  2010.04.08 ����
			if( i+1<m_len && m_str[i+1]==0x5c){
				tmp[++j] = m_str[++i];
				continue;
			}
			else tmp[++j]=0x5c;
		}
	}

	ret = (char*)tmp;

	delete tmp;

}
