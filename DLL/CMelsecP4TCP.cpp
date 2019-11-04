//================================================================================================
//	FileName	:CMelsecP4TCP.cpp
//	Class		:CMelsecP4TCP
// 				:Melsec �v���g�R���S�`���ʐMonTCP/IP
//				 Serial-Ethernet�R���o�[�^�̃T�[�o���[�h�p�ɁA�ʐM���e��TCP�ɏ悹�đ���
//	sum�`�F�b�N�L��Axon/off����Ȃ��B
//	�쐬��		:2013.09.20 coba
//================================================================================================
#include "CMelsecP4TCP.h"
#include <stdio.h>

//========================================================================================================
//	CMelsecP4TCP::CMelsecP4TCP(void)
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F�R���X�g���N�^�B�f�t�H���g�T�C�Y�Ńf�[�^�o�b�t�@���쐬
//========================================================================================================
CMelsecP4TCP::CMelsecP4TCP(void)
{
	m_fOpen=FALSE;
}

//========================================================================================================
//	CMelsecP4TCP::~CMelsecP4TCP()
//	�f�X�g���N�^
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F
//========================================================================================================
CMelsecP4TCP::~CMelsecP4TCP()
{
	WSACleanup();
}


//========================================================================================================
//	BOOL CMelsecP4TCP::Init(void)
//	type�@�Fprivate
//	�@�\�@�F������
//	�Ԓl�@�FTRUE or FALSE
//========================================================================================================
BOOL CMelsecP4TCP::Init(int wait)
{
	// ����ݒ���OK
	if( m_fOpen ) return(TRUE);

  	WSADATA			WSAData;

	m_Wait = wait;

	// �\�P�b�g������
  	if(WSAStartup( MAKEWORD(2,2), &WSAData)) return(-1);


	m_fOpen=TRUE;

	return(TRUE);
}

//========================================================================================================
//	int CMelsecP4TCP::P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h�f�[�^���u���b�N��M
//	�����P�F��M�o�b�t�@
//	�����Q�F��M�o�b�t�@�T�C�Y
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F��M�擪�A�h���X
//	�����U�F��M�u���b�N��(max64)
//	type�@�Fpublic
//========================================================================================================
int CMelsecP4TCP::P4ReadW( char* ipaddr,unsigned short port,char* buf,int bufsize
												,char* station,char* pcstation,char* addr, short counts)
{
	int ret,len;
	int retry=0,sum;
	char data[10],txt[10],rcv[1024];
	char initcom[5];

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

//printf("\n");
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 64 ) return(-2);

	memset( data, 0, sizeof(data));
	// ���M���[�v
	while( retry < 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}		
		//----���M������̍쐬-----
		// �ʏ�R�}���h
		if(strlen(addr) == 5 )
			MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "WR",addr,counts,"");
		// 2013.02.17 coba �g���R�}���h(D1023�ȍ~�Ή�)
		else MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "QR",addr,counts,"");

		//-----�f�[�^����M------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),rcv,sizeof(rcv));
		if( len < 1){
			printf("len=%d",len);
			return(-3);
		}
		// �c��̏��������R�}���h�ʂɌ��߂�
		//-----STX�̏ꍇ(data����)------
		if( rcv[0] == STX ){
			// 1���W�X�^�ǂݏo���ł��Œ�14�o�C�g����
			if( len < 14 ){
				retry++;										// ��ײ����
				continue;
			}

			// �ŏ���STX�ƁA�Ō�4��(sum�`�F�b�Ncrlf)��`���ăR�s-
			memset( m_buf, 0, sizeof(m_buf) );
			memcpy( m_buf,rcv+1,len-5);

			// �T���`�F�b�N�����R�s�[
			memcpy( data,rcv+len-4,2);

			// �T���`�F�b�N���r
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			// OK�̏ꍇ��ACK��Ԃ��Đ���I��
			if( !memcmp( txt, data, 2 ) ){
				Answer(ipaddr,port,ACK,station,pcstation);
				break;
			}
			// NG�̎���NAK��Ԃ�
			Answer(ipaddr,port,NAK,station,pcstation);
			retry++;										// ��ײ����
			continue;
		}

		//-----NAK�̏ꍇ------
		else if( rcv[0] == NAK ){
			retry++;										// ��ײ����
			continue;
		}

		//----- �G���[���B-------
		else return(-8);
	}
	if(retry >= 8) return(-9);

	// ETX���O���Ď�M�f�[�^���o�b�t�@�ɺ�߰����
	memcpy( buf, m_buf+4,(int)strlen(m_buf+4)-1 > bufsize ? bufsize:strlen(m_buf+4)-1);

	return(0);
}

