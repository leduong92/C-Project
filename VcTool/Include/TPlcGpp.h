#ifndef TPLCGPP_H
#define TPLCGPP_H
//================================================================================================
//
//	FileName	:TPlcGpp.H
//	Class		:TPlcGpp (SubClass of "Trs232C")
// 				:�O�H�V�[�P���T�̂b�o�t�|�[�g�𗘗p�����ʐM
//	�쐬��		:1998.2.10	Coded by M.Ishida
//
//================================================================================================

#include "TRS232C.h"

#define READ_LEN_MAX	1024					// ���ɓǂݍ��݉\�ȍő�o�C�g��

// ��M�o�b�t�@�̃f�t�H���g�l

// 
#define READ_DEVICES	0
#define WRITE_DEVICES	1

#define READ_WORD_DEVICE_MAXPOINT	128		// D/W ڼނ̈��ɓǂݍ��݉\�ȍő�_��
#define READ_WORD_DEVICE_MAXBYTE	512		// D/W ڼނ̈��ɓǂݍ��݉\�ȍő�o�C�g��
#define WORD_DEVICE_MAXPOINT		6144	// D/W �ް�ڼ޽����޲��_��



class TPlcGpp:public TRS232C
{
	private:
		HWND hwParent;		// �޲�۸������

		BOOL m_fOpen;		// ����ݒ�=TRUE,close��=FALSE

		unsigned char CpuType;
	public:
	
		TPlcGpp( void );
		~TPlcGpp( void );

		BOOL Init(int portNo,int baudrate,int databit,int stopbit,int parity,int wait);

		short	PortOpen(void);			// �߰Ă̵����
		short	PortOpenFx(HWND hwnd);		// �߰Ă̵����
		void	PortClose(void);			// �߰Ă̸۰��
		short	SRead( char memory, short address, short dvno, char *data );
												// ܰ�����޲�������ް��ǂݍ���
		short	SWrite( char memory, short address,  char *data );
												// ܰ�����޲��ւ��ް���������
		short	SWriteN( char memory, short address, short dvno, char *data );
												// ܰ�����޲��ւ��ް���������
		unsigned char htoi( char *hexstr , short len );
		short	SumCheck( unsigned char *p, short theLen );
		short	SumCheckC( unsigned char *p, short theLen );
		short	MakeCodeFunction(short function, short dvno, unsigned char *code, short theLen);
		short	MakeCodeDevice(char memory, short address, short dvno, unsigned char *code, short theLen);

		int		Hex2Dec( char* hexstr, short len );
	protected:


};
#endif
