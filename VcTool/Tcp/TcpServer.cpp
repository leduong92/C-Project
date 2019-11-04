////=====================================================================================================================
//
//	FileName:TcpServer.h
//	Class	:TcpServer
//			:ソケット通信クラス
//	作成日	:2002.07.22	  TAKE
//  修正日	:2003.10.18 TAKE タイムアウト設定を変更
//				ret = WSAWaitForMultipleEvents(cEvents,&m_Event,FALSE,5000,FALSE);→ret = WSAWaitForMultipleEvents(cEvents,&m_Event,FALSE,30000,FALSE);
//				time.tv_sec  = 1;  /* ﾀｲﾑｱｳﾄ1秒 */→time.tv_sec  = 3; 
//  修正日	:2005.01.18 TAKE Recv() 最大10秒待ってエラーとする機能を追加
//  修正日	:2005.04.15 TAKE 最大Queの数を１に変更
//  修正日	:2005.05.08 ﾏﾂﾀﾞ Recv() 大量データの受信に耐えられるよう修正
//	修正日	:2008.04.22 ﾏﾂﾀﾞ Send()のprint文をｺﾒﾝﾄｱｳﾄ (dt_down でバグるためコメントアウト)
//	修正日	:2008.11.11 matuda get to be enable client ip address
// 	修正日	:2010.06.08 ﾏﾂﾀﾞ RecvDirectでタイムアウトを指定できるよう変更
//=====================================================================================================================
#include "TcpServer.h"
#include "stdio.h"
#include "getnow.h"

TcpServer::TcpServer(int port)
{
	m_Status=0;
	m_PortNo = port;
	ITcpServer();
}

TcpServer::~TcpServer()
{
	if(m_Status==1){
		closesocket( m_FdAcpt );
		closesocket( m_Fd );
		WSACleanup();
	}
}

