//=======================================================================================================================================
//
//  	�v�����^�[�̐����{�N���X
//	
//		FileName : TPrinter.cpp
//		�m�n�j������� �Z�p�{�� ���Y�Z�p��
//		�쐬���F2000.05.15 coba ���U�����ߋ��̃\�[�X���������č�蒼��
//		�C�����F
//
//===========================================================================
#include	<stdio.h>
#include	"Tprinter.h"



// ASCII�������o�[�R�[�h�e�[�u���̔ԍ��֕ϊ�����e�[�u��
static char BarcodeConvTbl[256]={
	//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//0
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//1
		38,41,38,38,40,43,38,38,38,38,39,42,38,36,37,38,	//2
		 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,38,38,38,38,38,	//3
		38,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,	//4
		25,26,27,28,29,30,31,32,33,34,35,38,38,38,38,38,	//5
		38,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,	//6
		25,26,27,28,29,30,31,32,33,34,35,38,38,38,38,38,	//7
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//8
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//9
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//A
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//B
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//C
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//D
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//E
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38};	//F



// �o�[�R�[�h�̃e�[�u��
// BarCode(Code-39) BASR(5)+SPACE(4) ���̍L�������P���������O
//���Ԃ͍��A���A���̏��Ԃɕ���ł���
static char BarcodeTbl[44][11]={
						{1,0,0,1,0,0,0,0,1,0,'1'},	// 00 ��������������������������
						{0,0,1,1,0,0,0,0,1,0,'2'},	// 01 ��������������������������
						{1,0,1,1,0,0,0,0,0,0,'3'},	// 02 ��������������������������
						{0,0,0,1,1,0,0,0,1,0,'4'},	// 03 ��������������������������
						{1,0,0,1,1,0,0,0,0,0,'5'},	// 04 ��������������������������
						{0,0,1,1,1,0,0,0,0,0,'6'},	// 05
						{0,0,0,1,0,0,1,0,1,0,'7'},	// 06
						{1,0,0,1,0,0,1,0,0,0,'8'},	// 07
						{0,0,1,1,0,0,1,0,0,0,'9'},	// 08
						{0,0,0,1,1,0,1,0,0,0,'0'},	// 09
						{1,0,0,0,0,1,0,0,1,0,'A'},	// 10
						{0,0,1,0,0,1,0,0,1,0,'B'},	// 11
						{1,0,1,0,0,1,0,0,0,0,'C'},	// 12
						{0,0,0,0,1,1,0,0,1,0,'D'},	// 13
						{1,0,0,0,1,1,0,0,0,0,'E'},	// 14
						{0,0,1,0,1,1,0,0,0,0,'F'},	// 15
						{0,0,0,0,0,1,1,0,1,0,'G'},	// 16
						{1,0,0,0,0,1,1,0,0,0,'H'},	// 17
						{0,0,1,0,0,1,1,0,0,0,'I'},	// 18
						{0,0,0,0,1,1,1,0,0,0,'J'},	// 19
						{1,0,0,0,0,0,0,1,1,0,'K'},	// 20
						{0,0,1,0,0,0,0,1,1,0,'L'},	// 21
						{1,0,1,0,0,0,0,1,0,0,'M'},	// 22
						{0,0,0,0,1,0,0,1,1,0,'N'},	// 23
						{1,0,0,0,1,0,0,1,0,0,'O'},	// 24
						{0,0,1,0,1,0,0,1,0,0,'P'},	// 25
						{0,0,0,0,0,0,1,1,1,0,'Q'},	// 26
						{1,0,0,0,0,0,1,1,0,0,'R'},	// 27
						{0,0,1,0,0,0,1,1,0,0,'S'},	// 28
						{0,0,0,0,1,0,1,1,0,0,'T'},	// 29
						{1,1,0,0,0,0,0,0,1,0,'U'},	// 30
						{0,1,1,0,0,0,0,0,1,0,'V'},	// 31
						{1,1,1,0,0,0,0,0,0,0,'W'},	// 32
						{0,1,0,0,1,0,0,0,1,0,'X'},	// 33
						{1,1,0,0,1,0,0,0,0,0,'Y'},	// 34
						{0,1,1,0,1,0,0,0,0,0,'Z'},	// 35
						{0,1,0,0,0,0,1,0,1,0,'_'},	// 36
						{1,1,0,0,0,0,1,0,0,0,'.'},	// 37
						{0,1,1,0,0,0,1,0,0,0,' '},	// 38
						{0,1,0,0,1,0,1,0,0,0,'*'},	// 39
						{0,1,0,1,0,1,0,0,0,0,'$'},	// 40
						{0,1,0,1,0,0,0,1,0,0,'!'},	// 41
						{0,1,0,0,0,1,0,1,0,0,'+'},	// 42
						{0,0,0,1,0,1,0,1,0,0,'%'}};	// 43

static char BarcodeConvTbl128A[256]={
	//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,	//0
		80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,	//1
		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,	//2
		16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,	//3
		32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,	//4
		48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,	//5
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//6
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//7
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//8
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//9
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//A
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//B
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//C
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//D
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//E
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38};	//F

static char BarcodeConvTbl128B[256]={
	//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,	//0
		80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,	//1
		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,	//2
		16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,	//3
		32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,	//4
		48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,	//5
		64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,	//6
		80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,	//7
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//8
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//9
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//A
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//B
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//C
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//D
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//E
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38};	//F

