// ========================================================================
//        FTP�v���g�R���𗘗p�����t�@�C���]���̃N���C���A���g�N���X
//        �t�@�C�����FFTP.cpp
//
//        �쐬�ҁF�Γc���O�i�m�n�j������А��Y�Z�p�{�����Y�Z�p���j
//        �쐬���F 2003�N5��24��
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

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <ctype.h>
#include "time.h"
#include "FTP.h"

FTP::FTP()
{
	m_printMode=FALSE;

    WORD wVersionRequested = MAKEWORD(2,2);//���ׂĒ��ڎw�肷�邱�ƁB
    WSADATA wsaData;
    int nRet;

	fStatus=0;

//1. Initialize WinSock.dll
    nRet = WSAStartup(wVersionRequested, &wsaData);
    if (nRet)
    {
        sprintf(pbuf,"\nWSAStartup()Err: %d\n", nRet);
		this->Print(pbuf);
        WSACleanup();
        return;
    }
//2. Check WinSock version
    if (wsaData.wVersion != wVersionRequested)
    {
        sprintf(pbuf,"\nWinSock version not supported\n");
		this->Print(pbuf);
        WSACleanup();
        return;
    }

//3. Create  socket
    fSocketC = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fSocketC == INVALID_SOCKET)
    {
        sprintf(pbuf,"socket()Err");
		this->Print(pbuf);
        WSACleanup();
        return;
    }

//4. Create event.
	fEvent = WSACreateEvent();
	if(fEvent == WSA_INVALID_EVENT)
	{
       sprintf(pbuf,"\nWSACreateEvent()Err: %d\n", nRet);
		this->Print(pbuf);
		closesocket(fSocketC);
        WSACleanup();
		return;
	}

//	nRet = WSAEventSelect(fSocketC,fEvent,FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
	nRet = WSAEventSelect(fSocketC,fEvent,FD_READ);
	if(nRet == SOCKET_ERROR)
	{
        sprintf(pbuf,"\nWSAEventSelect()Err: %d\n", nRet);
		this->Print(pbuf);
		WSACloseEvent(fEvent);
		closesocket(fSocketC);
		WSACleanup();
		return;
	}

	bConnected=0;
	fStatus=1;
    IFTP();
}

FTP::~FTP()
{

	if(fStatus==1)
	{
		if(fSocketC!=INVALID_SOCKET)
		{
			if(WSAIsBlocking())
			{
				WSACancelBlockingCall();
			}
		}
  		closesocket(fSocketC);
		WSACloseEvent(fEvent);
		WSACleanup();
	}

}

BOOL FTP::IFTP()
{
	return(TRUE);
}


//Initial connection
int FTP::Connect()
{
	int iFlag=1;
	char answer[500];
	char buf[256];
	int replyNo;
	int nRet,len;
    int nReason;

	// m_log�̏�����
	memset(m_log,0,sizeof(m_log));


	if(bConnected==1)
	{
		sprintf(pbuf,"Already connected!");
		this->Print(pbuf);
		return(1);
	}

//0.Address ��ݒ肷��B
////	m_PortNo=PORTNO;				//�萔����SetPortNo()�ŕϐ��ɕύX 2005.05.06 ����
////	strcpy(m_IPAdress,IPADRESS);	//�萔����SetServerIp()�ŕϐ��ɕύX 2005.05.06 ����

	memset(&fAddrC, 0, sizeof(fAddrC));
	fAddrC.sin_family		= AF_INET;
    fAddrC.sin_addr.s_addr   = inet_addr(m_IPAdress);
//	fAddrC.sin_addr.S_un.S_addr= inet_addr(fIPAdress);
    fAddrC.sin_port          = htons(m_PortNo);

// 1.Create a connected socket
	nRet=connect(fSocketC,(struct sockaddr*)&fAddrC,sizeof(struct sockaddr));
	if(nRet==SOCKET_ERROR)
	{
		nReason=WSAGetLastError();
		if(nReason!=WSAEWOULDBLOCK)
		{
			sprintf(pbuf,"Connection failed=%d",nReason);
			this->Print(pbuf);
			return(1);
		}
	}
//3.Get initial message recieve
	memset(answer,0,sizeof(answer));
	nRet=this->Recv(answer);
	if(nRet==SOCKET_ERROR)
	{
		sprintf(pbuf,"getsocknameErr1=%d",WSAGetLastError());
		this->Print(pbuf);
		return(1);
	}


//2. Get information about local end of the connection
	len=sizeof(struct sockaddr);
	nRet=getsockname(fSocketC,(struct sockaddr*)&fCtrlAddr,&len);
	if(nRet==SOCKET_ERROR)
	{
		sprintf(pbuf,"getsocknameErr2=%d",WSAGetLastError());
		this->Print(pbuf);
		return(1);
	}
	else
	{
		sprintf(pbuf,"Local : %s port %u \n\n",inet_ntoa(fCtrlAddr.sin_addr),ntohs(fCtrlAddr.sin_port));
		this->Print(pbuf);
	}


//4. Send User ID and PASS WORD

	//���[�U�[��
	memset(buf,0,sizeof(buf));
	sprintf(buf,"USER %s",m_User);
	nRet=CommandSend(buf,&replyNo,answer);
////	nRet=CommandSend("USER nxtftp",&replyNo,answer);
	if(nRet==SOCKET_ERROR)
	{
		sprintf(pbuf,"getsocknameErr3=%d",WSAGetLastError());
		this->Print(pbuf);
		return(1);
	}

	if(replyNo!=331)
	{
		sprintf(pbuf,"UserNameErr");
		this->Print(pbuf);
		return(1);
	}
//5. Send PassWord �p�X���[�h
	memset(buf,0,sizeof(buf));
	sprintf(buf,"PASS %s",m_Password);
	nRet=CommandSend(buf,&replyNo,answer);
////	nRet=CommandSend("PASS nxtftp",&replyNo,answer);
	if(nRet==SOCKET_ERROR)
	{
		sprintf(pbuf,"getsocknameErr4=%d",WSAGetLastError());
		this->Print(pbuf);
		return(1);
	}
	if(replyNo!=230)
	{
		sprintf(pbuf,"PassWord Err\n");
		this->Print(pbuf);
		return(1);
	}
	return(0);
}


