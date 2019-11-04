#ifndef TPrinter_H
#define TPrinter_H
//===========================================================================
//
//		プリンター制御基本クラス(ver.2.00)
//
//		FileName : TPrinter.h
//		ＮＯＫ株式会社 技術本部 生産技術部
//		作成日：2000.05.15 coba 分散した過去のソースを合併して作り直し
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

			// プリンタ詳細情報設定
			int IPrinter(void);

			virtual	int	Print(void);					// 印刷実行

			// ページスタート／エンド
			int	PageStart(void);				// 
			int	PageEnd(void);	

			// ドキュメントスタート／エンド
			int	DocStart(void);	
			int	DocEnd(void);	
												// 文字印刷
			//---------------以下、描画系の関数です--------------------------
			// 文字列描画（ノーマル）
			void PrMessage(	int	x,				// Ｘ座標(0.1mm)
							int	y,				// Ｙ座標(0.1mm)
							int	nFont,			// フォントの番号(登録済み)
							int nBold,			// フォント太さ(VC定数)
							int	nPoint1,		// 文字のポイント数(X)(0.1mm)
							int	nPoint2,		// 文字のポイント数(Y)(0.1mm)
							LPSTR Message );	// 文字印刷列

			// センタリング文字列描画
			void CenterText(int	x,				// Ｘ座標(0.1mm)
							int	y,				// Ｙ座標(0.1mm)
							int	endX,			// 終了座標(0.1mm)
							int	nFont,			// フォントの番号(登録済み)
							int nBold,			// フォント太さ(VC定数)
							int	nPoint1,		// 文字のポイント数(X)(0.1mm)
							int	nPoint2,		// 文字のポイント数(Y)(0.1mm)
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

			// 互換性のため
			void PrBCode(	int x,				// Ｘ座標(0.1mm)
							int	y,				// Ｙ座標(0.1mm)
							int nWidth,			// Ｘ幅(0.1mm)(最も細い線を対象とする)
							int nHeight,		// Ｙ幅(0.1mm)
							LPSTR code);		// 印刷文字列
			void PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code);

			// 楕円
			void DrawCircle( int x, int y ,int rx ,int ry , int LineTye ,int width);

			// 真円
			void DrawCircle( int x,int y,int r, BOOL paint);

			// 線
			void DrawLine( int sx,int sy,int ex, int ey,int LineType,int LineWidth);

			// 枠（塗りつぶし色指定可能）
			void DrawRect( int sx, int sy, int ex, int ey,COLORREF theColor);

		protected :					// このクラスからのみアクセス可能==============

			int	InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth);
			int	InitPageSettingEx(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short thePage, short theCopies);

		private :					// 派生クラスとフレンドのみアクセス可能========



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

};

#endif

