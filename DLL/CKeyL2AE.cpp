//================================================================================================
//	FileName	:CKeyL2AE.h
//	Class		:CKeyL20AE
// 				:Keyence KV-LE20A �`���ʐM(��ʃ����N�ʐM
//	�쐬��		:2007.02.17 coba
//================================================================================================
#include <stdio.h>
#include <conio.h>
#include "CKeyL2AE.h"

#include <string>
using namespace std;
//========================================================================================================
//	CKeyL2AE::CKeyL2AE(void)
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F�R���X�g���N�^�B�f�t�H���g�T�C�Y�Ńf�[�^�o�b�t�@���쐬
//========================================================================================================
CKeyL2AE::CKeyL2AE(void)
{
	m_fOpen=FALSE;
}
//========================================================================================================
//	CKeyL2AE::~CKeyL2AE()
//	�f�X�g���N�^
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F
//========================================================================================================
CKeyL2AE::~CKeyL2AE()
{
	WSACleanup();
}
//========================================================================================================
//	int CKeyL2AE::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type�@�Fprivate
//�@�@�\�@�FWinSock������
//========================================================================================================
BOOL CKeyL2AE::Init(void)
{
	// ����ݒ���OK
	if( m_fOpen ) return(TRUE);

  	WSADATA			WSAData;

	// �\�P�b�g������
  	if(WSAStartup( MAKEWORD(2,2), &WSAData)) return(-1);

	m_fOpen=TRUE;

	return(TRUE);
}
//========================================================================================================
//	int CKeyL2AE::L2ARead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
//	type�@�Fpublic
//�@�@�\�@�FKeyence�@��ʌ݊��ʐM
//	�����P�F�^�[�Q�b�g��IP�A�h���X
//	�����Q�F�^�[�Q�b�g��UDP�|�[�g�ԍ�
//	�����R�F��M�o�b�t�@
//	�����S�F��M�o�b�t�@�T�C�Y
//	�����T�F�f�o�C�X�̎��(0=���[�h�f�o�C�X�A1=�r�b�g�f�o�C�X)
//	�����T�F�ǂݏo���擪�A�h���X�@Device + address �Œ�̌����Ȃ�  
//	�����U�F��M�u���b�N��(Max�F120)
//========================================================================================================
int CKeyL2AE::L2ARead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
{

	// ���M�o�b�t�@
	char send[5120];
	int ret;

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 120 ) return(-2);	//���̓ǂݏo����120

	// IP�A�h���X
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// �|�[�g�ԍ�
	if(port <1024 || port >65535) return(-5);

	memset( send, 0, sizeof(send) );

	// �@�Œ蕔
	// �A���ǂݏo���R�}���h"RDE"
	strcpy( send,"RDE ");
	
	// �A�f�o�C�X�@+ ���̃A�h���X
	//DM(�f�[�^������),MR(���������[)
	strcat( send, devaddr );
	
	// �Bsuffix
	if( devtype==DEV_BIT)
		strcat( send,".U");	// �r�b�g�f�o�C�X
	strcat( send, " " );	

	// �B�ǂݏo����
	sprintf( &send[strlen(send)],"%d",counts);

	//�I�� cr
	strcat( send, "\r" );	

	// ���M����
	ret=UdpSend(ipaddr,port,send,strlen(send),buf,bufsize);
	
	char buf2[500]={0};
	char buf3[20]={0};
	for(int i=0 ; i< counts ; i++){
		memcpy(buf3, buf+(6*i),5 );
		buf3[5]=0;
		sprintf( buf2+strlen(buf2), "%04X", atoi(buf3) );
	}

	strcpy(buf,buf2);

	return(ret);
}

