// ========================================================================
//        FTPプロトコルを利用したファイル転送のクラインアントクラス
//        ファイル名：FTP.cpp
//
//        作成者：石田光弘（ＮＯＫ株式会社生産技術本部生産技術部）
//        作成日： 2003年5月24日
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

#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <ctype.h>
#include "time.h"
#include "FTP.h"

FTP::FTP()
{
	m_printMode=FALSE;

    WORD wVersionRequested = MAKEWORD(2,2);//調べて直接指定すること。
    WSADATA wsaData;
    int nRet;

	fStatus=0;

//1. Initialize WinSock.dll
    nRet = WSAStartup(wVersionRequested, &wsaData);
    if (nRet)
    {
        sprintf(pbuf,"\nWSAStartup()Err: %d\n", nRet);
		this->Print(pbuf);
        WSACleanup();
        return;
    }
//2. Check WinSock version
    if (wsaData.wVersion != wVersionRequested)
    {
        sprintf(pbuf,"\nWinSock version not supported\n");
		this->Print(pbuf);
        WSACleanup();
        return;
    }

//3. Create  socket
    fSocketC = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fSocketC == INVALID_SOCKET)
    {
        sprintf(pbuf,"socket()Err");
		this->Print(pbuf);
        WSACleanup();
        return;
    }

//4. Create event.
	fEvent = WSACreateEvent();
	if(fEvent == WSA_INVALID_EVENT)
	{
       sprintf(pbuf,"\nWSACreateEvent()Err: %d\n", nRet);
		this->Print(pbuf);
		closesocket(fSocketC);
        WSACleanup();
		return;
	}

//	nRet = WSAEventSelect(fSocketC,fEvent,FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
	nRet = WSAEventSelect(fSocketC,fEvent,FD_READ);
	if(nRet == SOCKET_ERROR)
	{
        sprintf(pbuf,"\nWSAEventSelect()Err: %d\n", nRet);
		this->Print(pbuf);
		WSACloseEvent(fEvent);
		closesocket(fSocketC);
		WSACleanup();
		return;
	}

	bConnected=0;
	fStatus=1;
    IFTP();
}

FTP::~FTP()
{

	if(fStatus==1)
	{
		if(fSocketC!=INVALID_SOCKET)
		{
			if(WSAIsBlocking())
			{
				WSACancelBlockingCall();
			}
		}
  		closesocket(fSocketC);
		WSACloseEvent(fEvent);
		WSACleanup();
	}

}

BOOL FTP::IFTP()
{
	return(TRUE);
}


//Initial connection
int FTP::Connect()
{
	int iFlag=1;
	char answer[500];
	char buf[256];
	int replyNo;
	int nRet,len;
    int nReason;

	// m_logの初期化
	memset(m_log,0,sizeof(m_log));


	if(bConnected==1)
	{
		sprintf(pbuf,"Already connected!");
		this->Print(pbuf);
		return(1);
	}

//0.Address を設定する。
////	m_PortNo=PORTNO;				//定数からSetPortNo()で変数に変更 2005.05.06 ﾏﾂﾀﾞ
////	strcpy(m_IPAdress,IPADRESS);	//定数からSetServerIp()で変数に変更 2005.05.06 ﾏﾂﾀﾞ

	memset(&fAddrC, 0, sizeof(fAddrC));
	fAddrC.sin_family		= AF_INET;
    fAddrC.sin_addr.s_addr   = inet_addr(m_IPAdress);
//	fAddrC.sin_addr.S_un.S_addr= inet_addr(fIPAdress);
    fAddrC.sin_port          = htons(m_PortNo);

// 1.Create a connected socket
	nRet=connect(fSocketC,(struct sockaddr*)&fAddrC,sizeof(struct sockaddr));
	if(nRet==SOCKET_ERROR)
	{
		nReason=WSAGetLastError();
		if(nReason!=WSAEWOULDBLOCK)
		{
			sprintf(pbuf,"Connection failed=%d",nReason);
			this->Print(pbuf);
			return(1);
		}
	}
//3.Get initial message recieve
	memset(answer,0,sizeof(answer));
	nRet=this->Recv(answer);
	if(nRet==SOCKET_ERROR)
	{
		sprintf(pbuf,"getsocknameErr1=%d",WSAGetLastError());
		this->Print(pbuf);
		return(1);
	}


//2. Get information about local end of the connection
	len=sizeof(struct sockaddr);
	nRet=getsockname(fSocketC,(struct sockaddr*)&fCtrlAddr,&len);
	if(nRet==SOCKET_ERROR)
	{
		sprintf(pbuf,"getsocknameErr2=%d",WSAGetLastError());
		this->Print(pbuf);
		return(1);
	}
	else
	{
		sprintf(pbuf,"Local : %s port %u \n\n",inet_ntoa(fCtrlAddr.sin_addr),ntohs(fCtrlAddr.sin_port));
		this->Print(pbuf);
	}


//4. Send User ID and PASS WORD

	//ユーザー名
	memset(buf,0,sizeof(buf));
	sprintf(buf,"USER %s",m_User);
	nRet=CommandSend(buf,&replyNo,answer);
////	nRet=CommandSend("USER nxtftp",&replyNo,answer);
	if(nRet==SOCKET_ERROR)
	{
		sprintf(pbuf,"getsocknameErr3=%d",WSAGetLastError());
		this->Print(pbuf);
		return(1);
	}

	if(replyNo!=331)
	{
		sprintf(pbuf,"UserNameErr");
		this->Print(pbuf);
		return(1);
	}
//5. Send PassWord パスワード
	memset(buf,0,sizeof(buf));
	sprintf(buf,"PASS %s",m_Password);
	nRet=CommandSend(buf,&replyNo,answer);
////	nRet=CommandSend("PASS nxtftp",&replyNo,answer);
	if(nRet==SOCKET_ERROR)
	{
		sprintf(pbuf,"getsocknameErr4=%d",WSAGetLastError());
		this->Print(pbuf);
		return(1);
	}
	if(replyNo!=230)
	{
		sprintf(pbuf,"PassWord Err\n");
		this->Print(pbuf);
		return(1);
	}
	return(0);
}


