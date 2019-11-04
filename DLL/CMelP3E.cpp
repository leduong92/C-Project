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
CMelsecP3E::CMelsecP3E()
{
	m_fOpen=FALSE;

	m_fBinMode = FALSE;	//������Ԃ̓A�X�L�[���[�h�Ƃ���	//2014.02.03 sanno
	
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
//	int CMelsecP3E::SetMode(BOOL binary_mode)
//	type�@�Fprivate
//�@�@�\�@�FWPLC�̃��[�h���Z�b�g
//========================================================================================================
void CMelsecP3E::SetMode(BOOL binary_mode)
{
	if(binary_mode == TRUE)
		m_fBinMode = TRUE;
	else
		m_fBinMode = FALSE;

	return;
}


//========================================================================================================
//	int CMelsecP3E::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type�@�Fprivate
//�@�@�\�@�FWinSock������
//========================================================================================================
BOOL CMelsecP3E::Init()
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
int CMelsecP3E::P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize
							,int devtype,char* devaddr, short counts)
{
	int ret;

	if(m_fBinMode == TRUE)
		ret = this->P3ERead(ipaddr, port, buf, bufsize, devtype, devaddr, counts, TRUE);
	else
		ret = this->P3ERead(ipaddr, port, buf, bufsize, devtype, devaddr, counts, FALSE);
		
	return ret;
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
int CMelsecP3E::P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize
							,int devtype,char* devaddr, short counts,BOOL binary)
{
	int ret;

	if(binary == TRUE)
		ret = this->P3EReadB(ipaddr, port, buf, bufsize, devtype, devaddr, counts);
	else
		ret = this->P3EReadA(ipaddr, port, buf, bufsize, devtype, devaddr, counts);
		
	return ret;
}


