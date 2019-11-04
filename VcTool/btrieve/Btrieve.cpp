//========================================================================================
//
//	FileName	:BTRIEVE.CPP	( For Win32 )
//	Class		:Btrieve	(C)1982-1995 Btrieve Technologies, Inc. All Rights Reserved
// 				:BTRIEVEのAPIコールをする基本クラス
//	作成日		:1996.2.16	Coded by M.Ishida
//	Modefied	:ファイルパスをINIファイルから取得　By Koba	1996.10.11
//
//========================================================================================
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "btrapi.h"
#include "btrconst.h"
#include "btrieve.h"

//#include <noktoolw.h>
//#define TEST
//========================================================================================
// コンストラクト
//
//	Set Btrieve Parameters
//========================================================================================
Btrieve::Btrieve(void)
{
#ifdef TEST
	printf( "BTRIVER.CPP For Win95 Ver.1.0\n");
#endif
	memset( dataBuf, 0, sizeof( dataBuf ) );
	memset( keyBuf, 0, sizeof( keyBuf ) );

	fileOpen		= FALSE;
	btrieveLoaded 	= FALSE;
	fileCreatePtr 	= 0;
	fileCreateLen 	= 0;
	keyNum			= 0;
	keyNumMax		= 256;				//statコマンドよりキ－の数を読み込んで下さい
	dataLen 		= sizeof( dataBuf );
	memset( itsFileName, 0, sizeof(itsFileName) );

	clientID[ 0 ] = 0;clientID[ 1 ] = 0;clientID[ 2 ] = 0;clientID[ 3 ] = 0;
	clientID[ 4 ] = 0;clientID[ 5 ] = 0;clientID[ 6 ] = 0;clientID[ 7 ] = 0;
	clientID[ 8 ] = 0;clientID[ 9 ] = 0;clientID[10 ] = 0;clientID[11 ] = 0;
	clientID[12 ] = 'M';clientID[13 ] = 'T';clientID[14 ] = 0;clientID[15 ] = 1;

//		short a,b,c;
//		Version(&a,&b,&c);
//		StopID();
}

//========================================================================================
//	デストラクト
//========================================================================================
Btrieve::~Btrieve(void)
{

}
//========================================================================================
//	Btrieve ヴァ－ジョン表示
//
//		BTRIEVE NOW SUPPORTS BTRVID ON ALL PLATFORMS.	WE WILL NOW EXERCISE
//		BTRVID BY USING IT TO GET THE BTRIEVE VERSION.
//		REMEMBER: YOU CAN USE BTRVID TO PERFORM TRANSACTIONS OR RESETS ON A
//		PER-CLIENT BASIS FROM THE SAME APPLICATION.
//========================================================================================
int Btrieve::Version(short *Ver,short *Rev,short *Plat)
{
	dataLen=10;
	status = BTRVID( B_VERSION, posBlock, &dataBuf, &dataLen, keyBuf,keyNum, clientID );
	if ( status == B_NO_ERROR )
	{
#ifdef TEST
		printf( "Btrieve Version Is %d.%d %c\n",(*(BTI_WORD *)&dataBuf[ VERSION_OFFSET ]),
												(*(BTI_WORD *)&dataBuf[ REVISION_OFFSET ]),
												(*(BTI_CHAR *)&dataBuf[ PLATFORM_ID_OFFSET ] ));
#endif
		*Ver	=dataBuf[ VERSION_OFFSET ];
		*Rev	=dataBuf[ REVISION_OFFSET ];
		*Plat =dataBuf[ PLATFORM_ID_OFFSET ];
		btrieveLoaded = TRUE;
	}
	else
	{
#ifdef TEST
		printf( "Btrieve B_VERSION status = %d\n", status );
#endif
		if ( status != B_RECORD_MANAGER_INACTIVE )	btrieveLoaded = TRUE;
		*Ver = -1;
	}
	return((int)status);
}


