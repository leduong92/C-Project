//=====================================================================================================================
//
//	FileName	:TFtpInet.h
//	Class		:TFtpInet
//				:WinInet.lib ���g����FTP����M�֐��׽
//  2006.07.05 coded by ����
//	2007.04.26 ���� ���C�u�����̃C���N���[�h�������I�ɍs��#pragma comment(lib, "wininet.lib")��ǉ�
//=====================================================================================================================
#include "TFtpInet.h"

#pragma comment(lib, "wininet.lib")
//---------------------------------------------------------------------------------------------------------------------
//	FtpInet::FtpInet(void)
//---------------------------------------------------------------------------------------------------------------------
TFtpInet::TFtpInet(void)
{
	m_FtpConnect = FALSE;
}
//---------------------------------------------------------------------------------------------------------------------
//	FtpInet::~FtpInet(void)
//---------------------------------------------------------------------------------------------------------------------
TFtpInet::~TFtpInet(void)
{
	if( m_FtpConnect == TRUE ) FtpClose();
}

//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpConnect(char *SrvAddr,char *user, char *pass)
//	�^�C�v�Fprivate
//	�@�\�@�Fwininet���g����FTP�ʐM�I�[�v��
//	�����P�G�T�[�o�[�A�h���X
//	�����Q�G���[�U�[��
//	�����R�G�p�X���[�h
//	�Ԓl�@�F����=0
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FtpConnect(char *SrvAddr,char *user, char *pass )
{
	int pasv,port;
	if(m_FtpConnect == TRUE) FtpClose();
	m_FtpConnect = FALSE;
	
	//�C���^�[�l�b�g�I�[�v��
	m_Inet.hInternet = InternetOpen("NOKFTP", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	if (m_Inet.hInternet == NULL) {
		InternetCloseHandle(m_Inet.hInternet);
		return -1;
	}
	
	//ftp Connect
	m_Inet.hSession = InternetConnect(m_Inet.hInternet,
									SrvAddr,
									INTERNET_INVALID_PORT_NUMBER, 
									user,
									pass,
									INTERNET_SERVICE_FTP,		//�g�p����T�[�r�X:FTP ���̂ق���HTTP�Ƃ����������B
									INTERNET_FLAG_PASSIVE,		// �p�b�V�u
									0);
	if (m_Inet.hSession == NULL) {
		InternetCloseHandle(m_Inet.hInternet);
		return -2;
	}
	
	m_FtpConnect = TRUE;
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpConnect()
//	�^�C�v�Fprivate
//	�@�\�@�Fwininet���N���[�Y
//	�Ԓl�@�F����=0
//---------------------------------------------------------------------------------------------------------------------
void TFtpInet::FtpClose( void )
{
	InternetCloseHandle(m_Inet.hSession);
	InternetCloseHandle(m_Inet.hInternet);
	m_FtpConnect = FALSE;
}
//---------------------------------------------------------------------------------------------------------------------
//	int TFtpInet::FindFtpFile( char *serchfile )
//	�^�C�v�Fprivate
//	�@�\�@�FFindFirst��FTP��
//	�����@�F�����Ώ�̧��
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FindFtpFile( char *serchfile )
{
	WIN32_FIND_DATA fd;
	HINTERNET hFind;
	int i;

	memset(&m_FileLst,0,sizeof(m_FileLst));

	i=0;
	hFind = FtpFindFirstFile(m_Inet.hSession, serchfile ,&fd,0,0); //�����f�B���N�g���̓J�����g�f�B���N�g���Ȃ̂� �t�@�C��������
	if(hFind != NULL) do {
		sprintf(m_FileLst.d[i].fname,"%s",fd.cFileName);
		i++;
	}while(InternetFindNextFile(hFind,&fd));
	InternetCloseHandle(hFind);

	m_FileLst.Total = i;
	if(i==0) return -1;
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpChdir( char *dir )
//	�^�C�v�Fprivate
//	�@�\�@�F�J�����g�f�B���N�g���̕ύX
//	�����P�F�ύX��f�B���N�g��
//	�Ԓl�@�F����=0
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FtpChdir( char *dir )
{
	if( m_FtpConnect == FALSE) return -1;
	
	if( !FtpSetCurrentDirectory(m_Inet.hSession, dir ) ){
		return -2;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpGet( char *downfile, char *savefile, BOOL rewrite )
//	�^�C�v�Fprivate
//	�@�\�@�FGET����
//	�����P�F�_�E�����[�h����t�@�C��
//	�����Q�F�ۑ�����t�@�C����
//	�����R�F�㏑�����邩�ǂ���
//	�Ԓl�@�F����=0
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FtpPut( char *putfile, char *savefile )
{
	if( m_FtpConnect == FALSE) return -1;
	if (!FtpPutFile( m_Inet.hSession, //FTP�̃C���^�[�l�b�g�n���h��
					putfile, //�_�E�����[�h����t�@�C��
					savefile, //�_�E�����[�h��̃p�X�t�t�@�C����
					FTP_TRANSFER_TYPE_BINARY, //�o�C�i���t�@�C���Ƃ��ă_�E�����[�h
					0 )){
		return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpGet( char *downfile, char *savefile, BOOL rewrite )
//	�^�C�v�Fprivate
//	�@�\�@�FGET����
//	�����P�F�_�E�����[�h����t�@�C��
//	�����Q�F�ۑ�����t�@�C����
//	�����R�F��M�����t�@�C�����������ꍇ�㏑�����邩�ǂ���
//	�����S�F�o�C�i�����[�h�Ŏ�M����ꍇ��TRUE,ASCII���[�h��FALSE
//	�Ԓl�@�F����=0
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FtpGet( char *downfile, char *savefile, BOOL rewrite ,BOOL binary/*=TRUE*/)
{
	int fmode;
	if( binary == TRUE) fmode = FTP_TRANSFER_TYPE_BINARY;
	else fmode = FTP_TRANSFER_TYPE_ASCII;
	
	if( m_FtpConnect == FALSE) return -1;
	if (!FtpGetFile( m_Inet.hSession, //FTP�̃C���^�[�l�b�g�n���h��
					downfile, //�_�E�����[�h����t�@�C��
					savefile, //�_�E�����[�h��̃p�X�t�t�@�C����
					rewrite, //�_�E�����[�h��ɓ����̃t�@�C��������Ƃ� TRUE:�G���[�ɂ���(�㏑���h�~) FALSE:�㏑������
					FILE_ATTRIBUTE_NORMAL, //�_�E�����[�h��ɍ����t�@�C���̃A�g���r���[�g
					fmode, //�o�C�i���t�@�C���Ƃ��ă_�E�����[�h
					0 )){
		return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpConnect( char *fname, char *chgname )
//	�^�C�v�Fprivate
//	�@�\�@�F���l�[��
//	�����P�F���t�@�C����
//	�����P�F�ύX����t�@�C����
//	�Ԓl�@�F����=0
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FtpRename( char *fname, char *chgname )
{
	if( m_FtpConnect == FALSE) return -1;
	
	if( !FtpRenameFile(m_Inet.hSession ,fname ,chgname ) ){
		return -2;
	}
	return 0;
}
