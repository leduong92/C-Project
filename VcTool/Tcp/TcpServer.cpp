////=====================================================================================================================
//
//	FileName:TcpServer.h
//	Class	:TcpServer
//			:�\�P�b�g�ʐM�N���X
//	�쐬��	:2002.07.22	  TAKE
//  �C����	:2003.10.18 TAKE �^�C���A�E�g�ݒ��ύX
//				ret = WSAWaitForMultipleEvents(cEvents,&m_Event,FALSE,5000,FALSE);��ret = WSAWaitForMultipleEvents(cEvents,&m_Event,FALSE,30000,FALSE);
//				time.tv_sec  = 1;  /* ��ѱ��1�b */��time.tv_sec  = 3; 
//  �C����	:2005.01.18 TAKE Recv() �ő�10�b�҂��ăG���[�Ƃ���@�\��ǉ�
//  �C����	:2005.04.15 TAKE �ő�Que�̐����P�ɕύX
//  �C����	:2005.05.08 ���� Recv() ��ʃf�[�^�̎�M�ɑς�����悤�C��
//	�C����	:2008.04.22 ���� Send()��print������ı�� (dt_down �Ńo�O�邽�߃R�����g�A�E�g)
//	�C����	:2008.11.11 matuda get to be enable client ip address
// 	�C����	:2010.06.08 ���� RecvDirect�Ń^�C���A�E�g���w��ł���悤�ύX
//=====================================================================================================================
#include "TcpServer.h"
#include "stdio.h"
#include "getnow.h"

TcpServer::TcpServer(int port)
{
	m_Status=0;
	m_PortNo = port;
	ITcpServer();
}

TcpServer::~TcpServer()
{
	if(m_Status==1){
		closesocket( m_FdAcpt );
		closesocket( m_Fd );
		WSACleanup();
	}
}