//==================================================================================================
// int Btrieve::SetFileName(char *FileName, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
//	Btrieve ファイル名のセット
//
//	引数１：ファイルの名前（拡張子あり)
//	引数２：INIファイルの名前（INIファイルは実行ディレクトリと同じ場所に作成して下さい)
//	引数３：INIファイル内のセクション名([]でくくられた名称)
//	引数４：INIファイル名のキー名
//			＊文字列なのですべて最後にNullを付加する事！！
//	返値　：成功＝０,失敗＝１
//==================================================================================================
int Btrieve::SetFileName( char *FileName, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	char theDir[256];						// プロファイル（*.INIファイル）サーチパス格納用
	char theBtiPath[256];					// INIファイルから取得したBtiのFile Path格納
	int i;

	if( FileName[0]==0 )
		return(-1);

	memset( theDir, 0, sizeof(theDir) );
	memset( theBtiPath, 0, sizeof(theBtiPath) );

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

	strcat( theDir,ProfileName );			// 引数２のプロファイル名と結合


	// *.INIファイルからBTRIEVEファイルのパス設定を取得(取得失敗時は"FALSE"が入ります。)
	GetPrivateProfileString( SectionName, KeyName, "FALSE", 
								(LPTSTR)theBtiPath, sizeof(theBtiPath), (LPCTSTR)theDir );

	// 失敗時windowsディレクトリから探す
	if( memcmp( theBtiPath,"FALSE",5 )== 0 ){
		memset( theBtiPath, 0, sizeof(theBtiPath) );
		GetPrivateProfileString( SectionName, KeyName, "FALSE", 
							(LPTSTR)theBtiPath, sizeof(theBtiPath), (LPCTSTR)ProfileName );
		if( memcmp( theBtiPath,"FALSE",5 )== 0 ){
			strcpy( itsFileName, FileName );
			return(0);
		}
	}

	// 成功
	strcat( theBtiPath,FileName);
	strcpy( itsFileName, theBtiPath );
	//	WinPrintf("\nFile=%s",itsFileName);
	return(0);



}


//========================================================================================
// short Btrieve::SetDataLen(short Len)
//	レコ－ド長のセット
//
//	引数：データ長
//	返値：成功＝０,失敗＝１
//========================================================================================
int Btrieve::SetDataLen(short Len)
{
	if(Len>0 && Len<3800)
	{
		RecLen = (BTI_WORD)Len;
		return(0);
	}
	else
	{
		RecLen = 10;
		return(-1);
	}
}
//========================================================================================
//	int Btrieve::Create(void)
//	Btrieve ファイルの作成
//
//　引数：なし
//　返値：btrのerrorコード
//========================================================================================
int Btrieve::Create(void)
{
//	memset( keyBuf, 0, sizeof( keyBuf ) );

	if ( fileCreatePtr != 0 && itsFileName[0] != 0)
	{
		strcpy( (BTI_CHAR *)keyBuf, itsFileName );
		keyNum = 0;
//WinPrintf("filename=%s",keyBuf);
		status = BTRV( B_CREATE, posBlock, fileCreatePtr, &fileCreateLen, keyBuf, keyNum );
	}
	else status = -1;
#ifdef TEST
	printf( "Btrieve B_CREATE status = %d\n", status );
#endif
	return((int)status);
}

//========================================================================================
//	int Btrieve::CreateSpec(void)
//	Btrieveオ－バ－ライドして下さい．
//
//	 ファイルスペックの作成	CREATE THE BTRIEVE FILE Spec.
//	引数：なし
//	返値：成功＝０,失敗＝１
//========================================================================================
int Btrieve::CreateSpec(void)
{
	Key1Len = 21;
	Key2Len = 21;
	memset( &fileCreateBuf, 0, sizeof( fileCreateBuf ) ); /* CLEAR BUFFERS */

	if ( itsFileName[0] == 0 ) return(-1);

	fileCreateBuf.fileSpecs.recLength	 = RecLen;
	fileCreateBuf.fileSpecs.pageSize	 = 512;
	fileCreateBuf.fileSpecs.indexCount	 = 2;
	fileCreateBuf.fileSpecs.flags		 = 0;
	fileCreateBuf.fileSpecs.allocations	= 0;

	fileCreateBuf.keySpecs[ 0 ].position = 1;					/* index 1 */
	fileCreateBuf.keySpecs[ 0 ].length	 = Key1Len;
	fileCreateBuf.keySpecs[ 0 ].flags	 = EXTTYPE_KEY;
	fileCreateBuf.keySpecs[ 0 ].type	 = ZSTRING_TYPE;
	fileCreateBuf.keySpecs[ 0 ].null	 = 0;

	fileCreateBuf.keySpecs[1].position	 = 1+Key1Len;			/* index 2 */
	fileCreateBuf.keySpecs[1].length	 = Key2Len;
	fileCreateBuf.keySpecs[1].flags 	 = EXTTYPE_KEY;
	fileCreateBuf.keySpecs[1].type		 = ZSTRING_TYPE;
	fileCreateBuf.keySpecs[1].null		 = 0;

	fileCreatePtr = (BTI_BYTE*)&fileCreateBuf;
	fileCreateLen = sizeof(fileCreateBuf);

	return(0);
}


