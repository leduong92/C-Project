#include "GetIni.h"
//#include "winprint.h"
//=======================================================================================================
//int GetIniString( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
//	���s�t�@�C���Ɠ����ꏊ�ɂ���INI�t�@�C�����當������擾�B����������Windows�f�B���N�g������擾�B
//
//	�����P�F�擾�o�b�t�@
//	�����Q�F�擾�o�b�t�@��(�܂�Null)
//	�����S�FINI�t�@�C���̖��O�i�g���q���BINI�t�@�C���͎��s�f�B���N�g���Ɠ����ꏊ�ɍ쐬���ĉ�����)
//	�����T�FINI�t�@�C�����̃Z�N�V������([]�ł�����ꂽ����)
//	�����S�FINI�t�@�C�����̃L�[��
//			��������Ȃ̂ł��ׂčŌ��Null��t�����鎖�I�I
//	�Ԓl�@�F�������O,���s���|�P
//========================================================================================================
int GetIniString( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	int i;
	char theDir[256];						// �v���t�@�C���i*.INI�t�@�C���j�T�[�`�p�X�i�[�p
//WinPrintf("aa");
	memset( GetBuf, 0, BufSize );
	memset( theDir, 0, sizeof(theDir) );
//WinPrintf("bb");
	// *.ini�t�@�C���̃p�X�쐬
	_snprintf(theDir,sizeof(theDir)-1,"%Fs",_pgmptr);			// "*.EXE�̃t���p�X�擾
//WinPrintf("cc");
	i=strlen(theDir);						// "*.EXE"�̕������J�b�g

	if( i > sizeof(theDir) || i < 0 ) return(-2);
	
	while(1){
		if( i<0) break;
		if( theDir[i]=='\\'|| i==0 ){
			theDir[i+1]=0;
			break;
		}
		i--;
	}

	strcat( theDir,ProfileName );			// �����R�̃v���t�@�C�����ƌ���

	char  *RetStr= new char[6+BufSize];
	memset ( RetStr, 0, sizeof(RetStr) );

	// �܂��w�肳�ꂽ�f�B���N�g������T���B
	GetPrivateProfileString( SectionName, KeyName, "FALSE", 
								(LPTSTR)RetStr, (DWORD)(BufSize+6), (LPCTSTR)theDir );
//	if( memcmp( RetStr,"FALSE",5 )== 0 ) return(-1);

	// ���s����WINDOWS�f�B���N�g������T���B
	if( memcmp( RetStr,"FALSE",5 )== 0 )
	{
		
		GetPrivateProfileString( SectionName, KeyName, "FALSE", 
								(LPTSTR)RetStr, (DWORD)(BufSize+6), (LPCTSTR)ProfileName );
		// ����ł��Ȃ�����
		if( memcmp( RetStr,"FALSE",5 )== 0 ){		
			memcpy( GetBuf,"FALSE\0",BufSize >6 ? 6:BufSize );
			delete RetStr;
			return(-1);
		}
	}

	// ����
	
	memcpy(GetBuf, RetStr, BufSize );
	delete RetStr;
	return(0);
}

