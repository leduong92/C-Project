#ifndef NOK_WINSHELLTOLL_H
#define NOK_WINSHELLTOLL_H
//===================================================================================================================
//
//	FileName	: shtool.h
//				windows�V�F���֌W�̂�����Ƃ������[�e�B���e�B
//				���܂�ɂ��Â�windows�ł͓����Ȃ�����(shell�̃o�[�W����4.00(������95�Ƃ�)�́H)
//	�쐬��		:2003.07.16 by coba 
//===================================================================================================================
//	�C������
//===================================================================================================================
#ifdef __cplusplus
extern "C"
{
#endif
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

// �f�X�N�g�b�v�A�X�^�[�g���j���[�Ȃǂ̓���t�H���_�̃p�X���擾
BOOL GetSpecialFolder( HWND hwnd,int target,char* buf,int bufsize);

// �V���[�g�J�b�g�̍쐬
BOOL CreateShellLink( const char *LinkFile, const char *TargetFile, const char *Arguments, const char *WorkDir, int ShowCmd );

#ifdef __cplusplus
}
#endif

#endif
