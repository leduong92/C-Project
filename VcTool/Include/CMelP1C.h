#ifndef COM_PLC1C_H
#define COM_PLC1C_H
//================================================================================================
//
//	FileName	:CMelP1C.h
//	Class		:CMelsecP1C
// 				:Melsec �v���g�R�� �`��1�ʐM
//	sum�`�F�b�N�L��Axon/off����Ȃ��B
//	�쐬��		:2007. 7.25 N.Maeda
//================================================================================================
// 2011.03.11 coba Str2Mes()�̃o�O(SJIS������Ȃ�)���C��
//================================================================================================
#include "TRS232C.h"


// �A�h���[�X�{�f�[�^���ȊO�̑��M�w�b�_�� ENQ(1)+ST(2)+PC(2)+CMD(2)+WAIT(1)+SUM(2)+�f�o�C�X��(2)+CRLF(2)
const int P4_HEADLEN=14;

class CMelsecP1C:public TRS232C
{
	public:
		CMelsecP1C( void );
		~CMelsecP1C( void );

		BOOL Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);

		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������
		int P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// �t�@�C�����W�X�^�i�`�݊��A�o���N�w�菑������"EW")

		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������(2C���[�h)
		int P4Send( char* station, char* pcstation,char* command,char* addr, short counts, char* msg);

		// 1�_�̃r�b�g�f�o�C�X�ǂݏo��&��������
		int P4WriteB( char* station, char* pcstation,char* addr, int onoff);
		int P4ReadB( char* station,char* pcstation,char* addr);

		// ������ϊ�
		int Str2Mel(char* buf, unsigned int bufsize, char* str);
		int Mel2Str(char* buf, unsigned int bufsize, char* melstr);

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

		// �����Ŏg������M�֐�
		int Receive( char* buf, int bufsize, int waitlen );
		int Answer( char flg, char* station,char* pc );

		// ����M�o�b�t�@
		char m_buf[4096];

		unsigned char htoi( char *hexstr , short len );

};
#endif
