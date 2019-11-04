#ifndef TPrinter_H
#define TPrinter_H

//===============================================================================================
//
//		プリンター制御基本クラス(改造版)
//
//		FileName : TPrinter.h
//		ＮＯＫ株式会社 技術本部 生産技術部
//		作成日：1996.10.9 by 前田
//		修正日：1998.2.27 by Koba	
//					スリム化 
//		修正日：1998.9.16 by Coba
//					マッピングモード依存をなくした
//		修正日：1998.12.22 by Coba
//					用紙サイズ指定機能の追加
//		修正日：2000.06.02 by TAKE
//					円または楕円の描画を追加 DrawCircle
//		修正日：2002.06.04 by coba
//					フォント指定の文字出力機能を追加
//		修正日：2002.06.11 by coba
//					フォント指定の文字出力関数にキャラクタセット指定機能
//		修正日：2002.07.25 by coba
//					「プリンタの設定ダイアログ」の内容をそのまま反映する関数を追加
//					これにより、ドライブ・デバイス・ポートの指定が不要に。
//		修正日：2003.07.08 by coba
//					InitPageSetting()で、"ﾕｰｻﾞ用紙"を明示的に使用するか指定できるように変更
//					"ﾕｰｻﾞ用紙"を持たないプリンタで"ﾕｰｻﾞ指定"を使うと予期せぬサイズで作成されて
//					しまうのを防止する。VP系プリンタドライバ対策
//		修正日：2003.11.17 by coba
//					ハッチング塗りつぶし長方形の描画関数DrawHatchBox()を追加
//		修正日：2004.04.28 by TAKE	InitPageSettingEx TAKE トレイのコントロールを追加
//		修正日：2004.09.27 by ﾏﾂﾀﾞ	QRCODE印刷機能の追加
//									QRCODEを使用するときは、TPrintQR.CPPを追加する
//		修正日：2004.10.05 by coba
//					PrGaiji()でBitMapデータ数が違っていたバグの修正//
//		修正日：2004.10.28 by coba
//					多言語対応のCenterText()を追加
//		修正日：2004.12.08 by ﾏﾂﾀﾞ
//					DrawR() と DrawBitmap()を追加
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
	public :					// どの関数からもアクセス可能==================
		HDC	hdcPrint;						// 印刷用デバイスコンテキスト

		// デフォルトコンストラクタ（従来との互換性のため）
		TPrinter(void);

		//	コンストラクター(デフォルト用紙使用版)
		TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat);

		//	コンストラクター(用紙サイズ指定版)
		TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth, int *initStat );

		//	コンストラクター(デフォルトプリンタ及び用紙使用版)
		TPrinter(int *initStat);

		~TPrinter(void);					//	デストラクター
		//	コンストラクター(デフォルトプリンタ及び用紙使用版)
		TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat,short pageSize,short orient);

		int OpenPrintDialog(HWND hwnd);

		// プリンタ詳細情報設定
		int IPrinter(void);

		virtual	int	Print(void);					// 印刷実行


												// 文字印刷
		void PrMessage(	int	x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						int	nFont,			// フォントの番号(登録済み)
						int nBold,			// フォント太さ(VC定数)
						int	nPoint1,		// 文字のポイント数(X)(0.1mm)
						int	nPoint2,		// 文字のポイント数(Y)(0.1mm)
						LPSTR Message );	// 文字印刷列

		void CenterText(int	x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						int	endX,			// 終了座標(0.1mm)
						int	nFont,			// フォントの番号(登録済み)
						int nBold,			// フォント太さ(VC定数)
						int	nPoint1,		// 文字のポイント数(X)(0.1mm)
						int	nPoint2,		// 文字のポイント数(Y)(0.1mm)
						LPSTR Message );	// 文字印刷列

		void CenterText(int	x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						int	endX,			// 終了座標(0.1mm)
						int	nFont,			// フォントの番号(登録済み)
						int	nPoint1,		// 文字のポイント数(X)(0.1mm)
						int	nPoint2,		// 文字のポイント数(Y)(0.1mm)
						LPSTR Message );	// 文字印刷列

		void CenterTextEx(int	x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						int	endX,			// 終了座標(0.1mm)
						int nBold,			// フォント太さ(VC定数)
						int	nPoint1,		// 文字のポイント数(X)(0.1mm)
						int	nPoint2,		// 文字のポイント数(Y)(0.1mm)
						char* fontname,		// フォントの名前
						DWORD charset,		// キャラクタセット
						LPSTR Message );	// 文字印刷列

		// 前景／背景色を指定した文字列描画
		void PrMessage(	int	x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						int	nFont,			// フォントの番号(登録済み)
						int nBold,			// フォント太さ(VC定数)
						int	nPoint1,		// 文字のポイント数(X)(0.1mm)
						int	nPoint2,		// 文字のポイント数(Y)(0.1mm)
						COLORREF colFont,
						COLORREF colBack,
						LPSTR Message );	// 文字印刷列

		void PrMsgEx(	int	x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						char* fontname,		// フォントの名前
						DWORD charset,		// キャラクタセット
						int nBold,			// フォント太さ(VC定数)
						int	nPoint1,		// 文字のポイント数(X)(0.1mm)
						int	nPoint2,		// 文字のポイント数(Y)(0.1mm)
						LPSTR Message );	// 文字印刷列

		void PrMsgEx(	int	x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						LPSTR Message,		// 文字印刷列
						// 以下はCreateFontの引数。但し、nHeightとnWidthは0.1mm単位の指定です
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


		// 互換性のため
		void PrMessage(	int	x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						int	nFont,			// フォントの番号(登録済み)
						int	nPoint1,		// 文字のポイント数(X)(0.1mm)
						int	nPoint2,		// 文字のポイント数(Y)(0.1mm)
						LPSTR Message );	// 文字印刷列

		// バーコード印刷
		void PrBCode(	int x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						int nWidth,			// Ｘ幅(0.1mm)(最も細い線を対象とする)
						int nHeight,		// Ｙ幅(0.1mm)
						int strOn,			// 文字印刷ありなし（0=なし、それ以外＝あり）
						LPSTR code);		// 印刷文字列

		// 規格無視の狭いバーコード印刷
		void PrNarrowBCode(	int x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						int nWidth,			// Ｘ幅(0.1mm)(最も細い線を対象とする)
						int nHeight,		// Ｙ幅(0.1mm)
						int strOn,			// 文字印刷ありなし（0=なし、それ以外＝あり）
						LPSTR code);		// 印刷文字列

		// 互換性のため
		void PrBCode(	int x,				// Ｘ座標(0.1mm)
						int	y,				// Ｙ座標(0.1mm)
						int nWidth,			// Ｘ幅(0.1mm)(最も細い線を対象とする)
						int nHeight,		// Ｙ幅(0.1mm)
						LPSTR code);		// 印刷文字列
												// 外字印字

		void PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code);

		int	PageStart(void);				// 
		int	PageEnd(void);	

	// 派生クラスとフレンドのみアクセス可能========
		// 図形描画
		void DrawCircle( int x, int y ,int rx ,int ry , int LineTye ,int width);
		void DrawLine( int sx,int sy,int ex, int ey,int LineType,int LineWidth);
		void DrawLine( int sx,int sy,int ex, int ey,int LineType,int LineWidth,COLORREF color);
		// 2003.11.17 こば追加
		void DrawHatchBox(int sx,int sy,int ex, int ey,int LineType,int LineWidth,int htachtype);
	
		//Rを描く 2004.12.08 ﾏﾂﾀﾞ追加
		void DrawR( int x, int y ,int r ,int mode , int LineType ,int width);

		//BitMapを描画する 2004.12.08 ﾏﾂﾀﾞ追加
		void DrawBitmap( int x, int y ,HANDLE hResource );
	
	protected :
		int	InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth,BOOL userpaper=FALSE);
		int	InitPageSettingEx(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short thePage, short theCopies);
		// 2004.04.28 TAKE トレイのコントロールを追加
		// short theTray : DMBIN_AUTO:自動　 DMBIN_MANUAL：手差しトレイ
		int	InitPageSettingEx(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short thePage, short theCopies, short tray);
			// 工程進捗用のオプション
			// 検査印
			void PrStump( int x, int y ,int theForm,LPSTR Day ,LPSTR Inspector);
			void PrStumpLaser( int x, int y ,int theForm,LPSTR Day ,LPSTR Inspector);

			// Snem等のマーク
			void PrGaiji(	int x,				// Ｘ座標(0.1mm)
							int y,				// Ｙ座標(0.1mm)
							int nWidth,			// Ｘ幅(0.1mm)
							int nHeight,		// Ｙ幅(0.1mm)
							int	GaijiNo );		// 外字登録No.
												// S = 0
												// N = 1
												// E = 2


		// 下位互換の為のみ
		int IPrinter( LPSTR sPrintName , int nPrinterNo , int PageLength, int dummy );


	// このクラスからのみアクセス可能==============
	private :
		int	flgIPrinter;					// プリンターの初期化フラグ

		unsigned int	PrinterResX,PrinterResY;					// プリンターの印字サイズ(ピクセル数)
		unsigned int	PrinterSizeX,PrinterSizeY;					// プリンターのピクセル数(mm)

													// バーコードの文字印刷
		void PrBCodeString(	int x,				// 
							int y,				// 
							int n,				// 
							LPSTR Message );	// 

		int	ConvertMmToPixcel( int mm , int xy );

		int GetDefaultPrinter(char *theDrv, char *theDev,char *thePort);

		// 下位互換の為のみ
		int GetIni( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName);