// PASV パッシブモードへの移行を指示する。サーバはデータコネクションで
//		使用するIPアドレスとポート番号を表示する。(取得する。）
int FTP::DoPASV(void)
{
	int replyNo,nRet;
	char message[100],reply[700],buf[300];;
	int i1,i2,i3,i4,p1,p2,len,i;
    int  thePortNo;
	char theIPAdress[20];

	char *ptr;
	memset(message,0,sizeof(message));
	memset(reply,0,sizeof(reply));
	memset(buf,0,sizeof(buf));
	sprintf(message,"PASV");
	nRet=CommandSend(message,&replyNo,reply);
	if(nRet==1) {sprintf(pbuf,"PASVError");this->Print(pbuf);return(nRet);}
	if(replyNo >=450 && replyNo <460) return(replyNo);
	strcpy(buf,reply);
	
	len =strlen(buf);
	ptr=NULL;i=3;
	while(i<len)
	{
		if(isdigit(buf[i])){ ptr=&buf[i];break;}
		i++;
	}
	if(ptr==NULL )					return(100);
	if((ptr=strtok(ptr,","))==NULL) return(101);
	i1=atoi(ptr);
	if((ptr=strtok(NULL,","))==NULL) return(102);
	i2=atoi(ptr);
	if((ptr=strtok(NULL,","))==NULL) return(103);
	i3=atoi(ptr);
	if((ptr=strtok(NULL,","))==NULL) return(104);
	i4=atoi(ptr);
	if((ptr=strtok(NULL,","))==NULL) return(104);
	
	p1=atoi(ptr);
	if((ptr=strtok(NULL,")\n"))==NULL) return(105);
	p2=atoi(ptr);

	thePortNo=p1*256+p2;
	sprintf(theIPAdress,"%d.%d.%d.%d",i1,i2,i3,i4);
	
//	fAddrT;		//ホストのファイル転送用アドレス。
	memset(&fAddrT, 0, sizeof(fAddrT));
	fAddrT.sin_family		= AF_INET;
    fAddrT.sin_addr.s_addr   = inet_addr(theIPAdress);
    fAddrT.sin_port          = htons(thePortNo);

//	sprintf(pbuf,"%s  :%d\n",theIPAdress,thePortNo);
//	this->Print(pbuf);

	return(nRet);	
}

int  FTP::SendFileWrite(char *fnCliant, char *fnServer,int code)
{
	return(SendFile(fnCliant,fnServer,1,code));

}
int  FTP::SendFileAppend(char *fnCliant, char *fnServer,int code)
{
	return(SendFile(fnCliant,fnServer,2,code));
}
int  FTP::SendFileUnique(char *fnCliant, char *fnServer,int code)
{
	return(SendFile(fnCliant,fnServer,3,code));

}


