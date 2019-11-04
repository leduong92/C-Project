//========================================================================================================
//
//	FileName	:TPlcGpp.cpp
//	Class		:TPlcGpp (SubClass of "TRS232C")
// 				:�O�H�V�[�P���T�b�o�t�|�[�g�ł̃R���g���[��
//	�쐬��		:1998. 9.21	Coded by Shiba
//	�C����		: 
//
//========================================================================================================
#include <stdio.h>
#include <conio.h>

//#include "globldef.h"
#include "TPlcGpp.h"

extern HWND			ghwnd;

//===========================================================================================
//	TPlcGpp::TPlcGpp(void) 
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F�R���X�g���N�^
//===========================================================================================
TPlcGpp::TPlcGpp(void) 
{
	m_fOpen=FALSE;
	CpuType=0x00;
	// �ʐM�I�[�v��
//	PortOpen();
}


//===========================================================================================
//	TPlcGpp::~TPlcGpp()
//	�����@�F�Ȃ�
//	�Ԓl�@�F
//�@�@�\�@�F�f�X�g���N�^
//===========================================================================================
TPlcGpp::~TPlcGpp()
{
	TRS232C::SetRTS(FALSE);
	TRS232C::SetDTR(FALSE);
	TRS232C::Close();

}
//========================================================================================================
//	void TPlcGpp::PortClose(void)
//	�|�[�g�N���[�Y
//	�����F�Ȃ�
//	�Ԓl�F�Ȃ�
//========================================================================================================
void TPlcGpp::PortClose(void)
{
	TRS232C::SetRTS(FALSE);
	TRS232C::SetDTR(FALSE);
	TRS232C::Close();

	m_fOpen=FALSE;
	CpuType=0x00;
}
//========================================================================================================
//	BOOL TPlcGpp::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
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
BOOL TPlcGpp::Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait)
{
	// ����ݒ���OK
	if( m_fOpen ) return(TRUE);

	//m_Wait = wait;

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
	ps.fOutxCtsFlow = FALSE;	// TRUE or FALSE
	ps.fOutxDsrFlow = FALSE;	// TRUE or FALSE
	ps.fDtrControl = DTR_CONTROL_DISABLE;		// DTR_CONTROL_ENABLE or DTR_CONTROL_HANDSHAKE or DTR_CONTROL_HANDSHAKE
	ps.fRtsControl = RTS_CONTROL_ENABLE;		// RTS_CONTROL_HANDSHAKE or RTS_CONTROL_DISABLE or RTS_CONTROL_HANDSHAKE or RTS_CONTROL_TOGGLE


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

// �V�[�P���T�[�֌W�֐� �� ****************************************************


//-------------------------------------------------------------------------------------------
//   �֐����FPortOpen
//   ���@���Fret=PortOpen(void)
//   �������F�Ȃ�
//   �Ԃ��l�Fshort ret=0	:����
//                 ret=1�`6	:�ʐM�װ
//   �@�@�\�F�ʐM�I�[�v������
//-------------------------------------------------------------------------------------------
short TPlcGpp::PortOpen(void)
{
	DWORD theLength,nToRead;

    unsigned char buf[4],ReadBuf[32];
	int InQue;
//char msg[256];

	RTS_Off();							// ���M�O �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();

	SleepEx(500L,TRUE);

	buf[0]=0xAA;		//���ޑ��M  0xAA
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(1);

	SleepEx(14L,TRUE);	
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();

	ClearRcvBuffer();

    buf[0]=0xB0;		//���ޑ��M  0xB0
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(2);

	SleepEx(14L,TRUE);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0xAA;		//���ޑ��M  0xAA
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(3);

	SleepEx(14L,TRUE);	
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();

	buf[0]=0xB2;		//���ޑ��M  0xB2
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(4);

	SleepEx(14L,TRUE);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();
	SleepEx(100L,TRUE);


//	TRS232C::Status();
	InQue = TRS232C::ReadLength();			// ��M�޲Đ�
//	printf("\nInQue=%d\n",InQue);
//sprintf(msg,"InQue=%d",InQue);
//MessageBox(ghwnd,msg,MSGTITLE, MB_OK);

	nToRead=InQue;
	if( !TRS232C::Read((BYTE*)ReadBuf, nToRead) ) return(5);
//	for(int i=0 ;i< (int)nToRead;i++) printf("%02x:",ReadBuf[i]);
//sprintf(msg,"%02x:%02x:%02x:%02x",ReadBuf[0],ReadBuf[1],ReadBuf[2],ReadBuf[3]);
//MessageBox(ghwnd,msg,MSGTITLE, MB_OK);

	if( ReadBuf[0]!=0xB0 ) return(6);

	CpuType=ReadBuf[1];
//sprintf(msg,"%02x",CpuType);
//MessageBox(ghwnd,msg,MSGTITLE, MB_OK);

	return(0);

}

//-------------------------------------------------------------------------------------------
//   �֐����FPortOpenFx
//   ���@���Fret=PortOpenFx(void)
//   �������F�Ȃ�
//   �Ԃ��l�Fshort ret=0	:����
//                 ret=1�`6	:�ʐM�װ
//   �@�@�\�F�ʐM�I�[�v������
//-------------------------------------------------------------------------------------------
short TPlcGpp::PortOpenFx(HWND hwnd)
{
	DWORD theLength,nToRead;

    unsigned char buf[4],ReadBuf[32];
	int InQue;

	// �E�C���h�E�n���h���̑��
	hwParent = hwnd;

	RTS_Off();							// ���M�O �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();

	SleepEx(500L,TRUE);

	buf[0]=0x82;		//���ޑ��M  0x82
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(1);

	SleepEx(14L,TRUE);	
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();

    buf[0]=0x30;		//���ޑ��M  0x30
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(2);

	SleepEx(14L,TRUE);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0x30;		//���ޑ��M  0x30
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(3);

	SleepEx(14L,TRUE);	
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();

    buf[0]=0xC5;		//���ޑ��M  0xC5
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(4);

	SleepEx(14L,TRUE);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0x30;		//���ޑ��M  0x30
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(5);

	SleepEx(14L,TRUE);	
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();

    buf[0]=0xB2;		//���ޑ��M  0xB2
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(6);

	SleepEx(14L,TRUE);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0x30;		//���ޑ��M  0x30
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(7);

	SleepEx(14L,TRUE);	
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();

    buf[0]=0xB2;		//���ޑ��M  0xB2
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(8);

	SleepEx(14L,TRUE);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0x03;		//���ޑ��M  0x03
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(9);

	SleepEx(14L,TRUE);	
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();

    buf[0]=0x36;		//���ޑ��M  0x36
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(10);

	SleepEx(14L,TRUE);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();
	SleepEx(14L,TRUE);

	buf[0]=0xC3;		//���ޑ��M  0xC3
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(11);

	SleepEx(14L,TRUE);	
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();
/*
	buf[0]=0xB2;		//���ޑ��M  0xB2
	theLength=1;
	if( !TRS232C::Write( (BYTE*)buf, theLength ) ) return(12);
*/
	SleepEx(14L,TRUE);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();
	SleepEx(30L,TRUE);


//	TRS232C::Status();
	InQue = TRS232C::ReadLength();			// ��M�޲Đ�
//	printf("\nInQue=%d\n",InQue);

	nToRead=InQue;
	if( !TRS232C::Read((BYTE*)ReadBuf, nToRead) ) return(13);
//	for(int i=0 ;i< (int)nToRead;i++) printf("%02x:",ReadBuf[i]);
		char buf2[256];
		wsprintf(buf2,"%d",nToRead);
		//wsprintf(buf2,"%02X",ReadBuf[11]);
		MessageBox(hwParent,buf2,"SEQUENCER-PC",MB_OK|MB_ICONEXCLAMATION);

//	if( ReadBuf[0]!=0xB0 ) return(14);

	CpuType=ReadBuf[5];

	return(0);

}


//-------------------------------------------------------------------------------------------
//   �֐����FSRead
//   ���@���Fret=SRead( char memory, short address, short dvno, char *data )
//   �������Fchar  memory		:�ި�޲���				:'�c'
//           short address		:�擪�ި�޲����ڽ		:0�`1023
//           short dvno			:�ި�޲� �_��			:1�`128�܂�
//           char  *data		:�ǂ݂����ް��i�[�ޯ̧�	:�m�ۂ��Ă�
//   �Ԃ��l�Fshort ret=�ް���	:����
//           short ret=1		:�������װ
//   �@�@�\�F�l�d�l�n�q�x�Ƃ`�c�q�d�r�r�Ŏw�肵���擪�ި�޲�����A�c�u�m�n
//           �Ŏw�肵���ި�޲��_�������ް���ǂݍ���data�Ɋi�[����
//           SRead('D',0,10,buf);  D0000����D0009�̓ǂݏo��
//           D0=H0001 D2=H0005 �̂Ƃ��@buf[]="00010005......�Ɠ���B
//-------------------------------------------------------------------------------------------
short TPlcGpp::SRead( char memory, short address, short dvno, char *data )
{
	DWORD theLength,nToRead;

    unsigned char buf[512],ReadBuf[READ_LEN_MAX+1];
	short i,j,hex;
	int InQue,pnbyte;

	RTS_Off();							// ���M�O �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();

	buf[0]=CpuType;								// CPU����
	theLength=1;
	TRS232C::Write( (BYTE*)buf, theLength );

	Sleep(14);
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();

	// �`�����ނ̍쐬
	MakeCodeFunction(READ_DEVICES, 0, buf, 0);	// �����
	MakeCodeDevice(memory,address,dvno,buf,4);	// ���޲�
	SumCheck(buf,8);							// ������

// @
//	for( i=0; i<9; i++ ) printf(":%02X:",buf[i]);printf("\n");

	// ���ޑ��M
	theLength=9;
	TRS232C::Write( (BYTE*)buf, theLength );

	// ��M�҂�����
	Sleep(500);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();

// @
//	TRS232C::Status();

	// �ް��̎擾
	j=0;
	InQue=TRS232C::ReadLength();

// @
//	printf("InQue=%d\n",InQue);
	if( InQue > 0 )
	{
		nToRead=InQue;
		pnbyte = TRS232C::Read( (BYTE*)ReadBuf, nToRead );

		if( SumCheckC( ReadBuf, pnbyte ) ) return(1);
// @
//		printf("pnbyte=%d\n",pnbyte);
//		int tt = TRS232C::ReadLength();
//		printf("buflen=%d\n",tt);
//		for( i=0; i<pnbyte; i++ ) printf(":%02X:",ReadBuf[i]);printf("\n");

		for( i=4; i<pnbyte-1; i+=2 )
		{
			hex=((ReadBuf[i+1] & 0xF0)  >> 4);if(hex >= 0x0A) data[j++]=hex+'7';else data[j++]=hex+'0';
			hex=((ReadBuf[i+1] & 0x0F)		);if(hex >= 0x0A) data[j++]=hex+'7';else data[j++]=hex+'0';
			hex=((ReadBuf[i]   & 0xF0)  >> 4);if(hex >= 0x0A) data[j++]=hex+'7';else data[j++]=hex+'0';
			hex=((ReadBuf[i]   & 0x0F)		);if(hex >= 0x0A) data[j++]=hex+'7';else data[j++]=hex+'0';
		}
	}
	else return(InQue);

	return((pnbyte-5)*2);
}


//-------------------------------------------------------------------------------------------
//   �֐����FSWrite
//   ���@���Fret=SWrite( char memory, short address, char *data )
//   �������Fchar  memory		:�ި�޲�			:'�c'
//           short address		:�ި�޲����ڽ		:�O����P�O�Q�R
//           char  *data		:���������ް�
//   �Ԃ��l�Fshort ret=0		:����
//           short ret=1		:�װ
//   �@�@�\�F�l�d�l�n�q�x�Ƃ`�c�q�d�r�r�Ŏw�肵�����޲��ԍ����ް�data����������
//           SWrite('D',0,buf);  D0000�ɏ�������
//           D0=H0001 �Ə������ނƂ��@buf[]="0001"�Ɠ����B
//-------------------------------------------------------------------------------------------
short TPlcGpp::SWrite( char memory, short address, char *data)
{
	DWORD theLength,nToRead;

    unsigned char buf[512],ReadBuf[512],work1,work2;
	int byteswritten;
	int InQue,pnbyte;

	RTS_Off();							// ���M�O �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();

	buf[0]=CpuType;								// CPU����
	theLength=1;
	TRS232C::Write( (BYTE*)buf, theLength );

	Sleep(14);
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();

	// �`�����ނ̍쐬
	MakeCodeFunction(WRITE_DEVICES, 1, buf, 0);	// �����
	MakeCodeDevice(memory,address,1,buf,4);		// ���޲�
	work1=htoi(&data[0],2);
	work2=htoi(&data[2],2);
	buf[8]=work2;
	buf[9]=work1;
	SumCheck(buf,10);							// ������

	// ���ޑ��M
	theLength=11;
	byteswritten=TRS232C::Write( (BYTE*)buf, theLength );
//	for( int i=0; i<11; i++ ) printf(":%02X:",buf[i]);printf("\n");
	if( byteswritten!=11 ) return(1);

	Sleep(100);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();

	// ��M�ޯ̧�̸ر
	InQue=TRS232C::ReadLength();
	if( InQue > 0 )
	{
		nToRead=InQue;
		pnbyte = TRS232C::Read( (BYTE*)ReadBuf, nToRead );
	}
	if( pnbyte!=5 ) return(1);

	return(0);
}


//-------------------------------------------------------------------------------------------
//   �֐����FSWriteN
//   ���@���Fret=SWriteN( char memory, short address, char *data )
//   �������Fchar  memory		:�ި�޲�			:'�c'
//           short address		:�擪�ި�޲����ڽ	:�O����P�O�Q�R
//           short dvno			:�ި�޲� �_��		:�P����P�Q�W�܂�
//           char  *data		:���������ް�
//   �Ԃ��l�Fshort ret=0		:����
//           short ret=1		:�װ
//   �@�@�\�F�l�d�l�n�q�x�Ƃ`�c�q�d�r�r�Ŏw�肵�����޲��ԍ�����A�c�u�m�n
//           �Ŏw�肵���ި�޲��_�����A�ް�data����������
//           SWrite('D',0,10,buf);  D0000����D0009�ɏ�������
//           D0=H0001 D2=H0005 ...�Ə������ނƂ��@buf[]="00010005..."�Ɠ����B
//-------------------------------------------------------------------------------------------
short TPlcGpp::SWriteN( char memory, short address, short dvno, char *data )
{
	DWORD theLength,nToRead;

    unsigned char buf[512],ReadBuf[512],work1,work2;
	int i;
	int byteswritten;
	int InQue,pnbyte;
	short theLen;

	RTS_Off();							// ���M�O �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();

	buf[0]=CpuType;									// CPU����
	theLength=1;
	TRS232C::Write( (BYTE*)buf, theLength );

	Sleep(14);
	RTS_On();							// �q�s�r�E�c�s�q ���ɃI��
	DTR_On();

	// �`�����ނ̍쐬
	MakeCodeFunction(WRITE_DEVICES, dvno, buf, 0);	// �����
	MakeCodeDevice(memory,address,dvno,buf,4);		// ���޲�
	theLen=8;
	for( i=0; i<dvno*4; i+=4 )
	{
		work1=htoi(&data[i],2);
		work2=htoi(&data[i+2],2);
		buf[theLen++]=work2;
		buf[theLen++]=work1;
	}
	SumCheck(buf,theLen);							// ������

	// ���ޑ��M
	theLength=theLen+1;
	byteswritten=TRS232C::Write( (BYTE*)buf, theLength );
//	for( i=0; i<11; i++ ) printf(":%02X:",buf[i]);printf("\n");
	if( byteswritten!=theLen+1 ) return(1);

	Sleep(100);
	RTS_Off();							// �q�s�r�E�c�s�q ���ɃI�t
	DTR_Off();

	// ��M�ޯ̧�̸ر
	InQue=TRS232C::ReadLength();
	if( InQue > 0 )
	{
		nToRead=InQue;
		pnbyte = TRS232C::Read( (BYTE*)ReadBuf, nToRead );
	}
	if( pnbyte!=5 ) return(1);

	return(0);
}
//========================================================================================================
//	int TPlcGpp::Hex2Dec( char* hexstr, short len )
//	�@�\�@�F16�i�i������S�����̂݁j�𐮐��l(int�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//========================================================================================================
int TPlcGpp::Hex2Dec( char* hexstr, short len )
{
	int	ret;
	int hi,lo;

	if( len!=4 ) return(-1);
	len = strlen(hexstr);

	hi= htoi( &hexstr[0],2);
	lo= htoi( &hexstr[2],2);

	ret= hi*0x100 + lo;

	return(ret);
}

//-------------------------------------------------------------------------------------------
//	�֐����Fshort htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������Q�����̂݁j�𐮐��l(unsigned char�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//-------------------------------------------------------------------------------------------
unsigned char TPlcGpp::htoi( char *hexstr , short len )
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


//-------------------------------------------------------------------------------------------
//   �֐����FSumCheck
//   ���@���FSumCheck( unsigned char *p, short theLen )
//   �������Funsigned char *p	:��������t�����镶����
//           short theLen		:������̒���
//   �Ԃ��l�F�O�F����t��  �P�F�ُ�I��
//   �@�@�\�F���������ނ𕶎���Ō�ɕt������
//           �a�h�m�ް��Ƃ��ĉ��Z�������ʂ̉��ʂP�޲Ă��`�r�b�h�h�Q���ɕϊ�
//           �����ް��̍Ō�ɕt������
//-------------------------------------------------------------------------------------------
short TPlcGpp::SumCheck( unsigned char *p, short theLen )
{
	int i,sum=0;

	for(i=0;i<theLen;i++)
	{
		sum = sum + p[i];
	}

	sum = sum - (sum/256)*256;

	p[theLen]=(unsigned char)sum;

	return(0);
}


//-------------------------------------------------------------------------------------------
//   �֐����FSumCheckC
//   ���@���Fret=SumCheckC( char *p, short theLen )
//   �������Fchar *p:�����������镶����
//   �Ԃ��l�Fshort ret=0:����
//                 ret=1:��������-
//   �@�@�\�F����������
//-------------------------------------------------------------------------------------------
short TPlcGpp::SumCheckC( unsigned char *p, short theLen )
{
	int i,sum=0;

	for(i=0;i<theLen-1;i++)
	{
		sum = sum + p[i];
	}

	sum = sum - (sum/256)*256;

	if( p[theLen-1] != sum ) return(1);

	return(0);
}

//-------------------------------------------------------------------------------------------
//   �֐����FMakeCodeFunction
//   ���@���Fret=MakeCodeFunction(short function, short dvno, unsigned char *code, short theLen)
//   �������Fshort function		:�@�\
//           unsigned char *code:���M�ް��i�[�ޯ̧�
//           short theLen		:������̒���
//   �Ԃ��l�Fshort ret=0		:����
//           short ret=1		:�װ
//   �@�@�\�F�o�k�b�ԍ��ƁA�e�t�m�b�s�h�n�m�Ŏw�肵���@�\������
//           �`���ް����b�n�c�d�ɕt������
//-------------------------------------------------------------------------------------------
short TPlcGpp::MakeCodeFunction(short function, short dvno, unsigned char *code, short theLen)
{
	unsigned short n;

	if( dvno<1 || dvno>128 ) dvno=1;

	switch(function){
		case READ_DEVICES:						// Read Devices
			code[theLen]=0x07;
			code[theLen+1]=0x00;
			code[theLen+2]=0xFF;// PLC�ԍ�
			code[theLen+3]=0x02;// �ǂݏo��
			break;
		case WRITE_DEVICES:						// Write Devices
			n = 7 + dvno*2;
			code[theLen] = n%256;
			code[theLen+1]=n/256;
			code[theLen+2]=0xFF;// PLC�ԍ�
			code[theLen+3]=0x04;// ��������
			break;
		default:
			return(1);
	}
	return(0);
}

//-------------------------------------------------------------------------------------------
//   �֐����FMakeCodeDevice
//   ���@���Fret=MakeCodeDevice(short memory,short address,short dvno,unsigned char *code,short theLen)
//   �������Fshort memory		:�ި�޲�
//           short address		:�擪�ި�޲����ڽ
//           unsigned char *code:���M�ް��i�[�ޯ̧�
//           short theLen		:������̒���
//   �Ԃ��l�Fshort ret=0		:����
//           short ret=1		:�װ
//   �@�@�\�F�l�d�l�n�q�x�Ƃ`�c�q�d�r�r�Ŏw�肵���擪�ި�޲��ƁA�c�u�m�n��
//           �w�肵���ި�޲��_���������`���ް����b�n�c�d�ɕt������
//-------------------------------------------------------------------------------------------
short TPlcGpp::MakeCodeDevice(char memory, short address, short dvno, unsigned char *code, short theLen)
{
	unsigned short n;
	code[theLen+2]= 0x00;

	switch(memory){
		case 0x58:											// �w-Input
			if( address<0 || address>2047 ) address=0;
			n = 0x8000 + (address/16)*2;
			break;
		case 0x59:											// �x-Output
			if( address<0 || address>2047 ) address=0;
			n = 0x8200 + (address/16)*2;
			break;
		case 0x4D:											// �l-Internal Relay
			if( address<0 || address>2047 ) address=0;
			n = 0x8400 + (address/16)*2;
			break;
		case 0x4C:											// �k-Latch Relay
			if( address<0 || address>2047 ) address=0;
			n = 0x8400 + (address/16)*2;
			break;
		case 0x44:										// �c-Data Register
			if( address<0 || address>=WORD_DEVICE_MAXPOINT ) address=0;
//			n = 0x8800 + address*2;
			n = 0x4000 + address*2;
			code[theLen+2]= 0x08;
			// 0x4000���g�p����code[theLen+2]=0x08������B��address��6143�܂Ŏg����
			break;
		case 0x57:										// �v-Link Register
			if( address<0 || address>=WORD_DEVICE_MAXPOINT ) address=0;
			n = 0x9000 + address*2;
			break;
		default:
			return(1);
	}

	if( dvno<1 || dvno>128 ) dvno=1;

	code[theLen] = n%256;
	code[theLen+1]=n/256;
//	code[theLen+2]= 0x00;
	code[theLen+3]=(dvno * 2)%256;

	return(0);
}