//=======================================================================================================
//int WriteIniString( char *data,short datalen, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
//	exe�Ɠ����p�X�ɂ���Ώ������݁B�����Ƃ���Windows�f�B���N�g���ɏ�������
//
//	�����P�F�擾�o�b�t�@
//	�����S�FINI�t�@�C���̖��O�i�g���q���BINI�t�@�C���͎��s�f�B���N�g���Ɠ����ꏊ�ɍ쐬���ĉ�����)
//	�����T�FINI�t�@�C�����̃Z�N�V������([]�ł�����ꂽ����)
//	�����S�FINI�t�@�C�����̃L�[��
//			��������Ȃ̂ł��ׂčŌ��Null��t�����鎖�I�I
//	�Ԓl�@�F�������O,���s���|�P
//========================================================================================================
void WriteIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	int i;
	char theDir[256];						// �v���t�@�C���i*.INI�t�@�C���j�T�[�`�p�X�i�[�p

	memset( theDir, 0, sizeof(theDir) );
	memset( &fd, 0, sizeof(fd) );

	// *.ini�t�@�C���̃p�X�쐬
	sprintf(theDir,"%Fs",_pgmptr);			// "*.EXE�̃t���p�X�擾

	i=strlen(theDir);						// "*.EXE"�̕������J�b�g
	while(1){
		if( theDir[i]=='\\'|| i==0 ){
			theDir[i+1]=0;
			break;
		}
		i--;
	}

	wsprintf( &theDir[strlen(theDir)],"%s\0\0",ProfileName );			// �����R�̃v���t�@�C�����ƌ���
	fd.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	hFind = FindFirstFile(theDir,&fd);
	// �t�@�C���������Ƃ���windri�ɏ�������
	if( hFind==INVALID_HANDLE_VALUE ){
		WritePrivateProfileString( SectionName,KeyName,data, ProfileName);
	}

	// ���鎞�͂�����
	else
		WritePrivateProfileString( SectionName,KeyName,data, theDir);

	FindClose(hFind);

}

//=======================================================================================================
//	int GetDefaultPrinter(char *theDrv, char *theDev, char *thePort)
//	�ʏ�g���v�����^�ɂ��Ă̏����擾����BWindowsNt,Windows95�ǂ��炩��̎d�l���\�I�I
//
//	�����P�F�h���C�u���擾�o�b�t�@�i256�o�C�g�ȏ�m�ۂ��āj
//	�����Q�F�f�o�C�X���擾�o�b�t�@�i32�o�C�g�m�ۂ��āj
//	�����R�F�|�|�g���擾�o�b�t�@�i256�o�C�g�ȏ�m�ۂ��āj
//	�Ԓl�@�F�������O,���s���|�P
//========================================================================================================

int GetDefaultPrinter(char *theDrv, char *theDev, char *thePort)
{
	static PAGESETUPDLG psd;		// �y�|�W�ݒ�_�C�A���O�������\����
	DEVNAMES *dvn; 					// �v�����^���\����
	int size;
	char lpDev[256];
	char lpDrv[256];
	char lpPort[256];

	// PAGESETUP�\���̂̏�����
	memset( &psd,0, sizeof(psd));
	psd.Flags = PSD_RETURNDEFAULT;		// �_�C�A���O���I�[�v�������Ƀf�t�H���g�v�����^���擾
 	psd.hDevNames=NULL;
	psd.hDevMode=NULL;
	psd.lStructSize=sizeof(PAGESETUPDLG);
	psd.hwndOwner=NULL;

	// �_�C�A���O���J���ăf�o�C�X�E�|�[�g�E�h���C�o���擾
	if(!PageSetupDlg( &psd )) return(-1);

	// DEVNAME�\���̂̃��������b�N�����蓖��
	dvn=(DEVNAMES*)GlobalLock(psd.hDevNames);
	size=dvn->wDeviceOffset;

	// �e��f�[�^���擾����
	memset(lpDev,0,sizeof(lpDev) );
	strncpy( lpDev,(char*)dvn+dvn->wDeviceOffset,sizeof(lpDev) );

	memset(lpDrv,0,sizeof(lpDrv) );
	strncpy( lpDrv,(char*)dvn+dvn->wDriverOffset, sizeof(lpDrv) );

	memset(lpPort,0,sizeof(lpPort) );
	strncpy( lpPort,(char*)dvn+dvn->wOutputOffset,sizeof(lpPort) );

	// ���������
	GlobalFree(psd.hDevMode);
	GlobalUnlock(psd.hDevNames);
	GlobalFree(psd.hDevNames);
	GlobalFree(psd.hPageSetupTemplate);
	GlobalUnlock(dvn);
	GlobalFree(dvn);

	// �o��
	strncpy(theDrv,lpDrv,256);
	strncpy(theDev,lpDev,256);
	strncpy(thePort,lpPort,256);
	return(0);

}



