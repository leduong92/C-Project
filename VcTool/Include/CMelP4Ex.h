#ifndef COM_PLC4EX_H
#define COM_PLC4EX_H
//================================================================================================
//
//	FileName	:CMelP4Ex.h
//	Class		:CMelsecP4Ex
// 				:Melsec �v���g�R���S�`���ʐM
//	sum�`�F�b�N�L��Axon/off����Ȃ��B
//	�쐬��		:2002. 7.1 N.Maeda
//	TRS232C�̃T�u�N���X�ɂȂ��Ă��܂��̂ŁA������������Ă�������
//================================================================================================
//	2004. 1. 7 �{�[���[�g�̐ݒ��ǉ� 300bps-115200bps
//	2008.01.09 coba Init()�ɁA������g�p�L����ǉ�(�L�[�G���X�݊����[�h�p)
//	2008.09.01 ���� UStr2Mel��ǉ�(�O�cSJH�̃\�[�X����R�s�[��������)
//	2009.01.26 coba ComClose()��ǉ�
//	2011.03.11 coba Str2Mes()�̃o�O(SJIS������Ȃ�)���C��
//	2011.06.18 coba �r�b�g�f�o�C�X�̃u���b�N�ǂݏo��P4ReadBB()��ǉ�
//	2012.06.20 coba �r�b�g�f�o�C�X���_�ꊇ��������P4WriteBB()��ǉ�
//	2013.02.17 coba 1C�v���g�R���̊g���R�}���hQR,QW�ɑΉ�(AnS�ŁAD1024�ȍ~���A�N�Z�X����ꍇ�Ɏg�p)
//	2013.04.11 coba ���g���C��over�ł�����I����Ԃ��o�O�C��
//	2013.11.22 matsuda P4ReadW�Ŗ������[�v�ɓ���\���̂��鏈���ɑ΂��āA�^�C���A�E�g��ݒ肷��悤�ύX
//	2014.01.07 ryuu ���̏C������20byte�ȏ��M�ł��Ȃ��̂ŊO��
//================================================================================================

#include "TRS232C.h"


// �A�h���[�X�{�f�[�^���ȊO�̑��M�w�b�_�� ENQ(1)+ST(2)+PC(2)+CMD(2)+WAIT(1)+SUM(2)+�f�o�C�X��(2)+CRLF(2)
const int P4_HEADLEN=14;

class CMelsecP4Ex:public TRS232C
{
	public:
		CMelsecP4Ex( void );
		~CMelsecP4Ex( void );

		BOOL Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait,BOOL ctrl_line=TRUE);
		void ComClose(void);	// 2009.01.26

		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������
		int P4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		// �t�@�C�����W�X�^�i�`�݊��A�o���N�w�菑������"EW")
// �G���[��������������Ă��邩�s���A���̂��߃R�����g�A�E�g����B�i������Ⴂ�������I�I�j
//		int P4WriteWExEW( char* station, char* pcstation,char* addr, short counts, char* msg);

		// �w�萔�̃��[�h�f�o�C�X�ǂݏo��&��������(2C���[�h)
		int P2C4WriteW( char* station, char* pcstation,char* addr, short counts, char* msg);
		int P2C4ReadW( char* buf,int bufsize,char* station,char* pcstation,char* addr, short counts);

		int P4Send( char* station, char* pcstation,char* command,char* addr, short counts, char* msg);

		// 1�_�̃r�b�g�f�o�C�X�ǂݏo��&��������
		int P4WriteB( char* station, char* pcstation,char* addr, int onoff);
		int P4ReadB( char* station,char* pcstation,char* addr);
		// ���_�̃r�b�g�f�o�C�X�ǂݏo��&��������
		int P4ReadBB( char* buf,int bufsize,char* station,char* pcstation,char* addr,short counts);
		int P4WriteBB( char* station, char* pcstation,char* addr, int cnt,char* dat); // 2012.06.20 coba

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
		int Receive( char* buf, int bufsize, int waitlen );
		int Answer( char flg, char* station,char* pc );

		// ����M�o�b�t�@
		char m_buf[4096];

		unsigned char htoi( char *hexstr , short len );

};
#endif
