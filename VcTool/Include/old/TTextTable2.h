#ifndef TTEXTDB2_H
#define TTEXTDB2_H
//=======================================================================================================
//	�e�L�X�gDBAccess�N���X�̃w�b�_
//	Class Name TTextTable2
//	File Name TTextTable2.h
//	�쐬�ҁFCoba
//	�쐬���F1999.05.10
//=======================================================================================================
//	�C��
//	2003.03.04 coba GetLast()�֐���ǉ�
//	2003.05.21 coba path���������AMAX_PATH�ɕύX
//					key�ޔ��o�b�t�@������Ȃ��ăI�[�o�[�t���[����o�O���C��
//	2006.10.11	Maeda	XOR �Í��������ǉ�(�b���)
//=======================================================================================================

#include "windows.h"
#include "winuser.h"
#include "winbase.h"

#include "EasyEnc.h"

// �G���[�R�[�h
#define TXDB_NOFILE	72
#define DEF_BUFSIZE 4096
#define TXDB_MAXKEYLEN	128
#define TXDB_WORKING 144

//----------------------------
// �ȈՈÍ����R�[�h
#define EASY_ENC_CODE 0xA5

class TTextTable2
{
	public:
		TTextTable2(void);
		~TTextTable2(void);
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

		int SetEncryption( int EncryptionCode );	// 2006.10.11 Maeda

	protected:

		typedef struct{
			char filename[30+1];
			short reclen;
			short keylen;					// �L�[���i�ő�50byte�ɂ��ĉ������j
			short offset;					// �L�[�̐擪������޲ăI�t�Z�b�g
			char *datptr;					// ���R�[�h�i�[�|�C���^
			char dir[MAX_PATH+1];					// �t�@�C���̃p�X
			char tmpdir[MAX_PATH+1];				// ��Ɨ̈�̃p�X
		} FSPEC;

		int Initialize(FSPEC *filespec,int bufsize);	//2006.10.11 Maeda
//		int InitializeEnc(FSPEC *filespec,int bufsize);
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

		// 2006.10.11 �Ǘ��Í�������  Maeda
		long	m_EncryptionEnableFlg;	// PC �̃��W�X�g���`�F�b�N����
										// 0=���W�X�g���o�^����
										// 1=���W�X�g���o�^����i�Í����ŁAAdd�\)
		long	m_Encryption;	// �Í����t���O
								// 0==Normal
								// 0!=XorCode
		BOOL ReadFileEnc(  HANDLE hFile,  LPVOID lpBuffer,  DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead,   LPOVERLAPPED lpOverlapped );
		BOOL WriteFileEnc( HANDLE hFile,  LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,LPOVERLAPPED lpOverlapped );

};

#endif
