//=======================================================================================================
//	テキストDBAccessクラス
//	Class Name TTextTable
//	File Name TTextTable.cpp
//	作成者：Coba
//	作成日：1999.05.10
//	修正日：2004.11.12 ﾏﾂﾀﾞ		ネットワークでの競合対策のため、Insert2,Del2,Upd2を追加
//=======================================================================================================
#include "TTextTable.h"


//---------------------------------------------------------------------------------------------------------------------
//	関数名：TTextTable::TTextTable(void)
//	メンバ：public
//	機能　：コンストラクタ
//---------------------------------------------------------------------------------------------------------------------
TTextTable::TTextTable(void)
{
	m_PrevRecNo = 0;
	m_BufSize = DEF_BUFSIZE;
}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：TTextTable::TTextTable(void)
//	メンバ：public
//	機能　：デストラクタ
//---------------------------------------------------------------------------------------------------------------------
TTextTable::~TTextTable(void)
{
}


//---------------------------------------------------------------------------------------------------------------------
//	int TTextTable::Initialize(FSPEC *filespec, int bufsize)
//	メンバ：protected
//	機能　：ファイル構造の設定
//	引数　：構造定義用の構造体”FSPEC”
//	引数２：ファイル編集時のバッファーサイズ
//	戻り値：0=成功、その他=引数エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Initialize(FSPEC *filespec, int bufsize)
{
	// 構造をメンバ変数に保存
	memcpy( &m_fSpec, filespec, sizeof(FSPEC) );

	// specのチェック
	if( strlen( m_fSpec.filename) > 30 ) return(-1);
	if( !strlen( m_fSpec.filename) ) return(-1);
	if( m_fSpec.reclen < 0 ) return(-2);
	if( m_fSpec.keylen < 0 ) return(-3);
	if( m_fSpec.keylen > TXDB_MAXKEYLEN ) return(-4);	// サイズ変更　2003.05.06 Y.Kogai
	//if( m_fSpec.keylen > 50 ) return(-4);

	if( !strlen( m_fSpec.dir) ) strcpy( m_fSpec.dir,".\\");
	if( !strlen( m_fSpec.tmpdir) ) strcpy( m_fSpec.tmpdir,".\\");


	m_BufSize = bufsize;

	return(0);
}


//---------------------------------------------------------------------------------------------------------------------
//	関数名：void TTextTable::Clear(void)
//	メンバ：private
//	機能　：データバッファの初期化
//---------------------------------------------------------------------------------------------------------------------
void TTextTable::Clear( void )
{
	memset( m_fSpec.datptr, 0, m_fSpec.reclen );

}


//---------------------------------------------------------------------------------------------------------------------
//	関数名：void TTextTable::GetFirst( void *key )
//	メンバ：protected
//	機能　：最初のレコードを取得
//	引数　：検索キー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::GetFirst( void )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;

	// open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
	if(!dwRead) ret = 41;
	m_PrevRecNo = 1;
	// Close
	Close(hFile);
	return(ret);

}
//---------------------------------------------------------------------------------------------------------------------
//	関数名：void TTextTable::GetFirst( void *key )
//	メンバ：protected
//	機能　：最初のレコードを取得
//	引数　：検索キー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::GetLast( void )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;

	nowrec=GetCounts();
	if(nowrec<1) return(-1);

	// open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	// ポインタ飛ばす
	if( SetFilePointer( hFile, m_fSpec.reclen*(-1), NULL, FILE_END ) ==0xffffffff ){
		Close(hFile);
		return(9);
	}
	ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
	if(!dwRead) ret = 41;
	m_PrevRecNo = nowrec;
	// Close
	Close(hFile);
	return(ret);

}

