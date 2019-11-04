//================================================================================================
//	FileName	:CMelP3E.h
//	Class		:CMelsecP1E
// 				:Melsec �v���g�R��3E�`���ʐM(QnA�݊�3E�t���[��)
//	�쐬��		:2007.02.17 coba
//================================================================================================
#include <stdio.h>
#include <conio.h>
#include "CMelP1E.h"

//========================================================================================================
//	CMelsecP1E::CMelsecP1E(void)
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F�R���X�g���N�^�B�f�t�H���g�T�C�Y�Ńf�[�^�o�b�t�@���쐬
//========================================================================================================
CMelsecP1E::CMelsecP1E(void)
{
	m_fOpen=FALSE;
	m_tcp=FALSE;	// 2012.12.21
}

//========================================================================================================
//	CMelsecP1E::~CMelsecP1E()
//	�f�X�g���N�^
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F
//========================================================================================================
CMelsecP1E::~CMelsecP1E()
{
	WSACleanup();
}

//========================================================================================================
//	int CMelsecP1E::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type�@�Fprivate
//�@�@�\�@�FWinSock������
//	�����P�FTRUE=TCP�ʐM,FALSE(�f�t�H���g)=UDP
//========================================================================================================
BOOL CMelsecP1E::Init(BOOL tcp /*=FALSE*/)
{
	// ����ݒ���OK
	if( m_fOpen ) return(TRUE);

  	WSADATA			WSAData;

	// �\�P�b�g������
  	if(WSAStartup( MAKEWORD(2,2), &WSAData)) return(-1);

	m_fOpen=TRUE;

	// 2012.12.21
	if(tcp) m_tcp=TRUE;
	else m_tcp=FALSE;

	return(TRUE);
}


//========================================================================================================
//	int CMelsecP1E::P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
//	type�@�Fpublic
//�@�@�\�@�F�v���g�R��3E�`���Ńf�o�C�X�f�[�^���u���b�N��M
//	�����P�F�^�[�Q�b�g��IP�A�h���X
//	�����Q�F�^�[�Q�b�g��UDP�|�[�g�ԍ�
//	�����R�F��M�o�b�t�@
//	�����S�F��M�o�b�t�@�T�C�Y
//	�����T�F�f�o�C�X�̎��(0=���[�h�f�o�C�X�A1=�r�b�g�f�o�C�X)
//	�����T�F�ǂݏo���擪�A�h���X(8��)
//	�����U�F��M�u���b�N��(max256)
//========================================================================================================
int CMelsecP1E::P1ERead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devname,int devaddr, short counts)
{

	// ���M�o�b�t�@
	char send[4096];
	int ret;
	char buf1[30];
	unsigned char buf2[30];
	unsigned long ldat;


	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 256 ) return(-2);

	// IP�A�h���X
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// �|�[�g�ԍ�
	if(port <1024 || port >65535) return(-5);

	memset( send, 0, sizeof(send) );

	// �@�T�u�w�b�_(�R�}���h�ԍ�)+PC�ԍ�(FF�Œ�)
	if( devtype==DEV_WORD) strcpy( send,"01FF");
	else strcpy( send,"00FF");

	// �A�Ď��^�C�}(000A���g�p)
	memcpy( send+4,"000A",4);

	// �B�擪�f�o�C�X��
	memset( buf1,0x20,2);
	memcpy( buf1,devname,strlen(devname) > 2 ? 2:strlen(devname));
	sprintf( &send[strlen(send)],"%02X%02X",buf1[0],buf1[1]);

	// �C�擪�f�o�C�X long�^�ɕϊ����A1�o�C�g�Â����񉻂���B�����ɒ���
	ldat=(unsigned long)devaddr;	// ldat�̊i�[�� �� LL LH HL HH (L:Low,H:High)
	memcpy( buf2,&ldat,4);
	sprintf( &send[strlen(send)],"%02X%02X%02X%02X",buf2[3],buf2[2],buf2[1],buf2[0]);

	// �D�f�[�^�_��
	sprintf( &send[strlen(send)],"%02X",counts);
	// �E�^�[�~�l�[�^
	memcpy( &send[strlen(send)],"00",2);
	// 2012.12.21 coba TCP�ʐM��ǉ�
	if(m_tcp)ret=TcpSend(ipaddr,port,send,strlen(send),buf,bufsize);
	else ret=UdpSend(ipaddr,port,send,strlen(send),buf,bufsize);

	return(ret);
}

