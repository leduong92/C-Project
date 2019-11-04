//================================================================================================
//
//	FileName	:CMelP4.cpp
//	Class		:CMelsecP4
// 				:Melsec �v���g�R���S�`���ʐM
//
//	�쐬��		:2001.12.13	Coded by coba
//
//
//================================================================================================
#include <stdio.h>
#include <conio.h>
#include "CMelP4.h"

//========================================================================================================
//	CMelsecP4::CMelsecP4(void)
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F�R���X�g���N�^�B�f�t�H���g�T�C�Y�Ńf�[�^�o�b�t�@���쐬
//========================================================================================================
CMelsecP4::CMelsecP4(void)
{
	m_fOpen=FALSE;
}

//========================================================================================================
//	CMelsecP4::~CMelsecP4()
//	�f�X�g���N�^
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F
//========================================================================================================
CMelsecP4::~CMelsecP4()
{
	TRS232C::SetRTS(FALSE);
	TRS232C::SetDTR(FALSE);
	TRS232C::Close();
}

//========================================================================================================
//	int CMelsecP4::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
//	type�@�Fprivate
//	�����P�F�|�[�g��
//	�����Q�F�{�[���[�g 1200,2400,4800,9600,19200
//	�����Q�F�f�[�^�r�b�g 7,8
//	�����Q�F�į���ޯ� 1,2
//	�����Q�F�p���e�B 0=�Ȃ��A 1=��A2=����
//	�����Q�F�E�G�C�g
//	�Ԓl�@�FTRUE or FALSE
//�@�@�\�@�F�|�[�g���J��
//========================================================================================================
BOOL CMelsecP4::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait)
{
	// ����ݒ���OK
	if( m_fOpen ) return(TRUE);

	m_Wait = wait;
	if( m_Wait < 0 || m_Wait > 0x0f ) m_Wait=0;

	PORTSETTINGEX ps;
	memset( &ps, 0, sizeof(PORTSETTINGEX));

	switch( baudrate ){
		case 1200:
			ps.BauRate=CBR_1200;				//1200bps
			break;
		case 2400:
			ps.BauRate=CBR_2400;				//1200bps
			break;
		case 4800:
			ps.BauRate=CBR_4800;				//1200bps
			break;
		case 19200:
			ps.BauRate=CBR_19200;				//1200bps
			break;
		case 9600:
		default:
			ps.BauRate=CBR_9600;				//1200bps
			break;
	}

	// DataBit
	switch( databit ){
		case 7:
			ps.DByte=7;						// �f�[�^�ޯ�=8
			break;
		case 8:
		default:
			ps.DByte=8;						// �f�[�^�ޯ�=8
			break;
	}

	// STOP BIT
	switch( stopbit){
		case 1:
			ps.StopBits=ONESTOPBIT;			// �į���ޯ�=1
			break;
		case 2:
		default:
			ps.StopBits=TWOSTOPBITS;			// �į���ޯ�=2
			break;
	}

	// PARITY
	switch( parity){
		case 1:
			ps.Parity=ODDPARITY;				// ��p���e�B
			break;
		case 2:									// ����
			ps.Parity=EVENPARITY;				// ��p���e�B
			break;
		case 0:
		default:
			ps.Parity=NOPARITY;				// �p���e�B�Ȃ�
			break;
	}
	ps.fBinary = TRUE;
	ps.fOutxCtsFlow = TRUE;	// TRUE or FALSE
	ps.fOutxDsrFlow = TRUE;		// TRUE or FALSE
	ps.fDtrControl = DTR_CONTROL_HANDSHAKE;		// DTR_CONTROL_DISABLE or DTR_CONTROL_ENABLE or DTR_CONTROL_HANDSHAKE
	ps.fRtsControl = RTS_CONTROL_HANDSHAKE;		// RTS_CONTROL_ENABLE or RTS_CONTROL_DISABLEE or RTS_CONTROL_HANDSHAKE or RTS_CONTROL_TOGGLE


	if(!TRS232C::CommOpenEx((long)portNo,&ps)){
		return(FALSE);
	}

	TRS232C::SetRTS(TRUE);
	TRS232C::SetDTR(TRUE);

	// ���f���̃|�[�g���N���A
	TRS232C::ClearRcvBuffer();
	m_fOpen=TRUE;

	return(TRUE);
}

