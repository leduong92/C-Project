#ifndef TPrinter_H
#define TPrinter_H
//===========================================================================
//
//		�v�����^�[�����{�N���X(ver.2.00)
//
//		FileName : TPrinter.h
//		�m�n�j������� �Z�p�{�� ���Y�Z�p��
//		�쐬���F2000.05.15 coba ���U�����ߋ��̃\�[�X���������č�蒼��
//
//===========================================================================
#include <windows.h>

#include <windowsx.h>
#include <wingdi.h>
#include <winspool.h>

#define		MAX_BARCODE		40
#define		MM_TO_INCHI		254
#define	STARTCODE_128		103


class TPrinter
{
	public :					// �ǂ̊֐�������A�N�Z�X�\==================
			HDC	hdcPrint;						// ����p�f�o�C�X�R���e�L�X�g

			// �f�t�H���g�R���X�g���N�^�i�]���Ƃ̌݊����̂��߁j
			TPrinter(void);

			//	�R���X�g���N�^�[(�f�t�H���g�p���g�p��)
			TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat);

			//	�R���X�g���N�^�[(�p���T�C�Y�w���)
			TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth, int *initStat );

			//	�R���X�g���N�^�[(�f�t�H���g�v�����^�y�їp���g�p��)
			TPrinter(int *initStat);

			~TPrinter(void);					//	�f�X�g���N�^�[
			//	�R���X�g���N�^�[(�f�t�H���g�v�����^�y�їp���g�p��)
			TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat,short pageSize,short orient);

			// �v�����^�ڍ׏��ݒ�
			int IPrinter(void);

			virtual	int	Print(void);					// ������s

			// �y�[�W�X�^�[�g�^�G���h
			int	PageStart(void);				// 
			int	PageEnd(void);	

			// �h�L�������g�X�^�[�g�^�G���h
			int	DocStart(void);	
			int	DocEnd(void);	
												// �������
			//---------------�ȉ��A�`��n�̊֐��ł�--------------------------
			// ������`��i�m�[�}���j
			void PrMessage(	int	x,				// �w���W(0.1mm)
							int	y,				// �x���W(0.1mm)
							int	nFont,			// �t�H���g�̔ԍ�(�o�^�ς�)
							int nBold,			// �t�H���g����(VC�萔)
							int	nPoint1,		// �����̃|�C���g��(X)(0.1mm)
							int	nPoint2,		// �����̃|�C���g��(Y)(0.1mm)
							LPSTR Message );	// ���������

			// �Z���^�����O������`��
			void CenterText(int	x,				// �w���W(0.1mm)
							int	y,				// �x���W(0.1mm)
							int	endX,			// �I�����W(0.1mm)
							int	nFont,			// �t�H���g�̔ԍ�(�o�^�ς�)
							int nBold,			// �t�H���g����(VC�萔)
							int	nPoint1,		// �����̃|�C���g��(X)(0.1mm)
							int	nPoint2,		// �����̃|�C���g��(Y)(0.1mm)
							LPSTR Message );	// ���������

			// �O�i�^�w�i�F���w�肵��������`��
			void PrMessage(	int	x,				// �w���W(0.1mm)
							int	y,				// �x���W(0.1mm)
							int	nFont,			// �t�H���g�̔ԍ�(�o�^�ς�)
							int nBold,			// �t�H���g����(VC�萔)
							int	nPoint1,		// �����̃|�C���g��(X)(0.1mm)
							int	nPoint2,		// �����̃|�C���g��(Y)(0.1mm)
							COLORREF colFont,
							COLORREF colBack,
							LPSTR Message );	// ���������

			// �݊����̂���
			void PrMessage(	int	x,				// �w���W(0.1mm)
							int	y,				// �x���W(0.1mm)
							int	nFont,			// �t�H���g�̔ԍ�(�o�^�ς�)
							int	nPoint1,		// �����̃|�C���g��(X)(0.1mm)
							int	nPoint2,		// �����̃|�C���g��(Y)(0.1mm)
							LPSTR Message );	// ���������

			// �o�[�R�[�h���
			void PrBCode(	int x,				// �w���W(0.1mm)
							int	y,				// �x���W(0.1mm)
							int nWidth,			// �w��(0.1mm)(�ł��ׂ�����ΏۂƂ���)
							int nHeight,		// �x��(0.1mm)
							int strOn,			// �����������Ȃ��i0=�Ȃ��A����ȊO������j
							LPSTR code);		// ���������

			// �݊����̂���
			void PrBCode(	int x,				// �w���W(0.1mm)
							int	y,				// �x���W(0.1mm)
							int nWidth,			// �w��(0.1mm)(�ł��ׂ�����ΏۂƂ���)
							int nHeight,		// �x��(0.1mm)
							LPSTR code);		// ���������
			void PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code);

			// �ȉ~
			void DrawCircle( int x, int y ,int rx ,int ry , int LineTye ,int width);

			// �^�~
			void DrawCircle( int x,int y,int r, BOOL paint);

			// ��
			void DrawLine( int sx,int sy,int ex, int ey,int LineType,int LineWidth);

			// �g�i�h��Ԃ��F�w��\�j
			void DrawRect( int sx, int sy, int ex, int ey,COLORREF theColor);

		protected :					// ���̃N���X����̂݃A�N�Z�X�\==============

			int	InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth);
			int	InitPageSettingEx(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short thePage, short theCopies);

		private :					// �h���N���X�ƃt�����h�̂݃A�N�Z�X�\========



			int	flgIPrinter;					// �v�����^�[�̏������t���O


			unsigned int	PrinterResX,PrinterResY;					// �v�����^�[�̈󎚃T�C�Y(�s�N�Z����)
			unsigned int	PrinterSizeX,PrinterSizeY;					// �v�����^�[�̃s�N�Z����(mm)

													// �o�[�R�[�h�̕������
			void PrBCodeString(	int x,				// 
								int y,				// 
								int n,				// 
								LPSTR Message );	// 

			int	ConvertMmToPixcel( int mm , int xy );


		int GetDefaultPrinter(char *theDrv, char *theDev,char *thePort);

};

#endif