//========================================================================================================
//	int CKeyL2AE::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
//	type�@�Fpublic
//�@�@�\�@�F��ʌ݊��ʐM�Ńf�o�C�X�f�[�^���u���b�N���M
//	�����P�F�^�[�Q�b�g��IP�A�h���X
//	�����Q�F�^�[�Q�b�g��UDP�|�[�g�ԍ�
//	�����R�F�f�o�C�X�̎��(0=���[�h�f�o�C�X�A1=�r�b�g�f�o�C�X)
//	�����S�F�������ݐ擪�A�h���X(8��)
//	�����T�F���M�u���b�N��(Max�F120)�@��M�o�b�t�@
//	�����U�F�������݃f�[�^(�����̏ꍇ��ASCII�R�[�h 2�������i2�~2��4���j�A���l�̏ꍇ��BCD)
//========================================================================================================
int CKeyL2AE::L2AWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
{
	// ���M�o�b�t�@
	char send[5120];
	int ret;
	char buf[40+1];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);

	if( counts <0 || counts > 120 ) return(-2);	//���̓ǂݏo����120

	// IP�A�h���X
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// �|�[�g�ԍ�
	if(port <1024 || port >65535) return(-5);

	// ���M�f�[�^��
	if( devtype==DEV_WORD){
		if( (short)strlen(data) !=counts*4) return(-6);
	}
	else{
		if( (short)strlen(data) !=counts) return(-7);
	}
	memset( send, 0, sizeof(send) );

	// �@�Œ蕔
	// �A���������݃R�}���h"WDE"
	strcpy( send,"WRE ");
	
	// �A�f�o�C�X�@+ ���̃A�h���X
	//DM(�f�[�^������),MR(���������[)
	strcat( send, devaddr );
	
	// �Bsuffix
	if( devtype==DEV_BIT)
		strcat( send,".U");	// �r�b�g�f�o�C�X
	strcat( send, " " );	

	// �B�ǂݏo����
	sprintf( &send[strlen(send)],"%d",counts);
	
	// �C�������݃f�[�^
	for(int i=0; i< counts;i++){
		strcat( send, " " );	
/*	
		if(i%2 == 0)
			strcat( send, " " );	

		memset( buf,0,sizeof(buf) );
		memcpy( buf, data+i, 1 );
		char ch;
		ch = buf[0]; 
		sprintf(buf,"%d",ch);  //ASCII code

		memcpy( send+strlen(send), buf, 2 );	
*/

		memset( buf,0,sizeof(buf) );
		memcpy( buf, data+(i*4), 4 );

		int data =0 ;
				
		data = htoi( buf , 4 );
		sprintf(buf,"%d",data);
		memcpy( send+strlen(send), buf, strlen(buf) );	

	}

	//�I�� cr
	strcat( send, "\r" );	

	// ���M����
	ret=UdpSend(ipaddr,port,send,strlen(send),buf,sizeof(buf));

	return(ret);
}


//========================================================================================================
//	int CKeyL2AE::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
//	type�@�Fpublic
//�@�@�\�@�F��ʌ݊��ʐM�Ńf�o�C�X�f�[�^���u���b�N���M
//	�����P�F�^�[�Q�b�g��IP�A�h���X
//	�����Q�F�^�[�Q�b�g��UDP�|�[�g�ԍ�
//	�����R�F�f�o�C�X�̎��(0=���[�h�f�o�C�X�A1=�r�b�g�f�o�C�X)
//	�����S�F�������ݐ擪�A�h���X(8��)
//	�����T�F���M�u���b�N��(Max�F120)�@��M�o�b�t�@
//	�����U�F�������݃f�[�^(�����̏ꍇ��ASCII�R�[�h 2�������i2�~2��4���j�A���l�̏ꍇ��BCD)
//========================================================================================================
int CKeyL2AE::L2ASetBit(char* ipaddr,unsigned short port,char* devaddr,BOOL on )
{
	// ���M�o�b�t�@
	char send[128];
	int ret;
	char buf[64];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);

	// IP�A�h���X
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// �|�[�g�ԍ�
	if(port <1024 || port >65535) return(-5);

	memset( send, 0, sizeof(send) );

	// �@�Œ蕔
	// SET or RESET
	if( on ) strcpy( send,"ST ");
	else strcpy( send,"RS ");

	// �A�f�o�C�X�@+ ���̃A�h���X
	strcat( send, devaddr );

	//�I�� cr
	strcat( send, "\r" );	

	// ���M����
	ret=UdpSend(ipaddr,port,send,strlen(send),buf,sizeof(buf));
	if(ret) return(-1);
	if( memcmp( buf,"OK",2)) return(1);

	return(ret);
}

