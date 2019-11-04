//========================================================================================
//
//	FileName	:BTRIEVE.CPP	( For Win32 )
//	Class		:Btrieve	(C)1982-1995 Btrieve Technologies, Inc. All Rights Reserved
// 				:BTRIEVE��API�R�[���������{�N���X
//	�쐬��		:1996.2.16	Coded by M.Ishida
//	Modefied	:�t�@�C���p�X��INI�t�@�C������擾�@By Koba	1996.10.11
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
// �R���X�g���N�g
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
	keyNumMax		= 256;				//stat�R�}���h���L�|�̐���ǂݍ���ŉ�����
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
//	�f�X�g���N�g
//========================================================================================
Btrieve::~Btrieve(void)
{

}
//========================================================================================
//	Btrieve ���@�|�W�����\��
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
//	Btrieve �t�@�C�����̃Z�b�g
//
//	�����P�F�t�@�C���̖��O�i�g���q����)
//	�����Q�FINI�t�@�C���̖��O�iINI�t�@�C���͎��s�f�B���N�g���Ɠ����ꏊ�ɍ쐬���ĉ�����)
//	�����R�FINI�t�@�C�����̃Z�N�V������([]�ł�����ꂽ����)
//	�����S�FINI�t�@�C�����̃L�[��
//			��������Ȃ̂ł��ׂčŌ��Null��t�����鎖�I�I
//	�Ԓl�@�F�������O,���s���P
//==================================================================================================
int Btrieve::SetFileName( char *FileName, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName)
{
	char theDir[256];						// �v���t�@�C���i*.INI�t�@�C���j�T�[�`�p�X�i�[�p
	char theBtiPath[256];					// INI�t�@�C������擾����Bti��File Path�i�[
	int i;

	if( FileName[0]==0 )
		return(-1);

	memset( theDir, 0, sizeof(theDir) );
	memset( theBtiPath, 0, sizeof(theBtiPath) );

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

	strcat( theDir,ProfileName );			// �����Q�̃v���t�@�C�����ƌ���


	// *.INI�t�@�C������BTRIEVE�t�@�C���̃p�X�ݒ���擾(�擾���s����"FALSE"������܂��B)
	GetPrivateProfileString( SectionName, KeyName, "FALSE", 
								(LPTSTR)theBtiPath, sizeof(theBtiPath), (LPCTSTR)theDir );

	// ���s��windows�f�B���N�g������T��
	if( memcmp( theBtiPath,"FALSE",5 )== 0 ){
		memset( theBtiPath, 0, sizeof(theBtiPath) );
		GetPrivateProfileString( SectionName, KeyName, "FALSE", 
							(LPTSTR)theBtiPath, sizeof(theBtiPath), (LPCTSTR)ProfileName );
		if( memcmp( theBtiPath,"FALSE",5 )== 0 ){
			strcpy( itsFileName, FileName );
			return(0);
		}
	}

	// ����
	strcat( theBtiPath,FileName);
	strcpy( itsFileName, theBtiPath );
	//	WinPrintf("\nFile=%s",itsFileName);
	return(0);



}


