//#include "stdafx.h"

#include "GetIni.h"
//#include "winprint.h"

//=======================================================================================================
//int GetIniString( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
//========================================================================================================

int GetIniString( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	int i;
	char theDir[256];						
//WinPrintf("aa");
	memset( GetBuf, 0, BufSize );
	memset( theDir, 0, sizeof(theDir) );
//WinPrintf("bb");
	// *.iniƒt
	_snprintf(theDir,sizeof(theDir)-1,"%Fs",_pgmptr);			// "*.EXE
//WinPrintf("cc");
	i=strlen(theDir);						// "*.EXE"

	if( i > sizeof(theDir) || i < 0 ) return(-2);
	
	while(1){
		if( i<0) break;
		if( theDir[i]=='\\'|| i==0 ){
			theDir[i+1]=0;
			break;
		}
		i--;
	}

	strcat( theDir,ProfileName );			

	char  *RetStr= new char[6+BufSize];
	memset ( RetStr, 0, 6+BufSize );
//	memset ( RetStr, 0, sizeof(RetStr) );

	GetPrivateProfileString( SectionName, KeyName, "FALSE", 
								(LPTSTR)RetStr, (DWORD)(BufSize+6), (LPCTSTR)theDir );
//	if( memcmp( RetStr,"FALSE",5 )== 0 ) return(-1);

	if( memcmp( RetStr,"FALSE",5 )== 0 )
	{
		
		GetPrivateProfileString( SectionName, KeyName, "FALSE", 
								(LPTSTR)RetStr, (DWORD)(BufSize+6), (LPCTSTR)ProfileName );
		if( memcmp( RetStr,"FALSE",5 )== 0 ){		
			memcpy( GetBuf,"FALSE\0",BufSize >6 ? 6:BufSize );
			delete RetStr;
			return(-1);
		}
	}

	
	memcpy(GetBuf, RetStr, BufSize );
	delete RetStr;
	return(0);
}

int GetNokIniString( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	char theDir[256];						

	memset( GetBuf, 0, BufSize );
	memset( theDir, 0, sizeof(theDir) );

	// windows
	if( !GetWindowsDirectory( theDir,sizeof(theDir))) return(-1);
	theDir[3]=0;
	sprintf( &theDir[3],"NOKENV\\%s",ProfileName );		

	char  *RetStr= new char[6+BufSize];
	memset ( RetStr, 0, 6+BufSize );

	GetPrivateProfileString( SectionName, KeyName, "FALSE", 
								(LPTSTR)RetStr, (DWORD)(BufSize+6), (LPCTSTR)theDir );
	if( memcmp( RetStr,"FALSE",5 )== 0 ){
		delete RetStr;
		return(-2);
	}
	memcpy(GetBuf, RetStr, BufSize );
	delete RetStr;
	return(0);
}

void WriteIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName,BOOL execpathonly/*=FALSE*/)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	int i;
	char theDir[256];						

	memset( theDir, 0, sizeof(theDir) );
	memset( &fd, 0, sizeof(fd) );

	// *.init..
	sprintf(theDir,"%Fs",_pgmptr);			// "*.EXE‚

	i=strlen(theDir);						// "*.EXE"
	while(1){
		if( theDir[i]=='\\'|| i==0 ){
			theDir[i+1]=0;
			break;
		}
		i--;
	}

	wsprintf( &theDir[strlen(theDir)],"%s\0\0",ProfileName );			
	fd.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	hFind = FindFirstFile(theDir,&fd);
	if( hFind==INVALID_HANDLE_VALUE && execpathonly==FALSE)
		WritePrivateProfileString( SectionName,KeyName,data, ProfileName);

	else
		WritePrivateProfileString( SectionName,KeyName,data, theDir);

	FindClose(hFind);

}


void WriteNokIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	char theDir[256];					

	memset( theDir, 0, sizeof(theDir) );
	if( !GetWindowsDirectory( theDir,sizeof(theDir))) return;	// windowsÃ

	WIN32_FIND_DATA fd;
	HANDLE hFind;
	theDir[3]=0;

	sprintf( &theDir[3],"NOKENV\0\0" );
	memset( &fd, 0, sizeof(fd) );
	hFind = FindFirstFile( theDir, &fd);
	if( hFind == INVALID_HANDLE_VALUE){
		if(!CreateDirectory(theDir,NULL)){
			FindClose(hFind);
			return;
		}
	}
	FindClose(hFind);


	theDir[3]=0;
	sprintf( &theDir[3],"NOKENV\\%s",ProfileName );			

	WritePrivateProfileString( SectionName,KeyName,data, theDir);


}

int GetDefaultPrinter(char *theDrv, char *theDev, char *thePort)
{
	static PAGESETUPDLG psd;	
	DEVNAMES *dvn; 				
	int size;
	char lpDev[256];
	char lpDrv[256];
	char lpPort[256];

	// PAGESETUP
	memset( &psd,0, sizeof(psd));
	psd.Flags = PSD_RETURNDEFAULT;		
 	psd.hDevNames=NULL;
	psd.hDevMode=NULL;
	psd.lStructSize=sizeof(PAGESETUPDLG);
	psd.hwndOwner=NULL;

	if(!PageSetupDlg( &psd )) return(-1);

	if( psd.hDevNames == NULL ) return(-2);

	// DEVNAME
	dvn=(DEVNAMES*)GlobalLock(psd.hDevNames);
	size=dvn->wDeviceOffset;

	memset(lpDev,0,sizeof(lpDev) );
	strncpy( lpDev,(char*)dvn+dvn->wDeviceOffset,sizeof(lpDev) );

	memset(lpDrv,0,sizeof(lpDrv) );
	strncpy( lpDrv,(char*)dvn+dvn->wDriverOffset, sizeof(lpDrv) );

	memset(lpPort,0,sizeof(lpPort) );
	strncpy( lpPort,(char*)dvn+dvn->wOutputOffset,sizeof(lpPort) );

//	GlobalFree(psd.hDevMode);
	GlobalUnlock(psd.hDevNames);
	GlobalFree(psd.hDevNames);
//	GlobalFree(psd.hPageSetupTemplate);
//	GlobalUnlock(dvn);
//	GlobalFree(dvn);

	strncpy(theDrv,lpDrv,256);
	strncpy(theDev,lpDev,256);
	strncpy(thePort,lpPort,256);
	return(0);

}


BOOL GetPrinterInfo(HWND hDlg,char *theDrv, char *theDev, char *thePort)
{
	static PRINTDLG psd;	
	DEVNAMES *dvn; 				
	int size;
	char lpDev[256];
	char lpDrv[256];
	char lpPort[256];

	// PRINTDLG
	memset( &psd,0, sizeof(psd));
//	psd.Flags = PD_RETURNDEFAULT;		
	psd.Flags = PD_PRINTSETUP|PD_DISABLEPRINTTOFILE;

	psd.hDevNames=NULL;
	psd.hDevMode=NULL;
	psd.hwndOwner = hDlg;
	psd.lStructSize=sizeof(PRINTDLG);

	if(!PrintDlg( &psd )) return(FALSE);


	dvn=(DEVNAMES*)GlobalLock(psd.hDevNames);
	size=dvn->wDeviceOffset;

	memset(lpDev,0,sizeof(lpDev) );
	strncpy( lpDev,(char*)dvn+dvn->wDeviceOffset,sizeof(lpDev) );

	memset(lpDrv,0,sizeof(lpDrv) );
	strncpy( lpDrv,(char*)dvn+dvn->wDriverOffset, sizeof(lpDrv) );

	memset(lpPort,0,sizeof(lpPort) );
	strncpy( lpPort,(char*)dvn+dvn->wOutputOffset,sizeof(lpPort) );

	GlobalFree(psd.hDevMode);
	GlobalUnlock(psd.hDevNames);
	GlobalFree(psd.hDevNames);
//	GlobalFree(psd.hPageSetupTemplate);
//	GlobalUnlock(dvn);
//	GlobalFree(dvn);

	strncpy(theDrv,lpDrv,256);
	strncpy(theDev,lpDev,256);
	strncpy(thePort,lpPort,256);
	return(TRUE);

}

