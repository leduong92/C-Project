//===================================================================================================================
//
//	FileName	: shtool.h
//				windows�V�F���֌W�̂�����Ƃ������[�e�B���e�B
//				���܂�ɂ��Â�windows�ł͓����Ȃ�����(shell�̃o�[�W����4.00(������95�Ƃ�)�́H)
//	�쐬��		:2003.07.16 by coba (�ق΃p�N��!!)
//===================================================================================================================
#include "shtool.h"

BOOL SaveShellLink( IShellLink *psl, const WCHAR *wcLinkName, BOOL bRemember );

//===================================================================================================================
//	�֐����FBOOL GetSpecialFolder( HWND hwnd,int target,char* buf,int bufsize)
//	�@�\�@�F�f�X�N�g�b�v�A�X�^�[�g���j���[�Ȃǂ̕����I�ȃp�X���擾����
//	�����P�F�E�C���h�E�n���h���iAPI��MessageBox���o���ꍇ������̂ŁA���̂Ƃ��ɐe�ɂȂ�E�C���h�E�̃n���h���j
//			�R���\�[���Ȃǂ̏ꍇ�́AHWND_DESKTOP(=NULL)���w�肷��Ƃ悢
//	�����Q�F�擾����t�H���_��ID�ASHGetFolderLocation()�̃w���v�����܂��傤
//			�悭�g�������Ȃ���
//				CSIDL_DESKTOPDIRECTORY	հ�ޕʂ̃f�X�N�g�b�v
//				CSIDL_COMMON_DESKTOPDIRECTORY	�Sհ�ދ��ʂ̃f�X�N�g�b�v
//				CSIDL_SARTMENU	հ�ޕʂ̃X�^�[�g���j���[
//				CSIDL_COMMAN_SARTMENU	�Sհ�ދ��ʂ̃X�^�[�g���j���[
//	�����R�F�擾�����p�X���i�[����o�b�t�@�BMAX_PATH+1�͊m�ۂ��܂��傤�B
//	�����S�F���̃T�C�Y
//===================================================================================================================
BOOL GetSpecialFolder( HWND hwnd,int target,char* buf,int bufsize)
{
	HRESULT hres;
	LPITEMIDLIST pid;
	char path[MAX_PATH+1];
	BOOL ret=FALSE;

	// shell-api
	hres= SHGetSpecialFolderLocation(NULL,target,&pid);

	if(SUCCEEDED(hres)){
		if( TRUE==SHGetPathFromIDList(pid,path)){
			memset( buf, 0, bufsize );
			memcpy( buf, path, (int)strlen(path)+1 > bufsize ? bufsize-1:strlen(path)+1);
			ret = TRUE;
		}
	}

	CoTaskMemFree(pid);			// �J��

	return(ret);
}

//===================================================================================================================
//	�֐����FBOOL CreateShellLink( const char *LinkFile, const char *TargetFile, 
//											const char *Arguments, const char *WorkDir, int ShowCmd )
//	�@�\�@�F�V���[�g�J�b�g�t�@�C�����쐬����i�p�N���j
//		�����̓��e�́A�V���[�g�J�b�g�̃v���p�e�B������Β����I�ɗ����ł���Ǝv���܂�
//	�����P�F�V���[�g�J�b�g�t�@�C���̖��O(*.lnk �����t�^���Ă����Ȃ��ƁA�V���[�g�J�b�g�ɂȂ��Ă���Ȃ�)
//	�����Q�F�����N��t�@�C���̃t���p�X�B
//	�����R�F�R�}���h���C������
//	�����S�F��ƃf�B���N�g���B�����N��t�@�C���̃t�H���_���w�肷��̂���ʓI�ł��B
//	�����T�F���s���̑傫���BShowWindwo()�̈������g���܂�
//	�߂�l�FTRUE or FALSE
//===================================================================================================================
BOOL CreateShellLink( const char *LinkFile, const char *TargetFile, const char *Arguments, const char *WorkDir, int ShowCmd )
{
	IShellLink *psl;
	WCHAR       wcLinkFile[MAX_PATH+1];

	CoInitialize( NULL );
	if ( S_OK == CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl ) )
	{
		psl->SetPath( TargetFile );                           // �����N���ݒ�
		if ( Arguments ) psl->SetArguments( Arguments );      // �R�}���h���C��������ݒ�
		if ( WorkDir   ) psl->SetWorkingDirectory( WorkDir ); // ��ƃt�H���_��ݒ�
		psl->SetShowCmd( ShowCmd );                           // ���s���̑傫����ݒ�

		MultiByteToWideChar( CP_ACP, 0, LinkFile, -1, wcLinkFile, MAX_PATH ); 
		if ( SaveShellLink( psl, wcLinkFile, TRUE ) ) // �V���[�g�J�b�g�̕ۑ�
		{
			psl->Release();
			CoUninitialize();
			return TRUE; // ����
		}
		psl->Release();
	}
	CoUninitialize();
	return FALSE; // ���s
}//CreateShellLink

//*********************************************************
// SaveShellLink
//*********************************************************
BOOL SaveShellLink( IShellLink *psl, const WCHAR *wcLinkName, BOOL bRemember )
{
	HRESULT       hResult;
	IPersistFile *ppf;

	if ( S_OK != psl->QueryInterface( IID_IPersistFile, (void **)&ppf ) )
		return FALSE;

	hResult = ppf->Save( wcLinkName, bRemember );
	ppf->Release();
	return S_OK == hResult;
}//SaveShellLink�֘A

