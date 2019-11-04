/* UNLHA32.H, Micco,  Nov.15,2004
**
** LH 2.02a
** Copyright (c) 1988-95   by Haruyasu Yoshizaki  All rights reserved.
**
** Win/WinNT-SFX
** Copyright (c) 1995-96   by mH        All rights reserved.
**
** UNLHA32.DLL
** Copyright (c) 1995-2004 by Micco     All rights reserved.
*/


/*
	　このヘッダファイルを読み込む前に，windows.h 等，基本的なヘッダを
	前もって読み込んでおく必要があります。統一されているようで，以外と
	各コンパイラ，プラットフォームで統一されていない関係上，このヘッダ
	内で必要ヘッダの読み込みは行っていません。
*/

#if !defined(UNLHA32_H)
#define UNLHA32_H

#define UNLHA32_VERSION		194

#ifndef FNAME_MAX32
#define FNAME_MAX32		512
#endif

#ifndef ARC_DECSTRACT
#define ARC_DECSTRACT

#if defined(__BORLANDC__)
#pragma option -a-
#else
#pragma pack(1)
#endif

#if !defined(__BORLANDC__) || __BORLANDC__ >= 0x550
typedef LONGLONG	ULHA_INT64;
#else
typedef struct {
	DWORD	LowPart;
	LONG	HighPart;
} ULHA_INT64, *LPULHA_INT64;
#endif

typedef	HGLOBAL	HARC;

typedef struct {
	DWORD 			dwOriginalSize;		/* ファイルのサイズ */
 	DWORD 			dwCompressedSize;	/* 圧縮後のサイズ */
	DWORD			dwCRC;				/* 格納ファイルのチェックサム */
	UINT			uFlag;				/* 処理結果 */
										/* Status flag */
	UINT			uOSType;			/* 書庫作成に使われた OS */
	WORD			wRatio;				/* 圧縮率 */
	WORD			wDate;				/* 格納ファイルの日付(DOS 形式) */
	WORD 			wTime;				/* 格納ファイルの時刻(〃) */
	char			szFileName[FNAME_MAX32 + 1];	/* 書庫名 */
	char			dummy1[3];
	char			szAttribute[8];		/* 格納ファイルの属性(書庫固有) */
	char			szMode[8];			/* 格納ファイルの格納モード(〃) */
}	INDIVIDUALINFO, *LPINDIVIDUALINFO;

typedef struct {
	DWORD 			dwFileSize;		/* 格納ファイルのサイズ */
	DWORD			dwWriteSize;	/* 書き込みサイズ */
	char			szSourceFileName[FNAME_MAX32 + 1];	/* 格納ファイル名 */
	char			dummy1[3];
	char			szDestFileName[FNAME_MAX32 + 1];
									/* 解凍先または圧縮元パス名 */
	char			dummy[3];
}	EXTRACTINGINFO, *LPEXTRACTINGINFO;

typedef struct {
	EXTRACTINGINFO exinfo;
	DWORD dwCompressedSize;
	DWORD dwCRC;
	UINT  uOSType;
	WORD  wRatio;
	WORD  wDate;
	WORD  wTime;
	char  szAttribute[8];
	char  szMode[8];
} EXTRACTINGINFOEX, *LPEXTRACTINGINFOEX;

typedef struct {
	DWORD			dwStructSize;
	EXTRACTINGINFO	exinfo;
	DWORD			dwFileSize;			/* 格納ファイルのサイズ */
	DWORD			dwCompressedSize;
	DWORD			dwWriteSize;		/* 書き込みサイズ */
	DWORD			dwAttributes;
	DWORD 			dwCRC;
	UINT  			uOSType;
	WORD  			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char  			szMode[8];
	char			szSourceFileName[FNAME_MAX32 + 1];	/* 格納ファイル名 */
	char			dummy1[3];
	char			szDestFileName[FNAME_MAX32 + 1];
										/* 解凍先または圧縮元パス名 */
	char			dummy2[3];
} EXTRACTINGINFOEX32, *LPEXTRACTINGINFOEX32;

typedef struct {
	DWORD			dwStructSize;
	EXTRACTINGINFO	exinfo;
	ULHA_INT64		llFileSize;			/* 格納ファイルのサイズ */
	ULHA_INT64		llCompressedSize;
	ULHA_INT64		llWriteSize;		/* 書き込みサイズ */
	DWORD			dwAttributes;
	DWORD 			dwCRC;
	UINT  			uOSType;
	WORD  			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char  			szMode[8];
	char			szSourceFileName[FNAME_MAX32 + 1];	/* 格納ファイル名 */
	char			dummy1[3];
	char			szDestFileName[FNAME_MAX32 + 1];
										/* 解凍先または圧縮元パス名 */
	char			dummy2[3];
} EXTRACTINGINFOEX64, *LPEXTRACTINGINFOEX64;

typedef BOOL CALLBACK ARCHIVERPROC(HWND _hwnd, UINT _uMsg,
						UINT _nState, LPVOID _lpEis);
typedef ARCHIVERPROC *LPARCHIVERPROC;

typedef struct {
	DWORD			dwStructSize;
	UINT			uCommand;
	DWORD			dwOriginalSize;
	DWORD			dwCompressedSize;
	DWORD			dwAttributes;
	DWORD			dwCRC;
	UINT			uOSType;
	WORD			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char			szFileName[FNAME_MAX32 + 1];
	char			dummy1[3];
	char			szAddFileName[FNAME_MAX32 + 1];
	char			dummy2[3];
}	UNLHA_ENUM_MEMBER_INFO, *LPUNLHA_ENUM_MEMBER_INFO;

