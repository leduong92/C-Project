//================================================================================================
//	FileName	:KeyenceVT3e.cpp
//	Class		:KeyenceVT3e
// 				:KEYENCE�^�b�`�p�l��VT3�ʐM
//	�쐬��		:2013.05.29 coba
//================================================================================================
#include <stdio.h>
#include <conio.h>
#include "KeyenceVT3e.h"

//========================================================================================================
//	KeyenceVT3e::KeyenceVT3e(void)
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F�R���X�g���N�^
//========================================================================================================
KeyenceVT3e::KeyenceVT3e(void)
{
	m_fOpen=FALSE;
}

//========================================================================================================
//	KeyenceVT3e::~KeyenceVT3e()
//	�f�X�g���N�^
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F
//========================================================================================================
KeyenceVT3e::~KeyenceVT3e()
{
	WSACleanup();
}

//========================================================================================================
//	int KeyenceVT3e::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type�@�Fprivate
//�@�@�\�@�FWinSock������
//	�����P�FTRUE=TCP�ʐM,FALSE(�f�t�H���g)=UDP
//========================================================================================================
BOOL KeyenceVT3e::Init(void)
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
// int KeyenceVT3e::ReadW(char* dev,int cnt,char* buf,unsigned int bufsize)
// �@�\�FTP�̓������[�h�f�o�C�X��ǂݏo��
// �ǂݏo����́AMelsec�Ɠ���1���[�h=4�o�C�g������`���ŕԂ��܂�
// �����P�FIP�A�h���X
// �����Q�F�|�[�g�ԍ�
// �����P�F�f�o�C�X�A�h���X
// �����Q�F�f�o�C�X�_��(1-64)
// �����R�F��M�o�b�t�@
// �����S�F��M�o�b�t�@�T�C�Y
//========================================================================================================
int KeyenceVT3e::ReadW(const char* ipaddr ,const int port
				,const int  dev_no,const int cnt,char* buf,unsigned int bufsize)
{
	char send[256],rcv[256];
	int ret;
	unsigned short* rcvdat;

	// �����`�F�b�N
	if( cnt <1 || cnt > 64 ) return(-1);
	if( (int)bufsize < cnt*4) return(-2);


	// �w�b�_������
	VT_UDP_SEND* vt=(VT_UDP_SEND*)send;
	memset(send,0,sizeof(send));
	memcpy( vt->cmd,"ERW",3);		// �R�}���h
	vt->siki='M';					// ���ʎq M:�����f�o�C�X�Œ�
	vt->dev_no=dev_no;				// �f�o�C�X�A�h���X
	vt->dev_cnt=cnt;				// �f�[�^��
	vt->len=7;						// ���M�f�[�^��(�ǂݏo���Ȃ̂Ńw�b�_���̂�)

	// ��M����
	ret=UdpSend((char*)ipaddr,port,send,sizeof(VT_UDP_SEND),rcv,sizeof(rcv));
	if(ret) return(ret);

	// ���ʃ`�F�b�N
	VT_UDP_RCV* vtr=(VT_UDP_RCV*)rcv;
	if( vtr->result ) return((int)vtr->result);

	// ��M�f�[�^�̈ʒu���Z�b�g
	rcvdat=(unsigned short*)(rcv+sizeof(VT_UDP_RCV));

	// �f�[�^�R�s�[
	memset( buf,0,bufsize);
	for(ret=0;ret<cnt;ret++,rcvdat++){
		sprintf( &buf[ret*4],"%04X",*rcvdat);
	}

	return(0);
}
//========================================================================================================
// int KeyenceVT3e::WriteW(char* dev,int cnt,char* buf,unsigned int bufsize)
// �@�\�FTP�̓������[�h�f�o�C�X�������o��
// �����P�FIP�A�h���X
// �����Q�F�|�[�g�ԍ�
// �����P�F�f�o�C�X�A�h���X
// �����Q�F�f�o�C�X�_��(1-64)
// �����R�F���M�f�[�^(Melsec�Ɠ�����1���[�h=4�o�C�g������`��)
//========================================================================================================
int KeyenceVT3e::WriteW(const char* ipaddr ,const int port,const int  dev_no,const int cnt,char* dat)
{
	char send[256],rcv[256];
	int ret;
	unsigned short* senddat;

	// �����`�F�b�N
	if( cnt <1 || cnt > 64 ) return(-1);
	if( (int)strlen(dat) < cnt*4) return(-2);


	// �w�b�_������
	VT_UDP_SEND* vt=(VT_UDP_SEND*)send;
	memset(send,0,sizeof(send));
	memcpy( vt->cmd,"EWW",3);		// �R�}���h
	vt->siki='M';					// ���ʎq M:�����f�o�C�X�Œ�
	vt->dev_no=dev_no;				// �f�o�C�X�A�h���X
	vt->dev_cnt=cnt;				// �f�[�^��
	vt->len=7+cnt*2;				// ���M�f�[�^��(�w�b�_��+�f�[�^)

	// �f�[�^���Z�b�g:������𐔒l�ɕϊ�
	senddat=(unsigned short*)(send+sizeof(VT_UDP_SEND));
	for(ret=0;ret<cnt;ret++,senddat++){
		*senddat=this->htous(&dat[ret*4],4);
	}

	// ���M����
	ret=UdpSend((char*)ipaddr,port,send,sizeof(VT_UDP_SEND)+cnt*2,rcv,sizeof(rcv));
	if(ret) return(ret);

	// ���ʃ`�F�b�N
	VT_UDP_RCV* vtr=(VT_UDP_RCV*)rcv;
	if( vtr->result ) return((int)vtr->result);


	return(0);
}