// 2005.07.11
TcpServer::TcpServer()
{
	m_Status=0;
	m_PortNo = -1;

}
//===================================================================================================================
// 初期化
//===================================================================================================================
int TcpServer::Initialize(int portNo,int PortCheck)
{
	if( PortCheck==TRUE ){
		if( portNo <1024) m_PortNo = DEF_TRANSPORT_NO;
		else m_PortNo = portNo;
	}else{
		m_PortNo = portNo;
	}

  	/* Winsockの初期設定 */
//  	if ((m_Status = WSAStartup( MAKEWORD(1,1), &WSAData )) == 0){
  	if ((m_Status = WSAStartup( MAKEWORD(2,2), &WSAData )) == 0){
		//printf( "\n%s", WSAData.szSystemStatus);
  	}
  	else{
    	printf( "\nWSAStartup(%d) error", m_Status );
    	return -1;
  	}

  	/* ｿｹｯﾄ生成 */
  	if ((m_FdAcpt = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    	printf( "\nsocket() error" );
    	return -2;
  	}

  	/* ｿｹｯﾄの名前付け */
  	memset(&m_AddrAcpt, 0, sizeof(m_AddrAcpt));  /* bzero(&addr, sizeof(addr)); */
  	m_AddrAcpt.sin_family      = PF_INET;
  	m_AddrAcpt.sin_port        = htons(m_PortNo);
  	m_AddrAcpt.sin_addr.s_addr = INADDR_ANY;
  	if (bind(m_FdAcpt, (struct sockaddr FAR *) &m_AddrAcpt, sizeof(m_AddrAcpt)) == SOCKET_ERROR) {
    	printf( "\nbind() error" );
    	closesocket( m_FdAcpt );
    	return -3;
  	}
	//4. Create event.
	m_Event = WSACreateEvent();
	if(m_Event == WSA_INVALID_EVENT)
	{
		closesocket(m_FdAcpt);
        WSACleanup();
		return -4;
	}

//	printf("R=%d W=%d a=%d O=%d Q=%d C=%d cl=%d\n", FD_READ ,FD_WRITE , FD_ACCEPT,FD_OOB,FD_QOS,FD_CONNECT , FD_CLOSE);
	//	nRet = WSAEventSelect(fSocket,fEvent,FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
	int err;
	err = WSAEventSelect(m_FdAcpt,m_Event,FD_ACCEPT );
	if(err == SOCKET_ERROR)
	{
        printf("\nWSAEventSelect()Err: %d\n", err);
		WSACloseEvent(m_Event);
		closesocket(m_FdAcpt);
		WSACleanup();
		return -5;
	}
	// 2005.04.14 キューを1にする
	err = listen(m_FdAcpt, 1); //err = listen(m_FdAcpt, 5);
  	if ( err == SOCKET_ERROR) {
		printf( "\nlisten() error=%d",WSAGetLastError() );
    	closesocket(m_FdAcpt);
    	return -6;
  	}	
	m_Status=1;
	return 0;
}


BOOL  TcpServer::ITcpServer()
{
	memset(m_ClientIp,0,sizeof(m_ClientIp));

  	printf( "\n" );
  	printf( "<<<   TCP Protocol Setup   >>>\n" );
  	printf( "\n" );
  	/* Winsockの初期設定 */
//  	if ((m_Status = WSAStartup( MAKEWORD(1,1), &WSAData )) == 0){
  	if ((m_Status = WSAStartup( MAKEWORD(2,2), &WSAData )) == 0){
		printf( "%s\n", WSAData.szSystemStatus);
  	}
  	else{
    	printf( "WSAStartup(%d) error", m_Status );
    	return FALSE;
  	}

  	/* ｿｹｯﾄ生成 */
  	if ((m_FdAcpt = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
    	printf( "socket() error\n" );
    	return FALSE;
  	}

  	/* ｿｹｯﾄの名前付け */
  	memset(&m_AddrAcpt, 0, sizeof(m_AddrAcpt));  /* bzero(&addr, sizeof(addr)); */
  	m_AddrAcpt.sin_family      = PF_INET;
  	m_AddrAcpt.sin_port        = htons(m_PortNo);
  	m_AddrAcpt.sin_addr.s_addr = INADDR_ANY;
  	if (bind(m_FdAcpt, (struct sockaddr FAR *) &m_AddrAcpt, sizeof(m_AddrAcpt)) == SOCKET_ERROR) {
    	printf( "bind() error\n" );
    	closesocket( m_FdAcpt );
    	return FALSE;
  	}
	//4. Create event.
	m_Event = WSACreateEvent();
	if(m_Event == WSA_INVALID_EVENT)
	{
		closesocket(m_FdAcpt);
        WSACleanup();
		return FALSE;
	}

//	printf("R=%d W=%d a=%d O=%d Q=%d C=%d cl=%d\n", FD_READ ,FD_WRITE , FD_ACCEPT,FD_OOB,FD_QOS,FD_CONNECT , FD_CLOSE);
	//	nRet = WSAEventSelect(fSocket,fEvent,FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);
	int err;
	err = WSAEventSelect(m_FdAcpt,m_Event,FD_ACCEPT );
	if(err == SOCKET_ERROR)
	{
        printf("\nWSAEventSelect()Err: %d\n", err);
		WSACloseEvent(m_Event);
		closesocket(m_FdAcpt);
		WSACleanup();
		return FALSE;
	}
//  2005.04.15 TAKE 最大Queの数を１に変更
//	err = listen(m_FdAcpt, 5);
	err = listen(m_FdAcpt, 1);  
	if ( err == SOCKET_ERROR) {
		printf( "listen() error=%d\n",WSAGetLastError() );
    	closesocket(m_FdAcpt);
    	return -1;
  	}	
	m_Status=1;
	return TRUE;
}
//===================================================================================================================
// get client ip address
// Arg1:data pointer of output
//===================================================================================================================
void TcpServer::GetClientIp(char *ip)
{
	strcpy(ip,m_ClientIp);
}

// 引数１：タイムアウト(リクエスト待ち時間) 05.12.01追加
int TcpServer::Wait( DWORD timeout/*=5000*/)
{
	WSAEVENT	hEvents[5];		//イベントハンドルの配列へのポインタ
	DWORD		cEvents;		//hEventsに含まれるイベントハンドルの数		
	WSANETWORKEVENTS events;
    int ret=0;
	

	hEvents[0] = m_Event;
	cEvents=1;
	events.lNetworkEvents=0;
	ret = WSAWaitForMultipleEvents(cEvents,&m_Event,FALSE,timeout,FALSE);
//	ret = WSAWaitForMultipleEvents(cEvents,&m_Event,FALSE,5000,FALSE);
//	ret = WSAWaitForMultipleEvents(cEvents,&m_Event,FALSE,30000,FALSE);

    if (ret == WSA_WAIT_TIMEOUT)
    {
         printf(".");	// Wait 状態表示
         return(-1);
    }
    ret = WSAEnumNetworkEvents(m_FdAcpt,m_Event,&events);
   if (ret == SOCKET_ERROR)
   {
        printf("WSAEnumNetworkEvents():Err=%d\n",WSAGetLastError());getchar();
        return(-2);
   }
#ifdef _DEBUGPRINT
printf("ret=%d event=%d\n",ret,events.lNetworkEvents);
#endif
    if((events.lNetworkEvents & FD_ACCEPT)==0)
	{
		printf("Event Err FD=%d\n",FD_ACCEPT);return(-3);
	}
    return(ret);
}
int  TcpServer::Recv( char *recvdat )
{
  	int		exit_flag;

	int		recv_len;
	int		err;
  	int len = sizeof(m_AddrAcpt);
	char buf[5+1];
	char revbuf[MAX_SENDLENGTH];
	
	int timecnt;
	int sockerr;

	/* ｸﾗｲｱﾝﾄの接続受け付け */ 
	printf( "Waiting for connect...\n" );
  	if ((m_Fd = accept(m_FdAcpt, (struct sockaddr *)&m_Addr, &len)) < 0){
    	printf( "accept() error\n" );
    	closesocket( m_FdAcpt );
		return -2;
  	}
  	printf( "Connected.\n" );

	// set client ip 2008.11.11 matsuda
	sprintf(m_ClientIp,"%s",inet_ntoa(m_Addr.sin_addr));


  	/* ﾙｰﾌﾟﾊﾞｯｸ処理の繰り返し */
  	exit_flag = 0;

	/* ﾃﾞｰﾀ受信 */
    printf( "Waiting to receive data...\n" );
    recv_len = 0;  /* 受信ﾃﾞｰﾀ長初期化 */
	timecnt = 0;
	sockerr=0;
    while( 1 ){
      	do{
       		/* ﾃﾞｰﾀ受信待ち */
        	FD_ZERO( &r_socket );
        	FD_ZERO( &w_socket );
        	FD_ZERO( &e_socket );
        	FD_SET( m_Fd, &r_socket );
//        	time.tv_sec  = 1;  /* ﾀｲﾑｱｳﾄ1秒 */
//        	time.tv_usec = 0;
        	time.tv_sec  = 2;  //タイムｱｳﾄ2秒に変更
	      	time.tv_usec = 0;
        	err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
        	if(err < 0){
          		printf( "select() error\n" );
          		exit_flag = 1;
          		break;
       		}
        	else if( err == 0 ){
          		printf( "(receive time out)\n" );  /* ﾀｲﾑｱｳﾄ発生 */
				return 0;
        	}
        	else if( FD_ISSET( m_Fd, &r_socket ) == 0 ){
          		printf( "FD_ISSET() error\n" );
          		exit_flag = 1;
          		break;
        	}
		}while( err == 0 );
		/* 受信 */
		memset(revbuf,0,sizeof(revbuf));
		recv_len=0;
		while(1){
			// 2014.05.22 coba binary対応
			err = recv( m_Fd, &revbuf[recv_len], sizeof(revbuf)-recv_len  , 0 );
			//err = recv( m_Fd, &revbuf[strlen(revbuf)], sizeof(revbuf)-strlen(revbuf)  , 0 );
			if( err == -1 ){  /* 受信ｴﾗｰ */
				sockerr++;
				if(sockerr > 10){
					printf( "Err. Rcv Transfer failed.\n" );
					exit_flag = 1;
					//break;
					//return に変更 2005.01.14 ﾏﾂﾀﾞ
					return -1;
				}
				else{
					Sleep(100);
					continue;
				}
			}
			sockerr=0;

			/* 受信ﾃﾞｰﾀ長の更新・確認 */
			recv_len += err;
			memcpy(buf,revbuf,5);
			buf[5]=0;
			sockerr=0;		// 2013.09.24 受信出来たらエラークリア

			if(atol(buf) <= recv_len-5){
				exit_flag = 0;
				break;
			}
			//まだ受信バイト数に満たなかったら
			else{
				Sleep(100);
				timecnt++;
				if(timecnt > 10){		//10秒待ってダメならタイムｱｳﾄ 2005.05.08 ﾏﾂﾀﾞ
					printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);
					exit_flag = 1;
					break;
				}
			}
		}

		if(atol(buf) <= recv_len-5) break;

	}
    /* ﾌﾟﾛｸﾞﾗﾑ終了確認 */
    if( exit_flag == 1 ){
		return -1;
	}
	memcpy(recvdat,revbuf+5,atol(buf));

	//	return recv_len;

	return atol(buf)-5;
}

/*
int  TcpServer::Recv( char *recvdat )
{
  	int		exit_flag;
	int		recv_len;
	int		err;
  	int len = sizeof(m_AddrAcpt);
	char buf[20];
	char revbuf[4096];
	char stime[20],etime[20];

	memset(stime,0,sizeof(stime));
	memset(etime,0,sizeof(etime));

	get_nowtime(stime,14,0);
	Calctime( etime , stime , 10 , MD_SEC );	// 最大10秒待つ

	// ｸﾗｲｱﾝﾄの接続受け付け  
	printf( "Waiting for connect...\n" );
  	if ((m_Fd = accept(m_FdAcpt, (struct sockaddr *)&m_Addr, &len)) < 0){
    	printf( "accept() error\n" );
    	closesocket( m_FdAcpt );
		return -2;
  	}
  	printf( "Connected.\n" );


  	// ﾙｰﾌﾟﾊﾞｯｸ処理の繰り返し 
  	exit_flag = 0;

	// ﾃﾞｰﾀ受信 
    printf( "Waiting to receive data...\n" );
    recv_len = 0;  // 受信ﾃﾞｰﾀ長初期化 

    do{
       	// ﾃﾞｰﾀ受信待ち 
        FD_ZERO( &r_socket );
        FD_ZERO( &w_socket );
        FD_ZERO( &e_socket );
        FD_SET( m_Fd, &r_socket );
        time.tv_sec  = 1;  // ﾀｲﾑｱｳﾄ1秒 
        time.tv_usec = 0;
        err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
        if(err < 0){
        	printf( "select() error\n" );
        	exit_flag = 1;
        	break;
       	}
        else if( err == 0 ){
        	printf( "(receive time out)\n" );  // ﾀｲﾑｱｳﾄ発生 
			return 0;
        }
        else if( FD_ISSET( m_Fd, &r_socket ) == 0 ){
        	printf( "FD_ISSET() error\n" );
        	exit_flag = 1;
        	break;
        }
	}while( err == 0 );
		// 受信 	
	
	
	while( 1 ){

		err = recv( m_Fd, revbuf, MAX_RECVLENGTH  , 0 );
		if( err == -1 ){  // 受信ｴﾗｰ 
			printf( "Rcv Transfer failed.\n" );
			exit_flag = 1;
			break;
		}
		// 受信ﾃﾞｰﾀ長の更新・確認 
		recv_len += err;
//		if( recv_len == data_len ){
//			printf( "Transfer complete. [Serial No.%03d]\n", (unsigned char)recvdat[0] );
//			break;
//		}
		memcpy(buf,revbuf,5);
		buf[5]=0;

		if(atol(buf)==recv_len-5) break;
	
		printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);

		get_nowtime(stime,14,0);
		if(memcmp(stime,etime,14)>0) return 0;  // ﾀｲﾑｱｳﾄ発生 
	}
    // ﾌﾟﾛｸﾞﾗﾑ終了確認 
    if( exit_flag == 1 ){
		return -1;
	}
	memcpy(recvdat,revbuf+5,atol(buf));

	//	return recv_len;

	return atol(buf)-5;
}
*/

//===================================================================================================================
//	int  TcpServer::Send(char *msg)
//	データ送信 Send(char*,len)をコールする
//===================================================================================================================
int  TcpServer::Send(char *msg)
{
	return(this->Send(msg,strlen(msg),0));
}
/* 2008.04.25 Send関数を一本化
int  TcpServer::Send(char *msg)
  	int		exit_flag=0;		// 初期化する 06.03.16 coba
	int		err;

/// TOKEN用に変更送信バイト数を追加
	char buf[5+1];
///	sprintf(buf,"%05ld",strlen(msg));		2005.01.19 TAKE
	sprintf(buf,"%05ld",strlen(msg)-5);
	memcpy(msg,buf,5);

    // ﾃﾞｰﾀ送信
    printf( "Waiting to send data...\n" );
 	do{
	// ﾃﾞｰﾀ送信待ち
	    FD_ZERO( &r_socket );
	    FD_ZERO( &w_socket );
	    FD_ZERO( &e_socket );
	    FD_SET( m_Fd, &w_socket );
	    time.tv_sec  = 1;  // ﾀｲﾑｱｳﾄ1秒
	    time.tv_usec = 0;
	    err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
	    if(err < 0){
	        printf( "select() error\n" );
	        exit_flag = 1;
	        break;
      	}
	    else if( err == 0 ){
	        printf( "(send time out)\n" );  // ﾀｲﾑｱｳﾄ発生
	    }
	    else if( FD_ISSET( m_Fd, &w_socket ) == 0 ){
	       	printf( "FD_ISSET() error\n" );
	        exit_flag = 1;
	        break;
	    }
	}while( err == 0 );

    // 送信 

//	FILE *fp;
//	fp=fopen("send.log","w");
//	fprintf(fp,"%s***",msg);
//	fclose(fp);
//	printf("msg=%s***len=%d\n",msg,strlen(msg));
//	err = send( m_Fd, &msg[0], strlen(msg) , 0 );


	
	err = send( m_Fd, &msg[0], MAX_SENDLENGTH , 0 );
    if( err == -1 ){
		printf( "Send Transfer failed.\n" );
    }
    printf( "Send Transfer complete.\n" );

    if(  exit_flag == 1 ) return -1;

	// 2008.04.24 ここでcloseはだめ
	closesocket( m_Fd );
//	closesocket( m_FdAcpt );
	printf( "Terminated.\n" );

	return 0;
}
*/

int TcpServer::SetPortNo(int portNo)
{
	if(portNo>0x09999 || portNo <0x900) return(1);
	m_PortNo = portNo;
	
	return(0);	

}
int TcpServer::SetIPAdress(char *IPAdress)
{
	strncpy(m_IPAdress,IPAdress,15);
	m_IPAdress[15]=0;
	return(0);	

}

int TcpServer::SetPcName(char *PcName)
{
	strncpy(fPcName,PcName,20);
	fPcName[20]=0;
	return(0);	
}

int TcpServer::SetProgramName(char *ProgramName)
{
	strncpy(fProgramName,ProgramName,20);
	fProgramName[20]=0;
	return(0);	

}
int TcpServer::CheckStatus()
{
	return(m_Status);
}

//===================================================================================================================
//	int  TcpServer::Recv( char *recvdat,int bufsize )
//	機能　：ソケットのデータを受信する
//	引数　：受信バッファ
//	戻り値：受信バイト数
//	2005.07.11追加
//===================================================================================================================
int  TcpServer::Recv( char *recvdat,int bufsize )
{
	int		recv_len;
	int		err;
  	int len = sizeof(m_AddrAcpt);
	char buf[5+1];
	char revbuf[MAX_SENDLENGTH+10];
	int timeout,sockerr;

	/* ｸﾗｲｱﾝﾄの接続受け付け */ 
#ifdef _DEBUGPRINT
	printf( "\nWaiting for connect..." );
#endif
  	if ((m_Fd = accept(m_FdAcpt, (struct sockaddr *)&m_Addr, &len)) < 0){
    	printf( "accept() error\n" );
    	closesocket( m_FdAcpt );
		return -2;
  	}
#ifdef _DEBUGPRINT
  	printf( "Connected." );
#endif

	// set client ip 2010.02.25 matsuda
	sprintf(m_ClientIp,"%s",inet_ntoa(m_Addr.sin_addr));

	/* ﾃﾞｰﾀ受信 */
#ifdef _DEBUGPRINT
    printf( "\nWaiting to receive data..." );
#endif

	/* ﾃﾞｰﾀ受信待ち */
   	FD_ZERO( &r_socket );
   	FD_ZERO( &w_socket );
   	FD_ZERO( &e_socket );
	FD_SET( m_Fd, &r_socket );
    time.tv_sec  = 1;  /* ﾀｲﾑｱｳﾄ1秒 */
    time.tv_usec = 0;
    err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
    if(err < 0){
   		printf( "select() error\n" );
   		return(0);
	}
   	else if( err == 0 ){
	printf( "(receive time out)\n" );  /* ﾀｲﾑｱｳﾄ発生 */
		return 0;
   	}
  	else if( FD_ISSET( m_Fd, &r_socket ) == 0 ){
		printf( "FD_ISSET() error\n" );
		return 0;
	}

	/* 受信ﾃﾞｰﾀ長初期化 */
    recv_len = 0; 
	timeout=0;
	sockerr=0;
	memset( revbuf,0,sizeof(revbuf));

	/* 受信 */
    while( 1 ){
		// 2014.05.22 coba binary対応
		err = recv( m_Fd, &revbuf[recv_len], sizeof(revbuf)-recv_len  , 0 );
		//err = recv( m_Fd, &revbuf[strlen(revbuf)], sizeof(revbuf)-strlen(revbuf)  , 0 );
		/* 受信ｴﾗｰ */
		if( err == -1 ){ 
			if(++sockerr > 10){
				printf( "Rcv Transfer failed.\n" );
				return(0);
			}
			else{
				Sleep(100);
				continue;
			}
		}
		sockerr=0;

		/* 受信ﾃﾞｰﾀ長の更新・確認 */
		recv_len += err;

		sockerr=0;		// 2013.09.24 受信出来たらエラークリア

		// ﾃﾞｰﾀヘッダから受信長を抜き出す
		memcpy(buf,revbuf,5);
		buf[5]=0;

		// 受信長ﾁｪｯｸ
		if(atol(buf)<=recv_len-5) break;
		// 残りあり
		else{
			if(++timeout >= 30){		//３秒待ってダメならタイムｱｳﾄ
				printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);
				return(0);
			}
			Sleep(100);
		}

#ifdef _DEBUGPRINT
		printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);
#endif
	}
	memcpy(recvdat,revbuf+5,atol(buf)> bufsize ? bufsize:atol(buf));

	//	return recv_len;
	// 受信バイト数を戻す
	return atol(buf);
}