typedef struct {
	DWORD			dwStructSize;
	UINT			uCommand;
	ULHA_INT64		llOriginalSize;
	ULHA_INT64		llCompressedSize;
	DWORD			dwAttributes;
	DWORD			dwCRC;
	UINT			uOSType;
	WORD			wRatio;
	FILETIME		ftCreateTime;
	FILETIME		ftAccessTime;
	FILETIME		ftWriteTime;
	char			szFileName[FNAME_MAX32 + 1];
	char			dummy1[3];
	char			szAddFileName[FNAME_MAX32 + 1];
	char			dummy2[3];
}	UNLHA_ENUM_MEMBER_INFO64, *LPUNLHA_ENUM_MEMBER_INFO64;

typedef BOOL (CALLBACK *UNLHA_WND_ENUMMEMBPROC)(LPVOID);

#if !defined(__BORLANDC__)
#pragma pack()
#else
#pragma option -a.
#endif

#endif /* ARC_DECSTRACT */

#if !defined(__BORLANDC__)
#define	_export
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ### LHA.DLL Ver 1.1 と互換性のある API 群です。### */
/* LHA.DLL compatible API */
WORD WINAPI _export UnlhaGetVersion(VOID);

BOOL WINAPI _export UnlhaGetRunning(VOID);

BOOL WINAPI _export UnlhaGetBackGroundMode(VOID);
BOOL WINAPI _export UnlhaSetBackGroundMode(const BOOL _BackGroundMode);
BOOL WINAPI _export UnlhaGetCursorMode(VOID);
BOOL WINAPI _export UnlhaSetCursorMode(const BOOL _CursorMode);
WORD WINAPI _export UnlhaGetCursorInterval(VOID);
BOOL WINAPI _export UnlhaSetCursorInterval(const WORD _Interval);

int WINAPI _export Unlha(const HWND _hwnd, LPCSTR _szCmdLine,
						LPSTR _szOutput, const DWORD _dwSize);

/* ###『統合アーカイバ API』共通の API です。### */
/* API of Common Archiver Project */
#if !defined(CHECKARCHIVE_RAPID)
#define	CHECKARCHIVE_RAPID		0	/* 簡易(最初の３ヘッダまで) */
#define	CHECKARCHIVE_BASIC		1	/* 標準(全てのヘッダ) */
#define	CHECKARCHIVE_FULLCRC	2	/* 完全(格納ファイルの CRC チェック) */

/* 以下のフラグは上記と組み合わせて使用。*/
#define CHECKARCHIVE_RECOVERY	4   /* 破損ヘッダを読み飛ばして処理 */
#define CHECKARCHIVE_SFX		8	/* SFX かどうかを返す */
#define CHECKARCHIVE_ALL		16	/* ファイルの最後まで検索する */
#define CHECKARCHIVE_ENDDATA	32	/* 書庫より後ろの余剰データを許可 */
#endif

BOOL WINAPI _export UnlhaCheckArchive(LPCSTR _szFileName, const int _iMode);

int WINAPI _export UnlhaGetFileCount(LPCSTR _szArcFile);

#if !defined(UNPACK_CONFIG_MODE)
#define	UNPACK_CONFIG_MODE		1	/* 解凍系の設定 */
#define PACK_CONFIG_MODE		2	/* 圧縮系の設定 */
#endif

BOOL WINAPI _export UnlhaConfigDialog(const HWND _hwnd, LPSTR _lpszComBuffer,
						const int _iMode);

#if !defined(ISARC_FUNCTION_START)
#define ISARC_FUNCTION_START			0
#define ISARC							0	/* Unlha */
#define ISARC_GET_VERSION				1	/* UnlhaGetVersion */
#define ISARC_GET_CURSOR_INTERVAL		2	/* UnlhaGetCursorInterval */
#define ISARC_SET_CURSOR_INTERVAL		3	/* UnlhaSetCursorInterval */
#define ISARC_GET_BACK_GROUND_MODE		4	/* UnlhaGetBackGroundMode */
#define ISARC_SET_BACK_GROUND_MODE		5	/* UnlhaSetBackGroundMode */
#define ISARC_GET_CURSOR_MODE			6	/* UnlhaGetCursorMode */
#define ISARC_SET_CURSOR_MODE			7	/* UnlhaSetCursorMode */
#define ISARC_GET_RUNNING				8	/* UnlhaGetRunning */

#define ISARC_CHECK_ARCHIVE				16	/* UnlhaCheckArchive */
#define ISARC_CONFIG_DIALOG				17	/* UnlhaConfigDialog */
#define ISARC_GET_FILE_COUNT			18	/* UnlhaGetFileCount */
#define ISARC_QUERY_FUNCTION_LIST		19	/* UnlhaQueryFunctionList */
#define ISARC_HOUT						20	/* (UnlhaHOut) */
#define ISARC_STRUCTOUT					21	/* (UnlhaStructOut) */
#define ISARC_GET_ARC_FILE_INFO			22	/* UnlhaGetArcFileInfo */

#define ISARC_OPEN_ARCHIVE				23	/* UnlhaOpenArchive */
#define ISARC_CLOSE_ARCHIVE				24	/* UnlhaCloseArchive */
#define ISARC_FIND_FIRST				25	/* UnlhaFindFirst */
#define ISARC_FIND_NEXT					26	/* UnlhaFindNext */
#define ISARC_EXTRACT					27	/* (UnlhaExtract) */
#define ISARC_ADD						28	/* (UnlhaAdd) */
#define ISARC_MOVE						29	/* (UnlhaMove) */
#define ISARC_DELETE					30	/* (UnlhaDelete) */
#define ISARC_SETOWNERWINDOW			31	/* UnlhaSetOwnerWindow */
#define ISARC_CLEAROWNERWINDOW			32	/* UnlhaClearOwnerWindow */
#define ISARC_SETOWNERWINDOWEX			33	/* UnlhaSetOwnerWindowEx */
#define ISARC_KILLOWNERWINDOWEX			34	/* UnlhaKillOwnerWindowEx */