void CMelsecP4::Close(void)
{
	m_fOpen=FALSE;
	TRS232C::SetRTS(FALSE);
	TRS232C::SetDTR(FALSE);
	TRS232C::Close();
}
//========================================================================================================
//	int CMelsecP4::P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h�f�[�^���u���b�N��M
//	�����P�F��M�o�b�t�@
//	�����Q�F��M�o�b�t�@�T�C�Y
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F��M�擪�A�h���X
//	�����U�F��M�u���b�N��(max64)
//	type�@�Fpublic
//========================================================================================================
int CMelsecP4::P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
{
	int ret,len;
	int retry=0,sum;
	char data[20],txt[10];

	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 64 ) return(-2);

	memset( data, 0, sizeof(data));
	// ���M���[�v
	while( retry < 8 ){
		//----���M������̍쐬-----
		// �ʏ�R�}���h
		if(strlen(addr) == 5 )
			MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "WR",addr,counts,"");
		// 2013.02.17 coba �g���R�}���h(D1023�ȍ~�Ή�)
		else MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "QR",addr,counts,"");

		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );

		//-----���ʎ�M------
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����STX or NAK�𒲂ׂ�
		if( ret ) return(-3);

		// �c��̏��������R�}���h�ʂɌ��߂�
		//-----STX�̏ꍇ(data����)------
		if( data[0] == STX ){
			// ETX�ɂȂ�܂Ŏ�M�𑱂���
			len =0;
			memset( m_buf, 0, sizeof(m_buf) );
			while( data[0] != ETX ){
				ret = Receive( data, sizeof(data), 1 );			// �c��̃o�C�g������M����
				if( ret ) return(-4);							// �G���[
				m_buf[len] = data[0];							// �o�b�t�@�ɂ��ߍ���
				if( ++len == sizeof(m_buf) ) return(-5); 						// �o�b�t�@�I�[�o�[�t���[
			}

			// �c��(4byte)����M
			ret = Receive( data, sizeof(data), 4 );
			if( ret ) return(-6);							// �G���[

			// �T���`�F�b�N���r
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			// OK�̏ꍇ��ACK��Ԃ��Đ���I��
			if( !memcmp( txt, data, 2 ) ){
				Answer(ACK,station,pcstation);
				break;
			}
			// NG�̎���NAK��Ԃ�
			Answer(NAK,station,pcstation);
			retry++;										// ��ײ����
			continue;
		}

		//-----NAK�̏ꍇ------
		else if( data[0] == NAK ){
			ret = Receive( data, sizeof(data), 8 );			// �c��̃o�C�g������M����
			if( ret ) return(-7);
			retry++;										// ��ײ����
			continue;
		}

		//----- �G���[���B-------
		else return(-8);
	}
	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);

	// ��M�f�[�^���o�b�t�@�ɺ�߰����
	memcpy( buf, m_buf+4,(int)(strlen(m_buf+4)-1) > bufsize ? bufsize:strlen(m_buf+4)-1);

	return(0);
}

//========================================================================================================
//	int CMelsecP4::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h���u���b�N���M
//	type�@�Fprivate
//	�����P�F�ǔ�
//	�����Q�FPC�ǔ�
//	�����R�F���M�擪�A�h���X
//	�����S�F���M�u���b�N��(max64)
//	�����T�F���M�f�[�^
//========================================================================================================
int CMelsecP4::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
{
	int ret,len;
	int retry=0;
	char data[20];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 64 ) return(-2);

	// �����I�[�o�[
	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);

	if( counts*4 != (int)strlen(msg) ) return(-11);

	// ���M������̍쐬
	// �ʏ�R�}���h
	if(strlen(addr) == 5 )
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "WW",addr,counts,msg);
	// 2013.02.17 coba �g���R�}���h(D1023�ȍ~�Ή�)
	else
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "QW",addr,counts,msg);
	//MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "WW",addr,counts,msg);

	// semd-loop
	while( retry < 8 ){
		// ���M
		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// ���ʎ�M
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����ACK or NAK�𒲂ׂ�
		if( ret ) return(-3);

		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		if( data[0] == ACK )  len = 6 ;		// ACK�̏ꍇ
		else if( data[0] == NAK )  len = 8;	// NAK�̏ꍇ
		else return(-4);						// �G���[���B

		// �c��̃o�C�g������M����
		ret = Receive( data, sizeof(data), len );
		if( ret ) return(-5);

		// NAK�̏ꍇ�̓G���[�Ƃ�����ײ����
		if( len == 8 ) retry++;

		// ACK�̏ꍇ��OK
		else break;
	}
	if( retry>=8) return(-9);
	return(0);
}

