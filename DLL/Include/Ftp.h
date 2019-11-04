//	*****************************************
//	FTP.h
//
//		FTP Class
// ========================================================================
//        FTP�v���g�R���𗘗p�����t�@�C���]���N���C���A���g�N���X
//        �t�@�C�����FFTP.cpp
//
//        �쐬�ҁF�Γc���O�i�m�n�j������ЋZ�p�{�����Y�Z�p���j
//        �쐬���F2003�N5��24��
//�@�@�@�@�X�V�����F
//
//     ���̑��FWinSock 2���g�p���Ă��邽�߂��������������X�T
//�@�@�@�@�@�@�@�@�ł́AWINsock�Q�D�O�̓������K�v
//     �K�v���C�u�����[
// Compile and link with ws2_32.lib
//                include file is <winsock2,h>
// Dynamically links to WS2_32.DLL
//
// ========================================================================
//		SuoerClass = None
//	
//		Copyright  2003 Nok Corporation.
//			All rights reserved.
//			Coded by M.Ishida
//	*****************************************
#ifndef FTP_H
#define FTP_H

//#define WIN32_WINNT 0x0400
//#include <windows.h>
#include <winsock2.h>

class FTP
{
    int  m_PortNo;
	char m_IPAdress[20];
	char m_Host[100];
	char m_User[100];
	char m_Password[100];

	long fRcvPacketNo,fSendPaketNo;
	SOCKET fSocketC,fSocketT;
	struct sockaddr_in	fAddrC;		//�z�X�g�̃R���g���[���p�A�h���X
	struct sockaddr_in  fAddrT;		//�z�X�g�̃t�@�C���]���p�A�h���X�B
	struct sockaddr_in	fCtrlAddr;	//���[�J���R���g���[���p�A�h���X
    WSAEVENT fEvent;
	int ftpRetCode;
	int fStatus;
	int bConnected;
	char pbuf[300];
	BOOL m_printMode;	//��ʕ\���̗L��

public:

	FTP();
	~FTP();

	char m_log[1024];		// log�o�b�t�@
	char m_Resp[1024];	// FTP���ް�̉������b�Z�[�W
	int  resCode;
	

	virtual BOOL	IFTP(void);
	virtual int	CheckStatus(void);
	virtual int SetUser(char *user);
	virtual int SetPassword(char *password);
	virtual int SetServerIP(char *serveip);
	virtual int SetHostName(char *hostname);
	virtual void SetPortNo(int port);

	virtual int Connect();

	virtual int CommandSend(char *message,int *replyNo,char *reply);
	virtual int Recv(char *answer);