// 戻り値：負:エラー　正：送信バイト数
int  FTP::SendFile(char *fnCliant, char *fnServer,int mode,int code)
{
	WSABUF		Buffers;		//WSABUF構造体配列
	DWORD		dwBufferCount;	//Buffersの示す配列に入っているWSABUF構造体の数
	DWORD		NumberOfBytesSent;//送信バイト数を受け取るDWORD変数へのポインタ
	DWORD		dwFlags;		//送信方法を制御するビットマスク
	int nRet,nReason,ret;	
    int     len,ftplen;
	char    buf[5000];
	long	i;
	
	FILE *fp;
	
	fp=fopen(fnCliant,"rb");
	if(fp==NULL)
	{
        sprintf(pbuf,"File Open Err =%S",fnCliant); 
		this->Print(pbuf);
        return(200);
    }	
	
	this->DoPASV();//パッシブモードへの移行
    fSocketT = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fSocketT == INVALID_SOCKET)
    {
        sprintf(pbuf,"socket()Err"); 
		this->Print(pbuf);
        WSACleanup();
		fclose(fp);
        return(100);
    }

	nRet=connect(fSocketT,(struct sockaddr*)&fAddrT,sizeof(struct sockaddr));
	if(nRet==SOCKET_ERROR)
	{
		nReason=WSAGetLastError();
		if(nReason!=WSAEWOULDBLOCK)
		{
			sprintf(pbuf,"Connection failed=%d",nReason);
			this->Print(pbuf);
			fclose(fp);
			return(1);
		}
	}
	
	ret=0;
	if		(mode==1)	ret=this->CmdStor(fnServer);
	else if	(mode==2)	ret=this->CmdStou(fnServer);
	else if	(mode==3)	ret=this->CmdAppe(fnServer);
	else				ret=this->CmdStor(fnServer);
	if(ret) return ret;		// 2004/3/30 oda 

	ftplen=0;
	memset(buf,0,sizeof(buf));
	while((len=fread(buf,1,1024,fp))!=0)
	{		
		// 小文字対応 2004/4/14 add oda
		if(code){
			for(i=0;i<len;i++){
				if((0x81 <= (unsigned char)buf[i] && (unsigned char)buf[i] <= 0x9F) 
				|| (0xE0 <= (unsigned char)buf[i] && (unsigned char)buf[i] <= 0xEF)){
					i++;
				}else{
					if(islower(buf[i])){
						buf[i]=toupper(buf[i]);
					}
				}
			}
		}
		Buffers.len=len;
		Buffers.buf=buf;
		dwBufferCount=1;
		dwFlags=MSG_DONTROUTE;//MSG_PARTIAL;


		nRet=WSASend(fSocketT,&Buffers,dwBufferCount, &NumberOfBytesSent,dwFlags,NULL,NULL);
		if (nRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAENETDOWN) return(-1);

			nRet=WSASend(fSocketT,&Buffers,dwBufferCount, &NumberOfBytesSent,dwFlags,NULL,NULL);
		    if (nRet == SOCKET_ERROR)
			{   
			    return(-3);
			}
		}
		ftplen=ftplen+len;
	}
	closesocket(fSocketT);

	this->Recv(buf);
	fclose(fp);

	this->Print((char*)nRet);
	
	if(nRet){
		return(nRet);
	}else{
		return(ftplen);
	}

	return(nRet);
}
int  FTP::RecvFile(char *fnCliant, char *fnServer)
{
	// m_logの初期化
	memset(m_log,0,sizeof(m_log));
//	WSABUF		Buffers;		//WSABUF構造体配列
//	DWORD		dwBufferCount;	//Buffersの示す配列に入っているWSABUF構造体の数
//	DWORD		NumberOfBytesRsv;//送信バイト数を受け取るDWORD変数へのポインタ
//	DWORD		dwFlags;		//送信方法を制御するビットマスク
//	WSAEVENT	hEvents[5];		//イベントハンドルの配列へのポインタ
//	DWORD		cEvents;		//hEventsに含まれるイベントハンドルの数		
	int nRet,nReason,ErrCnt=0;
//	WSANETWORKEVENTS events;
	long ret;
  
	WSAEVENT theEvent;
	
	char    buf[1024];			//5000 to 1024 oda
	
	FILE *fp;

	fp=fopen(fnCliant,"wb");
	if(fp==NULL)
	{
        sprintf(pbuf,"File Open Err =%s",fnCliant); 
		this->Print(pbuf);
		strcat(m_log,pbuf);
		m_log[strlen(m_log)]=0x0d;
		m_log[strlen(m_log)+1]=0x0a;
//		strcat(m_log,"\r\n");
        return(200);
    }

	this->DoPASV();//パッシブモードへの移行

    fSocketT = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fSocketT == INVALID_SOCKET)
    {
        sprintf(pbuf,"socket()Err"); 
		this->Print(pbuf);
		fclose(fp);
        return(100);
    }
//4. Create event.
	theEvent = WSACreateEvent();
	if(theEvent == WSA_INVALID_EVENT)
	{
		sprintf(pbuf,"\nWSACreateEvent()Err\n");
		this->Print(pbuf);
		closesocket(fSocketT);
		fclose(fp);
		return(101);
	}