// 2005.07.11
TcpServer::TcpServer()
{
	m_Status=0;
	m_PortNo = -1;

}
//===================================================================================================================
// ������
//===================================================================================================================
int TcpServer::Initialize(int portNo,int PortCheck)
{
	if( PortCheck==TRUE ){
		if( portNo <1024) m_PortNo = DEF_TRANSPORT_NO;
		else m_PortNo = portNo;
	}else{
		m_PortNo = portNo;
	}

  	/* Winsock�̏����ݒ� */
//  	if ((m_Status = WSAStartup( MAKEWORD(1,1), &WSAData )) == 0){
  	if ((m_Status = WSAStartup( MAKEWORD(2,2), &WSAData )) == 0){
		//printf( "\n%s", WSAData.szSystemStatus);
  	}
  	else{
    	printf( "\nWSAStartup(%d) error", m_Status );
    	return -1;
  	}

  	/* ���Đ��� */
  	if ((m_FdAcpt = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    	printf( "\nsocket() error" );
    	return -2;
  	}

  	/* ���Ă̖��O�t�� */
  	memset(&m_AddrAcpt, 0, sizeof(m_AddrAcpt));  /* bzero(&addr, sizeof(addr)); */
  	m_AddrAcpt.sin_family      = PF_INET;
  	m_AddrAcpt.sin_port        = htons(m_PortNo);
  	m_AddrAcpt.sin_addr.s_addr = INADDR_ANY;
  	if (bind(m_FdAcpt, (struct sockaddr FAR *) &m_AddrAcpt, sizeof(m_AddrAcpt)) == SOCKET_ERROR) {
    	printf( "\nbind() error" );
    	closesocket( m_FdAcpt );
    	return -3;
  	}
	//4. Create event.
	m_Event = WSACreateEvent();
	if(m_Event == WSA_INVALID_EVENT)
	{
		closesocket(m_FdAcpt);
        WSACleanup();
		return -4;
	}

//	printf("R=%d W=%d a=%d O=%d Q=%d C=%d cl=%d\n", FD_READ ,FD_WRITE , FD_ACCEPT,FD_OOB,FD_QOS,FD_CONNECT , FD_CLOSE);
	//	nRet = WSAEventSelect(fSocket,fEvent,FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
	int err;
	err = WSAEventSelect(m_FdAcpt,m_Event,FD_ACCEPT );
	if(err == SOCKET_ERROR)
	{
        printf("\nWSAEventSelect()Err: %d\n", err);
		WSACloseEvent(m_Event);
		closesocket(m_FdAcpt);
		WSACleanup();
		return -5;
	}
	// 2005.04.14 �L���[��1�ɂ���
	err = listen(m_FdAcpt, 1); //err = listen(m_FdAcpt, 5);
  	if ( err == SOCKET_ERROR) {
		printf( "\nlisten() error=%d",WSAGetLastError() );
    	closesocket(m_FdAcpt);
    	return -6;
  	}	
	m_Status=1;
	return 0;
}


BOOL  TcpServer::ITcpServer()
{
	memset(m_ClientIp,0,sizeof(m_ClientIp));

  	printf( "\n" );
  	printf( "<<<   TCP Protocol Setup   >>>\n" );
  	printf( "\n" );
  	/* Winsock�̏����ݒ� */
//  	if ((m_Status = WSAStartup( MAKEWORD(1,1), &WSAData )) == 0){
  	if ((m_Status = WSAStartup( MAKEWORD(2,2), &WSAData )) == 0){
		printf( "%s\n", WSAData.szSystemStatus);
  	}
  	else{
    	printf( "WSAStartup(%d) error", m_Status );
    	return FALSE;
  	}

  	/* ���Đ��� */
  	if ((m_FdAcpt = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    	printf( "socket() error\n" );
    	return FALSE;
  	}

  	/* ���Ă̖��O�t�� */
  	memset(&m_AddrAcpt, 0, sizeof(m_AddrAcpt));  /* bzero(&addr, sizeof(addr)); */
  	m_AddrAcpt.sin_family      = PF_INET;
  	m_AddrAcpt.sin_port        = htons(m_PortNo);
  	m_AddrAcpt.sin_addr.s_addr = INADDR_ANY;
  	if (bind(m_FdAcpt, (struct sockaddr FAR *) &m_AddrAcpt, sizeof(m_AddrAcpt)) == SOCKET_ERROR) {
    	printf( "bind() error\n" );
    	closesocket( m_FdAcpt );
    	return FALSE;
  	}
	//4. Create event.
	m_Event = WSACreateEvent();
	if(m_Event == WSA_INVALID_EVENT)
	{
		closesocket(m_FdAcpt);
        WSACleanup();
		return FALSE;
	}

//	printf("R=%d W=%d a=%d O=%d Q=%d C=%d cl=%d\n", FD_READ ,FD_WRITE , FD_ACCEPT,FD_OOB,FD_QOS,FD_CONNECT , FD_CLOSE);
	//	nRet = WSAEventSelect(fSocket,fEvent,FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
	int err;
	err = WSAEventSelect(m_FdAcpt,m_Event,FD_ACCEPT );
	if(err == SOCKET_ERROR)
	{
        printf("\nWSAEventSelect()Err: %d\n", err);
		WSACloseEvent(m_Event);
		closesocket(m_FdAcpt);
		WSACleanup();
		return FALSE;
	}
//  2005.04.15 TAKE �ő�Que�̐����P�ɕύX
//	err = listen(m_FdAcpt, 5);
	err = listen(m_FdAcpt, 1);  
	if ( err == SOCKET_ERROR) {
		printf( "listen() error=%d\n",WSAGetLastError() );
    	closesocket(m_FdAcpt);
    	return -1;
  	}	
	m_Status=1;
	return TRUE;
}
//===================================================================================================================
// get client ip address
// Arg1:data pointer of output
//===================================================================================================================
void TcpServer::GetClientIp(char *ip)
{
	strcpy(ip,m_ClientIp);
}

// �����P�F�^�C���A�E�g(���N�G�X�g�҂�����) 05.12.01�ǉ�
int TcpServer::Wait( DWORD timeout/*=5000*/)
{
	WSAEVENT	hEvents[5];		//�C�x���g�n���h���̔z��ւ̃|�C���^
	DWORD		cEvents;		//hEvents�Ɋ܂܂��C�x���g�n���h���̐�		
	WSANETWORKEVENTS events;
    int ret=0;
	

	hEvents[0] = m_Event;
	cEvents=1;
	events.lNetworkEvents=0;
	ret = WSAWaitForMultipleEvents(cEvents,&m_Event,FALSE,timeout,FALSE);
//	ret = WSAWaitForMultipleEvents(cEvents,&m_Event,FALSE,5000,FALSE);
//	ret = WSAWaitForMultipleEvents(cEvents,&m_Event,FALSE,30000,FALSE);

    if (ret == WSA_WAIT_TIMEOUT)
    {
         printf(".");	// Wait ��ԕ\��
         return(-1);
    }
    ret = WSAEnumNetworkEvents(m_FdAcpt,m_Event,&events);
   if (ret == SOCKET_ERROR)
   {
        printf("WSAEnumNetworkEvents():Err=%d\n",WSAGetLastError());getchar();
        return(-2);
   }
#ifdef _DEBUGPRINT
printf("ret=%d event=%d\n",ret,events.lNetworkEvents);
#endif
    if((events.lNetworkEvents & FD_ACCEPT)==0)
	{
		printf("Event Err FD=%d\n",FD_ACCEPT);return(-3);
	}
    return(ret);
}
int  TcpServer::Recv( char *recvdat )
{
  	int		exit_flag;

	int		recv_len;
	int		err;
  	int len = sizeof(m_AddrAcpt);
	char buf[5+1];
	char revbuf[MAX_SENDLENGTH];
	
	int timecnt;
	int sockerr;

	/* �ײ��Ă̐ڑ��󂯕t�� */ 
	printf( "Waiting for connect...\n" );
  	if ((m_Fd = accept(m_FdAcpt, (struct sockaddr *)&m_Addr, &len)) < 0){
    	printf( "accept() error\n" );
    	closesocket( m_FdAcpt );
		return -2;
  	}
  	printf( "Connected.\n" );

	// set client ip 2008.11.11 matsuda
	sprintf(m_ClientIp,"%s",inet_ntoa(m_Addr.sin_addr));


  	/* ٰ���ޯ������̌J��Ԃ� */
  	exit_flag = 0;

	/* �ް���M */
    printf( "Waiting to receive data...\n" );
    recv_len = 0;  /* ��M�ް��������� */
	timecnt = 0;
	sockerr=0;
    while( 1 ){
      	do{
       		/* �ް���M�҂� */
        	FD_ZERO( &r_socket );
        	FD_ZERO( &w_socket );
        	FD_ZERO( &e_socket );
        	FD_SET( m_Fd, &r_socket );
//        	time.tv_sec  = 1;  /* ��ѱ��1�b */
//        	time.tv_usec = 0;
        	time.tv_sec  = 2;  //�^�C�����2�b�ɕύX
	      	time.tv_usec = 0;
        	err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
        	if(err < 0){
          		printf( "select() error\n" );
          		exit_flag = 1;
          		break;
       		}
        	else if( err == 0 ){
          		printf( "(receive time out)\n" );  /* ��ѱ�Ĕ��� */
				return 0;
        	}
        	else if( FD_ISSET( m_Fd, &r_socket ) == 0 ){
          		printf( "FD_ISSET() error\n" );
          		exit_flag = 1;
          		break;
        	}
		}while( err == 0 );
		/* ��M */
		memset(revbuf,0,sizeof(revbuf));
		recv_len=0;
		while(1){
			// 2014.05.22 coba binary�Ή�
			err = recv( m_Fd, &revbuf[recv_len], sizeof(revbuf)-recv_len  , 0 );
			//err = recv( m_Fd, &revbuf[strlen(revbuf)], sizeof(revbuf)-strlen(revbuf)  , 0 );
			if( err == -1 ){  /* ��M�װ */
				sockerr++;
				if(sockerr > 10){
					printf( "Err. Rcv Transfer failed.\n" );
					exit_flag = 1;
					//break;
					//return �ɕύX 2005.01.14 ����
					return -1;
				}
				else{
					Sleep(100);
					continue;
				}
			}
			sockerr=0;

			/* ��M�ް����̍X�V�E�m�F */
			recv_len += err;
			memcpy(buf,revbuf,5);
			buf[5]=0;
			sockerr=0;		// 2013.09.24 ��M�o������G���[�N���A

			if(atol(buf) <= recv_len-5){
				exit_flag = 0;
				break;
			}
			//�܂���M�o�C�g���ɖ����Ȃ�������
			else{
				Sleep(100);
				timecnt++;
				if(timecnt > 10){		//10�b�҂��ă_���Ȃ�^�C����� 2005.05.08 ����
					printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);
					exit_flag = 1;
					break;
				}
			}
		}

		if(atol(buf) <= recv_len-5) break;

	}
    /* ��۸��яI���m�F */
    if( exit_flag == 1 ){
		return -1;
	}
	memcpy(recvdat,revbuf+5,atol(buf));

	//	return recv_len;

	return atol(buf)-5;
}

/*
int  TcpServer::Recv( char *recvdat )
{
  	int		exit_flag;
	int		recv_len;
	int		err;
  	int len = sizeof(m_AddrAcpt);
	char buf[20];
	char revbuf[4096];
	char stime[20],etime[20];

	memset(stime,0,sizeof(stime));
	memset(etime,0,sizeof(etime));

	get_nowtime(stime,14,0);
	Calctime( etime , stime , 10 , MD_SEC );	// �ő�10�b�҂�

	// �ײ��Ă̐ڑ��󂯕t��  
	printf( "Waiting for connect...\n" );
  	if ((m_Fd = accept(m_FdAcpt, (struct sockaddr *)&m_Addr, &len)) < 0){
    	printf( "accept() error\n" );
    	closesocket( m_FdAcpt );
		return -2;
  	}
  	printf( "Connected.\n" );


  	// ٰ���ޯ������̌J��Ԃ� 
  	exit_flag = 0;

	// �ް���M 
    printf( "Waiting to receive data...\n" );
    recv_len = 0;  // ��M�ް��������� 

    do{
       	// �ް���M�҂� 
        FD_ZERO( &r_socket );
        FD_ZERO( &w_socket );
        FD_ZERO( &e_socket );
        FD_SET( m_Fd, &r_socket );
        time.tv_sec  = 1;  // ��ѱ��1�b 
        time.tv_usec = 0;
        err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
        if(err < 0){
        	printf( "select() error\n" );
        	exit_flag = 1;
        	break;
       	}
        else if( err == 0 ){
        	printf( "(receive time out)\n" );  // ��ѱ�Ĕ��� 
			return 0;
        }
        else if( FD_ISSET( m_Fd, &r_socket ) == 0 ){
        	printf( "FD_ISSET() error\n" );
        	exit_flag = 1;
        	break;
        }
	}while( err == 0 );
		// ��M 	
	
	
	while( 1 ){

		err = recv( m_Fd, revbuf, MAX_RECVLENGTH  , 0 );
		if( err == -1 ){  // ��M�װ 
			printf( "Rcv Transfer failed.\n" );
			exit_flag = 1;
			break;
		}
		// ��M�ް����̍X�V�E�m�F 
		recv_len += err;
//		if( recv_len == data_len ){
//			printf( "Transfer complete. [Serial No.%03d]\n", (unsigned char)recvdat[0] );
//			break;
//		}
		memcpy(buf,revbuf,5);
		buf[5]=0;

		if(atol(buf)==recv_len-5) break;
	
		printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);

		get_nowtime(stime,14,0);
		if(memcmp(stime,etime,14)>0) return 0;  // ��ѱ�Ĕ��� 
	}
    // ��۸��яI���m�F 
    if( exit_flag == 1 ){
		return -1;
	}
	memcpy(recvdat,revbuf+5,atol(buf));

	//	return recv_len;

	return atol(buf)-5;
}
*/

//===================================================================================================================
//	int  TcpServer::Send(char *msg)
//	�f�[�^���M Send(char*,len)���R�[������
//===================================================================================================================
int  TcpServer::Send(char *msg)
{
	return(this->Send(msg,strlen(msg),0));
}
/* 2008.04.25 Send�֐�����{��
int  TcpServer::Send(char *msg)
  	int		exit_flag=0;		// ���������� 06.03.16 coba
	int		err;

/// TOKEN�p�ɕύX���M�o�C�g����ǉ�
	char buf[5+1];
///	sprintf(buf,"%05ld",strlen(msg));		2005.01.19 TAKE
	sprintf(buf,"%05ld",strlen(msg)-5);
	memcpy(msg,buf,5);

    // �ް����M
    printf( "Waiting to send data...\n" );
 	do{
	// �ް����M�҂�
	    FD_ZERO( &r_socket );
	    FD_ZERO( &w_socket );
	    FD_ZERO( &e_socket );
	    FD_SET( m_Fd, &w_socket );
	    time.tv_sec  = 1;  // ��ѱ��1�b
	    time.tv_usec = 0;
	    err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
	    if(err < 0){
	        printf( "select() error\n" );
	        exit_flag = 1;
	        break;
      	}
	    else if( err == 0 ){
	        printf( "(send time out)\n" );  // ��ѱ�Ĕ���
	    }
	    else if( FD_ISSET( m_Fd, &w_socket ) == 0 ){
	       	printf( "FD_ISSET() error\n" );
	        exit_flag = 1;
	        break;
	    }
	}while( err == 0 );

    // ���M 

//	FILE *fp;
//	fp=fopen("send.log","w");
//	fprintf(fp,"%s***",msg);
//	fclose(fp);
//	printf("msg=%s***len=%d\n",msg,strlen(msg));
//	err = send( m_Fd, &msg[0], strlen(msg) , 0 );


	
	err = send( m_Fd, &msg[0], MAX_SENDLENGTH , 0 );
    if( err == -1 ){
		printf( "Send Transfer failed.\n" );
    }
    printf( "Send Transfer complete.\n" );

    if(  exit_flag == 1 ) return -1;

	// 2008.04.24 ������close�͂���
	closesocket( m_Fd );
//	closesocket( m_FdAcpt );
	printf( "Terminated.\n" );

	return 0;
}
*/

int TcpServer::SetPortNo(int portNo)
{
	if(portNo>0x09999 || portNo <0x900) return(1);
	m_PortNo = portNo;
	
	return(0);	

}
int TcpServer::SetIPAdress(char *IPAdress)
{
	strncpy(m_IPAdress,IPAdress,15);
	m_IPAdress[15]=0;
	return(0);	

}

int TcpServer::SetPcName(char *PcName)
{
	strncpy(fPcName,PcName,20);
	fPcName[20]=0;
	return(0);	
}

int TcpServer::SetProgramName(char *ProgramName)
{
	strncpy(fProgramName,ProgramName,20);
	fProgramName[20]=0;
	return(0);	

}
int TcpServer::CheckStatus()
{
	return(m_Status);
}

//===================================================================================================================
//	int  TcpServer::Recv( char *recvdat,int bufsize )
//	�@�\�@�F�\�P�b�g�̃f�[�^����M����
//	�����@�F��M�o�b�t�@
//	�߂�l�F��M�o�C�g��
//	2005.07.11�ǉ�
//===================================================================================================================
int  TcpServer::Recv( char *recvdat,int bufsize )
{
	int		recv_len;
	int		err;
  	int len = sizeof(m_AddrAcpt);
	char buf[5+1];
	char revbuf[MAX_SENDLENGTH+10];
	int timeout,sockerr;

	/* �ײ��Ă̐ڑ��󂯕t�� */ 
#ifdef _DEBUGPRINT
	printf( "\nWaiting for connect..." );
#endif
  	if ((m_Fd = accept(m_FdAcpt, (struct sockaddr *)&m_Addr, &len)) < 0){
    	printf( "accept() error\n" );
    	closesocket( m_FdAcpt );
		return -2;
  	}
#ifdef _DEBUGPRINT
  	printf( "Connected." );
#endif

	// set client ip 2010.02.25 matsuda
	sprintf(m_ClientIp,"%s",inet_ntoa(m_Addr.sin_addr));

	/* �ް���M */
#ifdef _DEBUGPRINT
    printf( "\nWaiting to receive data..." );
#endif

	/* �ް���M�҂� */
   	FD_ZERO( &r_socket );
   	FD_ZERO( &w_socket );
   	FD_ZERO( &e_socket );
	FD_SET( m_Fd, &r_socket );
    time.tv_sec  = 1;  /* ��ѱ��1�b */
    time.tv_usec = 0;
    err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
    if(err < 0){
   		printf( "select() error\n" );
   		return(0);
	}
   	else if( err == 0 ){
	printf( "(receive time out)\n" );  /* ��ѱ�Ĕ��� */
		return 0;
   	}
  	else if( FD_ISSET( m_Fd, &r_socket ) == 0 ){
		printf( "FD_ISSET() error\n" );
		return 0;
	}

	/* ��M�ް��������� */
    recv_len = 0; 
	timeout=0;
	sockerr=0;
	memset( revbuf,0,sizeof(revbuf));

	/* ��M */
    while( 1 ){
		// 2014.05.22 coba binary�Ή�
		err = recv( m_Fd, &revbuf[recv_len], sizeof(revbuf)-recv_len  , 0 );
		//err = recv( m_Fd, &revbuf[strlen(revbuf)], sizeof(revbuf)-strlen(revbuf)  , 0 );
		/* ��M�װ */
		if( err == -1 ){ 
			if(++sockerr > 10){
				printf( "Rcv Transfer failed.\n" );
				return(0);
			}
			else{
				Sleep(100);
				continue;
			}
		}
		sockerr=0;

		/* ��M�ް����̍X�V�E�m�F */
		recv_len += err;

		sockerr=0;		// 2013.09.24 ��M�o������G���[�N���A

		// �ް��w�b�_�����M���𔲂��o��
		memcpy(buf,revbuf,5);
		buf[5]=0;

		// ��M������
		if(atol(buf)<=recv_len-5) break;
		// �c�肠��
		else{
			if(++timeout >= 30){		//�R�b�҂��ă_���Ȃ�^�C�����
				printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);
				return(0);
			}
			Sleep(100);
		}

#ifdef _DEBUGPRINT
		printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);
#endif
	}
	memcpy(recvdat,revbuf+5,atol(buf)> bufsize ? bufsize:atol(buf));

	//	return recv_len;
	// ��M�o�C�g����߂�
	return atol(buf);
}

