#ifndef TARCHIVE_H
#define TARCHIVE_H
//////////////////////////////////////////////////////////////////
// �N���X   ::TArchive
// ͯ�ް̧��::TArchive.h
// �@�\::̧�و��k�A�𓀂����s����
// 
// �C���|�[�g���C�u���� UNLHAVC.LIB
// �쐬::2005.01.25 ����
//////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <time.h>
#include "unlha32.h"
//�C���|�[�g���C�u������UNLHAVC.LIB�I�I

		int Freeze(char *outFile ,char *arcFile);	//���k
		int Melt(char *fname, char *dest,BOOL renew);			//��
#ifdef __cplusplus
}
#endif

#endif