//	nRet = WSAEventSelect(fSocketC,fEvent,FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
	nRet = WSAEventSelect(fSocketT,theEvent,FD_READ | FD_CLOSE);
	if(nRet == SOCKET_ERROR)
	{
        sprintf(pbuf,"\nWSAEventSelect()Err: %d\n", nRet);
		this->Print(pbuf);
		WSACloseEvent(theEvent);
		closesocket(fSocketT);
		fclose(fp);
		return(102);
	}

	nRet=connect(fSocketT,(struct sockaddr*)&fAddrT,sizeof(struct sockaddr));
	if(nRet==SOCKET_ERROR)
	{
		nReason=WSAGetLastError();
		if(nReason!=WSAEWOULDBLOCK)
		{
			this->Print(pbuf);
			sprintf(pbuf,"Connection failed=%d",nReason);
			fclose(fp);
			return(1);
		}
	}

	nRet=this->CmdRetr(fnServer);//受信ファイルを指定。転送起動。
	//	nRet=this->DoLIST();//リストの受信。転送起動。

	// 前の処理がこけていたら終了
	if(resCode >=400 && resCode < 600) {
		return resCode;
	}
/*
	while(nRet==0)
	{
		//hEvents[0]=fEvent;
		//cEvents=1;
		// イベント待ち
		nRet = WSAWaitForMultipleEvents(1,&theEvent,FALSE,1000,FALSE);		//timeOut 10000
		if(nRet == WSA_WAIT_FAILED)
        {
			sprintf(pbuf,"\nWait Failed\n");
			this->Print(pbuf);
			sprintf(pbuf,"timeout Event=%02x\n",events.lNetworkEvents);
			this->Print(pbuf);
			memset(pbuf,0,sizeof(pbuf));
			memcpy(pbuf,Buffers.buf,NumberOfBytesRsv);
			this->Print(pbuf);
            break;
        }
		if (nRet == WSA_WAIT_TIMEOUT)
		{
			sprintf(pbuf,"\nWait timed out\n");
			this->Print(pbuf);
			sprintf(pbuf,"timeout Event=%02x\n",events.lNetworkEvents);
			this->Print(pbuf);
			memset(pbuf,0,sizeof(pbuf));
			memcpy(pbuf,Buffers.buf,NumberOfBytesRsv);
			this->Print(pbuf);
			break;
		}
		// イベント調査		
		nRet = WSAEnumNetworkEvents(fSocketT,theEvent,&events);
		//memset(pbuf,0,sizeof(pbuf));
		//sprintf(pbuf,"Event=%02x\n",events.lNetworkEvents);
		//this->Print(pbuf);
		if (nRet == SOCKET_ERROR)
		{
			this->Print(pbuf);
			sprintf(pbuf,"WSAEnumNetworkEvents()");
			continue;
		}
		if(events.lNetworkEvents == 0x21){
			strcat(m_log,"events.lNetworkEvents = 0x21!!\r\n");
			return 1;
		}		

		if(events.lNetworkEvents & FD_READ)
		{
			memset(buf,0,sizeof(buf));
			Buffers.len=sizeof(buf);
			Buffers.buf=buf;
			dwBufferCount=1;
			dwFlags=0;
			nRet=WSARecv(fSocketT,&Buffers,dwBufferCount,&NumberOfBytesRsv,&dwFlags,NULL,NULL);
			if(nRet!=0)
			{
				sprintf(pbuf,"WSARecv ERROR\n");
				this->Print(pbuf);
			}
			//fwrite(Buffers.buf,dwBufferCount,NumberOfBytesRsv,fp);		
			fwrite(Buffers.buf,NumberOfBytesRsv,dwBufferCount,fp);		
		}
		if(events.lNetworkEvents == FD_CLOSE)
		// if(events.lNetworkEvents & FD_CLOSE)
		{
			sprintf(pbuf,">>>>>Received Complete<<<<<\n");
			this->Print(pbuf);
			break;
		}
	}
*/
// WSARecvをしようしない 2004/06/23 oda

	while(nRet==0)
	{
		memset(buf,0,sizeof(buf));
		ret=recv(fSocketT,buf,sizeof(buf),0);
		if(!ret){
			sprintf(pbuf,">>>>>Received Complete<<<<<\n");
			this->Print(pbuf);
			break;
		}
		if(ret==SOCKET_ERROR){
			if(WSAGetLastError()==WSAEWOULDBLOCK){
				continue;		// WSAEWOULDBLOCKの場合は問題なし add oda
			}
			sprintf(pbuf,">>>>>socet_error<<<<<\n");
			this->Print(pbuf);
			break;
		}
		if(ret>0){
			fwrite(buf,ret,1,fp);
		}
	}

	closesocket(fSocketT);
	WSACloseEvent(theEvent);

	this->Recv(buf);

	fclose(fp);
	return(nRet);
}


