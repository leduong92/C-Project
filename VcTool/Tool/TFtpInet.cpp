//=====================================================================================================================
//
//	FileName	:TFtpInet.h
//	Class		:TFtpInet
//				:WinInet.lib を使ったFTP送受信関数ｸﾗｽ
//  2006.07.05 coded by ﾏﾂﾀﾞ
//	2007.04.26 ﾏﾂﾀﾞ ライブラリのインクルードを自動的に行う#pragma comment(lib, "wininet.lib")を追加
//=====================================================================================================================
#include "TFtpInet.h"

#pragma comment(lib, "wininet.lib")
//---------------------------------------------------------------------------------------------------------------------
//	FtpInet::FtpInet(void)
//---------------------------------------------------------------------------------------------------------------------
TFtpInet::TFtpInet(void)
{
	m_FtpConnect = FALSE;
}
//---------------------------------------------------------------------------------------------------------------------
//	FtpInet::~FtpInet(void)
//---------------------------------------------------------------------------------------------------------------------
TFtpInet::~TFtpInet(void)
{
	if( m_FtpConnect == TRUE ) FtpClose();
}

//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpConnect(char *SrvAddr,char *user, char *pass)
//	タイプ：private
//	機能　：wininetを使ったFTP通信オープン
//	引数１；サーバーアドレス
//	引数２；ユーザー名
//	引数３；パスワード
//	返値　：成功=0
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FtpConnect(char *SrvAddr,char *user, char *pass )
{
	int pasv,port;
	if(m_FtpConnect == TRUE) FtpClose();
	m_FtpConnect = FALSE;
	
	//インターネットオープン
	m_Inet.hInternet = InternetOpen("NOKFTP", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	if (m_Inet.hInternet == NULL) {
		InternetCloseHandle(m_Inet.hInternet);
		return -1;
	}
	
	//ftp Connect
	m_Inet.hSession = InternetConnect(m_Inet.hInternet,
									SrvAddr,
									INTERNET_INVALID_PORT_NUMBER, 
									user,
									pass,
									INTERNET_SERVICE_FTP,		//使用するサービス:FTP そのほかにHTTPとかもあった。
									INTERNET_FLAG_PASSIVE,		// パッシブ
									0);
	if (m_Inet.hSession == NULL) {
		InternetCloseHandle(m_Inet.hInternet);
		return -2;
	}
	
	m_FtpConnect = TRUE;
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpConnect()
//	タイプ：private
//	機能　：wininetをクローズ
//	返値　：成功=0
//---------------------------------------------------------------------------------------------------------------------
void TFtpInet::FtpClose( void )
{
	InternetCloseHandle(m_Inet.hSession);
	InternetCloseHandle(m_Inet.hInternet);
	m_FtpConnect = FALSE;
}
//---------------------------------------------------------------------------------------------------------------------
//	int TFtpInet::FindFtpFile( char *serchfile )
//	タイプ：private
//	機能　：FindFirstのFTP版
//	引数　：検索対象ﾌｧｲﾙ
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FindFtpFile( char *serchfile )
{
	WIN32_FIND_DATA fd;
	HINTERNET hFind;
	int i;

	memset(&m_FileLst,0,sizeof(m_FileLst));

	i=0;
	hFind = FtpFindFirstFile(m_Inet.hSession, serchfile ,&fd,0,0); //検索ディレクトリはカレントディレクトリなので ファイル名だけ
	if(hFind != NULL) do {
		sprintf(m_FileLst.d[i].fname,"%s",fd.cFileName);
		i++;
	}while(InternetFindNextFile(hFind,&fd));
	InternetCloseHandle(hFind);

	m_FileLst.Total = i;
	if(i==0) return -1;
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpChdir( char *dir )
//	タイプ：private
//	機能　：カレントディレクトリの変更
//	引数１：変更先ディレクトリ
//	返値　：成功=0
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FtpChdir( char *dir )
{
	if( m_FtpConnect == FALSE) return -1;
	
	if( !FtpSetCurrentDirectory(m_Inet.hSession, dir ) ){
		return -2;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpGet( char *downfile, char *savefile, BOOL rewrite )
//	タイプ：private
//	機能　：GETする
//	引数１：ダウンロードするファイル
//	引数２：保存するファイル名
//	引数３：上書きするかどうか
//	返値　：成功=0
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FtpPut( char *putfile, char *savefile )
{
	if( m_FtpConnect == FALSE) return -1;
	if (!FtpPutFile( m_Inet.hSession, //FTPのインターネットハンドル
					putfile, //ダウンロードするファイル
					savefile, //ダウンロード先のパス付ファイル名
					FTP_TRANSFER_TYPE_BINARY, //バイナリファイルとしてダウンロード
					0 )){
		return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpGet( char *downfile, char *savefile, BOOL rewrite )
//	タイプ：private
//	機能　：GETする
//	引数１：ダウンロードするファイル
//	引数２：保存するファイル名
//	引数３：受信したファイルがあった場合上書きするかどうか
//	引数４：バイナリモードで受信する場合はTRUE,ASCIIモードはFALSE
//	返値　：成功=0
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FtpGet( char *downfile, char *savefile, BOOL rewrite ,BOOL binary/*=TRUE*/)
{
	int fmode;
	if( binary == TRUE) fmode = FTP_TRANSFER_TYPE_BINARY;
	else fmode = FTP_TRANSFER_TYPE_ASCII;
	
	if( m_FtpConnect == FALSE) return -1;
	if (!FtpGetFile( m_Inet.hSession, //FTPのインターネットハンドル
					downfile, //ダウンロードするファイル
					savefile, //ダウンロード先のパス付ファイル名
					rewrite, //ダウンロード先に同名のファイルがあるとき TRUE:エラーにする(上書き防止) FALSE:上書きする
					FILE_ATTRIBUTE_NORMAL, //ダウンロード先に作られるファイルのアトリビュート
					fmode, //バイナリファイルとしてダウンロード
					0 )){
		return -1;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------
//	void TFtpInet::FtpConnect( char *fname, char *chgname )
//	タイプ：private
//	機能　：リネーム
//	引数１：元ファイル名
//	引数１：変更するファイル名
//	返値　：成功=0
//---------------------------------------------------------------------------------------------------------------------
int TFtpInet::FtpRename( char *fname, char *chgname )
{
	if( m_FtpConnect == FALSE) return -1;
	
	if( !FtpRenameFile(m_Inet.hSession ,fname ,chgname ) ){
		return -2;
	}
	return 0;
}