// �o�[�R�[�h�̃e�[�u��
static char BarcodeTbl128[106][6]={
						{2,1,2,2,2,2},	// 00 ��������������������������
						{2,2,2,1,2,2},	// 01 ��������������������������
						{2,2,2,2,2,1},
						{1,2,1,2,2,3},
						{1,2,1,3,2,2},
						{1,3,1,2,2,2},
						{1,2,2,2,1,3},
						{1,2,2,3,1,2},
						{1,3,2,2,1,2},
						{2,2,1,2,1,3},
						{2,2,1,3,1,2},
						{2,3,1,2,1,2},	// 01 ��������������������������
						{1,1,2,2,3,2},
						{1,2,2,1,3,2},
						{1,2,2,2,3,1},
						{1,1,3,2,2,2},
						{1,2,3,1,2,2},
						{1,2,3,2,2,1},
						{2,2,3,2,1,1},
						{2,2,1,1,3,2},
						{2,2,1,2,3,1},
						{2,1,3,2,1,2},	// 01 ��������������������������
						{2,2,3,1,1,2},
						{3,1,2,1,3,1},
						{3,1,1,2,2,2},
						{3,2,1,1,2,2},
						{3,2,1,2,2,1},
						{3,1,2,2,1,2},
						{3,2,2,1,1,2},
						{3,2,2,2,1,1},
						{2,1,2,1,2,3},
						{2,1,2,3,2,1},	// 01 ��������������������������
						{2,3,2,1,2,1},
						{1,1,1,3,2,3},
						{1,3,1,1,2,3},
						{1,3,1,3,2,1},
						{1,1,2,3,1,3},
						{1,3,2,1,1,3},
						{1,3,2,3,1,1},
						{2,1,1,3,1,3},
						{2,3,1,1,1,3},
						{2,3,1,3,1,1},	// 01 ��������������������������
						{1,1,2,1,3,3},
						{1,1,2,3,3,1},
						{1,3,2,1,3,1},
						{1,1,3,1,2,3},
						{1,1,3,3,2,1},
						{1,3,3,1,2,1},
						{3,1,3,1,2,1},
						{2,1,1,3,3,1},
						{2,3,1,1,3,1},
						{2,1,3,1,1,3},	// 01 ��������������������������
						{2,1,3,3,1,1},
						{2,1,3,1,3,1},
						{3,1,1,1,2,3},
						{3,1,1,3,2,1},
						{3,3,1,1,2,1},
						{3,1,2,1,1,3},
						{3,1,2,3,1,1},
						{3,3,2,1,1,1},
						{3,1,4,1,1,1},
						{2,2,1,4,1,1},	// 01 ��������������������������
						{4,3,1,1,1,1},
						{1,1,1,2,2,4},
						{1,1,1,4,2,2},
						{1,2,1,1,2,4},
						{1,2,1,4,2,1},
						{1,4,1,1,2,2},
						{1,4,1,2,2,1},
						{1,1,2,2,1,4},
						{1,1,2,4,1,2},
						{1,2,2,1,1,4},	// 01 ��������������������������
						{1,2,2,4,1,1},
						{1,4,2,1,1,2},
						{1,4,2,2,1,1},
						{2,4,1,2,1,1},
						{2,2,1,1,1,4},
						{4,1,3,1,1,1},
						{2,4,1,1,1,2},
						{1,3,4,1,1,1},
						{1,1,1,2,4,2},
						{1,2,1,1,4,2},	// 01 ��������������������������
						{1,2,1,2,4,1},
						{1,1,4,2,1,2},
						{1,2,4,1,1,2},
						{1,2,4,2,1,1},
						{4,1,1,2,1,2},
						{4,2,1,1,1,2},
						{4,2,1,2,1,1},
						{2,1,2,1,4,1},
						{2,1,4,1,2,1},
						{4,1,2,1,2,1},	// 01 ��������������������������
						{1,1,1,1,4,3},
						{1,1,1,3,4,1},
						{1,3,1,1,4,1},
						{1,1,4,1,1,3},
						{1,1,4,3,1,1},
						{4,1,1,1,1,3},
						{4,1,1,3,1,1},
						{1,1,3,1,4,1},
						{1,1,4,1,3,1},
						{3,1,1,1,4,1},	// 01 ��������������������������
						{4,1,1,1,3,1},
						{2,1,1,4,1,2},		// 103=STOP-A
						{2,1,1,2,1,4},
						{2,1,1,2,3,2}};



static char BarcodeTblStop[7]={2,3,3,1,1,1,2};

// ������镶���t�H���g�̃e�[�u��
static char *FontName[]={{"Default"},					// 0
						{"�l�r ����"},					// 1
						{"�l�r �S�V�b�N"},				// 2
						{"Century Gothic"},				// 3
						{"Courier New"},				// 4
						{"Courier New Bold"},			// 5
						{"Courier New Bold Italic"},	// 6
						{"Courier New Italic"},			// 7
					//	{"�g�f�޼��E-PRO"},				// 8
						{"Arial"},				// 8
						{"Impact"}};					// 9



//================================================================================================
//	TPrinter::TPrinter(void)
//	�f�t�H���g�R���X�g���N�^�[
//	�����F������g���ꍇ�́A���̌�ɖ����I�� InitPageSetting()���ĂԂ���!!
//	�����F�g�p����v�����^���w�肵�ăI�u�W�F�N�g�쐬�B�p���̓v�����^�̃f�t�H���g���g��
//	�����@�Ȃ�
//================================================================================================
TPrinter::TPrinter(void)
{
	flgIPrinter = 0;		// �v�����^�[�̏������t���O(��������)
}

//================================================================================================
// TPrinter::TPrinter(int *initStat)
//	�R���X�g���N�^�[(�f�t�H���g�p���A�f�t�H���g�v�����^���g�p)
//	�����F������g���ꍇ�́A���̌�ɖ����I�� InitPageSetting()���ĂԂ���!!
//	�����F�g�p����v�����^���w�肵�ăI�u�W�F�N�g�쐬�B�p���̓v�����^�̃f�t�H���g���g��
//	�����@�Ȃ�
//================================================================================================
TPrinter::TPrinter(int *initStat)
{
	int stat;
	char csDrv[256], csDev[256], csPort[256];

	flgIPrinter = 0;		// �v�����^�[�̏������t���O(��������)

	// �ڑ��v�����^���擾
	stat = GetDefaultPrinter(csDrv,csDev,csPort);

	// �擾���s
	if( stat ){
		*initStat = stat;
		return;
	}

	// �f�o�C�X�R���e�L�X�g�̍쐬
	hdcPrint = CreateDC( csDrv, csDev, csPort,NULL);
	//hdcPrint = CreateDC(lpPrtDriver,lpPrtDevice,lpPrtPort, NULL);
	if( hdcPrint == NULL ){
		*initStat = -2;
		return ;
	}

	// �p���T�C�Y�̎擾(�s�N�Z����)
	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	// 
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	// 


	// �}�b�s���O���[�h�̎擾
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		// ����ɂ��v�����^�̍��W�n����ʂ̍��W�n�Ə㉺���]���܂�
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );


	IPrinter();
	*initStat = 0;
}

//================================================================================================
//	TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat)
//	�R���X�g���N�^�[(�f�t�H���g�p���g�p�ް�ޮ�)
//	�����F�g�p����v�����^���w�肵�ăI�u�W�F�N�g�쐬�B�p���̓v�����^�̃f�t�H���g���g��
//	�����P�FtheDrv	�h���C�o��
//	�����Q�FtheDev	�f�o�C�X��
//	�����R�FthePort	�|�[�g��
//	�����S�FinitStat	�������X�e�[�^�X�i�o�́j
//================================================================================================
TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat)
{
	flgIPrinter = 0;		// �v�����^�[�̏������t���O(��������)
	int stat;

	// �f�o�C�X�R���e�L�X�g�̍쐬
	hdcPrint = CreateDC( theDrv, theDev, thePort,NULL);
	//hdcPrint = CreateDC(lpPrtDriver,lpPrtDevice,lpPrtPort, NULL);
	if( hdcPrint == NULL ){
		*initStat = -2;
		return ;
	}

	// �p���T�C�Y�̎擾(�s�N�Z����)
	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	// 
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	// 


	// �}�b�s���O���[�h�̎擾
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		// ����ɂ��v�����^�̍��W�n����ʂ̍��W�n�Ə㉺���]���܂�
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );


	IPrinter();
	*initStat = 0;
}