//ディレクトリー・ファイル操作関係
int FTP::CmdCwd(LPSTR path)		{return(this->DoCommand(1,path));}	// CWD カレントディレクトリの変更とする。
int FTP::CmdCdup()				{return(this->DoCommand(2,""));}	// CDUP 1つ上位のディレクトリ(親ディレクトリ)に戻る。
int FTP::CmdMkd(LPSTR path)		{return(this->DoCommand(3,path));}	// MKD ディレクトリの作成する。
int FTP::CmdRmd(LPSTR path)		{return(this->DoCommand(4,path));}	// RMD ディレクトリを削除する。
int FTP::CmdPwd()				{return(this->DoCommand(5));}	// PWD 現在のワーキングディレクトリを表示する。
int FTP::CmdDele(LPSTR path)	{return(this->DoCommand(10,path));}	// DELE ファイル名 指定したファイルを削除する。
int FTP::CmdRename(LPSTR path1,LPSTR path2)
{
	int ret;	// RNFR ファイル名 RNTO ファイル名 指定したファイル名を指定ファイル名に変更。
	ret=this->DoCommand(11,path1);
	if(ret!=0) return(ret);
	ret=this->DoCommand(12,path2);
	return(ret);
}
int FTP::CmdList()				{return(this->DoCommand(13,""));}	// [ファイル／ディレクトリ名] [オプション]
int FTP::CmdNList()				{return(this->DoCommand(14,""));}	// [ファイル／ディレクトリ名] [オプション]



//ＯＳ関係
int FTP::CmdSyst()				{return(this->DoCommand(20));}	// SYST システム名を表示する。
int FTP::CmdNOOP()				{return(this->DoCommand(21));}	// NOOP 何もしない。サーバの稼動を確認するために実行される。常に正常終了する。
int FTP::CmdQuit()				{return(this->DoCommand(22));}	// QUIT ログアウトする。
int FTP::CmdAllc()				{return(this->DoCommand(23));}	// ALLO ファイルバイト数 [R レコード/ページバイト数] あらかじめサーバー上で指定したバイト分領域を確保する。レコード/ページでも指定しなくてはならない場合は、オプションで指定できる（必要かどうかはOSに依存する）。
int FTP::CmdAbor()				{return(this->DoCommand(24));}	// ABOR  現在実行中のデータ転送をすべて中止する。データコネクションは破棄される。
int FTP::CmdSite(PSTR cmd)		{return(this->DoCommand(25,cmd));}	// SITE コマンド 任意のOSコマンドを実行する。
int FTP::CmdStat(LPSTR path)	{return(this->DoCommand(26,path));}	// STAT [ファイル／ディレクトリ名] 現在のシステムや転送状態の情報を表示する。
int FTP::CmdRCmd(LPSTR cmd)		{return(this->DoCommand(27,cmd));}	// STAT [ファイル／ディレクトリ名] 現在のシステムや転送状態の情報を表示する。


//転送ﾓｰﾄﾞ関係
int FTP::CmdTypeASCII()			{return(this->DoCommand(40,"A"));}	// TYPE A：ASCII 転送データの形式を指定する。
int FTP::CmdTypeImage()			{return(this->DoCommand(40,"I"));}	// TYPE I：Image（バイナリ） 転送データの形式を指定する。
int FTP::CmdTypeEBOCDIC()		{return(this->DoCommand(40,"E"));}	// TYPE E：EBCDIC 転送データの形式を指定する。
int FTP::CmdTypeSJIS()			{return(this->DoCommand(40,"J"));}	// TYPE E：SJIS 転送データの形式を指定する。
int FTP::CmdStruFile()			{return(this->DoCommand(41,"F"));}	// STRU ファイル構造オプション 転送するファイルのファイル構造をオプションで示す。F：ファイル構造（バイト志向の通常のバイナリ）　デフォルト。
int FTP::CmdStruRecord()		{return(this->DoCommand(41,"R"));}	// STRU ファイル構造オプション 転送するファイルのファイル構造をオプションで示す。R：レコード構造（シーケンシャルなどのレコードイメージを持ったデータ）
int FTP::CmdStruPage()			{return(this->DoCommand(41,"P"));}	// STRU ファイル構造オプション 転送するファイルのファイル構造をオプションで示す。P：ページ構造（インデックス化されたページ志向のファイル）
int FTP::CmdModeStream()		{return(this->DoCommand(42,"S"));}	// MODE 転送モードオプション  S：ストリーム（バイトストリームによる転送）デフォルト
int FTP::CmdModeBlock()			{return(this->DoCommand(42,"B"));}	// MODE 転送モードオプション  B：ブロック（転送をブロックごとに行う）
int FTP::CmdModeCompression()	{return(this->DoCommand(42,"C"));}	// MODE 転送モードオプション  C：圧縮（Run-Length手法による圧縮）

