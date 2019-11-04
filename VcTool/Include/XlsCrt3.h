//////////////////////////////////////////////////////////////////////////////
// Excel クリエーター Ver3.0 - C/C++ Header
//

#ifndef __XLSCRT3_H__
#define __XLSCRT3_H__

//////////////////////////////////////////////////////////////////////////////
// Excel クリエーター Ver1 互換

#ifndef _EXCEL_VER3_
#ifndef __EXCELOCX_IDL_

// 基本定義名
#define XL_DEFAULT_SHEETS			16		// 標準シート数
#define XL97_DEFAULT_SHEETS			3		// 標準シート数

// 基本色定義名
#define XLP_DEFAULT					0		// 自動
#define XLP_BLACK					8		// 黒
#define XLP_WHITE					9		// 白
#define XLP_RED						10		// 赤
#define XLP_GREEN					11		// 緑
#define XLP_BLUE					12		// 青
#define XLP_YELLOW					13		// 黄
#define XLP_MAGENTA					14		// 紫
#define XLP_CYAN					15		// 水

// 罫線種定義名
#define XL_NORMAL_LINE				1		// 細実線
#define XL_THICK_LINE				2		// 太線
#define XL_BROKEN_LINE				3		// 細破線
#define XL_DOT_LINE					4		// 細点線
#define XL_THICK2_LINE				5		// 極太線
#define XL_DOUBLE_LINE				6		// 細二重線
#define XL_SLENDER_LINE				7		// 極細実線
#define XL_MEDIUM_DASHED_LINE		8		// 中破線
#define XL_DASH_DOT_LINE			9		// 細一点鎖線
#define XL_MEDIUM_DASH_DOT_LINE		10		// 中一点鎖線
#define XL_DASH_DOT2_LINE			11		// 細二点鎖線
#define XL_MEDIUM_DASH_DOT2_LINE	12		// 中二点鎖線
#define XL_SLANTED_DASH_DOT_LINE	13		// 中一点斜鎖線

// 位置揃え情報定義名
#define XLAH_DEFAULT				0		// 水平位置標準
#define XLAH_LEFT					1		// 左詰
#define XLAH_RIGHT					3		// 右詰
#define XLAH_CENTER					2		// 中央揃え
#define XLAV_DEFAULT				2		// 垂直位置標準
#define XLAV_TOP					0		// 上詰
#define XLAV_BOTTOM					2		// 下詰
#define XLAV_CENTER					1		// 中央揃え
#define XLAO_DEFAULT				0		// 回転標準
#define XLAO_180					1		// 縦書き            (EXCEL95)
#define XLAO_L90					2		// 反時計まわり９０゜(EXCEL95)
#define XLAO_R90					3		// 時計まわり９０゜  (EXCEL95)
#define XL97AO_180					255		// 縦書き            (EXCEL97)
#define XL97_DIAG_NONE				0		// 斜線無し          (EXCEL97)
#define XL97_DIAG_DOWN				1		// 右下がり斜線      (EXCEL97)
#define XL97_DIAG_UP				2		// 右上がり斜線      (EXCEL97)
#define XL97_DIAG_BOTH				3		// 両方              (EXCEL97)

// ページレイアウト用紙種類定義名
#define XL_PS_LETTER				1		// レター
#define XL_PS_REJOER				3		// レジャー
#define XL_PS_REGAL					5		// リーガル
#define XL_PS_EXECUTIVE				7		// エグゼクティブ
#define XL_PS_A3					8		// Ａ３
#define XL_PS_A4					9		// Ａ４
#define XL_PS_A5					11		// Ａ５
#define XL_PS_B4					12		// Ｂ４
#define XL_PS_B5					13		// Ｂ５
#define XL_MMTOINCH					25.4	// ㎜→ｲﾝﾁ計算定数

// エラー定数
#define XL_FILE_OPEN_ERR			-1		// ファイルがオープンできない
#define XL_HANDLE_ERR				-2		// 無効なファイルハンドル
#define XL_NO_OPEN					-3		// 未オープンエラー
#define XL_NO_SHEET					-4		// シート番号エラー
#define XL_WRITE_ERR				-5		// 書き込み時のエラー
#define XL_ACTION_ERR				-6		// 動作モードエラー
#define XL_DATA_CAT_ERR				-7		// データ種別エラー
#define XL_ERRORVAL_ERR				-8		// エラー値
#define XL_LENGTH_ERR				-31		// 長さエラー
#define XL_POINT_ERR				-32		// 座標エラー
#define XL_ATTR_ERR					-33		// 属性番号エラー
#define XL_PARAM_ERR				-34		// パラメータエラー
#define XL_NO_DATA					-35		// データが無い
#define XL_END_OF_DATA				-36		// データの終わり
#define XL_VERPOINT_ERR				-37		// 指定した変数が存在しない
#define XL_NOMEMORY1_ERR			-40		// メモリー不足エラー１
#define XL_NOMEMORY2_ERR			-41		// メモリー不足エラー２
#define XL_INTERNAL_ERR				-50		// 内部エラー
#define XL_FUNC_ERR					-100	// 計算式形式エラー（-100 ～ -199）
#define XL_FILE_CREATE_ERR			-300	// 作成エラー
#define XL_TEMPFILE_CREATE_ERR		-301	// 一時ファイルが作成できない
#define XL_TEMPFILE_OPEN_ERR		-302	// 一時ファイルがオープンできない
#define XL_STREAM_ERR				-303	// ストリームエラー
#define XL_PROTECT_ERR				-304	// 保護されたシート
#define XL_MUTEX_ERR				-400	// ミューテックスエラー

// 属性定義構造体定義
struct CELLATTR_t {
	short FntNo;	// フォント情報番号
	short FmtNo;	// 書式情報番号
	short alcH;		// 水平位置   0:標準 1:左詰 2:中央揃え 3:右詰 4:繰り返し
					//            5:両端揃え 6:選択範囲内で中央 7:均等
	short alcK;		// 列幅で折り返して表示 0:折り返さない 1:折り返す
	short alcV;		// 垂直位置   0:上詰 1:中央 2:下詰 3:両端 4:均等
	short alcO;		// 回転 0:横書き 1:縦書き
					//      2:反時計回り90ﾟ 3:時計回り90ﾟ  (EXCEL95用)
	short sTop;		// 上罫線種 1～13 (8～ EXCEL97のみ有効)
	short sBtm;		// 下 〃
	short sLft;		// 左 〃
	short sRit;		// 右 〃
	short pTop;		// 上罫線カラーパレット番号　8～63
	short pBtm;		// 下 〃
	short pLft;		// 左 〃
	short pRit;		// 右 〃
	short fPat;		// 塗潰しパターン
	short fPatFG;	// 塗潰パターン前景カラーパレット番号　8～63
	short fPatBG;	// 塗潰パターン背景カラーパレット番号　8～63
	short fSFit;	// 縮小して全体の表示 0:しない 1:する (EXCEL97のみ有効)
	short fMCell;	// セルの結合         0:しない 1:する (EXCEL97のみ有効)
	short sgDiag;	// 斜め線         0:無し   1:右斜め下がり
					//                2:右斜上がり 3:両方 (EXCEL97のみ有効)
	short sDiag;	// 斜め罫線種 0～13                   (EXCEL97のみ有効)
	short pDiag;	// 斜め罫線カラーパレット番号         (EXCEL97のみ有効)
	short sIdnt;	// インデント値 0～15     (EXCEL97: alcHが左詰のみ有効)
	short alcO97;	// 回転 -90ﾟ ～ 90ﾟ まで 1ﾟ刻みで指定可能
					// 縦書き:255を指定する               (EXCEL97のみ有効)
};
typedef struct CELLATTR_t CELLATTR;