//==========================================================================================================
//	�R���X�g���N�^�[(�p���w���ް�ޮ�)
//	�����F�g�p����v�����^�Ɨp���T�C�Y���w�肵�ăI�u�W�F�N�g�쐬�B
//	�����P�FtheDrv		�h���C�o��
//	�����Q�FtheDev		�f�o�C�X��
//	�����R�FthePort		�|�[�g��
//	�����S�FtheLength	�p���c�T�C�Y(0.1mm)
//	�����T�FtheLength	�p�����T�C�Y(0.1mm)
//	�����U�FinitStat	�������X�e�[�^�X�i�o�́j
//==========================================================================================================
TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth, int *initStat)
{
	flgIPrinter = 0;		// �v�����^�[�̏������t���O(��������)

	// �f�o�C�X�R���e�L�X�g���쐬
	*initStat = InitPageSetting(theDrv, theDev, thePort, theLength,theWidth);

	return;
}



//================================================================================================
//	�f�X�g���N�^�[
//================================================================================================
TPrinter::~TPrinter()
{
	// �v�����^�[�̏������t���O
	if( flgIPrinter != 0 ){
		EndDoc(hdcPrint);		// ����I��

		DeleteDC(hdcPrint);		// ����p�R���e�L�X�g�폜
	}
}


//============================================================================
//	�T�v : �������֌W�̏�����(ʰ�ޤ���)���s���֐�
//
//	Return: int ( 0:OK or 1:NG )
//
//============================================================================
int TPrinter::IPrinter( void )
{
	DocStart();
	flgIPrinter = 1;						// ���Ready(�v�����^�w��ς�)�̃t���O

	return 0;
}

//===========================================================================================================
//	int	TPrinter::InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth)
//	�����F�g�p����v�����^�Ɨp���T�C�Y���w�肵�ăf�o�C�X�R���e�L�X�g���쐬�B
//	�����P�FtheDrv		�h���C�o��
//	�����Q�FtheDev		�f�o�C�X��
//	�����R�FthePort		�|�[�g��
//	�����S�FtheLength	�p���c�T�C�Y(0.1mm)
//	�����T�FtheLength	�p�����T�C�Y(0.1mm)
//	�����U�FinitStat	�������X�e�[�^�X�i�o�́j
//===========================================================================================================
int	TPrinter::InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth)
{

	int		stat;				// �ݒ�l�ǂݏo���X�e�[�^�X
	HANDLE	phPrinter;			// �v�����^�n���h��
	DWORD	dwDmSize;				// �v�����^�[�̃C���t�H���[�V�������f�[�^�T�C�Y
	DEVMODE	*devNew;			// �����l�|���ύX����
	char *pszIn;
	DWORD	newdmVal;


	// �v�����^�[�̃n���h���擾���āADEVMODE�\���̂ɕK�v�ȃT�C�Y���擾
	if( OpenPrinter( theDev , &phPrinter , NULL ) == FALSE ){
		return(-1);
	}


	// (�v�����^�[�̐ݒ���"DEVMODE"�ɕK�v�ȃf�[�^�T�C�Y�̎擾)
	dwDmSize = DocumentProperties(	NULL ,			// handle to window that displays dialog box
									phPrinter,			// handle to printer object
									theDrv,				// pointer to device name (�����炭Driver name�̂��Ƃł��傤)
								 	NULL ,				// pointer to modified device mode structure
									NULL ,				// pointer to original device mode structure
 									0 );				// mode flg ( 0:Get DEVMODE structure size )

 
	// DEVMODE�\���̗p�Ƀ������A���P�[�g
	pszIn = new char[dwDmSize+1];
	devNew = (DEVMODE *)pszIn;

	// DEVMODE�\���̂Ƀf�[�^�擾
	stat = DocumentProperties(	NULL,phPrinter,theDrv,devNew ,NULL ,	DM_OUT_BUFFER );	// mode flg (�ݒ�l�擾)
	if( stat != IDOK ){
		ClosePrinter( phPrinter );
		delete( pszIn );
		return(-2);
	}

	// �v�����^�n���h���N���[�Y
	ClosePrinter( phPrinter );

	// �p���T�C�Y�֘A�̐ݒ��ύX
	devNew -> dmPaperSize = DMPAPER_USER;			// ���[�U�[�w��T�C�Y
	devNew -> dmPaperLength = theLength;			// �p������
	devNew -> dmPaperWidth = theWidth;				// �p������

	// dmFields�����o�̃t���O��ݒ�
	newdmVal = devNew -> dmFields;
	devNew -> dmFields = newdmVal|DM_PAPERSIZE|DM_PAPERLENGTH|DM_PAPERWIDTH;


	// �f�o�C�X�R���e�L�X�g�̍쐬
	hdcPrint = CreateDC( theDrv, theDev, thePort, devNew );
	if( hdcPrint == NULL ){
		delete( pszIn );
		return(-3);
	}

	// �p���T�C�Y�̎擾(�s�N�Z����)
	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	// 
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	// 
	// �}�b�s���O���[�h�̎擾
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		// ����ɂ��v�����^�̍��W�n����ʂ̍��W�n�Ə㉺���]���܂�
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );

	flgIPrinter = 1;						// ���Ready(�v�����^�w��ς�)�̃t���O

	IPrinter();

	return(0);
}

//================================================================================================
// TPrinter::TPrinter(int *initStat)
//	�R���X�g���N�^�[(�f�t�H���g�v�����^���g�p)
//	���� �F�g�p����v�����^���w�肵�ăI�u�W�F�N�g�쐬�B�p���ƌ������w��
//	����1�F�X�e�[�^�X�o�͗p
//	����2�F�ŃT�C�Y�iDEVMODE�\���̂�dmPaperSize�Q�Ɓj
//	����3:�������
//================================================================================================
TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat,short pageSize,short orient)
{

	flgIPrinter = 0;		// �v�����^�[�̏������t���O(��������)


	// �f�o�C�X�R���e�L�X�g�̍쐬
	*initStat = InitPageSettingEx(theDrv, theDev, thePort, pageSize, orient);
	return;
}