//転送命令関係
int FTP::CmdRetr(LPSTR path)	{return(this->DoCommand(50,path));}	// RETR ファイル名 指定したファイルの内容をサーバから取得する。
int FTP::CmdStor(LPSTR path)	{return(this->DoCommand(51,path));}	// STOR ファイル名 指定したファイル名で、サーバへ送信するデータでファイルを作成する。同一名のファイルがすでにある場合には、上書きする。ファイルが存在していない場合は、新規作成する。
int FTP::CmdStou(LPSTR path)	{return(this->DoCommand(52,path));}	// STOU STORと同様に指定したファイルを作成するが、すでに同一名のファイルがあった場合には、重ならないようなファイル名を自動的に付けて作成する。
int FTP::CmdAppe(LPSTR path)	{return(this->DoCommand(53,path));}	// APPE ファイル名 サーバへ送信するデータを指定したファイルへ追加する。
int FTP::CmdPasv()				{return(this->DoCommand(60));}		// PASV パッシブモードへの移行を指示する。サーバはデータコネクションで使用するIPアドレスとポート番号を表示する。



int FTP::DoCommand(int CmdNo)
{
	char buf[200];
	memset(buf,0,sizeof(buf));
	return(this->DoCommand(CmdNo," ",buf));
}

int FTP::DoCommand(int CmdNo,LPSTR theParameters1)
{
	char buf[200];
	memset(buf,0,sizeof(buf));
	return(this->DoCommand(CmdNo,theParameters1,buf));
}