//========================================================================================
//	int Btrieve::Open(void)
//		Btrieve ファイルオ－プン
//
//　引数：なし
//　返値：btrのerrorコード
//========================================================================================
int Btrieve::Open(void)
{

	if ( fileOpen == FALSE && itsFileName[0]!=0 )
	{
		strcpy( (BTI_CHAR *)keyBuf, itsFileName );

		keyNum = 0;
		dataLen = 0;
		status = BTRV( B_OPEN, posBlock, dataBuf, &dataLen, keyBuf, keyNum );
#ifdef TEST
	printf("\nName=%s",itsFileName);
	printf( "Btrieve B_OPEN status = %d\n", status );
	//getchar();
#endif

		//WinPrintf("filename(at open )=%s,status=%d",keyBuf,status);
		if ( status == B_NO_ERROR )	 fileOpen = TRUE;
	}
	else{
		status = -1;
		return((int)status);
	}

	// キー長の取得 2004.01.
	BTI_BYTE dbuf[ sizeof(FILE_SPECS)+sizeof(KEY_SPECS)*BTI_KEYSEG_MAX+10 ];
	int num,i;
	FILE_SPECS* fs;
	KEY_SPECS* ks;

	memset( m_KeyLen, 0, sizeof(BTI_SINT)*BTI_KEYSEG_MAX);
	keyNum = 0;
	dataLen = sizeof(dbuf);
	status = BTRV( B_STAT, posBlock, dbuf, &dataLen, keyBuf, keyNum );
	if(status == B_NO_ERROR){
		// 各キーの長さを取得
		fs=(FILE_SPECS*)dbuf;
		num = 0;
		for( i=0; i < BTI_KEYSEG_MAX; i++){
			ks = (KEY_SPECS*)&dbuf[sizeof(FILE_SPECS)+sizeof(KEY_SPECS)*i];
			m_KeyLen[num]+=ks->length;
			if( ( ks->flags & SEG)==0 ) num++;
			if( num == fs->indexCount ) break;
		}
	}

	return((int)status);
}


//========================================================================================
//	int Btrieve::Setkey(short keyno)
//	 キ－番号をセットする
//
//	引数： short keyno　変更するキー№
//	返値：　成功＝０,失敗＝１
//========================================================================================
int Btrieve::SetKey(short keyno)
{
	if ( keyno < keyNumMax ){
		keyNum = (BTI_WORD)keyno; 
		return(0);
	}
	else{
		return(-1);
	}
}


//========================================================================================
//	int Btrieve ::Insert(char* Record)
//	 INSERT A RECORD INTO
//
//　引数：char *Record	追加するレコードﾃﾞｰﾀ
//　返値：btrのerrorコード
//========================================================================================
int Btrieve::Insert(char* Record)
{
	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;

		status = BTRV( B_INSERT, posBlock, Record,&Len, keyBuf, keyNum );
#ifdef TEST
		printf( "Btrieve B_INSERT status = %d\n", status );
#endif
	}
	else	status = -1;

	return((int)status);
}