//===================================================================================================================
//	int  TcpServer::Send(char *msg,int sendlen)
//	�@�\�@�F�f�[�^�̑��M
//	�����@�F���M�o�b�t�@
//	�߂�l�F���M�o�C�g��
//	2005.07.11�ǉ�
//	2008.04.25 ���M��A�ʐM���肩���close��҂��Ă��炱���瑤��close����
//===================================================================================================================
int  TcpServer::Send(char *msg,int sendlen,DWORD close_wait)
{
  	int		exit_flag=0;		// ���������� 06.03.16 coba
	int		err;
	char buf[MAX_SENDLENGTH+10];

/// TOKEN�p�ɕύX���M�o�C�g����ǉ�

	sprintf(buf,"%05ld",sendlen);
//	sprintf(buf,"%05ld",strlen(msg));
	memcpy(buf+5, msg, sendlen);
	buf[sendlen+5]=0;

	//2008.04.22 ���� dt_down �Ńo�O�邽�߃R�����g�A�E�g
//	printf("\nsend=%s",buf+5);

    /* �ް����M */
#ifdef _DEBUGPRINT
    printf( "\nWaiting to send data..." );
#endif
 	do{
	/* �ް����M�҂� */
	    FD_ZERO( &r_socket );
	    FD_ZERO( &w_socket );
	    FD_ZERO( &e_socket );
	    FD_SET( m_Fd, &w_socket );
	    time.tv_sec  = 1;  /* ��ѱ��1�b */
	    time.tv_usec = 0;
	    err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
	    if(err < 0){
	        printf( "select() error\n" );
	        exit_flag = 1;
	        break;
      	}
	    else if( err == 0 ){
	        printf( "(send time out)\n" );  /* ��ѱ�Ĕ��� */
	    }
	    else if( FD_ISSET( m_Fd, &w_socket ) == 0 ){
	       	printf( "FD_ISSET() error\n" );
	        exit_flag = 1;
	        break;
	    }
	}while( err == 0 );

	err = send( m_Fd, buf, sendlen+5 , 0 );
    if( err == -1 ){
		printf( "Send Transfer failed.\n" );
    }
#ifdef _DEBUGPRINT
    printf( "Send Transfer complete.\n" );
#endif
    if(  exit_flag == 1 ) return -1;

	// 2008.05.08 ����݊��̂��ߑ��N���[�Y��ǉ�
	if( close_wait < 1 ){
		closesocket( m_Fd );
		return 0;
	}

	// 2008.04.25 coba	�������I���܂�
	WSAEVENT	hEvent;		//�C�x���g�n���h���̔z��ւ̃|�C���^
	WSANETWORKEVENTS events;
	DWORD ret;
	hEvent = WSACreateEvent();
	if(hEvent == WSA_INVALID_EVENT)
	{
		closesocket(m_Fd);
        WSACleanup();
		return -4;
	}
	// �������Close���󂯎���Ă���ڑ������
	// �����������ꍇ�ȂǁA���肪�󂯎��O�ɂ�����ŕ��Ă��܂��̂�h������
	err = WSAEventSelect(m_Fd,hEvent,FD_CLOSE );
	if(err == SOCKET_ERROR)
	{
        printf("\nWSAEventSelect()Err: %d\n", err);
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
		return -5;
	}

	ret=WSAWaitForMultipleEvents(1,&hEvent,FALSE,close_wait,FALSE);
	if (ret == WSA_WAIT_TIMEOUT)
    {
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
        return(-1);
    }
    ret = WSAEnumNetworkEvents(m_Fd,hEvent,&events);
   if (ret == SOCKET_ERROR)
   {
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
        printf("WSAEnumNetworkEvents():Err=%d\n",WSAGetLastError());
        return(-2);
   }

// �m�F�p 2008.04.25
#ifdef _DEBUGPRINT
    if(events.lNetworkEvents & FD_CLOSE)
	{
		printf("Event close%d,\n",FD_CLOSE);
	}
	getchar();
#endif

	// �C�x���g����A�\�P�b�g��close����
	WSACloseEvent(hEvent);
	closesocket( m_Fd );
	memset(m_ClientIp,0,sizeof(m_ClientIp));
	return 0;
}

