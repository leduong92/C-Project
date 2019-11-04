#ifndef TTEXTDB_H
#define TTEXTDB_H
//=======================================================================================================
//	�e�L�X�gDBAccess�N���X�̃w�b�_
//	Class Name TTextTable
//	File Name TTextTable.h
//	�쐬�ҁFCoba
//	�쐬���F1999.05.10
//=======================================================================================================
//	�C��
//	2003.03.04 coba GetLast()�֐���ǉ�
//	2003.05.21 coba path���������AMAX_PATH�ɕύX
//					key�ޔ��o�b�t�@������Ȃ��ăI�[�o�[�t���[����o�O���C��
//	2014.04.10 coba �t�@�C�������֐�Drop()��ǉ�
//  2015.02.26 sanno	�t�@�C�����̒������g��
//=======================================================================================================

#include "windows.h"
#include "winuser.h"
#include "winbase.h"

// �G���[�R�[�h
#define TXDB_NOFILE	72
#define DEF_BUFSIZE 4096
#define TXDB_MAXKEYLEN	128
#define TXDB_WORKING 144

class TTextTable
{
	public:
		TTextTable(void);
		~TTextTable(void);
		int GetEqual( void *key );
		int GetEqual2( void *key );
		int FindFirst( void *key );
		int Upd( void *key);
		int Upd2( void *key);
		int Del( void *key );
		int Del2( void *key );		//2004.11.09 ����
		int Insert( void *key );
		int Insert2( void *key );	//2004.11.09 ����
		int GetFirst(void);
		int GetLast( void  );
		int Next(void);
		int Prev(void);
		int Add( void );

		int GetCounts(void);

		int Drop(void);		// 2014.4.10 coba

	protected:

		typedef struct{
//			char filename[30+1];
			//char filename[50+1];	//2009.01.22 ���� �T�C�Y�ύX
			char filename[80+1];	//2015.02.26 sanno �T�C�Y�ύX
			short reclen;
			short keylen;					// �L�[���i�ő�50byte�ɂ��ĉ������j
			short offset;					// �L�[�̐擪������޲ăI�t�Z�b�g
			char *datptr;					// ���R�[�h�i�[�|�C���^
			char dir[MAX_PATH+1];					// �t�@�C���̃p�X
			char tmpdir[MAX_PATH+1];				// ��Ɨ̈�̃p�X
		} FSPEC;

		int Initialize(FSPEC *filespec,int bufsize);
		FSPEC m_fSpec;					// �t�@�C���X�y�b�N

	private:

		long m_PrevRecNo;				// ���O�ɑ��삵�����R�[�h�ԍ�
		void Clear(void);

		int m_BufSize;					// �t�@�C�����쎞�̃o�b�t�@��

		int Open( HANDLE& hFile ,BOOL readonly);
		void Close(HANDLE& hFile);
		
		int CheckFile(void);			// 2004.11.09 ����

		int MoveRecord( HANDLE hfile ,void *key , long *recNo );		// �L�[�ɊY������ں��ނɈړ�����
		int MoveRecord2( HANDLE hfile ,void *key , long *recNo );		// �L�[�ɊY������ں��ނɈړ�����(��)


};

#endif