//========================================================================================================
//	int CMelsecP1E::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
//	type�@�Fpublic
//�@�@�\�@�F�v���g�R��3E�`���Ńf�o�C�X�f�[�^���u���b�N���M
//	�����P�F�^�[�Q�b�g��IP�A�h���X
//	�����Q�F�^�[�Q�b�g��UDP�|�[�g�ԍ�
//	�����R�F��M�o�b�t�@
//	�����S�F��M�o�b�t�@�T�C�Y
//	�����T�F�f�o�C�X�̎��(0=���[�h�f�o�C�X�A1=�r�b�g�f�o�C�X)
//	�����T�F�������ݐ擪�A�h���X(8��)
//	�����U�F���M�u���b�N��(max256)
//	�����V�F�������݃f�[�^
//========================================================================================================
int CMelsecP1E::P1EWrite(char* ipaddr,unsigned short port,int devtype,char* devname,int devaddr, short counts, char* data)
{
	// ���M�o�b�t�@
	char send[4096];
	int ret;
	char buf[40+1];
	unsigned char buf2[30];
	unsigned long ldat;

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 256 ) return(-2);

	// IP�A�h���X
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// �|�[�g�ԍ�
	if(port <1024 || port >65535) return(-5);

	memset( send, 0, sizeof(send) );

	// �@�T�u�w�b�_(�R�}���h�ԍ�)+PC�ԍ�(FF�Œ�)
	if( devtype==DEV_WORD) strcpy( send,"03FF");
	else strcpy( send,"02FF");

	// �A�Ď��^�C�}(000A���g�p)
	memcpy( send+4,"000A",4);

	// �B�擪�f�o�C�X��
	memset( buf,0x20,2);
	memcpy( buf,devname,strlen(devname) > 2 ? 2:strlen(devname));
	sprintf( &send[strlen(send)],"%02X%02X",buf[0],buf[1]);

	// �C�擪�f�o�C�X long�^�ɕϊ����A1�o�C�g�Â����񉻂���B�����ɒ���
	ldat=(unsigned long)devaddr;	// ldat�̊i�[�� �� LL LH HL HH (L:Low,H:High)
	memcpy( buf2,&ldat,4);
	sprintf( &send[strlen(send)],"%02X%02X%02X%02X",buf2[3],buf2[2],buf2[1],buf2[0]);

	// �D�f�[�^�_��
	sprintf( &send[strlen(send)],"%02X",counts);
	// �E�^�[�~�l�[�^
	memcpy( &send[strlen(send)],"00",2);

	// �F���M�f�[�^
	if( devtype==DEV_WORD){
		memcpy( &send[strlen(send)],data,counts*4);
	}
	else{
		memcpy( &send[strlen(send)],data,counts);
		// ��̏ꍇ�̓_�~�[��t��
		if( counts % 2 )
			memcpy( &send[strlen(send)],"0",1);
	}
//printf("\nsend=%s",send);

	// ���M����
	// 2012.12.21 coba TCP�ʐM��ǉ�
	if(m_tcp)ret=TcpSend(ipaddr,port,send,strlen(send),buf,sizeof(buf));
	else ret=UdpSend(ipaddr,port,send,strlen(send),buf,sizeof(buf));

	return(ret);
}