// PASV �p�b�V�u���[�h�ւ̈ڍs���w������B�T�[�o�̓f�[�^�R�l�N�V������
//		�g�p����IP�A�h���X�ƃ|�[�g�ԍ���\������B(�擾����B�j
int FTP::DoPASV(void)
{
	int replyNo,nRet;
	char message[100],reply[700],buf[300];;
	int i1,i2,i3,i4,p1,p2,len,i;
    int  thePortNo;
	char theIPAdress[20];

	char *ptr;
	memset(message,0,sizeof(message));
	memset(reply,0,sizeof(reply));
	memset(buf,0,sizeof(buf));
	sprintf(message,"PASV");
	nRet=CommandSend(message,&replyNo,reply);
	if(nRet==1) {sprintf(pbuf,"PASVError");this->Print(pbuf);return(nRet);}
	if(replyNo >=450 && replyNo <460) return(replyNo);
	strcpy(buf,reply);
	
	len =strlen(buf);
	ptr=NULL;i=3;
	while(i<len)
	{
		if(isdigit(buf[i])){ ptr=&buf[i];break;}
		i++;
	}
	if(ptr==NULL )					return(100);
	if((ptr=strtok(ptr,","))==NULL) return(101);
	i1=atoi(ptr);
	if((ptr=strtok(NULL,","))==NULL) return(102);
	i2=atoi(ptr);
	if((ptr=strtok(NULL,","))==NULL) return(103);
	i3=atoi(ptr);
	if((ptr=strtok(NULL,","))==NULL) return(104);
	i4=atoi(ptr);
	if((ptr=strtok(NULL,","))==NULL) return(104);
	
	p1=atoi(ptr);
	if((ptr=strtok(NULL,")\n"))==NULL) return(105);
	p2=atoi(ptr);

	thePortNo=p1*256+p2;
	sprintf(theIPAdress,"%d.%d.%d.%d",i1,i2,i3,i4);
	
//	fAddrT;		//�z�X�g�̃t�@�C���]���p�A�h���X�B
	memset(&fAddrT, 0, sizeof(fAddrT));
	fAddrT.sin_family		= AF_INET;
    fAddrT.sin_addr.s_addr   = inet_addr(theIPAdress);
    fAddrT.sin_port          = htons(thePortNo);

//	sprintf(pbuf,"%s  :%d\n",theIPAdress,thePortNo);
//	this->Print(pbuf);

	return(nRet);	
}

int  FTP::SendFileWrite(char *fnCliant, char *fnServer,int code)
{
	return(SendFile(fnCliant,fnServer,1,code));

}
int  FTP::SendFileAppend(char *fnCliant, char *fnServer,int code)
{
	return(SendFile(fnCliant,fnServer,2,code));
}
int  FTP::SendFileUnique(char *fnCliant, char *fnServer,int code)
{
	return(SendFile(fnCliant,fnServer,3,code));

}


