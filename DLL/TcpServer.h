#ifndef TCPServer_H
#define TCPServer_H

////=====================================================================================================================
//
//	FileName:TcpServer.h
//	Class	:TcpServer
//=====================================================================================================================

//#include <winsock.h>                    /* for socket API           */
#include <winsock2.h>



//Include library into project 
//	There is 2 ways: 
//				-1- Write code: #pragma comment(lib, "ws2_32.lib") ...
//				-2- Project menu -> Settings...-> Link tab -> Object/library modules: wsock32.lib ws2_32.lib libpqdll.lib
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")
//#pragma comment(lib, "libpqdll.lib")	//the first: add libpq.dll file to debug folder


#pragma comment(lib, "QR2DLL.lib")




#define DEF_TRANSPORT_NO	26578

//#define MAX_SENDLENGTH	4096
//#define MAX_RECVLENGTH	4096
// 2008.05.09 ÏÂÀÞ
#define MAX_SENDLENGTH	8192
#define MAX_RECVLENGTH	8192

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

	
	 TcpServer();
	int Initialize(int PortNo = DEF_TRANSPORT_NO,int PortCheck=TRUE);
	int Send( char *message,int sendlen,DWORD close_wait);	
	int Recv( char *msg,int len);
	

	virtual BOOL ITcpServer(void);
	virtual int CheckStatus(void);

//	virtual int Wait();
	virtual int Send( char *message);
	virtual int Recv( char *msg);
    virtual int SetPortNo(int PortNo);
	virtual int SetIPAdress(char *IPAdress);
	virtual int SetPcName(char *PcName);
	virtual int SetProgramName(char *ProgramName);
	int Wait( DWORD timeout=5000);	

	int SendDirect( char *message,int sendlen,DWORD close_wait);
	int RecvDirect( char *msg,int len);
	void GetClientIp( char *ip );	
protected:

};
#endif