//========================================================================================================
//	int CMelsecP3E::P3EReadA( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
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
int CMelsecP3E::P3EReadA( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
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
	ret=UdpSendA(ipaddr,port,send,strlen(send),buf,bufsize);

	return(ret);
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
int CMelsecP3E::P3EReadB( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts)
{

	// ���M�o�b�t�@
	unsigned char send[120];
	int ret;
	unsigned short len;
	char txt[30];
	short hex_sign;	//�ǂݏo���A�h���X�̎w����@��16�i�����ǂ����̃T�C��	0:10�i	1:16�i
	unsigned int top_addres;
	
	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 960 ) return(-2);

	// ���ڽ(�f�o�C�X�R�[�h(2)+�擪�f�o�C�X(6))
	if( strlen(devaddr) !=8) return(-3);

	// IP�A�h���X
	if( INADDR_NONE == inet_addr(ipaddr) ) return(-4);

	// �|�[�g�ԍ�
	if(port <1024 || port >65535) return(-5);


	// �@�Œ蕔
	memset(send,0,sizeof(send));
	// �T�u�w�b�_"5000"+NW�ԍ�"00"+PC�ԍ�"FF"+�v����Unit#"03FF"+�v���ǔ�"00"
	send[0]=0x50;//�T�u�w�b�_�[
	send[1]=0x00;//�T�u�w�b�_�[
	send[2]=0x00;//�l�b�g���[�N�ԍ�
	send[3]=0xFF;//PC�ԍ�
	send[4]=0xFF;//�v���惆�j�b�g
	send[5]=0x03;//I/O�ԍ�
	send[6]=0x00;//�ǔ�

	// �A���M�f�[�^�����Z�o����=�ǂݏo���Ȃ̂ŌŒ蒷(�Ď��^�C�}�`�ǂݏo���_��)
//		sprintf( &send[strlen(send)],"%04X",P3E_FIX_DATALEN);
	len=P3E_FIX_DATALEN/2;
	memcpy(&send[7],&len,2);//�v���f�[�^��0x0c


	// �BCPU�Ď��^�C�}(4)+�R�}���h(4)+�T�u�R�}���h(4)
	if( devtype==DEV_WORD)
	{
//			strcat( send,"001004010000");	// ���[�h�f�o�C�X
		send[ 9]=0x10;//CPU�Ď��^�C�}�[L
		send[10]=0x00;//CPU�Ď��^�C�}�[H
		send[11]=0x01;//�R�}���hL
		send[12]=0x04;//�R�}���hH
		send[13]=0x00;//�T�u�R�}���hL
		send[14]=0x00;//�T�u�R�}���hH
	}
	else 
	{
	//			strcat( send,"001004010001");	// �r�b�g�f�o�C�X
		send[ 9]=0x10;//CPU�Ď��^�C�}�[L
		send[10]=0x00;//CPU�Ď��^�C�}�[H
		send[11]=0x01;//�R�}���hL
		send[12]=0x04;//�R�}���hH
		send[13]=0x01;//�T�u�R�}���hL
		send[14]=0x00;//�T�u�R�}���hH
	}	

	// �f�o�C�X�R�[�h�ɕϊ�
	hex_sign = 0;

	if(!memcmp(devaddr,"D*",2)) send[18]=0xa8;			//�f�[�^���W�X�^
	else if(!memcmp(devaddr,"M*",2)) send[18]=0x90;		//���������[
	else if(!memcmp(devaddr,"SM",2)) send[18]=0x91;		//���ꃊ���[
	else if(!memcmp(devaddr,"SD",2)) send[18]=0xA9;		//���ꃌ�W�X�^
	else if(!memcmp(devaddr,"X*",2)){					//���̓����[
		send[18]=0x9C;
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"Y*",2)){					//�o�̓����[
		send[18]=0x9D;
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"L*",2)) send[18]=0x92;		//���b�`�����[
	else if(!memcmp(devaddr,"F*",2)) send[18]=0x93;		//�A�i���V�F�[�^
	else if(!memcmp(devaddr,"V*",2)) send[18]=0x94;		//�G�b�W�����[
	else if(!memcmp(devaddr,"B*",2)){					//�����N�����[
		send[18]=0xA0;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"W*",2)){					//�����N���W�X�^
		send[18]=0xB4;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"TS",2)) send[18]=0xC1;		//�^�C�}�@�ړ_
	else if(!memcmp(devaddr,"TC",2)) send[18]=0xC0;		//�^�C�}�@�R�C��
	else if(!memcmp(devaddr,"TN",2)) send[18]=0xC2;		//�^�C�}�@���ݒn
	else if(!memcmp(devaddr,"SS",2)) send[18]=0xC7;		//�ώZ�^�C�}�@�ړ_
	else if(!memcmp(devaddr,"SC",2)) send[18]=0xC6;		//�ώZ�^�C�}�@�R�C��
	else if(!memcmp(devaddr,"SN",2)) send[18]=0xC8;		//�ώZ�^�C�}�@���ݒn
	else if(!memcmp(devaddr,"CS",2)) send[18]=0xC4;		//�J�E���^�@�ړ_
	else if(!memcmp(devaddr,"CC",2)) send[18]=0xC3;		//�J�E���^�@�R�C��
	else if(!memcmp(devaddr,"CN",2)) send[18]=0xC5;		//�J�E���^�@���ݒn
	else if(!memcmp(devaddr,"SB",2)){					//���ꃊ���N�����[
		send[18]=0xA1;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"SW",2)){					//���ꃊ���N���W�X�^
		send[18]=0xB5;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"S*",2)) send[18]=0x98;		//�X�e�b�v�����[
	else if(!memcmp(devaddr,"DX",2)){					//�_�C���N�g����
		send[18]=0xA2;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"DY",2)){					//�_�C���N�g�o��
		send[18]=0xA3;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"Z*",2)) send[18]=0xCC;		//�C���f�b�N�X���W�X�^
	else if(!memcmp(devaddr,"R*",2)) send[18]=0xAF;		//�t�@�C���W�X�^
	else if(!memcmp(devaddr,"ZR",2)){					//�t�@�C�����W�X�^
		send[18]=0xB0;		
		hex_sign = 1;
	}
	else                     return(-100);

	// �C�ǂݏo�����ڽ(8)
	if(hex_sign == 0){
		memset(txt, 0, sizeof(txt));
		memcpy(txt, devaddr+2, 6);
		top_addres = atoi(txt);
		memcpy(&send[15], (unsigned int*)&top_addres ,3);
	}
	else{
		memset(txt, 0, sizeof(txt));
		memcpy(txt, devaddr+6, 2);
		send[15]=(unsigned char)htoi(txt, 2);//�f�B�o�C�X�_��  L
		memcpy(txt, devaddr+4, 2);
		send[16]=(unsigned char)htoi(txt, 2);//�f�B�o�C�X�_��  -
		memcpy(txt, devaddr+2, 2);
		send[17]=(unsigned char)htoi(txt, 2);//�f�B�o�C�X�_��  H	
	}


	// �D�ǂݏo���_��
	memcpy(&send[19], (unsigned short*)&counts ,2);