// �߂�l�F��:�G���[�@���F���M�o�C�g��
int  FTP::SendFile(char *fnCliant, char *fnServer,int mode,int code)
{
	WSABUF		Buffers;		//WSABUF�\���̔z��
	DWORD		dwBufferCount;	//Buffers�̎����z��ɓ����Ă���WSABUF�\���̂̐�
	DWORD		NumberOfBytesSent;//���M�o�C�g�����󂯎��DWORD�ϐ��ւ̃|�C���^
	DWORD		dwFlags;		//���M���@�𐧌䂷��r�b�g�}�X�N
	int nRet,nReason,ret;	
    int     len,ftplen;
	char    buf[5000];
	long	i;
	
	FILE *fp;
	
	fp=fopen(fnCliant,"rb");
	if(fp==NULL)
	{
        sprintf(pbuf,"File Open Err =%S",fnCliant); 
		this->Print(pbuf);
        return(200);
    }	
	
	this->DoPASV();//�p�b�V�u���[�h�ւ̈ڍs
    fSocketT = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fSocketT == INVALID_SOCKET)
    {
        sprintf(pbuf,"socket()Err"); 
		this->Print(pbuf);
        WSACleanup();
		fclose(fp);
        return(100);
    }

	nRet=connect(fSocketT,(struct sockaddr*)&fAddrT,sizeof(struct sockaddr));
	if(nRet==SOCKET_ERROR)
	{
		nReason=WSAGetLastError();
		if(nReason!=WSAEWOULDBLOCK)
		{
			sprintf(pbuf,"Connection failed=%d",nReason);
			this->Print(pbuf);
			fclose(fp);
			return(1);
		}
	}
	
	ret=0;
	if		(mode==1)	ret=this->CmdStor(fnServer);
	else if	(mode==2)	ret=this->CmdStou(fnServer);
	else if	(mode==3)	ret=this->CmdAppe(fnServer);
	else				ret=this->CmdStor(fnServer);
	if(ret) return ret;		// 2004/3/30 oda 

	ftplen=0;
	memset(buf,0,sizeof(buf));
	while((len=fread(buf,1,1024,fp))!=0)
	{		
		// �������Ή� 2004/4/14 add oda
		if(code){
			for(i=0;i<len;i++){
				if((0x81 <= (unsigned char)buf[i] && (unsigned char)buf[i] <= 0x9F) 
				|| (0xE0 <= (unsigned char)buf[i] && (unsigned char)buf[i] <= 0xEF)){
					i++;
				}else{
					if(islower(buf[i])){
						buf[i]=toupper(buf[i]);
					}
				}
			}
		}
		Buffers.len=len;
		Buffers.buf=buf;
		dwBufferCount=1;
		dwFlags=MSG_DONTROUTE;//MSG_PARTIAL;


		nRet=WSASend(fSocketT,&Buffers,dwBufferCount, &NumberOfBytesSent,dwFlags,NULL,NULL);
		if (nRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAENETDOWN) return(-1);

			nRet=WSASend(fSocketT,&Buffers,dwBufferCount, &NumberOfBytesSent,dwFlags,NULL,NULL);
		    if (nRet == SOCKET_ERROR)
			{   
			    return(-3);
			}
		}
		ftplen=ftplen+len;
	}
	closesocket(fSocketT);

	this->Recv(buf);
	fclose(fp);

	this->Print((char*)nRet);
	
	if(nRet){
		return(nRet);
	}else{
		return(ftplen);
	}

	return(nRet);
}
int  FTP::RecvFile(char *fnCliant, char *fnServer)
{
	// m_log�̏�����
	memset(m_log,0,sizeof(m_log));
//	WSABUF		Buffers;		//WSABUF�\���̔z��
//	DWORD		dwBufferCount;	//Buffers�̎����z��ɓ����Ă���WSABUF�\���̂̐�
//	DWORD		NumberOfBytesRsv;//���M�o�C�g�����󂯎��DWORD�ϐ��ւ̃|�C���^
//	DWORD		dwFlags;		//���M���@�𐧌䂷��r�b�g�}�X�N
//	WSAEVENT	hEvents[5];		//�C�x���g�n���h���̔z��ւ̃|�C���^
//	DWORD		cEvents;		//hEvents�Ɋ܂܂��C�x���g�n���h���̐�		
	int nRet,nReason,ErrCnt=0;
//	WSANETWORKEVENTS events;
	long ret;
  
	WSAEVENT theEvent;
	
	char    buf[1024];			//5000 to 1024 oda
	
	FILE *fp;

	fp=fopen(fnCliant,"wb");
	if(fp==NULL)
	{
        sprintf(pbuf,"File Open Err =%s",fnCliant); 
		this->Print(pbuf);
		strcat(m_log,pbuf);
		m_log[strlen(m_log)]=0x0d;
		m_log[strlen(m_log)+1]=0x0a;
//		strcat(m_log,"\r\n");
        return(200);
    }

	this->DoPASV();//�p�b�V�u���[�h�ւ̈ڍs

    fSocketT = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fSocketT == INVALID_SOCKET)
    {
        sprintf(pbuf,"socket()Err"); 
		this->Print(pbuf);
		fclose(fp);
        return(100);
    }
//4. Create event.
	theEvent = WSACreateEvent();
	if(theEvent == WSA_INVALID_EVENT)
	{
		sprintf(pbuf,"\nWSACreateEvent()Err\n");
		this->Print(pbuf);
		closesocket(fSocketT);
		fclose(fp);
		return(101);
	}
//	nRet = WSAEventSelect(fSocketC,fEvent,FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
	nRet = WSAEventSelect(fSocketT,theEvent,FD_READ | FD_CLOSE);
	if(nRet == SOCKET_ERROR)
	{
        sprintf(pbuf,"\nWSAEventSelect()Err: %d\n", nRet);
		this->Print(pbuf);
		WSACloseEvent(theEvent);
		closesocket(fSocketT);
		fclose(fp);
		return(102);
	}

	nRet=connect(fSocketT,(struct sockaddr*)&fAddrT,sizeof(struct sockaddr));
	if(nRet==SOCKET_ERROR)
	{
		nReason=WSAGetLastError();
		if(nReason!=WSAEWOULDBLOCK)
		{
			this->Print(pbuf);
			sprintf(pbuf,"Connection failed=%d",nReason);
			fclose(fp);
			return(1);
		}
	}

	nRet=this->CmdRetr(fnServer);//��M�t�@�C�����w��B�]���N���B
	//	nRet=this->DoLIST();//���X�g�̎�M�B�]���N���B

	// �O�̏����������Ă�����I��
	if(resCode >=400 && resCode < 600) {
		return resCode;
	}