//========================================================================================================
//	int CMelsecP4::P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h�f�[�^���u���b�N��M�B�g���f�o�C�X�p
//	�����P�F��M�o�b�t�@
//	�����Q�F��M�o�b�t�@�T�C�Y
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F��M�擪�A�h���X
//	�����U�F��M�u���b�N��(max64)
//	type�@�Fpublic
//========================================================================================================
int CMelsecP4::P4ReadWEx( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
{
	int ret,len;
	int retry=0,sum;
	char data[20],txt[10];

	if( m_fOpen == FALSE ) return(-1);
	if( counts <1 || counts > 64 ) return(-2);

	memset( data, 0, sizeof(data));
	// ���M���[�v
	while( retry < 8 ){
		//----���M������̍쐬-----
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "QR",addr,counts,"");
		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
//intf("\nretry=%d,%s",retry,m_buf);

		//-----���ʎ�M------
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����STX or NAK�𒲂ׂ�
		if( ret ) return(-3);

		// �c��̏��������R�}���h�ʂɌ��߂�
		//-----STX�̏ꍇ(data����)------
		if( data[0] == STX ){
			// ETX�ɂȂ�܂Ŏ�M�𑱂���
			len =0;
			memset( m_buf, 0, sizeof(m_buf) );
			while( data[0] != ETX ){
				ret = Receive( data, sizeof(data), 1 );			// �c��̃o�C�g������M����
				if( ret ) return(-4);							// �G���[
				m_buf[len] = data[0];							// �o�b�t�@�ɂ��ߍ���
				if( ++len == sizeof(m_buf) ) return(-5); 						// �o�b�t�@�I�[�o�[�t���[
			}

			// �c��(4byte)����M
			ret = Receive( data, sizeof(data), 4 );
			if( ret ) return(-6);							// �G���[

			// �T���`�F�b�N���r
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			// OK�̏ꍇ��ACK��Ԃ��Đ���I��
			if( !memcmp( txt, data, 2 ) ){
				Answer(ACK,station,pcstation);
				break;
			}
			// NG�̎���NAK��Ԃ�
			Answer(NAK,station,pcstation);
			retry++;										// ��ײ����
			continue;
		}

		//-----NAK�̏ꍇ------
		else if( data[0] == NAK ){
			ret = Receive( data, sizeof(data), 8 );			// �c��̃o�C�g������M����
			if( ret ) return(-7);
			retry++;										// ��ײ����
			continue;
		}

		//----- �G���[���B-------
		else return(-8);
	}

	if( retry>=8) return(-9);

	// ��M�f�[�^���o�b�t�@�ɺ�߰����
	memcpy( buf, m_buf+4,(int)(strlen(m_buf+4)-1) > bufsize ? bufsize:strlen(m_buf+4)-1);

	return(0);
}


//========================================================================================================
//	int CMelsecP4::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h���u���b�N���M
//	type�@�Fprivate
//	�����P�F�ǔ�
//	�����Q�FPC�ǔ�
//	�����R�F���M�擪�A�h���X
//	�����S�F���M�u���b�N��(max64)
//	�����T�F���M�f�[�^
//========================================================================================================
int CMelsecP4::P4WriteWEx( char* station, char* pcstation,char* addr, short counts, char* msg)
{
	int ret,len;
	int retry=0;
	char data[20];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <1 || counts > 64 ) return(-2);

	// �����I�[�o�[
	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);

	if( counts*4 != (int)strlen(msg) ) return(-11);

	// ���M������̍쐬
	MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "QW",addr,counts,msg);

	// semd-loop
	while( retry < 8 ){
		// ���M
		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// ���ʎ�M
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����ACK or NAK�𒲂ׂ�
		if( ret ) return(-3);

		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		if( data[0] == ACK )  len = 6 ;		// ACK�̏ꍇ
		else if( data[0] == NAK )  len = 8;	// NAK�̏ꍇ
		else return(-4);						// �G���[���B

		// �c��̃o�C�g������M����
		ret = Receive( data, sizeof(data), len );
		if( ret ) return(-5);

		// NAK�̏ꍇ�̓G���[�Ƃ�����ײ����
		if( len == 8 ) retry++;

		// ACK�̏ꍇ��OK
		else break;
	}
	if( retry>=8) return(-9);
	return(0);
}

