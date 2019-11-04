//===========================================================================
//
//  	プリンターの制御基本クラス
//	
//		FileName : TPrintQR.cpp
//		ＮＯＫ株式会社 技術本部 生産技術部
//		作成日：2004.09.27 by ﾏﾂﾀﾞ
//		修正日：2004.11.09		 カタカナ印字に対応
//===========================================================================
#include	"Tprinter.h"
#include 	"QR2DLL.H"

#define MAX_TEXT_LENGTH 7089
#define MAX_VER 7

static const short QR_VER_TBL[]={
	0,		// Ver0:ほんとは無いけど計算の都合上
	34,		// Ver1:34byte
	63,		// Ver2:63byte
	91,		// Ver3:91byte
	149,	// Ver4:149byte
	202,	// Ver5:202yte
	255,	// Ver6:255yte
};

//=============================================================================
//	int	TPrinter::PrintQRCode(char *txt)
//	引数１：char *txt	印字文字(最大文字数256)
//	引数２：long version	QRバージョン
//	引数３：long lngErrorLevel	誤り訂正レベル L(7%),M(15%),Q(25%),H(30%)
//	引数４：long lngPrintQlty	印刷サイズ（0:0.16mm,1:0.19mm,2:0.25mm,3:0.32mm）
//	引数５：Ｘ座標
//	引数６：Ｙ座標
//		QRコードを印刷する
//	Caution!! 2バイト文字に対応していません!！ ﾏﾂﾀﾞ
//=============================================================================
int TPrinter::PrintQRCode(char *txt,long lngErrorLevel,long lngPrintQlty ,int x ,int y)
{
	char strDummy[MAX_TEXT_LENGTH];		// 元データ
    long lngLoopDummy;		// 繰り返し処理用の変数
    long lngReturn;			// 関数の戻り値用変数
    long lngSell;			// ＱＲバージョンに呼応する一辺のセル数
    long lngOriginalLength;	// 元データの文字数
 	long lngResult[MAX_TEXT_LENGTH*4+20];	// 結果用の配列 ( ※元データの４倍＋17は最低必要 )
	long lngOriginal[MAX_TEXT_LENGTH];		// 元データ用の配列
	long lngX,lngY;		// 表示用Ｘ軸,表示用Ｙ軸
    double dblPitch;	// 表示用セルあたりの長さ
	int ver;
	long lngQRversion=0;

	int   sx,sy;

    // 元データの取得
	memset(strDummy,0,sizeof(strDummy));
	memcpy(strDummy,txt,strlen(txt)>sizeof(strDummy) ? sizeof(strDummy) : strlen(txt));

	// 印刷サイズ
	if( lngPrintQlty < 0 || lngPrintQlty > 3) 	lngPrintQlty = 2;

    // 誤り訂正レベルの取得
	if(lngErrorLevel < 0 || lngErrorLevel > 3)	lngErrorLevel = 2;	// L(7%),M(15%),Q(25%),H(30%)

    
    // 結果用配列の再宣言
	memset(lngResult,0,sizeof(lngResult));

    // オリジナルデータの文字数
    lngOriginalLength = strlen(strDummy);
	//QRは最大7089文字まで
	if(lngOriginalLength < 0 || lngOriginalLength > MAX_TEXT_LENGTH) return -1;	//Length オーバー
    
	//バージョンを決定する
	for(ver=0;ver < MAX_VER;ver++){
		if(lngOriginalLength < QR_VER_TBL[ver]) break;
	}
	if(!ver) return -2;		//Ver６(255文字以上)以上のテーブルを作ってません。ﾏﾂﾀﾞ
	lngQRversion = ver;
    // 一辺のセル数
    lngSell = lngQRversion * 4 + 17;

	// オリジナルデータ用の配列の再宣言
	memset(lngOriginal,0,sizeof(lngOriginal));
    
	//2バイト文字に対応していません!！ ﾏﾂﾀﾞ
	for( lngLoopDummy = 0;lngLoopDummy < lngOriginalLength; lngLoopDummy++){
		memcpy(&lngOriginal[lngLoopDummy],&strDummy[lngLoopDummy],1);	//カタカナに対応させるためこちらに変更 2004.11.09 ﾏﾂﾀﾞ
//		lngOriginal[lngLoopDummy] = strDummy[lngLoopDummy];
	}

	// Ver1:34byte
	// Ver2:63byte
	// Ver3:91byte
	// Ver4:149byte
	// Ver5:202yte
	// Ver6:255yte
    // QRVerの確認lngReturnが0になるVerまで上げていく Verは40まで(ﾊﾞｰｼﾞｮﾝと文字数の関係がよくわからないため)
//	while(lngQRversion <= 40){
	
	lngReturn = QRmodel2(&lngOriginal[0], lngOriginalLength, lngQRversion, lngErrorLevel, &lngResult[0]);
	//ダメだったらVerをあげてもう一度トライ
	if(lngReturn){
		lngQRversion += 1;
	    lngSell = lngQRversion * 4 + 17;
		memset(lngResult,0,sizeof(lngResult));
		lngReturn = QRmodel2(&lngOriginal[0], lngOriginalLength, lngQRversion, lngErrorLevel, &lngResult[0]);
		if(lngReturn)	return -3;			//DLLバージョン不適合エラー
	}

	// 印刷用（一辺のセル数とスタート位置と１セルあたりの大きさ）データ演算
	switch(lngPrintQlty){
		case 0:	dblPitch = 0.16 * 56.7;	break;
		case 1:	dblPitch = 0.19 * 56.7;	break;
		case 2:	dblPitch = 0.25 * 56.7;	break;
		case 3:	dblPitch = 0.32 * 56.7;	break;		
		default:	dblPitch = 0.25 * 56.7;	break;	
	}

	if(lngReturn) return -4;

	HDC	hdc;
	// 塗りつぶし
	RECT rect;
	
	hdc = hdcPrint;

	//位置pixel変換
	sx = ConvertMmToPixcel(x,0);
	sy = ConvertMmToPixcel(y,1);

	//lngResultをRectに渡す
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
//	互換性のため
//=============================================================================
int TPrinter::PrintQRCode(	char *txt,long lngQRversion,long lngErrorLevel, long lngPrintQlty, int	 x,	int  y)
{
	return (PrintQRCode(txt, lngErrorLevel,lngPrintQlty, x, y));
}

