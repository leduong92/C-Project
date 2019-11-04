//===========================================================================
//
//  	�v�����^�[�̐����{�N���X
//	
//		FileName : TPrintQR.cpp
//		�m�n�j������� �Z�p�{�� ���Y�Z�p��
//		�쐬���F2004.09.27 by ����
//		�C�����F2004.11.09		 �J�^�J�i�󎚂ɑΉ�
//===========================================================================
#include	"Tprinter.h"
#include 	"QR2DLL.H"

#define MAX_TEXT_LENGTH 7089
#define MAX_VER 7

static const short QR_VER_TBL[]={
	0,		// Ver0:�ق�Ƃ͖������ǌv�Z�̓s����
	34,		// Ver1:34byte
	63,		// Ver2:63byte
	91,		// Ver3:91byte
	149,	// Ver4:149byte
	202,	// Ver5:202yte
	255,	// Ver6:255yte
};

//=============================================================================
//	int	TPrinter::PrintQRCode(char *txt)
//	�����P�Fchar *txt	�󎚕���(�ő啶����256)
//	�����Q�Flong version	QR�o�[�W����
//	�����R�Flong lngErrorLevel	���������x�� L(7%),M(15%),Q(25%),H(30%)
//	�����S�Flong lngPrintQlty	����T�C�Y�i0:0.16mm,1:0.19mm,2:0.25mm,3:0.32mm�j
//	�����T�F�w���W
//	�����U�F�x���W
//		QR�R�[�h���������
//	Caution!! 2�o�C�g�����ɑΉ����Ă��܂���!�I ����
//=============================================================================
int TPrinter::PrintQRCode(char *txt,long lngErrorLevel,long lngPrintQlty ,int x ,int y)
{
	char strDummy[MAX_TEXT_LENGTH];		// ���f�[�^
    long lngLoopDummy;		// �J��Ԃ������p�̕ϐ�
    long lngReturn;			// �֐��̖߂�l�p�ϐ�
    long lngSell;			// �p�q�o�[�W�����Ɍĉ������ӂ̃Z����
    long lngOriginalLength;	// ���f�[�^�̕�����
 	long lngResult[MAX_TEXT_LENGTH*4+20];	// ���ʗp�̔z�� ( �����f�[�^�̂S�{�{17�͍Œ�K�v )
	long lngOriginal[MAX_TEXT_LENGTH];		// ���f�[�^�p�̔z��
	long lngX,lngY;		// �\���p�w��,�\���p�x��
    double dblPitch;	// �\���p�Z��������̒���
	int ver;
	long lngQRversion=0;

	int   sx,sy;

    // ���f�[�^�̎擾
	memset(strDummy,0,sizeof(strDummy));
	memcpy(strDummy,txt,strlen(txt)>sizeof(strDummy) ? sizeof(strDummy) : strlen(txt));

	// ����T�C�Y
	if( lngPrintQlty < 0 || lngPrintQlty > 3) 	lngPrintQlty = 2;

    // ���������x���̎擾
	if(lngErrorLevel < 0 || lngErrorLevel > 3)	lngErrorLevel = 2;	// L(7%),M(15%),Q(25%),H(30%)

    
    // ���ʗp�z��̍Đ錾
	memset(lngResult,0,sizeof(lngResult));

    // �I���W�i���f�[�^�̕�����
    lngOriginalLength = strlen(strDummy);
	//QR�͍ő�7089�����܂�
	if(lngOriginalLength < 0 || lngOriginalLength > MAX_TEXT_LENGTH) return -1;	//Length �I�[�o�[
    
	//�o�[�W���������肷��
	for(ver=0;ver < MAX_VER;ver++){
		if(lngOriginalLength < QR_VER_TBL[ver]) break;
	}
	if(!ver) return -2;		//Ver�U(255�����ȏ�)�ȏ�̃e�[�u��������Ă܂���B����
	lngQRversion = ver;
    // ��ӂ̃Z����
    lngSell = lngQRversion * 4 + 17;

	// �I���W�i���f�[�^�p�̔z��̍Đ錾
	memset(lngOriginal,0,sizeof(lngOriginal));
    
	//2�o�C�g�����ɑΉ����Ă��܂���!�I ����
	for( lngLoopDummy = 0;lngLoopDummy < lngOriginalLength; lngLoopDummy++){
		memcpy(&lngOriginal[lngLoopDummy],&strDummy[lngLoopDummy],1);	//�J�^�J�i�ɑΉ������邽�߂�����ɕύX 2004.11.09 ����
//		lngOriginal[lngLoopDummy] = strDummy[lngLoopDummy];
	}

	// Ver1:34byte
	// Ver2:63byte
	// Ver3:91byte
	// Ver4:149byte
	// Ver5:202yte
	// Ver6:255yte
    // QRVer�̊m�FlngReturn��0�ɂȂ�Ver�܂ŏグ�Ă��� Ver��40�܂�(�ް�ޮ݂ƕ������̊֌W���悭�킩��Ȃ�����)
//	while(lngQRversion <= 40){
	
	lngReturn = QRmodel2(&lngOriginal[0], lngOriginalLength, lngQRversion, lngErrorLevel, &lngResult[0]);
	//�_����������Ver�������Ă�����x�g���C
	if(lngReturn){
		lngQRversion += 1;
	    lngSell = lngQRversion * 4 + 17;
		memset(lngResult,0,sizeof(lngResult));
		lngReturn = QRmodel2(&lngOriginal[0], lngOriginalLength, lngQRversion, lngErrorLevel, &lngResult[0]);
		if(lngReturn)	return -3;			//DLL�o�[�W�����s�K���G���[
	}

	// ����p�i��ӂ̃Z�����ƃX�^�[�g�ʒu�ƂP�Z��������̑傫���j�f�[�^���Z
	switch(lngPrintQlty){
		case 0:	dblPitch = 0.16 * 56.7;	break;
		case 1:	dblPitch = 0.19 * 56.7;	break;
		case 2:	dblPitch = 0.25 * 56.7;	break;
		case 3:	dblPitch = 0.32 * 56.7;	break;		
		default:	dblPitch = 0.25 * 56.7;	break;	
	}

	if(lngReturn) return -4;

	HDC	hdc;
	// �h��Ԃ�
	RECT rect;
	
	hdc = hdcPrint;

	//�ʒupixel�ϊ�
	sx = ConvertMmToPixcel(x,0);
	sy = ConvertMmToPixcel(y,1);

	//lngResult��Rect�ɓn��
	if(lngReturn==0){
		for( lngY = 0;lngY <= lngSell-1;lngY++){
			for( lngX = 0 ; lngX <= lngSell - 1; lngX++){
				if( lngResult[lngX * lngSell + lngY] >= 1 ){
					
					SetRect( &rect,	sx + (int)(lngX * dblPitch),			sy + (int)(lngY * dblPitch),
									sx + (int)(lngX * dblPitch+dblPitch),	sy + (int)(lngY * dblPitch+dblPitch) );

					FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
				}
			}
		}				
	}
	
	return 0;
}
//=============================================================================
//	int	TPrinter::PrintQRCode(	char *txt,long lngQRversion,long lngErrorLevel, long lngPrintQlty, int	 x,	int  y)
//	�݊����̂���
//=============================================================================
int TPrinter::PrintQRCode(	char *txt,long lngQRversion,long lngErrorLevel, long lngPrintQlty, int	 x,	int  y)
{
	return (PrintQRCode(txt, lngErrorLevel,lngPrintQlty, x, y));
}