/*
	while(nRet==0)
	{
		//hEvents[0]=fEvent;
		//cEvents=1;
		// �C�x���g�҂�
		nRet = WSAWaitForMultipleEvents(1,&theEvent,FALSE,1000,FALSE);		//timeOut 10000
		if(nRet == WSA_WAIT_FAILED)
        {
			sprintf(pbuf,"\nWait Failed\n");
			this->Print(pbuf);
			sprintf(pbuf,"timeout Event=%02x\n",events.lNetworkEvents);
			this->Print(pbuf);
			memset(pbuf,0,sizeof(pbuf));
			memcpy(pbuf,Buffers.buf,NumberOfBytesRsv);
			this->Print(pbuf);
            break;
        }
		if (nRet == WSA_WAIT_TIMEOUT)
		{
			sprintf(pbuf,"\nWait timed out\n");
			this->Print(pbuf);
			sprintf(pbuf,"timeout Event=%02x\n",events.lNetworkEvents);
			this->Print(pbuf);
			memset(pbuf,0,sizeof(pbuf));
			memcpy(pbuf,Buffers.buf,NumberOfBytesRsv);
			this->Print(pbuf);
			break;
		}
		// �C�x���g����		
		nRet = WSAEnumNetworkEvents(fSocketT,theEvent,&events);
		//memset(pbuf,0,sizeof(pbuf));
		//sprintf(pbuf,"Event=%02x\n",events.lNetworkEvents);
		//this->Print(pbuf);
		if (nRet == SOCKET_ERROR)
		{
			this->Print(pbuf);
			sprintf(pbuf,"WSAEnumNetworkEvents()");
			continue;
		}
		if(events.lNetworkEvents == 0x21){
			strcat(m_log,"events.lNetworkEvents = 0x21!!\r\n");
			return 1;
		}		

		if(events.lNetworkEvents & FD_READ)
		{
			memset(buf,0,sizeof(buf));
			Buffers.len=sizeof(buf);
			Buffers.buf=buf;
			dwBufferCount=1;
			dwFlags=0;
			nRet=WSARecv(fSocketT,&Buffers,dwBufferCount,&NumberOfBytesRsv,&dwFlags,NULL,NULL);
			if(nRet!=0)
			{
				sprintf(pbuf,"WSARecv ERROR\n");
				this->Print(pbuf);
			}
			//fwrite(Buffers.buf,dwBufferCount,NumberOfBytesRsv,fp);		
			fwrite(Buffers.buf,NumberOfBytesRsv,dwBufferCount,fp);		
		}
		if(events.lNetworkEvents == FD_CLOSE)
		// if(events.lNetworkEvents & FD_CLOSE)
		{
			sprintf(pbuf,">>>>>Received Complete<<<<<\n");
			this->Print(pbuf);
			break;
		}
	}
*/
// WSARecv�����悤���Ȃ� 2004/06/23 oda

	while(nRet==0)
	{
		memset(buf,0,sizeof(buf));
		ret=recv(fSocketT,buf,sizeof(buf),0);
		if(!ret){
			sprintf(pbuf,">>>>>Received Complete<<<<<\n");
			this->Print(pbuf);
			break;
		}
		if(ret==SOCKET_ERROR){
			if(WSAGetLastError()==WSAEWOULDBLOCK){
				continue;		// WSAEWOULDBLOCK�̏ꍇ�͖��Ȃ� add oda
			}
			sprintf(pbuf,">>>>>socet_error<<<<<\n");
			this->Print(pbuf);
			break;
		}
		if(ret>0){
			fwrite(buf,ret,1,fp);
		}
	}

	closesocket(fSocketT);
	WSACloseEvent(theEvent);

	this->Recv(buf);

	fclose(fp);
	return(nRet);
}


//�f�B���N�g���[�E�t�@�C������֌W
int FTP::CmdCwd(LPSTR path)		{return(this->DoCommand(1,path));}	// CWD �J�����g�f�B���N�g���̕ύX�Ƃ���B
int FTP::CmdCdup()				{return(this->DoCommand(2,""));}	// CDUP 1��ʂ̃f�B���N�g��(�e�f�B���N�g��)�ɖ߂�B
int FTP::CmdMkd(LPSTR path)		{return(this->DoCommand(3,path));}	// MKD �f�B���N�g���̍쐬����B
int FTP::CmdRmd(LPSTR path)		{return(this->DoCommand(4,path));}	// RMD �f�B���N�g�����폜����B
int FTP::CmdPwd()				{return(this->DoCommand(5));}	// PWD ���݂̃��[�L���O�f�B���N�g����\������B
int FTP::CmdDele(LPSTR path)	{return(this->DoCommand(10,path));}	// DELE �t�@�C���� �w�肵���t�@�C�����폜����B
int FTP::CmdRename(LPSTR path1,LPSTR path2)
{
	int ret;	// RNFR �t�@�C���� RNTO �t�@�C���� �w�肵���t�@�C�������w��t�@�C�����ɕύX�B
	ret=this->DoCommand(11,path1);
	if(ret!=0) return(ret);
	ret=this->DoCommand(12,path2);
	return(ret);
}
int FTP::CmdList()				{return(this->DoCommand(13,""));}	// [�t�@�C���^�f�B���N�g����] [�I�v�V����]
int FTP::CmdNList()				{return(this->DoCommand(14,""));}	// [�t�@�C���^�f�B���N�g����] [�I�v�V����]