int FTP::DoCommand(int CmdNo,LPSTR theParameters1,char* buf)
{
	char reply[200];
	int replyNo,nRet;
	char message[200];

	memset(reply,0,sizeof(reply));
	memset(message,0,sizeof(message));
	switch(CmdNo)
	{	
	//ディレクトリー・ファイル操作関係
	
		case 1:// CWD ディレクトリ名 指定したディレクトリをカレントディレクトリとする。
			sprintf(message,"CWD %s",theParameters1);	
			break;
		case 2:	// CDUP 1つ上位のディレクトリ(親ディレクトリ)をカレントディレクトリとする。
			sprintf(message,"CDUP");				
			break;
		case 3:// MKD ディレクトリの作成する。
			sprintf(message,"MKD %s",theParameters1);	
			break;
		case 4:// RMD ディレクトリを削除する。
			sprintf(message,"RMD %s",theParameters1);	
			break;
		case 5://PWD 現在のワーキングディレクトリを表示する。
			sprintf(message,"PWD");	
			break;
		case 10:// DELE ファイル名 指定したファイルを削除する。
			sprintf(message,"DELE %s",theParameters1);	
			break;
		case 11:// ファイル名の変更（変更前の名前）。
			sprintf(message,"RNFR %s",theParameters1);
			break;
		case 12:// ファイル名の変更。（変更後の名前）
			sprintf(message,"RNTO %s",theParameters1);
			break;
		case 13:
			sprintf(message,"LIST %s",theParameters1);//[ファイル／ディレクトリ名] [オプション]
			break;
		case 14:
			sprintf(message,"NLIST %s",theParameters1);//[ファイル／ディレクトリ名] [オプション]
			break;

//ＯＳ関係
		case 20:// SYST システム名を表示する。
			sprintf(message,"SYST");	
			break;
		case 21:// NOOP 何もしない。サーバの稼動を確認するために実行される。常に正常終了する。
			sprintf(message,"NOOP",theParameters1);
			break;
		case 22:// QUIT ログアウトする。
			sprintf(message,"QUIT",theParameters1);
			break;
		case 23:// ALLO ファイルバイト数 [R レコード/ページバイト数] あらかじめサーバー上で指定したバイト分領域を確保する。レコード/ページでも指定しなくてはならない場合は、オプションで指定できる（必要かどうかはOSに依存する）。
			sprintf(message,"Allc %s",theParameters1);
			break;
		case 24:// ABOR  現在実行中のデータ転送をすべて中止する。データコネクションは破棄される。
			sprintf(message,"ABOR %s",theParameters1);
			break;
		case 25:// SITE コマンド 任意のOSコマンドを実行する。
			sprintf(message,"SITE %s",theParameters1);
			break;
		case 26:// STAT [ファイル／ディレクトリ名] 現在のシステムや転送状態の情報を表示する。
			sprintf(message,"STAT %s",theParameters1);
			break;
		case 27:// AS400のプログラムを実行する。
			sprintf(message,"RCMD %s",theParameters1);
			break;
		case 40:// TYPE A：ASCII 転送データの形式を指定する。
			if     (theParameters1[0]=='A') sprintf(message,"TYPE A");
			else if(theParameters1[0]=='I')	sprintf(message,"TYPE I");
			else if(theParameters1[0]=='E')	sprintf(message,"TYPE E");
			else if(theParameters1[0]=='J')	sprintf(message,"TYPE B 1");
			else							sprintf(message,"TYPE I");
			break;
		case 41://STRU ファイル構造オプション 転送するファイルのファイル構造をオプションで示す。F：ファイル構造（バイト志向の通常のバイナリ）　デフォルト。
			if     (theParameters1[0]=='F') sprintf(message,"STRU F");
			else if(theParameters1[0]=='R')	sprintf(message,"STRU R");
			else if(theParameters1[0]=='P')	sprintf(message,"STRU P");
			else							sprintf(message,"STRU F");
			break;
		case 42://MODE 転送モードオプション  S：ストリーム（バイトストリームによる転送）デフォルト 
			if     (theParameters1[0]=='S') sprintf(message,"MODE S");
			else if(theParameters1[0]=='B')	sprintf(message,"MODE B");
			else if(theParameters1[0]=='C')	sprintf(message,"MODE C");
			else							sprintf(message,"MODE S");
			break;

		case 50://RETR ファイル名 指定したファイルの内容をサーバから取得する。 
			sprintf(message,"RETR %s",theParameters1); 
			break;
		case 51://STOR ファイル名 指定したファイル名で、サーバへ送信するデータでファイルを作成する。同一名のファイルがすでにある場合には、上書きする。ファイルが存在していない場合は、新規作成する。
			sprintf(message,"STOR %s",theParameters1);
			break;
		case 52://STOU STORと同様に指定したファイルを作成するが、すでに同一名のファイルがあった場合には、重ならないようなファイル名を自動的に付けて作成する。
			sprintf(message,"STOU %s",theParameters1);
			break;
		case 53:// APPE ファイル名 サーバへ送信するデータを指定したファイルへ追加する。 
			sprintf(message,"APPE %s",theParameters1);
			break;

		case 60://PASV パッシブモードへの移行を指示する。サーバはデータコネクションで使用するIPアドレスとポート番号を表示する。
			sprintf(message,"PASV");
			break;

		default:
			sprintf(message,"NOOP");
			break;
			
	}		

	memset(reply,0,sizeof(reply));	
	strcat(m_log,"FTP::");
	strcat(m_log,message);
	m_log[strlen(m_log)]=0x0d;
	m_log[strlen(m_log)+1]=0x0a;
//	strcat(m_log,"\r\n");

	nRet=this->CommandSend(message,&replyNo,reply);

	if(nRet==1) {sprintf(pbuf,"\n%s Error",message);this->Print(pbuf);return(1);}

	if(replyNo >=400 && replyNo < 600) {
		sprintf(pbuf,"%s Err ",message);
		this->Print(pbuf);
		return(1);
	}
	strcpy(buf,reply);
	return(0);
}

int FTP::CommandSend(char *message,int *replyNo,char *reply)
{
	WSABUF		Buffers;		//WSABUF構造体配列
	DWORD		dwBufferCount;	//lpBuffersの示す配列に入っているWSABUF構造体の数
	DWORD		NumberOfBytesSent;//送信バイト数を受け取るDWORD変数へのポインタ
	DWORD		dwFlags;		//送信方法を制御するビットマスク
	int		len;

    int nRet;
	
	char    buf[300];

	strcpy(buf,message);
	len=strlen(message);
	buf[len++]='\r';
	buf[len++]='\n';
	buf[len]=0;

 	Buffers.len=len;
	Buffers.buf=buf;
	dwBufferCount=1;
	dwFlags=MSG_DONTROUTE;		//MSG_PARTIAL;

	// パスは表示しない
	if(memcmp(message,"PASS",4)){
		sprintf(pbuf,":%s",buf);
		this->Print(pbuf);
	}

    nRet=WSASend(fSocketC,&Buffers,dwBufferCount, &NumberOfBytesSent,dwFlags,NULL,NULL);
    if (nRet == SOCKET_ERROR)
	{
        if (WSAGetLastError() != WSAENETDOWN) return(nRet);

		nRet=WSASend(fSocketC,&Buffers,dwBufferCount, &NumberOfBytesSent,dwFlags,NULL,NULL);
        if (nRet == SOCKET_ERROR)
        {   
            return(nRet);
        }
    }
	nRet=this->Recv(reply);
	*replyNo=atol(reply);
	resCode=atoi(reply);
    return(nRet);
}