//========================================================================================
//	int Btrieve::Delete(char* key, char* Record)
//		DELETE A RECORD
//
//　引数：char *key 		削除するレコードのキーデータ
//			char *Record		削除するレコードのポインタ
//　返値：btrのerrorコード
//========================================================================================
int Btrieve::Delete(char* key,char* Record)
{
	char dummy[4096];

	memset(m_Key, 0,sizeof(m_Key));
	memcpy(m_Key, key, m_KeyLen[keyNum]);

	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_EQUAL+S_WAIT_LOCK, posBlock, dummy,&Len, (BTI_CHAR *)m_Key, keyNum );
		if( status )
			return((int) status);

		status = BTRV( B_DELETE, posBlock, Record,&Len, (BTI_CHAR *)m_Key, keyNum );

#ifdef TEST
		printf( "Btrieve B_DELETE status = %d\n", status );
#endif
	}
	else	status = -1;

	return((int)status);
}


//========================================================================================
//	int Btrieve::Update(char* key, char* Record)
//		UPDATE A RECORD
//
//　引数：char *key 		変更前のキーデータ
//			char *Record		取得するレコードのポインタ
//　返値：btrのerrorコード
//========================================================================================
int Btrieve::Update(char* key,char* Record)
{
	char dummy[4096];

	memset(m_Key, 0,sizeof(m_Key));
	memcpy(m_Key, key, m_KeyLen[keyNum]);

	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_EQUAL+S_WAIT_LOCK, posBlock, dummy,&Len, (BTI_CHAR *)m_Key, keyNum );
		if( status ){
#ifdef TEST
			printf( "Btrieve B_GET_EQUAL status = %ld\n", status );
#endif
			return((int)status);
		}
		status = BTRV( B_UPDATE, posBlock, Record,&Len, (BTI_CHAR *)m_Key, keyNum );

#ifdef TEST

		printf( "Btrieve B_UPDATE status = %ld\n", status );
#endif
	}
	else	status = -1;

	return((int)status);
}


//========================================================================================
//	int Btrieve::GetFirst(char* Record)
//	 GET THE RECORD WITH GET_FIRST
//
//　引数：char* Record		取得データ
//　返値：btr-ErrorCode
//========================================================================================
int Btrieve::GetFirst(char* Record)
{
	if	( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_FIRST, posBlock, Record, &Len, keyBuf, keyNum );
#ifdef TEST
		printf( "Len=%d",Len );
		printf( "Btrieve B_Get_FIRST status = %d\n", status );
#endif
	}
	else status = -1;
	return((int)status);
}


//========================================================================================
//	int Btrieve::GetLast(char* Record)
//	 GET THE RECORD WITH GET_LAST
//
//　引数：char* Record		取得データ
//　返値：btr-ErrorCode
//========================================================================================
int Btrieve::GetLast(char* Record)
{
	if	( fileOpen != FALSE )
	{

		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_LAST, posBlock, Record, &Len, keyBuf, keyNum );
#ifdef TEST
		printf( "Btrieve B_Get_LAST status = %d\n", status );
#endif
	}
	else status = -1;
	return((int)status);
}


//========================================================================================
//	int Btrieve::GetNext(char* Record)
//	 GET THE RECORD WITH GET_NEXT
//
//　引数：char* Record		取得データ
//　返値：Btrieve Error Code
//========================================================================================
int Btrieve::GetNext(char* Record)
{
	if	( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_NEXT , posBlock, Record, &Len, keyBuf, keyNum );
#ifdef TEST
		printf( "Btrieve B_GET_NEXT status = %d\n", status );
#endif
	}
	else status = -1;
	return((int)status);
}


//========================================================================================
//	int Btrieve::GetPrev(char* Record)
//	 GET THE RECORD WITH GET_PREV
//
//　引数：char* Record		取得データ
//　返値：Btrieve Error Code
//========================================================================================
int Btrieve::GetPrev(char* Record)
{
	if	( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_PREVIOUS , posBlock, Record, &Len, keyBuf, keyNum );
#ifdef TEST
		printf( "Btrieve B_GET_PREV status = %d\n", status );
#endif
	}
	else status = -1;
	return((int)status);

}

//========================================================================================
//	int Btrieve::GetEqual(char* Record,char* Key)
//	 GET THE RECORD WITH GET_EQUAL
//
//　引数：char* Record		取得データ
//			char* key 		取得したいレコードのキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int Btrieve::GetEqual(char* Record,char* key)
{
	memset(m_Key, 0,sizeof(m_Key));
	memcpy(m_Key, key, m_KeyLen[keyNum]);

	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_EQUAL, posBlock, Record , &Len, (BTI_CHAR *)m_Key, keyNum );