//�n�r�֌W
int FTP::CmdSyst()				{return(this->DoCommand(20));}	// SYST �V�X�e������\������B
int FTP::CmdNOOP()				{return(this->DoCommand(21));}	// NOOP �������Ȃ��B�T�[�o�̉ғ����m�F���邽�߂Ɏ��s�����B��ɐ���I������B
int FTP::CmdQuit()				{return(this->DoCommand(22));}	// QUIT ���O�A�E�g����B
int FTP::CmdAllc()				{return(this->DoCommand(23));}	// ALLO �t�@�C���o�C�g�� [R ���R�[�h/�y�[�W�o�C�g��] ���炩���߃T�[�o�[��Ŏw�肵���o�C�g���̈���m�ۂ���B���R�[�h/�y�[�W�ł��w�肵�Ȃ��Ă͂Ȃ�Ȃ��ꍇ�́A�I�v�V�����Ŏw��ł���i�K�v���ǂ�����OS�Ɉˑ�����j�B
int FTP::CmdAbor()				{return(this->DoCommand(24));}	// ABOR  ���ݎ��s���̃f�[�^�]�������ׂĒ��~����B�f�[�^�R�l�N�V�����͔j�������B
int FTP::CmdSite(PSTR cmd)		{return(this->DoCommand(25,cmd));}	// SITE �R�}���h �C�ӂ�OS�R�}���h�����s����B
int FTP::CmdStat(LPSTR path)	{return(this->DoCommand(26,path));}	// STAT [�t�@�C���^�f�B���N�g����] ���݂̃V�X�e����]����Ԃ̏���\������B
int FTP::CmdRCmd(LPSTR cmd)		{return(this->DoCommand(27,cmd));}	// STAT [�t�@�C���^�f�B���N�g����] ���݂̃V�X�e����]����Ԃ̏���\������B


//�]��Ӱ�ފ֌W
int FTP::CmdTypeASCII()			{return(this->DoCommand(40,"A"));}	// TYPE A�FASCII �]���f�[�^�̌`�����w�肷��B
int FTP::CmdTypeImage()			{return(this->DoCommand(40,"I"));}	// TYPE I�FImage�i�o�C�i���j �]���f�[�^�̌`�����w�肷��B
int FTP::CmdTypeEBOCDIC()		{return(this->DoCommand(40,"E"));}	// TYPE E�FEBCDIC �]���f�[�^�̌`�����w�肷��B
int FTP::CmdTypeSJIS()			{return(this->DoCommand(40,"J"));}	// TYPE E�FSJIS �]���f�[�^�̌`�����w�肷��B
int FTP::CmdStruFile()			{return(this->DoCommand(41,"F"));}	// STRU �t�@�C���\���I�v�V���� �]������t�@�C���̃t�@�C���\�����I�v�V�����Ŏ����BF�F�t�@�C���\���i�o�C�g�u���̒ʏ�̃o�C�i���j�@�f�t�H���g�B
int FTP::CmdStruRecord()		{return(this->DoCommand(41,"R"));}	// STRU �t�@�C���\���I�v�V���� �]������t�@�C���̃t�@�C���\�����I�v�V�����Ŏ����BR�F���R�[�h�\���i�V�[�P���V�����Ȃǂ̃��R�[�h�C���[�W���������f�[�^�j
int FTP::CmdStruPage()			{return(this->DoCommand(41,"P"));}	// STRU �t�@�C���\���I�v�V���� �]������t�@�C���̃t�@�C���\�����I�v�V�����Ŏ����BP�F�y�[�W�\���i�C���f�b�N�X�����ꂽ�y�[�W�u���̃t�@�C���j
int FTP::CmdModeStream()		{return(this->DoCommand(42,"S"));}	// MODE �]�����[�h�I�v�V����  S�F�X�g���[���i�o�C�g�X�g���[���ɂ��]���j�f�t�H���g
int FTP::CmdModeBlock()			{return(this->DoCommand(42,"B"));}	// MODE �]�����[�h�I�v�V����  B�F�u���b�N�i�]�����u���b�N���Ƃɍs���j
int FTP::CmdModeCompression()	{return(this->DoCommand(42,"C"));}	// MODE �]�����[�h�I�v�V����  C�F���k�iRun-Length��@�ɂ�鈳�k�j