#define ISARC_GET_ARC_FILE_NAME			40	/* UnlhaGetArcFileName */
#define ISARC_GET_ARC_FILE_SIZE			41	/* UnlhaGetArcFileSize */
#define ISARC_GET_ARC_ORIGINAL_SIZE		42	/* UnlhaArcOriginalSize */
#define ISARC_GET_ARC_COMPRESSED_SIZE	43	/* UnlhaGetArcCompressedSize */
#define ISARC_GET_ARC_RATIO				44	/* UnlhaGetArcRatio */
#define ISARC_GET_ARC_DATE				45	/* UnlhaGetArcDate */
#define ISARC_GET_ARC_TIME				46	/* UnlhaGetArcTime */
#define ISARC_GET_ARC_OS_TYPE			47	/* UnlhaGetArcOSType */
#define ISARC_GET_ARC_IS_SFX_FILE		48	/* UnlhaGetArcIsSFXFile */
#define ISARC_GET_ARC_WRITE_TIME_EX		49	/* UnlhaGetArcWriteTimeEx */
#define ISARC_GET_ARC_CREATE_TIME_EX	50	/* UnlhaGetArcCreateTimeEx */
#define	ISARC_GET_ARC_ACCESS_TIME_EX	51	/* UnlhaGetArcAccessTimeEx */
#define	ISARC_GET_ARC_CREATE_TIME_EX2	52	/* UnlhaGetArcCreateTimeEx2 */
#define ISARC_GET_ARC_WRITE_TIME_EX2	53	/* UnlhaGetArcWriteTimeEx2 */
#define ISARC_GET_FILE_NAME				57	/* UnlhaGetFileName */
#define ISARC_GET_ORIGINAL_SIZE			58	/* UnlhaGetOriginalSize */
#define ISARC_GET_COMPRESSED_SIZE		59	/* UnlhaGetCompressedSize */
#define ISARC_GET_RATIO					60	/* UnlhaGetRatio */
#define ISARC_GET_DATE					61	/* UnlhaGetDate */
#define ISARC_GET_TIME					62	/* UnlhaGetTime */
#define ISARC_GET_CRC					63	/* UnlhaGetCRC */
#define ISARC_GET_ATTRIBUTE				64	/* UnlhaGetAttribute */
#define ISARC_GET_OS_TYPE				65	/* UnlhaGetOSType */
#define ISARC_GET_METHOD				66	/* UnlhaGetMethod */
#define ISARC_GET_WRITE_TIME			67	/* UnlhaGetWriteTime */
#define ISARC_GET_CREATE_TIME			68	/* UnlhaGetCreateTime */
#define ISARC_GET_ACCESS_TIME			69	/* UnlhaGetAccessTime */
#define ISARC_GET_WRITE_TIME_EX			70	/* UnlhaGetWriteTimeEx */
#define ISARC_GET_CREATE_TIME_EX		71	/* UnlhaGetCreateTimeEx */
#define ISARC_GET_ACCESS_TIME_EX		72	/* UnlhaGetAccessTimeEx */
#define ISARC_SET_ENUM_MEMBERS_PROC		80  /* UnlhaSetEnumMembersProc */
#define ISARC_CLEAR_ENUM_MEMBERS_PROC	81	/* UnlhaClearEnumMembersProc */
#define ISARC_GET_ARC_FILE_SIZE_EX		82	/* UnlhaGetArcFileSizeEx */
#define ISARC_GET_ARC_ORIGINAL_SIZE_EX	83	/* UnlhaArcOriginalSizeEx */
#define ISARC_GET_ARC_COMPRESSED_SIZE_EX	84	/* UnlhaGetArcCompressedSizeEx */
#define ISARC_GET_ORIGINAL_SIZE_EX		85	/* UnlhaGetOriginalSizeEx */
#define ISARC_GET_COMPRESSED_SIZE_EX	86	/* UnlhaGetCompressedSizeEx */
#define ISARC_SETOWNERWINDOWEX64		87	/* UnlhaSetOwnerWindowEx64 */
#define ISARC_KILLOWNERWINDOWEX64		88	/* UnlhaKillOwnerWindowEx64 */
#define ISARC_SET_ENUM_MEMBERS_PROC64	89  /* UnlhaSetEnumMembersProc64 */
#define ISARC_CLEAR_ENUM_MEMBERS_PROC64	90	/* UnlhaClearEnumMembersProc64 */
#define ISARC_OPEN_ARCHIVE2				91	/* UnlhaOpenArchive2 */
#define ISARC_GET_ARC_READ_SIZE			92	/* UnlhaGetArcReadSize */
#define ISARC_GET_ARC_READ_SIZE_EX		93	/* UnlhaGetArcReadSizeEx*/
#define ISARC_FUNCTION_END				ISARC_GET_ARC_READ_SIZE_EX


#endif	/* ISARC_FUNCTION_START */

BOOL WINAPI _export UnlhaQueryFunctionList(const int _iFunction);

#ifndef WM_ARCEXTRACT
#define	WM_ARCEXTRACT	"wm_arcextract"

#define	ARCEXTRACT_BEGIN		0	/* 該当ファイルの処理の開始 */
#define	ARCEXTRACT_INPROCESS	1	/* 該当ファイルの展開中 */
#define	ARCEXTRACT_END			2	/* 処理終了、関連メモリを開放 */
#define ARCEXTRACT_OPEN			3	/* 該当書庫の処理の開始 */
#define ARCEXTRACT_COPY			4	/* ワークファイルの書き戻し */

BOOL WINAPI _export UnlhaSetOwnerWindow(const HWND _hwnd);
BOOL WINAPI _export UnlhaClearOwnerWindow(VOID);