#ifdef TEST
		printf( "Btrieve B_GET_EQUAL status = %d\n", status );
#endif
	}
	else status = -1;

	return( (int)status );
}


//========================================================================================
//	int BtrieveGetGE(char* Record,char* Key)
//	 GET THE RECORD WITH GET_Greater Than or EQUAL
//
//　引数：char* Record		取得データ
//			char* key 		取得したいレコードのキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int Btrieve::GetGE(char* Record,char* key)
{

	memcpy(m_Key, key, m_KeyLen[keyNum]);

	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_GE, posBlock, Record,&Len, (BTI_CHAR *)m_Key, keyNum );
#ifdef TEST
		printf( "Btrieve B_GET_GE status = %d\n", status );
#endif
	}
	else status = -1;

	return( (int)status );
}


//========================================================================================
//	int Btrieve::GetLE(char* Record,char* key)
//	 GET THE RECORD WITH GET_Less Than or EQUAL
//
//　引数：char* Record		取得データ
//			char* key 		取得したいレコードのキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int Btrieve::GetLE(char* Record,char* key)
{
	memcpy(m_Key, key, m_KeyLen[keyNum]);

	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_LE, posBlock, Record,&Len, (BTI_CHAR *)m_Key, keyNum );
#ifdef TEST
		printf( "Btrieve B_GET_LE status = %d\n", status );
#endif
	}
	else status = -1;

	return( (int)status );
}


//========================================================================================
//	int BtrieveGetGT(char* Record,char* Key)
//	 GET THE RECORD WITH GET_Greater Than
//
//　引数：char* Record		取得データ
//			char* key 		条件となるキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int Btrieve::GetGT(char* Record,char* key)
{
	memcpy(m_Key, key, m_KeyLen[keyNum]);
	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_GT, posBlock, Record,&Len, (BTI_CHAR *)m_Key, keyNum );
#ifdef TEST
		printf( "Btrieve B_GET_GT status = %d\n", status );
#endif
	}
	else status = -1;

	return( (int)status );
}


//========================================================================================
//	int Btrieve::GetLT(char* Record,char* key)
//	 GET THE RECORD WITH GET_Less Than
//
//　引数：char* Record		取得データ
//			char* key 		条件となるキーデータ
//　返値：Btrieve Error Code
//========================================================================================
int Btrieve::GetLT(char* Record,char* key)
{
	memcpy(m_Key, key, m_KeyLen[keyNum]);
	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_GET_LT, posBlock, Record,&Len, (BTI_CHAR *)m_Key, keyNum );
#ifdef TEST
		printf( "Btrieve B_GET_LT status = %d\n", status );
#endif
	}
	else status = -1;

	return( (int)status );
}

//========================================================================================
//	int Btrieve::StartTran()
//	 Start Transaction
//
//　引数：なし
//　返値：Btrieve Error Code
//========================================================================================
int Btrieve::StartTran(void)
{
	char Record[100];
	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_BEGIN_TRAN, posBlock, Record,&Len, keyBuf, keyNum );
#ifdef TEST
		printf( "Btrieve B_BEGIN_TRAN status = %d\n", status );
#endif
	}
	else status = -1;

	return((int)status );
}


//========================================================================================
//	int Btrieve::PauseTran()
//	 Pause Transaction
//
//　引数：なし
//　返値：Btrieve Error Code
//========================================================================================
int Btrieve::PauseTran(void)
{
	char Record[100];
	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_ABORT_TRAN, posBlock, Record,&Len, keyBuf, keyNum );
#ifdef TEST
		printf( "Btrieve B_ABORT_TRAN status = %d\n", status );
#endif
	}
	else status = -1;

	return( (int)status );
}


//========================================================================================
//	int Btrieve::EndTran()
//	 End Transaction
//
//　引数：なし
//　返値：Btrieve Error Code
//========================================================================================
int Btrieve::EndTran(void)
{
	char Record[100];
	if ( fileOpen != FALSE )
	{
		BTI_WORD Len = RecLen;
		status = BTRV( B_END_TRAN, posBlock, Record,&Len, keyBuf, keyNum );
#ifdef TEST
		printf( "Btrieve B_END_TRAN status = %d\n", status );
#endif
	}
	else status = -1;

	return( (int)status );
}