//�]�����ߊ֌W
int FTP::CmdRetr(LPSTR path)	{return(this->DoCommand(50,path));}	// RETR �t�@�C���� �w�肵���t�@�C���̓��e���T�[�o����擾����B
int FTP::CmdStor(LPSTR path)	{return(this->DoCommand(51,path));}	// STOR �t�@�C���� �w�肵���t�@�C�����ŁA�T�[�o�֑��M����f�[�^�Ńt�@�C�����쐬����B���ꖼ�̃t�@�C�������łɂ���ꍇ�ɂ́A�㏑������B�t�@�C�������݂��Ă��Ȃ��ꍇ�́A�V�K�쐬����B
int FTP::CmdStou(LPSTR path)	{return(this->DoCommand(52,path));}	// STOU STOR�Ɠ��l�Ɏw�肵���t�@�C�����쐬���邪�A���łɓ��ꖼ�̃t�@�C�����������ꍇ�ɂ́A�d�Ȃ�Ȃ��悤�ȃt�@�C�����������I�ɕt���č쐬����B
int FTP::CmdAppe(LPSTR path)	{return(this->DoCommand(53,path));}	// APPE �t�@�C���� �T�[�o�֑��M����f�[�^���w�肵���t�@�C���֒ǉ�����B
int FTP::CmdPasv()				{return(this->DoCommand(60));}		// PASV �p�b�V�u���[�h�ւ̈ڍs���w������B�T�[�o�̓f�[�^�R�l�N�V�����Ŏg�p����IP�A�h���X�ƃ|�[�g�ԍ���\������B



int FTP::DoCommand(int CmdNo)
{
	char buf[200];
	memset(buf,0,sizeof(buf));
	return(this->DoCommand(CmdNo," ",buf));
}

int FTP::DoCommand(int CmdNo,LPSTR theParameters1)
{
	char buf[200];
	memset(buf,0,sizeof(buf));
	return(this->DoCommand(CmdNo,theParameters1,buf));
}

