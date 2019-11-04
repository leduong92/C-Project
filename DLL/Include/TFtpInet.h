#ifndef FTP_INET_H
#define FTP_INET_H
//=====================================================================================================================
//
//	FileName	:TFtpInet.h
//	Class		:TFtpInet
//				:WinInet.lib を使ったFTP送受信関数ｸﾗｽ
//  2006.07.05 coded by ﾏﾂﾀﾞ
//	NOTE!! インクルードライブラリにwininet.libを追加してください!!
//=====================================================================================================================
#include <windows.h>
#include <wininet.h>
#include <stdio.h>

class TFtpInet
{
	private:
		typedef struct{
			char fname[128];
		}FILE_NAME;
		typedef struct{
			int Total;
			FILE_NAME d[500];
		}DOWN_FILE_LST;

		//FTP通信用 wininet
		typedef struct _tagINETHANDLE{
		    HINTERNET hInternet;
		    HINTERNET hSession;
		} INETHANDLE;
		INETHANDLE m_Inet;

	public:
		TFtpInet();
		~TFtpInet();

		int TFtpInet::FtpConnect(char *SrvAddr,char *user, char *pass);
		void FtpClose( void );

		int FindFtpFile( char *serchfile );
		int FtpChdir( char *dir );
		int FtpPut( char *putfile, char *savefile );
		int FtpGet( char *downfile, char *savefile, BOOL rewrite ,BOOL binary=TRUE);
		int FtpRename( char *fname, char *chgname );

		DOWN_FILE_LST m_FileLst;		//ﾀﾞｳﾝﾛｰﾄﾞﾌｧｲﾙﾘｽﾄ

	private:
		BOOL m_FtpConnect;

};

#endif
