//#define TEST
//========================================================================================
//
//	Copyright 1996-1996 NOK Cororation All Rights Reserved
//	FileName	:BTIACS.CPP
//	Class		:TBtiAccess ( SubClass of "Btrieve" )
//				:Btrieve.cpp�̃t�@�C������@�\�g���@�\
//	�쐬��		:1996.10.10	Coded By Koba
//	Modefied	:�����o�֐�Last(),GT(),LT()��ǉ� By Koba	1996.10.10
//
//========================================================================================
#include <stdio.h>
#include "btiacs.h"

//#include "noktoolw.h"
//========================================================================================
// TBtiAccess::TBtiAccess()
//	�R���X�g���N�^
//	�����F�Ȃ�
//	�Ԓl�F�Ȃ�
//========================================================================================
TBtiAccess::TBtiAccess()
{
	itsFileOpen=1;
}

//========================================================================================
// TBtiAccess::TBtiAccess()
//	�f�X�g���N�^
//	�����F�Ȃ�
//	�Ԓl�F�Ȃ�
//========================================================================================
TBtiAccess::~TBtiAccess()
{
}

//========================================================================================
//	void TBtiAccess::SetDataPointer(void *DataPtr);
//	�f�[�^���R�[�h�i�[�|�C���^�����肷��B
//	�����F���ۂɃf�[�^�����i�[����|�C���^(SubClass�̃f�[�^�\���̕��̃|�C���^)
//	�Ԓl�F�Ȃ�
//========================================================================================
void TBtiAccess::SetDataPointer(void *DataPtr)
{
	itsDataPtr=(char*)DataPtr;
}

//========================================================================================
//	void TBtiAccess::SetKeyPointer(void *KeyPtr);
//	�f�[�^���R�[�h�i�[�|�C���^�����肷��B
//	�����F���ۂɃf�[�^�����i�[����|�C���^(SubClass�̃f�[�^�\���̕��̃|�C���^)
//	�Ԓl�F�Ȃ�
//========================================================================================
void TBtiAccess::SetKeyPointer(void *KeyPtr)
{
	itsKeyPtr=(char*)KeyPtr;
}

//========================================================================================
//	int TBtiAccess::Open(void)
//	Btrieve �t�@�C���I�|�v��
//	Btrieve�����o�֐��̃I�[�o�[���C�h
//�@�����F�Ȃ�
//�@�Ԓl�Fbtr��error�R�[�h
//========================================================================================
int TBtiAccess::Open(void)
{
	itsFileOpen=Btrieve::Open();

	return(itsFileOpen);
}


//========================================================================================
//	int TBtiAccess::Close(void)
//	Btrieve �t�@�C���I�|�v��
//	Btrieve�����o�֐��̃I�[�o�[���C�h
//�@�����F�Ȃ�
//�@�Ԓl�Fbtr��error�R�[�h
//========================================================================================
int TBtiAccess::Close(void)
{
	if(itsFileOpen==0){
		Btrieve::Close();
		itsFileOpen=1;
	}
	return(0);
}

//========================================================================================
// int TBtiAccess::Add()
//	���R�|�h�̑}�����s��
//
//�@ �����F�Ȃ�
//�@ �Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::Add()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = Insert(itsDataPtr);

//	WinPrintf( "AddR Status = %d \n",theFstat);
#ifdef TEST
#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::Del(void *key)
//	���R�|�h�̍폜���s��
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����F�폜���郌�R�[�h�̃L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::Del(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = Delete( (char*)key, itsDataPtr);

#ifdef TEST
	printf( "Delete Status = %d \n",theFstat);

#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::Del(void)
//	���R�|�h�̍폜���s��
//		�i�����s����O�ɂ́ASubClass��SetKey?Data(,,,)�����s���Ă��Ȃ���
//			 ���퓮��͊��҂ł��Ȃ��B�j
//�@�����F�Ȃ�
//	���́F�L�[�f�[�^(itsKeyPtr)
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::Del(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = Delete( itsKeyPtr, itsDataPtr);

#ifdef TEST
	printf( "Delete Status = %d \n",theFstat);

#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::Upd(void *key)
//	���R�|�h�̍X�V���s��
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����Fchar* key	�ύX�O�L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::Upd(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = Update( (char*)key, itsDataPtr);

#ifdef TEST
	printf( "Update Status = %d \n",theFstat);

#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::Upd(void)
//	���R�|�h�̍X�V���s��
//		�i�����s����O�ɂ́ASubClass��SetKey?Data(,,,)�����s���Ă��Ȃ���
//			 ���퓮��͊��҂ł��Ȃ��B�j
//�@�����F�Ȃ�
//	���́F�L�[�f�[�^(itsKeyPtr)
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::Upd(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = Update( itsKeyPtr, itsDataPtr);

#ifdef TEST
	printf( "Update Status = %d \n",theFstat);

#endif

	return(theFstat);
}

//========================================================================================
// short TBtiAccess::First()
//	�ŏ��̃��R�|�h��ǂݍ���
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����F�Ȃ�
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::First()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);
	theFstat= GetFirst(itsDataPtr);
#ifdef TEST
	printf( "Read First Status = %d \n",theFstat);
#endif
	return(theFstat);
}


//========================================================================================
// short TBtiAccess::Last()
//	�Ō�̃��R�|�h��ǂݍ���
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����F�Ȃ�
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::Last()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);
	theFstat= GetLast(itsDataPtr);
#ifdef TEST
	printf( "Read Last Status = %d \n",theFstat);
#endif
	return(theFstat);
}


