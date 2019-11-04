#ifndef KEYENCE_LV21A_H
#define KEYENCE_LV21A_H
//================================================================================================
//	FileName	:CKeyL2AE.h
//	Class		:CKeyL2AE
// 				:
//	�쐬��		:2012.09.05 kobashi
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
//	2014.12.05 coba �r�b�g��ON��OFF��ǉ�
//================================================================================================
#include <winsock2.h>
#include <windows.h>



// ��ʃ����N�̃f�t�H���g�|�[�g�ԍ�(�o�C�i���ʐM�ł����g���Ȃ�)
//#define UDP_MELPORT 5000

// �f�o�C�X�̎��
#define DEV_WORD 0
#define DEV_BIT  1

// ���M�f�[�^���̌Œ蕔h
// CPU�Ď��^�C�}(4)+�R�}���h(4)+�T�u�R�}���h(4)+�f�o�C�X�R�[�h(2)
// +Dev�R�[�h(2)+�擪�f�o�C�X(6)+�_��(2)
//#define P3E_FIX_DATALEN 24

class CKeyL2AE
{
	public:
		CKeyL2AE( void );
		~CKeyL2AE( void );

		BOOL Init(void);


		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������(2C���[�h)
		int L2AWrite(char* ipaddr,unsigned short port,int devtype,char* devaddr, short counts, char* data);
		int L2ARead( char* ipaddr,unsigned short port,char* buf,int bufsize,int devtype,char* devaddr, short counts);

		int L2ASetBit(char* ipaddr,unsigned short port,char* devaddr,BOOL on );

		//�ڑ��e�X�g�p
		int GetPlcType(char* ipaddr,unsigned short port,char* recvBuf);

		// ������ϊ�
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);

		// ��ʉ��ʂ̃��[�h����ւ�(BCD�W���̕ϊ�)
		BOOL ULChg( char* buf );

		unsigned short htoi( char *hexstr , short len );
	protected:

	private:
		BOOL m_fOpen;					// ����ݒ�=TRUE,close��=FALSE

		//���M����
		int UdpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize);


};
#endif