//===========================================================================================================
//	int	TPrinter::InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth)
//	�����F�g�p����v�����^�Ɨp���T�C�Y�ƕ������w�肵�ăf�o�C�X�R���e�L�X�g���쐬�B
//	�����P�FtheDrv		�h���C�o��
//	�����Q�FtheDev		�f�o�C�X��
//	�����R�FthePort		�|�[�g��
//	�����S�FthePage		�p���c�T�C�Y(DEVMODE�\����)
//	�����T�FtheOrient	�p������
//===========================================================================================================
int	TPrinter::InitPageSettingEx(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short thePage ,short theOrient)
{

	int		stat;				// �ݒ�l�ǂݏo���X�e�[�^�X
	HANDLE	phPrinter;			// �v�����^�n���h��
	DWORD	dwDmSize;				// �v�����^�[�̃C���t�H���[�V�������f�[�^�T�C�Y
	DEVMODE	*devNew;			// �����l�|���ύX����
	char *pszIn;
	DWORD	newdmVal;


	// �v�����^�[�̃n���h���擾���āADEVMODE�\���̂ɕK�v�ȃT�C�Y���擾
	if( OpenPrinter( theDev , &phPrinter , NULL ) == FALSE ){
		return(-1);
	}


	// (�v�����^�[�̐ݒ���"DEVMODE"�ɕK�v�ȃf�[�^�T�C�Y�̎擾)
	dwDmSize = DocumentProperties(	NULL ,			// handle to window that displays dialog box
									phPrinter,			// handle to printer object
									theDrv,				// pointer to device name (�����炭Driver name�̂��Ƃł��傤)
								 	NULL ,				// pointer to modified device mode structure
									NULL ,				// pointer to original device mode structure
 									0 );				// mode flg ( 0:Get DEVMODE structure size )

 
	// DEVMODE�\���̗p�Ƀ������A���P�[�g
	pszIn = new char[dwDmSize+1];
	devNew = (DEVMODE *)pszIn;

	// DEVMODE�\���̂Ƀf�[�^�擾
	stat = DocumentProperties(	NULL,phPrinter,theDrv,devNew ,NULL ,	DM_OUT_BUFFER );	// mode flg (�ݒ�l�擾)
	if( stat != IDOK ){
		ClosePrinter( phPrinter );
		delete( pszIn );
		return(-2);
	}

	// �v�����^�n���h���N���[�Y
	ClosePrinter( phPrinter );

	// �p���T�C�Y�֘A�̐ݒ��ύX
	devNew -> dmPaperSize = thePage;			// �p���T�C�Y
	devNew -> dmCopies = 1;			// �p���T�C�Y
	devNew -> dmOrientation = theOrient;			// �p������
	devNew ->dmColor=DMCOLOR_COLOR;

	// dmFields�����o�̃t���O��ݒ�
	newdmVal = devNew -> dmFields;
	devNew -> dmFields = newdmVal|DM_PAPERSIZE|DM_COPIES|DM_ORIENTATION|DM_COLOR;


	// �f�o�C�X�R���e�L�X�g�̍쐬
	hdcPrint = CreateDC( theDrv, theDev, thePort, devNew );
	if( hdcPrint == NULL ){
		delete( pszIn );
		return(-3);
	}

	// �p���T�C�Y�̎擾(�s�N�Z����)
	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	// 
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	// 
	// �}�b�s���O���[�h�̎擾
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		// ����ɂ��v�����^�̍��W�n����ʂ̍��W�n�Ə㉺���]���܂�
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );

	flgIPrinter = 1;						// ���Ready(�v�����^�w��ς�)�̃t���O

	IPrinter();

	return(0);
}



//============================================================================
//	�T�v : ����̎��s(�I�[�o�[���C�h�����)
//	
//	Return:

//	
//============================================================================
int TPrinter::Print(void)
{
	char	prMes[] = "����t�H�[��Print();���ݒ�(�I�[�o�[���C�h)����Ă��܂���I�I";
	// �v�����^�̑I�����s���Ă��邩�̃`�F�b�N
	if( flgIPrinter == 0 ){
		return 1;	// �G���[
	}
	return 0;
}

//============================================================================
//			�T  �v : ����y�[�W�̊J�n
//	Input:	
//	Return:
//============================================================================
int	TPrinter::PageStart( void )
{
	int stat;

	if( flgIPrinter == 0 ) return 1;		// �G���[

	stat = StartPage( hdcPrint );


	if( stat != 0 ){
		return GetLastError();
	}
	return 0;

}

//============================================================================
//			�T  �v : ����y�[�W�̏I��
//	Input:
//	Return:
//============================================================================
int	TPrinter::PageEnd( void )
{
	int stat;

	if( flgIPrinter == 0 ) return 1;		// �G���[
	stat = EndPage( hdcPrint );

	if( stat != 0 ){
		return GetLastError();
	}
	return 0;
}


//============================================================================
//	int TPrinter::DocStart( void )
//	public
//	�O������v�����^�[��StartDoc()��������
//
//============================================================================
int TPrinter::DocStart( void )
{
	DOCINFO docinfo;				// ����p�f�o�C�X�R���e�L�X�g

	// �v�����^�ݒ�
	docinfo.cbSize = sizeof(docinfo);		// 
	docinfo.lpszDocName= "NOK Printer";	// ������Ƀv�����^�ɏo�郁�b�Z�[�W
	docinfo.lpszOutput = (LPSTR)NULL;		// 
	docinfo.lpszDatatype = NULL;			// 
	docinfo.fwType = 0;						// 
	StartDoc(hdcPrint,&docinfo);			// �������


	return 0;
}

//============================================================================
//	int	TPrinter::DocEnd( void )
//	public
//	�O������v�����^�[��EndDoc()��������
//
//============================================================================
int	TPrinter::DocEnd( void )
{
	int stat;

	if( flgIPrinter == 0 ) return 1;		// �G���[
		stat = EndDoc( hdcPrint );

	if( stat != 0 ){
		return GetLastError();
	}
	return 0;
}