BOOL WINAPI _export UnlhaSetOwnerWindowEx(const HWND _hwnd,
		LPARCHIVERPROC _lpArcProc);
BOOL WINAPI _export UnlhaKillOwnerWindowEx(const HWND _hwnd);
BOOL WINAPI _export UnlhaSetOwnerWindowEx64(const HWND _hwnd,
		LPARCHIVERPROC _lpArcProc, DWORD _dwStructSize);
BOOL WINAPI _export UnlhaKillOwnerWindowEx64(const HWND _hwnd);
#endif

/* OpenArchive 系 API です。 */

#if !defined(EXTRACT_FOUND_FILE)
/* MODE (for UnarjOpenArchive) */
#define M_INIT_FILE_USE			0x00000001L	/* レジストリの設定を使用 */
#define M_REGARDLESS_INIT_FILE	0x00000002L	/* 〃 を使用しない */
#define M_NO_BACKGROUND_MODE	0x00000004L	/* バックグラウンドを禁止 */
#define M_NOT_USE_TIME_STAMP	0x00000008L
#define M_EXTRACT_REPLACE_FILE	0x00000010L
#define M_EXTRACT_NEW_FILE		0x00000020L
#define M_EXTRACT_UPDATE_FILE	0x00000040L
#define M_CHECK_ALL_PATH		0x00000100L	/* 厳格なファイル名サーチ */
#define M_CHECK_FILENAME_ONLY	0x00000200L	/* 〃を行わない */
#define M_CHECK_DISK_SIZE		0x00000400L
#define M_REGARDLESS_DISK_SIZE	0x00000800L
#define M_USE_DRIVE_LETTER		0x00001000L	/* ドライブ名から格納 */
#define M_NOT_USE_DRIVE_LETTER	0x00002000L	/* 〃 を格納しない */
#define M_INQUIRE_DIRECTORY		0x00004000L
#define M_NOT_INQUIRE_DIRECTORY 0x00008000L
#define M_INQUIRE_WRITE			0x00010000L
#define M_NOT_INQUIRE_WRITE		0x00020000L
#define M_CHECK_READONLY		0x00040000L
#define M_REGARDLESS_READONLY	0x00080000L
#define M_REGARD_E_COMMAND		0x00100000L
#define M_REGARD_X_COMMAND		0x00200000L
#define M_ERROR_MESSAGE_ON		0x00400000L	/* エラーメッセージを表示 */
#define M_ERROR_MESSAGE_OFF		0x00800000L	/* 〃を表示しない */
#define M_BAR_WINDOW_ON			0x01000000L
#define M_BAR_WINDOW_OFF		0x02000000L
#define M_CHECK_PATH			0x04000000L
#define M_RECOVERY_ON			0x08000000L /* 破損ヘッダの読み飛ばし */

#define M_MAKE_INDEX_FILE		0x10000000L
#define M_NOT_MAKE_INDEX_FILE	0x20000000L
#define EXTRACT_FOUND_FILE		0x40000000L	/* 検索されたファイルを解凍 */
#define EXTRACT_NAMED_FILE		0x80000000L	/* 指定したファイルを解凍 */
#endif /* EXTRACT_FOUND_FILE */

#if !defined(SFX_NOT)
#define SFX_NOT					0	/* 通常の書庫 */

#define SFX_DOS_S				1		/* LHA's SFX 系 (small) */
#define SFX_DOS_204S			1		/* LHA's SFX 2.04S 以降 */
#define SFX_DOS_213S			1		/* LHA's SFX 2.04S 以降 */
#define SFX_DOS_250S			2		/* LHA's SFX 2.50S 以降 */
#define SFX_DOS_260S			3		/* LHA's SFX 2.60S 以降 */
#define SFX_DOS_265S			3		/* LHA's SFX 2.60S 以降 */

#define SFX_DOS_L				51		/* LHA's SFX 系 (large) */
#define SFX_DOS_204L			51		/* LHA's SFX 2.04L 以降 */
#define SFX_DOS_213L			51		/* LHA's SFX 2.04L 以降 */
#define SFX_DOS_250L			52		/* LHA's SFX 2.50L 以降 */
#define SFX_DOS_260L			53		/* LHA's SFX 2.60L 以降 */
#define SFX_DOS_265L			53		/* LHA's SFX 2.60L 以降 */

#define SFX_DOS_LARC			201		/* SFX by LARC 系 */
#define SFX_DOS_LARC_S			201		/* SFX by LARC (small) */
#define SFX_DOS_LARC_L			202		/* SFX by LARC (large) */

#define SFX_DOS_LHARC			301		/* LHarc's SFX 系 */
#define SFX_DOS_LHARC_S			301		/* LHarc's SFX (small) */
#define SFX_DOS_LHARC_L			351		/* LHarc's SFX (large) */

#define SFX_WIN16_213			1001	/* LHA's SFX 2.13.w16 系 */
#define SFX_WIN16_213_1			1001	/* WinSFX 2.13.w16.1 */
#define SFX_WIN16_213_2			1002	/* WinSFX 2.13.w16.2 */
#define SFX_WIN16_213_3			1003	/* WinSFX 2.13.w16.3 以降 */

#define SFX_WIN16_250			1011	/* LHA's SFX 2.50.w16 系 */
#define SFX_WIN16_250_1			1011	/* WinSFXM 2.50.w16.0001 以降 */
#define SFX_WIN16_255_1			1021	/* WinSFXM 2.55.w16.0001 以降 */

#define SFX_WIN32_213			2001	/* LHA's SFX 2.13.w32 系 */
#define SFX_WIN32_213_1			2001	/* WinSFX32 2.13.w32.1 以降 */
#define SFX_WIN32_213_3			2002	/* WinSFX32 2.13.w32.3 以降 */