// 計算結果格納構造体定義
struct FUNCANSER_t {
	double dt;		// 結果区分が 0,1,2 のときの計算式結果値
	short fkbn;		// 結果区分   0:数値 1:BOOL型 2:ERROR型 3:文字列型
	short slen;		// 結果区分が 3 のときの計算式結果文字列長
	char str[256];	// 結果区分が 3 のときの計算式結果文字列
};
typedef struct FUNCANSER_t FUNCANSER;

#ifdef __cplusplus
extern "C" {  // Assume C declarations for C++
#endif
// ワークシート制御
int PASCAL XL_CreateT(char* szFileName, char* szTFileName);
int PASCAL XM_CreateT(char* szFileName, char* szTFileName);
int PASCAL XL_Create(char* szFileName, int SheetMax);
int PASCAL XL_Create97(char* szFileName, int SheetMax);
int PASCAL XL_SelSheetNo(int SheetNo);
int PASCAL XL_SheetName(char* szSheetName);
int PASCAL XL_SetColWidth(int sx, int ex, int wid);
int PASCAL XL_SetRowHeight(int y, double PointHigt);
int PASCAL XL_Close();
int PASCAL XL_Cancel();
// 属性設定
int PASCAL XL_GetAttrNo(long Kei, long Keip, int alc, int FntNo, int FmtNo);
int PASCAL XL_GetAttrNo2(CELLATTR* pCell);
int PASCAL XL_InitAttr(CELLATTR* pCell);
int PASCAL XL_FontNo(char* szFontName, double Point, int col, int styl, int UnderLine);
int PASCAL XL_FmtNo(char* szFmt);
int PASCAL XL_SetAttr(int x, int y, int xfno);
int PASCAL XL_SetAttrExt(int sx, int sy, int ex, int ey, int xfno);
int PASCAL XL_SetAttr2(int x, int y, CELLATTR* pCell);
int PASCAL XL_SetAttrExt2(int sx, int sy, int ex, int ey, CELLATTR* pCell);
int PASCAL XL_SetAttrMode(int fAttrMode);
int PASCAL XL_SetRefSheet(char* szSheets);
int PASCAL XL_SetGridMode(int fGridMode);
// 簡易書き込み
int PASCAL XL_WrtStr(char* str);
int PASCAL XL_WrtStr2(char* str, int xfno);
int PASCAL XL_WrtInt(long dt);
int PASCAL XL_WrtInt2(long dt, int xfno);
int PASCAL XL_WrtDouble(double dt);
int PASCAL XL_WrtDouble2(double dt, int xfno);
int PASCAL XL_WrtFunc(char* str);
int PASCAL XL_WrtFunc2(char* str, int xfno);
int PASCAL XL_MovePos(int x, int y);
int PASCAL XL_WrtCR();
// 標準書き込み
int PASCAL XL_WriteStr(int x, int y, char* str);
int PASCAL XL_WriteStr2(int x, int y, char* str, int xfno);
int PASCAL XL_WriteInt(int x, int y, long dt);
int PASCAL XL_WriteInt2(int x, int y, long dt, int xfno);
int PASCAL XL_WriteDouble(int x, int y, double dt);
int PASCAL XL_WriteDouble2(int x, int y, double dt, int xfno);
int PASCAL XL_WriteFunc(int x, int y, char* str);
int PASCAL XL_WriteFunc2(int x, int y, char* str, int xfno);
int PASCAL XL_WriteFunc3(int x, int y, char* str, int xfno, FUNCANSER* pAns);
// 箱型罫線描画
int PASCAL XL_BoxLine(int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XL_LtcLine(int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XL_UnderLine(int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XL_OverLine(int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XL_LeftLine(int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XL_RightLine(int sx, int sy, int ex, int ey, int LStyl);
// 罫線設定
long PASCAL XL_KeiNo(int Top, int Bottom, int Left, int Right);
long PASCAL XL_KeiPNo(int Top, int Bottom, int Left, int Right);
long PASCAL XL_KBoxNo(int LStyl);
long PASCAL XL_KTopNo(int LStyl);
long PASCAL XL_KBtmNo(int LStyl);
long PASCAL XL_KLftNo(int LStyl);
long PASCAL XL_KRitNo(int LStyl);
long PASCAL XL_KBoxPNo(int LStyl);
long PASCAL XL_KTopPNo(int LStyl);
long PASCAL XL_KBtmPNo(int LStyl);
long PASCAL XL_KLftPNo(int LStyl);
long PASCAL XL_KRitPNo(int LStyl);
// ワークシート制御（マルチファイル／マルチシート対応）
int PASCAL XM_Create(char* szFileName, int SheetMax);
int PASCAL XM_Create97(char* szFileName, int SheetMax);
int PASCAL XM_SheetName(int hXLFile, int SheetNo, char* szSheetName);
int PASCAL XM_SetColWidth(int hXLFile, int SheetNo, int sx, int ex, int wid);
int PASCAL XM_SetRowHeight(int hXLFile, int SheetNo, int y, double PointHigt);
int PASCAL XM_WriteBin(int hXLFile, int SheetNo, int id, int len, void* dt);
int PASCAL XM_Close(int hXLFile);
int PASCAL XM_Cancel(int hXLFile);
// 属性設定（マルチファイル／マルチシート対応）
int PASCAL XM_GetAttrNo(int hXLFile, long Kei, long Keip, int alc, int FntNo, int FmtNo);
int PASCAL XM_GetAttrNo2(int hXLFile, CELLATTR* pCell);
int PASCAL XM_InitAttr(CELLATTR* pCell);
int PASCAL XM_FontNo(int hXLFile, char* szFontName, double Point, int col, int styl, int UnderLine);
int PASCAL XM_FmtNo(int hXLFile, char* szFmt);
int PASCAL XM_SetAttr(int hXLFile, int SheetNo, int x, int y, int xfno);
int PASCAL XM_SetAttrExt(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int xfno);
int PASCAL XM_SetAttr2(int hXLFile, int SheetNo, int x, int y, CELLATTR* pCell);
int PASCAL XM_SetAttrExt2(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, CELLATTR* pCell);
int PASCAL XM_SetRefSheet(int hXLFile, int SheetNo, char* szSheets);
int PASCAL XM_SetGridMode(int hXLFile, int SheetNo, int fGridMode);
// 標準書き込み（マルチファイル／マルチシート対応）
int PASCAL XM_WriteStr(int hXLFile, int SheetNo, int x, int y, char* str);
int PASCAL XM_WriteStr2(int hXLFile, int SheetNo, int x, int y, char* str, int xfno);
int PASCAL XM_WriteInt(int hXLFile, int SheetNo, int x, int y, long dt);
int PASCAL XM_WriteInt2(int hXLFile, int SheetNo, int x, int y, long dt, int xfno);
int PASCAL XM_WriteDouble(int hXLFile, int SheetNo, int x, int y, double dt);
int PASCAL XM_WriteDouble2(int hXLFile, int SheetNo, int x, int y, double dt, int xfno);
int PASCAL XM_WriteFunc(int hXLFile, int SheetNo, int x, int y, char* str);
int PASCAL XM_WriteFunc2(int hXLFile, int SheetNo, int x, int y, char* str, int xfno);
int PASCAL XM_WriteFunc3(int hXLFile, int SheetNo, int x, int y, char* str, int xfno, FUNCANSER* pAns);
// 罫線設定（マルチファイル／マルチシート対応）
int PASCAL XM_BoxLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XM_LtcLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XM_UnderLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XM_OverLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XM_LeftLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XM_RightLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
                       
#ifdef  __cplusplus
}
#endif

#endif	// __EXCELOCX_IDL_
#endif	// _EXCEL_VER3_

//////////////////////////////////////////////////////////////////////////////
// Excel クリエータ Ver2 Header

// Define値

#define XL_VER95            95              // Excel95
#define XL_VER97            97              // Excel97
#define XL_VER2000          2000            // Excel2000
#define XL_VER2002          2002            // Excel2002
#define XL_VER2003          2003            // Excel2003

#define XL_XCDEFAULT        0               // カラーパレット 自動
#define XL_XCBLACK          8               //                黒
#define XL_XCWHITE          9               //                白
#define XL_XCRED            10              //                赤
#define XL_XCGREEN          11              //                緑
#define XL_XCBLUE           12              //                青
#define XL_XCYELLOW         13              //                黄
#define XL_XCPINK           14              //                ピンク
#define XL_XCCYAN           15              //                水

#define XL_XSNORMAL         0x00            // フォントスタイル ノーマル
#define XL_XSBOLD           0x01            //                  ボールド
#define XL_XSITALIC         0x02            //                  イタリック
#define XL_XSSTRIKE         0x04            //                  取消し線
#define XL_XSUP             0x08            //                  上付き
#define XL_XSDOWN           0x10            //                  下付き

#define XL_FUNONE           0               // フォント下線 なし
#define XL_FUNORMAL         1               //              下線
#define XL_FUDOUBLE         2               //              二重線

#define XL_LSNONE           0x00000000      // 罫線なし
#define XL_LSNORMAL         0x01000000      // 罫線 実線
#define XL_LSTHICK          0x02000000      //      太線
#define XL_LSBROKEN         0x03000000      //      破線
#define XL_LSDOT            0x04000000      //      点線
#define XL_LSTHICK2         0x05000000      //      極太線
#define XL_LSDOUBLE         0x06000000      //      二重線
#define XL_LSSLENDER        0x07000000      //      細実線
#define XL_LSMIDBROKEN      0x08000000      //      中破線
#define XL_LSSLNCHAIN1      0x09000000      //      細一点鎖線
#define XL_LSMIDCHAIN1      0x0A000000      //      中一点鎖線
#define XL_LSSLNCHAIN2      0x0B000000      //      細二点鎖線
#define XL_LSMIDCHAIN2      0x0C000000      //      中二点鎖線
#define XL_LSMIDCHAINS      0x0D000000      //      中一点斜鎖線

#define XL_PN01             0x01000000      // パターン番号 １
#define XL_PN02             0x02000000      // パターン番号 ２
#define XL_PN03             0x03000000      // パターン番号 ３
#define XL_PN04             0x04000000      // パターン番号 ４
#define XL_PN05             0x05000000      // パターン番号 ５
#define XL_PN06             0x06000000      // パターン番号 ６
#define XL_PN07             0x07000000      // パターン番号 ７
#define XL_PN08             0x08000000      // パターン番号 ８
#define XL_PN09             0x09000000      // パターン番号 ９
#define XL_PN10             0x0A000000      // パターン番号 10
#define XL_PN11             0x0B000000      // パターン番号 11
#define XL_PN12             0x0C000000      // パターン番号 12
#define XL_PN13             0x0D000000      // パターン番号 13
#define XL_PN14             0x0E000000      // パターン番号 14
#define XL_PN15             0x0F000000      // パターン番号 15
#define XL_PN16             0x10000000      // パターン番号 16
#define XL_PN17             0x11000000      // パターン番号 17
#define XL_PN18             0x12000000      // パターン番号 18

#define XL_BTBOX            0               // ボックスの形状 箱線
#define XL_BTLTC            1               //                格子線
#define XL_BTOVER           2               //                上横線
#define XL_BTUNDER          3               //                下横線
#define XL_BTLEFT           4               //                左縦線
#define XL_BTRIGHT          5               //                右縦線

#define XL_PHNORMAL         20              // 水平位置 標準
#define XL_PHLEFT           21              //          左詰
#define XL_PHCENTER         22              //          中央揃え
#define XL_PHRIGHT          23              //          右詰
#define XL_PHLOOP           24              //          繰り返し
#define XL_PHBOTHEVEN       25              //          両端揃え
#define XL_PHSELCENTER      26              //          選択範囲内で中央
#define XL_PHEVEN           27              //          均等

#define XL_PVUP             0               // 垂直位置 上詰め
#define XL_PVCENTER         1               //          中央揃え
#define XL_PVDOWN           2               //          下詰め
#define XL_PVBOTHEVEN       3               //          両端揃え
#define XL_PVEVEN           4               //          均等

#define XL_PTHORZ           100             // 文字方向 横
#define XL_PTVERT           101             //          縦
#define XL_PTRVTURN90       102             //          半時計回りに９０°
#define XL_PTTURN90         103             //          時計回りに９０°

#define XL_PSLETTER         1               // 用紙サイズ レター
#define XL_PSREJOER         3               //            レジャー
#define XL_PSREGAL          5               //            リーガル
#define XL_PSEXECUTIVE      7               //            エグゼクティブ
#define XL_PSA3             8               //            Ａ３
#define XL_PSA4             9               //            Ａ４
#define XL_PSA5             11              //            Ａ５
#define XL_PSB4             12              //            Ｂ４
#define XL_PSB5             13              //            Ｂ５

#define XL_ORLANDSCAPE      0               // 用紙方向 横
#define XL_ORPORTRAIT       2               //          縦

#define XL_MAXPOINT         0               // データ、属性範囲 最大範囲
#define XL_ENDPOINT         1               //                  最終ポイント

#define XL_PCNONE			0x00			// ページ中央 指定なし
#define XL_PCHORZ			0x01			//            水平
#define XL_PCVERT			0x02			//            垂直

#define XL_VMVALIDALL		0x00			// 変数名差込モード 全て差込
#define XL_VMVALIDANSWER	0x01			//                  セルのみ
#define XL_VMVALIDTEXTBOX	0x02			//                  計算式のみ
#define XL_VMVALIDCELLTEXT	0x04			//                  テキストボックス

#define XL_PORIGHTFROMLEFT	0				// ページ方向 左から右へ
#define XL_POBOTTOMFROMTOP	1				//            上から下へ

#define XL_OPOPEN           0               // オープンモード オープン
#define XL_OPCREATE         1               //                クリエイト

#define XL_KCNONE           0               // 漢字コード 未使用
#define XL_KCNECINTERNAL    1               //  〃        ＮＥＣ内部
#define XL_KCJIPSE          2               //  〃        JIPS(E)
#define XL_KCIBM            3               //  〃        IBM標準
#define XL_KCJEF9           4               //  〃        JEF9
#define XL_KCJEF12          5               //  〃        JEF12
#define XL_KCKEIS           6               //  〃        KEIS
#define XL_KCMELCOM         7               //  〃        MELCOM
#define XL_KCHAYAC          8               //  〃        HAYAC
#define XL_KCJIS            9               //  〃        JIS C6226

#define XL_DTCSV            0               // データ形式 ＣＳＶ
#define XL_DTBINARY         1               //  〃        バイナリ
#define XL_EBCDIC           2               //  〃        通信データ

#define ERR_DONTOPEN        1               // ファイルがオープンできない
#define ERR_FILEHANDLE      2               // 無効なファイルハンドル
#define ERR_NOTOPEN         3               // 未オープンエラー
#define ERR_SHEETNO         4               // シート番号エラー
#define ERR_WRITE           5               // 書き込み時のエラー
#define ERR_ACTION          6               // 動作モードエラー
#define ERR_DATACAT         7               // データ種別エラー
#define ERR_ERRORVALUE      8               // エラー値
#define ERR_CLEAR	        20              // クリアエラー
#define ERR_COPY	        21              // コピーエラー
#define ERR_PASTE	        22              // 貼り付けエラー
#define ERR_INSERT	        23              // 挿入エラー
#define ERR_DELETE	        24              // 削除エラー
#define ERR_LENGTH          31              // 長さエラー
#define ERR_LOCATE          32              // 座標エラー
#define ERR_ATTR            33              // 属性番号エラー
#define ERR_PARAM           34              // パラメータエラー
#define ERR_NODATA          35              // データが無い
#define ERR_ENDOFDATA       36              // データの終わり
#define ERR_VERPOINT        37              // 指定した変数が存在しない
#define ERR_BREAKCOUNT      38              // 改ページ数が制限を超えたた
#define ERR_MEMORY1         40              // メモリー不足エラー１
#define ERR_MEMORY2         41              // メモリー不足エラー２
#define ERR_OTHER           50              // 内部エラー
#define ERR_FUNCTION        100             // 計算式形式エラー
#define ERR_FILE_CREATE		300				// 作成エラー
#define ERR_TEMPFILE_CREATE	301				// 一時ファイルが作成できない
#define ERR_TEMPFILE_OPEN	302				// 一時ファイルがオープンできない
#define ERR_STREAM			303				// ストリームエラー
#define ERR_PROTECT			304				// 保護されたシート
#define ERR_MUTEX			400				// ミューテックスエラー

// C/C++ 用の Define 値

#ifndef   __CREATOROCX_RC_
#ifndef   __EXCELOCX_IDL_
#ifndef   __EXCELCRTOCX_H_

// Excelバージョン
#define ver95           XL_VER95            // Excel95
#define ver97           XL_VER97            // Excel97
#define ver2000         XL_VER2000          // Excel2000
#define ver2002         XL_VER2002          // Excel2002
#define ver2003         XL_VER2003          // Excel2003
// カラーパレット
#define xcDefault       XL_XCDEFAULT        // カラーパレット 自動
#define xcBlack         XL_XCBLACK          //                黒
#define xcWhite         XL_XCWHITE          //                白
#define xcRed           XL_XCRED            //                赤
#define xcGreen         XL_XCGREEN          //                緑
#define xcBlue          XL_XCBLUE           //                青
#define xcYellow        XL_XCYELLOW         //                黄
#define xcPink          XL_XCPINK           //                ピンク
#define xcCyan          XL_XCCYAN           //                水
// フォントスタイル
#define xsDefault       XL_XSNORMAL         // フォントスタイル ノーマル
#define xsNormal        XL_XSNORMAL         //                  ノーマル
#define xsBold          XL_XSBOLD           //                  ボールド
#define xsItalic        XL_XSITALIC         //                  イタリック
#define xsStrike        XL_XSSTRIKE         //                  取消し線
#define xsUp            XL_XSUP             //                  上付き
#define xsDown          XL_XSDOWN           //                  下付き
// フォント下線
#define fuDefault       XL_FUNONE           // フォント下線 なし
#define fuNone          XL_FUNONE           //              なし
#define fuNormal        XL_FUNORMAL         //              下線
#define fuDouble        XL_FUDOUBLE         //              二重線
// 罫線の線種
#define lsNone          XL_LSNONE           // 罫線 なし
#define lsDefault       XL_LSNONE           //      なし
#define lsNormal        XL_LSNORMAL         //      実線
#define lsThick         XL_LSTHICK          //      太線
#define lsBroken        XL_LSBROKEN         //      破線
#define lsDot           XL_LSDOT            //      点線
#define lsThick2        XL_LSTHICK2         //      極太線
#define lsDouble        XL_LSDOUBLE         //      二重線
#define lsSlender       XL_LSSLENDER        //      細実線
#define lsMidBroken     XL_LSMIDBROKEN      //      中破線
#define lsSlnChain1     XL_LSSLNCHAIN1      //      細一点鎖線
#define lsMidChain1     XL_LSMIDCHAIN1      //      中一点鎖線
#define lsSlnChain2     XL_LSSLNCHAIN2      //      細二点鎖線
#define lsMidChain2     XL_LSMIDCHAIN2      //      中二点鎖線
#define lsMidChains     XL_LSMIDCHAINS      //      中一点斜鎖線
// パターン番号
#define pn01            XL_PN01             // パターン番号 １
#define pn02            XL_PN02             // パターン番号 ２
#define pn03            XL_PN03             // パターン番号 ３
#define pn04            XL_PN04             // パターン番号 ４
#define pn05            XL_PN05             // パターン番号 ５
#define pn06            XL_PN06             // パターン番号 ６
#define pn07            XL_PN07             // パターン番号 ７
#define pn08            XL_PN08             // パターン番号 ８
#define pn09            XL_PN09             // パターン番号 ９
#define pn10            XL_PN10             // パターン番号 10
#define pn11            XL_PN11             // パターン番号 11
#define pn12            XL_PN12             // パターン番号 12
#define pn13            XL_PN13             // パターン番号 13
#define pn14            XL_PN14             // パターン番号 14
#define pn15            XL_PN15             // パターン番号 15
#define pn16            XL_PN16             // パターン番号 16
#define pn17            XL_PN17             // パターン番号 17
#define pn18            XL_PN18             // パターン番号 18
// ボックスの形状
#define btBox           XL_BTBOX            // ボックスの形状 箱線
#define btLtc           XL_BTLTC            //                格子線
#define btOver          XL_BTOVER           //                上横線
#define btUnder         XL_BTUNDER          //                下横線
#define btLeft          XL_BTLEFT           //                左縦線
#define btRight         XL_BTRIGHT          //                右縦線
// 水平位置
#define phNormal        XL_PHNORMAL         // 水平位置 標準
#define phLeft          XL_PHLEFT           //          左詰
#define phCenter        XL_PHCENTER         //          中央揃え
#define phRight         XL_PHRIGHT          //          右詰
#define phLoop          XL_PHLOOP           //          繰り返し
#define phBothEven      XL_PHBOTHEVEN       //          両端揃え
#define phSelCenter     XL_PHSELCENTER      //          選択範囲内で中央
#define phEven          XL_PHEVEN           //          均等
// 垂直位置
#define pvUp            XL_PVUP             // 垂直位置 上詰め
#define pvCenter        XL_PVCENTER         //          中央揃え
#define pvDown          XL_PVDOWN           //          下詰め
#define pvBothEven      XL_PVBOTHEVEN       //          両端揃え
#define pvEven          XL_PVEVEN           //          均等
// 文字方向
#define ptHorz          XL_PTHORZ           // 文字方向 横
#define ptVert          XL_PTVERT           //          縦
#define ptRvTurn90      XL_PTRVTURN90       //          半時計回りに９０°
#define ptTurn90        XL_PTTURN90         //          時計回りに９０°
// 用紙サイズ
#define psLetter        XL_PSLETTER         // 用紙サイズ レター
#define psRejoer        XL_PSREJOER         //            レジャー
#define psRegal         XL_PSREGAL          //            リーガル
#define psExecutive     XL_PSEXECUTIVE      //            エグゼクティブ
#define psA3            XL_PSA3             //            Ａ３
#define psA4            XL_PSA4             //            Ａ４
#define psA5            XL_PSA5             //            Ａ５
#define psB4            XL_PSB4             //            Ｂ４
#define psB5            XL_PSB5             //            Ｂ５
// 用紙方向
#define orLandscape     XL_ORLANDSCAPE      // 用紙方向 横
#define orPortrait      XL_ORPORTRAIT       //          縦
// データ、属性範囲
#define xlMaxPoint      XL_MAXPOINT         // データ、属性範囲 最大範囲
#define xlEndPoint      XL_ENDPOINT         //                     最終ポイント
// ページ中央
#define pcNone			XL_PCNONE			// ページ中央 指定なし
#define pcHorz			XL_PCHORZ			//            水平
#define pcVert			XL_PCVERT			//            垂直
// ページ方向
#define poRightFromLeft	XL_PORIGHTFROMLEFT	// ページ方向 左から右へ
#define poBottomFromTop	XL_POBOTTOMFROMTOP	//            上から下へ
// 変数名差込モード
#define vmValidAll			XL_VMVALIDALL		// 変数名差込モード 全て差込
#define vmInvalidAnswer		XL_VMVALIDANSWER	//                  セルのみ
#define vmInvalidTextBox	XL_VMVALIDTEXTBOX	//                  計算式のみ
#define vmInvalidCellText	XL_VMVALIDCELLTEXT	//                  テキストボックス

// オープンモード
#define opOpen          XL_OPOPEN           // オープンモード オープン
#define opCreate        XL_OPCREATE         //                クリエイト

#define vtStr           0                   // 文字列
#define vtDouble        1                   // 実数値
#define vtInt           2                   // 整数
#define vtFunc          3                   // 計算式
#define vtNull          4                   // データなし

#define kcNone          XL_KCNONE           // 漢字コード 未使用
#define kcNECInternal   XL_KCNECINTERNAL    //  〃        ＮＥＣ内部
#define kcJIPSE         XL_KCJIPSE          //  〃        JIPS(E)
#define kcIBM           XL_KCIBM            //  〃        IBM標準
#define kcJEF9          XL_KCJEF9           //  〃        JEF9
#define kcJEF12         XL_KCJEF12          //  〃        JEF12
#define kcKEIS          XL_KCKEIS           //  〃        KEIS
#define kcMELCOM        XL_KCMELCOM         //  〃        MELCOM
#define kcHAYAC         XL_KCHAYAC          //  〃        HAYAC
#define kcJIS           XL_KCJIS            //  〃        JIS C6226

#define dtCSV           XL_DTCSV            // データ形式 ＣＳＶ
#define dtBinary        XL_DTBINARY         //  〃        バイナリ
#define dtEBCDIC        XL_EBCDIC           //  〃        通信データ

#define errDontOpen     ERR_DONTOPEN        // ファイルがオープンできない
#define errFileHandle   ERR_FILEHANDLE      // 無効なファイルハンドル
#define errNotOpen      ERR_NOTOPEN         // 未オープンエラー
#define errSheetNo      ERR_SHEETNO         // シート番号エラー
#define errWrite        ERR_WRITE           // 書き込み時のエラー
#define errAction       ERR_ACTION          // 動作モードエラー
#define errDataCat      ERR_DATACAT         // データ種別エラー
#define errErrorValue	ERR_ERRORVALUE      // エラー値
#define errClear		ERR_CLEAR			// クリアエラー
#define errCopy			ERR_COPY			// コピーエラー
#define errPaste		ERR_PASTE			// 貼り付けエラー
#define errInsert		ERR_INSERT			// 挿入エラー
#define errDelete		ERR_DELETE			// 削除エラー
#define errLength       ERR_LENGTH          // 長さエラー
#define errLocate       ERR_LOCATE          // 座標エラー
#define errAttr         ERR_ATTR            // 属性番号エラー
#define errParam        ERR_PARAM           // パラメータエラー
#define errNoData       ERR_NODATA          // データが無い
#define errEndOfData    ERR_ENDOFDATA       // データの終わり
#define errVerPoint     ERR_VERPOINT        // 指定した変数が存在しない
#define errBreakCount   ERR_BREAKCOUNT      // 改ページ数が制限を越えた
#define errMemory1      ERR_MEMORY1         // メモリー不足エラー１
#define errMemory2      ERR_MEMORY2         // メモリー不足エラー２
#define errOther        ERR_OTHER           // 内部エラー
#define errFunction     ERR_FUNCTION        // 計算式形式エラー
#define errFileCreate	ERR_FILE_CREATE		// 作成エラー
#define errTempCreate	ERR_TEMPFILE_CREATE	// 一時ファイルが作成できない
#define errTempOpen		ERR_TEMPFILE_OPEN	// 一時ファイルがオープンできない
#define errStream		ERR_STREAM			// ストリームエラー
#define errProtect		ERR_PROTECT			// 保護されたシート
#define errMutex		ERR_MUTEX			// ミューテックスエラー

#endif // __EXCELCRTOCX_H_

// 関数の定義
#ifdef __cplusplus
extern "C" {  // Assume C declarations for C++
#endif
    // クリエータ関数
    BOOL PASCAL XlsPutFormat(LPSTR Format, LPSTR Cell);
    BOOL PASCAL XlsPutFontName(LPSTR FontName, LPSTR Cell);
    BOOL PASCAL XlsPutFontPoint(double FontPoint, LPSTR Cell);
    BOOL PASCAL XlsPutFontColor(long FontColor, LPSTR Cell);
    BOOL PASCAL XlsPutFontStyle(long FontStyle, LPSTR Cell);
    BOOL PASCAL XlsPutFontULine(long FontULine, LPSTR Cell);
    BOOL PASCAL XlsPutLineTop(long LineTop, LPSTR Cell);
    BOOL PASCAL XlsPutLineBottom(long LineBottom, LPSTR Cell);
    BOOL PASCAL XlsPutLineLeft(long LineLeft, LPSTR Cell);
    BOOL PASCAL XlsPutLineRight(long LineRight, LPSTR Cell);
    BOOL PASCAL XlsPutLineLeftUp(long LineLeftUp, LPSTR Cell);
    BOOL PASCAL XlsPutLineRightUp(long LineRightUp, LPSTR Cell);
    BOOL PASCAL XlsPutPattern(long PatNo, LPSTR Cell);
    BOOL PASCAL XlsPutBackColor(long BackColor, LPSTR Cell);
    BOOL PASCAL XlsPutPosHorz(long PosHorz, LPSTR Cell);
    BOOL PASCAL XlsPutPosVert(long PosVert, LPSTR Cell);
    BOOL PASCAL XlsPutPosTurn(long PosTurn, LPSTR Cell);
    BOOL PASCAL XlsPutOverReturn(BOOL OverReturn, LPSTR Cell);
    BOOL PASCAL XlsPutFit(BOOL Fit, LPSTR Cell);
    BOOL PASCAL XlsPutJoint(BOOL Joint, LPSTR Cell);

    BOOL PASCAL XlsPutDouble(double Double, LPSTR Cell);
    BOOL PASCAL XlsGetDouble(double *Double, LPSTR Cell);
    BOOL PASCAL XlsPutLong(long Long, LPSTR Cell);
    BOOL PASCAL XlsGetLong(long *Long, LPSTR Cell);
    BOOL PASCAL XlsPutStr(LPSTR Str, LPSTR Cell);
    BOOL PASCAL XlsGetStr(LPSTR Str, int StrSize, LPSTR Cell);
    BOOL PASCAL XlsPutFunc(LPSTR Func, LPSTR Ans, LPSTR Cell);
    BOOL PASCAL XlsPutBox(long LineStyle, long BoxType, LPSTR Cell);
    BOOL PASCAL XlsPutValue(LPSTR Value, LPSTR Cell);
    BOOL PASCAL XlsGetValue(LPSTR pVal, int nSize, LPSTR Cell);
    BOOL PASCAL XlsPutValueV(VARIANT Value, LPSTR Cell);
    BOOL PASCAL XlsGetValueV(VARIANT* pVal, LPSTR Cell);
    BOOL PASCAL XlsPutFuncV(LPSTR Func, VARIANT Ans, LPSTR Cell);

    BOOL PASCAL XlsPutColWidth(long ColWidth, LPSTR Cell);
    BOOL PASCAL XlsPutColWidthEx(double ColWidth, LPSTR Cell);
    BOOL PASCAL XlsPutColWidth2(long ColWidth, LPSTR Cell);
    BOOL PASCAL XlsGetColWidth2(long* ColWidth, LPSTR Cell);
    BOOL PASCAL XlsPutRowHeight(double RowHeight, LPSTR Cell);
    BOOL PASCAL XlsPutRowHeight2(double RowHeight, LPSTR Cell);
    BOOL PASCAL XlsGetRowHeight2(double* RowHeight, LPSTR Cell);
    BOOL PASCAL XlsPutRefSheet(LPSTR RefSheet);
    BOOL PASCAL XlsPutSheetName(LPSTR SheetName);
    BOOL PASCAL XlsPutDefFontName(LPSTR FontName);
    BOOL PASCAL XlsPutDefFontPoint(double FontPoint);
    BOOL PASCAL XlsMove(LPSTR Cell);

    BOOL PASCAL XlsCreateBook(LPSTR FileName, long Ver, long SheetMax, long *ID);
    BOOL PASCAL XlsReadBook(LPSTR FileName, long *ID);
    BOOL PASCAL XlsOpenBook(LPSTR FileName, LPSTR OverlayName, long *ID);
    BOOL PASCAL XlsCloseBook(BOOL flg);
    BOOL PASCAL XlsSelectBook(long Book);
    BOOL PASCAL XlsSheetNo(long SheetNo);
    BOOL PASCAL XlsPageMode(long PageSize, long Scale, long Orientation, double HdrMergin, double FtrMergin);
    BOOL PASCAL XlsSetMargin(int Left, int Right, int Top, int Bottom);
    BOOL PASCAL XlsAddSheet(long SheetCnt);
    BOOL PASCAL XlsDelSheet(long SheetNo, long SheetCnt);
    BOOL PASCAL XlsModeGrid(BOOL ModeGrid);

    BOOL PASCAL XlsGetErrorNo(long *ErrorNo);
    BOOL PASCAL XlsGetErrorMessage(LPSTR ErrorMessage, int ErrorMessageSize);

    LPSTR PASCAL CELL(LPSTR Cell, long x, long y);
    LPSTR PASCAL CELL2(LPSTR Cell, long x, long y);
    LPSTR PASCAL POS(long sx, long sy, long ex, long ey);
    LPSTR PASCAL POS2(long sx, long sy, long ex, long ey);

    BOOL PASCAL XlsGetSheetName(long lSheetNo, LPSTR pszSheetName, int nSize);

    int PASCAL XlsGetSheetCount();
    BOOL PASCAL XlsPutBreak(BOOL bBreak, LPSTR pszCell);
    BOOL PASCAL XlsSetHeader(LPCTSTR pszLeft, LPCTSTR pszCenter, LPCTSTR pszRight);
    BOOL PASCAL XlsSetFooter(LPCTSTR pszLeft, LPCTSTR pszCenter, LPCTSTR pszRight);
	BOOL PASCAL XlsCopySheet(long lSource, long lInsertPos, LPCTSTR pszSheetName);
    int PASCAL XlsGetActiveSheet();
    BOOL PASCAL XlsSetActiveSheet(long lSheetNo);
	int PASCAL XlsSetKeyWord(LPCTSTR pszKeyWord);
	BOOL PASCAL XlsSetZoom(long lZoom);
	BOOL PASCAL XlsGetLastPos(long* ex, long* ey, BOOL bDataOnly, int pt);
	BOOL PASCAL XlsRowDelete(long sy, long lCount);
	BOOL PASCAL XlsRowClear(long sy, long lCount);
	BOOL PASCAL XlsRowInsert(long sy, long lCount);
	BOOL PASCAL XlsRowCopy(long sy, long dy);
	BOOL PASCAL XlsRowPaste(long dy);
	BOOL PASCAL XlsColumnDelete(long sx, long lCount);
	BOOL PASCAL XlsColumnClear(long sx, long lCount);
	BOOL PASCAL XlsColumnInsert(long sx, long lCount);
	BOOL PASCAL XlsColumnCopy(long sx, long dx);
	BOOL PASCAL XlsColumnPaste(long dx);
	BOOL PASCAL XlsClear(LPCTSTR pszCurCell);
	BOOL PASCAL XlsCopy(LPCTSTR pszCurCell, LPCTSTR pszCell);
	BOOL PASCAL XlsPaste(LPCTSTR pszCurCell);
	long PASCAL XlsGetAttrNo(LPCTSTR pszCurCell);
    BOOL PASCAL XlsPutDouble2(double Double, LPSTR Cell, long xfno);
    BOOL PASCAL XlsPutLong2(long Long, LPSTR Cell, long xfno);
    BOOL PASCAL XlsPutStr2(LPSTR Str, LPSTR Cell, long xfno);
    BOOL PASCAL XlsPutValue2(VARIANT Value, LPSTR Cell, long xfno);
	BOOL PASCAL XlsGetSheetName2(long lSheetNo, LPSTR pszSheetName);
	int PASCAL XlsGetSheetNo2(LPCTSTR pszSheetName);
	BOOL PASCAL XlsVarCheck(LPCTSTR pszVarName);
	int PASCAL XlsSetSizeFree(double dblWidth, double dblHeight, LPCTSTR pszPaperName, LPCTSTR pszPrinterName);
	int PASCAL XlsSetPrintArea(int sx, int sy, int ex, int ey);
	int PASCAL XlsPutCenter(long lCenter);
	int PASCAL XlsPutPageOrder(long lPageOrder);
	void PASCAL XlsPutVarInsertMode(long lInsertMode);
	BOOL PASCAL XlsGetVarNamePos(LPCTSTR pszVarName, int* cx, int* cy, int nIndex);
	BOOL PASCAL XlsGetNamePos(LPCTSTR pszName, int* cx, int* cy);

    // ビューア関数
    HWND PASCAL ViewCreateWnd(HWND hwndParent, RECT *prc, UINT id);
    BOOL PASCAL ViewRelease(HWND hwnd);
    BOOL PASCAL ViewCreateClass(HINSTANCE hInst);
    BOOL PASCAL ViewReleaseClass();
    BOOL PASCAL ViewPutZoom(HWND hwnd, long nZoom);
    BOOL PASCAL ViewPutUrlName(HWND hwnd, LPSTR pUrlName);
    BOOL PASCAL ViewPutModeGrid(HWND hwnd, BOOL ModeGrid);
    BOOL PASCAL ViewPutSheetNo(HWND hwnd, long SheetNo);
    BOOL PASCAL ViewGetErrorNo(long *ErrorNo);
    BOOL PASCAL ViewGetErrorMessage(LPSTR ErrorMessage, int ErrorMessageSize);
    BOOL PASCAL ViewPutContextMenu(HWND hwnd, BOOL bContextMenu);
    BOOL PASCAL ViewPutOpenEnable(HWND hwnd, BOOL bOpenEnable);
    BOOL PASCAL ViewPutSaveEnable(HWND hwnd, BOOL bSaveEnable);
    BOOL PASCAL ViewPutPrintEnable(HWND hwnd, BOOL bPrintEnable);
    BOOL PASCAL ViewSetPrint(HWND hwnd);
    BOOL PASCAL ViewPrint(HWND hwnd, long nSheetNoStart,long nSheetNoEnd);
    BOOL PASCAL ViewPreView(HWND hwnd, long x, long y, long width, long height);

    // コンバート関数
    int  PASCAL EXCEL_Conv(char *);
    long PASCAL ConvGetCnt();
    BOOL PASCAL ConvGetErrorNo(long *ErrorNo);
    BOOL PASCAL ConvGetErrorMessage(LPSTR ErrorMessage, int ErrorMessageSize);
#ifdef  __cplusplus
}
#endif

// クリエータ関数
typedef BOOL (CALLBACK* XLSPUTFORMATPROC)(LPSTR Format, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFONTNAMEPROC)(LPSTR FontName, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFONTPOINTPROC)(double FontPoint, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFONTCOLORPROC)(long FontColor, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFONTSTYLEPROC)(long FontStyle, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFONTULINEPROC)(long FontULine, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINETOPPROC)(long LineTop, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINEBOTTOMPROC)(long LineBottom, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINELEFTPROC)(long LineLeft, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINERIGHTPROC)(long LineRight, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINELEFTUPPROC)(long LineLeftUp, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINERIGHTUPPROC)(long LineRightUp, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTPATTERNPROC)(long PatNo, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTBACKCOLORPROC)(long BackColor, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTPOSHORZPROC)(long PosHorz, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTPOSVERTPROC)(long PosVert, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTPOSTURNPROC)(long PosTurn, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTOVERRETURNPROC)(BOOL OverReturn, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFITPROC)(BOOL Fit, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTJOINTPROC)(BOOL Joint, LPSTR Cell);

typedef BOOL (CALLBACK* XLSPUTDOUBLEPROC)(double Double, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETDOUBLEPROC)(double *Double, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLONGPROC)(long Long, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETLONGPROC)(long *Long, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTSTRPROC)(LPSTR Str, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETSTRPROC)(LPSTR Str, int StrSize, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETSTRWPROC)(WCHAR* Str, int StrSize, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFUNCPROC)(LPSTR Func, VARIANT Ans, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTBOXPROC)(long LineStyle, long BoxType, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTVALUEPROC)(LPSTR Value, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETVALUEPROC)(LPSTR pVal, int ValueSize, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTVALUEVPROC)(VARIANT Value, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETVALUEVPROC)(VARIANT* pVal, LPSTR Cell);

typedef BOOL (CALLBACK* XLSPUTCOLWIDTHPROC)(long ColWidth, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTROWHEIGHTPROC)(double RowHeight, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTREFSHEETPROC)(LPSTR RefSheet);
typedef BOOL (CALLBACK* XLSPUTSHEETNAMEPROC)(LPSTR SheetName);
typedef BOOL (CALLBACK* XLSPUTDEFFONTNAMEPROC)(LPSTR FontName);
typedef BOOL (CALLBACK* XLSPUTDEFFONTPOINTPROC)(double FontPoint);
typedef BOOL (CALLBACK* XLSMOVEPROC)(LPSTR Cell);

typedef BOOL (CALLBACK* XLSCREATEBOOKPROC)(LPSTR FileName, long Ver, long SheetMax, long *ID);
typedef BOOL (CALLBACK* XLSREADBOOKPROC)(LPSTR FileName, long *ID);
typedef BOOL (CALLBACK* XLSOPENBOOKPROC)(LPSTR FileName, LPSTR OverlayName, long *ID);
typedef BOOL (CALLBACK* XLSCLOSEBOOKPROC)(BOOL flg);
typedef BOOL (CALLBACK* XLSSELECTBOOKPROC)(long Book);
typedef BOOL (CALLBACK* XLSSHEETNOPROC)(long SheetNo);
typedef BOOL (CALLBACK* XLSPAGEMODEPROC)(long PageSize, long Scale, long Orientation, double HdrMergin, double FtrMergin);
typedef BOOL (CALLBACK* XLSSETMARGIN)(int Left, int Right, int Top, int Bottom);
typedef BOOL (CALLBACK* XLSADDSHEETPROC)(long SheetCnt);
typedef BOOL (CALLBACK* XLSDELSHEETPROC)(long SheetNo, long SheetCnt);
typedef BOOL (CALLBACK* XLSMODEGRIDPROC)(BOOL ModeGrid);

typedef BOOL (CALLBACK* XLSGETERRORNOPROC)(long *ErrorNo);
typedef BOOL (CALLBACK* XLSGETERRORMESSAGEPROC)(LPSTR ErrorMessage, int ErrorMessageSize);

typedef LPSTR (CALLBACK* CELLPROC)(LPSTR Cell, long x, long y);
typedef LPSTR (CALLBACK* POSPROC)(long sx, long sy, long ex, long ey);

typedef BOOL (CALLBACK* XLSGETSHEETNAMEPROC)(int sno, LPSTR pstr, int strsize);

typedef BOOL (CALLBACK* XLSGETSHEETCOUNTPROC)();
typedef BOOL (CALLBACK* XLSPUTBREAKPROC)(BOOL Break, LPSTR Cell);
typedef BOOL (CALLBACK* XLSSETHEADERPROC)(LPCTSTR pszLeft, LPCTSTR pszCenter, LPCTSTR pszRight);
typedef BOOL (CALLBACK* XLSSETFOOTERPROC)(LPCTSTR pszLeft, LPCTSTR pszCenter, LPCTSTR pszRight);
typedef BOOL (CALLBACK* XLSCOPYSHEETPROC)(long lSource, long lInsertPos, LPCTSTR pszSheetName);
typedef int (CALLBACK* XLSGETACTIVESHEETPROC)();
typedef BOOL (CALLBACK* XLSSETACTIVESHEETPROC)(long lSheetNo);
typedef BOOL (CALLBACK* XLSSETKEYWORDPROC)(LPCTSTR pszKeyWord);

typedef BOOL (CALLBACK* XLSSETZOOMPROC)(long lZoom);
typedef BOOL (CALLBACK* XLSGETLASTPOSPROC)(long* ex, long* ey, BOOL bDataOnly);
typedef BOOL (CALLBACK* XLSROWDELETEPROC)(long sy, long lCount);
typedef BOOL (CALLBACK* XLSROWCLEARPROC)(long sy, long lCount);
typedef BOOL (CALLBACK* XLSROWINSERTPROC)(long sy, long lCount);
typedef BOOL (CALLBACK* XLSROWCOPYPROC)(long sy, long dy);
typedef BOOL (CALLBACK* XLSROWPASTEPROC)(long dy);
typedef BOOL (CALLBACK* XLSCOLUMNDELETEPROC)(long sx, long lCount);
typedef BOOL (CALLBACK* XLSCOLUMNCLEARPROC)(long sx, long lCount);
typedef BOOL (CALLBACK* XLSCOLUMNINSERTPROC)(long sx, long lCount);
typedef BOOL (CALLBACK* XLSCOLUMNCOPYPROC)(long sx, long dx);
typedef BOOL (CALLBACK* XLSCOLUMNPASTEPROC)(long dx);
typedef BOOL (CALLBACK* XLSCLEARPROC)(LPCTSTR pszCurCell);
typedef BOOL (CALLBACK* XLSCOPYPROC)(LPCTSTR pszCurCell, LPCTSTR pszCell);
typedef BOOL (CALLBACK* XLSPASTEPROC)(LPCTSTR pszCurCell);
typedef long (CALLBACK* XLSGETATTRNOPROC)(LPCTSTR pszCurCell);
typedef BOOL (CALLBACK* XLSPUTDOUBLE2PROC)(double Double, LPSTR Cell, long xfno);
typedef BOOL (CALLBACK* XLSPUTLONG2PROC)(long Long, LPSTR Cell, long xfno);
typedef BOOL (CALLBACK* XLSSTR2PROC)(LPSTR Str, LPSTR Cell, long xfno);
typedef BOOL (CALLBACK* XLSVALUE2PROC)(VARIANT Value, LPSTR Cell, long xfno);
typedef BOOL (CALLBACK* XLSGETSHEETNAME2PROC)(long lSheetNo, LPSTR pszSheetName);
typedef int (CALLBACK* XLSGETSHEETNO2PROC)(LPCTSTR pszSheetName);

// ビューア関数
typedef HWND (CALLBACK* VIEWCREATEWNDPROC)(HWND hwndParent,RECT *prc,UINT id);
typedef BOOL (CALLBACK* VIEWCREATECLASSPROC)(HINSTANCE hInst);
typedef BOOL (CALLBACK* VIEWRELEASECLASSPROC)();
typedef BOOL (CALLBACK* VIEWPUTZOOMPROC)(HWND hwnd,long nZoom);
typedef BOOL (CALLBACK* VIEWPUTURLNAMEPROC)(HWND hwnd,LPSTR pUrlName);
typedef BOOL (CALLBACK* VIEWPUTMODEGRIDPROC)(HWND hwnd,BOOL ModeGrid);
typedef BOOL (CALLBACK* VIEWPUTSHEETNOPROC)(HWND hwnd,long SheetNo);
typedef BOOL (CALLBACK* VIEWGETERRORNOPROC)(long *ErrorNo);
typedef BOOL (CALLBACK* VIEWGETERRORMESSAGEPROC)(LPSTR ErrorMessage, int ErrorMessageSize);
typedef BOOL (CALLBACK* VIEWPUTCONTEXTMENUPROC)(HWND hwnd,BOOL bContextMenu);
typedef BOOL (CALLBACK* VIEWPUTOPENENABLEPROC)(HWND hwnd,BOOL bOpenEnable);
typedef BOOL (CALLBACK* VIEWPUTSAVEENABLEPROC)(HWND hwnd,BOOL bSaveEnable);
typedef BOOL (CALLBACK* VIEWPUTPRINTENABLEPROC)(HWND hwnd,BOOL bPrintEnable);
typedef BOOL (CALLBACK* VIEWSETPRINTPROC)(HWND hwnd);
typedef BOOL (CALLBACK* VIEWPRINTPROC)(HWND hwnd,long nSheetNoStart,long nSheetNoEnd);
typedef BOOL (CALLBACK* VIEWPREVIEWPROC)(HWND hwnd,long x,long y,long width,long height);

// コンバート関数
typedef int  (CALLBACK* EXCEL_CONVPROC)(char *);
typedef long (CALLBACK* CONVGETCNTPROC)();
typedef BOOL (CALLBACK* CONVGETERRORNOPROC)(long *ErrorNo);
typedef BOOL (CALLBACK* CONVGETERRORMESSAGEPROC)(LPSTR ErrorMessage, int ErrorMessageSize);

#endif	// __EXCELOCX_IDL_
#endif	// __CREATOROCX_RC_

#endif	// __XLSCRT3_H