//========================================================================================================
//	int P4WriteREx( char* station, char* pcstation,char* addr, short counts, char* msg);
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h�f�[�^���g���t�@�C�����W�X�^�ɏ�������
//	type�@�Fprivate
//	�����P�F�ǔ�
//	�����Q�FPC�ǔ�
//	�����R�F���M�擪�A�h���X(nnnnnn,7��, ex "0012345")
//	�����S�F���M�u���b�N��(max64)
//	�����T�F���M�f�[�^
//========================================================================================================
int CMelsecP4::P4WriteREx( char* station, char* pcstation,char* addr, short counts, char* msg)
{
	int ret,len;
	int retry=0;
	char data[20];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <1 || counts > 64 ) return(-2);

	// �����I�[�o�[
	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);

	if( counts*4 != (int)strlen(msg) ) return(-11);

	// ���M������̍쐬
	MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "NW",addr,counts,msg);

	// semd-loop
	while( retry < 8 ){
		// ���M
		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// ���ʎ�M
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����ACK or NAK�𒲂ׂ�
		if( ret ) return(-3);

		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		if( data[0] == ACK )  len = 6 ;		// ACK�̏ꍇ
		else if( data[0] == NAK )  len = 8;	// NAK�̏ꍇ
		else return(-4);						// �G���[���B

		// �c��̃o�C�g������M����
		ret = Receive( data, sizeof(data), len );
		if( ret ) return(-5);

		// NAK�̏ꍇ�̓G���[�Ƃ�����ײ����
		if( len == 8 ) retry++;

		// ACK�̏ꍇ��OK
		else break;
	}
	if( retry>=8) return(-9);
	return(0);
}

//========================================================================================================
//	int CMelsecP4Ex::P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h�f�[�^���u���b�N��M
//	�����P�F��M�o�b�t�@
//	�����Q�F��M�o�b�t�@�T�C�Y
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F��M�擪�A�h���X
//	�����U�F��M�u���b�N��(max64)
//	type�@�Fpublic
//========================================================================================================
int CMelsecP4::P2C4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts)
{
	int ret,len;
	int retry=0;
	char data[4096];
	char lsum[4096];
	char initcom[5];
	char st;
	char sumt[16];

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 960 ) return(-2);

	// �����I�[�o�[
//	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);
//
//	if( counts*4 != (int)strlen(msg) ) return(-11);

	// ���M������̍쐬(2C)
	MakeCmd2C( m_buf, sizeof(m_buf), station,pcstation, "2",addr,counts,"");

	// semd-loop
	while( retry < 8 ){
		if( retry ){
			TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
			ret = TRS232C::Write( (BYTE*)initcom, 3 );
		}		
		// ���M
		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// ���ʎ�M
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����ACK or NAK�𒲂ׂ�
		if( ret ){
			if( retry++ < 8 )continue;
			return(-3);
		}
		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		st=data[0];
		if( data[0] == STX )len = counts*4+9;	// STX�̏ꍇ
		else if( data[0] == NAK )  len = 12;	// NAK�̏ꍇ
		else{
			if( retry++ < 8 )continue;
			return(-4);						// �G���[���B
		}
		// �c��̃o�C�g������M����
		ret = Receive( data, sizeof(data), len );
		if( ret ){
			if( retry++ < 8 )continue;
			return(-5);
		}
		if( st==STX ){
			memset(lsum,0,sizeof(lsum));
			memcpy(lsum,data,len-2);
			sprintf(sumt,"%02X",GetCheckSum(lsum));
		}

		// NAK�̏ꍇ�̓G���[�Ƃ�����ײ����
		// �T���`�F�b�N�G���[����������
		if( st == NAK || memcmp(sumt,data+len-2,2) ) retry++;

		// ACK�̏ꍇ��OK
		else break;
	}
	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);

	// ��M�f�[�^���o�b�t�@�ɺ�߰����
	memcpy( buf, lsum+6,(int)(strlen(lsum+6)-1) > bufsize ? bufsize:strlen(lsum+6)-1);
	return(0);
