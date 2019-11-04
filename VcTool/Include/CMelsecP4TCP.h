#ifndef COM_PLC4E_TCP_H
#define COM_PLC4E_TCP_H
//================================================================================================
//	FileName	:CMelsecP4TCP.h
//	Class		:CMelsecP4TCP
// 				:Melsec �v���g�R���S�`���ʐMonTCP/IP
//				 Serial-Ethernet�R���o�[�^�̃T�[�o���[�h�p�ɁA�ʐM���e��TCP�ɏ悹�đ���
//	sum�`�F�b�N�L��Axon/off����Ȃ��B
//	�쐬��		:2013.09.20 coba
//================================================================================================
//	�C��
//================================================================================================
#include <winsock2.h>
#include <windows.h>

// ����R�[�h��`
#ifndef CR
	#define CR 0x0d
#endif
#ifndef LF
	#define LF 0x0a
#endif
#ifndef SOH
	#define SOH 0x01
#endif
#ifndef STX
	#define STX 0x02
#endif
#ifndef ETX
	#define ETX 0x03
#endif
#ifndef EOT
	#define EOT 0x04
#endif
#ifndef ENQ
	#define ENQ 0x05
#endif
#ifndef ACK
	#define ACK 0x06
#endif
#ifndef NAK
	#define NAK 0x15
#endif


// �A�h���[�X�{�f�[�^���ȊO�̑��M�w�b�_�� ENQ(1)+ST(2)+PC(2)+CMD(2)+WAIT(1)+SUM(2)+�f�o�C�X��(2)+CRLF(2)
const int P4_HEADLEN=14;

class CMelsecP4TCP
{
	public:
		CMelsecP4TCP( void );
		~CMelsecP4TCP( void );

		BOOL Init(int wait);

		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������
		int P4WriteW( char* ip ,unsigned short port,char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadW( char* ip ,unsigned short port,char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// 1�_�̃r�b�g�f�o�C�X�ǂݏo��&��������
		int P4WriteB( char* ip ,unsigned short port,char* station, char* pcstation,char* addr, int onoff);
		int P4ReadB( char* ip ,unsigned short port,char* station,char* pcstation,char* addr);

		// ���_�̃r�b�g�f�o�C�X�ǂݏo��&��������
		int P4ReadBB( char* ip ,unsigned short port,char* buf,int bufsize,char* station,char* pcstation,char* addr,short counts);
		int P4WriteBB( char* ip ,unsigned short port,char* station, char* pcstation,char* addr, int cnt,char* dat); // 2012.06.20 coba

		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������(2C���[�h)
		int P2C4WriteW( char* ip ,unsigned short port,char* station, char* pcstation,char* addr, short counts, char* msg);
		int P2C4ReadW( char* ip ,unsigned short port,char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);


		// ������ϊ�
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);
		int UStr2Mel(char* buf, unsigned int bufsize, unsigned char* str);

		// ��ʉ��ʂ̃��[�h����ւ�(BCD�W���̕ϊ�)
		BOOL ULChg( char* buf );

	protected:

	private:
		BOOL m_fOpen;					// ����ݒ�=TRUE,close��=FALSE
		int m_Wait;

		// sum�`�F�b�N�̌v�Z
		int GetCheckSum(char* str );

		// �R�}���h�̍쐬
		void MakeCmd(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg);
		void MakeCmd2C(char* buf,int bufsize,char* station,char* pc,char* cmd,char* addr,short counts,char*msg);

		// �����Ŏg������M�֐�
		int TcpSend( char* ipaddr,unsigned short port,char* data,int size,char* rcv,int rsize);
		int Answer( char* ip ,unsigned short port,char flg, char* station,char* pc );

		// ����M�o�b�t�@
		char m_buf[4096];

		unsigned char htoi( char *hexstr , short len );

};
#endif
