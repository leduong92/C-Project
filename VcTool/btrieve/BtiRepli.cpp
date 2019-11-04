//========================================================================================
//	Btrieve.cpp�̃t�@�C������@�\�N���X(�g����:�t�@�C���~���[�����O�@�\�t��)
//	�t�@�C������N���X���쐬����Ƃ��͂�����p�����ĉ������I
//	FileName	:BtiRepli.cpp
//	Class		:TBtiReplication
//	�쐬��		:2006.08.21	Coded By coba
//========================================================================================
#include <stdio.h>
#include "btirepli.h"

//========================================================================================
// TBtiReplication::TBtiReplication()
//	�R���X�g���N�^
//	�����F�Ȃ�
//	�Ԓl�F�Ȃ�
//========================================================================================
TBtiReplication::TBtiReplication()
{
	itsFileOpen=1;
	itsFileOpenBk=1;
}

//========================================================================================
// TBtiReplication::TBtiReplication()
//	�f�X�g���N�^
//	�����F�Ȃ�
//	�Ԓl�F�Ȃ�
//========================================================================================
TBtiReplication::~TBtiReplication()
{
}

//========================================================================================
//	int TBtiReplication::SetFileName( char *FileName, char *ProfileName
//					, LPCTSTR SectionName ,LPCTSTR KeyName)
// �@�\�F�t�@�C�����̃Z�b�g�B���ع���ݗp��̧�ٖ��́A�����Ɠ���ini�t�@�C������p�X���擾
//		  �A���A�Z�N�V��������[BTIBACK]����擾����B
// �����P�Fbtrievȩ�ٖ�
// �����Q�F�p�X���擾����ini̧�ٖ�
// �����R�F�p�X���擾����ini̧�ٓ��̃Z�N�V������
// �����S�F�p�X���擾����ini̧�ٓ��̃L�[��
//========================================================================================
int TBtiReplication::SetFileName( char *FileName, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	// �t�@�C���{��
	Btrieve::SetFileName( FileName,ProfileName,SectionName,KeyName);

	// �ޯ�����̧��
	m_bkFile.SetFileName( FileName,ProfileName,"BTIBACK",KeyName);
	return(0);
}
//========================================================================================
// int TBtiReplication::SetDataLen(short Len)
// �@�\�@�F���R�[�h���̃Z�b�g�B�{�́����ع����̧�ق̗����ɐݒ肷��
// �����P�Fbti̧�ق̃��R�[�h��
//========================================================================================
int TBtiReplication::SetDataLen(short Len)
{
	// �t�@�C���{��
	TBtiAccess::SetDataLen(Len);
	// �ޯ�����̧��
	m_bkFile.SetDataLen(Len);
	return(0);
}

//========================================================================================
// int TBtiReplication::SetKey(short Keyno)
// �@�\�@�F���삷�鎞�̃L�[�ԍ����Z�b�g�B�{�́����ع����̧�ق̗����ɐݒ肷��
//========================================================================================
int TBtiReplication::SetKey(short Keyno)
{
	// �t�@�C���{��
	Btrieve::SetKey(Keyno);
	// �ޯ�����̧��
	m_bkFile.SetKey(Keyno);
	return(0);
}
//========================================================================================
//	void TBtiReplication::SetDataPointer(void *DataPtr);
//	�f�[�^���R�[�h�i�[�|�C���^�����肷��B�{�́����ع����̧�ق̗����ɐݒ�B
//	�����F���ۂɃf�[�^�����i�[����|�C���^(SubClass�̃f�[�^�\���̕��̃|�C���^)
//	�Ԓl�F�Ȃ�
//========================================================================================
void TBtiReplication::SetDataPointer(void *DataPtr)
{
	// �t�@�C���{��
	TBtiAccess::SetDataPointer(DataPtr);	//itsDataPtr=(char*)DataPtr;
	// �ޯ�����̧��
	m_bkFile.SetDataPointer(DataPtr);
}

//========================================================================================
// int TBtiReplication::Create(void)
// �@�\�@�F���bti̧�ق��쐬����BϽ��̍쐬�ɐ��������Ƃ��̂����ع����̧�ٍ쐬����
//========================================================================================
int TBtiReplication::Create(void)
{
	int ret;

	// ̧�ٖ{�̂��쐬
	Btrieve::fileCreatePtr = this->fileCreatePtr;
	Btrieve::fileCreateLen = this->fileCreateLen;

	ret=Btrieve::Create();

	// �{��(Ͻ�)�̍쐬�ɐ���������A���ع����(�ڰ��)���쐬
	if(!ret){
		m_bkFile.SetCreateSpec(this->fileCreatePtr,this->fileCreateLen);
		m_bkFile.Create();
	}

	return(ret);

}

//========================================================================================
//	int TBtiReplication::Open(void)
//	Btrieve �t�@�C���I�|�v���B�{�́����ع����̧�ق̗������J��
//	Btrieve�����o�֐��̃I�[�o�[���C�h
//�@�����F�Ȃ�
//�@�Ԓl�Fbtr��error�R�[�h
//========================================================================================
int TBtiReplication::Open(void)
{
	// �t�@�C���{��
	itsFileOpen=TBtiAccess::Open();//	itsFileOpen=Btrieve::Open();

	// �ޯ�����̧��(�{�̂�OK�̏ꍇ�̂�)
	if(itsFileOpen==0)	itsFileOpenBk=m_bkFile.Open();

	return(itsFileOpen);
}


//========================================================================================
//	int TBtiReplication::Close(void)
//	Btrieve �t�@�C���I�|�v��
//	Btrieve�����o�֐��̃I�[�o�[���C�h
//�@�����F�Ȃ�
//�@�Ԓl�Fbtr��error�R�[�h
//========================================================================================
int TBtiReplication::Close(void)
{
	if(itsFileOpen==0){
		TBtiAccess::Close();//		Btrieve::Close();
		itsFileOpen=1;
	}

	// �ޯ�����̧��
	if(itsFileOpenBk==0){
		m_bkFile.Close();
		itsFileOpenBk=1;
	}
	return(0);
}

//========================================================================================
// int TBtiReplication::Add()
//	���R�|�h�̑}�����s��
//
//�@ �����F�Ȃ�
//�@ �Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiReplication::Add()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = TBtiAccess::Add();//	theFstat = Insert(itsDataPtr);

	// �{�̂������������ޯ����߂̂ق����X�V����
	if( !theFstat && !itsFileOpenBk) m_bkFile.Add();

	return(theFstat);
}

//========================================================================================
// int TBtiReplication::Del(void *key)
//	���R�|�h�̍폜���s��
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����F�폜���郌�R�[�h�̃L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiReplication::Del(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = TBtiAccess::Del( (char*)key);	//	theFstat = Delete( (char*)key, itsDataPtr);

	// �{�̂������������ޯ����߂̂ق����X�V����
	if( !theFstat && !itsFileOpenBk) m_bkFile.Del((char*)key);

#ifdef TEST
	printf( "Delete Status = %d \n",theFstat);

#endif

	return(theFstat);
}


//========================================================================================
// int TBtiReplication::Upd(void *key)
//	���R�|�h�̍X�V���s��
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����Fchar* key	�ύX�O�L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiReplication::Upd(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = TBtiAccess::Upd( (char*)key);//	theFstat = Update( (char*)key, itsDataPtr);

	// �{�̂������������ޯ����߂̂ق����X�V����
	if( !theFstat && !itsFileOpenBk) m_bkFile.Upd((char*)key);

#ifdef TEST
	printf( "Update Status = %d \n",theFstat);

#endif

	return(theFstat);
}