/*
	int ret,len;
	int retry=0;
	char data[4096];
	char lsum[4096];
	char initcom[5];
	char st;
	char sumt[16];

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 960 ) return(-2);

	// �����I�[�o�[
//	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);
//
//	if( counts*4 != (int)strlen(msg) ) return(-11);

	// ���M������̍쐬(2C)
	MakeCmd2C( m_buf, sizeof(m_buf), station,pcstation, "2",addr,counts,"");

	// semd-loop
	while( retry < 8 ){
		if( retry ){
			TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
			ret = TRS232C::Write( (BYTE*)initcom, 3 );
		}		
		// ���M
		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// ���ʎ�M
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����ACK or NAK�𒲂ׂ�
		if( ret ){
			if( retry++ < 8 )continue;
			return(-3);
		}
		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		st=data[0];
		if( data[0] == STX )len = counts*4+9;	// STX�̏ꍇ
		else if( data[0] == NAK )  len = 12;	// NAK�̏ꍇ
		else{
			if( retry++ < 8 )continue;
			return(-4);						// �G���[���B
		}
		// �c��̃o�C�g������M����
		ret = Receive( data, sizeof(data), len );
		if( ret ){
			if( retry++ < 8 )continue;
			return(-5);
		}
		if( st==STX ){
			memset(lsum,0,sizeof(lsum));
			memcpy(lsum,data,len-2);
			sprintf(sumt,"%02X",GetCheckSum(lsum));
		}

		// NAK�̏ꍇ�̓G���[�Ƃ�����ײ����
		// �T���`�F�b�N�G���[����������
		if( st == NAK || memcmp(sumt,data+len-2,2) ) retry++;

		// ACK�̏ꍇ��OK
		else break;
	}
	// ��M�f�[�^���o�b�t�@�ɺ�߰����
	memcpy( buf, lsum+6,(int)(strlen(lsum+6)-1) > bufsize ? bufsize:strlen(lsum+6)-1);

	return(0);

*/
}

//========================================================================================================
//	int CMelsecP4Ex::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//�@�@�\�@�F�v���g�R���S�`���Ń��[�h���u���b�N���M
//	type�@�Fprivate
//	�����P�F�ǔ�
//	�����Q�FPC�ǔ�
//	�����R�F���M�擪�A�h���X
//	�����S�F���M�u���b�N��(max64)
//	�����T�F���M�f�[�^
//========================================================================================================
int CMelsecP4::P2C4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
{
	int ret,len;
	int retry=0;
	char data[20];
	char initcom[5];

	initcom[0]=4;
	initcom[1]=0xD;
	initcom[2]=0xA;
	initcom[3]=0;


	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( counts <0 || counts > 960 ) return(-2);

	// �����I�[�o�[
	if( strlen(addr)+strlen(msg) + P4_HEADLEN >= sizeof(m_buf) ) return(-10);

	if( counts*4 != (int)strlen(msg) ) return(-11);

	// ���M������̍쐬(2C)
	MakeCmd2C( m_buf, sizeof(m_buf), station,pcstation, "4",addr,counts,msg);

	// semd-loop
	while( retry < 8 ){
		if( retry ){
			TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
			ret = TRS232C::Write( (BYTE*)initcom, 3 );
		}		
		// ���M
		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// ���ʎ�M
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����ACK or NAK�𒲂ׂ�
		if( ret ){
			if( retry++ < 8 )continue;
			return(-3);
		}
		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		if( data[0] == ACK )  len = 6 ;		// ACK�̏ꍇ
		else if( data[0] == NAK )  len = 12;	// NAK�̏ꍇ
		else{
			if( retry++ < 8 )continue;
			return(-4);						// �G���[���B
		}
		// �c��̃o�C�g������M����
		ret = Receive( data, sizeof(data), len );
		if( ret ){
			if( retry++ < 8 )continue;
			return(-5);
		}
		// NAK�̏ꍇ�̓G���[�Ƃ�����ײ����
		if( len == 12 ) retry++;

		// ACK�̏ꍇ��OK
		else break;
	}
	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);
	return(0);
}