int FTP::Recv(char *answer)
{
	WSABUF		Buffers;		//WSABUF構造体配列
	DWORD		dwBufferCount;	//Buffersの示す配列に入っているWSABUF構造体の数
	DWORD		NumberOfBytesRsv;//送信バイト数を受け取るDWORD変数へのポインタ
	DWORD		dwFlags;		//送信方法を制御するビットマスク
	WSAEVENT	hEvents[5];		//イベントハンドルの配列へのポインタ
	DWORD		cEvents;		//hEventsに含まれるイベントハンドルの数		

    int ret,nRet=0;
	WSANETWORKEVENTS events;
	int	cnt=0,i;	
	char buf[500];

	memset(buf,0,sizeof(buf));

	while(nRet==0)
	{
		hEvents[0]=fEvent;
		cEvents=1;
		nRet = WSAWaitForMultipleEvents(1,&fEvent,TRUE,20000,TRUE);		//timeOut 10000→20000 oda
		if (nRet == WSA_WAIT_TIMEOUT)
		{
			sprintf(pbuf,"\nWait timed out");
			this->Print(pbuf);
			 continue;
		}
		nRet = WSAEnumNetworkEvents(fSocketC,fEvent,&events);
		if (nRet == SOCKET_ERROR)
		{
			sprintf(pbuf,"WSAEnumNetworkEvents()");
			this->Print(pbuf);
			continue;
		}
		if((events.lNetworkEvents & FD_READ)==0)
		{
			sprintf(pbuf,"Event Err FD=%d\n",FD_READ);
			this->Print(pbuf);
		}

		Buffers.len=sizeof(buf);
		Buffers.buf=buf;
		dwBufferCount=1;
		dwFlags=0;//MSG_PEEK;//MSG_PARTIAL;

		ret=WSARecv(fSocketC,&Buffers,dwBufferCount,&NumberOfBytesRsv,&dwFlags,NULL,NULL);
		if(ret!=0)
		{
			sprintf(pbuf,"WSARecv ERROR\n");
			this->Print(pbuf);
		}
	   
		if(NumberOfBytesRsv>400) NumberOfBytesRsv=400;
		Buffers.buf[NumberOfBytesRsv]=0;
		if(cnt==0)		{strcpy(answer,Buffers.buf);cnt=strlen(answer);}
		else			{strcat(answer,Buffers.buf);}
		if( isdigit(answer[0]) && isdigit(answer[1]) && isdigit(answer[2]) && (answer[3]==' ') ) break;
		i=10;
		while(answer[i]!=NULL)
		{
			if( isdigit(answer[i-5]) && isdigit(answer[i-4]) && isdigit(answer[i-3]) && (answer[i-2]==' ') ){cnt=1; break;}
			i++;
		}
		if(cnt==1) break;
	}
	memset(pbuf,0,sizeof(pbuf));
	sprintf(pbuf,">%s",buf);
	// ******* ここでftpの戻り値が分かる
	strcat(m_log,"FTP:>");
	strcat(m_log,buf);
	
	this->Print(pbuf);
	return(nRet);
}



int FTP::CheckStatus()
{
	return(fStatus);
}
//=====================================
// int FTP::SetUser(char *user)
// FTPサーバーのユーザーをｾｯﾄする
//=====================================
int FTP::SetUser(char *user)
{
	memset(m_User,0,sizeof(m_User));
	strcpy(m_User,user);
	return(0);
}
//=====================================
// int FTP::SSetPassword(char *password)
// FTPサーバーのパスワードをｾｯﾄする
//=====================================
int FTP::SetPassword(char *password)
{
	memset(m_Password,0,sizeof(m_Password));
	strcpy(m_Password,password);
	return(0);
}
//=====================================
// int FTP::SetServerIP(char *serveip)
// FTPサーバーのIPｱﾄﾞﾚｽをｾｯﾄする
//=====================================
int FTP::SetServerIP(char *serveip)
{
	memset(m_IPAdress,0,sizeof(m_IPAdress));
	strcpy(m_IPAdress,serveip);
	return(0);
}
//=====================================
// int FTP::SetPortNo(int port)
// FTPサーバーのポートをｾｯﾄする
//=====================================
void FTP::SetPortNo(int port)
{
	m_PortNo=0;
	m_PortNo = port;
}

//不要？
int FTP::SetHostName(char *hostname)
{
	strcpy(m_Host,hostname);
	return(0);
}

int  FTP::Print(char *buf)
{
	if( m_printMode == TRUE )
		printf("FTP:%s",buf);
	return(0);
}