//---------------------------------------------------------------------------------------------------------------------
//	�֐����FGetPrinterInfo(HWND hDlg,char *theDrv, char *theDev, char *thePort)
//	�@�\�@�F�v�����^�ݒ�_�C�A���O���Ђ炫�A�������v�����^�����擾����
//	�����@�F�_�C�A���O�̃E�C���h�E�n���h��
//	�߂�l�F���͂��ꂽ���s���� -1=���̓G���[
//---------------------------------------------------------------------------------------------------------------------
BOOL GetPrinterInfo(HWND hDlg,char *theDrv, char *theDev, char *thePort)
{
	static PRINTDLG psd;		// �y�|�W�ݒ�_�C�A���O�������\����
	DEVNAMES *dvn; 					// �v�����^���\����
	int size;
	char lpDev[256];
	char lpDrv[256];
	char lpPort[256];

	// PRINTDLG�\���̂̏�����
	memset( &psd,0, sizeof(psd));
//	psd.Flags = PD_RETURNDEFAULT;		//�I�[�v�������Ƀf�t�H���g�v�����^���擾
	psd.Flags = PD_PRINTSETUP|PD_DISABLEPRINTTOFILE;

	psd.hDevNames=NULL;
	psd.hDevMode=NULL;
	psd.hwndOwner = hDlg;
	psd.lStructSize=sizeof(PRINTDLG);

	if(!PrintDlg( &psd )) return(FALSE);


	dvn=(DEVNAMES*)GlobalLock(psd.hDevNames);
	size=dvn->wDeviceOffset;

	// �e��f�[�^���擾����
	memset(lpDev,0,sizeof(lpDev) );
	strncpy( lpDev,(char*)dvn+dvn->wDeviceOffset,sizeof(lpDev) );

	memset(lpDrv,0,sizeof(lpDrv) );
	strncpy( lpDrv,(char*)dvn+dvn->wDriverOffset, sizeof(lpDrv) );

	memset(lpPort,0,sizeof(lpPort) );
	strncpy( lpPort,(char*)dvn+dvn->wOutputOffset,sizeof(lpPort) );

	// ���������
	GlobalFree(psd.hDevMode);
	GlobalUnlock(psd.hDevNames);
	GlobalFree(psd.hDevNames);
//	GlobalFree(psd.hPageSetupTemplate);
	GlobalUnlock(dvn);
	GlobalFree(dvn);

	// �o��
	strncpy(theDrv,lpDrv,256);
	strncpy(theDev,lpDev,256);
	strncpy(thePort,lpPort,256);
	return(TRUE);

}
// �����̉E�X�y�[�X�͂���
void rTrim( char *buf, unsigned int bufsize )
{
	int i;

	for(i=bufsize;i>0;i--){
		if( buf[i-1]!=0x20) break;
		if( buf[i-1]==0x20) buf[i-1] = 0;
	}
}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint zentohan(char*buf,int bufsize,char*str){
//	�@�\�@�F�S�p�����p�ϊ��i�p���ƈꕔ�L���̂݁j
//	�����@�F�o�̓o�b�t�@
//	�����@�F�o�̓o�b�t�@�T�C�Y
//	�����@�F�ϊ���������
//	�߂�l�F�R�s�[����������
//---------------------------------------------------------------------------------------------------------------------
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
			//��-��(az),�`-�y(AZ)
			if(low==0x82){
				i++;
				low=(unsigned char)str[i];
				// �S�p��-��
				if( low >=0x81 && low <=0x9a ){
					buf[j++]=low-0x20;
				}
				// �S�p�`-�y
				if( low >=0x60 && low <=0x79 ){
					buf[j++]=low-0x1f;
				}
				if( low >=0x4f && low <=0x58 ){
					buf[j++]=low-0x1f;
				}
			}
			//�o���{�|�^�i��=�A�D�B
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
			// ���̑��̑S�p
			else i++;
		}
		// �ʏ��han�p
		else{
			buf[j++]=str[i];
		}
		if( j>= bufsize ) break;
	}
	return(j);
}