int FTP::DoCommand(int CmdNo,LPSTR theParameters1,char* buf)
{
	char reply[200];
	int replyNo,nRet;
	char message[200];

	memset(reply,0,sizeof(reply));
	memset(message,0,sizeof(message));
	switch(CmdNo)
	{	
	//�f�B���N�g���[�E�t�@�C������֌W
	
		case 1:// CWD �f�B���N�g���� �w�肵���f�B���N�g�����J�����g�f�B���N�g���Ƃ���B
			sprintf(message,"CWD %s",theParameters1);	
			break;
		case 2:	// CDUP 1��ʂ̃f�B���N�g��(�e�f�B���N�g��)���J�����g�f�B���N�g���Ƃ���B
			sprintf(message,"CDUP");				
			break;
		case 3:// MKD �f�B���N�g���̍쐬����B
			sprintf(message,"MKD %s",theParameters1);	
			break;
		case 4:// RMD �f�B���N�g�����폜����B
			sprintf(message,"RMD %s",theParameters1);	
			break;
		case 5://PWD ���݂̃��[�L���O�f�B���N�g����\������B
			sprintf(message,"PWD");	
			break;
		case 10:// DELE �t�@�C���� �w�肵���t�@�C�����폜����B
			sprintf(message,"DELE %s",theParameters1);	
			break;
		case 11:// �t�@�C�����̕ύX�i�ύX�O�̖��O�j�B
			sprintf(message,"RNFR %s",theParameters1);
			break;
		case 12:// �t�@�C�����̕ύX�B�i�ύX��̖��O�j
			sprintf(message,"RNTO %s",theParameters1);
			break;
		case 13:
			sprintf(message,"LIST %s",theParameters1);//[�t�@�C���^�f�B���N�g����] [�I�v�V����]
			break;
		case 14:
			sprintf(message,"NLIST %s",theParameters1);//[�t�@�C���^�f�B���N�g����] [�I�v�V����]
			break;

//�n�r�֌W
		case 20:// SYST �V�X�e������\������B
			sprintf(message,"SYST");	
			break;
		case 21:// NOOP �������Ȃ��B�T�[�o�̉ғ����m�F���邽�߂Ɏ��s�����B��ɐ���I������B
			sprintf(message,"NOOP",theParameters1);
			break;
		case 22:// QUIT ���O�A�E�g����B
			sprintf(message,"QUIT",theParameters1);
			break;
		case 23:// ALLO �t�@�C���o�C�g�� [R ���R�[�h/�y�[�W�o�C�g��] ���炩���߃T�[�o�[��Ŏw�肵���o�C�g���̈���m�ۂ���B���R�[�h/�y�[�W�ł��w�肵�Ȃ��Ă͂Ȃ�Ȃ��ꍇ�́A�I�v�V�����Ŏw��ł���i�K�v���ǂ�����OS�Ɉˑ�����j�B
			sprintf(message,"Allc %s",theParameters1);
			break;
		case 24:// ABOR  ���ݎ��s���̃f�[�^�]�������ׂĒ��~����B�f�[�^�R�l�N�V�����͔j�������B
			sprintf(message,"ABOR %s",theParameters1);
			break;
		case 25:// SITE �R�}���h �C�ӂ�OS�R�}���h�����s����B
			sprintf(message,"SITE %s",theParameters1);
			break;
		case 26:// STAT [�t�@�C���^�f�B���N�g����] ���݂̃V�X�e����]����Ԃ̏���\������B
			sprintf(message,"STAT %s",theParameters1);
			break;
		case 27:// AS400�̃v���O���������s����B
			sprintf(message,"RCMD %s",theParameters1);
			break;
		case 40:// TYPE A�FASCII �]���f�[�^�̌`�����w�肷��B
			if     (theParameters1[0]=='A') sprintf(message,"TYPE A");
			else if(theParameters1[0]=='I')	sprintf(message,"TYPE I");
			else if(theParameters1[0]=='E')	sprintf(message,"TYPE E");
			else if(theParameters1[0]=='J')	sprintf(message,"TYPE B 1");
			else							sprintf(message,"TYPE I");
			break;
		case 41://STRU �t�@�C���\���I�v�V���� �]������t�@�C���̃t�@�C���\�����I�v�V�����Ŏ����BF�F�t�@�C���\���i�o�C�g�u���̒ʏ�̃o�C�i���j�@�f�t�H���g�B
			if     (theParameters1[0]=='F') sprintf(message,"STRU F");
			else if(theParameters1[0]=='R')	sprintf(message,"STRU R");
			else if(theParameters1[0]=='P')	sprintf(message,"STRU P");
			else							sprintf(message,"STRU F");
			break;
		case 42://MODE �]�����[�h�I�v�V����  S�F�X�g���[���i�o�C�g�X�g���[���ɂ��]���j�f�t�H���g 
			if     (theParameters1[0]=='S') sprintf(message,"MODE S");
			else if(theParameters1[0]=='B')	sprintf(message,"MODE B");
			else if(theParameters1[0]=='C')	sprintf(message,"MODE C");
			else							sprintf(message,"MODE S");
			break;

		case 50://RETR �t�@�C���� �w�肵���t�@�C���̓��e���T�[�o����擾����B 
			sprintf(message,"RETR %s",theParameters1); 
			break;
		case 51://STOR �t�@�C���� �w�肵���t�@�C�����ŁA�T�[�o�֑��M����f�[�^�Ńt�@�C�����쐬����B���ꖼ�̃t�@�C�������łɂ���ꍇ�ɂ́A�㏑������B�t�@�C�������݂��Ă��Ȃ��ꍇ�́A�V�K�쐬����B
			sprintf(message,"STOR %s",theParameters1);
			break;
		case 52://STOU STOR�Ɠ��l�Ɏw�肵���t�@�C�����쐬���邪�A���łɓ��ꖼ�̃t�@�C�����������ꍇ�ɂ́A�d�Ȃ�Ȃ��悤�ȃt�@�C�����������I�ɕt���č쐬����B
			sprintf(message,"STOU %s",theParameters1);
			break;
		case 53:// APPE �t�@�C���� �T�[�o�֑��M����f�[�^���w�肵���t�@�C���֒ǉ�����B 
			sprintf(message,"APPE %s",theParameters1);
			break;

		case 60://PASV �p�b�V�u���[�h�ւ̈ڍs���w������B�T�[�o�̓f�[�^�R�l�N�V�����Ŏg�p����IP�A�h���X�ƃ|�[�g�ԍ���\������B
			sprintf(message,"PASV");
			break;

		default:
			sprintf(message,"NOOP");
			break;
			
	}		

	memset(reply,0,sizeof(reply));	
	strcat(m_log,"FTP::");
	strcat(m_log,message);
	m_log[strlen(m_log)]=0x0d;
	m_log[strlen(m_log)+1]=0x0a;
//	strcat(m_log,"\r\n");

	nRet=this->CommandSend(message,&replyNo,reply);

	if(nRet==1) {sprintf(pbuf,"\n%s Error",message);this->Print(pbuf);return(1);}

	if(replyNo >=400 && replyNo < 600) {
		sprintf(pbuf,"%s Err ",message);
		this->Print(pbuf);
		return(1);
	}
	strcpy(buf,reply);
	return(0);
}

int FTP::CommandSend(char *message,int *replyNo,char *reply)
{
	WSABUF		Buffers;		//WSABUF�\���̔z��
	DWORD		dwBufferCount;	//lpBuffers�̎����z��ɓ����Ă���WSABUF�\���̂̐�
	DWORD		NumberOfBytesSent;//���M�o�C�g�����󂯎��DWORD�ϐ��ւ̃|�C���^
	DWORD		dwFlags;		//���M���@�𐧌䂷��r�b�g�}�X�N
	int		len;

    int nRet;
	
	char    buf[300];

	strcpy(buf,message);
	len=strlen(message);
	buf[len++]='\r';
	buf[len++]='\n';
	buf[len]=0;

 	Buffers.len=len;
	Buffers.buf=buf;
	dwBufferCount=1;
	dwFlags=MSG_DONTROUTE;		//MSG_PARTIAL;

	// �p�X�͕\�����Ȃ�
	if(memcmp(message,"PASS",4)){
		sprintf(pbuf,":%s",buf);
		this->Print(pbuf);
	}

    nRet=WSASend(fSocketC,&Buffers,dwBufferCount, &NumberOfBytesSent,dwFlags,NULL,NULL);
    if (nRet == SOCKET_ERROR)
	{
        if (WSAGetLastError() != WSAENETDOWN) return(nRet);

		nRet=WSASend(fSocketC,&Buffers,dwBufferCount, &NumberOfBytesSent,dwFlags,NULL,NULL);
        if (nRet == SOCKET_ERROR)
        {   
            return(nRet);
        }
    }
	nRet=this->Recv(reply);
	*replyNo=atol(reply);
	resCode=atoi(reply);
    return(nRet);
}

