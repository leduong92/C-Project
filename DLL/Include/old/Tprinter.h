#ifndef TPrinter_H
#define TPrinter_H

//===============================================================================================
//
//		�v�����^�[�����{�N���X(������)
//
//		FileName : TPrinter.h
//		�m�n�j������� �Z�p�{�� ���Y�Z�p��
//		�쐬���F1996.10.9 by �O�c
//		�C�����F1998.2.27 by Koba	
//					�X������ 
//		�C�����F1998.9.16 by Coba
//					�}�b�s���O���[�h�ˑ����Ȃ�����
//		�C�����F1998.12.22 by Coba
//					�p���T�C�Y�w��@�\�̒ǉ�
//		�C�����F2000.06.02 by TAKE
//					�~�܂��͑ȉ~�̕`���ǉ� DrawCircle
//		�C�����F2002.06.04 by coba
//					�t�H���g�w��̕����o�͋@�\��ǉ�
//		�C�����F2002.06.11 by coba
//					�t�H���g�w��̕����o�͊֐��ɃL�����N�^�Z�b�g�w��@�\
//		�C�����F2002.07.25 by coba
//					�u�v�����^�̐ݒ�_�C�A���O�v�̓��e�����̂܂ܔ��f����֐���ǉ�
//					����ɂ��A�h���C�u�E�f�o�C�X�E�|�[�g�̎w�肪�s�v�ɁB
//		�C�����F2003.07.08 by coba
//					InitPageSetting()�ŁA"հ�ޗp��"�𖾎��I�Ɏg�p���邩�w��ł���悤�ɕύX
//					"հ�ޗp��"�������Ȃ��v�����^��"հ�ގw��"���g���Ɨ\�����ʃT�C�Y�ō쐬�����
//					���܂��̂�h�~����BVP�n�v�����^�h���C�o�΍�
//		�C�����F2003.11.17 by coba
//					�n�b�`���O�h��Ԃ������`�̕`��֐�DrawHatchBox()��ǉ�
//		�C�����F2004.04.28 by TAKE	InitPageSettingEx TAKE �g���C�̃R���g���[����ǉ�
//		�C�����F2004.09.27 by ����	QRCODE����@�\�̒ǉ�
//									QRCODE���g�p����Ƃ��́ATPrintQR.CPP��ǉ�����
//		�C�����F2004.10.05 by coba
//					PrGaiji()��BitMap�f�[�^��������Ă����o�O�̏C��//
//		�C�����F2004.10.28 by coba
//					������Ή���CenterText()��ǉ�
//		�C�����F2004.12.08 by ����
//					DrawR() �� DrawBitmap()��ǉ�
//===============================================================================================

//#include "..\stdafx.h"
#include <windows.h>

#include <windowsx.h>
#include <wingdi.h>
#include <winspool.h>

