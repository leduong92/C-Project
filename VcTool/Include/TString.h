#ifndef EXTSTRING_H
#define EXTSTRING_H
//========================================================================================
//	NOK Windows Class Libraly
//	FileName	:Strings.H
//	Class		:TString
//				: �ȈՔŉϒ������񑀍�N���X
//	�쐬��		:1999.08.19	Coded by Coba
//					!�ő�U�S�j�܂�
//========================================================================================
//	�C��
//	2003.06.02 coba LTrim()�̃o�O���
//	2006.11.25 coba Dash()�֐��ŁA�ϊ����̃R�s�[�����Ԉ���Ă����o�O�C��
//	2006.12.07 coab ��r���Z == ��!=��ǉ�
//	2008.01.30 coba ��r���Z == ��!=��char�^��ǉ�
//	2010.04.08 ���� Dash()�֐���0x27��0x5c���A�����Ă��ď������K�v�����ꍇ�ɂ͖������Ƃ���悤�ύX
//	2013.04.18 matsuda Add Trim( char target ) function
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

		// ���݂̕�����
		int len(){return (int)m_len;}

		// �ϊ��֐�
		operator char*(){return(char*)m_str;}
		operator unsigned char*(){return (unsigned char*)m_str;}

		// ����֐�
		void operator =(const char*);
		void operator =(unsigned char*);
		void operator =(unsigned char);
		void operator =(const char);
		void operator =(TString&);
		// ��r���Z 2006.12.07
		int operator !=(TString&);
		int operator ==(TString&);
		// ��r���Z 2008.01.30
		int operator !=(char*);
		int operator ==(char*);

		// �����Z�b�g
		int Set(char* str,int len);

		void RTrim();						// �E���̃X�y�[�X���Ƃ�
		void LTrim();						// �E���̃X�y�[�X���Ƃ�
		void Trim(char target);				// remove target character into the data pointer.
		void Dash(void);				// �V���O���R�[�e�C�V�����̗L�����`�F�b�N�Ə��u���{��

		void Dash(TString& ret);				// �V���O���R�[�e�C�V�����̗L�����`�F�b�N�Ə��u���{��
		int funcSplit(char *str,char *str1,char *str2,char *s);

	private:
		unsigned char *m_str;				// ������ւ̃|�C���^
		unsigned int m_len;					// ���݂̃o�b�t�@��

		BOOL Destroy();						// �������N���A
		void Create(void* str,int leng);

};


#endif
