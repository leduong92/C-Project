//=============================================================================
//        Ås Generic Åt
//	vnn_app Common Function
//	File name   : VnnTool.H
//	Creater     : Tsuchiuchi
//	Created on  : 2017.11.27
//=============================================================================
// Modify

//=============================================================================
#include "windows.h"
#include "VnnTool.h"

int GetPointChar(char *str,char *sfind)
{
	int i,vt;
	vt=0;
	char st[1];
	memset(st,0,sizeof(st));
	sprintf(st, "%s", (char*)sfind);
	for(i=0;i<(int)strlen(str);i++)
	{
		//if(!memcmp(str[i],"/",1))
		if(str[i]==st[0])
		{
			vt=i;
			break;
		}
	}
	return vt;
}

int FindIndex(char* val, char find)
{
	char *str = val;
	char *e;
	int index = 0;

	e = strchr(str, find);

	index = (int)(e - str);

	return index;
}