//============================================================================
//		�T�v : �������
//		Input:	int x;			�w���W
//				int y;			�x���W
//				int nFont;		�t�H���g�̔ԍ��f�t�H���g�͂O
//				int nBold:		���̑����iCreateFont�̒萔�j
//				int nPoint1;	�t�H���g�̃T�C�Y(X)
//				int nPoint2;	�t�H���g�̃T�C�Y(Y)
//				LPSTR Message;	������
//		Return:
//============================================================================
void TPrinter::PrMessage( int x, int y, int nFont, int nBold,int nPoint1, int nPoint2, LPSTR Message )
{
	HFONT	holdf,hnewf;
	HDC		hdc;
	int		sx,sy;

	TEXTMETRIC	textmetric;

	hdc = hdcPrint;


	sx = ConvertMmToPixcel( x , 0 );	// �󎚂w���W
	sy = ConvertMmToPixcel( y , 1 );	// �󎚂x���W

	GetTextMetrics(hdc,&textmetric);

	if( nPoint2 == 0 ) nPoint2 = nPoint1;

	nPoint1 = ConvertMmToPixcel( nPoint1 , 0 );
	nPoint2 = ConvertMmToPixcel( nPoint2 , 1 );

	if( nFont != 0 && nFont <= 8 ){		// �t�H���g�̌���
		hnewf = CreateFont(	nPoint2,nPoint1,				// �t�H���g�T�C�Y
							0,					// Escapement
							0,					// Orientation
							nBold,				// Weight(�t�H���g�̑���)
							0,					// Ital(�C�^���b�N��)
							0,					// Underline(����)
							0,					// StrikeThru(�ł�������)
							SHIFTJIS_CHARSET,	// Charset(�����Z�b�g)
							OUT_DEFAULT_PRECIS,	// Precision(�o�͐��x)
							CLIP_DEFAULT_PRECIS,// ClipPrecision(�N���b�s���O���x)
							DEFAULT_QUALITY,	// Quality(�t�H���g�̏o�͕i��)
							DEFAULT_PITCH | FF_DONTCARE,	// Pitch(�t�H���g�̃s�b�`�ƃt�@�~��)
//							DEFAULT_PITCH | FF_ROMAN,	// Pitch(�t�H���g�̃s�b�`�ƃt�@�~��)
							FontName[nFont] );	// FontName
		holdf = SelectObject( hdc , hnewf );	// �t�H���g�̐؂�ւ�

		TextOut(hdc,sx,sy,Message,strlen(Message));

		hnewf = SelectObject( hdc , holdf );
		DeleteObject( hnewf );

	}else{
		TextOut(hdc,sx,sy,Message,strlen(Message));
	}

}
// �݊����̈�
void TPrinter::PrMessage( int x, int y, int nFont,int nPoint1, int nPoint2, LPSTR Message )
{
	PrMessage( x,  y, nFont, FW_SEMIBOLD, nPoint1, nPoint2, Message );
}
//============================================================================
//		�T�v : �������(�����悹)
//	x,endX�Ԃŕ����𒆉��悹�ň�
//		Input:	int x;			�w���W
//				int y;			�x���W
//				int endX;		X���W
//				int nFont;		�t�H���g�̔ԍ��f�t�H���g�͂O
//				int nBold:		���̑����iCreateFont�̒萔�j
//				int nPoint1;	�t�H���g�̃T�C�Y(X)
//				int nPoint2;	�t�H���g�̃T�C�Y(Y)
//				LPSTR Message;	������
//		Return:
//============================================================================
void TPrinter::CenterText( int x, int y, int endX,int nFont, int nBold,int nPoint1, int nPoint2, LPSTR Message )
{
	HFONT	holdf,hnewf;
	HDC		hdc;
	int		sx,sy,ex;
	RECT rect;

	TEXTMETRIC	textmetric;

	hdc = hdcPrint;


	sx = ConvertMmToPixcel( x , 0 );	// �󎚂w���W
	ex = ConvertMmToPixcel( endX , 0 );	// �󎚂w���W
	sy = ConvertMmToPixcel( y , 1 );	// �󎚂x���W

	GetTextMetrics(hdc,&textmetric);

	if( nPoint2 == 0 ) nPoint2 = nPoint1;

	nPoint1 = ConvertMmToPixcel( nPoint1 , 0 );
	nPoint2 = ConvertMmToPixcel( nPoint2 , 1 );

	SetRect(&rect	,x
					,sy
					,ex
					,sy+nPoint2);

	if( nFont != 0 && nFont <= 8 ){		// �t�H���g�̌���
		hnewf = CreateFont(	nPoint2,nPoint1,				// �t�H���g�T�C�Y
							0,					// Escapement
							0,					// Orientation
							nBold,				// Weight(�t�H���g�̑���)
							0,					// Ital(�C�^���b�N��)
							0,					// Underline(����)
							0,					// StrikeThru(�ł�������)
							SHIFTJIS_CHARSET,	// Charset(�����Z�b�g)
							OUT_DEFAULT_PRECIS,	// Precision(�o�͐��x)
							CLIP_DEFAULT_PRECIS,// ClipPrecision(�N���b�s���O���x)
							DEFAULT_QUALITY,	// Quality(�t�H���g�̏o�͕i��)
							DEFAULT_PITCH | FF_DONTCARE,	// Pitch(�t�H���g�̃s�b�`�ƃt�@�~��)
//							DEFAULT_PITCH | FF_ROMAN,	// Pitch(�t�H���g�̃s�b�`�ƃt�@�~��)
							FontName[nFont] );	// FontName
		holdf = SelectObject( hdc , hnewf );	// �t�H���g�̐؂�ւ�

//		TextOut(hdc,sx,sy,Message,strlen(Message));
		DrawText(hdc, Message,-1,&rect,DT_CENTER|DT_SINGLELINE);

		hnewf = SelectObject( hdc , holdf );
		DeleteObject( hnewf );

	}else{
		DrawText(hdc, Message,-1,&rect,DT_CENTER|DT_SINGLELINE);
	}

}

//=======================================================================================================================================
// void TPrinter::PrMessage( int x, int y, int nFont, int nBold,int nPoint1, int nPoint2, COLORREF colFont,COLORREF colBack,LPSTR Message )
//	public
//	�O�w�i�F���w�肵���������
//	����:		int x;			�w���W
//				int y;			�x���W
//				int nFont;		�t�H���g�̔ԍ��f�t�H���g�͂O
//				int nBold:		���̑����iCreateFont�̒萔�j
//				int nPoint1;	�t�H���g�̃T�C�Y(X)
//				int nPoint2;	�t�H���g�̃T�C�Y(Y)
//				COLORREF colFont
//				COLORREF colBack
//				LPSTR Message;	������
//=======================================================================================================================================
void TPrinter::PrMessage( int x, int y, int nFont, int nBold,int nPoint1, int nPoint2, COLORREF colFont,COLORREF colBack,LPSTR Message )
{
	COLORREF oldFontCol,oldBackCol;

	// �t�H���g�F
	oldFontCol = SetTextColor( hdcPrint,colFont);

	// �w�i�F
	oldBackCol = SetBkColor(hdcPrint,colBack );

	PrMessage( x, y, nFont, nBold,nPoint1, nPoint2, Message );

	// �t�H���g�F
	SetTextColor( hdcPrint,oldFontCol);

	// �w�i�F
	SetBkColor(hdcPrint,oldBackCol );
}

