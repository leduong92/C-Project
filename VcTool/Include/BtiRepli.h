#ifndef BTIACS_EXT_H
#define BTIACS_EXT_H
//========================================================================================
//	Btrieve.cpp�̃t�@�C������@�\�N���X(�g����:�t�@�C���~���[�����O�@�\�t��)
//	�t�@�C������N���X���쐬����Ƃ��͂�����p�����ĉ������I
//	FileName	:BtiRepli.H
//	Class		:TBtiReplication
//	�쐬��		:2006.08.21	Coded By coba
//========================================================================================
//	Modefied
//========================================================================================
#include "btiacs.h"

class TBtiReplication :public TBtiAccess
{
	private:
		int itsFileOpen;			// �t�@�C���I�[�v���X�e�[�^�X
		int itsFileOpenBk;			// �t�@�C���I�[�v���X�e�[�^�X

		TBtiAccess m_bkFile;		// �ޯ�����̧�ّ���p�̵�޼ު��

	protected:
		int SetFileName( char *FileName, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName);
		int SetDataLen(short Len);
		void SetDataPointer(void *DataPtr);

		BTI_BYTE *fileCreatePtr;
		BTI_WORD	fileCreateLen;

	public:
		int SetKey(short Keyno);				// �L�[�ԍ����Z�b�g

		TBtiReplication();
		~TBtiReplication();

		int Create(void);		// �t�@�C���̍쐬

		int Close(void);		// Btrieve Close
		int Open(void);			// Btrive Open

		int Add(void);			// ���R�[�h�̒ǉ�

		int Del(void *key);		// ���R�[�h�̍폜

		int Upd(void *key);		// ���R�[�h�̍X�V

 };

#endif