//===================================================================================================================
//	int  TcpServer::Recv( char *recvdat,int bufsize )
//	�@�\�@�F�\�P�b�g�̃f�[�^���o�b�t�@�������M����B���M�f�[�^�����J�b�g���Ȃ�
//	�����@�F��M�o�b�t�@
//	�߂�l�F��M�o�C�g��
//	2005.07.11�ǉ�
// 2010.06.08 ���� �^�C���A�E�g���w��ł���悤�ύX
//===================================================================================================================
int  TcpServer::RecvDirect( char *recvdat,int bufsize ,int time_out/*=1*/)
{
	int		recv_len;
	int		err;
  	int len = sizeof(m_AddrAcpt);
	int timeout,sockerr;
	char buf[10];

	/* �ײ��Ă̐ڑ��󂯕t�� */ 
#ifdef _DEBUGPRINT
	printf( "\nWaiting for connect..." );
#endif
  	if ((m_Fd = accept(m_FdAcpt, (struct sockaddr *)&m_Addr, &len)) < 0){
    	printf( "accept() error\n" );
    	closesocket( m_FdAcpt );
		return -2;
  	}
#ifdef _DEBUGPRINT
  	printf( "Connected." );
#endif

	// set client ip 2010.02.25 matsuda
	sprintf(m_ClientIp,"%s",inet_ntoa(m_Addr.sin_addr));

	/* �ް���M */
#ifdef _DEBUGPRINT
    printf( "\nWaiting to receive data..." );
#endif

	/* �ް���M�҂� */
   	FD_ZERO( &r_socket );
   	FD_ZERO( &w_socket );
   	FD_ZERO( &e_socket );
	FD_SET( m_Fd, &r_socket );
//    time.tv_sec  = 1;  /* ��ѱ��1�b */
	// 2010.06.08 ���� �^�C���A�E�g���w��ł���悤�ύX
    time.tv_sec  = time_out;  /* ��ѱ��1�b */
    time.tv_usec = 0;
    err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
    if(err < 0){
   		printf( "select() error\n" );
   		return(-1);
	}
   	else if( err == 0 ){
	printf( "(receive time out)\n" );  /* ��ѱ�Ĕ��� */
		return -2;
   	}
  	else if( FD_ISSET( m_Fd, &r_socket ) == 0 ){
		printf( "FD_ISSET() error\n" );
		return -3;
	}

	/* ��M�ް��������� */
    recv_len = 0; 
	timeout=0;
	sockerr=0;
	memset( recvdat,0,bufsize);
	int chk=0,pos=0,dlen=0;

	/* ��M */
    while( 1 ){
		err = recv( m_Fd, &recvdat[recv_len], bufsize-recv_len  , 0 );
		/* ��M�װ */
		if( err == -1 ){ 
			if(++sockerr > 10){
				printf( "Rcv Transfer failed.\n" );
				return(0);
			}
			else{
				Sleep(50);
				continue;
			}
		}

		/* ��M�ް����̍X�V�E�m�F */
		recv_len += err;
		sockerr=0;		// 2013.09.24 ��M�o������G���[�N���A

		// �ް��w�b�_�����M���𔲂��o��
		if(!chk){
			memcpy(buf,recvdat,5);
			buf[5]=0;
			dlen=atol(buf);
			chk=1;
		}

		// ��M������
		if(dlen<=recv_len-5) break;
		else{
			if(++timeout >= 30){		//�R�b�҂��ă_���Ȃ�^�C�����
				printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);
				return(-10);
			}
			Sleep(100);
		}
//		else return(0);
#ifdef _DEBUGPRINT
		printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);