//========================================================================================================
//	int CMelsecP4TCP::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h���u���b�N���M
//	type�@�Fprivate
//	�����P�F�ǔ�
//	�����Q�FPC�ǔ�
//	�����R�F���M�擪�A�h���X
//	�����S�F���M�u���b�N��(max64)
//	�����T�F���M�f�[�^
//========================================================================================================
int CMelsecP4TCP::P4WriteW( char* ipaddr,unsigned short port,char* station, char* pcstation,char* addr, short counts, char* msg)
{
	int ret,len;
	int retry=0;
	char data[128];
	char initcom[5];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 64 ) return(-2);

	// �����I�[�o�[
	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);

	if( counts*4 != (int)strlen(msg) ) return(-11);

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;


	// ���M������̍쐬
	// �ʏ�R�}���h
	if(strlen(addr) == 5 )
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "WW",addr,counts,msg);
	// 2013.02.17 coba �g���R�}���h(D1023�ȍ~�Ή�)
	else
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "QW",addr,counts,msg);

	// semd-loop
	while( retry < 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}		
		//-----�f�[�^����M------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),data,sizeof(data));
		if( len < 1){
			if( retry++ < 8 )continue;
			return(-3);
		}
		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		if( data[0] == ACK )  break;		// ACK�̏ꍇ
		else{
			if( retry++ < 8 )continue;
			return(-4);						// �G���[���B
		}
	}
	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);
	return(0);
}
//========================================================================================================
//	int CMelsecP4TCP::P4ReadB( char* station,char* pcstation,char* addr)
//	type�@�Fpublic
//�@�@�\�@�F�v���g�R���S�`���Ńr�b�g�f�[�^���P�f�o�C�X��M
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F��M�擪�A�h���X
//  �߂�l�F0=OFF,1=ON,0����=�G���[
//========================================================================================================
int CMelsecP4TCP::P4ReadB( char* ipaddr,unsigned short port,char* station,char* pcstation,char* addr)
{
	int ret,buf;
	char data[20];

	memset(data,0,sizeof(data));
	ret=P4ReadBB(ipaddr,port,data,sizeof(data),station,pcstation,addr,1);
	if(ret) return(ret);

	// ��M�f�[�^���o�b�t�@�ɺ�߰����
	buf=data[0]-0x30;

	return(buf);
}
//========================================================================================================
//	int CMelsecP4TCP::P4ReadB( char* station,char* pcstation,char* addr)
//	type�@�Fpublic
//�@�@�\�@�F�v���g�R���S�`���Ńr�b�g�f�[�^���P�f�o�C�X��M
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F��M�擪�A�h���X
//  �߂�l�F0=OFF,1=ON,0����=�G���[
//========================================================================================================
int CMelsecP4TCP::P4ReadBB(  char* ipaddr,unsigned short port,char* buf,int bufsize
											,char* station,char* pcstation,char* addr,short cnt)
{
	int ret,len;
	int retry=0,sum;
	char data[10],txt[10],rcv[1024];
	char initcom[5];

	if( m_fOpen == FALSE ) return(-1);

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	memset( data, 0, sizeof(data));
	// ���M���[�v
	while( retry < 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}

		//----���M������̍쐬-----
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "BR",addr,cnt,"");

		//-----�f�[�^����M------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),rcv,sizeof(rcv));
		if( len < 1) return(-3);

		// �c��̏��������R�}���h�ʂɌ��߂�
		//-----STX�̏ꍇ(data����)------
		if( rcv[0] == STX ){
			// 1bit�ǂݏo���ł��Œ�11�o�C�g����
			if( len < 11){
				retry++;										// ��ײ����
				continue;
			}
			// �ŏ���STX�ƁA�Ō�4��(sum�`�F�b�Ncrlf)��`���ăR�s-
			memset( m_buf, 0, sizeof(m_buf) );
			memcpy( m_buf,rcv+1,len-5);

			// �T���`�F�b�N�����R�s�[
			memcpy( data,rcv+len-4,2);

			// �T���`�F�b�N���r
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			// OK�̏ꍇ��ACK��Ԃ��Đ���I��
			if( !memcmp( txt, data, 2 ) ){
				Answer(ipaddr,port,ACK,station,pcstation);
				break;
			}
			// NG�̎���NAK��Ԃ�
			Answer(ipaddr,port,NAK,station,pcstation);
			retry++;										// ��ײ����
			continue;
		}

		//-----NAK�̏ꍇ------
		else if( rcv[0] == NAK ){
			retry++;										// ��ײ����
			continue;
		}

		//----- �G���[���B-------
		else return(-8);
	}

	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);

	// ETX�J�b�g���Ď�M�f�[�^���o�b�t�@�ɺ�߰����
	memcpy( buf, m_buf+4,(int)(strlen(m_buf+4)-1) > bufsize ? bufsize:strlen(m_buf+4)-1);

	return(0);
}