//============================================================================
//			�T  �v : �o�[�R�[�h���
//	Input:	int x;			�������w���W(0.1mm)
//			int y;			�������x���W(0.1mm)
//			int nWidth;		�������o�[�R�[�h�̕�(��ԍׂ������h�b�g�P�ʂŎw������)
//			int nHeight;	�������o�[�R�[�h�̍���(0.1mm) �����̕����͊܂܂Ȃ�
//			int strON		������������Ȃ��i�O�ȊO���������j
//			LPSTR Message;	������
//	Return:
//============================================================================
void TPrinter::PrBCode( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message )
{

	HDC	hdc;
	char	mojiBarcode[MAX_BARCODE+1];		// �o�[�R�[�h�̉��ɕ\�����镶����
	char	dspBarcode[MAX_BARCODE];		// BarcodeTbl[no]�ɕϊ�����������
	int i;

	int		mojisuu;					// ������
	int		sx,ox1,ox2;					// �������w���W(�J�n�A���Έʒu)
	int		sy,ey;						// �������x���W(�J�n�A�I��)
	int		l;							// �o�[�R�[�h�̃��C��No.
	double		bigBar,smallBar;			// �o�[�R�[�h�̕�

	// �h��Ԃ�
	RECT rect;

	// ����f�[�^�����݂��邩�H
	hdc = hdcPrint;

	// ������`�F�b�N
	memset( dspBarcode , 0 , sizeof( dspBarcode ));
	mojisuu = strlen(Message);
	if( mojisuu > MAX_BARCODE-2 )mojisuu = MAX_BARCODE-2;	// ���[*���t�����邽��


	// ������镶���̗��[�� '*' ��t������
	memset( mojiBarcode , 0 , sizeof( mojiBarcode ));
	mojiBarcode[0]='*';
	memcpy( &mojiBarcode[1] , Message , mojisuu );
	mojiBarcode[mojisuu+1] = '*';

	// �o�[�R�[�h�\�̕��тɕϊ�����
	for( i = 0 ; i < mojisuu+2 ; i++ ){
		dspBarcode[i] = BarcodeConvTbl[mojiBarcode[i]];
	}

	// �g������(DEBUG�p)
	//	Rectangle( hdc , x-5 , y-5 , x+nWidth*((mojisuu)*13-1)+5 , y+nHeight+5);

//	sx = x;
	sx = ConvertMmToPixcel( x , 0 );	// �J�n�w���W
	sy = ConvertMmToPixcel( y , 1 );	// �J�n�x���W
	ey = ConvertMmToPixcel((y+nHeight),1);	// �I���x���W



	smallBar = ConvertMmToPixcel(nWidth,0);
	bigBar = 2*smallBar;
//	bigBar = 2.5*smallBar;




	// �o�[�����������邽�߂ɂQ�h�b�g�ȏ�̃o�[�͕ʏ���
	if( smallBar == 1 ){
	
		ox1 = 0;						// ���������n�߂�ʒu
		for( i = 0 ; i < mojisuu+2 ; i++ ){	// ����
			for( l = 0 ; l < 10 ; l++ ){		// ���C��
				// ���������I���ʒu���Z�o����
				ox2 = ox1 + (int)(BarcodeTbl[dspBarcode[i]][l] ? bigBar : smallBar);
				if( l % 2 == 0 ){			// �����o�[
					SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
					FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
				}
				ox1 = ox2;		// ���̊J�n�ʒu
			}
		}
	}else{
	
		ox1 = 0;						// ���������n�߂�ʒu
		for( i = 0 ; i < mojisuu+2 ; i++ ){	// ����
			for( l = 0 ; l < 10 ; l++ ){		// ���C��
				// ���������I���ʒu���Z�o����
				ox2 = ox1 + (int)(BarcodeTbl[dspBarcode[i]][l] ? bigBar : smallBar);
				if( l % 2 == 0 ){			// �����o�[
					SetRect( &rect,sx+ox1,sy,sx+ox2-1,ey );
					FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
				}
				ox1 = ox2;		// ���̊J�n�ʒu
			}
		}
	}



	// �o�[�R�[�h�̉��ɕ�����\������
	if(strON)	PrBCodeString( x , y+nHeight+1 , sx+ox2 , mojiBarcode );


}



// �݊����̈�
void TPrinter::PrBCode( int x, int y, int nWidth, int nHeight,LPSTR Message )
{
	PrBCode( x, y, nWidth, nHeight,1 , Message );
}