int  CKeyL2AE::GetPlcType(char* ipaddr,unsigned short port,char* recvBuf)
{
	string stbuf;
	int ret=0;

	//"?K"�Ō^���Q�b�g
	stbuf = "?K\r";

	// ���M����
	ret=UdpSend(ipaddr,port,(char*)stbuf.c_str() ,stbuf.length() ,recvBuf,3);

	return ret;
}
//========================================================================================================
//	int CKeyL2AE::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
//	type�@�Fprivate
//�@�@�\�@�FUDP����M�i�f�[�^�t���[����data�̓��e���邾���j
//	�����P�F�^�[�Q�b�g��IP�A�h���X
//	�����Q�F�^�[�Q�b�g��UDP�|�[�g�ԍ�
//	�����R�F���M�f�[�^
//	�����S�F���M�f�[�^�T�C�Y
//	�����T�F��M�o�b�t�@
//	�����U�F��M�o�b�t�@�T�C�Y
//	�߂�l�F0=OK,0����=�l�b�g���[�N�G���[�A1=��M�f�[�^�G���[
//========================================================================================================
int CKeyL2AE::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
{
	SOCKET fSocket;
	WSAEVENT fEvent;
	//DWORD		cEvents;		//hEvents�Ɋ܂܂��C�x���g�n���h���̐�
	unsigned long laddr;
	WSABUF		wsBuf;			//WSABUF�\���̔z��
	DWORD		dwBufferCount;	//lpwsBuf�̎����z��ɓ����Ă���WSABUF�\���̂̐�
	DWORD		cnt;			//���M�o�C�g�����󂯎��DWORD�ϐ��ւ̃|�C���^
	DWORD		dwFlags;		//���M���@�𐧌䂷��r�b�g�}�X�N
	struct sockaddr_in	addr;
	int ret;
	WSANETWORKEVENTS events;
	int			iFromlen;		//lpTo�̎���sockaddr�\���̂̒������w�肷�鐮��
	char rbuf[4096];

	// IP���ڽ�ϊ�(to 32bit)
	if( INADDR_NONE ==( laddr=inet_addr(ipaddr)) ) return(-101);

	fSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	fSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (fSocket == INVALID_SOCKET){
		printf("socket()Err"); 
		return(-102);
	}
	fEvent = WSACreateEvent();
	if(fEvent == WSA_INVALID_EVENT){
		printf("\nWSACreateEvent()Err: %d\n", fEvent);
		closesocket(fSocket);
		return(-103);
	}

	ret = WSAEventSelect(fSocket,fEvent,FD_READ);
	if(ret == SOCKET_ERROR){
		printf("\nWSAEventSelect()Err: %d\n", ret);
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-104);
	}

	// ���M�f�[�^�쐬
	wsBuf.len=size;
	wsBuf.buf=data;
	dwBufferCount=1;
	dwFlags=MSG_DONTROUTE;//MSG_PARTIAL;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family			= AF_INET;
	addr.sin_addr.s_addr	= laddr;
	addr.sin_port			= htons(port);

	// ���M
	ret=WSASendTo(fSocket,&wsBuf,dwBufferCount, &cnt,dwFlags,
					(LPSOCKADDR)&addr,sizeof(addr),NULL,NULL);
	if (ret == SOCKET_ERROR){
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-105);
	}

	// ��M�҂�
	ret = WSAWaitForMultipleEvents(1,&fEvent,TRUE,10000,TRUE);
	if(ret == WSA_WAIT_TIMEOUT){
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-106);
	}

	ret = WSAEnumNetworkEvents(fSocket,fEvent,&events);
	if(ret == SOCKET_ERROR){
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-107);
	}

	// ��M�C�x���g�ȊO
	if((events.lNetworkEvents & FD_READ)==0){
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-108);
	}

	// ��M����
	//printf("Event Err FD=%d\n",FD_READ);
	memset( rbuf,0,sizeof(rbuf));
	wsBuf.len=sizeof(rbuf);
	wsBuf.buf=rbuf;
	dwBufferCount=1;
	dwFlags=0;//MSG_PEEK;//MSG_PARTIAL;
	memset(&addr, 0, sizeof(addr));
	iFromlen=sizeof(addr);

	ret=WSARecvFrom(fSocket,&wsBuf,dwBufferCount, &cnt,&dwFlags,
							(LPSOCKADDR)&addr, &iFromlen,NULL,NULL);

	WSACloseEvent(fEvent);
	closesocket(fSocket);

	// ��M�f�[�^�R�s�[
	if( strlen(rbuf) > 0 ){
		if( rsize  > strlen(rbuf)+1 )
			strcpy(rcv,rbuf);
		else
			memcpy(rcv,rbuf,rsize);
	}

	return(0);
}