	virtual int SendFileWrite(char *fnCliant, char *fnServer,int code);
	virtual int SendFileAppend(char *fnCliant, char *fnServer,int code);
	virtual int SendFileUnique(char *fnCliant, char *fnServer,int code);
	virtual int SendFile(char *fnCliant, char *fnServer,int mode,int code);
	virtual int RecvFile(char *fnCliant, char *fnServer);
	
//�f�B���N�g���[�E�t�@�C������֌W
	virtual int CmdCwd(LPSTR path);// CWD �J�����g�f�B���N�g���̕ύX�Ƃ���B
	virtual int CmdCdup();	// CDUP 1��ʂ̃f�B���N�g��(�e�f�B���N�g��)�ɖ߂�B
	virtual int CmdMkd(LPSTR path);	// MKD �f�B���N�g���̍쐬����B
	virtual int CmdRmd(LPSTR path);// RMD �f�B���N�g�����폜����B
	virtual int CmdPwd();	// PWD ���݂̃��[�L���O�f�B���N�g����\������B
	virtual int CmdDele(LPSTR path);// DELE �t�@�C���� �w�肵���t�@�C�����폜����B
	virtual int CmdRename(LPSTR path1,LPSTR path2);
	virtual int CmdList();	// [�t�@�C���^�f�B���N�g����] [�I�v�V����]
	virtual int CmdNList();// [�t�@�C���^�f�B���N�g����] [�I�v�V����]



//�n�r�֌W
	virtual int CmdSyst();	// SYST �V�X�e������\������B
	virtual int CmdNOOP();	// NOOP �������Ȃ��B�T�[�o�̉ғ����m�F���邽�߂Ɏ��s�����B��ɐ���I������B
	virtual int CmdQuit();	// QUIT ���O�A�E�g����B
	virtual int CmdAllc();	// ALLO �t�@�C���o�C�g�� [R ���R�[�h/�y�[�W�o�C�g��] ���炩���߃T�[�o�[��Ŏw�肵���o�C�g���̈���m�ۂ���B���R�[�h/�y�[�W�ł��w�肵�Ȃ��Ă͂Ȃ�Ȃ��ꍇ�́A�I�v�V�����Ŏw��ł���i�K�v���ǂ�����OS�Ɉˑ�����j�B
	virtual int CmdAbor();	// ABOR  ���ݎ��s���̃f�[�^�]�������ׂĒ��~����B�f�[�^�R�l�N�V�����͔j�������B
	virtual int CmdSite(LPSTR cmd);	// SITE �R�}���h �C�ӂ�OS�R�}���h�����s����B
	virtual int CmdStat(LPSTR path);// STAT [�t�@�C���^�f�B���N�g����] ���݂̃V�X�e����]����Ԃ̏���\������B
	virtual int CmdRCmd(LPSTR cmd);	// AS400�̃v���O���������s����B


//�]��Ӱ�ފ֌W
	virtual int CmdTypeASCII();	// TYPE A�FASCII �]���f�[�^�̌`�����w�肷��B
	virtual int CmdTypeImage();	// TYPE I�FImage�i�o�C�i���j �]���f�[�^�̌`�����w�肷��B
	virtual int CmdTypeEBOCDIC();	// TYPE E�FEBCDIC �]���f�[�^�̌`�����w�肷��B
	virtual int CmdTypeSJIS();	// QUOTE TYPE B 1�FSJIS �]���f�[�^�̌`�����w�肷��B
	virtual int CmdStruFile();	// STRU �t�@�C���\���I�v�V���� �]������t�@�C���̃t�@�C���\�����I�v�V�����Ŏ����BF�F�t�@�C���\���i�o�C�g�u���̒ʏ�̃o�C�i���j�@�f�t�H���g�B
	virtual int CmdStruRecord();	// STRU �t�@�C���\���I�v�V���� �]������t�@�C���̃t�@�C���\�����I�v�V�����Ŏ����BR�F���R�[�h�\���i�V�[�P���V�����Ȃǂ̃��R�[�h�C���[�W���������f�[�^�j
	virtual int CmdStruPage();	// STRU �t�@�C���\���I�v�V���� �]������t�@�C���̃t�@�C���\�����I�v�V�����Ŏ����BP�F�y�[�W�\���i�C���f�b�N�X�����ꂽ�y�[�W�u���̃t�@�C���j
	virtual int CmdModeStream();	// MODE �]�����[�h�I�v�V����  S�F�X�g���[���i�o�C�g�X�g���[���ɂ��]���j�f�t�H���g
	virtual int CmdModeBlock();	// MODE �]�����[�h�I�v�V����  B�F�u���b�N�i�]�����u���b�N���Ƃɍs���j
	virtual int CmdModeCompression();// MODE �]�����[�h�I�v�V����  C�F���k�iRun-Length��@�ɂ�鈳�k�j

//�]�����ߊ֌W
	virtual int CmdRetr(LPSTR path)	;// RETR �t�@�C���� �w�肵���t�@�C���̓��e���T�[�o����擾����B
	virtual int CmdStor(LPSTR path) ;// STOR �t�@�C���� �w�肵���t�@�C�����ŁA�T�[�o�֑��M����f�[�^�Ńt�@�C�����쐬����B���ꖼ�̃t�@�C�������łɂ���ꍇ�ɂ́A�㏑������B�t�@�C�������݂��Ă��Ȃ��ꍇ�́A�V�K�쐬����B
	virtual int CmdStou(LPSTR path)	;// STOU STOR�Ɠ��l�Ɏw�肵���t�@�C�����쐬���邪�A���łɓ��ꖼ�̃t�@�C�����������ꍇ�ɂ́A�d�Ȃ�Ȃ��悤�ȃt�@�C�����������I�ɕt���č쐬����B
	virtual int CmdAppe(LPSTR path)	;// APPE �t�@�C���� �T�[�o�֑��M����f�[�^���w�肵���t�@�C���֒ǉ�����B
	virtual int CmdPasv(void)		;//PASV �p�b�V�u���[�h�ւ̈ڍs���w������B�T�[�o�̓f�[�^�R�l�N�V�����Ŏg�p����IP�A�h���X�ƃ|�[�g�ԍ���\������B

	virtual int DoCommand(int CmdNo);
	virtual int DoCommand(int CmdNo,LPSTR path);
	virtual int DoCommand(int CmdNo,LPSTR path,char* buf);

//	virtual int DoRETR(LPSTR path);
//	virtual int DoSTOR(LPSTR path);
//	virtual int DoLIST(void);
//	virtual int DoCWD(LPSTR path);
	virtual int DoPASV(void);
	virtual int Print(char *buf);

	void SetPrintMode( BOOL mode ){ m_printMode = mode;};

protected:

};
#endif
