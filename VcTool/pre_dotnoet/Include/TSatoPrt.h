#ifndef SATO_LABEL_PRINTER
#define SATO_LABEL_PRINTER
//=====================================================================================================================
//
//  	���Ɓ[�o�[�R�[�h�v�����^�[�̐����{�N���X(�p�������C���^�[�t�F�[�X��)
//	
//		FileName : tsatoprt.cpp
//		�m�n�j������� �Z�p�{�� ���Y�Z�p��
//		2001.6.15 coba
//		�C�����F2004. 2. 4 PDF417/MAXI �ǉ� (by N.Maeda)
//              2004. 5.12 �p���T�C�Y�A���x���T�C�Y�ǉ� (by N.Maeda)
//              2004. 5.24 ���X�v���������R�}���h�ǉ�
//=====================================================================================================================

#include <windows.h>
#include <winspool.h>
#include <winbase.h>

#define S_ESC 0x1B
#define S_STX 0x02
#define S_ETX 0x03
#define S_ENQ 0x05

// �v�����^�^��
#define SATO_ER400 0
#define SATO_ER410 1

typedef struct{
	short PrintSpeed;			//00H=2inch/sec�A01H=3�A02H=4�A03H=5�A04H=6  (03H,04H��R8�̂�)
	short Cutter;				// �J�b�^�[���� (00H,01H,02H)
	short Hakuri;				// �n�N������ (00H,01H)
	short NonSepa;				// �m���Z�p���� (00H,01H)
	short ConcentrationSetup;	// �󎚔Z�x�ݒ� 41H�`46H (41H=R8�����l�A42H=R12�����l)
	short ConcentrationLevel;	// �󎚔Z�x���x��
	short Sensor;				// �Z���T�� (00H=����,01H=����,02H=�Z���T����)
	short Zero;					//
	short Kanji;				//
	short Paper;				//
	short Feed;					//
	short Pitch;				//
	short VDot;					// �c���x���T�C�Y(�h�b�g)
	short HDot;					// �����x���T�C�Y
	short VOffset;				// �c�N�_�␳�l(�h�b�g�j
	short HOffset;				// ��
	short PitchOffset;			// �p�n�s�b�`�I�t�Z�b�g
	short TearOffset;			// TearOff�I�t�Z�b�g
	short CutterOffset;			// �J�b�^�I�t�Z�b�g
	short HakuriOffset;			// �n�N���I�t�Z�b�g
	short Comatibility;			// �󎚈ʒu�݊�
	short Gap;					// ���x���ԃM���b�v
	short Buzzer;				// �u�U�[���ݒ�
}SATO_LESPRIT_SETTING;

class TSatoPrinter
{
	public:
		TSatoPrinter( char* ptrname, int ptrtype, BOOL* status );
		~TSatoPrinter();

		BOOL PrintStart(void);
		BOOL PrintEnd(void);

		int StartLabel(int mode, char speed);
		int EndLabel(long page);
		int SetPageSize(int x , int y);
		int SetLabelSize(int sx,int sy,int lx,int ly,int slx,int sly,int nx,int ny);
		int SetOffset(int x,int y);
		int SetPrinter( SATO_LESPRIT_SETTING *setting );


		int	 DrawStringOutline( int x,int y,char* str, int len,short type,short yoko,short tate,short propaty);
		int	 DrawString( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch);
		int	 DrawKanji( int x,int y,char* str, int len,short size ,short yoko,short tate,short pitch);
		int	 Bar39( int x,int y,short h,char* bcr,int len,BOOL guide,int hiritu,short kakudai);
		int	 Bar128( int x,int y,short h,char* bcr,int len,BOOL guide,short kakudai);

		int	 BarQR2( int x,int y,char err,short size,short mode,short joint,char* bcr, int len,short c1=0,short c2=0,short c3=0);
		int	 BarPDF417( int x,int y,short module,short pitch,short ayamari,short data_w,short symbol_row,short type,char* bcr, int len);
		int	 BarMAXI( int x,int y,short mode,char* bcr,int len,short service=0,short country=0,char* post=NULL);

		int DrawFrame(int x,int y,int xlen,int ylen,short linewidth );
		int DrawLine(int x,int y,int len,char direct,short linewidth );

	private:
		HANDLE	m_hPrinter;			// �v�����^�n���h��

	 	int  ConnectPrinter(char* ptrname);
		int  SendData(char *data,int len);
		void SetPosition( int x, int y);

		int m_prtType;

		double m_dpm;

};
#endif