//========================================================================================================
//	int CMelsecP4TCP::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//�@�@�\�@�F�v���g�R���S�`���Ńr�b�g�f�o�C�X(1�_)��ON/OFF
//	type�@�Fprivate
//	�����P�F�ǔ�
//	�����Q�FPC�ǔ�
//	�����R�F���M�A�h���X
//	�����T�F���M�f�[�^ 0=OFF,����ȊO=ON
//	�߂�l�F0.=���� ����ȊO=�G���[
//========================================================================================================
int CMelsecP4TCP::P4WriteB( char* ipaddr,unsigned short port,char* station, char* pcstation,char* addr, int onoff)
{
	int ret;
	char send[12];

	// ���M������̍쐬
	if( !onoff ) strcpy(send,"0\0");
	else strcpy(send,"1\0");				// off

	ret=P4WriteBB(ipaddr,port,station,pcstation,addr,1,send);
	return(ret);
}
//========================================================================================================
// int CMelsecP4TCP::P4WriteBB( char* station, char* pcstation,char* addr, int cnt,char* dat)
//	2012.06.20 coba�ǉ�
//�@�@�\�@�F�v���g�R���S�`���Ńr�b�g�f�o�C�X�����_���ꊇON/OFF
//	type�@�Fprivate
//	�����P�F�ǔ�
//	�����Q�FPC�ǔ�
//	�����R�F���M�A�h���X
//	�����T�F���M�f�[�^ 0=OFF,1=ON�̔z��
//	�߂�l�F0.=���� ����ȊO=�G���[
//========================================================================================================
int CMelsecP4TCP::P4WriteBB( char* ipaddr,unsigned short port,char* station, char* pcstation,char* addr, int cnt,char* dat)
{
	int ret,len;
	int retry=0;
	char data[128];
	char initcom[5];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);

	// �Z�b�g���郌�W�X�^��=�f�[�^�o�C�g���Ȃ̂Ń`�F�b�N
	if( (int)strlen(dat) != cnt ) return(-2);

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "BW",addr,cnt,dat);

	// semd-loop
	while( retry < 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}		

		//-----�f�[�^����M------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),data,sizeof(data));
		if( len < 1){
			if( retry++ < 8 )continue;
			return(-3);
		}

		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		if( data[0] == ACK ) break;		// ACK�̏ꍇ
		else retry++;
	}

	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);
	return(0);
}



