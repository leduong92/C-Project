#ifndef WIN_PRIN
#define WIN_PRIN
#include<windows.h>
#include<stdio.h>
#include <stdarg.h>

//#define w_printf WinPrintf

int WinPrintf(const char *format, ...);
void WpInit(void);


#endif