//========================================================================================
// short Btrieve::SetDataLen(short Len)
//	���R�|�h���̃Z�b�g
//
//	�����F�f�[�^��
//	�Ԓl�F�������O,���s���P
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
//	Btrieve �t�@�C���̍쐬
//
//�@�����F�Ȃ�
//�@�Ԓl�Fbtr��error�R�[�h
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
//	Btrieve�I�|�o�|���C�h���ĉ������D
//
//	 �t�@�C���X�y�b�N�̍쐬	CREATE THE BTRIEVE FILE Spec.
//	�����F�Ȃ�
//	�Ԓl�F�������O,���s���P
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
//		Btrieve �t�@�C���I�|�v��
//
//�@�����F�Ȃ�
//�@�Ԓl�Fbtr��error�R�[�h
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

	// �L�[���̎擾 2004.01.
	BTI_BYTE dbuf[ sizeof(FILE_SPECS)+sizeof(KEY_SPECS)*BTI_KEYSEG_MAX+10 ];
	int num,i;
	FILE_SPECS* fs;
	KEY_SPECS* ks;

	memset( m_KeyLen, 0, sizeof(BTI_SINT)*BTI_KEYSEG_MAX);
	keyNum = 0;
	dataLen = sizeof(dbuf);
	status = BTRV( B_STAT, posBlock, dbuf, &dataLen, keyBuf, keyNum );
	if(status == B_NO_ERROR){
		// �e�L�[�̒������擾
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
//	 �L�|�ԍ����Z�b�g����
//
//	�����F short keyno�@�ύX����L�[��
//	�Ԓl�F�@�������O,���s���P
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
//�@�����Fchar *Record	�ǉ����郌�R�[�h�ް�
//�@�Ԓl�Fbtr��error�R�[�h
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
//�@�����Fchar *key 		�폜���郌�R�[�h�̃L�[�f�[�^
//			char *Record		�폜���郌�R�[�h�̃|�C���^
//�@�Ԓl�Fbtr��error�R�[�h
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
//�@�����Fchar *key 		�ύX�O�̃L�[�f�[�^
//			char *Record		�擾���郌�R�[�h�̃|�C���^
//�@�Ԓl�Fbtr��error�R�[�h
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
//�@�����Fchar* Record		�擾�f�[�^
//�@�Ԓl�Fbtr-ErrorCode
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
//�@�����Fchar* Record		�擾�f�[�^
//�@�Ԓl�Fbtr-ErrorCode
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
//�@�����Fchar* Record		�擾�f�[�^
//�@�Ԓl�FBtrieve Error Code
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
//�@�����Fchar* Record		�擾�f�[�^
//�@�Ԓl�FBtrieve Error Code
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
//�@�����Fchar* Record		�擾�f�[�^
//			char* key 		�擾���������R�[�h�̃L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
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
//�@�����Fchar* Record		�擾�f�[�^
//			char* key 		�擾���������R�[�h�̃L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
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
//�@�����Fchar* Record		�擾�f�[�^
//			char* key 		�擾���������R�[�h�̃L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
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
//�@�����Fchar* Record		�擾�f�[�^
//			char* key 		�����ƂȂ�L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
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
//�@�����Fchar* Record		�擾�f�[�^
//			char* key 		�����ƂȂ�L�[�f�[�^
//�@�Ԓl�FBtrieve Error Code
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
//�@�����F�Ȃ�
//�@�Ԓl�FBtrieve Error Code
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
//�@�����F�Ȃ�
//�@�Ԓl�FBtrieve Error Code
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
//�@�����F�Ȃ�
//�@�Ԓl�FBtrieve Error Code
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
//		Btrieve �t�@�C�� �N���|�Y
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
//	Btrieve ��~
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
//	Btrieve ��~
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
// �֐����FBOOL Btrieve::IsExist(void)
// ����Ώۂ�btrieve�t�@�C�������݂��邩���ׂ�
// �߂�l�FTRUE=����AFALSE=�Ȃ�
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

	// �t�@�C������
	hFind = FindFirstFile( fname,&fd);
	if( hFind != INVALID_HANDLE_VALUE )	fRet=TRUE;
	else fRet=FALSE;
	// ����
	FindClose(hFind);

	return(fRet);
}

//========================================================================================
// �֐����Fint BtiTransaction(int mode)
// �g�����U�N�V�����B�����o�֐��ł͂Ȃ�! 2006.02.09
// �����F�g�����U�N�V�����̎��
//			BTI_BEGIN,
//			BTI_COMMIT,
//			BTI_ROLLBACK,
// �߂�l�FBtrieve Error Code
//========================================================================================
int BtiTransaction(int mode)
{
	BTI_WORD ope;
	BTI_SINT ret;

	// �I�y���[�V�����̌���
//	if( mode == BTI_BEGIN) ope = B_BEGIN_TRAN;
	//2013.02.28 matsuda �r���g�����U�N�V�����ł͂Ȃ����s�g�����U�N�V�����ɕύX
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
