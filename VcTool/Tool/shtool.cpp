//===================================================================================================================
//
//	FileName	: shtool.h
//				windowsシェル関係のちょっとしたユーティリティ
//				あまりにも古いwindowsでは動かないかも(shellのバージョン4.00(初期の95とか)は？)
//	作成日		:2003.07.16 by coba (ほばパクリ!!)
//===================================================================================================================
#include "shtool.h"

BOOL SaveShellLink( IShellLink *psl, const WCHAR *wcLinkName, BOOL bRemember );

//===================================================================================================================
//	関数名：BOOL GetSpecialFolder( HWND hwnd,int target,char* buf,int bufsize)
//	機能　：デスクトップ、スタートメニューなどの物理的なパスを取得する
//	引数１：ウインドウハンドル（APIがMessageBoxを出す場合があるので、そのときに親になるウインドウのハンドル）
//			コンソールなどの場合は、HWND_DESKTOP(=NULL)を指定するとよい
//	引数２：取得するフォルダのID、SHGetFolderLocation()のヘルプを見ましょう
//			よく使いそうなもの
//				CSIDL_DESKTOPDIRECTORY	ﾕｰｻﾞ別のデスクトップ
//				CSIDL_COMMON_DESKTOPDIRECTORY	全ﾕｰｻﾞ共通のデスクトップ
//				CSIDL_SARTMENU	ﾕｰｻﾞ別のスタートメニュー
//				CSIDL_COMMAN_SARTMENU	全ﾕｰｻﾞ共通のスタートメニュー
//	引数３：取得したパスを格納するバッファ。MAX_PATH+1は確保しましょう。
//	引数４：↑のサイズ
//===================================================================================================================
BOOL GetSpecialFolder( HWND hwnd,int target,char* buf,int bufsize)
{
	HRESULT hres;
	LPITEMIDLIST pid;
	char path[MAX_PATH+1];
	BOOL ret=FALSE;

	// shell-api
	hres= SHGetSpecialFolderLocation(NULL,target,&pid);

	if(SUCCEEDED(hres)){
		if( TRUE==SHGetPathFromIDList(pid,path)){
			memset( buf, 0, bufsize );
			memcpy( buf, path, (int)strlen(path)+1 > bufsize ? bufsize-1:strlen(path)+1);
			ret = TRUE;
		}
	}

	CoTaskMemFree(pid);			// 開放

	return(ret);
}

//===================================================================================================================
//	関数名：BOOL CreateShellLink( const char *LinkFile, const char *TargetFile, 
//											const char *Arguments, const char *WorkDir, int ShowCmd )
//	機能　：ショートカットファイルを作成する（パクリ）
//		引数の内容は、ショートカットのプロパティを見れば直感的に理解できると思います
//	引数１：ショートカットファイルの名前(*.lnk これを付与しておかないと、ショートカットになってくれない)
//	引数２：リンク先ファイルのフルパス。
//	引数３：コマンドライン引数
//	引数４：作業ディレクトリ。リンク先ファイルのフォルダを指定するのが一般的です。
//	引数５：実行時の大きさ。ShowWindwo()の引数を使います
//	戻り値：TRUE or FALSE
//===================================================================================================================
BOOL CreateShellLink( const char *LinkFile, const char *TargetFile, const char *Arguments, const char *WorkDir, int ShowCmd )
{
	IShellLink *psl;
	WCHAR       wcLinkFile[MAX_PATH+1];

	CoInitialize( NULL );
	if ( S_OK == CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl ) )
	{
		psl->SetPath( TargetFile );                           // リンク先を設定
		if ( Arguments ) psl->SetArguments( Arguments );      // コマンドライン引数を設定
		if ( WorkDir   ) psl->SetWorkingDirectory( WorkDir ); // 作業フォルダを設定
		psl->SetShowCmd( ShowCmd );                           // 実行時の大きさを設定

		MultiByteToWideChar( CP_ACP, 0, LinkFile, -1, wcLinkFile, MAX_PATH ); 
		if ( SaveShellLink( psl, wcLinkFile, TRUE ) ) // ショートカットの保存
		{
			psl->Release();
			CoUninitialize();
			return TRUE; // 成功
		}
		psl->Release();
	}
	CoUninitialize();
	return FALSE; // 失敗
}//CreateShellLink

//*********************************************************
// SaveShellLink
//*********************************************************
BOOL SaveShellLink( IShellLink *psl, const WCHAR *wcLinkName, BOOL bRemember )
{
	HRESULT       hResult;
	IPersistFile *ppf;

	if ( S_OK != psl->QueryInterface( IID_IPersistFile, (void **)&ppf ) )
		return FALSE;

	hResult = ppf->Save( wcLinkName, bRemember );
	ppf->Release();
	return S_OK == hResult;
}//SaveShellLink関連

