#ifndef VNNTOOL_H
#define VNNTOOL_H
//=============================================================================
//        Ås Generic Åt
//	vnn_app Common Function
//	File name   : VnnTool.H
//	Creater     : Tsuchiuchi
//	Created on  : 2017.11.27
//=============================================================================
// Modify

//=============================================================================
#include <stdio.h>

int GetPointChar(char *str,char *sfind);		// 2017.11.28 by tsuchi

// VnnGetVersion.cpp(need libraly 'version.lib'
void GetFileVersion(char* OutVersion);			// 2017.11.30 arai

int FindIndex(char* val, char find);


#endif
