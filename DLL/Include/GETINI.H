#ifndef GET_INI
#define GET_INI

#ifdef __cplusplus
extern "C"
{
#endif
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <commdlg.h>

	int GetIniString(char *GetBuf, short BufSize, char *ProfileName, LPCTSTR SectionName, LPCTSTR KeyName);
	// 2005.12.09 í«â¡
	int GetNokIniString(char *GetBuf, short BufSize, char *ProfileName, LPCTSTR SectionName, LPCTSTR KeyName);

#ifdef __cplusplus
	void WriteIniString(char *data, char *ProfileName, LPCTSTR SectionName, LPCTSTR KeyName, BOOL execpathonly = FALSE);
#else
	void WriteIniString(char *data, char *ProfileName, LPCTSTR SectionName, LPCTSTR KeyName, BOOL execpathonly);
#endif

	// 2005.12.09 í«â¡
	void WriteNokIniString(char *data, char *ProfileName, LPCTSTR SectionName, LPCTSTR KeyName);

	//----------------------------------------------------------------------
	// êVÇµÇ¢APIÇ∆ñºëOÇ™Ç©Ç‘Ç¡ÇƒÇµÇ‹Ç¡ÇΩÇÃÇ≈.....
#ifndef GetDefaultPrinterNOKSP
#define GetDefaultPrinterNOKSP
#undef GetDefaultPrinter
#define GetDefaultPrinter GetDefaultPrinterNOK
#endif
	int GetDefaultPrinterNOK(char *theDrv, char *theDev, char *thePort);
	//----------------------------------------------------------------------
	BOOL	GetPrinterInfo(HWND hDlg, char *theDrv, char *theDev, char *thePort);
	void rTrim(char *buf, unsigned int bufsize);
	void lTrim(char *buf);
	int zentohan(char*buf, int bufsize, char*str);
#ifdef __cplusplus
}
#endif

int MakeFolders(char* path);

void SetDiv(char* buf, unsigned int bufsize, double val, int digits);//2016.03.09 yakuwa


#endif
