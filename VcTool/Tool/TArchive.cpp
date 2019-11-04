//////////////////////////////////////////////////////////////////
// �N���X   ::TArchive
// ͯ�ް̧��::TArchive.h
// �@�\::̧�و��k�A�𓀂����s����
// 
// �C���|�[�g���C�u���� UNLHAVC.LIB
// �쐬::2005.01.25 ����
//////////////////////////////////////////////////////////////////

#include "TArchive.h"


//================================================================
//  �֐����Fint Freeze(char *outFile, char *arcFile)
//  �@�\�@�F̧�ق����k����
//  �����P�F�o�̓t�@�C����
//  �����Q�F���k�t�@�C����(�t�@�C�����ƃt�@�C�����̊Ԃ�1�o�C�g��߰������邱��)
//================================================================
int Freeze(char *outFile, char *arcFile)
{
	char command[1024];
	char buf[256];
	int ret;

	memset(buf,0,sizeof(buf));
	memset(command,0,sizeof(command));

	sprintf(command,"a %s %s",outFile,arcFile);
	
	ret = Unlha(NULL, command ,buf, sizeof(buf)-1);
	if(ret) return -1;

	return 0;
}
//================================================================
//  �֐����Fint Melt(char *fname, char *dest,,BOOL renew)
//  �����P�F�𓀑Ώ�̧��
//  �����Q�F�𓀐�̫���
//	�����R�F�����㏑���� TRUE : �����㏑�� FALSE:�����㏑�����Ȃ�
//================================================================
int Melt(char *fname, char *dest,BOOL renew)
{
	char command[1024];
	char buf[256];
	int ret;

	memset(buf,0,sizeof(buf));
	memset(command,0,sizeof(command));

	if(renew == TRUE) 	sprintf(command,"e %s -u0 -y1 %s\\",fname,dest);
	else sprintf(command,"e %s %s\\",fname,dest);
	
	ret = Unlha(NULL, command ,buf, sizeof(buf)-1);
	if(ret) return -1;

	return 0;
}