//========================================================================================
// int TBtiAccess::Find(void *key)
//	�w�肵���L�[�̃��R�|�h��ǂݍ���
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����Fchar* key	�擾���������R�[�h�̃L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::Find(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat= GetEqual(itsDataPtr, (char*)key);
#ifdef TEST
	printf( "Read Find Status = %d \n",theFstat);
#endif
	// ���I�[�v�����͍ēx�J���ăg���C
	if( theFstat == 3 ){
		theFstat = Open();
		if( !theFstat ){
			theFstat =Btrieve::Open();
			theFstat= GetEqual(itsDataPtr, (char*)key);
		}
	}
	return(theFstat);
}


//========================================================================================
// int TBtiAccess::Find(void)
//	�w�肵���L�[�̃��R�|�h��ǂݍ���
//		�i�����s����O�ɂ́ASubClass��SetKey?Data(,,,)�����s���Ă��Ȃ���
//			 ���퓮��͊��҂ł��Ȃ��B�j
//�@�����F�Ȃ�
//	���́F�L�[�f�[�^(itsKeyPtr)
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::Find(void)
{
	int theFstat;



	if(itsFileOpen != 0 ) return(-1);

	
	theFstat= GetEqual(itsDataPtr, itsKeyPtr);
#ifdef TEST
	printf( "Read Find Status = %d \n",theFstat);
#endif
	return(theFstat);
}


//========================================================================================
// int TBtiAccess::Next()
//	���̃��R�|�h��ǂݍ���
//
//�@�����F�Ȃ�
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::Next()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetNext(itsDataPtr);

#ifdef TEST
	printf( "Read Next Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::Prev()
//	 �O�̃��R�|�h��ǂݍ���
//
//�@�����F
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::Prev()
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetPrev(itsDataPtr);

#ifdef TEST
	printf( "Read Prev Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::GE(void *key)
//	�w��L�|�ȏ�̃��R�|�h��ǂݍ���
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����F�����ƂȂ�L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::GE(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetGE(itsDataPtr,(char*)key);

#ifdef TEST
	printf( "Read GE Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::GE(void)
//	�w��L�|�ȏ�̃��R�|�h��ǂݍ���
//		�i�����s����O�ɂ́ASubClass��SetKey?Data(,,,)�����s���Ă��Ȃ���
//			 ���퓮��͊��҂ł��Ȃ��B�j
//�@�����F�Ȃ�
//	���́F�L�[�f�[�^(itsKeyPtr)
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::GE(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetGE(itsDataPtr,itsKeyPtr);

#ifdef TEST
	printf( "Read GE Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::LE(void *key)
//	�w��L�[�ȉ��̃��R�|�h��ǂݍ���
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����F�����ƂȂ�L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::LE(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetLE(itsDataPtr,(char*)key);

#ifdef TEST
	printf( "Read LE Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::LE(void)
//	�w��L�[�ȉ��̃��R�|�h��ǂݍ���
//		�i�����s����O�ɂ́ASubClass��SetKey?Data(,,,)�����s���Ă��Ȃ���
//			 ���퓮��͊��҂ł��Ȃ��B�j
//�@�����F�Ȃ�
//	���́F�L�[�f�[�^(itsKeyPtr)
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::LE(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetLE(itsDataPtr,itsKeyPtr);

#ifdef TEST
	printf( "Read LE Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::GT(void *key)
//	�w��L�|���傫�����R�|�h��ǂݍ���
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����F�����ƂȂ�L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::GT(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetGT(itsDataPtr,(char*)key);

#ifdef TEST
	printf( "Read GT Status = %d \n",theFstat);
#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::GT(void)
//	�w��L�|���傫�����R�|�h��ǂݍ���
//		�i�����s����O�ɂ́ASubClass��SetKey?Data(,,,)�����s���Ă��Ȃ���
//			 ���퓮��͊��҂ł��Ȃ��B�j
//�@�����F�Ȃ�
//	���́F�L�[�f�[�^(itsKeyPtr)
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::GT(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetGT(itsDataPtr,itsKeyPtr);

#ifdef TEST
	printf( "Read GT Status = %d \n",theFstat);
#endif

	return(theFstat);
}


//========================================================================================
// int TBtiAccess::LT(void *key)
//	�w��L�[��菬�������R�|�h��ǂݍ���
//		�i�����s����O��Btrieve::SetKey(short keyno)�����s���A
//			 �L�[�ԍ����w������Ă������ƁB�j
//�@�����F�����ƂȂ�L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::LT(void *key)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetLT(itsDataPtr,(char*)key);

#ifdef TEST
	printf( "Read LT Status = %d \n",theFstat);
#endif

	return(theFstat);
}

//========================================================================================
// int TBtiAccess::LT(void)
//	�w��L�[��菬�������R�|�h��ǂݍ���
//		�i�����s����O�ɂ́ASubClass��SetKey?Data(,,,)�����s���Ă��Ȃ���
//			 ���퓮��͊��҂ł��Ȃ��B�j
//�@�����F�Ȃ�
//	���́F�L�[�f�[�^(itsKeyPtr)
//�@�Ԓl�FBtrieve Error Code
//========================================================================================
int TBtiAccess::LT(void)
{
	int theFstat;

	if(itsFileOpen != 0 ) return(-1);

	theFstat = GetLT(itsDataPtr,itsKeyPtr);

#ifdef TEST
	printf( "Read LT Status = %d \n",theFstat);
#endif

	return(theFstat);
}
