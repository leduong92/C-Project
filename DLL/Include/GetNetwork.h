#ifndef GETNWETWORK_H
#define GETNWETWORK_H
//----------------------------------------------------
//  iphlpapi.lib Winsk2.libを追加して実行すること
//
//----------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <stdlib.h>




int GetNICInfo( char *pIPAddress , char *pMACAddress=NULL);

#endif
