#ifndef TCPServer_H
#define TCPServer_H
////=====================================================================================================================
//
//	FileName:TcpServer.h
//	Class	:TcpServer
//			:�\�P�b�g�ʐM�N���X
//	�쐬��	:2002.07.22	  TAKE
//=====================================================================================================================
//	�X�V��
//	�X�V���@ 2004.04.05 TAKE�@MAX_SENDLENGTH,MAX_RECVLENGTH�̒l��4096�ɃA�b�v�i�ʐM�g���u���Ή��j
//	2005.04.14 coba �҂��L���[�̐����A5����1�ɂւ炵��
//	2005.07.11 coba ���̃\�[�X�ƍ��̂̂��߁A�������֐��𑝂₵��
//	2005.12.01 coba Wait()�Ɉ�����t���A�҂����Ԃ��w��ł���悤�ɕύX
//	2006.03.16 coba Send()�ł̕ϐ��������Y����C��
//	2008.04.24 coba Send()��ɂ����瑤��closesocket()���Ă��o�O���C��
//					�_�C���N�g����M�֐���ǉ�
//	2008.04.25 coba Send()�֐�����ASend()�ŁA������Close��҂��Ă��炱����̃\�P�b�g�����
//	2008.05.08 coba ����݊��̂��߁ASend()�֐��ŁAclose�҂��̕b���������Ŏw�肷��悤�ɂ���
//	2008.05.09 ���� MAX_SENDLEN�AMAX_RECVLEN��4096��8192�ɕύX
//  2008.05.29 Maeda PortNo�`�F�b�N�̃p�����[�^�ǉ�
//	2008.07.26 coba ReceiveDirect()�ŁA������M�ɑΉ����Ă��Ȃ������̂��C��
//	2008.11.11 matuda get to be enable client ip address
//	2013.09.24 coba �ő呗��M�T�C�Y��8192=>20480�ɕύX
//	2014.05.22 coba �o�C�i����M�ɑΉ�:Recv()x2
//=====================================================================================================================

//#include <winsock.h>                    /* for socket API           */
#include <winsock2.h>


#define DEF_TRANSPORT_NO	26578               /* �]���߰Ĕԍ�       */

// 2004.04.28 TAKE
//#define MAX_SENDLENGTH	4096
//#define MAX_RECVLENGTH	4096
// 2008.05.09 ����
// #define MAX_SENDLENGTH	8192
// #define MAX_RECVLENGTH	8192
// 2013.09.24 coba
#define MAX_SENDLENGTH	8192//20480
#define MAX_RECVLENGTH	8192//20480

#define SHORT_SENDLENGTH	254
#define SHORT_RECVLENGTH	254

class TcpServer
{

	char	fPcName[30];
	char	fProgramName[30];
	int	 	fCommand;
	long 	fRcvPacketNo,fSendPaketNo;
//	SOCKET 	fSocket;
	int  	m_PortNo;
	char 	m_IPAdress[20];
	char 	m_ClientIp[20];	//client ip

//    WSAEVENT fEvent;
	int m_Status;
  	struct sockaddr_in  m_AddrAcpt;
  	SOCKET              m_FdAcpt;
  	struct sockaddr_in  m_Addr;
  	SOCKET              m_Fd;
  	WSADATA             WSAData;
    WSAEVENT			m_Event;

  	struct timeval      time;
  	fd_set              r_socket, w_socket, e_socket;

public:

			 TcpServer(int port);
			 ~TcpServer();

	// 2005.07.11
	 TcpServer();
	int Initialize(int PortNo = DEF_TRANSPORT_NO,int PortCheck=TRUE);
	int Send( char *message,int sendlen,DWORD close_wait);	// 08.05.08 close_wait��ǉ�
	int Recv( char *msg,int len);
	// ---2005.07.11

	virtual BOOL ITcpServer(void);
	virtual int CheckStatus(void);

//	virtual int Wait();
	virtual int Send( char *message);
	virtual int Recv( char *msg);
    virtual int SetPortNo(int PortNo);
	virtual int SetIPAdress(char *IPAdress);
	virtual int SetPcName(char *PcName);
	virtual int SetProgramName(char *ProgramName);
	int Wait( DWORD timeout=5000);		// �^�C���A�E�g���w��ł���悤�ɂ���

	// 2008.04.24 �ǉ�
	int SendDirect( char *message,int sendlen,DWORD close_wait);
	int RecvDirect( char *msg,int len,int time_out=1);	//�^�C���A�E�g���w��ł���悤�ύX 2010.06.08 matsuda
	void GetClientIp( char *ip );		// 2008.11.11 matuda
protected:

};
#endif