//========================================================================================================
//	int CMelsecP4::P4ReadB( char* station,char* pcstation,char* addr)
//	type�@�Fpublic
//�@�@�\�@�F�v���g�R���S�`���Ńr�b�g�f�[�^���P�f�o�C�X��M
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F��M�擪�A�h���X
//  �߂�l�F0=OFF,1=ON,0����=�G���[
//========================================================================================================
int CMelsecP4::P4ReadB( char* station,char* pcstation,char* addr)
{
	int ret,len;
	int retry=0,sum;
	char data[20],txt[10];
	char buf;

	if( m_fOpen == FALSE ) return(-1);

	memset( data, 0, sizeof(data));
	// ���M���[�v
	while( retry < 8 ){
		//----���M������̍쐬-----
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "BR",addr,8,"");

		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );

		//-----���ʎ�M------
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����STX or NAK�𒲂ׂ�
		if( ret ) return(-3);

		// �c��̏��������R�}���h�ʂɌ��߂�
		//-----STX�̏ꍇ(data����)------
		if( data[0] == STX ){
			// ETX�ɂȂ�܂Ŏ�M�𑱂���
			len =0;
			memset( m_buf, 0, sizeof(m_buf) );
			while( data[0] != ETX ){
				ret = Receive( data, sizeof(data), 1 );			// �c��̃o�C�g������M����
				if( ret ) return(-4);							// �G���[
				m_buf[len] = data[0];							// �o�b�t�@�ɂ��ߍ���
				if( ++len == sizeof(m_buf) ) return(-5); 						// �o�b�t�@�I�[�o�[�t���[
			}

			// �c��(4byte)����M
			ret = Receive( data, sizeof(data), 4 );
			if( ret ) return(-6);							// �G���[

			// �T���`�F�b�N���r
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			// OK�̏ꍇ��ACK��Ԃ��Đ���I��
			if( !memcmp( txt, data, 2 ) ){
				Answer(ACK,station,pcstation);
				break;
			}
			// NG�̎���NAK��Ԃ�
			Answer(NAK,station,pcstation);
			retry++;										// ��ײ����
			continue;
		}

		//-----NAK�̏ꍇ------
		else if( data[0] == NAK ){
			ret = Receive( data, sizeof(data), 8 );			// �c��̃o�C�g������M����
			if( ret ) return(-7);
			retry++;										// ��ײ����
			continue;
		}

		//----- �G���[���B-------
		else return(-8);
	}
	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);

	// ��M�f�[�^���o�b�t�@�ɺ�߰����
	buf=m_buf[4];

	return(buf-0x30);
}
//========================================================================================================
//	int CMelsecP4::P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg)
//�@�@�\�@�F�v���g�R���S�`���Ńr�b�g�f�o�C�X(1�_)��ON/OFF
//	type�@�Fprivate
//	�����P�F�ǔ�
//	�����Q�FPC�ǔ�
//	�����R�F���M�A�h���X
//	�����T�F���M�f�[�^ 0=OFF,����ȊO=ON
//	�߂�l�F0.=���� ����ȊO=�G���[
//========================================================================================================
int CMelsecP4::P4WriteB( char* station, char* pcstation,char* addr, int onoff)
{
	int ret,len;
	int retry=0;
	char data[20];
	char send[10];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);

	// ���M������̍쐬
	if( !onoff ) strcpy(send,"0\0");
	else strcpy(send,"1\0");				// off

	MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "BW",addr,1,send);

	// semd-loop
	while( retry < 8 ){
		// ���M
		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// ���ʎ�M
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����ACK or NAK�𒲂ׂ�
		if( ret ) return(-3);

		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		if( data[0] == ACK )  len = 6 ;		// ACK�̏ꍇ
		else if( data[0] == NAK )  len = 8;	// NAK�̏ꍇ
		else return(-4);						// �G���[���B

		// �c��̃o�C�g������M����
		ret = Receive( data, sizeof(data), len );
		if( ret ) return(-5);

		// NAK�̏ꍇ�̓G���[�Ƃ�����ײ����
		if( len == 8 ) retry++;

		// ACK�̏ꍇ��OK
		else break;
	}
	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);
	return(0);
}

//========================================================================================================
//	int CMelsecP4::Receive( char* buf, int bufsize, int waitlen )
//�@�@�\�@�F��M�o�b�t�@����f�[�^��ǂݏo��
//	type�@�Fprivate
//	�����P�F�o�b�t�@
//	�����Q�F�o�b�t�@�T�C�Y
//	�����R�F��M�҂�����f�[�^�o�C�g��
//========================================================================================================
int CMelsecP4::Receive( char* buf, int bufsize, int waitlen )
{
	int theLen,prevLen=0;
	int counter = 0;

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);
	if( waitlen <1 ) return(-2);
	if( bufsize < waitlen ) return(-2);

	memset( buf, 0, bufsize );

	while(1){
		// ��M�o�b�t�@�Ƀf�[�^�����Ă��邩���ׂ�
		theLen = TRS232C::ReadLength();
		// �w�肵���o�C�g���ɒB����
		if( theLen >= waitlen ){
			break;
		}
		// �w�肵���o�C�g���ɖ����Ȃ�
		else{
			Sleep(15);
			if( theLen != prevLen ){	// �O��葝���Ă�����J�E���^���Z�b�g
				theLen = prevLen;
				counter=0;
			}
			else counter++;				// ��M�o�C�g�����ς��Ȃ�
		}
		// �^�C���A�E�g
		if( counter > 40 ) return(-3);
	}

	// ����I��
	TRS232C::Read( (BYTE*)buf, waitlen );
	return(0);
}