//========================================================================================================
//	int CMelsecP4TCP::P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h�f�[�^���u���b�N��M
//	�����P�F��M�o�b�t�@
//	�����Q�F��M�o�b�t�@�T�C�Y
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F��M�擪�A�h���X
//	�����U�F��M�u���b�N��(max64)
//	type�@�Fpublic
//========================================================================================================
int CMelsecP4TCP::P2C4ReadW( char* ipaddr,unsigned short port,char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
{
	int ret,len;
	int retry=0,sum;
	char rcv[4096],data[10],txt[10];
	char initcom[5];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 960 ) return(-2);

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	// semd-loop
	while( retry <= 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}

		// ���M������̍쐬(2C)
		MakeCmd2C( m_buf, sizeof(m_buf), station,pcstation, "2",addr,counts,"");

		//-----�f�[�^����M------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),rcv,sizeof(rcv));
		if( len < 1) return(-3);

		// �c��̏��������R�}���h�ʂɌ��߂�
		//-----STX�̏ꍇ(data����)------
		if( rcv[0] == STX ){
			// 1���W�X�^�ǂݏo���ł��Œ�16�o�C�g����
			if( len < 16 ){
				retry++;										// ��ײ����
				continue;
			}

			// �ŏ���STX�ƁA�Ō�4��(sum�`�F�b�Ncrlf)��`���ăR�s-
			memset( m_buf, 0, sizeof(m_buf) );
			memcpy( m_buf,rcv+1,len-5);

			// �T���`�F�b�N�����R�s�[
			memcpy( data,rcv+len-4,2);

			// �T���`�F�b�N���r
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			if( !memcmp( txt, data, 2 ) ) break;
		}
		else retry++;
	}
	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);

	// ��M�f�[�^���o�b�t�@�ɺ�߰����
	memcpy( buf, m_buf+6,(int)(strlen(m_buf+6)-1) > bufsize ? bufsize:strlen(m_buf+6)-1);

	return(0);

	
}

//========================================================================================================
//	int CMelsecP4TCP::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h���u���b�N���M
//	type�@�Fprivate
//	�����P�F�ǔ�
//	�����Q�FPC�ǔ�
//	�����R�F���M�擪�A�h���X
//	�����S�F���M�u���b�N��(max64)
//	�����T�F���M�f�[�^
//========================================================================================================
int CMelsecP4TCP::P2C4WriteW( char* ipaddr,unsigned short port,char* station, char* pcstation,char* addr, short counts, char* msg)
{
	int ret,len;
	int retry=0;
	char data[20];
	char initcom[5];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 960 ) return(-2);

	// �����I�[�o�[
	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);

	if( counts*4 != (int)strlen(msg) ) return(-11);

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	// ���M������̍쐬(2C)
	MakeCmd2C( m_buf, sizeof(m_buf), station,pcstation, "4",addr,counts,msg);

	// semd-loop
	while( retry <= 8 ){
		if( retry ){
			ret=TcpSend( ipaddr,port,initcom,3,NULL,0);
		}		

		//-----�f�[�^����M------
		len=TcpSend( ipaddr,port,m_buf,strlen(m_buf),data,sizeof(data));
		if( len < 1){
			if( retry++ < 8 )continue;
			return(-3);
		}
		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		if( data[0] == ACK )  break;		// ACK�̏ꍇ
		else{
			if( retry++ < 8 )continue;
			return(-4);						// �G���[���B
		}
	}
	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);
	return(0);
}



//========================================================================================================
//	int CMelsecP4TCP::Answer( char flg, char* station,char* pc )
//�@�@�\�@�FPLC�ɕԎ���Ԃ�
//	type�@�Fprivate
//	�����P�FACK or NAK
//	�����Q�F�ǔ�
//	�����R�F�o�b�ǔ�
//========================================================================================================
int CMelsecP4TCP::Answer( char* ip ,unsigned short port,char flg, char* station,char* pc )
{
	char buf[10];
	int ret;

	memset( buf, 0, sizeof(buf));

	buf[0]= flg;
	memcpy( buf+1, station, 2 );
	memcpy( buf+3, pc, 2 );
	buf[5]=CR;
	buf[6]=LF;

	ret=TcpSend( ip,port,buf,strlen(buf),NULL,0);
	return(0);
}

