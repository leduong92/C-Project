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
		operator =(const char*);
		operator =(unsigned char*);
		operator =(unsigned char);
		operator =(const char);
		operator =(TString&);

		// �����Z�b�g
		Set(char* str,int len);

		void RTrim();						// �E���̃X�y�[�X���Ƃ�
		void LTrim();						// �E���̃X�y�[�X���Ƃ�
		void Dash(void);				// �V���O���R�[�e�C�V�����̗L�����`�F�b�N�Ə��u���{��

		void Dash(TString& ret);				// �V���O���R�[�e�C�V�����̗L�����`�F�b�N�Ə��u���{��

	private:
		unsigned char *m_str;				// ������ւ̃|�C���^
		unsigned int m_len;					// ���݂̃o�b�t�@��

		BOOL Destroy();						// �������N���A
		void Create(void* str,int leng);

};


#endif