//========================================================================================================
//	int CMelsecP4::Answer( char flg, char* station,char* pc )
//�@�@�\�@�FPLC�ɕԎ���Ԃ�
//	type�@�Fprivate
//	�����P�FACK or NAK
//	�����Q�F�ǔ�
//	�����R�F�o�b�ǔ�
//========================================================================================================
int CMelsecP4::Answer( char flg, char* station,char* pc )
{
	char buf[10];
	int ret;

	memset( buf, 0, sizeof(buf));

	buf[0]= flg;
	memcpy( buf+1, station, 2 );
	memcpy( buf+3, pc, 2 );
	buf[5]=CR;
	buf[6]=LF;

	ret = TRS232C::Write( (BYTE*)buf, strlen(buf) );
	return(0);
}

//========================================================================================================
//	void CMelsecP4::MakeCmd(char* buf,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
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
void CMelsecP4::MakeCmd(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
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
	wsprintf( txt, "%X", m_Wait );	// 2004.07.13
//	wsprintf( txt, "%d", m_Wait );
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
//	void CMelsecP4Ex::MakeCmd(char* buf,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
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
void CMelsecP4::MakeCmd2C(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg)
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
//	int CMelsecP4::GetCheckSum(char* str )
//	type�@�Fprivate
//	�����@�F�v�Z���镶����
//	�Ԓl�@�F�Z�o����sam�`�F�b�N
//�@�@�\�@�Fsum�`�F�b�N�̌v�Z
//========================================================================================================
int CMelsecP4::GetCheckSum(char* str )
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
//	int CMelsecP4::Str2Mel(char* buf, unsigned int bufsize, char* str)
//	�@�\�@�F��������AMelsec���M�`���ɕϊ�����
//			ex) "A1234"->"31413433"
//	�����P�F�o�̓o�b�t�@
//	�����Q�F�o�b�t�@�T�C�Y�i�ϊ����f�[�^���̂Q�{+1�͕K�v�j
//	�����R�F�ϊ���������f�[�^(NULL-terminated)
//	�Ԓl�@�F0=OK
//========================================================================================================
int CMelsecP4::Str2Mel(char* buf, unsigned int bufsize, char* str)
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

		wsprintf( txt,"%02X",(unsigned char)str[i]);			// 2011.03.11 coba
//		wsprintf( txt,"%02X",str[i]);
		i++;
		memcpy( &buf[i*2], txt, 2 );
	}

	return(0);

}