int TTextTable::Next( void )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;

	// Open
	Clear();
	
	ret = Open( hFile,TRUE);
	if( ret ) return(ret);
	
	// ポインタ飛ばす
	if( SetFilePointer( hFile, m_fSpec.reclen*m_PrevRecNo, NULL, FILE_BEGIN ) ==0xffffffff ){
		Close(hFile);
		return(9);
	}

	ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
	if(!dwRead)
		ret = 41;
	else 
		m_PrevRecNo++;

	// Close
	Close(hFile);
	return(ret);
}
int TTextTable::Prev( void )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;

	nowrec = m_PrevRecNo-2;
	if( nowrec < 0 ) return(42);


	// Open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	// ポインタ飛ばす
	if( SetFilePointer( hFile, m_fSpec.reclen*nowrec, NULL, FILE_BEGIN ) ==0xffffffff ){
		Close(hFile);
		return(9);
	}

	ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
	if(!dwRead)
		ret = 41;
	else 
		m_PrevRecNo--;

	// Close
	Close(hFile);
	return(ret);
}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：void TTextTable::GetEqual( void *key )
//	メンバ：protected
//	機能　：レコード検索/キーに合致するレコードを検索（ユニークキー対象）
//	引数　：検索キー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::GetEqual( void *key )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;

	// Open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	// Find
	ret = MoveRecord( hFile, key, &nowrec );

	// 見つかった場合読み出す
	if( !ret ){
		ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
		m_PrevRecNo = nowrec;
	}
	// Close
	Close(hFile);
	return(ret);

}
//---------------------------------------------------------------------------------------------------------------------
//	関数名：void TTextTable::GetEqual2( void *key )
//	メンバ：protected
//	機能　：レコード検索/キーに合致するレコードを検索（ユニークキー対象）
//	引数　：検索キー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::GetEqual2( void *key )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;

	//ファイルは作業中か？
	ret = CheckFile();
	//現在作業中
	if(ret < 0) return TXDB_WORKING;
	
	// Open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	// Find
	ret = MoveRecord( hFile, key, &nowrec );

	// 見つかった場合読み出す
	if( !ret ){
		ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
		m_PrevRecNo = nowrec;
	}
	// Close
	Close(hFile);
	return(ret);

}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：void TTextTable::GetEqual( void *key )
//	メンバ：protected
//	機能　：レコード検索/キーに合致する最初のレコードを検索（ユニークキー対象）
//	引数　：検索キー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::FindFirst( void *key )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;
	char prevKey[256];

	// Open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	// Find
	ret = MoveRecord( hFile, key, &nowrec );

	// 見つかった場合1レコードずつバックしながら読み出す
	if( !ret ){
		ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
		SetFilePointer( hFile, -1*( m_fSpec.reclen - m_fSpec.offset -m_fSpec.keylen ), NULL, FILE_CURRENT );

		// キーが変わるまでバック
		while( nowrec > 1 ){
			// 前のﾚｺｰﾄﾞのキー位置まで戻る
			SetFilePointer( hFile, -1*(m_fSpec.reclen+m_fSpec.keylen), NULL, FILE_CURRENT );
			ReadFile( hFile, prevKey, m_fSpec.keylen, &dwRead, NULL );

			// キーが変わった場合はレコード取得
			if( memcmp( key, prevKey, m_fSpec.keylen ) ){
				SetFilePointer( hFile, (m_fSpec.reclen-m_fSpec.offset-m_fSpec.keylen), NULL, FILE_CURRENT );
				ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
				break;
			}

			nowrec --;
			// ファイルの先頭
			if( nowrec==1 ){
				SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
				ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
				break;
			}
		}

		m_PrevRecNo = nowrec;
	}


	// Close
	Close(hFile);
	return(ret);

}