void rTrim( char *buf, unsigned int bufsize )
{
	int i;

	for(i=bufsize;i>0;i--){
		if( buf[i-1]!=0x20) break;
		if( buf[i-1]==0x20) buf[i-1] = 0;
	}
}

void lTrim(char* buf)
{
	int i,j,l;

	if( NULL==buf) return;
	l=strlen(buf);

	for(i=0,j=0;i<l;i++){
		if( buf[i]!=0x20){
			buf[j]=buf[i];
			j++;
		}
	}
	buf[j]=0;
}

int zentohan(char*buf,int bufsize,char*str){
	if( buf==NULL || str ==NULL )return 0;
	if( bufsize < 1 ) return 0;

	unsigned int code;
	BOOL ret;
	int i,j;
	unsigned char low;

	j=0;
	for(i=0;i<(int)strlen(str);i++){
		code = (unsigned int)str[i];
		ret=IsDBCSLeadByteEx(0,code);
		if( ret ){
			low=(unsigned char)str[i];
			if(low==0x82){
				i++;
				low=(unsigned char)str[i];
				if( low >=0x81 && low <=0x9a ){
					buf[j++]=low-0x20;
				}
				if( low >=0x60 && low <=0x79 ){
					buf[j++]=low-0x1f;
				}
				if( low >=0x4f && low <=0x58 ){
					buf[j++]=low-0x1f;
				}
			}
			//{*+-/(#=,.
			else if(low==0x81){
				i++;
				low=(unsigned char)str[i];
				switch(low){
					case 0x6f:
					case 0x69:
						buf[j++]='(';
						break;
					case 0x6a:
					case 0x70:
						buf[j++]=')';
						break;

					case 0x96:
					case 0x7e:
						buf[j++]='*';
						break;
					case 0x80:
					case 0x5e:
						buf[j++]='/';
						break;
					case 0x7b:
						buf[j++]='+';
						break;
					case 0x7c:
						buf[j++]='-';
						break;
					case 0x81:
						buf[j++]='=';
						break;
					case 0x94:
						buf[j++]='#';
						break;
					case 0x41:
					case 0x43:
						buf[j++]=',';
						break;
					case 0x42:
					case 0x44:
						buf[j++]='.';
						break;

					default:
						break;
	   			}
			}
			else i++;
		}
		else{
			buf[j++]=str[i];
		}
		if( j>= bufsize ) break;
	}
	return(j);
}

int MakeFolders( char* path )
{
	char fullDir[MAX_PATH+1];
	char tmpDir[MAX_PATH+1];
	WIN32_FIND_DATA fd;
	HANDLE hFind;
	int i;

	memset( fullDir , 0 , sizeof( fullDir ));
	if( NULL ==_fullpath(fullDir,path, MAX_PATH)) return(-1);

	strcpy( tmpDir,fullDir);
	while(1){
		hFind = FindFirstFile( fullDir, &fd);
		if( hFind != INVALID_HANDLE_VALUE && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY ){
			FindClose(hFind);
			break;
		}
		FindClose(hFind);

		if( CreateDirectory(tmpDir,NULL)){
			strcpy(tmpDir,fullDir);
		}
		else{
			for(i=strlen(tmpDir)-1;i>=0;i--){
				if( tmpDir[i]=='\\' ){
					tmpDir[i]=0;
					break;
				}
			}
			if( i==0 )	return(-2);
		}
	}
	return(0);
}