//========================================================================================================
//	unsigned char CMelsecP4::htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
int CMelsecP4::Mel2Str(char* buf, unsigned int bufsize, char* melstr)
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
//	unsigned char CMelsecP4::htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
unsigned char CMelsecP4::htoi( char *hexstr , short len )
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
//	int CMelsecP4Ex::P4ReadB( char* station,char* pcstation,char* addr)
//	type�@�Fpublic
//�@�@�\�@�F�v���g�R���S�`���Ńr�b�g�f�[�^���P�f�o�C�X��M
//	�����R�F�ǔ�
//	�����S�FPC�ǔ�
//	�����T�F��M�擪�A�h���X
//  �߂�l�F0=OFF,1=ON,0����=�G���[
//========================================================================================================
int CMelsecP4::P4ReadBB(  char* buf,int bufsize,char* station,char* pcstation,char* addr,short cnt)
{
	int ret,len;
	int retry=0,sum;
	char data[20],txt[10];
//	char buf;

	if( m_fOpen == FALSE ) return(-1);

	memset( data, 0, sizeof(data));
	// ���M���[�v
	while( retry < 8 ){
		//----���M������̍쐬-----
		MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "BR",addr,cnt,"");

		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );

		//-----���ʎ�M------
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����STX or NAK�𒲂ׂ�
		if( ret ) return(-3);

		// �c��̏��������R�}���h�ʂɌ��߂�
		//-----STX�̏ꍇ(data����)------
		if( data[0] == STX ){
			// ETX�ɂȂ�܂Ŏ�M�𑱂���
			len =0;
			memset( m_buf, 0, sizeof(m_buf) );
			while( data[0] != ETX ){
				ret = Receive( data, sizeof(data), 1 );			// �c��̃o�C�g������M����
				if( ret ) return(-4);							// �G���[
				m_buf[len] = data[0];							// �o�b�t�@�ɂ��ߍ���
				if( ++len == sizeof(m_buf) ) return(-5); 						// �o�b�t�@�I�[�o�[�t���[
			}

			// �c��(4byte)����M
			ret = Receive( data, sizeof(data), 4 );
			if( ret ) return(-6);							// �G���[

			// �T���`�F�b�N���r
			sum = GetCheckSum( m_buf );
			wsprintf(txt, "%02X", sum);

			// OK�̏ꍇ��ACK��Ԃ��Đ���I��
			if( !memcmp( txt, data, 2 ) ){
				Answer(ACK,station,pcstation);
				break;
			}
			// NG�̎���NAK��Ԃ�
			Answer(NAK,station,pcstation);
			retry++;										// ��ײ����
			continue;
		}

		//-----NAK�̏ꍇ------
		else if( data[0] == NAK ){
			ret = Receive( data, sizeof(data), 8 );			// �c��̃o�C�g������M����
			if( ret ) return(-7);
			retry++;										// ��ײ����
			continue;
		}

		//----- �G���[���B-------
		else return(-8);
	}

	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);

	// ��M�f�[�^���o�b�t�@�ɺ�߰����
	memcpy( buf, m_buf+4,(int)(strlen(m_buf+4)-1) > bufsize ? bufsize:strlen(m_buf+4)-1);

	return(0);
}

//========================================================================================================
// int CMelsecP4Ex::P4WriteBB( char* station, char* pcstation,char* addr, int cnt,char* dat)
//	2012.06.20 coba�ǉ�
//�@�@�\�@�F�v���g�R���S�`���Ńr�b�g�f�o�C�X�����_���ꊇON/OFF
//	type�@�Fprivate
//	�����P�F�ǔ�
//	�����Q�FPC�ǔ�
//	�����R�F���M�A�h���X
//	�����T�F���M�f�[�^ 0=OFF,1=ON�̔z��
//	�߂�l�F0.=���� ����ȊO=�G���[
//========================================================================================================
int CMelsecP4::P4WriteBB( char* station, char* pcstation,char* addr, int cnt,char* dat)
{
	int ret,len;
	int retry=0;
	char data[20];

	// ���I�[�v���G���[
	if( m_fOpen == FALSE ) return(-1);

	// �Z�b�g���郌�W�X�^��=�f�[�^�o�C�g���Ȃ̂Ń`�F�b�N
	if( (int)strlen(dat) != cnt ) return(-2);

	MakeCmd( m_buf, sizeof(m_buf), station,pcstation, "BW",addr,cnt,dat);

	// semd-loop
	while( retry < 8 ){
		// ���M
		TRS232C::ClearRcvBuffer();				// ���f���̃|�[�g���N���A
		ret = TRS232C::Write( (BYTE*)m_buf, strlen(m_buf) );
		//if(ret !=strlen(buf) ) return(-3);

		// ���ʎ�M
		ret = Receive( data, sizeof(data), 1 );		// 1�o�C�g��M����ACK or NAK�𒲂ׂ�
		if( ret ) return(-3);

		// �c��̃o�C�g�����R�}���h�ʂɌ��߂�
		if( data[0] == ACK )  len = 6 ;		// ACK�̏ꍇ
		else if( data[0] == NAK )  len = 8;	// NAK�̏ꍇ
		else return(-4);						// �G���[���B

		// �c��̃o�C�g������M����
		ret = Receive( data, sizeof(data), len );
		if( ret ) return(-5);

		// NAK�̏ꍇ�̓G���[�Ƃ�����ײ����
		if( len == 8 ) retry++;

		// ACK�̏ꍇ��OK
		else break;
	}
	// 2013.04.11 coba ���g���C��over�ł�0�ŕԂ��Ă���
	if(retry >= 8) return(-9);
	return(0);
}