//========================================================================================================
//	2012.12.21 coba�ǉ�
//	int CMelsecP1E::TcpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
//	type�@�Fprivate
//�@�@�\�@�FUDP����M
//	�����P�F�^�[�Q�b�g��IP�A�h���X
//	�����Q�F�^�[�Q�b�g��UDP�|�[�g�ԍ�
//	�����R�F���M�f�[�^
//	�����S�F���M�f�[�^�T�C�Y
//	�����T�F��M�o�b�t�@
//	�����U�F��M�o�b�t�@�T�C�Y
//	�߂�l�F0=OK,0����=�l�b�g���[�N�G���[�A1=��M�f�[�^�G���[
//========================================================================================================
int CMelsecP1E::TcpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
{
	unsigned long laddr;
	SOCKET soc;
	struct sockaddr_in rmthost;
	int ret;
	unsigned long arg;
	int len,len2,dlen;
	int wait_cnt,chk;
	fd_set r_socket;
  	struct timeval      time;
	char rbuf[4096];

	//2008.04.25 ���� �������`�ŊJ�n����悤�ǉ�
//	WSADATA WSAData;
//	WSAStartup( MAKEWORD(2,2), &WSAData);

	// IP���ڽ�ϊ�(to 32bit)
	if( INADDR_NONE ==( laddr=inet_addr(ipaddr)) )return(-101);

	// ���M�T�C�Y���`�F�b�N
	if( size < 1 ) return(-103);

	soc = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if(soc ==INVALID_SOCKET ) return(-104);

	// �z�X�g�\�P�b�g�\���̓��̓f�[�^�쐬
	// �ڑ���i�����[�g�z�X�g�j�̃A�h���X��sockaddr�\���̂Ɋi�[
	memset( &rmthost, 0, sizeof(rmthost) );			// �[���N���A
	rmthost.sin_family = AF_INET;						// PF_INET
	rmthost.sin_port = htons((unsigned short)port);		// �|�[�g�ԍ�
//	rmthost.sin_addr.s_addr = htonl(laddr);				// IP�A�h���X
	rmthost.sin_addr.s_addr = laddr;				// IP�A�h���X

	ret = connect( soc, (struct sockaddr*)&rmthost, sizeof(struct sockaddr) );
	if(ret==SOCKET_ERROR){
		closesocket(soc);
//	swprintf( cData,TEXT("WLAN=port=%d"),WSAGetLastError());
		Sleep(30);	// �E�G�C�g
		return -105;										// �R�l�N�g���s
	}


	arg = 0;
	if( (ioctlsocket( soc, FIONBIO, &arg) ) != 0 ) {
		closesocket(soc);
		Sleep(30);	// �E�G�C�g
		return -106;										// ioctl���s
	}


   	FD_ZERO( &r_socket );
	FD_SET( soc, &r_socket );
//    time.tv_sec  = 3;  /* ��ѱ��1�b */
    time.tv_sec  = 2000;	//���M�^�C���A�E�g��ݒ�ł���悤�ύX 2008.07.01 ����
    time.tv_usec = 0;
    ret = select( 1, NULL,&r_socket, NULL, &time );
	if(ret==SOCKET_ERROR){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// �E�G�C�g
		return -107;										// �f�[�^���M���s
	}
   	else if( ret == 0 ){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// �E�G�C�g
		return -108;										// �f�[�^���M���s
   	}
  	else if( FD_ISSET( soc, &r_socket ) == 0 ){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// �E�G�C�g
		return -109;										// �f�[�^���M���s
	}


	len = size;
	len2 = 0;
	wait_cnt=0;
	do{
		ret = send( soc, &data[len2], len-len2, 0 );
		if( ret ==SOCKET_ERROR ) {
			shutdown(soc, 2);
			closesocket(soc);
			Sleep(30);	// �E�G�C�g
			return -110;										// �f�[�^���M���s
		}
		// ���M�ς݃o�C�g��
		len2 += ret;
		if( len !=len2 ){
			if( ++wait_cnt > 4000 ){
				shutdown(soc, 2);
				closesocket(soc);
				Sleep(30);	// �E�G�C�g
				return -111;										// �f�[�^���M���s
			}
			Sleep(5);
		}
		// wait timer�����Z�b�g 2005.11.03
		else wait_cnt=0;
	}while( len !=len2);				// ���肫��܂�


   	FD_ZERO( &r_socket );
	FD_SET( soc, &r_socket );
//    time.tv_sec  = 7;  /* ��ѱ��1�b */
//    time.tv_sec  = 15;  // ��ѱ��15�b�ɕύX 2006.09.19 matsuda
    time.tv_sec  = 2000;  // ��M�^�C���A�E�g��ݒ�ł���悤�ύX 2008.07.01 ����
    time.tv_usec = 0;
    ret = select( 1, &r_socket, NULL, NULL, &time );
	if(ret==SOCKET_ERROR){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// �E�G�C�g
		return -112;										// �f�[�^���M���s
	}
   	else if( ret == 0 ){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// �E�G�C�g
		return -113;										// �f�[�^���M���s
   	}
  	else if( FD_ISSET( soc, &r_socket ) == 0 ){
		shutdown(soc, 2);
		closesocket(soc);
		Sleep(30);	// �E�G�C�g
		return -114;										// �f�[�^���M���s
	}

	// 2011.08.30 coba ��͂胋�[�v�K�v
	len =0,len2=0,wait_cnt=0,dlen=0,chk=0;
	memset(rbuf,0,sizeof(rbuf));
	while(1){
		len = recv(soc, &rbuf[len2], rsize-len2, 0);
		if(len == SOCKET_ERROR){
		//if(len < 1 || len == SOCKET_ERROR){
			shutdown(soc, 2);
			closesocket(soc);
			return -115;										// �f�[�^���M���s
		}
		if(len > 0 ){
			len2 +=len;
			break;
		}

		//�R�b�҂��ă_���Ȃ�^�C�����
		if(len < 1) wait_cnt++;
		else wait_cnt=0;
		if(wait_cnt > 2000){
			shutdown(soc, 2);						//
			closesocket(soc);
			Sleep(30);	// �E�G�C�g
			return(-116);
		}
		Sleep(5);
	}

	shutdown(soc, 2);							// 2:SHUT_RDWR
	closesocket(soc);
	//2008.04.25 ���� �������`�ŏI������悤�ǉ�
	//WSACleanup();

	// �I���R�[�h���`�F�b�N(�擪����2�o�C�g)
	memset( rcv,0,rsize);
	if( memcmp( rbuf+2,"00",2)){
//		printf("\nlen=%d,rcv=%s",strlen(rbuf),rbuf);
		// �I���R�[�h���o�͂��ďI��
		memcpy( rcv,rbuf+2,4);
		Sleep(30);	// �E�G�C�g
		return(1);
	}

	ret=strlen(rbuf+4);

	// ��M�f�[�^�R�s�[
	if(ret>0){
		memcpy( rcv, rbuf+4, ret> rsize ? rsize:ret);
	}
	Sleep(30);	// �E�G�C�g
	return(0);
}
int CMelsecP1E::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
{
	SOCKET fSocket;
	WSAEVENT fEvent;
	//DWORD		cEvents;		//hEvents�Ɋ܂܂��C�x���g�n���h���̐�
	unsigned long laddr;
	WSABUF		wsBuf;		//WSABUF�\���̔z��
	DWORD		dwBufferCount;	//lpwsBuf�̎����z��ɓ����Ă���WSABUF�\���̂̐�
	DWORD		cnt;//���M�o�C�g�����󂯎��DWORD�ϐ��ւ̃|�C���^
	DWORD		dwFlags;		//���M���@�𐧌䂷��r�b�g�}�X�N
	struct sockaddr_in	addr;
	int ret;
	WSANETWORKEVENTS events;
	int			iFromlen;//lpTo�̎���sockaddr�\���̂̒������w�肷�鐮��
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
	//ret = WSAWaitForMultipleEvents(1,&fEvent,FALSE,10000,FALSE);
	//ret = WSAWaitForMultipleEvents(2,&fEvent,TRUE,10000,TRUE);
	ret = WSAWaitForMultipleEvents(1,&fEvent,TRUE,10000,TRUE);	//2013.01.11 kobashi
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
//printf("\nrbuf=%s",rbuf);
	// �I���R�[�h���`�F�b�N(�擪����2�o�C�g)
	if( memcmp( rbuf+2,"00",2)){
//		printf("\nlen=%d,rcv=%s",strlen(rbuf),rbuf);
		// �I���R�[�h���o�͂��ďI��
		memcpy( rcv,rbuf+2,4);
		return(1);
	}

	ret=strlen(rbuf+4);

	// ��M�f�[�^�R�s�[
	if(ret>0){
		memset( rcv,0,rsize);
		memcpy( rcv, rbuf+4, ret> rsize ? rsize:ret);
	}

	return(0);
}