//==================================================================================================
//==================================================================================================
//		以下、本体は TPrintQR.cppにあるので、使う場合はTPrinQR.cppを追加し、
//		インポートライブラリ"qr2dll.lib"を使用すること
//==================================================================================================
//==================================================================================================
	public:
		int PrintQRCode(	char *txt,			//char *txt	印字文字(最大文字数256)
							long lngErrorLevel,	//long lngErrorLevel	誤り訂正レベル L(7%),M(15%),Q(25%),H(30%)
							long lngPrintQlty,	//long lngPrintQlty	印刷サイズ（0:0.16mm,1:0.19mm,2:0.25mm,3:0.32mm）
							int	 x,				// Ｘ座標(0.1mm)
							int  y);				// Ｙ座標(0.1mm)

		//互換性のため
		int PrintQRCode(	char *txt,			//char *txt	印字文字(最大文字数256)
							long lngQRversion,	//long version	QRバージョン
							long lngErrorLevel,	//long lngErrorLevel	誤り訂正レベル L(7%),M(15%),Q(25%),H(30%)
							long lngPrintQlty,	//long lngPrintQlty	印刷サイズ（0:0.16mm,1:0.19mm,2:0.25mm,3:0.32mm）
							int	 x,				// Ｘ座標(0.1mm)
							int  y);				// Ｙ座標(0.1mm)


};



#endif