#endif
	}

	// ��M�o�C�g����߂�
	return recv_len;
}

//===================================================================================================================
//	int  TcpServer::Send(char *msg,int sendlen)
//	�@�\�@�F�o�b�t�@����������M�B���M�o�C�g�������Ȃ�
//	�����@�F���M�o�b�t�@
//	�߂�l�F���M�o�C�g��
//	2008.04.24 coba �ǉ�
//===================================================================================================================
int  TcpServer::SendDirect(char *msg,int sendlen,DWORD close_wait)
{
  	int		exit_flag=0;		// ���������� 06.03.16 coba
	int		err;

    /* �ް����M */
#ifdef _DEBUGPRINT
    printf( "\nWaiting to send data..." );
#endif
 	do{
	/* �ް����M�҂� */
	    FD_ZERO( &r_socket );
	    FD_ZERO( &w_socket );
	    FD_ZERO( &e_socket );
	    FD_SET( m_Fd, &w_socket );
	    time.tv_sec  = 1;  /* ��ѱ��1�b */
	    time.tv_usec = 0;
	    err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
	    if(err < 0){
	        printf( "select() error\n" );
	        exit_flag = 1;
	        break;
      	}
	    else if( err == 0 ){
	        printf( "(send time out)\n" );  /* ��ѱ�Ĕ��� */
	    }
	    else if( FD_ISSET( m_Fd, &w_socket ) == 0 ){
	       	printf( "FD_ISSET() error\n" );
	        exit_flag = 1;
	        break;
	    }
	}while( err == 0 );

	// ���M����
	err = send( m_Fd, msg, sendlen, 0 );
    if( err == -1 ){
		printf( "Send Transfer failed.\n" );
    }
#ifdef _DEBUGPRINT
    printf( "Send Transfer complete.\n" );
#endif
    if(  exit_flag == 1 ) return -1;

	// 2008.04.25 coba	�������I���܂�
	WSAEVENT	hEvent;		//�C�x���g�n���h���̔z��ւ̃|�C���^
	WSANETWORKEVENTS events;
	DWORD ret;
	hEvent = WSACreateEvent();
	if(hEvent == WSA_INVALID_EVENT)
	{
		closesocket(m_Fd);
        WSACleanup();
		return -4;
	}
	// �������Close���󂯎���Ă���ڑ������
	// �����������ꍇ�ȂǁA���肪�󂯎��O�ɂ�����ŕ��Ă��܂��̂�h������
	err = WSAEventSelect(m_Fd,hEvent,FD_CLOSE );
	if(err == SOCKET_ERROR)
	{
        printf("\nWSAEventSelect()Err: %d\n", err);
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
		return -5;
	}

	if( close_wait < 1 ) close_wait=1000;
	ret=WSAWaitForMultipleEvents(1,&hEvent,FALSE,close_wait,FALSE);
	if (ret == WSA_WAIT_TIMEOUT)
    {
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
        return(-1);
    }
    ret = WSAEnumNetworkEvents(m_Fd,hEvent,&events);
   if (ret == SOCKET_ERROR)
   {
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
        printf("WSAEnumNetworkEvents():Err=%d\n",WSAGetLastError());
        return(-2);
   }

// �m�F�p 2008.04.25
#ifdef _DEBUGPRINT
    if(events.lNetworkEvents & FD_CLOSE)
	{
		printf("Event close%d,\n",FD_CLOSE);
	}
	getchar();
#endif

	// �C�x���g����A�\�P�b�g��close����
	WSACloseEvent(hEvent);
	closesocket( m_Fd );

	return 0;
}