//===================================================================================================================
//	int  TcpServer::Send(char *msg,int sendlen)
//	機能　：データの送信
//	引数　：送信バッファ
//	戻り値：送信バイト数
//	2005.07.11追加
//	2008.04.25 送信後、通信相手からのcloseを待ってからこちら側をcloseする
//===================================================================================================================
int  TcpServer::Send(char *msg,int sendlen,DWORD close_wait)
{
  	int		exit_flag=0;		// 初期化する 06.03.16 coba
	int		err;
	char buf[MAX_SENDLENGTH+10];

/// TOKEN用に変更送信バイト数を追加

	sprintf(buf,"%05ld",sendlen);
//	sprintf(buf,"%05ld",strlen(msg));
	memcpy(buf+5, msg, sendlen);
	buf[sendlen+5]=0;

	//2008.04.22 ﾏﾂﾀﾞ dt_down でバグるためコメントアウト
//	printf("\nsend=%s",buf+5);

    /* ﾃﾞｰﾀ送信 */
#ifdef _DEBUGPRINT
    printf( "\nWaiting to send data..." );
#endif
 	do{
	/* ﾃﾞｰﾀ送信待ち */
	    FD_ZERO( &r_socket );
	    FD_ZERO( &w_socket );
	    FD_ZERO( &e_socket );
	    FD_SET( m_Fd, &w_socket );
	    time.tv_sec  = 1;  /* ﾀｲﾑｱｳﾄ1秒 */
	    time.tv_usec = 0;
	    err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
	    if(err < 0){
	        printf( "select() error\n" );
	        exit_flag = 1;
	        break;
      	}
	    else if( err == 0 ){
	        printf( "(send time out)\n" );  /* ﾀｲﾑｱｳﾄ発生 */
	    }
	    else if( FD_ISSET( m_Fd, &w_socket ) == 0 ){
	       	printf( "FD_ISSET() error\n" );
	        exit_flag = 1;
	        break;
	    }
	}while( err == 0 );

	err = send( m_Fd, buf, sendlen+5 , 0 );
    if( err == -1 ){
		printf( "Send Transfer failed.\n" );
    }
#ifdef _DEBUGPRINT
    printf( "Send Transfer complete.\n" );
#endif
    if(  exit_flag == 1 ) return -1;

	// 2008.05.08 後方互換のため即クローズを追加
	if( close_wait < 1 ){
		closesocket( m_Fd );
		return 0;
	}

	// 2008.04.25 coba	正しい終了まち
	WSAEVENT	hEvent;		//イベントハンドルの配列へのポインタ
	WSANETWORKEVENTS events;
	DWORD ret;
	hEvent = WSACreateEvent();
	if(hEvent == WSA_INVALID_EVENT)
	{
		closesocket(m_Fd);
        WSACleanup();
		return -4;
	}
	// 相手方のCloseを受け取ってから接続を閉じる
	// 距離が長い場合など、相手が受け取る前にこちらで閉じてしまうのを防ぐため
	err = WSAEventSelect(m_Fd,hEvent,FD_CLOSE );
	if(err == SOCKET_ERROR)
	{
        printf("\nWSAEventSelect()Err: %d\n", err);
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
		return -5;
	}

	ret=WSAWaitForMultipleEvents(1,&hEvent,FALSE,close_wait,FALSE);
	if (ret == WSA_WAIT_TIMEOUT)
    {
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
        return(-1);
    }
    ret = WSAEnumNetworkEvents(m_Fd,hEvent,&events);
   if (ret == SOCKET_ERROR)
   {
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
        printf("WSAEnumNetworkEvents():Err=%d\n",WSAGetLastError());
        return(-2);
   }

// 確認用 2008.04.25
#ifdef _DEBUGPRINT
    if(events.lNetworkEvents & FD_CLOSE)
	{
		printf("Event close%d,\n",FD_CLOSE);
	}
	getchar();
#endif

	// イベントを閉じ、ソケットをcloseする
	WSACloseEvent(hEvent);
	closesocket( m_Fd );
	memset(m_ClientIp,0,sizeof(m_ClientIp));
	return 0;
}

