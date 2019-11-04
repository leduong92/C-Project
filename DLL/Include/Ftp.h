//	*****************************************
//	FTP.h
//
//		FTP Class
// ========================================================================
//        FTPプロトコルを利用したファイル転送クラインアントクラス
//        ファイル名：FTP.cpp
//
//        作成者：石田光弘（ＮＯＫ株式会社技術本部生産技術部）
//        作成日：2003年5月24日
//　　　　更新履歴：
//
//     その他：WinSock 2を使用しているためｗｉｎｄｏｗｓ９５
//　　　　　　　　では、WINsock２．０の導入が必要
//     必要ライブラリー
// Compile and link with ws2_32.lib
//                include file is <winsock2,h>
// Dynamically links to WS2_32.DLL
//
// ========================================================================
//		SuoerClass = None
//	
//		Copyright  2003 Nok Corporation.
//			All rights reserved.
//			Coded by M.Ishida
//	*****************************************
#ifndef FTP_H
#define FTP_H

//#define WIN32_WINNT 0x0400
//#include <windows.h>
#include <winsock2.h>

class FTP
{
    int  m_PortNo;
	char m_IPAdress[20];
	char m_Host[100];
	char m_User[100];
	char m_Password[100];

	long fRcvPacketNo,fSendPaketNo;
	SOCKET fSocketC,fSocketT;
	struct sockaddr_in	fAddrC;		//ホストのコントロール用アドレス
	struct sockaddr_in  fAddrT;		//ホストのファイル転送用アドレス。
	struct sockaddr_in	fCtrlAddr;	//ローカルコントロール用アドレス
    WSAEVENT fEvent;
	int ftpRetCode;
	int fStatus;
	int bConnected;
	char pbuf[300];
	BOOL m_printMode;	//画面表示の有無

public:

	FTP();
	~FTP();

	char m_log[1024];		// logバッファ
	char m_Resp[1024];	// FTPｻｰﾊﾞｰの応答メッセージ
	int  resCode;
	

	virtual BOOL	IFTP(void);
	virtual int	CheckStatus(void);
	virtual int SetUser(char *user);
	virtual int SetPassword(char *password);
	virtual int SetServerIP(char *serveip);
	virtual int SetHostName(char *hostname);
	virtual void SetPortNo(int port);

	virtual int Connect();

	virtual int CommandSend(char *message,int *replyNo,char *reply);
	virtual int Recv(char *answer);

	virtual int SendFileWrite(char *fnCliant, char *fnServer,int code);
	virtual int SendFileAppend(char *fnCliant, char *fnServer,int code);
	virtual int SendFileUnique(char *fnCliant, char *fnServer,int code);
	virtual int SendFile(char *fnCliant, char *fnServer,int mode,int code);
	virtual int RecvFile(char *fnCliant, char *fnServer);
	
//ディレクトリー・ファイル操作関係
	virtual int CmdCwd(LPSTR path);// CWD カレントディレクトリの変更とする。
	virtual int CmdCdup();	// CDUP 1つ上位のディレクトリ(親ディレクトリ)に戻る。
	virtual int CmdMkd(LPSTR path);	// MKD ディレクトリの作成する。
	virtual int CmdRmd(LPSTR path);// RMD ディレクトリを削除する。
	virtual int CmdPwd();	// PWD 現在のワーキングディレクトリを表示する。
	virtual int CmdDele(LPSTR path);// DELE ファイル名 指定したファイルを削除する。
	virtual int CmdRename(LPSTR path1,LPSTR path2);
	virtual int CmdList();	// [ファイル／ディレクトリ名] [オプション]
	virtual int CmdNList();// [ファイル／ディレクトリ名] [オプション]



//ＯＳ関係
	virtual int CmdSyst();	// SYST システム名を表示する。
	virtual int CmdNOOP();	// NOOP 何もしない。サーバの稼動を確認するために実行される。常に正常終了する。
	virtual int CmdQuit();	// QUIT ログアウトする。
	virtual int CmdAllc();	// ALLO ファイルバイト数 [R レコード/ページバイト数] あらかじめサーバー上で指定したバイト分領域を確保する。レコード/ページでも指定しなくてはならない場合は、オプションで指定できる（必要かどうかはOSに依存する）。
	virtual int CmdAbor();	// ABOR  現在実行中のデータ転送をすべて中止する。データコネクションは破棄される。
	virtual int CmdSite(LPSTR cmd);	// SITE コマンド 任意のOSコマンドを実行する。
	virtual int CmdStat(LPSTR path);// STAT [ファイル／ディレクトリ名] 現在のシステムや転送状態の情報を表示する。
	virtual int CmdRCmd(LPSTR cmd);	// AS400のプログラムを実行する。


//転送ﾓｰﾄﾞ関係
	virtual int CmdTypeASCII();	// TYPE A：ASCII 転送データの形式を指定する。
	virtual int CmdTypeImage();	// TYPE I：Image（バイナリ） 転送データの形式を指定する。
	virtual int CmdTypeEBOCDIC();	// TYPE E：EBCDIC 転送データの形式を指定する。
	virtual int CmdTypeSJIS();	// QUOTE TYPE B 1：SJIS 転送データの形式を指定する。
	virtual int CmdStruFile();	// STRU ファイル構造オプション 転送するファイルのファイル構造をオプションで示す。F：ファイル構造（バイト志向の通常のバイナリ）　デフォルト。
	virtual int CmdStruRecord();	// STRU ファイル構造オプション 転送するファイルのファイル構造をオプションで示す。R：レコード構造（シーケンシャルなどのレコードイメージを持ったデータ）
	virtual int CmdStruPage();	// STRU ファイル構造オプション 転送するファイルのファイル構造をオプションで示す。P：ページ構造（インデックス化されたページ志向のファイル）
	virtual int CmdModeStream();	// MODE 転送モードオプション  S：ストリーム（バイトストリームによる転送）デフォルト
	virtual int CmdModeBlock();	// MODE 転送モードオプション  B：ブロック（転送をブロックごとに行う）
	virtual int CmdModeCompression();// MODE 転送モードオプション  C：圧縮（Run-Length手法による圧縮）

//転送命令関係
	virtual int CmdRetr(LPSTR path)	;// RETR ファイル名 指定したファイルの内容をサーバから取得する。
	virtual int CmdStor(LPSTR path) ;// STOR ファイル名 指定したファイル名で、サーバへ送信するデータでファイルを作成する。同一名のファイルがすでにある場合には、上書きする。ファイルが存在していない場合は、新規作成する。
	virtual int CmdStou(LPSTR path)	;// STOU STORと同様に指定したファイルを作成するが、すでに同一名のファイルがあった場合には、重ならないようなファイル名を自動的に付けて作成する。
	virtual int CmdAppe(LPSTR path)	;// APPE ファイル名 サーバへ送信するデータを指定したファイルへ追加する。
	virtual int CmdPasv(void)		;//PASV パッシブモードへの移行を指示する。サーバはデータコネクションで使用するIPアドレスとポート番号を表示する。

	virtual int DoCommand(int CmdNo);
	virtual int DoCommand(int CmdNo,LPSTR path);
	virtual int DoCommand(int CmdNo,LPSTR path,char* buf);

//	virtual int DoRETR(LPSTR path);
//	virtual int DoSTOR(LPSTR path);
//	virtual int DoLIST(void);
//	virtual int DoCWD(LPSTR path);
	virtual int DoPASV(void);
	virtual int Print(char *buf);

	void SetPrintMode( BOOL mode ){ m_printMode = mode;};

protected:

};
#endif