#define SFX_WIN32_214_1			2003	/* WinSFX32 2.14.w32.0001 以降 */

#define SFX_WIN32_250			2011	/* LHA's SFX 2.50.w32 系 */
#define SFX_WIN32_250_1			2011	/* WinSFX32M 2.50.w32.0001 以降 */
#define SFX_WIN32_250_6			2012	/* WinSFX32M 2.50.w32.0006 以降 */

#define SFX_LZHSFX				2051	/* LZHSFX 系 */
#define SFX_LZHSFX_1002			2051	/* LZHSFX 1.0.0.2 以降 */
#define SFX_LZHSFX_1100			2052	/* LZHSFX 1.1.0.0 以降 */

#define SFX_LZHAUTO				2101	/* LZHAUTO 系 */
#define SFX_LZHAUTO_0002		2101	/* LZHAUTO 0.0.0.2 以降 */
#define SFX_LZHAUTO_1000		2102	/* LZHAUTO 1.0.0.0 以降 */
#define SFX_LZHAUTO_1002		2102	/* LZHAUTO 1.0.0.0 以降 */
#define SFX_LZHAUTO_1100		2103	/* LZHAUTO 1.1.0.0 以降 */

#define SFX_WIN32_LHASA			3001	/* Lhasa インストーラ */

#define SFX_DOS_UNKNOWN			9901	/* 認識できない DOS SFX */
#define SFX_WIN16_UNKNOWN		9911	/* 認識できない Win16 SFX */
#define SFX_WIN32_UNKNOWN		9921	/* 認識できない Win32 SFX */
#endif

HARC WINAPI _export UnlhaOpenArchive(const HWND _hwnd, LPCSTR _szFileName,
							const DWORD _dwMode);
HARC WINAPI _export UnlhaOpenArchive2(const HWND _hwnd, LPCSTR _szFileName,
							const DWORD _dwMode, LPCSTR _szOption);
int WINAPI _export UnlhaCloseArchive(HARC _harc);
int WINAPI _export UnlhaFindFirst(HARC _harc, LPCSTR _szWildName,
							INDIVIDUALINFO *_lpSubInfo);