//===================================================================================================================
//	int  TcpServer::Recv( char *recvdat,int bufsize )
//	機能　：ソケットのデータをバッファを介さず受信する。送信データ数もカットしない
//	引数　：受信バッファ
//	戻り値：受信バイト数
//	2005.07.11追加
// 2010.06.08 ﾏﾂﾀﾞ タイムアウトを指定できるよう変更
//===================================================================================================================
int  TcpServer::RecvDirect( char *recvdat,int bufsize ,int time_out/*=1*/)
{
	int		recv_len;
	int		err;
  	int len = sizeof(m_AddrAcpt);
	int timeout,sockerr;
	char buf[10];

	/* ｸﾗｲｱﾝﾄの接続受け付け */ 
#ifdef _DEBUGPRINT
	printf( "\nWaiting for connect..." );
#endif
  	if ((m_Fd = accept(m_FdAcpt, (struct sockaddr *)&m_Addr, &len)) < 0){
    	printf( "accept() error\n" );
    	closesocket( m_FdAcpt );
		return -2;
  	}
#ifdef _DEBUGPRINT
  	printf( "Connected." );
#endif

	// set client ip 2010.02.25 matsuda
	sprintf(m_ClientIp,"%s",inet_ntoa(m_Addr.sin_addr));

	/* ﾃﾞｰﾀ受信 */
#ifdef _DEBUGPRINT
    printf( "\nWaiting to receive data..." );
#endif

	/* ﾃﾞｰﾀ受信待ち */
   	FD_ZERO( &r_socket );
   	FD_ZERO( &w_socket );
   	FD_ZERO( &e_socket );
	FD_SET( m_Fd, &r_socket );
//    time.tv_sec  = 1;  /* ﾀｲﾑｱｳﾄ1秒 */
	// 2010.06.08 ﾏﾂﾀﾞ タイムアウトを指定できるよう変更
    time.tv_sec  = time_out;  /* ﾀｲﾑｱｳﾄ1秒 */
    time.tv_usec = 0;
    err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
    if(err < 0){
   		printf( "select() error\n" );
   		return(-1);
	}
   	else if( err == 0 ){
	printf( "(receive time out)\n" );  /* ﾀｲﾑｱｳﾄ発生 */
		return -2;
   	}
  	else if( FD_ISSET( m_Fd, &r_socket ) == 0 ){
		printf( "FD_ISSET() error\n" );
		return -3;
	}

	/* 受信ﾃﾞｰﾀ長初期化 */
    recv_len = 0; 
	timeout=0;
	sockerr=0;
	memset( recvdat,0,bufsize);
	int chk=0,pos=0,dlen=0;

	/* 受信 */
    while( 1 ){
		err = recv( m_Fd, &recvdat[recv_len], bufsize-recv_len  , 0 );
		/* 受信ｴﾗｰ */
		if( err == -1 ){ 
			if(++sockerr > 10){
				printf( "Rcv Transfer failed.\n" );
				return(0);
			}
			else{
				Sleep(50);
				continue;
			}
		}

		/* 受信ﾃﾞｰﾀ長の更新・確認 */
		recv_len += err;
		sockerr=0;		// 2013.09.24 受信出来たらエラークリア

		// ﾃﾞｰﾀヘッダから受信長を抜き出す
		if(!chk){
			memcpy(buf,recvdat,5);
			buf[5]=0;
			dlen=atol(buf);
			chk=1;
		}

		// 受信長ﾁｪｯｸ
		if(dlen<=recv_len-5) break;
		else{
			if(++timeout >= 30){		//３秒待ってダメならタイムｱｳﾄ
				printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);
				return(-10);
			}
			Sleep(100);
		}
//		else return(0);
#ifdef _DEBUGPRINT
		printf("err=%d:len=%ld:rcv=%s\n",err,recv_len,recvdat);
#endif
	}

	// 受信バイト数を戻す
	return recv_len;
}

