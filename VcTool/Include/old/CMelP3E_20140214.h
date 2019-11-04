#ifndef MELSEC_P3E_H
#define MELSEC_P3E_H
//================================================================================================
//	FileName	:CMelP3E.h
//	Class		:CMelsecP3E
// 				:Melsec �v���g�R��3E�`���ʐM(QnA�݊�3E�t���[��)
//	�쐬��		:2007.02.17 coba
//
//	�l�b�g���[�N�p�����[�^��GX Developer�Őݒ肷��
//	�E����ݒ�
//		�X�V�ް�����	ASCII�X�V
//		�Ƽ������ݸ�	���OPEN�҂�
//		IP�A�h���X		PLC����IP�A�h���X��ݒ�
//		���M�t���[��	Ethernet(V2.0)
//		RUN���������݂�����
//	�E����ݐݒ�
//		�v���g�R��		UDP
//		����ݕ���		(�ݒ�s��)
//		�Œ��ޯ̧		���M
//		�X�V�菇		�菇����
//		�߱�ݸ޵����	�y�A�ɂ��Ȃ�
//		�����m�F		�m�F���Ȃ�
//		�����߰Ĕԍ�	PLC���̑҂����߰�
//		�X�V����IP		���̃v���O�����𑖂点��PC����IP
//		�X�V�����߰�	FFFF
//	�Eٰ�ݸޏ��
//		ٰ����p�@�\		�g�p����
//		���ȯ�Ͻ�		�l�b�g���[�N�ɂ��킹�Đݒ�
//		��ٰ̫����ڽ	�_�~�[�ł���������ݒ肷��
//================================================================================================
//	�C��
//	2011.03.11 coba Str2Mes()�̃o�O(SJIS������Ȃ�)���C��
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

class CMelsecP3E
{
	public:
		CMelsecP3E( void );
		~CMelsecP3E( void );

		BOOL Init(void);


		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������(2C���[�h)
		int P3EWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data);
		int P3ERead( char* ipaddr,unsigned short port,char* buf,int bufsize
							,int devtype,char* devaddr, short counts);

		// ������ϊ�
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);

		// ��ʉ��ʂ̃��[�h����ւ�(BCD�W���̕ϊ�)
		BOOL ULChg( char* buf );

		unsigned short htoi( char *hexstr , short len );
	protected:

	private:
		BOOL m_fOpen;					// ����ݒ�=TRUE,close��=FALSE


		int UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize);


};
#endif
