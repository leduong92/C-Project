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
//	2005.03.05 coba lTrim()追加
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

#ifdef __cplusplus
void WriteIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName,BOOL execpathonly=FALSE);
#else
void WriteIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName,BOOL execpathonly);
#endif
int GetDefaultPrinter(char *theDrv, char *theDev,char *thePort);
BOOL	GetPrinterInfo(HWND hDlg,char *theDrv, char *theDev, char *thePort);
void rTrim( char *buf, unsigned int bufsize );
void lTrim(char* buf);
int zentohan(char*buf,int bufsize,char*str);
#ifdef __cplusplus
}
#endif

#endif