//========================================================================================================
//	int CKeyL2AE::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	�@�\�@�F��������AMelsec���M�`���ɕϊ�����
//			ex) "A1234"->"31413433"
//	�����P�F�o�̓o�b�t�@
//	�����Q�F�o�b�t�@�T�C�Y�i�ϊ����f�[�^���̂Q�{+1�͕K�v�j
//	�����R�F�ϊ���������f�[�^(NULL-terminated)
//	�Ԓl�@�F0=OK
//========================================================================================================
int CKeyL2AE::Str2Mel(char* buf, unsigned int bufsize, char* str)
{
	unsigned int i,len;
	char txt[10];

	len = strlen(str);
	if( len*2 >= bufsize ) return(-1);

	memset( buf, 0, bufsize );

	for( i=0;i<len;i++){
		// �㉺�����ւ���
		if( i+1==len )
			wsprintf( txt,"00");
		else
			wsprintf( txt,"%02X",(unsigned char)str[i+1]);	// 2011.03.11 coba
//			wsprintf( txt,"%02X",str[i+1]);
		memcpy( &buf[i*2], txt, 2 );

		wsprintf( txt,"%02X",(unsigned char)str[i]);		// 2011.03.11 coba
//		wsprintf( txt,"%02X",str[i]);
		i++;
		memcpy( &buf[i*2], txt, 2 );
	}

	return(0);

}

//========================================================================================================
//	unsigned char CKeyL2AE::htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
int CKeyL2AE::Mel2Str(char* buf, unsigned int bufsize, char* melstr)
{
	unsigned int i,len;

	len = strlen(melstr);
	if( bufsize<=len/2 ) return(-1);

	memset( buf, 0, bufsize );

	for( i=0;i<len;i+=2){
		// �㉺�����ւ���
		if( i+2>=len )
			buf[strlen(buf)]=0x20;
		else
			buf[strlen(buf)]=(char)htoi( &melstr[i+2],2);

		buf[strlen(buf)]=(char)htoi( &melstr[i],2);
		i+=2;
	}
	return(0);
}

//========================================================================================================
//	unsigned char CKeyL2AE::htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
unsigned short CKeyL2AE::htoi( char *hexstr , short len )
{
	unsigned short ret = 0;
	short i;

	if( len > 4 ) return 0;
	for( i = 0 ; i < len ; i++ ){
		ret *= 16;
		if( hexstr[i] < 'A' ) ret += hexstr[i] - '0';
		else
		if( hexstr[i] < 'a' ) ret += hexstr[i] - 'A'+10;
		else
			ret += hexstr[i] - 'a'+10;
	}
	return(ret);
}


BOOL CKeyL2AE::ULChg( char* buf )
{
	char tmp[8];

	if( strlen(buf)<8 )
		return FALSE;

	memcpy( tmp , buf , 8);
	memcpy( buf+0 , tmp+4 , 4 );
	memcpy( buf+4 , tmp+0 , 4 );
	
	return TRUE;
}