//========================================================================================================
//	void CMelsecP4TCP::MakeCmd(char* buf,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
//�@�@�\�@�F�v���g�R���S�`���ł̑��M�f�[�^���쐬
//	�����P�F�쐬�o�b�t�@
//	�����Q�F�쐬�o�b�t�@�T�C�Y
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F�R�}���h
//	�����U�F�擪�A�h���X
//	�����V�F�f�o�C�X�u���b�N��(max64)
//	type�@�Fpublic
//========================================================================================================
void CMelsecP4TCP::MakeCmd(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
{
	char txt[10];
	int sum;

	memset( buf, 0, bufsize );
	buf[0] = ENQ;

	// �ǔԁAPC�ǔ�
	strcat( buf, station );
	strcat( buf, pc );

	// �R�}���h
	strcat( buf, cmd );

	// �E�G�C�g
	wsprintf( txt, "%d", m_Wait );
	memcpy( &buf[strlen(buf)], txt, 1 );

	// �擪�f�o�C�X
	strcat( buf, addr );

	// �f�o�C�X�_��
	wsprintf( txt, "%02X", counts );
	memcpy( &buf[strlen(buf)], txt, 2 );

	// ���M���b�Z�[�W
	strcat( buf, msg );

	// ���ރ`�F�b�N
	sum = GetCheckSum(&buf[1]);
	wsprintf( txt, "%02X", sum );
	memcpy( &buf[strlen(buf)], txt, 2 );

	buf[strlen (buf)] = CR;
	buf[strlen (buf)] = LF;
}

//========================================================================================================
//	void CMelsecP4TCP::MakeCmd(char* buf,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
//�@�@�\�@�F�Q�b �v���g�R���S�`���ł̑��M�f�[�^���쐬
//	�����P�F�쐬�o�b�t�@
//	�����Q�F�쐬�o�b�t�@�T�C�Y
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F�R�}���h
//	�����U�F�擪�A�h���X
//	�����V�F�f�o�C�X�u���b�N��(max64)
//	type�@�Fpublic
//========================================================================================================
void CMelsecP4TCP::MakeCmd2C(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
{
	char txt[10];
	int sum;

	memset( buf, 0, bufsize );
	buf[0] = ENQ;
	buf[1] = 'F';
	buf[2] = 'B';


	// �ǔԁAPC�ǔ�
	strcat( buf, station );
	strcat( buf, pc );

	// �R�}���h
	strcat( buf, cmd );


	// �擪�f�o�C�X (8) Ex:ZR123456
	strcat( buf, addr );

	// �f�o�C�X�_�� (4)
	wsprintf( txt, "%04X", counts );
	memcpy( &buf[strlen(buf)], txt, 4 );

	// ���M���b�Z�[�W
	strcat( buf, msg );

	// ���ރ`�F�b�N
	sum = GetCheckSum(&buf[1]);
	wsprintf( txt, "%02X", sum );
	memcpy( &buf[strlen(buf)], txt, 2 );

	buf[strlen (buf)] = CR;
	buf[strlen (buf)] = LF;
}


//========================================================================================================
//	int CMelsecP4TCP::GetCheckSum(char* str )
//	type�@�Fprivate
//	�����@�F�v�Z���镶����
//	�Ԓl�@�F�Z�o����sam�`�F�b�N
//�@�@�\�@�Fsum�`�F�b�N�̌v�Z
//========================================================================================================
int CMelsecP4TCP::GetCheckSum(char* str )
{
	int data=0;
	int i,len;

	if( str==NULL ) return(0);

	len = strlen(str);
	if( len <=0 ) return(0);

	for( i=0; i< len; i++ )	data += (int)str[i];

	data = data & 0x000000ff;

	return(data);
}
//========================================================================================================
//	int CMelsecP4TCP::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	�@�\�@�F��������AMelsec���M�`���ɕϊ�����
//			ex) "A1234"->"31413433"
//	�����P�F�o�̓o�b�t�@
//	�����Q�F�o�b�t�@�T�C�Y�i�ϊ����f�[�^���̂Q�{+1�͕K�v�j
//	�����R�F�ϊ���������f�[�^(NULL-terminated)
//	�Ԓl�@�F0=OK
//========================================================================================================
int CMelsecP4TCP::Str2Mel(char* buf, unsigned int bufsize, char* str)
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
			wsprintf( txt,"%02X",(unsigned char)str[i+1]);		// 2011.03.11 coba
//			wsprintf( txt,"%02X",str[i+1]);
		memcpy( &buf[i*2], txt, 2 );

		wsprintf( txt,"%02X",(unsigned char)str[i]);		// 2011.03.11
//		wsprintf( txt,"%02X",str[i]);
		i++;
		memcpy( &buf[i*2], txt, 2 );
	}

	return(0);

}

