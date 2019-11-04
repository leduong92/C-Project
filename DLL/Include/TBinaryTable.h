#ifndef TBINARYDB_NOK_H
#define TBINARYDB_NOK_H
//=======================================================================================================
//	�ϒ��o�C�i���t�@�C������N���X�̃w�b�_
//	Class Name TBinaryTable
//	File Name TBinaryTable.h
//	�쐬�ҁFcoba
//	�쐬���F2002.02.01
//	�C�����F
//	�t�@�C���̍ő�T�C�Y 4,294,967,295�o�C�g
//	���R�[�h�̍ő咷	65,535-16(�|�W�V�����u���b�N)=65,519 �o�C�g
//	���R�[�h�̐擪�Ƀ|�W�V�������i���R�[�h�ʒu�⃌�R�[�h���j�������Ă�
//=======================================================================================================

#include "windows.h"
#include "winuser.h"
#include "winbase.h"

#include <stdio.h>
#include <time.h>
#include <share.h>
#include <io.h>

#pragma pack(push,1)

// �G���[�R�[�h(Btrieve�̃p�N��)
#define BTB_TOF	0
#define BTB_OK			0
#define BTB_OPE_ERR		1
#define BTB_IO_ERR		2
#define BTB_NOTFIND		4
#define BTB_DUP			5
#define BTB_KEY_ERR		6
#define BTB_POS_ERR		8
#define BTB_EOF			9
#define BTB_NAME_FAULT	11
#define BTB_NOFILE	 	12
#define BTB_NOINIT	 	20
#define BTB_KEYLEN_ERR	 	21
#define BTB_DBUFLEN_ERR	 	22
#define BTB_CANT_CREATE	 	25
#define BTB_RECLEN_ERR	 	28
// �݊����̖����t�@�C��
#define BTB_DIFF_FILE	 	30
#define BTB_LOCKED	 	85


// windows����߂��Ƃ��̑΍�
#ifndef MAX_PATH
#define MAX_PATH 255
#endif

#define NO_POSITON 0
#define MAX_RECORDLENGTH 65519
#define MAX_KEY_LENGTH 100

// ��`��
static const char* BF_NOK_NAME="BF_NOK";

// �o�[�W����
static const char* BF_NOK_VERSION="--.--";

// �t�@�C���\����`
typedef struct{
	unsigned short keylen;					// �L�[��(�ő�100)
	unsigned short offset;					// �L�[�̐擪������޲ăI�t�Z�b�g
} BF_SPEC;

// �t�@�C���w�b�_(���̍\���Ɉ�v���Ȃ��t�@�C���̓G���[)
typedef struct{
	char code[6];			//"BF_NOK"
	char date[8];			// YYYYMMDD		// �쐬��
	char ver[5];			// **.**
	unsigned short keylen;					// �L�[��(�ő�100)
	unsigned short offset;					// �L�[�̐擪������޲ăI�t�Z�b�g
	unsigned long first_pos;			// �ŏ��̃��R�[�h�̃t�@�C���擪����̈ʒu
	unsigned long last_pos;				// �ŏI���R�[�h�̃t�@�C���擪����̈ʒu
	char crlf[2];
}BF_HEAD;


class TBinaryTable
{
	public:
		TBinaryTable(void);
		~TBinaryTable(void);

	protected:
		int Open(char* filename);

		int Del( void *key );
		int Add( void* data, unsigned short reclen);
		int Upd( void* data, unsigned short reclen);


		int First( unsigned short* reclen);
		int Last( unsigned short* reclen);
		int Next( unsigned short* reclen);
		int Prev( unsigned short* reclen);
		int Create(char* fname,BF_SPEC *filespec);

		int Find( void *key ,unsigned short* reclen);
		int GE( void *key ,unsigned short* reclen);
		int GT( void *key ,unsigned short* reclen);
		int LE( void *key ,unsigned short* reclen);
		int LT( void *key ,unsigned short* reclen);

		int ReadRecord(void* buf, unsigned short bufsize);


//		int GetFileInfo(BF_HEAD* buf,char* filename);

	private:
		char m_fName[MAX_PATH+1];		// ���쒆�̃t�@�C����

		// �|�W�V�����u���b�N
		struct BF_POSBLOCK{
			unsigned short del_sign;		// �L���t���O
			unsigned long rec_pos;			// �t�@�C���擪����̈ʒu�i�|�W�V�����u���b�N���܂ށj
			unsigned short rec_len;			// �i�|�W�V�����u���b�N���������j���R�[�h��
			unsigned long next_pos;			// ���̃��R�[�h�̃t�@�C���擪����̈ʒu
			unsigned long prev_pos;			// �O�̃��R�[�h�̃t�@�C���擪����̈ʒu
		} m_Pblock;

		int Move(unsigned short* reclen,int direction);

		int SearchFile(char* fname);
		int SearchKey( void *key ,int operation ,unsigned short* reclen);

};
#pragma pack(pop)

#endif
