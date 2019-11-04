#ifndef EASY_ENC_H
#define EASY_ENC_H
//=======================================================================================================
//	�ȈՔňÍ��������N���X
//	Class Name EasyEnc.h
//	File Name CEasyEnc.h
//	Date : 2006.10.12 N.Maeda
//=======================================================================================================

#include "windows.h"

#define ENC_REG_KEY "Software\\NokEasyEnc"
#define ENC_REG_DATA "EASY_ENC_KEY1"	// �L�[
#define ENC_REG_DATE "EASY_ENC_DATE"	// �L�[��o�^������

// 2006.10.12
// �ȈՈÍ������W�X�g���ƍ��R�[�h
// MAC�A�h���X���w�n�q�Ō����Ȃ�����
#define ENC_REG_CODE "ISHIDA"

#define L_MAC_STRING_MAX 10

class CEasyEnc
{
	public:
		CEasyEnc(void);
		~CEasyEnc(void);

		// �v���O�����̍ŏ��Ƀ��W�X�g���̒l�����������H�`�F�b�N����
		// ���W�X�g���̓o�^���������Ȃ��ꍇ�́A�l���폜����B
		// �i�ȍ~�̃t�@�C���I�[�v���̃`�F�b�N�ň���������j
		BOOL	CheckRegEncKey( char*EncCode );

		// �t�@�C������N���X�Ń��W�X�g���ɒl���o�^����Ă��邩�H�`�F�b�N����
		// TText2����CALL
		BOOL	CheckEnc(void);

		// ���W�X�g�L�[�o�^
		BOOL	SetRegEncKey( char*EncCode );

		// ���W�X�g�L�[�폜
		BOOL	DelRegEncKey(void);


		//--------------------------------------------------------
		//GetReg
		BOOL	GetReg( char*szKeyName,char*szDataName,char*buf,int buf_len);

		//PutReg
		BOOL	PutReg( char*szKeyName,char*szDataName,char*buf);

		//DelReg
		BOOL	DelReg( char*szKeyName,char*szDataName);

	protected:

		//GetMac
		BOOL	GetMac( unsigned char*buf );
		int	GetMacAddress();	// 

	private:

		long htol( char *hexstr , short len );


		char MacString[L_MAC_STRING_MAX][6+1];

};



#endif
