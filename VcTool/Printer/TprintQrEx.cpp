//===========================================================================
//
//  	プリンターの制御基本クラス
//	
//		FileName : TPrintQR.cpp
//		ＮＯＫ株式会社 技術本部 生産技術部
//		作成日：2004.09.27 by ﾏﾂﾀﾞ
//		修正日：2004.11.09		 カタカナ印字に対応
//              2006. 7. 7 QRmodel2の呼び出し処理変更
//===========================================================================
#include	"Tprinter.h"
#include "QR_Encode.h"

#include <stdio.h>


//=============================================================================
//	int TPrinter::PrintQREx(char *txt,long lngErrorLevel,int pixPitch ,int x ,int y)
//	引数１：char *txt	印字文字(最大文字数4096)
//	引数２：long lngErrorLevel	誤り訂正レベル L(7%),M(15%),Q(25%),H(30%)
//	引数３：int pixPitch 最小ピクセル数(Exで違う箇所)
//	引数４：Ｘ座標
//	引数５：Ｙ座標
//		QRコードを印刷する
//	Caution!! 2バイト文字対応
//=============================================================================
int TPrinter::PrintQR(char *txt,long lngErrorLevel,int pixPitch ,int x ,int y)
{
	int i,j;
	BOOL ret;
	CQR_Encode* QrTool;
	QrTool = new CQR_Encode;

	if( !strlen(txt) ) return -1;

	if( lngErrorLevel < 0 || lngErrorLevel > 3 ) lngErrorLevel=1;

	//QRエンコード実行
	ret = QrTool->EncodeData( lngErrorLevel,0,TRUE,-1,txt);
	if(ret==FALSE) return -2;

	int   sx,sy;
	HDC	hdc;
	// 塗りつぶし
	RECT rect;
	
	hdc = hdcPrint;

	//位置pixel変換
	sx = ConvertMmToPixcel(x,0);
	sy = ConvertMmToPixcel(y,1);

	if( pixPitch < 2 ) pixPitch=2;

	// ドット描画
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
