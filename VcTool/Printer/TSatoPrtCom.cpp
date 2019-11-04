//===========================================================================
//
//  	���Ɓ[�o�[�R�[�h�v�����^�[�̐����{�N���X
//	
//		FileName : tsatoprt.cpp
//		�m�n�j������� �Z�p�{�� ���Y�Z�p��
//		2001.6.15 coba
//===========================================================================


#include "tsatoprtcom.h"
#include <stdio.h>

//---------------------------------------------------------------------------------------------------------------------
//	TSatoPrinter::TSatoPrinter( char* ptrname, int ptrtype, BOOL* status )
//	�^�C�v�Fpublic
//	�@�\�@�F�|�[�g�w��R���X�g���N�^
//	�����P�F�v�����^�^�C�v 400�n=0,����ȊO=410
//	�����Q�F�g�p����|�[�g�ԍ�
//	�����R�F�{�[���[�g
//	�����S�F�f�[�^�r�b�g
//	�����T�F�X�g�b�v�r�b�g
//	�����U�F�p���e�B
//	�����V�F�i�o�́j�������ð�� True or False
//---------------------------------------------------------------------------------------------------------------------
TSatoPrinter::TSatoPrinter( int ptrtype,int portNo, int speed,int data,int stop,int parity, BOOL* status )
{
	*status = FALSE;
	m_Com = NULL;

	// 232C�J��
	if( InitPrinter(ptrtype, portNo,speed,data,stop,parity)){
		return;
	}

	*status = TRUE;

}
//---------------------------------------------------------------------------------------------------------------------
//	TSatoPrinter::TSatoPrinter( char* ptrname, int ptrtype, BOOL* status )
//	�^�C�v�Fpublic
//	�@�\�@�F�f�t�H���g�R���X�g���N�^
//		���̌�AInitPrinter()�ŏ��������K�v
//---------------------------------------------------------------------------------------------------------------------
TSatoPrinter::TSatoPrinter()
{
	m_Com = NULL;
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TSatoPrinter::InitPrinter( int portNo, PORTSETTING* pset,int prtype)
//	�^�C�v�Fpublic
//	�@�\�@�F�|�[�g�w��R���X�g���N�^
//	�����P�F�g�p����|�[�g�ԍ�
//	�����Q�F�|�[�g�̐ݒ�
//	�����R�F�v�����^�^�C�v 400�n=0,����ȊO=410
//	�߂�l�F�������ð�� True or False
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::InitPrinter( int ptrtype,int portNo, int speed,int data,int stop,int parity)
{
	PORTSETTING ps;

	// �������ς�
	if( m_Com != NULL ) return(-1);

	// �����o�ϐ��̏�����
	m_prtType = ptrtype;


	memset( &ps, 0, sizeof(PORTSETTING));
	switch( speed ){
		case 300:
			ps.BauRate=CBR_300;				//1200bps
			break;
		case 600:
			ps.BauRate=CBR_600;				//1200bps
			break;
		case 1200:
			ps.BauRate=CBR_1200;				//1200bps
			break;
		case 2400:
			ps.BauRate=CBR_2400;				//bps
			break;
		case 4800:
			ps.BauRate=CBR_4800;				//bps
			break;
		case 14400:
			ps.BauRate=CBR_14400;				//bps
			break;
		case 19200:
			ps.BauRate=CBR_19200;				//bps
			break;
		case 28800:
			ps.BauRate=28800;				//bps
			break;
		case 38400:
			ps.BauRate=CBR_38400;				//bps
			break;
		case 57600:
			ps.BauRate=CBR_57600;				//bps
			break;
		case 115200:
			ps.BauRate=CBR_115200;				//bps
			break;
		case 9600:
		default:
			ps.BauRate=CBR_9600;				//1200bps
			break;
	}

	// DataBit
	switch( data ){
		case 7:
			ps.DByte=7;						// �f�[�^�ޯ�=8
			break;
		case 8:
		default:
			ps.DByte=8;						// �f�[�^�ޯ�=8
			break;
	}

	// STOP BIT
	switch( stop){
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
		case 2:									// 
			ps.Parity=EVENPARITY;				// �����p���e�B
			break;
		case 0:
		default:
			ps.Parity=NOPARITY;				// �p���e�B�Ȃ�
			break;
	}

	// 232C�J��
	m_Com = NULL;
	m_Com = new TRS232C;
	if( NULL == m_Com ) return(-2);

	if(portNo<1) portNo=1;

	// TRs232C�N���X�̃C���X�^���X���쐬
	if(!m_Com->CommOpen(portNo,&ps)){
		delete m_Com;
		m_Com = NULL;
		return(-3);
	}

	// 0.1mm������̃h�b�g�����Z�o
	m_dpm = 0.0;
	// 400�n(203dpi 1�h�b�g=0.125mm)
	if( ptrtype==SATO_ER400)m_dpm = 0.1/0.125;
	// 410�n(305dpi 1�h�b�g=0.083mm)
	else m_dpm = 0.1/0.083;

	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	TSatoPrinter::~TSatoPrinter()
//	�^�C�v�Fpublic
//	�@�\�@�F�f�X�g���N�^
//---------------------------------------------------------------------------------------------------------------------
TSatoPrinter::~TSatoPrinter()
{
	if( m_Com !=NULL){
		m_Com->Close();
		delete m_Com;
		m_Com = NULL;
	}
}


//---------------------------------------------------------------------------------------------------------------------
//	TSatoPrinter::TSatoPrinter( char* ptrname, int ptrtype, BOOL* status )
//	�^�C�v�Fpublic
//	�@�\�@�F����X�^�[�g
//			232C�łł́A�݊���ۂׂ̃_�~�[
//---------------------------------------------------------------------------------------------------------------------
BOOL TSatoPrinter::PrintStart(void)
{
	if( m_Com == NULL ) return(FALSE);

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	TSatoPrinter::TSatoPrinter( char* ptrname, int ptrtype, BOOL* status )
//	�^�C�v�Fpublic
//	�@�\�@�F����I��
//---------------------------------------------------------------------------------------------------------------------
BOOL TSatoPrinter::PrintEnd(void)
{
	if( m_Com == NULL ) return(TRUE);
	m_Com->Close();
	delete m_Com;
	m_Com = NULL;

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::StartLabel(int mode)
//	�^�C�v�Fpublic
//	�@�\�@�F������StartPage()
//	�����P�F��] �R�}���hESC+%�ɑΉ�
//			�i�v�����^���ʂ���݂āj�������=0
//			1=90�x��]
//			2=180�x��](�t��)
//			3=270�x��](�t��)
//	�����Q�F�󎚑��x 2�`6
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::StartLabel(int mode,char speed)
{
	if( m_Com == NULL ) return(-1);

	char cmd[20];
	int ret;

	// �ް����M�J�n STX+ESCA
	cmd[0] = S_STX;
	cmd[1] = S_ESC;
	cmd[2] = 'A';
	cmd[3] = 0;
	if( mode >0 && mode <4){
		cmd[3] = S_ESC;
		cmd[4] = '%';
		cmd[5] = '0'+(char)mode;
		cmd[6] = 0;
	}
	ret = SendData(cmd,(int)strlen(cmd));
	if(ret)return(ret);

	// S-JIS�R�[�h
	// �ް����M�J�n ESCKC
	cmd[0] = S_ESC;
	cmd[1] = 'K';
	cmd[2] = 'C';
	cmd[3] = '1';					// 1=S-JIS
	cmd[4] = 0;
	ret = SendData(cmd,(int)strlen(cmd));
	if(ret)return(ret);

	// �A������offset�w�� ESCPO
	cmd[0] = S_ESC;
	cmd[1] = 'P';
	cmd[2] = 'O';
	cmd[3] = '3';
	cmd[4] = '+';
	cmd[5] = '0';
	cmd[6] = '0';
	cmd[7] = 0;
	ret = SendData(cmd,(int)strlen(cmd));
	if(ret)return(ret);

	// TearOff����offset�w�� ESCPO
	cmd[0] = S_ESC;
	cmd[1] = 'P';
	cmd[2] = 'O';
	cmd[3] = '2';
	cmd[4] = '+';
	cmd[5] = '0';
	cmd[6] = '0';
	cmd[7] = 0;
	ret = SendData(cmd,(int)strlen(cmd));
	if(ret)return(ret);

	// �󎚑��x
	if( speed < 2 ) speed = 2;		// �Œ�l
	if( speed > 6 ) speed = 6;

	// 410�n�͂S��
	if( m_prtType==SATO_ER410 && speed > 4 ) speed=4;
	cmd[0] = S_ESC;
	cmd[1] = 'C';
	cmd[2] = 'S';
	cmd[3] =  speed+0x30;
	cmd[4] = 0;
	ret = SendData(cmd,(int)strlen(cmd));


	return(ret);

}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::EndLabel(short page)
//	�^�C�v�Fpublic
//	�@�\�@�F������EndPage()
//	�����@�F��߰����
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::EndLabel(long page)
{
	if( m_Com == NULL ) return(-1);

	// ���s����
	if( page < 1 || page > 999999 ) page=1;

	char cmd[20];

	// �ް����M�I��ESCQ+maisu+ESCZ+ETX
	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	cmd[1] = 'Q';
	wsprintf( &cmd[2],"%ld", page);
	cmd[strlen(cmd)]=S_ESC;
	cmd[strlen(cmd)]='Z';
	cmd[strlen(cmd)]=S_ETX;

	return( SendData(cmd,(int)strlen(cmd)) );

}

//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::DrawStringOutline( int x,int y,char* str, int len,short type,short yoko,short tate,short propaty);
//	�^�C�v�Fprivate
//	�@�\�@�FAscii�����̈��(�����͂���)
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�F������
//	�����S�F������
//	�����T�F������ނ̎w�� 'A'=�w���x�`�J�{�[���h(�v���|�[�V���i��)
//                         'B'=�w���x�`�J�{�[���h(�����ԃs�b�`�Œ�)
//	�����U�F������(50-999)
//	�����V�F��������(50-999)
//	�����W�F�t�H���g�`��(0-9)
//          0=�W��(���j
//          1=������
//          2=�D�F����(�p�^�[��1�j
//          3=�D�F����(�p�^�[��2�j
//          4=�D�F����(�p�^�[��3�j
//          5=�e�t������
//          6=�������e�t������
//          7=�����]����
//          8=�W���Α̕���
//          9=�������e�t���Α̕���
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::DrawStringOutline( int x,int y,char* str, int len,short type,short yoko,short tate,short propaty)
{
	if( m_Com == NULL ) return(-1);

	char cmd[50];

	SetPosition(x,y);

	// �c���{��
	if( yoko < 50 || yoko >999) yoko=50;
	if( tate < 50 || tate >999) tate=50;
	if( type!='A' && type!='B' ) type='A'; 
	if( propaty<0 || propaty>9 ) propaty=0;

	// �����Z�b�g
	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%c$%c,%03d,%03d,%d",S_ESC,type,yoko,tate,propaty);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));
	cmd[strlen(cmd)] = S_ESC;
	cmd[strlen(cmd)] = '$';
	cmd[strlen(cmd)] = '=';
	SendData(cmd,(int)strlen(cmd) );
	return( SendData(str, len) );
}


//---------------------------------------------------------------------------------------------------------------------
//	int  TSatoPrinter::SendData(char *data)
//	�^�C�v�Fprivate
//	�@�\�@�FAscii�����̈��(�����͂���)
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�F������
//	�����S�F������
//	�����T�F�����T�C�Y(0-4)
//	�����U�F���{��(1-12)
//	�����V�F�c�{��(1-12)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::DrawString( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch)
{
	if( m_Com == NULL ) return(-1);

	char cmd[50];

	SetPosition(x,y);

	// �c���{��
	if( yoko < 1 || yoko >12 ) yoko=1;
	if( tate < 1 || tate >12 ) tate=1;

	if( pitch < 0 || pitch > 99 ) pitch=2;

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP%02d",S_ESC,pitch);
//	wsprintf( &cmd[strlen(cmd)],"%cP%2d",S_ESC,pitch);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));
	cmd[strlen(cmd)] = S_ESC;
	cmd[strlen(cmd)] = 'L';
	wsprintf( &cmd[strlen(cmd)],"%02d%02d",yoko,tate);

	SendData(cmd,(int)strlen(cmd) );


	// �����T�C�Y
	if( size < 0 || size > 4 ) size=0;
	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	// �X���[�W���O�@�\�Ȃ�
	if( size < 3 )
		wsprintf( &cmd[strlen(cmd)],"X2%d,",size);
	else
		wsprintf( &cmd[strlen(cmd)],"X2%d,1",size);

	SendData(cmd,(int)strlen(cmd) );

	return( SendData(str, len) );
}


//---------------------------------------------------------------------------------------------------------------------
//	int	 DrawKanji( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch)
//	�^�C�v�Fprivate
//	�@�\�@�F�S�������������������i�v�����^�̐ݒ��S-JIS�ɂ��Ă������Ɓj
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�F������
//	�����S�F������
//	�����T�F�����T�C�Y(0(16*16)-1(24*24))
//	�����U�F���{��(1-12)
//	�����V�F�c�{��(1-12)
//	�����V�F�����ԃs�b�`(0-99)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::DrawKanji( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch)
{
	if( m_Com == NULL ) return(-1);

	char cmd[50];

	SetPosition(x,y);

	// �c���{��
	if( yoko < 1 || yoko >12 ) yoko=1;
	if( tate < 1 || tate >12 ) tate=1;

	if( pitch < 0 || pitch > 99 ) pitch=2;

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP%02d",S_ESC,pitch);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));
	cmd[strlen(cmd)] = S_ESC;
	cmd[strlen(cmd)] = 'L';
	wsprintf( &cmd[strlen(cmd)],"%02d%02d",yoko,tate);

	SendData(cmd,(int)strlen(cmd) );


	// �����T�C�Y
	if( !size  ) size=5;		// 16*16
	else size=6;				// 24*24
	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	wsprintf( &cmd[strlen(cmd)],"K%dC",size);

	SendData(cmd,(int)strlen(cmd) );

	// �����̏�ʃr�b�g�Ɖ��ʃr�b�g�̓���ւ�
	unsigned short a,b,i;
	for( i=0; i< len; i++){
		// ���̕����͊������H
		if( IsDBCSLeadByteEx(CP_ACP, str[i])==TRUE){
			b=(unsigned short)str[i];
			b= b << 8;
			b= b >> 8;
			a=(unsigned short)str[++i];
			a= a << 8;
			b= a|b;
			SendData((char*)&b,sizeof(short) );
		}
		// ���p�̏ꍇ�͂��̂܂ܑ���
		else{
			SendData(&str[i],1 );
		}
	}

	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::Bar39( int x,int y,int h,char* bcr,BOOL guide,int hiritu,int kakudai);
//	�^�C�v�Fpublic
//	�@�\�@�Fcode39�ް���ނ̈�
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�F�ް���ލ���(0.1mm)
//	�����S�F�ް���ޕ�����
//	�����T�F������
//	�����U�F�K�C�h�����L��
//	�����V�F�����T�C�Y(0-4)
//	�����W�F�䗦 0=1,2(��̫��) 1=1:3, 2=2:5
//	�����X�F��1�{��(1-12)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::Bar39( int x,int y,short h,char* bcr, int len, BOOL guide,int hiritu,short kakudai)
{
	if( m_Com == NULL ) return(-1);

	int pos;
	char cmd[30];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP00",S_ESC);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));


	// �䗦
	switch(hiritu){
		case 1:
			wsprintf( cmd,"%cB1",S_ESC);
			break;
		case 2:
			wsprintf( cmd,"%cBD1",S_ESC);
			break;
		default:
			wsprintf( cmd,"%cD1",S_ESC);
	}
	// ���g�嗦
	if( kakudai < 1 || kakudai >12 ) kakudai =1;
	// �o�[����
	pos = (int)((double)h*m_dpm);
	if( pos<1 || pos > 600 ) pos=20;
	wsprintf( &cmd[strlen(cmd)], "%02d%03d", kakudai,pos);

	SendData(cmd,(int)strlen(cmd) );
	return( SendData(bcr, len) );

}
//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::Bar128( int x,int y,int h,char* bcr,BOOL guide,int hiritu,int kakudai);
//	�^�C�v�Fpublic
//	�@�\�@�Fcode128�ް���ނ̈�
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�F�ް���ލ���(0.1mm)
//	�����S�F�ް���ޕ�����
//	�����T�F������
//	�����U�F�K�C�h�����L��
//	�����V�F�����T�C�Y(0-4)
//	�����X�F��1�{��(1-12)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::Bar128( int x,int y,short h,char* bcr, int len, BOOL guide,short kakudai)
{
	if( m_Com == NULL ) return(-1);

	int pos;
	char cmd[30];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP00",S_ESC);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));


	wsprintf( cmd,"%cBG",S_ESC);

	// ���g�嗦
	if( kakudai < 1 || kakudai >12 ) kakudai =1;
	// �o�[����
	pos = (int)((double)h*m_dpm);
	if( pos<1 || pos > 600 ) pos=20;
	wsprintf( &cmd[strlen(cmd)], "%02d%03d", kakudai,pos);

	SendData(cmd,(int)strlen(cmd) );
	return( SendData(bcr, len) );

}

//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::BarQR2( int x,int y,char err,short size,short mode,short joint,char* bcr, int len,short c1,short c2,char* c3)
//	�^�C�v�Fpublic
//	�@�\�@�FQR2�����ް���ނ̈� (Model2)
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�F�G���[�R���N�V����(�������j���x��('L'=7%�A'M'=15%�A'Q'=25%�A'H'=30%)
//  �����S�F�Z����Ђ̃T�C�Y�w��(1-32 Dot)
//  �����T�F���̓��[�h(0=�����ݒ�A1=�����A2=�p�����A3=����:SJIS)
//	�����U�F�A�� (0=�ʏ탂�[�h�A1=�A�����[�h)
//	�����V�F�ް���ޕ�����
//	�����W�F������
//  �ȉ��A�����[�h�̎��w��--------------------
//	�����X�F�A�����[�h������ (1-16)
//	����10�F�A�����[�h���������ʔ�(1-16)
//	����11�F�A�����[�h�p���e�B�f�[�^(00-FF)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::BarQR2( int x,int y,char err,short size,short mode,short joint,char* bcr, int len,short c1,short c2,short c3)
{
	if( m_Com == NULL ) return(-1);

	char cmd[64];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP00",S_ESC);
	SendData(cmd,(int)strlen(cmd) );

	// �R�}���h�i�P�^�Q�j
	memset( cmd, 0, sizeof(cmd));
	wsprintf( cmd,"%c2D30",S_ESC);
	//Mode
	if( err=='L' || err=='l' ) wsprintf( &cmd[strlen(cmd)], ",L");
	else if( err=='M' || err=='m' ) wsprintf( &cmd[strlen(cmd)], ",M");
	else if( err=='H' || err=='h' ) wsprintf( &cmd[strlen(cmd)], ",H");
	else if( err=='Q' || err=='q' ) wsprintf( &cmd[strlen(cmd)], ",Q");
	else wsprintf( &cmd[strlen(cmd)], ",L");
	//Size
	if( size<1 || size > 32 ) size=10;
	wsprintf( &cmd[strlen(cmd)], ",%02d", size);
	//Data Mode
	if( mode<0 || mode > 3 ) mode=0;
	if( mode==0 )
		wsprintf( &cmd[strlen(cmd)], ",1");	// Auto
	else
		wsprintf( &cmd[strlen(cmd)], ",0");	// Manual
	//Joint
	if( joint<0 || joint > 1 ) joint=0;
	wsprintf( &cmd[strlen(cmd)], ",%1d", joint);
	if( joint==1 ){
		if( c1<1 || c1 > 16 ) c1=1;
		if( c2<1 || c2 > 16 ) c2=1;
		if( c3<0 || c3 > 255 ) c3=0;
		wsprintf( &cmd[strlen(cmd)], ",%02d,%02d,%02X", c1,c2,c3);
	}

	SendData(cmd,(int)strlen(cmd) );



	// �R�}���h�i�Q�^�Q�j�}�j���A�����[�h
	if( mode!=0 ){
		memset( cmd, 0, sizeof(cmd));
		wsprintf( cmd,"%cDS",S_ESC);
		wsprintf( &cmd[strlen(cmd)], "%1d,", mode);
		SendData(cmd,(int)strlen(cmd) );
		return( SendData(bcr, len) );
	}else
	// �R�}���h�i�Q�^�Q�j�I�[�g���[�h
	{
		memset( cmd, 0, sizeof(cmd));
		wsprintf( cmd,"%cDN",S_ESC);
		wsprintf( &cmd[strlen(cmd)], "%04d,", len);
		SendData(cmd,(int)strlen(cmd) );
		return( SendData(bcr, len) );
	}
}
//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::BarPDF417( int x,int y,int h,char* bcr,BOOL guide,int hiritu,int kakudai);
//	�^�C�v�Fpublic
//	�@�\�@�FPDF417 2�����ް���ނ̈�
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�F�ŏ����W���[����(1-9dot)
//	�����S�F�ŏ����W���[���s�b�`���@(1-24dot)
//	�����T�F�Z�L�����e�B�i�������j���x��(0-8)
//	�����U�F�P�s������̃f�[�^�R�[�h���[�h(1-30)
//	�����V�F�V���{��������̍s��(3-90)
//	�����W�F�R�[�h��(0=�ʏ�,1=�؂�l��)
//	�����X�F�ް���ޕ�����
//	����10�F������
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::BarPDF417( int x,int y,short module,short pitch,short ayamari,short data_w,short symbol_row,short type,char* bcr, int len)
{
	if( m_Com == NULL ) return(-1);

//	int pos;
	char cmd[30];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP00",S_ESC);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));

	if( module < 1 || module >9 ) module =1;
	if( pitch < 1 || pitch >24 ) pitch =1;
	if( ayamari < 0 || ayamari >8 ) ayamari =0;
	if( data_w < 1 || data_w >30 ) data_w =1;
	if( symbol_row < 3 || symbol_row >90 ) symbol_row =3;
	if( type < 0 || type >1 ) type =0;



	wsprintf( cmd,"%c2D10,%02d,%02d,%d,%02d,%02d",S_ESC,module,pitch,ayamari,data_w,symbol_row);
	if( type )
		wsprintf( &cmd[strlen(cmd)], ",%1d", type);
	SendData(cmd,(int)strlen(cmd) );

	// �f�[�^��
	wsprintf( cmd,"%cDN%04d,",S_ESC,len);
	SendData(cmd,(int)strlen(cmd) );
	return( SendData(bcr, len) );

}
//---------------------------------------------------------------------------------------------------------------------
//	int	 TSatoPrinter::BarMAXI( int x,int y,short mode,char* bcr,int len,short service,short country,short post)
//	�^�C�v�Fpublic
//	�@�\�@�FMAXI  2�����ް���ނ̈�
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�F���[�h 2=�z����p�@�@(�󎚃f�[�^�ő� �����̂�=123/�p������=84)
//                 3=�z����p�@�@(�󎚃f�[�^�ő� �����̂�=123/�p������=84)
//                 4=�W���L���@�@(�󎚃f�[�^�ő� �����̂�=138/�p������=93)
//                 6=�ǂݎ��@��p�@�@(�󎚃f�[�^�ő� �����̂�=138/�p������=93)
//	�����S�F�ް���ޕ�����
//	�����T�F������
//	�����U�F�T�[�r�X�N���X (1-999) ���[�h4,6�͏ȗ�
//	�����V�F���R�[�h(1-999) ���[�h4,6�͏ȗ�
//	�����X�F�X�փR�[�h(0-999999) ���[�h4,6�͏ȗ�
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::BarMAXI( int x,int y,short mode,char* bcr,int len,short service,short country,char* post)
{
	if( m_Com == NULL ) return(-1);

	char cmd[30];
	char tmp[30];
	long postV;
	int maxDatLen;

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP00",S_ESC);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));

	if( mode!=2 && mode!=3 && mode!=4 && mode!=6 ) mode=6;
	if( service < 1 || service > 999 ) service=1;
	if( country < 1 || country > 999 ) country=1;

	wsprintf( cmd,"%c2D20,",S_ESC);

	// ���[�h

	if( mode==2 ){
		// Mode 2
		postV=atoi(post);
		if( postV < 0 || postV > 999999 ) postV=0;
		wsprintf( &cmd[strlen(cmd)], "%01d,%03d,%03d,%06ld", mode,service,country,postV);
		maxDatLen=84;
	}else if( mode==3 ){
		// Mode 3
		memset(tmp,0,sizeof(tmp));
		memset(tmp,' ' , 6 );
		memcpy(tmp,post,strlen(post)<6?strlen(post):6);
		wsprintf( &cmd[strlen(cmd)], "%01d,%03d,%03d,%s", mode,service,country,tmp);
		maxDatLen=84;
	}else{
		// Mode 4,5
		wsprintf( &cmd[strlen(cmd)], "%01d", mode);
		maxDatLen=93;
	}
	SendData(cmd,(int)strlen(cmd) );

	// �f�[�^��
	wsprintf( cmd,"%cDN%",S_ESC);
	if( len > maxDatLen )len=maxDatLen;
	wsprintf( &cmd[strlen(cmd)], "%04d,", len);
	SendData(cmd,(int)strlen(cmd) );
	return( SendData(bcr, len) );

}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::DrawFrame(int x,int y,int xlen,int ylen,short linewidth )
//	�^�C�v�Fpublic
//	�@�\�@�F�g���������i��]�����Ă���ꍇ�́A���W�y�ђ����ɒ��Ӂj
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�Fx��������(0.1mm)
//	�����S�Fy��������(0.1mm)
//	�����T�F���̑���(0.1mm)
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::DrawFrame(int x,int y,int xlen,int ylen,short linewidth )
{
	if( m_Com == NULL ) return(-1);

	int pos,xl,yl;
	char cmd[50];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	pos = (int)((double) linewidth *m_dpm);
	xl = (int)((double) xlen *m_dpm);
	yl = (int)((double) ylen *m_dpm);
	if( pos < 2 || pos >99 ) pos=2;
	wsprintf( cmd,"%cFW%02d%02dV%04ldH%04ld",S_ESC,pos ,pos,yl,xl );
	return(SendData(cmd,(int)strlen(cmd) ) );
}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::DrawLine(int x,int y,int len,char direct,short linewidth )
//	�^�C�v�Fpublic
//	�@�\�@�F�r���������i��]�����Ă���ꍇ�́A���W�y�ђ����ɒ��Ӂj
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�F����(0.1mm)
//	�����S�F���� 'H' or 'V'
//	�����T�F���̑���(0.1mm)
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::DrawLine(int x,int y,int len,char direct,short linewidth )
{
	if( m_Com == NULL ) return(-1);

	int pos,l;
	char cmd[50];

	SetPosition(x,y);

	memset( cmd, 0, sizeof(cmd));
	memset( cmd, 0, sizeof(cmd));

	pos = (int)((double) linewidth *m_dpm);
	l = (int)((double) len *m_dpm);
	if( pos < 2 || pos >99 ) pos=2;
	wsprintf( cmd,"%cFW%02d%c%04ld",S_ESC,pos ,direct,l );
	return(SendData(cmd,(int)strlen(cmd) ) );
}

//---------------------------------------------------------------------------------------------------------------------
//	void TSatoPrinter::SetPosition( int x, int y)
//	�^�C�v�Fprivate
//	�@�\�@�F�J�����g���W�̈ړ�
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//---------------------------------------------------------------------------------------------------------------------
void TSatoPrinter::SetPosition( int x, int y)
{

	int pos;
	char cmd[50];

	// �󎚈ʒu���M�i�����j
	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	cmd[1] = 'V';
	pos = (int)( (double)y * m_dpm);
	wsprintf( &cmd[2],"%ld",pos);

	// �󎚈ʒu���M�i�����j
	cmd[strlen(cmd)] = S_ESC;
	cmd[strlen(cmd)] = 'H';
	pos = (int)( (double)x * m_dpm);
	wsprintf( &cmd[strlen(cmd)],"%ld",pos);

	SendData(cmd,(int)strlen(cmd) );

}

//---------------------------------------------------------------------------------------------------------------------
//	int  TSatoPrinter::SendData(char *data)
//	�^�C�v�Fprivate
//	�@�\�@�F�ް����v�����^�ɑ��M
//	�����@�F���M������
//---------------------------------------------------------------------------------------------------------------------
int  TSatoPrinter::SendData(char *data,int len)
{
	int stat=0;

	if( m_Com == NULL ) return(-1);

	stat = m_Com->Write((BYTE*)data,(DWORD)len);

	// �������݌��� !=�������ݐ�
	if(stat !=len ){
		return 1;
	}

	return 0;
}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::SetPageSize( int x, int y );
//	�^�C�v�Fpublic
//	�@�\�@�F�p���T�C�Y�w��
//	�����P�Fx�T�C�Y(0.1mm)
//	�����Q�Fy�T�C�Y(0.1mm)
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::SetPageSize( int x, int y )
{
	char cmd[256];

	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	wsprintf( &cmd[1],"A1%04d%04d",(long)(m_dpm*y),(long)(m_dpm*x));
	return(SendData(cmd,(int)strlen(cmd) ) );

}

//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::SetLabelSize(int sx,int sy,int lx,int ly,int slx,int sly,int nx,int ny)
//	�^�C�v�Fpublic
//	�@�\�@�F���x���T�C�Y�w��
//  �ڍׂ�sato�}�j���A�� ESC+RI ���Q��
//  �w��͑S�� �O�D�P����
//	�����P�F�u���V�[�g�T�C�Y�i�p���̈敝�j�v �� �L���͈� �F �P�T�O�O�`�R�U�O�O�i�h�b�g�j
//	�����Q�F�u�c�V�[�g�T�C�Y�i�p���̈捂���j�v �� �L���͈� �F �S�W�O�`�R�U�O�O�i�h�b�g�j
//	�����R�F�u�����x���ԃT�C�Y�v �� �L���͈� �F �O�`�U�O�i�h�b�g�j
//	�����S�F�u�c���x���ԃT�C�Y�v �� �L���͈� �F �O�O�`�U�O�i�h�b�g�j
//	�����T�F�u�����x�����p���T�C�Y�i�p���T�C�Y���j�v �� �L���͈� �F �S�W�O�`�R�Q�O�O�i�h�b�g�j
//	�����U�F�u�����x���c�p���T�C�Y�i�p���T�C�Y�����j�v �� �L���͈� �F �S�W�O�`�R�U�O�O�i�h�b�g�j
//	�����V�F�u�����x�������v �� �L���͈� �F �O�P�`�O�U�i���j
//	�����W�F�u�c���x�������v �� �L���͈� �F �O�P�`�O�V�i���j
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::SetLabelSize(int sx,int sy,int lx,int ly,int slx,int sly,int nx,int ny)
{
 	char cmd[256];

	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
 	wsprintf( &cmd[1],"RI%04d,%04d,%02d,%02d,%04d,%04d,%02d,%02d"
		,(long)(m_dpm*sx)
		,(long)(m_dpm*sy)
		,(long)(m_dpm*lx)
		,(long)(m_dpm*ly)
		,(long)(m_dpm*slx)
		,(long)(m_dpm*sly)
		,nx
		,ny
		);
	return(SendData(cmd,(int)strlen(cmd) ) );

}
//---------------------------------------------------------------------------------------------------------------------
//	int TSatoPrinter::SetOffset( int x, int y );
//	�^�C�v�Fpublic
//	�@�\�@�F��_�␳�w��
//	�����P�Fx�T�C�Y(0.1mm)
//	�����Q�Fy�T�C�Y(0.1mm)
//---------------------------------------------------------------------------------------------------------------------
int TSatoPrinter::SetOffset( int x, int y )
{
	char cmd[256];

	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	if( y >=0 )
		wsprintf( &cmd[1],"A3V+%d%",(long)(m_dpm*y));
	else
		wsprintf( &cmd[1],"A3V%d%",(long)(m_dpm*y));

	if( x >=0 )
		wsprintf( &cmd[strlen(cmd)],"H+%d%",(long)(m_dpm*x));
	else
		wsprintf( &cmd[strlen(cmd)],"H%d%",(long)(m_dpm*x));

	return(SendData(cmd,(int)strlen(cmd) ) );

}

//int TSatoPrinter::SetPrinter( SATO_LESPRIT_SETTING *setting );
int TSatoPrinter::SetPrinter( SATO_LESPRIT_SETTING *setting )
{
	char cmd[1024];

	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_STX;
	cmd[1] = S_ESC;
	cmd[2] = 'A';
	cmd[3] = 0;
	cmd[4] = S_ESC;
	cmd[5] = 'P';
	cmd[6] = 'G';
	wsprintf( cmd+7,"0000%02X00%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%04X%04X%04X%04X%02X%02X%02X%02X%02X%02X%02X"
		,setting->PrintSpeed
		,setting->Cutter
		,setting->Hakuri
		,setting->NonSepa
		,setting->ConcentrationSetup
		,setting->ConcentrationLevel
		,setting->Sensor
		,setting->Zero
		,setting->Kanji
		,setting->Paper
		,setting->Feed
		,setting->Pitch
		,setting->VDot
		,setting->HDot
		,setting->VOffset
		,setting->HOffset
		,setting->PitchOffset
		,setting->TearOffset
		,setting->CutterOffset
		,setting->HakuriOffset
		,setting->Comatibility
		,setting->Gap
		,setting->Buzzer
		);
	// �ް����M�J�n STX+ESCA
	cmd[strlen(cmd)]=S_ESC;
	cmd[strlen(cmd)]='Z';
	cmd[strlen(cmd)]=S_ETX;

	return(SendData(cmd,(int)strlen(cmd) ) );

}
//---------------------------------------------------------------------------------------------------------------------
//	int	 DrawKanjiPT( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch)
//	�^�C�v�Fprivate
//	�@�\�@�F�v�`���p��(PT-200e)�p�E�S�������������������i�v�����^�̐ݒ��S-JIS�ɂ��Ă������Ɓj
//	�����P�Fx���W(0.1mm)
//	�����Q�Fy���W(0.1mm)
//	�����R�F������
//	�����S�F������
//	�����T�F�����T�C�Y(1(16*16),2(24*24),3(22*22))
//	�����U�F���{��(1-12)
//	�����V�F�c�{��(1-12)
//	�����V�F�����ԃs�b�`(0-99)
//---------------------------------------------------------------------------------------------------------------------
int	 TSatoPrinter::DrawKanjiPT( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch)
{
	if( m_Com == NULL ) return(-1);

	char cmd[50];

	SetPosition(x,y);

	// �c���{��
	if( yoko < 1 || yoko >12 ) yoko=1;
	if( tate < 1 || tate >12 ) tate=1;

	if( pitch < 0 || pitch > 99 ) pitch=2;

	memset( cmd, 0, sizeof(cmd));
	wsprintf( &cmd[strlen(cmd)],"%cP%02d",S_ESC,pitch);
	SendData(cmd,(int)strlen(cmd) );

	memset( cmd, 0, sizeof(cmd));
	cmd[strlen(cmd)] = S_ESC;
	cmd[strlen(cmd)] = 'L';
	wsprintf( &cmd[strlen(cmd)],"%02d%02d",yoko,tate);

	SendData(cmd,(int)strlen(cmd) );


	// �����T�C�Y
	if( size !=1 && size !=2 && size !=3 ) size=1;		// 16*16
	memset( cmd, 0, sizeof(cmd));
	cmd[0] = S_ESC;
	wsprintf( &cmd[strlen(cmd)],"K%dH",size);
	SendData(cmd,(int)strlen(cmd) );

	// �����R�[�h�𕶎��ϊ����đ���
	int i;
	for( i=0; i< len; i++){
		// ���̕����͊������H
		if( IsDBCSLeadByteEx(CP_ACP, str[i])==TRUE){
			sprintf( cmd,"%02X%02X",(unsigned char)str[i],(unsigned char)str[i+1]);
			SendData(cmd,(int)strlen(cmd) );
			i++;
		}
		// ���p�̏ꍇ�͂��̂܂ܑ���
		else{
			sprintf( cmd,"%02X",(unsigned short)str[i]);
			SendData(cmd,2 );
		}

	}

	return(0);
}

