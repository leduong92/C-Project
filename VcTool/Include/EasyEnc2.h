#ifndef EASY_ENC_H
#define EASY_ENC_H
//=======================================================================================================
//	�ȈՔňÍ��������N���X
//	Class Name EasyEnc.h
//	File Name CEasyEnc.h
//	Date : 2006.10.12 N.Maeda
//=======================================================================================================

#include "windows.h"

// 1:DATE , 2:
#define KEY_INFO_MAC_ADR -1
#define KEY_INFO_DATE 1
#define KEY_INFO_MAC  2

// Memo--------------------
// Link lib =  shlwapi.lib NetApi32.lib 
//
// check enc key
// ret=l_EasyEnc->CheckEncKey();		//CheckRegEncKey(ENC_REG_CODE);
//
// Compile mode =  /MLd
// �R���p�C�����[�h���ԈႤ��
// ���̃��C�v�����̎g�p�Ƌ������Ă��܂��B / NODEFAULTLIB:libraly ���g�p���Ă��������B
//-------------------------


class CEasyEnc
{
	public:
		CEasyEnc(void);
		~CEasyEnc(void);

		// �t�@�C������N���X�Ń��W�X�g���ɒl���o�^����Ă��邩�H�`�F�b�N����
		// TText2����CALL
		BOOL	CheckEncKey(void);


		// �L�[�o�^
		BOOL	SetRegEncKey( char*InstallKey );

		// �L�[�폜
		BOOL	DelRegEncKey(void);

		// �L�[���
		// -1:Mac address
		// 1:ENTRY DATE
		// 2:ENTRY MAC address
		BOOL	GetRegEncKeyInfo(int info_no,char*buf,int buf_size);

		// �f�[�^�̕ϊ�
		BOOL EncData( unsigned char*data , int data_len );

		// �Í������Ă��邩�H�̃`�F�b�N
		BOOL CheckEncFile( unsigned char * chkData );

		// �Í��L�[�̍쐬
		BOOL	MakeInstallKey( int KeyMode , unsigned char* keyCode , unsigned char* mac );

	protected:
		// �v���O�����̍ŏ��Ƀ��W�X�g���̒l�����������H�`�F�b�N����
		// ���W�X�g���̓o�^���������Ȃ��ꍇ�́A�l���폜����
		// �i�ȍ~�̃t�@�C���I�[�v���̃`�F�b�N�ň���������j
		BOOL	CheckRegEncKey( int DeleteMode=FALSE );
		
		//--------------------------------------------------------
		//GetReg
		BOOL	GetReg( char*szKeyName,char*szDataName,char*buf,int buf_len,int keyMode=1);

		//PutReg
		BOOL	PutReg( char*szKeyName,char*szDataName,char*buf);

		//DelReg
		BOOL	DelReg( char*szKeyName,char*szDataName);


	private:
		//GetMac
		BOOL	GetMac( unsigned char*buf );
		int		GetMacAddress();	// 

		long htol( char *hexstr , short len );

		BOOL	m_Enc_Flg;	// TRUE=OK , FALSE=NG
		int		m_EndErrNo;	// 1=LAN
							// 2=No regist key
							// 3=Illegal key


};



#endif
