//===========================================================================
//
//  	�v�����^�[�̐����{�N���X
//	
//		FileName : TPrintQR.cpp
//		�m�n�j������� �Z�p�{�� ���Y�Z�p��
//		�쐬���F2004.09.27 by ����
//		�C�����F2004.11.09		 �J�^�J�i�󎚂ɑΉ�
//              2006. 7. 7 QRmodel2�̌Ăяo�������ύX
//===========================================================================
#include	"Tprinter.h"
#include "QR_Encode.h"

#include <stdio.h>


//=============================================================================
//	int TPrinter::PrintQREx(char *txt,long lngErrorLevel,int pixPitch ,int x ,int y)
//	�����P�Fchar *txt	�󎚕���(�ő啶����4096)
//	�����Q�Flong lngErrorLevel	���������x�� L(7%),M(15%),Q(25%),H(30%)
//	�����R�Fint pixPitch �ŏ��s�N�Z����(Ex�ňႤ�ӏ�)
//	�����S�F�w���W
//	�����T�F�x���W
//		QR�R�[�h���������
//	Caution!! 2�o�C�g�����Ή�
//=============================================================================
int TPrinter::PrintQR(char *txt,long lngErrorLevel,int pixPitch ,int x ,int y)
{
	int i,j;
	BOOL ret;
	CQR_Encode* QrTool;
	QrTool = new CQR_Encode;

	if( !strlen(txt) ) return -1;

	if( lngErrorLevel < 0 || lngErrorLevel > 3 ) lngErrorLevel=1;

	//QR�G���R�[�h���s
	ret = QrTool->EncodeData( lngErrorLevel,0,TRUE,-1,txt);
	if(ret==FALSE) return -2;

	int   sx,sy;
	HDC	hdc;
	// �h��Ԃ�
	RECT rect;
	
	hdc = hdcPrint;

	//�ʒupixel�ϊ�
	sx = ConvertMmToPixcel(x,0);
	sy = ConvertMmToPixcel(y,1);

	if( pixPitch < 2 ) pixPitch=2;

	// �h�b�g�`��
	for (i = 0; i < QrTool->m_nSymbleSize; ++i)
	{
		for (j = 0; j < QrTool->m_nSymbleSize; ++j)
		{
			if (QrTool->m_byModuleData[i][j])
			{
				SetRect( &rect,	sx + (int)(i * pixPitch),			sy + (int)(j * pixPitch),
								sx + (int)(i * pixPitch+pixPitch),	sy + (int)(j * pixPitch+pixPitch));

				FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
			}
		}
	}


	delete QrTool;
	return 0;
}
