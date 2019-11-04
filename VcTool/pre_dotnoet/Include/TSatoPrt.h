#ifndef SATO_LABEL_PRINTER
#define SATO_LABEL_PRINTER
//=====================================================================================================================
//
//  	さとーバーコードプリンターの制御基本クラス(パラレルインターフェース版)
//	
//		FileName : tsatoprt.cpp
//		ＮＯＫ株式会社 技術本部 生産技術部
//		2001.6.15 coba
//		修正日：2004. 2. 4 PDF417/MAXI 追加 (by N.Maeda)
//              2004. 5.12 用紙サイズ、ラベルサイズ追加 (by N.Maeda)
//              2004. 5.24 レスプリ初期化コマンド追加
//=====================================================================================================================

#include <windows.h>
#include <winspool.h>
#include <winbase.h>

#define S_ESC 0x1B
#define S_STX 0x02
#define S_ETX 0x03
#define S_ENQ 0x05

// プリンタ型番
#define SATO_ER400 0
#define SATO_ER410 1

typedef struct{
	short PrintSpeed;			//00H=2inch/sec、01H=3、02H=4、03H=5、04H=6  (03H,04HはR8のみ)
	short Cutter;				// カッター動作 (00H,01H,02H)
	short Hakuri;				// ハクリ動作 (00H,01H)
	short NonSepa;				// ノンセパ動作 (00H,01H)
	short ConcentrationSetup;	// 印字濃度設定 41H～46H (41H=R8初期値、42H=R12初期値)
	short ConcentrationLevel;	// 印字濃度レベル
	short Sensor;				// センサ種 (00H=反射,01H=透過,02H=センサ無視)
	short Zero;					//
	short Kanji;				//
	short Paper;				//
	short Feed;					//
	short Pitch;				//
	short VDot;					// 縦ラベルサイズ(ドット)
	short HDot;					// 横ラベルサイズ
	short VOffset;				// 縦起点補正値(ドット）
	short HOffset;				// 横
	short PitchOffset;			// 用地ピッチオフセット
	short TearOffset;			// TearOffオフセット
	short CutterOffset;			// カッタオフセット
	short HakuriOffset;			// ハクリオフセット
	short Comatibility;			// 印字位置互換
	short Gap;					// ラベル間ギャップ
	short Buzzer;				// ブザー音設定
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
		HANDLE	m_hPrinter;			// プリンタハンドル

	 	int  ConnectPrinter(char* ptrname);
		int  SendData(char *data,int len);
		void SetPosition( int x, int y);

		int m_prtType;

		double m_dpm;

};
#endif
