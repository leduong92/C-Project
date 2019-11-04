#ifndef GET_INI
#define GET_INI
//=============================================================================
//        《 汎用 》
//	windows "iniファイル" 読み出し/設定関数
//	File name : GETNOW.H
//	技術本部  生産技術部
//	作成者 ： 小林
//	作成日 ： 1996.1.11
//=============================================================================
// 修正
// 2005.03.05 coba lTrim()追加
// 2005.10.25 coba フォルダを作成する関数MakeFolders()追加
// 2005.12.09 coba ローカル設定のINI(C:\NOKENV)から取得する関数を追加
// 2006.09.26 coba ローカル設定のINI(C:\NOKENV)へ書き込む関数を追加
// 2006.12.12 coba WriteNokIniString()で、NOKENVがないときは作成するように変更
// 2007.01.24 coba MakeFolders()のバグを修正
//=============================================================================
#ifdef __cplusplus
extern "C"
{
#endif
#include<windows.h>
#include<stdio.h>
#include <stdarg.h>
#include<commdlg.h>

int GetIniString( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName);
// 2005.12.09 追加
int GetNokIniString( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName);

#ifdef __cplusplus
void WriteIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName,BOOL execpathonly=FALSE);
#else
void WriteIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName,BOOL execpathonly);
#endif

// 2005.12.09 追加
void WriteNokIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName);

//----------------------------------------------------------------------
// 新しいAPIと名前がかぶってしまったので.....
#ifndef GetDefaultPrinterNOKSP
#define GetDefaultPrinterNOKSP
#undef GetDefaultPrinter
#define GetDefaultPrinter GetDefaultPrinterNOK
#endif
int GetDefaultPrinterNOK(char *theDrv, char *theDev,char *thePort);
//----------------------------------------------------------------------
BOOL	GetPrinterInfo(HWND hDlg,char *theDrv, char *theDev, char *thePort);
void rTrim( char *buf, unsigned int bufsize );
void lTrim( char *buf);
int zentohan(char*buf,int bufsize,char*str);
#ifdef __cplusplus
}
#endif

int MakeFolders( char* path );

#endif
