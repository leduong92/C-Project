#ifndef MELSEC_P1E_H
#define MELSEC_P1E_H
//================================================================================================
//	FileName	:CMelP3E.h
//	Class		:CMelsecP3E
// 				:Melsec �v���g�R��1E�`���ʐM(QnA�݊�1E�t���[��)
//	�쐬��		:2008.01.10 coba
//
//	Ether���j�b�g�̐ݒ�ŁA����ݐݒ�̂Ƃ���ŁA���̃v���O�����𑖂点��@���IP��ݒ肷��
//		�v���g�R��		UDP
//		����ݕ���		(�ݒ�s��)
//		�Œ��ޯ̧		���M
//		�߱�ݸ޵����	�y�A�ɂ��Ȃ�
//		�����m�F		�m�F���Ȃ�
//		�����߰Ĕԍ�	PLC���̑҂����߰�
//		�X�V����IP		���̃v���O�����𑖂点��@���IP
//		�X�V�����߰�	FFFF
//================================================================================================
//	�C��
//	2011.03.11 coba Str2Mel()�̃o�O(SJIS������Ȃ�)���C��
//	2012.12.21 coba ENET-ADAP�ł�UDP�����܂������Ȃ��̂�TCP��ǉ�
//================================================================================================
#include <winsock2.h>
#include <windows.h>

// Melsec�̃f�t�H���g�|�[�g�ԍ�(�o�C�i���ʐM�ł����g���Ȃ�)
#define UDP_MELPORT 5000

// �f�o�C�X�̎��
#define DEV_WORD 0
#define DEV_BIT  1

// ���M�f�[�^���̌Œ蕔h
// CPU�Ď��^�C�}(4)+�R�}���h(4)+�T�u�R�}���h(4)+�f�o�C�X�R�[�h(2)
// +Dev�R�[�h(2)+�擪�f�o�C�X(6)+�_��(2)
#define P3E_FIX_DATALEN 24

class CMelsecP1E
{
	public:
		CMelsecP1E( void );
		~CMelsecP1E( void );

		BOOL Init(BOOL tcp=FALSE);


		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������(2C���[�h)
		int P1EWrite(char* ipaddr,unsigned short port,int devtype,char* devname,int devaddr, short counts, char* data);
		int P1ERead( char* ipaddr,unsigned short port,char* buf,int bufsize
							,int devtype,char* devname,int devaddr, short counts);

		// ������ϊ�
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);

		// ��ʉ��ʂ̃��[�h����ւ�(BCD�W���̕ϊ�)
		BOOL ULChg( char* buf );

	protected:

	private:
		BOOL m_fOpen;					// ����ݒ�=TRUE,close��=FALSE

		// 2012.12.21 coba TCP�ǉ�
		BOOL m_tcp;
		int TcpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize);

		int UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize);

		unsigned char htoi( char *hexstr , short len );

};
#endif
