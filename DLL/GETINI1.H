#ifndef GET_INI
#define GET_INI
//=============================================================================
//	File name : GETNOW.H
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

int GetNokIniString( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName);

#ifdef __cplusplus
void WriteIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName,BOOL execpathonly=FALSE);
#else
void WriteIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName,BOOL execpathonly);
#endif


void WriteNokIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName);

//----------------------------------------------------------------------
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