#define		MAX_BARCODE		40
#define		MM_TO_INCHI		254
#define		GAIJIBITMAPSIZE		100

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

		int OpenPrintDialog(HWND hwnd);

		// �v�����^�ڍ׏��ݒ�
		int IPrinter(void);

		virtual	int	Print(void);					// ������s


												// �������
		void PrMessage(	int	x,				// �w���W(0.1mm)
						int	y,				// �x���W(0.1mm)
						int	nFont,			// �t�H���g�̔ԍ�(�o�^�ς�)
						int nBold,			// �t�H���g����(VC�萔)
						int	nPoint1,		// �����̃|�C���g��(X)(0.1mm)
						int	nPoint2,		// �����̃|�C���g��(Y)(0.1mm)
						LPSTR Message );	// ���������

		void CenterText(int	x,				// �w���W(0.1mm)
						int	y,				// �x���W(0.1mm)
						int	endX,			// �I�����W(0.1mm)
						int	nFont,			// �t�H���g�̔ԍ�(�o�^�ς�)
						int nBold,			// �t�H���g����(VC�萔)
						int	nPoint1,		// �����̃|�C���g��(X)(0.1mm)
						int	nPoint2,		// �����̃|�C���g��(Y)(0.1mm)
						LPSTR Message );	// ���������

		void CenterText(int	x,				// �w���W(0.1mm)
						int	y,				// �x���W(0.1mm)
						int	endX,			// �I�����W(0.1mm)
						int	nFont,			// �t�H���g�̔ԍ�(�o�^�ς�)
						int	nPoint1,		// �����̃|�C���g��(X)(0.1mm)
						int	nPoint2,		// �����̃|�C���g��(Y)(0.1mm)
						LPSTR Message );	// ���������

		void CenterTextEx(int	x,				// �w���W(0.1mm)
						int	y,				// �x���W(0.1mm)
						int	endX,			// �I�����W(0.1mm)
						int nBold,			// �t�H���g����(VC�萔)
						int	nPoint1,		// �����̃|�C���g��(X)(0.1mm)
						int	nPoint2,		// �����̃|�C���g��(Y)(0.1mm)
						char* fontname,		// �t�H���g�̖��O
						DWORD charset,		// �L�����N�^�Z�b�g
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

		void PrMsgEx(	int	x,				// �w���W(0.1mm)
						int	y,				// �x���W(0.1mm)
						char* fontname,		// �t�H���g�̖��O
						DWORD charset,		// �L�����N�^�Z�b�g
						int nBold,			// �t�H���g����(VC�萔)
						int	nPoint1,		// �����̃|�C���g��(X)(0.1mm)
						int	nPoint2,		// �����̃|�C���g��(Y)(0.1mm)
						LPSTR Message );	// ���������

		void PrMsgEx(	int	x,				// �w���W(0.1mm)
						int	y,				// �x���W(0.1mm)
						LPSTR Message,		// ���������
						// �ȉ���CreateFont�̈����B�A���AnHeight��nWidth��0.1mm�P�ʂ̎w��ł�
						int nHeight, // logical height of font
						int nWidth, // logical average character width
						int nEscapement, // angle of escapement
						int nOrientation, // base-line orientation angle
						int fnWeight, // font weight
						DWORD fdwItalic, // italic attribute flag
						DWORD fdwUnderline, // underline attribute flag
						DWORD fdwStrikeOut, // strikeout attribute flag
						DWORD fdwCharSet, // character set identifier
						DWORD fdwOutputPrecision, // output precision
						DWORD fdwClipPrecision, // clipping precision
						DWORD fdwQuality, // output quality
						DWORD fdwPitchAndFamily, // pitch and family
						LPCTSTR lpszFace // pointer to typeface name string
					);


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

		// �K�i�����̋����o�[�R�[�h���
		void PrNarrowBCode(	int x,				// �w���W(0.1mm)
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
												// �O����

		void PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code);

		int	PageStart(void);				// 
		int	PageEnd(void);	

	// �h���N���X�ƃt�����h�̂݃A�N�Z�X�\========
		// �}�`�`��
		void DrawCircle( int x, int y ,int rx ,int ry , int LineTye ,int width);
		void DrawLine( int sx,int sy,int ex, int ey,int LineType,int LineWidth);
		void DrawLine( int sx,int sy,int ex, int ey,int LineType,int LineWidth,COLORREF color);
		// 2003.11.17 ���Βǉ�
		void DrawHatchBox(int sx,int sy,int ex, int ey,int LineType,int LineWidth,int htachtype);
	
		//R��`�� 2004.12.08 ���ޒǉ�
		void DrawR( int x, int y ,int r ,int mode , int LineType ,int width);

		//BitMap��`�悷�� 2004.12.08 ���ޒǉ�
		void DrawBitmap( int x, int y ,HANDLE hResource );
	
	protected :
		int	InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth,BOOL userpaper=FALSE);
		int	InitPageSettingEx(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short thePage, short theCopies);
		// 2004.04.28 TAKE �g���C�̃R���g���[����ǉ�
		// short theTray : DMBIN_AUTO:�����@ DMBIN_MANUAL�F�荷���g���C
		int	InitPageSettingEx(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short thePage, short theCopies, short tray);
			// �H���i���p�̃I�v�V����
			// ������
			void PrStump( int x, int y ,int theForm,LPSTR Day ,LPSTR Inspector);
			void PrStumpLaser( int x, int y ,int theForm,LPSTR Day ,LPSTR Inspector);

			// Snem���̃}�[�N
			void PrGaiji(	int x,				// �w���W(0.1mm)
							int y,				// �x���W(0.1mm)
							int nWidth,			// �w��(0.1mm)
							int nHeight,		// �x��(0.1mm)
							int	GaijiNo );		// �O���o�^No.
												// S = 0
												// N = 1
												// E = 2


		// ���ʌ݊��ׂ̈̂�
		int IPrinter( LPSTR sPrintName , int nPrinterNo , int PageLength, int dummy );


	// ���̃N���X����̂݃A�N�Z�X�\==============
	private :
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

		// ���ʌ݊��ׂ̈̂�
		int GetIni( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName);

//==================================================================================================
//==================================================================================================
//		�ȉ��A�{�̂� TPrintQR.cpp�ɂ���̂ŁA�g���ꍇ��TPrinQR.cpp��ǉ����A
//		�C���|�[�g���C�u����"qr2dll.lib"���g�p���邱��
//==================================================================================================
//==================================================================================================
	public:
		int PrintQRCode(	char *txt,			//char *txt	�󎚕���(�ő啶����256)
							long lngErrorLevel,	//long lngErrorLevel	���������x�� L(7%),M(15%),Q(25%),H(30%)
							long lngPrintQlty,	//long lngPrintQlty	����T�C�Y�i0:0.16mm,1:0.19mm,2:0.25mm,3:0.32mm�j
							int	 x,				// �w���W(0.1mm)
							int  y);				// �x���W(0.1mm)

		//�݊����̂���
		int PrintQRCode(	char *txt,			//char *txt	�󎚕���(�ő啶����256)
							long lngQRversion,	//long version	QR�o�[�W����
							long lngErrorLevel,	//long lngErrorLevel	���������x�� L(7%),M(15%),Q(25%),H(30%)
							long lngPrintQlty,	//long lngPrintQlty	����T�C�Y�i0:0.16mm,1:0.19mm,2:0.25mm,3:0.32mm�j
							int	 x,				// �w���W(0.1mm)
							int  y);				// �x���W(0.1mm)


};



#endif