/*
	int i;
	i = 0;
	while(i<25){
		printf("%02d:%02x\n",i,send[i]);
		i++;
	}
*/
	// ���M����
	if( devtype==DEV_WORD)
		ret=UdpSendB(ipaddr,port,(char*)send,21,buf,bufsize);
	else
		ret=UdpSendBit(ipaddr,port,(char*)send,21,buf,bufsize, counts);

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
	int ret;

	if(m_fBinMode == TRUE)
		ret = this->P3EWrite(ipaddr, port, devtype, devaddr, counts, data, TRUE);
	else
		ret = this->P3EWrite(ipaddr, port, devtype, devaddr, counts, data, FALSE);

	return ret;
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
//	�����W�F�������݃��[�h�i�A�X�L�[or�o�C�i���j
//========================================================================================================
int CMelsecP3E::P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data, BOOL binary)
{
	int ret;

	if(binary == TRUE)
		ret = this->P3EWriteB(ipaddr, port, devtype, devaddr, counts, data);
	else
		ret = this->P3EWriteA(ipaddr, port, devtype, devaddr, counts, data);

	return ret;
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
int CMelsecP3E::P3EWriteA(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
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
	ret=UdpSendA(ipaddr,port,send,strlen(send),buf,sizeof(buf));

	return(ret);
}


//========================================================================================================
//	int CMelsecP3E::P3EWriteB(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
//	type�@�Fpublic
//�@�@�\�@�F�v���g�R��3E�`���Ńf�o�C�X�f�[�^���u���b�N���M(�o�C�i�����[�h)
//	�����P�F�^�[�Q�b�g��IP�A�h���X
//	�����Q�F�^�[�Q�b�g��UDP�|�[�g�ԍ�
//	�����R�F��M�o�b�t�@
//	�����S�F��M�o�b�t�@�T�C�Y
//	�����T�F�f�o�C�X�̎��(0=���[�h�f�o�C�X�A1=�r�b�g�f�o�C�X)
//	�����T�F�������ݐ擪�A�h���X(8��)
//	�����U�F���M�u���b�N��(max256)
//	�����V�F�������݃f�[�^
//========================================================================================================
int CMelsecP3E::P3EWriteB(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data)
{
	// ���M�o�b�t�@
	unsigned char send[5120];
	int ret,i;
	char buf[40+1];
	unsigned short len;
	char txt[30];
	short hex_sign;
	unsigned int top_addres;
	int send_bytes;

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
	else
		if( (short)strlen(data) !=counts) return(-7);

	memset( send, 0, sizeof(send) );


	// �@�Œ蕔
	// �T�u�w�b�_"5000"+NW�ԍ�"00"+PC�ԍ�"FF"+�v����Unit#"03FF"+�v���ǔ�"00"
	send[0]=0x50;//�T�u�w�b�_�[
	send[1]=0x00;//�T�u�w�b�_�[
	send[2]=0x00;//�l�b�g���[�N�ԍ�
	send[3]=0xFF;//PC�ԍ�
	send[4]=0xFF;//I/O�ԍ�
	send[5]=0x03;//�v���惆�j�b�g
	send[6]=0x00;//�ǔ�


	
	// ���[�h�f�o�C�X
	if( devtype==DEV_WORD){
		// �A���M�f�[�^�����Z�o����=(�Œ�:�Ď��^�C�}�`�ǂݏo���_��)+�f�o�C�X�_���~4(�o�C�g)
		//send[7]=(P3E_FIX_DATALEN+counts*4)/2;//�v���f�[�^��
		len=(P3E_FIX_DATALEN + counts*4)/2;
		memcpy(&send[7],&len,2);//�v���f�[�^��

		// �BCPU�Ď��^�C�}(4)+�R�}���h(4)+�T�u�R�}���h(4)
//			strcat( send,"001014010000");	// ���[�h�f�o�C�X
		send[ 9]=0x10;//CPU�Ď��^�C�}�[L
		send[10]=0x00;//CPU�Ď��^�C�}�[H
		send[11]=0x01;//�R�}���hL
		send[12]=0x14;//�R�}���hH
		send[13]=0x00;//�T�u�R�}���hL
		send[14]=0x00;//�T�u�R�}���hH
	}
	// �r�b�g�f�o�C�X
	else{
		// �A���M�f�[�^�����Z�o����=(�Œ�:�Ď��^�C�}�`�ǂݏo���_��)+�f�o�C�X�_���~4(�o�C�g)
		if(counts % 2 == 0)
			len= (P3E_FIX_DATALEN + counts) /2;
		else
			len= (P3E_FIX_DATALEN + counts) /2 +1;
		memcpy(&send[7],&len,2);//�v���f�[�^��

		// �BCPU�Ď��^�C�}(4)+�R�}���h(4)+�T�u�R�}���h(4)
		//strcat( send,"001014010001");	// �r�b�g�f�o�C�X

		send[ 9]=0x10;//CPU�Ď��^�C�}�[L
		send[10]=0x00;//CPU�Ď��^�C�}�[H
		send[11]=0x01;//�R�}���hL
		send[12]=0x14;//�R�}���hH
		send[13]=0x01;//�T�u�R�}���hL
		send[14]=0x00;//�T�u�R�}���hH

	}
	
	// �f�o�C�X�R�[�h�ɕϊ�
	hex_sign = 0;

	if(!memcmp(devaddr,"D*",2)) send[18]=0xa8;			//�f�[�^���W�X�^
	else if(!memcmp(devaddr,"M*",2)) send[18]=0x90;		//���������[
	else if(!memcmp(devaddr,"SM",2)) send[18]=0x91;		//���ꃊ���[
	else if(!memcmp(devaddr,"SD",2)) send[18]=0xA9;		//���ꃌ�W�X�^
	else if(!memcmp(devaddr,"X*",2)){					//���̓����[
		send[18]=0x9C;
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"Y*",2)){					//�o�̓����[
		send[18]=0x9D;
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"L*",2)) send[18]=0x92;		//���b�`�����[
	else if(!memcmp(devaddr,"F*",2)) send[18]=0x93;		//�A�i���V�F�[�^
	else if(!memcmp(devaddr,"V*",2)) send[18]=0x94;		//�G�b�W�����[
	else if(!memcmp(devaddr,"B*",2)){					//�����N�����[
		send[18]=0xA0;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"W*",2)){					//�����N���W�X�^
		send[18]=0xB4;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"TS",2)) send[18]=0xC1;		//�^�C�}�@�ړ_
	else if(!memcmp(devaddr,"TC",2)) send[18]=0xC0;		//�^�C�}�@�R�C��
	else if(!memcmp(devaddr,"TN",2)) send[18]=0xC2;		//�^�C�}�@���ݒn
	else if(!memcmp(devaddr,"SS",2)) send[18]=0xC7;		//�ώZ�^�C�}�@�ړ_
	else if(!memcmp(devaddr,"SC",2)) send[18]=0xC6;		//�ώZ�^�C�}�@�R�C��
	else if(!memcmp(devaddr,"SN",2)) send[18]=0xC8;		//�ώZ�^�C�}�@���ݒn
	else if(!memcmp(devaddr,"CS",2)) send[18]=0xC4;		//�J�E���^�@�ړ_
	else if(!memcmp(devaddr,"CC",2)) send[18]=0xC3;		//�J�E���^�@�R�C��
	else if(!memcmp(devaddr,"CN",2)) send[18]=0xC5;		//�J�E���^�@���ݒn
	else if(!memcmp(devaddr,"SB",2)){					//���ꃊ���N�����[
		send[18]=0xA1;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"SW",2)){					//���ꃊ���N���W�X�^
		send[18]=0xB5;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"S*",2)) send[18]=0x98;		//�X�e�b�v�����[
	else if(!memcmp(devaddr,"DX",2)){					//�_�C���N�g����
		send[18]=0xA2;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"DY",2)){					//�_�C���N�g�o��
		send[18]=0xA3;		
		hex_sign = 1;
	}
	else if(!memcmp(devaddr,"Z*",2)) send[18]=0xCC;		//�C���f�b�N�X���W�X�^
	else if(!memcmp(devaddr,"R*",2)) send[18]=0xAF;		//�t�@�C���W�X�^
	else if(!memcmp(devaddr,"ZR",2)){					//�t�@�C�����W�X�^
		send[18]=0xB0;		
		hex_sign = 1;
	}
	else                     return(-100);

	// �C�ǂݏo�����ڽ(8)
	if(hex_sign == 0){
		memset(txt, 0, sizeof(txt));
		memcpy(txt, devaddr+2, 6);
		top_addres = atoi(txt);
		memcpy(&send[15], (unsigned int*)&top_addres ,3);
	}
	else{
		memset(txt, 0, sizeof(txt));
		memcpy(txt, devaddr+6, 2);
		send[15]=(unsigned char)htoi(txt, 2);//�f�B�o�C�X�_��  L
		memcpy(txt, devaddr+4, 2);
		send[16]=(unsigned char)htoi(txt, 2);//�f�B�o�C�X�_��  -
		memcpy(txt, devaddr+2, 2);
		send[17]=(unsigned char)htoi(txt, 2);//�f�B�o�C�X�_��  H	
	}

	// �D�ǂݏo���_��
	memcpy(&send[19], (unsigned short*)&counts ,2);//�v���f�[�^��


	// �E���M�f�[�^
	// ���[�h�f�o�C�X
	if( devtype==DEV_WORD){
		// 1���[�h=2�o�C�g�ŕ\��
		for(i=0;i<counts;i++){
			// 2�o�C�g�ɕ�����(HL=>LH����ւ�)
			// ���M�f�[�^(data)�́A�������4�o�C�g=1���[�h�œ����Ă���
			send[21+(i*2)] = (unsigned char)htoi(data+(i*4)+2,2);	// L
			send[21+(i*2)+1] = (unsigned char)htoi(data+(i*4),2);	// H
		}
		// ���M�o�C�g��(1���[�h=2�o�C�g)
		send_bytes=21+counts*2;
/*
		// 1���[�h=2�o�C�g�ŕ\��
		for(i=0,j=21;i<counts*4;i=i+4,j=j+2){
			//2�o�C�g�ɕ�����
			send[j] = (unsigned char)htoi(data+(i+2),2);
			send[j+1] = (unsigned char)htoi(data+i,2);		
		}
*/
	}
	// BIT�f�o�C�X
	else{
		// 2�r�b�g��1�o�C�g�ŕ\��
		for(i=0;i<counts;i++){
			if(data[i] == '1'){
				if(i % 2==0 ) send[21+i/2] +=0x10;
				else send[21+i/2] +=0x1;
			}
		}
		// ���M�o�C�g��(2�r�b�g=1�o�C�g)
		send_bytes=21+counts/2;
		if(counts%2) send_bytes++;		// ����M�̏ꍇ��+1����
/*
		for(i=0,j=21;i<counts;i++)
		{
			if(i % 2 == 0){
				if(data[i] == '1')
					send[j] = (unsigned short)16;
				else
					send[j] = (unsigned short)0;
			}
			else{
				if(data[i] == '1')
					send[j] += (unsigned short)1;
				else
					send[j] += (unsigned short)0;
			}

			//2�r�b�g����1�o�C�g�̃f�[�^�ɓ��ꂽ��send[]���ЂƂi�߂�
			if(i % 2 == 1)
				j++;
		}
*/
	}