//================================================================================================
//	void TPrinter::PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code)
//			�T  �v : code128�o�[�R�[�h���(�R�[�h�Œ�)
//	Input:	int x;			�������w���W(0.1mm)
//			int y;			�������x���W(0.1mm)
//			int nWidth;		�������o�[�R�[�h�̕�(��ԍׂ������h�b�g�P�ʂŎw������)
//			int nHeight;	�������o�[�R�[�h�̍���(0.1mm) �����̕����͊܂܂Ȃ�
//			int strON		������������Ȃ��i�O�ȊO���������j
//			LPSTR Message;	������
//			int code		����Ɏg���R�[�h 0=A,1=B,2=C
//================================================================================================
void TPrinter::PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code)
{
	HDC	hdc;
	char mojiBarcode[MAX_BARCODE+1];		// �o�[�R�[�h�̉��ɕ\�����镶����
	char dspBarcode[MAX_BARCODE];		// BarcodeTbl[no]�ɕϊ�����������
	int i,j,mojisuu;
	int	sx,ox1,ox2;					// �������w���W(�J�n�A���Έʒu)
	int	sy,ey;						// �������x���W(�J�n�A�I��)
	int	chkD;						// code128�������޼ޯ�
	int barWidth;

	// �����`�F�b�N
	if(code < 0 || code > 2 ) code=1;	// �����codeB�ɂ���

	// �h��Ԃ�
	RECT rect;

	// ����f�[�^�����݂��邩�H
	hdc = hdcPrint;

	// ������`�F�b�N
	memset( dspBarcode , 0 , sizeof( dspBarcode ));
	memset( mojiBarcode , 0 , sizeof( mojiBarcode ));
	mojisuu = strlen(Message);
	if( mojisuu > MAX_BARCODE)mojisuu = MAX_BARCODE;
	memcpy( mojiBarcode, Message , mojisuu );

	// �o�[�R�[�h�\�̕��тɕϊ�����
	for( i = 0 ; i < mojisuu ; i++ ){
		switch(code){
			case 2:		// code A
				if(mojiBarcode[i] > 0 )
					dspBarcode[i] = mojiBarcode[i];
				break;

			case 0:
				dspBarcode[i] = BarcodeConvTbl128A[mojiBarcode[i]];
				break;
			default:
				dspBarcode[i] = BarcodeConvTbl128B[mojiBarcode[i]];
				break;
		}
	}

	sx = ConvertMmToPixcel( x , 0 );	// �J�n�w���W
	sy = ConvertMmToPixcel( y , 1 );	// �J�n�x���W
	ey = ConvertMmToPixcel((y+nHeight),1);	// �I���x���W
	barWidth =  ConvertMmToPixcel( nWidth , 0 );	// �o�[�̕�

	ox1 = 0;						// ���������n�߂�ʒu
	
	// �X�^�[�g�R�[�h
	for( i=0; i<6; i++ ){ 
		if( i % 2 == 0 ){	// �����o�[
			ox2=ox1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
		}
		else{				// �����o�[
			ox2=ox1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
		}
		ox1=ox2;
	}
	// �f�[�^
	for( i=0; i<mojisuu; i++ ){

		for( j=0; j<6; j++ ){ 
			if( j % 2 == 0 ){	// �����o�[
				ox2=ox1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
				SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
				FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
			}
			else{				// �����o�[
				ox2=ox1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
			}
			ox1=ox2;
		}
	}

	// �����޼ޯ�
	chkD=0;
	for( i=0; i<mojisuu; i++ ){
		chkD+=( dspBarcode[i]*(i+1) );
	}
	chkD=(chkD+STARTCODE_128+code)%103;
	for( j=0; j<6; j++ ){ 
		if( j % 2 == 0 ){	// �����o�[
			ox2=ox1+(BarcodeTbl128[chkD][j]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
		}
		else{				// �����o�[
			ox2=ox1+(BarcodeTbl128[chkD][j]*barWidth);
		}
		ox1=ox2;
	}
	
	// stop�R�[�h
	for( j=0; j<7; j++ ){ 
		if( j % 2 == 0 ){	// �����o�[
			ox2=ox1+(BarcodeTblStop[j]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,GetStockObject(BLACK_BRUSH) );
		}
		else{				// �����o�[
			ox2=ox1+(BarcodeTblStop[j]*barWidth);
		}
		ox1=ox2;
	}

	// �o�[�R�[�h�̉��ɕ�����\������
	if(strON)	
		PrBCodeString( x , y+nHeight+1 , sx+ox2 , mojiBarcode );


}

//============================================================================
//			�T  �v : �o�[�R�[�h�̕������
//	Input:	int x;			�������w���W(0.1mm)
//			int y;			�������x���W(0.1mm)
//			int endX;		����I������X���W(Pixcel)
//			LPSTR Message;	������
//	Return:
//============================================================================
void	TPrinter::PrBCodeString( int x, int y, int endX, LPSTR Message )
{
	int i;
	int mojisuu;
	char	moji[4];
	int	 endXmm,txtSpace;

	memset( moji , 0 , sizeof( moji ));
	mojisuu = strlen( Message );

	// �����Ԃ̌v�Z
	// �I�����W��mm�ɕϊ�
	endXmm = endX * 10 * PrinterSizeX / PrinterResX;
	txtSpace = (endXmm-x-12) / (mojisuu-1);	// 12�������̂̓��X�g�P�������̕�

	for( i = 0 ; i < mojisuu ; i++ ){
		moji[0] = Message[i];
		PrMessage( x+i*txtSpace , y , 1 ,FW_NORMAL, 12 , 30 , moji );
	}
}

//==================================================================================================
//	void TPrinter::DrawCircle( int x,int y,int r, BOOL paint)
//	public
//	�^�~�̕`��
//	����:	int sx;			���S�w���W(0.1mm)
//			int sy;			���S�x���W(0.1mm)
//			int r;			���a(0.1mm)
//			BOOL paint;		�h��Ԃ�����=TRUE
//==================================================================================================
void TPrinter::DrawCircle( int x,int y,int r, BOOL paint)
{
	int px,py,rx,ry;

	px = ConvertMmToPixcel( x , 0 );	// �J�n�w���W
	rx = ConvertMmToPixcel( r , 0 );	// �J�n�w���W

	py = ConvertMmToPixcel( y , 1 );	// �J�n�w���W
	ry = ConvertMmToPixcel( r , 1 );	// �J�n�w���W

	HPEN	hOldpen,hPen;				// Handle of pen
	HBRUSH hbrush,hold;

	hPen   = CreatePen( PS_SOLID ,3, RGB(0,0,0));
	hOldpen   = (HPEN)SelectObject(hdcPrint,hPen);
	hbrush = CreateSolidBrush(RGB(0,0,0));
	hold   = (HPEN)SelectObject(hdcPrint,hbrush);

	// �h��Ԃ�
	if( paint ){
		Ellipse( hdcPrint , px-rx ,py-ry,px+rx,py+ry);
	}
	// ����́�
	else{
		Arc( hdcPrint , px-rx ,py-ry,px+rx,py+ry,px,py-ry,px,py-ry);
	}
	hPen   = (HPEN)SelectObject(hdcPrint,hOldpen);
	hbrush   = (HPEN)SelectObject(hdcPrint,hold);
	DeleteObject( hPen );
	DeleteObject( hbrush );
}
//============================================================================
//			�T  �v : �~�܂��͑ȉ~��`��
//	Input:	int x;			�������w���W(0.1mm)
//			int y;			�������x���W(0.1mm)
//			int rx;			x�����a
//			int ry;			y�����a
//			int LineType ���̎��
//					0:����
//					1:�j��
//					2:�_��
//			int LineType ���̂͂΁i���H�w�莞�̂ݗL���j
//	Return:
//============================================================================
void TPrinter::DrawCircle( int x, int y ,int rx ,int ry , int LineType ,int width)
{
	HDC	hdc;
	HPEN	hPen,hOldPen;				// Handle of pen
	int theStyle,theWidth;
	int sx,sy;
	int ex,ey;

	// ����f�[�^�����݂��邩�H
	hdc = hdcPrint;

	sx = ConvertMmToPixcel( x-rx , 0 );	// �󎚂w���W
	sy = ConvertMmToPixcel( y-ry , 1 );	// �󎚂x���W
	ex = ConvertMmToPixcel( x+rx , 0 );	// �󎚂w���W
	ey = ConvertMmToPixcel( y+ry , 1 );	// �󎚂x���W	

	switch( LineType ){
		case 1:						// �j��
			theStyle = PS_DASH;
			theWidth = 1;
			break;
		case 2:						// �_��
			theStyle = PS_DOT;
			theWidth = 1;
			break;

		default:
			theStyle = PS_SOLID;
			theWidth = width;
			break;
	}

	hPen   = CreatePen( theStyle ,width, RGB(0,0,0));
	hOldPen   = (HPEN)SelectObject(hdc,hPen);

	// �ȉ~��`��
	Arc( hdc , sx , sy , ex , ey, sx+(ex-sx)/2 , sy+(ey-sy)/2 , sx+(ex-sx)/2, sy+(ey-sy)/2);

	SelectObject(hdc,hOldPen);

	DeleteObject( hPen );

}

//============================================================================
//	��������
//	Input:	int sx;			�������w���W(0.1mm)
//			int sy;			�������x���W(0.1mm)
//			int ex;
//			int ey;
//			int LineType ���̎��
//					0:����
//					1:�j��
//					2:�_��
//			int LineType ���̂͂΁i���H�w�莞�̂ݗL���j
//	Return:
//============================================================================
void TPrinter::DrawLine( int sx,int sy,int ex, int ey,int LineType ,int LineWidth)
{
	HDC	hdc;
	HPEN	hOldpen,hPen;				// Handle of pen
	int theStyle,theWidth;
	int x1,x2,y1,y2;

	switch( LineType ){
		case 1:						// �j��
			theStyle = PS_DASH;
			theWidth = 1;
			break;
		case 2:						// �_��
			theStyle = PS_DOT;
			theWidth = 1;
			break;

		default:
			theStyle = PS_SOLID;
			theWidth = LineWidth;
			break;
	}


	// ����f�[�^�����݂��邩�H
	hdc = hdcPrint;
	x1 = ConvertMmToPixcel( sx , 0 );	// �J�n�w���W
	x2 = ConvertMmToPixcel( ex , 0 );	// �J�n�w���W
	y1 = ConvertMmToPixcel( sy , 1 );	// �J�n�x���W
	y2 = ConvertMmToPixcel(ey,1);		// �I���x���W

	hPen   = CreatePen( theStyle ,theWidth, RGB(0,0,0));
	hOldpen   = (HPEN)SelectObject(hdc,hPen);

	// ��������
	MoveToEx( hdc , x1 , y1 , NULL );
	LineTo(   hdc , x2 , y2 );


	// GDI���ǂ�
	hPen   = (HPEN)SelectObject(hdc,hOldpen);
	DeleteObject( hPen );
}

//==================================================================================================
//	void TPrinter::DrawRect( int sx, int sy, int ex, int ey,COLORREF theColor)
//	public
//	�w��͈͓��̓h��Ԃ�
//	Input:	int sx;			����w���W(0.1mm)
//			int sy;			����x���W(0.1mm)
//			int ex;			�E��X
//			int ey	;		�E��Y
//			COLORREF theColor	�h��Ԃ��F
//==================================================================================================
void TPrinter::DrawRect( int sx, int sy, int ex, int ey,COLORREF theColor)
{

	RECT rect;	// �h��Ԃ��̈�

	HBRUSH hBrush,hOldBrush;

	SetRect( &rect,ConvertMmToPixcel(sx,0),ConvertMmToPixcel(sy,1),
				   ConvertMmToPixcel(ex,0),ConvertMmToPixcel(ey,1 ));

	// �u���V�쐬
	hBrush = CreateSolidBrush( theColor);

	hOldBrush = SelectObject( hdcPrint , hBrush );
	FillRect( hdcPrint,&rect,hBrush );

	// GDI�߂�
	hBrush = SelectObject( hdcPrint , hOldBrush );
	DeleteObject( hBrush );
}

//============================================================================
//	�T�v :	�w��̍��W(����)���v�����^�[�̍��W(�s�N�Z��)�ɕϊ�����
//			�u�o�|�U�O�O���g�p����ۂɎg�p����
//	
//	Input : int mm;			�w��̍��W	�O�D�P�������P�Ƃ���
//			int xy;			���W�n		�O�F�w���W
//										�P�F�x���W
//	Return: �v�Z����
//	
//============================================================================
int	TPrinter::ConvertMmToPixcel( int mm , int xy )
{
	double val;

	if( xy == 0 ){			// �w���W�̕ϊ�
		val =  (double)mm * PrinterResX/(PrinterSizeX*10);
//val=mm;
	}else{					// �x���W�̕ϊ�
		val =  (double)mm * PrinterResY/(PrinterSizeY*10);

	}


	return (int)val;
}




// �f�t�H���g�v�����^�Ɋւ�����̎擾
int TPrinter::GetDefaultPrinter(char *theDrv, char *theDev, char *thePort)
{
	static PAGESETUPDLG psd;		// �y�|�W�ݒ�_�C�A���O�������\����
	DEVNAMES *dvn; 					// �v�����^���\����
	int size;
	char lpDev[256];
	char lpDrv[256];
	char lpPort[256];

	// PAGESETUP�\���̂̏�����
	memset( &psd,0, sizeof(psd));
	psd.Flags = PSD_RETURNDEFAULT;		// �_�C�A���O���I�[�v�������Ƀf�t�H���g�v�����^���擾
 	psd.hDevNames=NULL;
	psd.hDevMode=NULL;
	psd.lStructSize=sizeof(PAGESETUPDLG);
	psd.hwndOwner=NULL;

	// �_�C�A���O���J���ăf�o�C�X�E�|�[�g�E�h���C�o���擾
	if(!PageSetupDlg( &psd )) return(-1);

	// DEVNAME�\���̂̃��������b�N�����蓖��
	dvn=(DEVNAMES*)GlobalLock(psd.hDevNames);
	size=dvn->wDeviceOffset;

	// �e��f�[�^���擾����
	memset(lpDev,0,sizeof(lpDev) );
	strncpy( lpDev,(char*)dvn+dvn->wDeviceOffset,sizeof(lpDev) );

	memset(lpDrv,0,sizeof(lpDrv) );
	strncpy( lpDrv,(char*)dvn+dvn->wDriverOffset, sizeof(lpDrv) );

	memset(lpPort,0,sizeof(lpPort) );
	strncpy( lpPort,(char*)dvn+dvn->wOutputOffset,sizeof(lpPort) );

	// ���������
	GlobalFree(psd.hDevMode);
	GlobalUnlock(psd.hDevNames);
	GlobalFree(psd.hDevNames);
	GlobalFree(psd.hPageSetupTemplate);
	GlobalUnlock(dvn);
	GlobalFree(dvn);

	// �o��
	strncpy(theDrv,lpDrv,256);
	strncpy(theDev,lpDev,256);
	strncpy(thePort,lpPort,256);
	return(0);

}





//===== End of Text =====
