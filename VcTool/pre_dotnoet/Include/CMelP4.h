#ifndef COM_PLC4_H
#define COM_PLC4_H
//================================================================================================
//
//	FileName	:CMelP4.h
//	Class		:CMelsecP4
// 				:Melsec �v���g�R���S�`���ʐM
//	sum�`�F�b�N�L��Axon/off����Ȃ��B
//	�쐬��		:2001.12.13	Coded by coba
//	TRS232C�̃T�u�N���X�ɂȂ��Ă��܂��̂ŁA������������Ă�������
//================================================================================================
//	�C��
//	2003.09.04 coba �g���t�@�C�����W�X�^�ւ̏������݊֐���ǉ�
//					QnA�̊ȈՃt���[���ʐM��ǉ��i�O�c�����FIX�j
//	2004.07.13 coba  wait�̎w����@�̃~�X���C��
//	2004.07.15 coba Close()��ǉ�
//================================================================================================

#include "TRS232C.h"


// �A�h���[�X�{�f�[�^���ȊO�̑��M�w�b�_�� ENQ(1)+ST(2)+PC(2)+CMD(2)+WAIT(1)+SUM(2)+�f�o�C�X��(2)+CRLF(2)
const int P4_HEADLEN=14;

class CMelsecP4:public TRS232C
{
	public:
		CMelsecP4( void );
		~CMelsecP4( void );

		BOOL Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);
		void Close(void);

		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������
		int P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// �w�萔�̊g�����[�h�f�o�C�X�ǂݏo��&��������
		int P4WriteWEx( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadWEx( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// �w�萔�̊g���t�@�C�����W�X�^�ǂݏo��&��������
		int P4WriteREx( char* station, char* pcstation,char* addr, short counts, char* msg);

		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������(2C���[�h)
		int P2C4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P2C4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// 1�_�̃r�b�g�f�o�C�X�ǂݏo��&��������
		int P4WriteB( char* station, char* pcstation,char* addr, int onoff);
		int P4ReadB( char* station,char* pcstation,char* addr);

		// ������ϊ�
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);

/*
		// 1�r�b�g�f�o�C�X�̓ǂݏo��&��������
		int P4WriteB( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadB( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);
*/

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
		int Receive( char* buf, int bufsize, int waitlen );
		int Answer( char flg, char* station,char* pc );

		// ����M�o�b�t�@
		char m_buf[4096];

		unsigned char htoi( char *hexstr , short len );

};
#endif
