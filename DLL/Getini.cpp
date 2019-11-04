#include "GetIni.h"
//#include "winprint.h"
//=======================================================================================================
//int GetIniString( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
//	実行ファイルと同じ場所にあるINIファイルから文字列を取得。無かったらWindowsディレクトリから取得。
//
//	引数１：取得バッファ
//	引数２：取得バッファ長(含むNull)
//	引数４：INIファイルの名前（拡張子つき。INIファイルは実行ディレクトリと同じ場所に作成して下さい)
//	引数５：INIファイル内のセクション名([]でくくられた名称)
//	引数４：INIファイル名のキー名
//			＊文字列なのですべて最後にNullを付加する事！！
//	返値　：成功＝０,失敗＝－１
//========================================================================================================
int GetIniString( char *GetBuf,short BufSize, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	int i;
	char theDir[256];						// プロファイル（*.INIファイル）サーチパス格納用
//WinPrintf("aa");
	memset( GetBuf, 0, BufSize );
	memset( theDir, 0, sizeof(theDir) );
//WinPrintf("bb");
	// *.iniファイルのパス作成
	_snprintf(theDir,sizeof(theDir)-1,"%Fs",_pgmptr);			// "*.EXEのフルパス取得
//WinPrintf("cc");
	i=strlen(theDir);						// "*.EXE"の部分をカット

	if( i > sizeof(theDir) || i < 0 ) return(-2);
	
	while(1){
		if( i<0) break;
		if( theDir[i]=='\\'|| i==0 ){
			theDir[i+1]=0;
			break;
		}
		i--;
	}

	strcat( theDir,ProfileName );			// 引数３のプロファイル名と結合

	char  *RetStr= new char[6+BufSize];
	memset ( RetStr, 0, sizeof(RetStr) );

	// まず指定されたディレクトリから探す。
	GetPrivateProfileString( SectionName, KeyName, "FALSE", 
								(LPTSTR)RetStr, (DWORD)(BufSize+6), (LPCTSTR)theDir );
//	if( memcmp( RetStr,"FALSE",5 )== 0 ) return(-1);

	// 失敗時はWINDOWSディレクトリから探す。
	if( memcmp( RetStr,"FALSE",5 )== 0 )
	{
		
		GetPrivateProfileString( SectionName, KeyName, "FALSE", 
								(LPTSTR)RetStr, (DWORD)(BufSize+6), (LPCTSTR)ProfileName );
		// それでもなっかた
		if( memcmp( RetStr,"FALSE",5 )== 0 ){		
			memcpy( GetBuf,"FALSE\0",BufSize >6 ? 6:BufSize );
			delete RetStr;
			return(-1);
		}
	}

	// 成功
	
	memcpy(GetBuf, RetStr, BufSize );
	delete RetStr;
	return(0);
}

//=======================================================================================================
//int WriteIniString( char *data,short datalen, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
//	exeと同じパスにあれば書き込み。無いときはWindowsディレクトリに書き込み
//
//	引数１：取得バッファ
//	引数４：INIファイルの名前（拡張子つき。INIファイルは実行ディレクトリと同じ場所に作成して下さい)
//	引数５：INIファイル内のセクション名([]でくくられた名称)
//	引数４：INIファイル名のキー名
//			＊文字列なのですべて最後にNullを付加する事！！
//	返値　：成功＝０,失敗＝－１
//========================================================================================================
void WriteIniString( char *data, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	int i;
	char theDir[256];						// プロファイル（*.INIファイル）サーチパス格納用

	memset( theDir, 0, sizeof(theDir) );
	memset( &fd, 0, sizeof(fd) );

	// *.iniファイルのパス作成
	sprintf(theDir,"%Fs",_pgmptr);			// "*.EXEのフルパス取得

	i=strlen(theDir);						// "*.EXE"の部分をカット
	while(1){
		if( theDir[i]=='\\'|| i==0 ){
			theDir[i+1]=0;
			break;
		}
		i--;
	}

	wsprintf( &theDir[strlen(theDir)],"%s\0\0",ProfileName );			// 引数３のプロファイル名と結合
	fd.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	hFind = FindFirstFile(theDir,&fd);
	// ファイルが無いときはwindriに書き込む
	if( hFind==INVALID_HANDLE_VALUE ){
		WritePrivateProfileString( SectionName,KeyName,data, ProfileName);
	}

	// ある時はそこへ
	else
		WritePrivateProfileString( SectionName,KeyName,data, theDir);

	FindClose(hFind);

}

//=======================================================================================================
//	int GetDefaultPrinter(char *theDrv, char *theDev, char *thePort)
//	通常使うプリンタについての情報を取得する。WindowsNt,Windows95どちらからの仕様も可能！！
//
//	引数１：ドライブ名取得バッファ（256バイト以上確保して）
//	引数２：デバイス名取得バッファ（32バイト確保して）
//	引数３：ポ－ト名取得バッファ（256バイト以上確保して）
//	返値　：成功＝０,失敗＝－１
//========================================================================================================

