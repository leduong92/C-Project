//================================================================================================
//	FileName	:CMelP3E.h
//	Class		:CMelsecP3E
// 				:Melsec �v���g�R��3E�`���ʐM(QnA�݊�3E�t���[��)
//	�쐬��		:2007.02.17 coba
//================================================================================================
#include <stdio.h>
#include <conio.h>
#include "CMelP3E.h"

//========================================================================================================
//	CMelsecP3E::CMelsecP3E(void)
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F�R���X�g���N�^�B�f�t�H���g�T�C�Y�Ńf�[�^�o�b�t�@���쐬
//========================================================================================================
CMelsecP3E::CMelsecP3E(void)
{
	m_fOpen=FALSE;
}

//========================================================================================================
//	CMelsecP3E::~CMelsecP3E()
//	�f�X�g���N�^
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F
//========================================================================================================
CMelsecP3E::~CMelsecP3E()
{
	WSACleanup();
}

//========================================================================================================
//	int CMelsecP3E::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type�@�Fprivate
//�@�@�\�@�FWinSock������
//========================================================================================================
BOOL CMelsecP3E::Init(void)
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
//	int CMelsecP3E::P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
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
int CMelsecP3E::P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
{

	// ���M�o�b�t�@
	char send[5120];
	int ret;

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 960 ) return(-2);

	// ���ڽ(�f�o�C�X�R�[�h(2)+�擪�f�o�C�X(6))
	if( strlen(devaddr) !=8) return(-3);

	// IP�A�h���X
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// �|�[�g�ԍ�
	if(port <1024 || port >65535) return(-5);

	memset( send, 0, sizeof(send) );

	// �@�Œ蕔
	// �T�u�w�b�_"5000"+NW�ԍ�"00"+PC�ԍ�"FF"+�v����Unit#"03FF"+�v���ǔ�"00"
	strcpy( send,"500000FF03FF00");

	// �A���M�f�[�^�����Z�o����=�ǂݏo���Ȃ̂ŌŒ蒷(�Ď��^�C�}�`�ǂݏo���_��)
	sprintf( &send[strlen(send)],"%04X",P3E_FIX_DATALEN);


	// �BCPU�Ď��^�C�}(4)+�R�}���h(4)+�T�u�R�}���h(4)
	if( devtype==DEV_WORD)
		strcat( send,"001004010000");	// ���[�h�f�o�C�X
	else 
		strcat( send,"001004010001");	// �r�b�g�f�o�C�X

	// �C�ǂݏo�����ڽ(8)
	strcat( send,devaddr);

	// �D�ǂݏo���_��
	sprintf( &send[strlen(send)],"%04X",counts);
	// ���M����
	ret=UdpSend(ipaddr,port,send,strlen(send),buf,bufsize);

	return(ret);
}

//========================================================================================================
//	int CMelsecP3E::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
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
int CMelsecP3E::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
{
	// ���M�o�b�t�@
	char send[5120];
	int ret;
	char buf[40+1];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 960 ) return(-2);

	// ���ڽ(�f�o�C�X�R�[�h(2)+�擪�f�o�C�X(6))
	if( strlen(devaddr) !=8) return(-3);

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
	// �T�u�w�b�_"5000"+NW�ԍ�"00"+PC�ԍ�"FF"+�v����Unit#"03FF"+�v���ǔ�"00"
	strcpy( send,"500000FF03FF00");


	// ���[�h�f�o�C�X
	if( devtype==DEV_WORD){
		// �A���M�f�[�^�����Z�o����=(�Œ�:�Ď��^�C�}�`�ǂݏo���_��)+�f�o�C�X�_���~4(�o�C�g)
		sprintf( &send[strlen(send)],"%04X",P3E_FIX_DATALEN+counts*4);
		// �BCPU�Ď��^�C�}(4)+�R�}���h(4)+�T�u�R�}���h(4)
		strcat( send,"001014010000");	// ���[�h�f�o�C�X
	}
	// �r�b�g�f�o�C�X
	else{
		// �A���M�f�[�^�����Z�o����=(�Œ�:�Ď��^�C�}�`�ǂݏo���_��)+�f�o�C�X�_��
		sprintf( &send[strlen(send)],"%04X",P3E_FIX_DATALEN+counts);
		// �BCPU�Ď��^�C�}(4)+�R�}���h(4)+�T�u�R�}���h(4)
		strcat( send,"001014010001");	// �r�b�g�f�o�C�X
	}
	// �C�ǂݏo�����ڽ(8)
	strcat( send,devaddr);

	// �D�ǂݏo���_��
	sprintf( &send[strlen(send)],"%04X",counts);

	// �E���M�f�[�^
	if( devtype==DEV_WORD)
		memcpy( &send[strlen(send)],data,counts*4);
	else
		memcpy( &send[strlen(send)],data,counts);


	// ���M����
	ret=UdpSend(ipaddr,port,send,strlen(send),buf,sizeof(buf));

	return(ret);
}

//========================================================================================================
//	int CMelsecP3E::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
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
int CMelsecP3E::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
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

	// �I���R�[�h���`�F�b�N(�擪����18�o�C�g)
	if( memcmp( rbuf+18,"0000",4)){
		printf("\nrcv=%s",rbuf);
		// �I���R�[�h���o�͂��ďI��
		memcpy( rcv,rbuf+18,4);
		return(1);
	}

	// ��M�f�[�^��(�擪����14�o�C�g��)�A�I���R�[�h(4�o�C�g)���܂�ł���̂ň���
	ret=this->htoi( rbuf+14,4)-4;

	// ��M�f�[�^�R�s�[
	if(ret>0){
		memset( rcv,0,rsize);
		memcpy( rcv, rbuf+22, ret> rsize ? rsize:ret);
	}

	return(0);
}

//========================================================================================================
//	int CMelsecP3E::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	�@�\�@�F��������AMelsec���M�`���ɕϊ�����
//			ex) "A1234"->"31413433"
//	�����P�F�o�̓o�b�t�@
//	�����Q�F�o�b�t�@�T�C�Y�i�ϊ����f�[�^���̂Q�{+1�͕K�v�j
//	�����R�F�ϊ���������f�[�^(NULL-terminated)
//	�Ԓl�@�F0=OK
//========================================================================================================
int CMelsecP3E::Str2Mel(char* buf, unsigned int bufsize, char* str)
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
//	unsigned char CMelsecP3E::htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
int CMelsecP3E::Mel2Str(char* buf, unsigned int bufsize, char* melstr)
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
//	unsigned char CMelsecP3E::htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
unsigned short CMelsecP3E::htoi( char *hexstr , short len )
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


BOOL CMelsecP3E::ULChg( char* buf )
{
	char tmp[8];

	if( strlen(buf)<8 )
		return FALSE;

	memcpy( tmp , buf , 8);
	memcpy( buf+0 , tmp+4 , 4 );
	memcpy( buf+4 , tmp+0 , 4 );
	
	return TRUE;
}