//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::Upd( void *key )
//	メンバ：protected
//	機能　：
//	引数　：更新するキー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Upd( void *key )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;


	// Open
	ret = Open( hFile, FALSE);
	if( ret ) return(ret);

	// 指定ﾚｺｰﾄﾞの有無を調べる
	ret = MoveRecord( hFile, key ,&nowrec );

	if(!ret){
		WriteFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
	}

	// Close
	Close(hFile);
	return(ret);

}
//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::Upd( void *key )
//	メンバ：protected
//	機能　：Updを改良し、処理前にマスタファイル名を変更し、処理中は他からのアクセスが出来ないよう変更
//	引数　：更新するキー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Upd2( void *key )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;
	char pcName[MAX_COMPUTERNAME_LENGTH + 1 ];
	DWORD pcNameLen = MAX_COMPUTERNAME_LENGTH + 1;
	char newFileName[MAX_PATH+1];
	char newPath[MAX_PATH+1];
	char oldFileName[MAX_PATH+1];
	char fname[MAX_PATH+1];
	//PC名取得 --拡張子に使う--
	memset(pcName,0,sizeof(pcName));
	GetComputerName(pcName,&pcNameLen);

	//元のファイル名を退避
	wsprintf( oldFileName, m_fSpec.filename );
	
	// ファイル名から拡張子をはずす
	memset( fname ,0, sizeof(fname) );
	memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
	if(!strlen(fname)){
		return(26);
	}
	if( strtok(fname, ".") ==NULL ){
		return(27);
	}
	wsprintf( newFileName, "%s.%s", fname ,pcName);				//新規ファイル名
	wsprintf( newPath, "%s\\%s",m_fSpec.dir,newFileName);	//全てのパス

	//元のファイル名
	memset(fname,0,sizeof(fname));
	wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );

	//元ファイルは存在するか？
	ret = CheckFile();
	//存在した場合
	if(!ret){
		DeleteFile(newPath);
		//ファイル名変更!!
		if( !MoveFile(fname ,newPath) ){
			//失敗したら
			return TXDB_WORKING;
		}
		else{
			//m_fSpec.filenameを書き換え
			wsprintf( m_fSpec.filename , newFileName );
		}
	}
	//ファイルが本当にない→新規作成
	else if(ret == 10){
		//m_fSpec.filenameを書き換え
		wsprintf( m_fSpec.filename , newFileName );
	}
	//作業中の場合
	else return TXDB_WORKING;

	// Open
	ret = Open( hFile, FALSE);
	if( ret ){
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(ret);
	}

	// 指定ﾚｺｰﾄﾞの有無を調べる
	ret = MoveRecord( hFile, key ,&nowrec );

	if(!ret){
		WriteFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
	}

	// Close
	Close(hFile);
	
	//もとのファイル名に戻す
	wsprintf( m_fSpec.filename ,"%s",oldFileName );
	wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
	MoveFile( newPath, fname );
	
	return(ret);

}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::Insert( void *key )
//	メンバ：protected
//	機能　：
//	引数　：削除するキー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Insert( void *key )
{
	HANDLE hFile,hTmp;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;


	// Open
	ret = Open( hFile, FALSE);
	if( ret ) return(ret);

	// 指定ﾚｺｰﾄﾞの有無と挿入位置調べる
	ret = MoveRecord( hFile, key ,&nowrec);

	// いったん閉じる
	Close(hFile);

	// 見つかった場合は追加しない
	if( ret !=4 ) return(5);

	// 更新
	if( !m_fSpec.reclen ) return(11);

	// 読みとりバッファ
	int readCount,i;
	DWORD  totalSize;
	char tmpFile[MAX_PATH+1];		// *.tmpファイル名
	char savFile[MAX_PATH+1];		// *.savファイル名
	char fname[MAX_PATH+1];

	// テンポラリファイルの作成(filename.tmp)
	memset( tmpFile, 0, sizeof(tmpFile) );
	memset( savFile, 0, sizeof(savFile) );
	memset( fname, 0, sizeof(fname) );

	// ファイル名から拡張子をはずす
	memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
	if(!strlen(fname)){
		return(16);
	}
	if( strtok(fname, ".") ==NULL ){
		return(17);
	}
	wsprintf( tmpFile, "%s\\%s.tmp",m_fSpec.tmpdir, fname );
	wsprintf( savFile, "%s\\%s.sav",m_fSpec.tmpdir, fname );

	hTmp = CreateFile( tmpFile, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if( hTmp == INVALID_HANDLE_VALUE ){
		return(12);// open err
	}
	// マスター再オープン
	ret = Open( hFile, FALSE);
	if( ret ){
		Close(hTmp);
		DeleteFile( tmpFile );
		return(13);
	}

	// 現在の登録ﾚｺｰﾄﾞ件数
	if( ( totalSize = GetFileSize(hFile, NULL)) == 0xffffffff ){
		Close(hTmp);
		Close(hFile);
		DeleteFile( tmpFile );
		return(14);
	}

	// 読みとりバッファ
	char *readBuf= new char[m_BufSize];

	// 追加位置の直前のﾚｺｰﾄﾞまでの読みとり回数（１回あたりbufSizeまとめて読む）を算出
	readCount = (nowrec * m_fSpec.reclen ) /m_BufSize;

	// 削除前までのデータをコピー
	// Block Read
	for( i=0; i < readCount; i++){
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
	}

	// 余り部分を読む
	memset( readBuf, 0, m_BufSize );
	ReadFile( hFile, readBuf, (nowrec * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );
	WriteFile( hTmp, readBuf, (nowrec * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );

	// 該当ﾚｺｰﾄﾞを書き込む
	WriteFile( hTmp, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );

	// 追加後－EOFまでをコピー
	readCount = (totalSize - (nowrec*m_fSpec.reclen )) / m_BufSize;

	for( i=0; i < readCount; i++){
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
	}

	// 余り部分を読む
	memset( readBuf, 0, m_BufSize );
	ReadFile( hFile, readBuf, (totalSize - (nowrec*m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );
	WriteFile( hTmp, readBuf, (totalSize - (nowrec*m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );

	// ファイルクローズ
	Close(hFile);
	Close(hTmp);

	// リネーム
	wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );
	DeleteFile( savFile);
	if( MoveFile( fname, savFile ) == FALSE ){
		DeleteFile( tmpFile );
		delete readBuf;
		return(15);
	}

	if( MoveFile( tmpFile, fname ) == FALSE ){
		DeleteFile( tmpFile);
		DeleteFile( savFile);
		delete readBuf;
		return(15);
	}

	// マスター削除
	DeleteFile( savFile );

	delete readBuf;
	return(ret);
}
//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::Insert2( void *key )
//	メンバ：protected
//	機能　：Insertを改良し、処理前にマスタファイル名を変更し、処理中は他からのアクセスが出来ないよう変更
//	引数　：削除するキー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Insert2( void *key )
{
	HANDLE hFile,hTmp;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;
	char tmpFile[MAX_PATH+1];		// *.tmpファイル名
	char fname[MAX_PATH+1];
	char pcName[MAX_COMPUTERNAME_LENGTH + 1 ];
	DWORD pcNameLen = MAX_COMPUTERNAME_LENGTH + 1;
	char newFileName[MAX_PATH+1];
	char newPath[MAX_PATH+1];
	char oldFileName[MAX_PATH+1];
	//PC名取得 --拡張子に使う--
	memset(pcName,0,sizeof(pcName));
	GetComputerName(pcName,&pcNameLen);

	//元のファイル名を退避
	wsprintf( oldFileName, m_fSpec.filename );
	
	// ファイル名から拡張子をはずす
	memset( fname ,0, sizeof(fname) );
	memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
	if(!strlen(fname)){
		return(26);
	}
	if( strtok(fname, ".") ==NULL ){
		return(27);
	}
	wsprintf( newFileName, "%s.%s", fname ,pcName);				//新規ファイル名
	wsprintf( newPath, "%s\\%s",m_fSpec.dir,newFileName);	//全てのパス

	//元のファイル名
	memset(fname,0,sizeof(fname));
	wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );

	//元ファイルは存在するか？
	ret = CheckFile();
	//存在した場合
	if(!ret){
		DeleteFile(newPath);
		if( !MoveFile(fname ,newPath) ){
			//失敗したら
			return TXDB_WORKING;
		}
		else{
			//m_fSpec.filenameを書き換え
			wsprintf( m_fSpec.filename , newFileName );
		}
	}
	//ファイルが本当にない→新規作成
	else if(ret == 10){
		//m_fSpec.filenameを書き換え
		wsprintf( m_fSpec.filename , newFileName );
	}
	//作業中の場合
	else return TXDB_WORKING;
	
	// Open 
	ret = Open( hFile, FALSE);
	if( ret ){
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(ret);		//ﾌｧｲﾙｵｰﾌﾟﾝ失敗したら抜ける
	}

	// 指定ﾚｺｰﾄﾞの有無と挿入位置調べる
	ret = MoveRecord( hFile, key ,&nowrec);

	// いったん閉じる
	Close(hFile);

	// 見つかった場合は追加しない
	if( ret !=4 ){
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(5);
	}

	// 更新
	if( !m_fSpec.reclen ){
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(11);
	}

	// 読みとりバッファ
	int readCount,i;
	DWORD  totalSize;
	char savFile[MAX_PATH+1];		// *.savファイル名

	// テンポラリファイルの作成(filename.tmp)
	memset( tmpFile, 0, sizeof(tmpFile) );
	memset( savFile, 0, sizeof(savFile) );
	memset( fname, 0, sizeof(fname) );

	// ファイル名から拡張子をはずす
	memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
	if(!strlen(fname)){
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(16);
	}
	if( strtok(fname, ".") ==NULL ){
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(17);
	}
	wsprintf( tmpFile, "%s\\%s.tmp",m_fSpec.tmpdir, fname );
	wsprintf( savFile, "%s\\%s.sav",m_fSpec.tmpdir, fname );

	//ﾌｧｲﾙ新規作成
	hTmp = CreateFile( tmpFile, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if( hTmp == INVALID_HANDLE_VALUE ){
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(12);// open err
	}
	// マスター再オープン
	ret = Open( hFile, FALSE);
	if( ret ){
		Close(hTmp);
		DeleteFile( tmpFile );
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(13);
	}

	// 現在の登録ﾚｺｰﾄﾞ件数
	if( ( totalSize = GetFileSize(hFile, NULL)) == 0xffffffff ){
		Close(hTmp);
		Close(hFile);
		DeleteFile( tmpFile );
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(14);
	}

	// 読みとりバッファ
	char *readBuf= new char[m_BufSize];

	// 追加位置の直前のﾚｺｰﾄﾞまでの読みとり回数（１回あたりbufSizeまとめて読む）を算出
	readCount = (nowrec * m_fSpec.reclen ) /m_BufSize;

	// 削除前までのデータをコピー
	// Block Read
	for( i=0; i < readCount; i++){
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
	}

	// 余り部分を読む
	memset( readBuf, 0, m_BufSize );
	ReadFile( hFile, readBuf, (nowrec * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );
	WriteFile( hTmp, readBuf, (nowrec * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );

	// 該当ﾚｺｰﾄﾞを書き込む
	WriteFile( hTmp, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );

	// 追加後－EOFまでをコピー
	readCount = (totalSize - (nowrec*m_fSpec.reclen )) / m_BufSize;

	for( i=0; i < readCount; i++){
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
	}

	// 余り部分を読む
	memset( readBuf, 0, m_BufSize );
	ReadFile( hFile, readBuf, (totalSize - (nowrec*m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );
	WriteFile( hTmp, readBuf, (totalSize - (nowrec*m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );

	// ファイルクローズ
	Close(hFile);
	Close(hTmp);

	// リネーム
	wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );
	DeleteFile( savFile);
	if( MoveFile( fname, savFile ) == FALSE ){
		DeleteFile( tmpFile );
		delete readBuf;
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(15);
	}

	//もとのファイル名に戻す
	wsprintf( m_fSpec.filename ,"%s",oldFileName );
	wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
	if( MoveFile( tmpFile, fname ) == FALSE ){
		DeleteFile( tmpFile);
		MoveFile(savFile,fname);
		delete readBuf;
		return(15);
	}

	// マスター削除
	DeleteFile( savFile );

	delete readBuf;
	return(ret);
}


//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::Del( void *key )
//	メンバ：protected
//	機能　：
//	引数　：削除するキー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Del( void *key )
{
	HANDLE hFile,hTmp;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;


	// Open
	ret = Open( hFile, FALSE);
	if( ret ) return(ret);

	// 指定ﾚｺｰﾄﾞの有無を調べる
	ret = MoveRecord( hFile, key ,&nowrec);

	// いったん閉じる
	Close(hFile);

	// 見つかった場合削除する
	if( !ret ){
		if( !m_fSpec.reclen ) return(11);

		int readCount,i;
		DWORD  totalSize;
		char tmpFile[MAX_PATH+1];		// *.tmpファイル名
		char savFile[MAX_PATH+1];		// *.savファイル名
		char fname[MAX_PATH+1];

		// テンポラリファイルの作成(filename.tmp)
		memset( tmpFile, 0, sizeof(tmpFile) );
		memset( savFile, 0, sizeof(savFile) );
		memset( fname, 0, sizeof(fname) );

		// ファイル名から拡張子をはずす
		memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
		if(!strlen(fname)) return(16);
		if( strtok(fname, ".") ==NULL ) return(17);

		wsprintf( tmpFile, "%s\\%s.tmp",m_fSpec.tmpdir, fname );
		wsprintf( savFile, "%s\\%s.sav",m_fSpec.tmpdir, fname );

		hTmp = CreateFile( tmpFile, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if( hTmp == INVALID_HANDLE_VALUE )			return(12);// open err

		// マスター再オープン
		ret = Open( hFile, FALSE);
		if( ret ){
			Close(hTmp);
			DeleteFile( tmpFile );
			return(13);
		}

		// 現在の登録ﾚｺｰﾄﾞ件数
		if( ( totalSize = GetFileSize(hFile, NULL)) == 0xffffffff ){
			Close(hTmp);
			Close(hFile);
			DeleteFile( tmpFile );
			return(14);
		}

		// 読みとりバッファ
		char *readBuf= new char[m_BufSize];

		// 削除ﾚｺｰﾄﾞまでの読みとり回数（１回あたりm_BufSizeまとめて読む）を算出
		readCount = ( (nowrec-1) * m_fSpec.reclen ) /m_BufSize;

		// 削除前までのデータをコピー
		// Block Read
		for( i=0; i < readCount; i++){
			memset( readBuf, 0, m_BufSize );
			ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
			WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
		}

		// 余り部分を読む
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, ( (nowrec-1) * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, ( (nowrec-1) * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );

		// 該当ﾚｺｰﾄﾞは飛ばす
		if( SetFilePointer( hFile, m_fSpec.reclen, NULL, FILE_CURRENT) ==0xffffffff ){
			Close(hTmp);
			Close(hFile);
			DeleteFile( tmpFile );
			return(15);
		}

		// 削除後－EOFまでをコピー
		readCount = (totalSize - (nowrec*m_fSpec.reclen )) / m_BufSize;

		for( i=0; i < readCount; i++){
			memset( readBuf, 0, m_BufSize );
			ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
			WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
		}

		// 余り部分を読む
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, (totalSize - ( nowrec * m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, (totalSize - ( nowrec * m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );

		// ファイルクローズ
		Close(hFile);
		Close(hTmp);

		// リネーム
		wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );
		DeleteFile( savFile);
		if( MoveFile( fname, savFile ) == FALSE ){
			DeleteFile( tmpFile );
			delete readBuf;
			return(15);
		}

		if( MoveFile( tmpFile, fname ) == FALSE ){
			DeleteFile( tmpFile);
			DeleteFile( savFile);
			delete readBuf;
			return(15);
		}

		// マスター削除
		DeleteFile( savFile );
		delete readBuf;

	}

	return(ret);
}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::Del2( void *key )
//	メンバ：protected
//	機能　：Delを改良し、処理前にマスタファイル名を変更し、処理中は他からのアクセスが出来ないよう変更
//	引数　：削除するキー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Del2( void *key )
{
	HANDLE hFile,hTmp;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;

	char fname[MAX_PATH+1];
	char pcName[MAX_COMPUTERNAME_LENGTH + 1 ];
	DWORD pcNameLen = MAX_COMPUTERNAME_LENGTH + 1;
	char newFileName[MAX_PATH+1];
	char newPath[MAX_PATH+1];
	char oldFileName[MAX_PATH+1];
	//PC名取得 --拡張子に使う--
	memset(pcName,0,sizeof(pcName));
	GetComputerName(pcName,&pcNameLen);

	//元のファイル名を退避
	wsprintf( oldFileName, m_fSpec.filename );
	
	// ファイル名から拡張子をはずす
	memset( fname ,0, sizeof(fname) );
	memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
	if(!strlen(fname)){
		return(26);
	}
	if( strtok(fname, ".") ==NULL ){
		return(27);
	}
	wsprintf( newFileName, "%s.%s", fname ,pcName);				//新規ファイル名
	wsprintf( newPath, "%s\\%s",m_fSpec.dir,newFileName);	//全てのパス

	//元のファイル名
	memset(fname,0,sizeof(fname));
	wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );

	//元ファイルは存在するか？
	ret = CheckFile();
	//存在した場合
	if(!ret){
		DeleteFile(newPath);
		if( !MoveFile(fname ,newPath) ){
			//失敗したら
			return TXDB_WORKING;
		}
		else{
			//m_fSpec.filenameを書き換え
			wsprintf( m_fSpec.filename , newFileName );
		}
	}
	//ファイルが本当にない→新規作成
	else if(ret == 10){
		//m_fSpec.filenameを書き換え
		wsprintf( m_fSpec.filename , newFileName );
	}
	//作業中の場合
	else return TXDB_WORKING;
	
	// Open
	ret = Open( hFile, FALSE);
	if( ret ){
		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(ret);
	}

	// 指定ﾚｺｰﾄﾞの有無を調べる
	ret = MoveRecord( hFile, key ,&nowrec);

	// いったん閉じる
	Close(hFile);

	// 見つかった場合削除する
	if( !ret ){
		if( !m_fSpec.reclen ){
			//もとのファイル名に戻す
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(11);
		}

		int readCount,i;
		DWORD  totalSize;
		char tmpFile[MAX_PATH+1];		// *.tmpファイル名
		char savFile[MAX_PATH+1];		// *.savファイル名
		char fname[MAX_PATH+1];

		// テンポラリファイルの作成(filename.tmp)
		memset( tmpFile, 0, sizeof(tmpFile) );
		memset( savFile, 0, sizeof(savFile) );
		memset( fname, 0, sizeof(fname) );

		// ファイル名から拡張子をはずす
		memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
		if(!strlen(fname)){
			//もとのファイル名に戻す
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(16);
		}
		if( strtok(fname, ".") ==NULL ){
			//もとのファイル名に戻す
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(17);
		}

		wsprintf( tmpFile, "%s\\%s.tmp",m_fSpec.tmpdir, fname );
		wsprintf( savFile, "%s\\%s.sav",m_fSpec.tmpdir, fname );

		hTmp = CreateFile( tmpFile, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if( hTmp == INVALID_HANDLE_VALUE ){
			//もとのファイル名に戻す
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(12);// open err
		}

		// マスター再オープン
		ret = Open( hFile, FALSE);
		if( ret ){
			Close(hTmp);
			DeleteFile( tmpFile );
			//もとのファイル名に戻す
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(13);
		}

		// 現在の登録ﾚｺｰﾄﾞ件数
		if( ( totalSize = GetFileSize(hFile, NULL)) == 0xffffffff ){
			Close(hTmp);
			Close(hFile);
			DeleteFile( tmpFile );
			//もとのファイル名に戻す
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(14);
		}

		// 読みとりバッファ
		char *readBuf= new char[m_BufSize];

		// 削除ﾚｺｰﾄﾞまでの読みとり回数（１回あたりm_BufSizeまとめて読む）を算出
		readCount = ( (nowrec-1) * m_fSpec.reclen ) /m_BufSize;

		// 削除前までのデータをコピー
		// Block Read
		for( i=0; i < readCount; i++){
			memset( readBuf, 0, m_BufSize );
			ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
			WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
		}

		// 余り部分を読む
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, ( (nowrec-1) * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, ( (nowrec-1) * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );

		// 該当ﾚｺｰﾄﾞは飛ばす
		if( SetFilePointer( hFile, m_fSpec.reclen, NULL, FILE_CURRENT) ==0xffffffff ){
			Close(hTmp);
			Close(hFile);
			DeleteFile( tmpFile );
			//もとのファイル名に戻す
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(15);
		}

		// 削除後－EOFまでをコピー
		readCount = (totalSize - (nowrec*m_fSpec.reclen )) / m_BufSize;

		for( i=0; i < readCount; i++){
			memset( readBuf, 0, m_BufSize );
			ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
			WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
		}

		// 余り部分を読む
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, (totalSize - ( nowrec * m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, (totalSize - ( nowrec * m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );

		// ファイルクローズ
		Close(hFile);
		Close(hTmp);

		// リネーム
		wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );
		DeleteFile( savFile);
		if( MoveFile( fname, savFile ) == FALSE ){
			DeleteFile( tmpFile );
			delete readBuf;
			//もとのファイル名に戻す
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(15);
		}

		//もとのファイル名に戻す
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		if( MoveFile( tmpFile, fname ) == FALSE ){
			DeleteFile( tmpFile);
			DeleteFile( savFile);
			delete readBuf;
			return(15);
		}

		// マスター削除
		DeleteFile( savFile );
		delete readBuf;
	}
	return(ret);
}



//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::MoveRecord( HANDLE hFile, void *key )
//	メンバ：private
//	機能　：引数で指定されたキーのﾚｺｰﾄﾞを検索し、該当レコードの先頭にファイルポインタを移動する
//	引数１：検索するファイルのハンドル
//	引数２：検索するキー
//	引数３：該当ﾚｺｰﾄﾞのﾚｺｰﾄﾞ番号（出力）
//	戻り値：0=成功、その他=エラー
//						＊ファイルオープンされてることが前提！
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::MoveRecord2( HANDLE hFile,void *key ,long *recNo )
{
	int ret;
	char keybuf[TXDB_MAXKEYLEN+1];
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead;

	*recNo = 0;

	// キーの位置までずらす
	if( SetFilePointer( hFile, m_fSpec.offset, NULL, FILE_BEGIN ) ==0xffffffff ){
		return(4);
	}

	while(1){
		// 読み出し(key部分のみ)
		memset(keybuf, 0, sizeof(keybuf) );
		if( ReadFile( hFile, keybuf, m_fSpec.keylen, &dwRead, NULL )==FALSE){
			ret = 2;
			break;
		}

		if( !dwRead  ){
			ret = 9;
			break;
		}

		// 検索キーとの比較
		if( !memcmp( key, keybuf, m_fSpec.keylen) ){
			// 該当レコードの場合は、ポインタをレコード先頭に戻す
			SetFilePointer( hFile, -1*(m_fSpec.offset+m_fSpec.keylen), NULL, FILE_CURRENT );
			ret = 0;

			// カレント行番号をメンバ変数に保存
			m_PrevRecNo = nowrec;
			*recNo = nowrec;
			break;
		}

		// レコード移動
		if( SetFilePointer( hFile, (m_fSpec.reclen - m_fSpec.keylen), NULL, FILE_CURRENT ) ==0xffffffff){
			ret = 4;
			break;
		}
		nowrec ++;
	}

	return(ret);
}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::MoveRecord( HANDLE hFile, void *key )
//	メンバ：private
//	機能　：引数で指定されたキーのﾚｺｰﾄﾞを検索し、該当レコードにファイルポインタを移動する
//			該当レコードが無いときはキーに一番近い直前のレコードに移動
//	引数１：検索するファイルのハンドル
//	引数２：検索するキー
//	引数３：該当ﾚｺｰﾄﾞのﾚｺｰﾄﾞ番号（出力）
//	戻り値：0=成功、その他=エラー
//						＊ファイルオープンされてることが前提！
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::MoveRecord( HANDLE hFile,void *key ,long *recNo )
{
	int cmpstat,i;
	char keybuf[TXDB_MAXKEYLEN+1];
	long nowrec=0;					// 現在のレコード位置
	DWORD dwRead,fsize;
	long under,now,over;


	*recNo = 0;

	// ファイルサイズ取得
	fsize = GetFileSize( hFile, NULL );
	// データ空
	if( !fsize )		return(4);


	// 先頭と比較
	SetFilePointer( hFile,m_fSpec.offset, NULL,FILE_BEGIN );
	if( ReadFile( hFile, keybuf, m_fSpec.keylen, &dwRead, NULL )==FALSE) return(2);
	cmpstat = memcmp( key, keybuf ,m_fSpec.keylen );
	// 先頭より前に当たる場合
	if( cmpstat < 0 ){
		SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
		return(4);
	}
	// 先頭データだった場合
	if( cmpstat ==0){
		SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
		*recNo = 1;
		return(0);
	}

	// 最後尾と比較
	SetFilePointer( hFile,-(m_fSpec.reclen-m_fSpec.offset), NULL,FILE_END );
	if( ReadFile( hFile, keybuf, m_fSpec.keylen, &dwRead, NULL )==FALSE) return(2);
	cmpstat = memcmp( key, keybuf, m_fSpec.keylen );
	// 最後尾に当たる場合
	if( cmpstat > 0 ){
		SetFilePointer( hFile, 0, NULL, FILE_END );
		*recNo = fsize/m_fSpec.reclen;
		return(4);
	}
	// 該当データだった場合
	if( cmpstat ==0){
		SetFilePointer( hFile, -m_fSpec.reclen, NULL, FILE_END );
		*recNo = fsize/m_fSpec.reclen;
		return(0);
	}

	under = 1;
	over = fsize/m_fSpec.reclen;

	// 2分検索
	while(1){
		// under,overの中間のレコードをチェックする
		now=( over+under) / 2;
		SetFilePointer( hFile, ( (now-1) * m_fSpec.reclen) +m_fSpec.offset, NULL, FILE_BEGIN );

		// 読み込み
		if( ReadFile( hFile, keybuf, m_fSpec.keylen, &dwRead, NULL )==FALSE) return(2);

		// 比較
		cmpstat = memcmp( key, keybuf, m_fSpec.keylen );

		// 発見
		if( cmpstat ==0){
			SetFilePointer( hFile,-(m_fSpec.keylen+m_fSpec.offset) , NULL, FILE_CURRENT );
			*recNo = now;
			return(0);
		}

		// レコードが中間点より前にある時
		if( cmpstat < 0 )			over = now;

		// レコードが中間点より後にある時
		if( cmpstat > 0 )			under=now;

		// ループ抜け条件
		if( over == under || over < under || over-under==1 ||now==0)
			break;
	}

	// 最終チェック
	if(cmpstat){
		SetFilePointer( hFile,((under-1)*m_fSpec.reclen)+m_fSpec.offset , NULL, FILE_BEGIN );
		for(i=0; i<2 ;i++ ){
			ReadFile( hFile, keybuf, m_fSpec.keylen, &dwRead, NULL );
			cmpstat = memcmp( key, keybuf, m_fSpec.keylen );

			// 発見
			if(!cmpstat){
				SetFilePointer( hFile,-(m_fSpec.keylen+m_fSpec.offset) , NULL, FILE_CURRENT );
				*recNo = under+i;
				return(0);
			}
			if( !i )
				SetFilePointer( hFile,m_fSpec.reclen-m_fSpec.keylen, NULL, FILE_CURRENT);
		}
	}

	SetFilePointer( hFile,((under-1)*m_fSpec.reclen) , NULL, FILE_BEGIN );
	*recNo = under;
	return(4);


}


//---------------------------------------------------------------------------------------------------------------------
//	関数名：void TTextTable::Close(HANDLE& hFile)
//	メンバ：private
//	機能　：ファイルクローズ
//	引数　：Closeするファイルハンドル
//---------------------------------------------------------------------------------------------------------------------
void TTextTable::Close(HANDLE& hFile)
{
	CloseHandle( hFile );
}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::Open(HANDLE& hFile)
//	メンバ：private
//	機能　：ファイルオープン
//	引数　：オープンするファイルハンドル
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Open(HANDLE& hFile,BOOL readonly)
{
	DWORD dwCre;
	DWORD dwAcs;
	DWORD dwAttr;
	char fname[MAX_PATH+1];


	if( readonly == TRUE ){
		dwCre = OPEN_EXISTING;
		dwAcs = GENERIC_READ;
		dwAttr =FILE_ATTRIBUTE_READONLY;
//		dwAttr =FILE_ATTRIBUTE_NORMAL;
	}
	else{
		dwCre = OPEN_ALWAYS;
		dwAcs = GENERIC_WRITE|GENERIC_READ;
		dwAttr =FILE_ATTRIBUTE_NORMAL;
	}

	wsprintf( fname, "%s\\%s", m_fSpec.dir, m_fSpec.filename);

	hFile = CreateFile(fname, dwAcs, 0, NULL, dwCre,dwAttr,NULL);
	// open err
	if( hFile == INVALID_HANDLE_VALUE ) return(TXDB_NOFILE);

	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::Open(HANDLE& hFile)
//	メンバ：private
//	機能　：ファイルエンドにレコードを追加
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Add(void)
{
	HANDLE hFile;
	int ret;
	DWORD dwRead;


	// Open
	ret = Open( hFile, FALSE);
	if( ret ) return(ret);

	SetFilePointer( hFile, 0, NULL, FILE_END );

	WriteFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );

	//2004.07.27 ﾏﾂﾀﾞ
	ret = FlushFileBuffers( hFile );

	// 2004.07.30 coba
	// FlushFileBuffers()はBOOL型なので、成功するとTRUE(非ゼロ)を返しエラーになってた
	if(ret==FALSE) return(10);

	// Close
	Close(hFile);
//	return(ret);	2004.07.30
	return(0);

}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::Open(HANDLE& hFile)
//	メンバ：public
//	機能　：現在登録されているレコードの合計数を返す
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::GetCounts(void)
{
	if(m_fSpec.reclen<1) return(-1);

	HANDLE hFile;
	int ret;
	long nowrec=0;					// 現在のレコード位置
	DWORD fsize;


	// Open
	ret = Open( hFile, TRUE);
	if( ret ) return(0);

	// ファイルサイズ取得
	fsize = GetFileSize( hFile, NULL );

	// Close
	Close(hFile);

	// データ空
	return( (int)(fsize /m_fSpec.reclen));


}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TTextTable::CheckFile(void)
//	メンバ：private
//	機能　：ネットワーク環境で使用時のための、マスタファイルとテンポラリファイルのチェック関数
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::CheckFile(void)
{
	char fname[MAX_PATH+1];
	char workFile[MAX_PATH+1];

	//ファイル検索
	HANDLE hFind;
	WIN32_FIND_DATA findFile;

	// ファイル名から拡張子をはずす
	//マスタファイル名
	wsprintf( fname, "%s\\%s", m_fSpec.dir, m_fSpec.filename);

	hFind = FindFirstFile(fname,&findFile);
	//ﾌｧｲﾙがあった時だけ
	if(hFind != INVALID_HANDLE_VALUE){
		//検索ハンドルｸﾛｰｽﾞ
		FindClose(hFind);
		return 0;	//ファイルアリ!!
	}
	
	//マスタファイルが無い.現在作業中か？
	if( strtok(fname, ".") ==NULL ){
		return(17);
	}

	//ファイル名.*で検索
	wsprintf(workFile,"%s.*",fname);
	hFind = FindFirstFile(workFile,&findFile);
	if(hFind != INVALID_HANDLE_VALUE){
		//作業中･･･ﾞ
		FindClose(hFind);
		return -1;
	}	
	
	//本当にない
	return 10;
}