//========================================================================================================
//	unsigned char CMelsecP4TCP::htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
int CMelsecP4TCP::Mel2Str(char* buf, unsigned int bufsize, char* melstr)
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
//	unsigned char CMelsecP4TCP::htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
unsigned char CMelsecP4TCP::htoi( char *hexstr , short len )
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
//========================================================================================================
//	int CMelsecP4TCP::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	�@�\�@�F��������AMelsec���M�`���ɕϊ�����
//			ex) "A1234"->"31413433"
//	�����P�F�o�̓o�b�t�@
//	�����Q�F�o�b�t�@�T�C�Y�i�ϊ����f�[�^���̂Q�{+1�͕K�v�j
//	�����R�F�ϊ���������f�[�^(NULL-terminated)
//	�Ԓl�@�F0=OK
//========================================================================================================
int CMelsecP4TCP::UStr2Mel(char* buf, unsigned int bufsize, unsigned char* str)
{
	unsigned int i,len;
	char txt[10];

	len = strlen((char*)str);
	if( len*2 >= bufsize ) return(-1);

	memset( buf, 0, bufsize );

	for( i=0;i<len;i++){
		// �㉺�����ւ���
		if( i+1==len )
			wsprintf( txt,"00");
		else
			wsprintf( txt,"%02X",(unsigned)str[i+1] & 0xFF);
		memcpy( &buf[i*2], txt, 2 );

		wsprintf( txt,"%02X",(unsigned)str[i] & 0xFF);
		i++;
		memcpy( &buf[i*2], txt, 2 );
	}

	return(0);

}


BOOL CMelsecP4TCP::ULChg( char* buf )
{
	char tmp[8];

	if( strlen(buf)<8 )
		return FALSE;

	memcpy( tmp , buf , 8);
	memcpy( buf+0 , tmp+4 , 4 );
	memcpy( buf+4 , tmp+0 , 4 );
	
	return TRUE;
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
int CMelsecP4TCP::TcpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize)
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
	printf( "WLAN=port=%d\n",WSAGetLastError());
		closesocket(soc);
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

	// ��M�Ȃ�
	if( rcv==NULL){
		shutdown(soc, 2);							// 2:SHUT_RDWR
		closesocket(soc);
		return(0);
	}


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
			// CR-LF��M
			if( rbuf[len2-2]==CR && rbuf[len2-1]==LF){
				// NAK�̏ꍇ
				if(rbuf[0]==NAK ) break;
				// ACK�̏ꍇ
				if(rbuf[0]==ACK ) break;
				// STX�̏ꍇ
				if(rbuf[0]==STX && len2 > 5){
					if(rbuf[len2-5]==ETX) break;
				}
			}
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

	if(!len2) return(-117);

	memset(rcv,0,rsize);
	memcpy( rcv, rbuf, len2> rsize ? rsize:len2);
	Sleep(10);	// �E�G�C�g
	return(len2);
}