int WINAPI _export UnlhaFindNext(HARC _harc, INDIVIDUALINFO *_lpSubInfo);
int WINAPI _export UnlhaGetArcFileName(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
DWORD WINAPI _export UnlhaGetArcFileSize(HARC _harc);
DWORD WINAPI _export UnlhaGetArcOriginalSize(HARC _harc);
DWORD WINAPI _export UnlhaGetArcCompressedSize(HARC _harc);
BOOL WINAPI _export UnlhaGetArcFileSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export UnlhaGetArcOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export UnlhaGetArcCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
DWORD WINAPI _export UnlhaGetArcReadSize(HARC _harc);
BOOL WINAPI _export UnlhaGetArcReadSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
WORD WINAPI _export UnlhaGetArcRatio(HARC _harc);
WORD WINAPI _export UnlhaGetArcDate(HARC _harc);
WORD WINAPI _export UnlhaGetArcTime(HARC _harc);
BOOL WINAPI _export UnlhaGetArcWriteTimeEx(HARC _harc,
							FILETIME *_lpftLastWriteTime);
BOOL WINAPI _export UnlhaGetArcCreateTimeEx(HARC _harc,
							FILETIME *_lpftCreationTime);
BOOL WINAPI _export UnlhaGetArcAccessTimeEx(HARC _harc,
							FILETIME *_lpftLastAccessTime);
UINT WINAPI _export UnlhaGetArcOSType(HARC _harc);
int WINAPI _export UnlhaIsSFXFile(HARC _harc);
int WINAPI _export UnlhaGetFileName(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
int WINAPI _export UnlhaGetMethod(HARC _harc, LPSTR _lpBuffer,
							const int _nSize);
DWORD WINAPI _export UnlhaGetOriginalSize(HARC _harc);
DWORD WINAPI _export UnlhaGetCompressedSize(HARC _harc);
BOOL WINAPI _export UnlhaGetOriginalSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
BOOL WINAPI _export UnlhaGetCompressedSizeEx(HARC _harc, ULHA_INT64 *_lpllSize);
WORD WINAPI _export UnlhaGetRatio(HARC _harc);
WORD WINAPI _export UnlhaGetDate(HARC _harc);
WORD WINAPI _export UnlhaGetTime(HARC _harc);
DWORD WINAPI _export UnlhaGetWriteTime(HARC _harc);
DWORD WINAPI _export UnlhaGetAccessTime(HARC _harc);
DWORD WINAPI _export UnlhaGetCreateTime(HARC _harc);
BOOL WINAPI _export UnlhaGetWriteTimeEx(HARC _harc,
							FILETIME *_lpftLastWriteTime);
BOOL WINAPI _export UnlhaGetCreateTimeEx(HARC _harc,
							FILETIME *_lpftCreationTime);
BOOL WINAPI _export UnlhaGetAccessTimeEx(HARC _harc,
							FILETIME *_lpftLastAccessTime);
DWORD WINAPI _export UnlhaGetCRC(HARC _harc);
int WINAPI _export UnlhaGetAttribute(HARC _harc);
UINT WINAPI _export UnlhaGetOSType(HARC _harc);

#ifndef FA_RDONLY
/* Attribute */
#define FA_RDONLY       0x01            /* 書き込み保護属性 */
#define FA_HIDDEN       0x02            /* 隠し属性 */
#define FA_SYSTEM       0x04            /* システム属性 */
#define FA_LABEL        0x08            /* ボリューム・ラベル */
#define FA_DIREC        0x10            /* ディレクトリ */
#define FA_ARCH         0x20            /* アーカイブ属性 */
#endif

#if !defined(SFXFLAG_AUTO)
#define SFXFLAG_AUTO			0x0001
#define SFXFLAG_OVERWRITE		0x0002
#define SFXFLAG_CHECKCRC		0x0004
#define SFXFLAG_CHECKTIME		0x0008
#define SFXFLAG_ATTRIBUTE		0x0010
#define SFXFLAG_NO_AUTO			0x0100
#define SFXFLAG_NO_OVERWRITE	0x0200
#define SFXFLAG_NO_CHECKCRC		0x0400
#define SFXFLAG_NO_CHECKTIME	0x0800
#define SFXFLAG_NO_ATTRIBUTE	0x1000
#define SFXFLAG_NO_WAIT			0x2000
#endif

#if !defined(BPL_NORMAL)
#define BPL_NORMAL					0
#define BPL_DENY_TOO_MANY_PARENTS	1
#define BPL_DENY_ABS_PATH			2
#define BPL_DENY_PARENTS			4
#define BPL_DENY_LEVEL_0			0
#define BPL_DENY_LEVEL_1			1
#define BPL_DENY_LEVEL_2			3
#define BPL_DENY_LEVEL_3			7
#endif

#if !defined(ERROR_START)
#define ERROR_START				0x8000
/* WARNING */
#define ERROR_DISK_SPACE		0x8005
#define ERROR_READ_ONLY			0x8006
#define ERROR_USER_SKIP			0x8007
#define ERROR_UNKNOWN_TYPE		0x8008
#define ERROR_METHOD			0x8009
#define ERROR_PASSWORD_FILE		0x800A
#define ERROR_VERSION			0x800B
#define ERROR_FILE_CRC			0x800C
#define ERROR_FILE_OPEN			0x800D
#define ERROR_MORE_FRESH		0x800E
#define ERROR_NOT_EXIST			0x800F
#define ERROR_ALREADY_EXIST		0x8010

#define ERROR_TOO_MANY_FILES	0x8011

/* ERROR */
#define ERROR_MAKEDIRECTORY		0x8012
#define ERROR_CANNOT_WRITE		0x8013
#define ERROR_HUFFMAN_CODE		0x8014
#define ERROR_COMMENT_HEADER	0x8015
#define ERROR_HEADER_CRC		0x8016
#define ERROR_HEADER_BROKEN		0x8017
#define ERROR_ARC_FILE_OPEN		0x8018
#define ERROR_NOT_ARC_FILE		0x8019
#define ERROR_CANNOT_READ		0x801A
#define ERROR_FILE_STYLE		0x801B
#define ERROR_COMMAND_NAME		0x801C
#define ERROR_MORE_HEAP_MEMORY	0x801D
#define ERROR_ENOUGH_MEMORY		0x801E
#if !defined(ERROR_ALREADY_RUNNING)
#define ERROR_ALREADY_RUNNING	0x801F
#endif
#define ERROR_USER_CANCEL		0x8020
#define ERROR_HARC_ISNOT_OPENED	0x8021
#define ERROR_NOT_SEARCH_MODE	0x8022
#define ERROR_NOT_SUPPORT		0x8023
#define ERROR_TIME_STAMP		0x8024
#define ERROR_TMP_OPEN			0x8025
#define ERROR_LONG_FILE_NAME	0x8026
#define ERROR_ARC_READ_ONLY		0x8027
#define ERROR_SAME_NAME_FILE	0x8028
#define ERROR_NOT_FIND_ARC_FILE 0x8029
#define ERROR_RESPONSE_READ		0x802A
#define ERROR_NOT_FILENAME		0x802B
#define ERROR_TMP_COPY			0x802C
#define ERROR_EOF				0x802D
#define ERROR_ADD_TO_LARC		0x802E
#define ERROR_TMP_BACK_SPACE	0x802F
#define ERROR_SHARING			0x8030
#define ERROR_NOT_FIND_FILE		0x8031
#define ERROR_LOG_FILE			0x8032
#define	ERROR_NO_DEVICE			0x8033
#define ERROR_GET_ATTRIBUTES	0x8034
#define ERROR_SET_ATTRIBUTES	0x8035
#define ERROR_GET_INFORMATION	0x8036
#define ERROR_GET_POINT			0x8037
#define ERROR_SET_POINT			0x8038
#define ERROR_CONVERT_TIME		0x8039
#define ERROR_GET_TIME			0x803a
#define ERROR_SET_TIME			0x803b
#define ERROR_CLOSE_FILE		0x803c
#define ERROR_HEAP_MEMORY		0x803d
#define ERROR_HANDLE			0x803e
#define ERROR_TIME_STAMP_RANGE	0x803f
#define ERROR_MAKE_ARCHIVE		0x8040
#define ERROR_NOT_CONFIRM_NAME	0x8041
#define ERROR_UNEXPECTED_EOF	0x8042
#define ERROR_INVALID_END_MARK	0x8043
#define ERROR_INVOLVED_LZH		0x8044
#define ERROR_NO_END_MARK		0x8045
#define ERROR_HDR_INVALID_SIZE	0x8046
#define ERROR_UNKNOWN_LEVEL		0x8047
#define ERROR_BROKEN_DATA		0x8048
#define ERROR_INVALID_PATH		0x8049

#define ERROR_END	ERROR_INVALID_PATH
#endif /* ERROR_START */

/* ### UNLHA32.DLL 独自の API です。### */
#define UNLHA_LIST_COMMAND		1
#define UNLHA_ADD_COMMAND		2
#define UNLHA_FRESH_COMMAND		3
#define UNLHA_DELETE_COMMAND	4
#define UNLHA_EXTRACT_COMMAND	5
#define UNLHA_PRINT_COMMAND		6
#define UNLHA_TEST_COMMAND		7
#define UNLHA_MAKESFX_COMMAND	8
#define UNLHA_JOINT_COMMAND		9
#define UNLHA_CONVERT_COMMAND	10
#define UNLHA_RENAME_COMMAND	11

WORD WINAPI _export UnlhaGetSubVersion(VOID);

int WINAPI _export UnlhaExtractMem(const HWND _hwndParent,
		LPCSTR _szCmdLine, LPBYTE _lpBuffer, const DWORD _dwSize,
		time_t *_lpTime, LPWORD _lpwAttr, LPDWORD _lpdwWriteSize);
int WINAPI _export UnlhaCompressMem(const HWND _hwndParent,
		LPCSTR _szCmdLine, const LPBYTE _lpBuffer, const DWORD _dwSize,
		const time_t *_lpTime, const LPWORD _lpwAttr,
		LPDWORD _lpdwWriteSize);

BOOL WINAPI _export UnlhaSetEnumMembersProc(UNLHA_WND_ENUMMEMBPROC
		_lpEnumProc);
BOOL WINAPI _export UnlhaClearEnumMembersProc(VOID);
BOOL WINAPI _export UnlhaSetEnumMembersProc64(UNLHA_WND_ENUMMEMBPROC
		_lpEnumProc, DWORD _dwStructSize);
BOOL WINAPI _export UnlhaClearEnumMembersProc64(VOID);

#define STID_FONTGOTICK			101
#define STID_FONTPGOTICK		102
#define STID_REG_STANDARD		103

#define STID_UNLHA32DLG1		1001
#define STID_UNLHA32DLG2		1002
#define STID_UNLHA32DLG3		1003
#define STID_UNLHA32DLG4		1004

#define STID_GETFNAMEDLG		1011
#define STID_GETCMTDLG			1012
#define STID_GETWINSFXDLG		1013
#define STID_UNLHA32OWDLG		1014
#define STID_UNLHA32MKDIRDLG	1015

#define STID_DLG_T_PROC			1101
#define STID_DLG_P_ARCNAME		1102
#define STID_DLG_P_FILENAME		1103
#define STID_DLG_P_EXTRACTDIR	1104
#define STID_DLG_P_WRITESIZE	1105
#define STID_DLG_P_CANCEL		1106

#define STID_DLG_T_LOCALSETTING	1111
#define STID_DLG_P_HIGH_MATCH	1112
#define STID_DLG_P_MAKE_DIR		1113
#define STID_DLG_P_CHK_SPACE	1114
#define STID_DLG_P_TOTAL		1115
#define STID_DLG_P_MINI_DLG		1116
#define STID_DLG_P_FLUSH		1117
#define STID_DLG_P_OLD_LOG		1118
#define STID_DLG_P_OLD_GF		1119
#define STID_DLG_P_OK			1120

#define STID_DLG_T_OWDIALOG		1131
#define STID_DLG_P_OWGROUP		1132
#define STID_DLG_P_OWOVWRITE	1133
#define STID_DLG_P_OWNOTOW		1134
#define STID_DLG_P_OWALLOW		1135
#define STID_DLG_P_OWALLNOT		1136
#define STID_DLG_P_BREAK		1137

#define STID_DLG_T_CHGFNAME		1141
#define STID_DLG_P_FNAMETITLE	1142

#define STID_DLG_T_CDDIALOG		1151
#define STID_DLG_P_CDCREATE		1152
#define STID_DLG_P_CDNOTCREATE	1153
#define STID_DLG_P_CDALLCREATE	1154
#define STID_DLG_P_CDALLNOT		1155

#define STID_DLG_T_CMTDIALOG	1161

#define STID_DLG_T_WINSFXSET	1171
#define STID_DLG_P_WSWINTITLE	1172
#define STID_DLG_P_WSDIRTITLE	1173
#define STID_DLG_P_WSINITDIR	1174
#define STID_DLG_P_WSCOMMAND	1175
#define STID_DLG_P_WSSETATTR	1176
#define STID_DLG_P_WSSETPATH	1177
#define STID_DLG_P_WSIGNORETIME	1178
#define STID_DLG_P_WSAUTOOW		1179
#define STID_DLG_P_WSAUTORUN	1180
#define STID_DLG_P_WSTAMPERCHK	1181

#define STID_DLG_T_SETTING		1191
#define STID_DLG_P_SUBASEDIR	1192
#define STID_DLG_P_SUFORCE_E	1193
#define STID_DLG_P_SUSEL_DIR	1194
#define STID_DLG_P_SUABS_DIR	1195
#define STID_DLG_P_SUREL_DIR	1196
#define STID_DLG_P_SUSEL_OW		1197
#define STID_DLG_P_SUALW_OW		1198
#define STID_DLG_P_SUQUE_OW		1199
#define STID_DLG_P_SUNOT_OW		1200
#define STID_DLG_P_SUATR_MODE	1201
#define STID_DLG_P_SULOCAL_SET	1202
#define STID_DLG_P_SUSAVE_ENV	1203
#define STID_DLG_P_SUSEL_PATH	1204
#define STID_DLG_P_SUDNY_NO		1205
#define STID_DLG_P_SUDNY_PAR	1206
#define STID_DLG_P_SUDNY_ABS	1207

#define STID_DLG_T_BPDIALOG		1211
#define STID_DLG_P_BPCHGNAME	1212
#define STID_DLG_P_BPNOT		1213
#define STID_DLG_P_BPALLCHG		1214

#define STID_DLG_T_APDIALOG		1221
#define STID_DLG_P_APRELATIVE	1222
#define STID_DLG_P_APABS		1223
#define STID_DLG_P_APINVALID	1224

#define STID_MES_T_UNLHA32_WAR	2001
#define STID_MES_T_UNLHA32_ERR	2002

#define STID_MES_T_PROC			2011
#define STID_MES_T_COMPRESS		2012
#define STID_MES_P_PROCFILE		2013
#define STID_MES_P_ADDFILE		2014
#define STID_MES_P_WORKFILE		2015

#define STID_MES_P_FILECOPY		2021

#define STID_MES_T_DELETE		2101
#define STID_MES_P_DELETE		2102
#define STID_MES_P_OVERWRITE	2104
#define STID_MES_T_ROWRITE		2105
#define STID_MES_P_ROWRITE		2106
#define STID_MES_P_CRCCHECK		2107
#define STID_MES_T_CRCERR		2108
#define STID_MES_P_CRCERR		2109
#define STID_MES_T_CRCERR2		2110
#define STID_MES_P_CRCERR2		2111
#define STID_MES_T_BADHUF		2112
#define STID_MES_P_BADHUF		2113
#define STID_MES_T_NAMECHANGE	2114
#define STID_MES_P_NAMECHANGE	2115
#define STID_MES_P_SHARE		2116
#define STID_MES_T_MAKEDIR		2117
#define STID_MES_P_MAKEDIR		2118
#define STID_MES_T_CONTINUE		2119
#define STID_MES_P_CONTINUE		2120
#define STID_MES_T_BADARC		2121
#define STID_MES_P_BADARC		2122
#define STID_MES_T_CONFIRM		2123
#define STID_MES_P_DICSIZ_WAR	2124
#define STID_MES_P_DISKSPACE	2125
#define STID_MES_T_SFXSET32		2126
#define STID_LOG_WAR_EXTDATA	2127
#define STID_LOG_WAR_SFX		2128
#define STID_MES_P_BADPATH		2129
#define STID_MES_P_ABSPATH		2130

#define STID_M_CANTOPEN			3001
#define STID_M_MAKEDIR			3002
#define STID_M_DIFFHOST			3003
#define STID_M_FEXISTS			3004
#define STID_M_FNEW				3005
#define STID_M_FOLD				3006
#define STID_M_FSAME			3007
#define STID_M_FNOEXIST			3008
#define STID_M_NOTSPCE			3009
#define STID_M_RDONLY			3010
#define STID_M_SPATTR			3011
#define STID_M_USKIP			3012
#define STID_M_WRITEER			3013
#define STID_M_DANNAME			3014
#define STID_M_SHARE			3015

#define STID_M_TOOLONG			3101
#define STID_M_TOOLARGE			3102
#define STID_M_UNKNMETH			3103
#define STID_M_BADPATH			3104

#define STID_M_ERRORCNT			3201
#define STID_M_NOSPCL			3202

#define STID_ERR_8005			9001
#define STID_ERR_8006			9002
#define STID_ERR_8007			9003
#define STID_ERR_8008			9004
#define STID_ERR_8009			9005
#define STID_ERR_800A			9006
#define STID_ERR_800B			9007
#define STID_ERR_800C			9008
#define STID_ERR_800D			9009
#define STID_ERR_800E			9010
#define STID_ERR_800F			9011
#define STID_ERR_8010			9012
#define STID_ERR_8011			9013

#define STID_ERR_8012			9014
#define STID_ERR_8013			9015
#define STID_ERR_8014			9016
#define STID_ERR_8015			9017
#define STID_ERR_8016			9018
#define STID_ERR_8017			9019
#define STID_ERR_8018			9020
#define STID_ERR_8019			9021
#define STID_ERR_801A			9022
#define STID_ERR_801B			9023
#define STID_ERR_801C			9024
#define STID_ERR_801D			9025
#define STID_ERR_801E			9026
#define STID_ERR_801F			9027
#define STID_ERR_8020			9028
#define STID_ERR_8021			9029
#define STID_ERR_8022			9030
#define STID_ERR_8023			9031
#define STID_ERR_8024			9032
#define STID_ERR_8025			9033
#define STID_ERR_8026			9034
#define STID_ERR_8027			9035
#define STID_ERR_8028			9036
#define STID_ERR_8029			9037
#define STID_ERR_802A			9038
#define STID_ERR_802B			9039
#define STID_ERR_802C			9040
#define STID_ERR_802D			9041
#define STID_ERR_802E			9042
#define STID_ERR_802F			9043
#define STID_ERR_8030			9044
#define STID_ERR_8031			9045
#define STID_ERR_8032			9046
#define STID_ERR_8033			9047
#define STID_ERR_8034			9048
#define STID_ERR_8035			9049
#define STID_ERR_8036			9050
#define STID_ERR_8037			9051
#define STID_ERR_8038			9052
#define STID_ERR_8039			9053
#define STID_ERR_803A			9054
#define STID_ERR_803B			9055
#define STID_ERR_803C			9056
#define STID_ERR_803D			9057
#define STID_ERR_803E			9058
#define STID_ERR_803F			9059
#define STID_ERR_8040			9060
#define STID_ERR_8041			9061
#define STID_ERR_8042			9062
#define STID_ERR_8043			9063
#define STID_ERR_8044			9064
#define STID_ERR_8045			9065
#define STID_ERR_8046			9066
#define STID_ERR_8047			9067
#define STID_ERR_8048			9068
#define STID_ERR_8049			9069

VOID WINAPI _export SetLangueSpecified(LANGID _wLanguage);
VOID WINAPI _export SetLangueJapanese(VOID);
VOID WINAPI _export SetLangueEnglish(VOID);
VOID WINAPI _export UnlhaSetLangueSpecified(LANGID _wLanguage);
VOID WINAPI _export UnlhaSetLangueJapanese(VOID);
VOID WINAPI _export UnlhaSetLangueEnglish(VOID);

#ifdef __cplusplus
}
#endif

#endif	/* UNLHA32_H */