int GetDefaultPrinter(char *theDrv, char *theDev, char *thePort)
{
	static PAGESETUPDLG psd;		// ペ－ジ設定ダイアログ初期化構造体
	DEVNAMES *dvn; 					// プリンタ情報構造体
	int size;
	char lpDev[256];
	char lpDrv[256];
	char lpPort[256];

	// PAGESETUP構造体の初期化
	memset( &psd,0, sizeof(psd));
	psd.Flags = PSD_RETURNDEFAULT;		// ダイアログをオープンせずにデフォルトプリンタを取得
 	psd.hDevNames=NULL;
	psd.hDevMode=NULL;
	psd.lStructSize=sizeof(PAGESETUPDLG);
	psd.hwndOwner=NULL;

	// ダイアログを開いてデバイス・ポート・ドライバ名取得
	if(!PageSetupDlg( &psd )) return(-1);

	// DEVNAME構造体のメモリロック＆割り当て
	dvn=(DEVNAMES*)GlobalLock(psd.hDevNames);
	size=dvn->wDeviceOffset;

	// 各種データを取得する
	memset(lpDev,0,sizeof(lpDev) );
	strncpy( lpDev,(char*)dvn+dvn->wDeviceOffset,sizeof(lpDev) );

	memset(lpDrv,0,sizeof(lpDrv) );
	strncpy( lpDrv,(char*)dvn+dvn->wDriverOffset, sizeof(lpDrv) );

	memset(lpPort,0,sizeof(lpPort) );
	strncpy( lpPort,(char*)dvn+dvn->wOutputOffset,sizeof(lpPort) );

	// メモリ解放
	GlobalFree(psd.hDevMode);
	GlobalUnlock(psd.hDevNames);
	GlobalFree(psd.hDevNames);
	GlobalFree(psd.hPageSetupTemplate);
	GlobalUnlock(dvn);
	GlobalFree(dvn);

	// 出力
	strncpy(theDrv,lpDrv,256);
	strncpy(theDev,lpDev,256);
	strncpy(thePort,lpPort,256);
	return(0);

}



//---------------------------------------------------------------------------------------------------------------------
//	関数名：GetPrinterInfo(HWND hDlg,char *theDrv, char *theDev, char *thePort)
//	機能　：プリンタ設定ダイアログをひらき、印刷するプリンタ情報を取得する
//	引数　：ダイアログのウインドウハンドル
//	戻り値：入力された発行枚数 -1=入力エラー
//---------------------------------------------------------------------------------------------------------------------
BOOL GetPrinterInfo(HWND hDlg,char *theDrv, char *theDev, char *thePort)
{
	static PRINTDLG psd;		// ペ－ジ設定ダイアログ初期化構造体
	DEVNAMES *dvn; 					// プリンタ情報構造体
	int size;
	char lpDev[256];
	char lpDrv[256];
	char lpPort[256];

	// PRINTDLG構造体の初期化
	memset( &psd,0, sizeof(psd));
//	psd.Flags = PD_RETURNDEFAULT;		//オープンせずにデフォルトプリンタを取得
	psd.Flags = PD_PRINTSETUP|PD_DISABLEPRINTTOFILE;

	psd.hDevNames=NULL;
	psd.hDevMode=NULL;
	psd.hwndOwner = hDlg;
	psd.lStructSize=sizeof(PRINTDLG);

	if(!PrintDlg( &psd )) return(FALSE);


	dvn=(DEVNAMES*)GlobalLock(psd.hDevNames);
	size=dvn->wDeviceOffset;

	// 各種データを取得する
	memset(lpDev,0,sizeof(lpDev) );
	strncpy( lpDev,(char*)dvn+dvn->wDeviceOffset,sizeof(lpDev) );

	memset(lpDrv,0,sizeof(lpDrv) );
	strncpy( lpDrv,(char*)dvn+dvn->wDriverOffset, sizeof(lpDrv) );

	memset(lpPort,0,sizeof(lpPort) );
	strncpy( lpPort,(char*)dvn+dvn->wOutputOffset,sizeof(lpPort) );

	// メモリ解放
	GlobalFree(psd.hDevMode);
	GlobalUnlock(psd.hDevNames);
	GlobalFree(psd.hDevNames);
//	GlobalFree(psd.hPageSetupTemplate);
	GlobalUnlock(dvn);
	GlobalFree(dvn);

	// 出力
	strncpy(theDrv,lpDrv,256);
	strncpy(theDev,lpDev,256);
	strncpy(thePort,lpPort,256);
	return(TRUE);

}
// 文字の右スペースはずし
void rTrim( char *buf, unsigned int bufsize )
{
	int i;

	for(i=bufsize;i>0;i--){
		if( buf[i-1]!=0x20) break;
		if( buf[i-1]==0x20) buf[i-1] = 0;
	}
}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int zentohan(char*buf,int bufsize,char*str){
//	機能　：全角→半角変換（英数と一部記号のみ）
//	引数　：出力バッファ
//	引数　：出力バッファサイズ
//	引数　：変換元文字列
//	戻り値：コピーした文字数
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
			//ａ-ｚ(az),Ａ-Ｚ(AZ)
			if(low==0x82){
				i++;
				low=(unsigned char)str[i];
				// 全角ａ-ｚ
				if( low >=0x81 && low <=0x9a ){
					buf[j++]=low-0x20;
				}
				// 全角Ａ-Ｚ
				if( low >=0x60 && low <=0x79 ){
					buf[j++]=low-0x1f;
				}
				if( low >=0x4f && low <=0x58 ){
					buf[j++]=low-0x1f;
				}
			}
			//｛＊＋－／（＃=、．。
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
			// その他の全角
			else i++;
		}
		// 通常のhan角
		else{
			buf[j++]=str[i];
		}
		if( j>= bufsize ) break;
	}
	return(j);
}