//========================================================================================================
// int KeyenceVT3e::ReadB(char* dev,int cnt,char* buf,unsigned int bufsize)
// �@�\�FTP�̓������[�h�f�o�C�X��ǂݏo��
// �����P�FIP�A�h���X
// �����Q�F�|�[�g�ԍ�
// �����R�F�f�o�C�X�A�h���X
// �߂�l 0=OFF,1=ON,����ȊO=�G���[
//========================================================================================================
int KeyenceVT3e::ReadB(const char* ipaddr ,const int port,const int  dev_no)
{
	char send[256],rcv[256];
	int ret;
	unsigned char* rcvdat;


	// �w�b�_������
	VT_UDP_SEND_BIT* vt=(VT_UDP_SEND_BIT*)send;
	memset(send,0,sizeof(send));
	memcpy( vt->cmd,"ERB",3);		// �R�}���h
	vt->siki='M';					// ���ʎq M:�����f�o�C�X�Œ�
	vt->dev_no=dev_no;				// �f�o�C�X�A�h���X
	vt->len=6;						// ���M�f�[�^��(�ǂݏo���Ȃ̂Ńw�b�_���̂�)


	// ��M����
	ret=UdpSend((char*)ipaddr,port,send,sizeof(VT_UDP_SEND_BIT),rcv,sizeof(rcv));
	if(ret) return(ret);

	// ���ʃ`�F�b�N
	VT_UDP_RCV* vtr=(VT_UDP_RCV*)rcv;
	if( vtr->result ) return(-1);

	// ��M�f�[�^�̈ʒu���Z�b�g
	rcvdat=(unsigned char*)(rcv+sizeof(VT_UDP_RCV));

	ret=(int)(*rcvdat);

	return(ret);
}

//========================================================================================================
// int KeyenceVT3e::ReadB(char* dev,int cnt,char* buf,unsigned int bufsize)
// �@�\�FTP�̓������[�h�f�o�C�X��ǂݏo��
// �����P�FIP�A�h���X
// �����Q�F�|�[�g�ԍ�
// �����R�F�f�o�C�X�A�h���X
// �����S�FTRUE=on,FALSE=OFF
// �߂�l 0=OK,����ȊO=�G���[
//========================================================================================================
int KeyenceVT3e::WriteB(const char* ipaddr ,const int port,const int  dev_no,const BOOL on)
{
	char send[256],rcv[256];
	int ret;
	unsigned char* senddat;


	// �w�b�_������
	VT_UDP_SEND_BIT* vt=(VT_UDP_SEND_BIT*)send;
	memset(send,0,sizeof(send));
	memcpy( vt->cmd,"EWB",3);		// �R�}���h
	vt->siki='M';					// ���ʎq M:�����f�o�C�X�Œ�
	vt->dev_no=dev_no;				// �f�o�C�X�A�h���X
	vt->len=6+1;					// ���M�f�[�^��(�ǂݏo���Ȃ̂Ńw�b�_+�f�[�^

	// �f�[�^�Z�b�g
	senddat=(unsigned char*)(send+sizeof(VT_UDP_SEND_BIT));
	if( on ) *senddat=1;

	// ���M����
	ret=UdpSend((char*)ipaddr,port,send,sizeof(VT_UDP_SEND_BIT)+1,rcv,sizeof(rcv));
	if(ret) return(ret);

	// ���ʃ`�F�b�N
	VT_UDP_RCV* vtr=(VT_UDP_RCV*)rcv;
	if( vtr->result ) return(vtr->result);

	return(ret);
}

//========================================================================================================
//	int KeyenceVT3e::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
//	type�@�Fprivate
//	�@�\�@�FUDP����M
//	�����P�F�^�[�Q�b�g��IP�A�h���X
//	�����Q�F�^�[�Q�b�g��UDP�|�[�g�ԍ�
//	�����R�F���M�f�[�^
//	�����S�F���M�f�[�^�T�C�Y
//	�����T�F��M�o�b�t�@
//	�����U�F��M�o�b�t�@�T�C�Y
//	�߂�l�F0=OK,0����=�l�b�g���[�N�G���[�A1=��M�f�[�^�G���[
//========================================================================================================
int KeyenceVT3e::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,unsigned int rsize)
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

	if(ret == SOCKET_ERROR){
		WSACloseEvent(fEvent);
		closesocket(fSocket);
		return(-108);
	}

	memset( rcv,0,rsize);
	memcpy( rcv, rbuf, cnt> rsize ? rsize:cnt);

	return(0);
}

//========================================================================================================
//	unsigned short KeyenceVT3e::htous( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
unsigned short KeyenceVT3e::htous( char *hexstr , short len )
{
	unsigned short ret = 0;
	unsigned short i;

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