/* ����Ȃ�
	if(counts % 2 == 1)
		j++;
	send[j]=0;
*/
	// ���M����
	ret=UdpSendB(ipaddr,port,(char*)send,send_bytes,buf,sizeof(buf));

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
int CMelsecP3E::UdpSendA( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
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
		//printf("\nrcv=%s",rbuf);
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
int CMelsecP3E::UdpSendB( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
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
	int i;
	unsigned short dcnt,*dat;

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


	// �I���R�[�h���`�F�b�N
	if( rbuf[9] != 0x00 || rbuf[10] != 0x00){
		printf("\nrcv=%d%d",rbuf[9],rbuf[10]);
		// �I���R�[�h���o�͂��ďI��
//			memcpy( rcv,rbuf+18,4);
		sprintf(rcv,"%02d%02d",rbuf[9],rbuf[10]);
		return(1);
	}

	// ��M�f�[�^��(�擪����7,8�o�C�g��)�A�I���R�[�h(2�o�C�g)���܂�ł���̂ň���
	//2�o�C�g��4�������\����̂ŁA2�Ŋ���
	memcpy( &dcnt,&rbuf[7],2);
	dcnt = (dcnt - 2) /2;

	//ret=rbuf[8]*100+rbuf[7]-2;

	// ��M�f�[�^�R�s�[
	if(dcnt>0)
	{
		memset( rcv,0,rsize);
		for(i=0;i<dcnt;i++)
		{
			// �f�[�^�ʒu(��M�f�[�^��11�Ԗڂ���)
			dat=(unsigned short*)(rbuf+11+i*2);
			sprintf( &rcv[i*4],"%04X",*dat);
		}
	}

	return(0);
}
//========================================================================================================
//	int CMelsecP3E::UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
//	type�@�Fprivate
//�@�@�\�@�FUDP����M(BIT�f�o�C�X�ǂݏo��,BINARY���[�h��p)
//	�����P�F�^�[�Q�b�g��IP�A�h���X
//	�����Q�F�^�[�Q�b�g��UDP�|�[�g�ԍ�
//	�����R�F���M�f�[�^
//	�����S�F���M�f�[�^�T�C�Y
//	�����T�F��M�o�b�t�@
//	�����U�F��M�o�b�t�@�T�C�Y
//	�����V�F��M�f�[�^�_��
//	�߂�l�F0=OK,0����=�l�b�g���[�N�G���[�A1=��M�f�[�^�G���[
//========================================================================================================
int CMelsecP3E::UdpSendBit( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize, short counts)
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
	int i;
	unsigned short dcnt;

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

		// �I���R�[�h���`�F�b�N
		if( rbuf[9] != 0x00 || rbuf[10] != 0x00){
			printf("\nrcv=%d%d",rbuf[9],rbuf[10]);
			// �I���R�[�h���o�͂��ďI��
//			memcpy( rcv,rbuf+18,4);
			sprintf(rcv,"%02d%02d",rbuf[9],rbuf[10]);
			return(1);
		}

	// ��M�f�[�^��(�擪����7,8�o�C�g��)�A�I���R�[�h(2�o�C�g)���܂�ł���̂ň���
	memcpy( &dcnt,&rbuf[7],2);
	dcnt = dcnt - 2;

	// ��M�f�[�^�R�s�[
	if(dcnt>0)
	{
		memset( rcv,0,rsize);
		for(i=0;i<dcnt;i++)
		{
			// �f�[�^�ʒu(��M�f�[�^��11�Ԗڂ���)
			sprintf( &rcv[i*2],"%02X",rbuf[11+i]);
		}
	}

	//�f�[�^�ǂݍ��ݓ_������̏ꍇ�́A�Ō��1���̓_�~�[�f�[�^�Ȃ̂ŏ���
	if(counts % 2 == 1)	rcv[counts] = 0;
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