//========================================================================================
//	int Btrieve::Close(void)
//		Btrieve ファイル クロ－ズ
//		CLOSE THE FILE
//========================================================================================
int Btrieve::Close(void)
{
	if ( fileOpen != FALSE )
	{
		dataLen = 0;
		status = BTRV( B_CLOSE, posBlock, dataBuf, &dataLen, keyBuf, keyNum );
#ifdef TEST
		printf("Btrieve B_CLOSE status = %d\n", status );
#endif
		fileOpen = FALSE;
	}
	else status = -1;

	return((int)status );
}


//========================================================================================
//	Btrieve 停止
//
//		ISSUE THE BTRIEVE STOP OPERATION - For multi-tasking environments,
//		such as MS Windows, OS2, and NLM, 'stop' frees all Btrieve resources
//		for this client.	In DOS and Extended DOS, it removes the Btrieve
//		engine from memory, which we choose not to do in this example.	In
//		multi-tasking environments, the engine will not unload on 'stop'
//		unless it has no more clients.
//========================================================================================
int Btrieve::Stop(void)
{
	if ( btrieveLoaded )
	{
		dataLen = 0;
		status = BTRV( B_STOP, posBlock, dataBuf, &dataLen, keyBuf, keyNum );
#ifdef TEST
		printf( "Btrieve STOP status = %d\n", status );
#endif
		if ( status != B_NO_ERROR )	status = EXIT_WITH_ERROR;
	}
	else status = -1;

	return((int)status );
}

//========================================================================================
//	Btrieve 停止
//	RELEASE RESOURCES FOR clientID IN NON-DOS ENVIRONMENTS.
//
//
//
//========================================================================================
int Btrieve::StopID(void)
{
	status = BTRVID( B_STOP, posBlock, &dataBuf, &dataLen, keyBuf, keyNum, clientID );
#ifdef TEST
	printf( "Btrieve B_STOP status = %d\n", status );
#endif
	return((int)status);
}

//========================================================================================
// 関数名：BOOL Btrieve::IsExist(void)
// 操作対象のbtrieveファイルが存在するか調べる
// 戻り値：TRUE=あり、FALSE=なし
//========================================================================================
BOOL Btrieve::IsExist(void)
{
	char fname[MAX_PATH+1];
	WIN32_FIND_DATA fd;
	BOOL fRet=TRUE;
	HANDLE hFind;

	memset(fname,0, sizeof(fname));
	strcpy(fname,itsFileName);
	memset( &fd, 0, sizeof(fd) );

	// ファイル検索
	hFind = FindFirstFile( fname,&fd);
	if( hFind != INVALID_HANDLE_VALUE )	fRet=TRUE;
	else fRet=FALSE;
	// 無い
	FindClose(hFind);

	return(fRet);
}

//========================================================================================
// 関数名：int BtiTransaction(int mode)
// トランザクション。メンバ関数ではない! 2006.02.09
// 引数：トランザクションの種類
//			BTI_BEGIN,
//			BTI_COMMIT,
//			BTI_ROLLBACK,
// 戻り値：Btrieve Error Code
//========================================================================================
int BtiTransaction(int mode)
{
	BTI_WORD ope;
	BTI_SINT ret;

	// オペレーションの決定
//	if( mode == BTI_BEGIN) ope = B_BEGIN_TRAN;
	//2013.02.28 matsuda 排他トランザクションではなく並行トランザクションに変更
	if( mode == BTI_BEGIN) ope = B_BEGIN_TRAN + 1000;
	else if( mode == BTI_COMMIT) ope = B_END_TRAN;
	else if( mode == BTI_ROLLBACK) ope = B_ABORT_TRAN;
	else return(-1);

	ret = BTRV( ope, NULL, NULL, NULL, NULL, NULL );
	return((int)ret);
}

// 2006.08.04
BOOL Btrieve::GetOpenStatus(void)
{
	if( fileOpen== FALSE) return(FALSE);
	return(TRUE);
}