//===================================================================================================================
//	int  TcpServer::Send(char *msg,int sendlen)
//	機能　：バッファを介さず直送信。送信バイト数もつけない
//	引数　：送信バッファ
//	戻り値：送信バイト数
//	2008.04.24 coba 追加
//===================================================================================================================
int  TcpServer::SendDirect(char *msg,int sendlen,DWORD close_wait)
{
  	int		exit_flag=0;		// 初期化する 06.03.16 coba
	int		err;

    /* ﾃﾞｰﾀ送信 */
#ifdef _DEBUGPRINT
    printf( "\nWaiting to send data..." );
#endif
 	do{
	/* ﾃﾞｰﾀ送信待ち */
	    FD_ZERO( &r_socket );
	    FD_ZERO( &w_socket );
	    FD_ZERO( &e_socket );
	    FD_SET( m_Fd, &w_socket );
	    time.tv_sec  = 1;  /* ﾀｲﾑｱｳﾄ1秒 */
	    time.tv_usec = 0;
	    err = select( m_Fd+1, &r_socket, &w_socket, &e_socket, &time );
	    if(err < 0){
	        printf( "select() error\n" );
	        exit_flag = 1;
	        break;
      	}
	    else if( err == 0 ){
	        printf( "(send time out)\n" );  /* ﾀｲﾑｱｳﾄ発生 */
	    }
	    else if( FD_ISSET( m_Fd, &w_socket ) == 0 ){
	       	printf( "FD_ISSET() error\n" );
	        exit_flag = 1;
	        break;
	    }
	}while( err == 0 );

	// 送信する
	err = send( m_Fd, msg, sendlen, 0 );
    if( err == -1 ){
		printf( "Send Transfer failed.\n" );
    }
#ifdef _DEBUGPRINT
    printf( "Send Transfer complete.\n" );
#endif
    if(  exit_flag == 1 ) return -1;

	// 2008.04.25 coba	正しい終了まち
	WSAEVENT	hEvent;		//イベントハンドルの配列へのポインタ
	WSANETWORKEVENTS events;
	DWORD ret;
	hEvent = WSACreateEvent();
	if(hEvent == WSA_INVALID_EVENT)
	{
		closesocket(m_Fd);
        WSACleanup();
		return -4;
	}
	// 相手方のCloseを受け取ってから接続を閉じる
	// 距離が長い場合など、相手が受け取る前にこちらで閉じてしまうのを防ぐため
	err = WSAEventSelect(m_Fd,hEvent,FD_CLOSE );
	if(err == SOCKET_ERROR)
	{
        printf("\nWSAEventSelect()Err: %d\n", err);
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
		return -5;
	}

	if( close_wait < 1 ) close_wait=1000;
	ret=WSAWaitForMultipleEvents(1,&hEvent,FALSE,close_wait,FALSE);
	if (ret == WSA_WAIT_TIMEOUT)
    {
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
        return(-1);
    }
    ret = WSAEnumNetworkEvents(m_Fd,hEvent,&events);
   if (ret == SOCKET_ERROR)
   {
		WSACloseEvent(hEvent);
		closesocket(m_Fd);
        printf("WSAEnumNetworkEvents():Err=%d\n",WSAGetLastError());
        return(-2);
   }

// 確認用 2008.04.25
#ifdef _DEBUGPRINT
    if(events.lNetworkEvents & FD_CLOSE)
	{
		printf("Event close%d,\n",FD_CLOSE);
	}
	getchar();
#endif

	// イベントを閉じ、ソケットをcloseする
	WSACloseEvent(hEvent);
	closesocket( m_Fd );

	return 0;
}