//========================================================================================================
//	int CMelsecP1E::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	�@�\�@�F��������AMelsec���M�`���ɕϊ�����
//			ex) "A1234"->"31413433"
//	�����P�F�o�̓o�b�t�@
//	�����Q�F�o�b�t�@�T�C�Y�i�ϊ����f�[�^���̂Q�{+1�͕K�v�j
//	�����R�F�ϊ���������f�[�^(NULL-terminated)
//	�Ԓl�@�F0=OK
//========================================================================================================
int CMelsecP1E::Str2Mel(char* buf, unsigned int bufsize, char* str)
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
//	unsigned char CMelsecP1E::htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
int CMelsecP1E::Mel2Str(char* buf, unsigned int bufsize, char* melstr)
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
			buf[strlen(buf)]=htoi( &melstr[i+2],2);

		buf[strlen(buf)]=htoi( &melstr[i],2);
		i+=2;
	}
	return(0);
}

//========================================================================================================
//	unsigned char CMelsecP1E::htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
unsigned char CMelsecP1E::htoi( char *hexstr , short len )
{
	unsigned char ret = 0;
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


BOOL CMelsecP1E::ULChg( char* buf )
{
	char tmp[8];

	if( strlen(buf)<8 )
		return FALSE;

	memcpy( tmp , buf , 8);
	memcpy( buf+0 , tmp+4 , 4 );
	memcpy( buf+4 , tmp+0 , 4 );
	
	return TRUE;
}