int FTP::Recv(char *answer)
{
	WSABUF		Buffers;		//WSABUF�\���̔z��
	DWORD		dwBufferCount;	//Buffers�̎����z��ɓ����Ă���WSABUF�\���̂̐�
	DWORD		NumberOfBytesRsv;//���M�o�C�g�����󂯎��DWORD�ϐ��ւ̃|�C���^
	DWORD		dwFlags;		//���M���@�𐧌䂷��r�b�g�}�X�N
	WSAEVENT	hEvents[5];		//�C�x���g�n���h���̔z��ւ̃|�C���^
	DWORD		cEvents;		//hEvents�Ɋ܂܂��C�x���g�n���h���̐�		

    int ret,nRet=0;
	WSANETWORKEVENTS events;
	int	cnt=0,i;	
	char buf[500];

	memset(buf,0,sizeof(buf));

	while(nRet==0)
	{
		hEvents[0]=fEvent;
		cEvents=1;
		nRet = WSAWaitForMultipleEvents(1,&fEvent,TRUE,20000,TRUE);		//timeOut 10000��20000 oda
		if (nRet == WSA_WAIT_TIMEOUT)
		{
			sprintf(pbuf,"\nWait timed out");
			this->Print(pbuf);
			 continue;
		}
		nRet = WSAEnumNetworkEvents(fSocketC,fEvent,&events);
		if (nRet == SOCKET_ERROR)
		{
			sprintf(pbuf,"WSAEnumNetworkEvents()");
			this->Print(pbuf);
			continue;
		}
		if((events.lNetworkEvents & FD_READ)==0)
		{
			sprintf(pbuf,"Event Err FD=%d\n",FD_READ);
			this->Print(pbuf);
		}

		Buffers.len=sizeof(buf);
		Buffers.buf=buf;
		dwBufferCount=1;
		dwFlags=0;//MSG_PEEK;//MSG_PARTIAL;

		ret=WSARecv(fSocketC,&Buffers,dwBufferCount,&NumberOfBytesRsv,&dwFlags,NULL,NULL);
		if(ret!=0)
		{
			sprintf(pbuf,"WSARecv ERROR\n");
			this->Print(pbuf);
		}
	   
		if(NumberOfBytesRsv>400) NumberOfBytesRsv=400;
		Buffers.buf[NumberOfBytesRsv]=0;
		if(cnt==0)		{strcpy(answer,Buffers.buf);cnt=strlen(answer);}
		else			{strcat(answer,Buffers.buf);}
		if( isdigit(answer[0]) && isdigit(answer[1]) && isdigit(answer[2]) && (answer[3]==' ') ) break;
		i=10;
		while(answer[i]!=NULL)
		{
			if( isdigit(answer[i-5]) && isdigit(answer[i-4]) && isdigit(answer[i-3]) && (answer[i-2]==' ') ){cnt=1; break;}
			i++;
		}
		if(cnt==1) break;
	}
	memset(pbuf,0,sizeof(pbuf));
	sprintf(pbuf,">%s",buf);
	// ******* ������ftp�̖߂�l��������
	strcat(m_log,"FTP:>");
	strcat(m_log,buf);
	
	this->Print(pbuf);
	return(nRet);
}



int FTP::CheckStatus()
{
	return(fStatus);
}
//=====================================
// int FTP::SetUser(char *user)
// FTP�T�[�o�[�̃��[�U�[��Ă���
//=====================================
int FTP::SetUser(char *user)
{
	memset(m_User,0,sizeof(m_User));
	strcpy(m_User,user);
	return(0);
}
//=====================================
// int FTP::SSetPassword(char *password)
// FTP�T�[�o�[�̃p�X���[�h��Ă���
//=====================================
int FTP::SetPassword(char *password)
{
	memset(m_Password,0,sizeof(m_Password));
	strcpy(m_Password,password);
	return(0);
}
//=====================================
// int FTP::SetServerIP(char *serveip)
// FTP�T�[�o�[��IP���ڽ��Ă���
//=====================================
int FTP::SetServerIP(char *serveip)
{
	memset(m_IPAdress,0,sizeof(m_IPAdress));
	strcpy(m_IPAdress,serveip);
	return(0);
}
//=====================================
// int FTP::SetPortNo(int port)
// FTP�T�[�o�[�̃|�[�g��Ă���
//=====================================
void FTP::SetPortNo(int port)
{
	m_PortNo=0;
	m_PortNo = port;
}

//�s�v�H
int FTP::SetHostName(char *hostname)
{
	strcpy(m_Host,hostname);
	return(0);
}

int  FTP::Print(char *buf)
{
	if( m_printMode == TRUE )
		printf("FTP:%s",buf);
	return(0);
}