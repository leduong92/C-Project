//=======================================================================================================
//	�e�L�X�gDBAccess�N���X
//	Class Name TTextTable
//	File Name TTextTable.cpp
//	�쐬�ҁFCoba
//	�쐬���F1999.05.10
//	�C�����F2004.11.12 ����		�l�b�g���[�N�ł̋����΍�̂��߁AInsert2,Del2,Upd2��ǉ�
//=======================================================================================================
#include "TTextTable.h"


//---------------------------------------------------------------------------------------------------------------------
//	�֐����FTTextTable::TTextTable(void)
//	�����o�Fpublic
//	�@�\�@�F�R���X�g���N�^
//---------------------------------------------------------------------------------------------------------------------
TTextTable::TTextTable(void)
{
	m_PrevRecNo = 0;
	m_BufSize = DEF_BUFSIZE;
}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����FTTextTable::TTextTable(void)
//	�����o�Fpublic
//	�@�\�@�F�f�X�g���N�^
//---------------------------------------------------------------------------------------------------------------------
TTextTable::~TTextTable(void)
{
}


//---------------------------------------------------------------------------------------------------------------------
//	int TTextTable::Initialize(FSPEC *filespec, int bufsize)
//	�����o�Fprotected
//	�@�\�@�F�t�@�C���\���̐ݒ�
//	�����@�F�\����`�p�̍\���́hFSPEC�h
//	�����Q�F�t�@�C���ҏW���̃o�b�t�@�[�T�C�Y
//	�߂�l�F0=�����A���̑�=�����G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Initialize(FSPEC *filespec, int bufsize)
{
	// �\���������o�ϐ��ɕۑ�
	memcpy( &m_fSpec, filespec, sizeof(FSPEC) );

	// spec�̃`�F�b�N
	if( strlen( m_fSpec.filename) > 30 ) return(-1);
	if( !strlen( m_fSpec.filename) ) return(-1);
	if( m_fSpec.reclen < 0 ) return(-2);
	if( m_fSpec.keylen < 0 ) return(-3);
	if( m_fSpec.keylen > TXDB_MAXKEYLEN ) return(-4);	// �T�C�Y�ύX�@2003.05.06 Y.Kogai
	//if( m_fSpec.keylen > 50 ) return(-4);

	if( !strlen( m_fSpec.dir) ) strcpy( m_fSpec.dir,".\\");
	if( !strlen( m_fSpec.tmpdir) ) strcpy( m_fSpec.tmpdir,".\\");


	m_BufSize = bufsize;

	return(0);
}


//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fvoid TTextTable::Clear(void)
//	�����o�Fprivate
//	�@�\�@�F�f�[�^�o�b�t�@�̏�����
//---------------------------------------------------------------------------------------------------------------------
void TTextTable::Clear( void )
{
	memset( m_fSpec.datptr, 0, m_fSpec.reclen );

}


//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fvoid TTextTable::GetFirst( void *key )
//	�����o�Fprotected
//	�@�\�@�F�ŏ��̃��R�[�h���擾
//	�����@�F�����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::GetFirst( void )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
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
//	�֐����Fvoid TTextTable::GetFirst( void *key )
//	�����o�Fprotected
//	�@�\�@�F�ŏ��̃��R�[�h���擾
//	�����@�F�����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::GetLast( void )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;

	nowrec=GetCounts();
	if(nowrec<1) return(-1);

	// open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	// �|�C���^��΂�
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
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;

	// Open
	Clear();
	
	ret = Open( hFile,TRUE);
	if( ret ) return(ret);
	
	// �|�C���^��΂�
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
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;

	nowrec = m_PrevRecNo-2;
	if( nowrec < 0 ) return(42);


	// Open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	// �|�C���^��΂�
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
//	�֐����Fvoid TTextTable::GetEqual( void *key )
//	�����o�Fprotected
//	�@�\�@�F���R�[�h����/�L�[�ɍ��v���郌�R�[�h�������i���j�[�N�L�[�Ώہj
//	�����@�F�����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::GetEqual( void *key )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;

	// Open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	// Find
	ret = MoveRecord( hFile, key, &nowrec );

	// ���������ꍇ�ǂݏo��
	if( !ret ){
		ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
		m_PrevRecNo = nowrec;
	}
	// Close
	Close(hFile);
	return(ret);

}
//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fvoid TTextTable::GetEqual2( void *key )
//	�����o�Fprotected
//	�@�\�@�F���R�[�h����/�L�[�ɍ��v���郌�R�[�h�������i���j�[�N�L�[�Ώہj
//	�����@�F�����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::GetEqual2( void *key )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;

	//�t�@�C���͍�ƒ����H
	ret = CheckFile();
	//���ݍ�ƒ�
	if(ret < 0) return TXDB_WORKING;
	
	// Open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	// Find
	ret = MoveRecord( hFile, key, &nowrec );

	// ���������ꍇ�ǂݏo��
	if( !ret ){
		ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
		m_PrevRecNo = nowrec;
	}
	// Close
	Close(hFile);
	return(ret);

}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fvoid TTextTable::GetEqual( void *key )
//	�����o�Fprotected
//	�@�\�@�F���R�[�h����/�L�[�ɍ��v����ŏ��̃��R�[�h�������i���j�[�N�L�[�Ώہj
//	�����@�F�����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::FindFirst( void *key )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;
	char prevKey[256];

	// Open
	Clear();
	ret = Open( hFile, TRUE);
	if( ret ) return(ret);

	// Find
	ret = MoveRecord( hFile, key, &nowrec );

	// ���������ꍇ1���R�[�h���o�b�N���Ȃ���ǂݏo��
	if( !ret ){
		ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
		SetFilePointer( hFile, -1*( m_fSpec.reclen - m_fSpec.offset -m_fSpec.keylen ), NULL, FILE_CURRENT );

		// �L�[���ς��܂Ńo�b�N
		while( nowrec > 1 ){
			// �O��ں��ނ̃L�[�ʒu�܂Ŗ߂�
			SetFilePointer( hFile, -1*(m_fSpec.reclen+m_fSpec.keylen), NULL, FILE_CURRENT );
			ReadFile( hFile, prevKey, m_fSpec.keylen, &dwRead, NULL );

			// �L�[���ς�����ꍇ�̓��R�[�h�擾
			if( memcmp( key, prevKey, m_fSpec.keylen ) ){
				SetFilePointer( hFile, (m_fSpec.reclen-m_fSpec.offset-m_fSpec.keylen), NULL, FILE_CURRENT );
				ReadFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
				break;
			}

			nowrec --;
			// �t�@�C���̐擪
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
//	�֐����Fint TTextTable::Upd( void *key )
//	�����o�Fprotected
//	�@�\�@�F
//	�����@�F�X�V����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Upd( void *key )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;


	// Open
	ret = Open( hFile, FALSE);
	if( ret ) return(ret);

	// �w��ں��ނ̗L���𒲂ׂ�
	ret = MoveRecord( hFile, key ,&nowrec );

	if(!ret){
		WriteFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
	}

	// Close
	Close(hFile);
	return(ret);

}
//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TTextTable::Upd( void *key )
//	�����o�Fprotected
//	�@�\�@�FUpd�����ǂ��A�����O�Ƀ}�X�^�t�@�C������ύX���A�������͑�����̃A�N�Z�X���o���Ȃ��悤�ύX
//	�����@�F�X�V����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Upd2( void *key )
{
	HANDLE hFile;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;
	char pcName[MAX_COMPUTERNAME_LENGTH + 1 ];
	DWORD pcNameLen = MAX_COMPUTERNAME_LENGTH + 1;
	char newFileName[MAX_PATH+1];
	char newPath[MAX_PATH+1];
	char oldFileName[MAX_PATH+1];
	char fname[MAX_PATH+1];
	//PC���擾 --�g���q�Ɏg��--
	memset(pcName,0,sizeof(pcName));
	GetComputerName(pcName,&pcNameLen);

	//���̃t�@�C������ޔ�
	wsprintf( oldFileName, m_fSpec.filename );
	
	// �t�@�C��������g���q���͂���
	memset( fname ,0, sizeof(fname) );
	memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
	if(!strlen(fname)){
		return(26);
	}
	if( strtok(fname, ".") ==NULL ){
		return(27);
	}
	wsprintf( newFileName, "%s.%s", fname ,pcName);				//�V�K�t�@�C����
	wsprintf( newPath, "%s\\%s",m_fSpec.dir,newFileName);	//�S�Ẵp�X

	//���̃t�@�C����
	memset(fname,0,sizeof(fname));
	wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );

	//���t�@�C���͑��݂��邩�H
	ret = CheckFile();
	//���݂����ꍇ
	if(!ret){
		DeleteFile(newPath);
		//�t�@�C�����ύX!!
		if( !MoveFile(fname ,newPath) ){
			//���s������
			return TXDB_WORKING;
		}
		else{
			//m_fSpec.filename����������
			wsprintf( m_fSpec.filename , newFileName );
		}
	}
	//�t�@�C�����{���ɂȂ����V�K�쐬
	else if(ret == 10){
		//m_fSpec.filename����������
		wsprintf( m_fSpec.filename , newFileName );
	}
	//��ƒ��̏ꍇ
	else return TXDB_WORKING;

	// Open
	ret = Open( hFile, FALSE);
	if( ret ){
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(ret);
	}

	// �w��ں��ނ̗L���𒲂ׂ�
	ret = MoveRecord( hFile, key ,&nowrec );

	if(!ret){
		WriteFile( hFile, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );
	}

	// Close
	Close(hFile);
	
	//���Ƃ̃t�@�C�����ɖ߂�
	wsprintf( m_fSpec.filename ,"%s",oldFileName );
	wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
	MoveFile( newPath, fname );
	
	return(ret);

}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TTextTable::Insert( void *key )
//	�����o�Fprotected
//	�@�\�@�F
//	�����@�F�폜����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Insert( void *key )
{
	HANDLE hFile,hTmp;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;


	// Open
	ret = Open( hFile, FALSE);
	if( ret ) return(ret);

	// �w��ں��ނ̗L���Ƒ}���ʒu���ׂ�
	ret = MoveRecord( hFile, key ,&nowrec);

	// �����������
	Close(hFile);

	// ���������ꍇ�͒ǉ����Ȃ�
	if( ret !=4 ) return(5);

	// �X�V
	if( !m_fSpec.reclen ) return(11);

	// �ǂ݂Ƃ�o�b�t�@
	int readCount,i;
	DWORD  totalSize;
	char tmpFile[MAX_PATH+1];		// *.tmp�t�@�C����
	char savFile[MAX_PATH+1];		// *.sav�t�@�C����
	char fname[MAX_PATH+1];

	// �e���|�����t�@�C���̍쐬(filename.tmp)
	memset( tmpFile, 0, sizeof(tmpFile) );
	memset( savFile, 0, sizeof(savFile) );
	memset( fname, 0, sizeof(fname) );

	// �t�@�C��������g���q���͂���
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
	// �}�X�^�[�ăI�[�v��
	ret = Open( hFile, FALSE);
	if( ret ){
		Close(hTmp);
		DeleteFile( tmpFile );
		return(13);
	}

	// ���݂̓o�^ں��ތ���
	if( ( totalSize = GetFileSize(hFile, NULL)) == 0xffffffff ){
		Close(hTmp);
		Close(hFile);
		DeleteFile( tmpFile );
		return(14);
	}

	// �ǂ݂Ƃ�o�b�t�@
	char *readBuf= new char[m_BufSize];

	// �ǉ��ʒu�̒��O��ں��ނ܂ł̓ǂ݂Ƃ�񐔁i�P�񂠂���bufSize�܂Ƃ߂ēǂށj���Z�o
	readCount = (nowrec * m_fSpec.reclen ) /m_BufSize;

	// �폜�O�܂ł̃f�[�^���R�s�[
	// Block Read
	for( i=0; i < readCount; i++){
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
	}

	// �]�蕔����ǂ�
	memset( readBuf, 0, m_BufSize );
	ReadFile( hFile, readBuf, (nowrec * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );
	WriteFile( hTmp, readBuf, (nowrec * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );

	// �Y��ں��ނ���������
	WriteFile( hTmp, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );

	// �ǉ���|EOF�܂ł��R�s�[
	readCount = (totalSize - (nowrec*m_fSpec.reclen )) / m_BufSize;

	for( i=0; i < readCount; i++){
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
	}

	// �]�蕔����ǂ�
	memset( readBuf, 0, m_BufSize );
	ReadFile( hFile, readBuf, (totalSize - (nowrec*m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );
	WriteFile( hTmp, readBuf, (totalSize - (nowrec*m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );

	// �t�@�C���N���[�Y
	Close(hFile);
	Close(hTmp);

	// ���l�[��
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

	// �}�X�^�[�폜
	DeleteFile( savFile );

	delete readBuf;
	return(ret);
}
//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TTextTable::Insert2( void *key )
//	�����o�Fprotected
//	�@�\�@�FInsert�����ǂ��A�����O�Ƀ}�X�^�t�@�C������ύX���A�������͑�����̃A�N�Z�X���o���Ȃ��悤�ύX
//	�����@�F�폜����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Insert2( void *key )
{
	HANDLE hFile,hTmp;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;
	char tmpFile[MAX_PATH+1];		// *.tmp�t�@�C����
	char fname[MAX_PATH+1];
	char pcName[MAX_COMPUTERNAME_LENGTH + 1 ];
	DWORD pcNameLen = MAX_COMPUTERNAME_LENGTH + 1;
	char newFileName[MAX_PATH+1];
	char newPath[MAX_PATH+1];
	char oldFileName[MAX_PATH+1];
	//PC���擾 --�g���q�Ɏg��--
	memset(pcName,0,sizeof(pcName));
	GetComputerName(pcName,&pcNameLen);

	//���̃t�@�C������ޔ�
	wsprintf( oldFileName, m_fSpec.filename );
	
	// �t�@�C��������g���q���͂���
	memset( fname ,0, sizeof(fname) );
	memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
	if(!strlen(fname)){
		return(26);
	}
	if( strtok(fname, ".") ==NULL ){
		return(27);
	}
	wsprintf( newFileName, "%s.%s", fname ,pcName);				//�V�K�t�@�C����
	wsprintf( newPath, "%s\\%s",m_fSpec.dir,newFileName);	//�S�Ẵp�X

	//���̃t�@�C����
	memset(fname,0,sizeof(fname));
	wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );

	//���t�@�C���͑��݂��邩�H
	ret = CheckFile();
	//���݂����ꍇ
	if(!ret){
		DeleteFile(newPath);
		if( !MoveFile(fname ,newPath) ){
			//���s������
			return TXDB_WORKING;
		}
		else{
			//m_fSpec.filename����������
			wsprintf( m_fSpec.filename , newFileName );
		}
	}
	//�t�@�C�����{���ɂȂ����V�K�쐬
	else if(ret == 10){
		//m_fSpec.filename����������
		wsprintf( m_fSpec.filename , newFileName );
	}
	//��ƒ��̏ꍇ
	else return TXDB_WORKING;
	
	// Open 
	ret = Open( hFile, FALSE);
	if( ret ){
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(ret);		//̧�ٵ���ݎ��s�����甲����
	}

	// �w��ں��ނ̗L���Ƒ}���ʒu���ׂ�
	ret = MoveRecord( hFile, key ,&nowrec);

	// �����������
	Close(hFile);

	// ���������ꍇ�͒ǉ����Ȃ�
	if( ret !=4 ){
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(5);
	}

	// �X�V
	if( !m_fSpec.reclen ){
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(11);
	}

	// �ǂ݂Ƃ�o�b�t�@
	int readCount,i;
	DWORD  totalSize;
	char savFile[MAX_PATH+1];		// *.sav�t�@�C����

	// �e���|�����t�@�C���̍쐬(filename.tmp)
	memset( tmpFile, 0, sizeof(tmpFile) );
	memset( savFile, 0, sizeof(savFile) );
	memset( fname, 0, sizeof(fname) );

	// �t�@�C��������g���q���͂���
	memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
	if(!strlen(fname)){
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(16);
	}
	if( strtok(fname, ".") ==NULL ){
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(17);
	}
	wsprintf( tmpFile, "%s\\%s.tmp",m_fSpec.tmpdir, fname );
	wsprintf( savFile, "%s\\%s.sav",m_fSpec.tmpdir, fname );

	//̧�ِV�K�쐬
	hTmp = CreateFile( tmpFile, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if( hTmp == INVALID_HANDLE_VALUE ){
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(12);// open err
	}
	// �}�X�^�[�ăI�[�v��
	ret = Open( hFile, FALSE);
	if( ret ){
		Close(hTmp);
		DeleteFile( tmpFile );
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(13);
	}

	// ���݂̓o�^ں��ތ���
	if( ( totalSize = GetFileSize(hFile, NULL)) == 0xffffffff ){
		Close(hTmp);
		Close(hFile);
		DeleteFile( tmpFile );
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(14);
	}

	// �ǂ݂Ƃ�o�b�t�@
	char *readBuf= new char[m_BufSize];

	// �ǉ��ʒu�̒��O��ں��ނ܂ł̓ǂ݂Ƃ�񐔁i�P�񂠂���bufSize�܂Ƃ߂ēǂށj���Z�o
	readCount = (nowrec * m_fSpec.reclen ) /m_BufSize;

	// �폜�O�܂ł̃f�[�^���R�s�[
	// Block Read
	for( i=0; i < readCount; i++){
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
	}

	// �]�蕔����ǂ�
	memset( readBuf, 0, m_BufSize );
	ReadFile( hFile, readBuf, (nowrec * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );
	WriteFile( hTmp, readBuf, (nowrec * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );

	// �Y��ں��ނ���������
	WriteFile( hTmp, m_fSpec.datptr, m_fSpec.reclen, &dwRead, NULL );

	// �ǉ���|EOF�܂ł��R�s�[
	readCount = (totalSize - (nowrec*m_fSpec.reclen )) / m_BufSize;

	for( i=0; i < readCount; i++){
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
	}

	// �]�蕔����ǂ�
	memset( readBuf, 0, m_BufSize );
	ReadFile( hFile, readBuf, (totalSize - (nowrec*m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );
	WriteFile( hTmp, readBuf, (totalSize - (nowrec*m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );

	// �t�@�C���N���[�Y
	Close(hFile);
	Close(hTmp);

	// ���l�[��
	wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );
	DeleteFile( savFile);
	if( MoveFile( fname, savFile ) == FALSE ){
		DeleteFile( tmpFile );
		delete readBuf;
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(15);
	}

	//���Ƃ̃t�@�C�����ɖ߂�
	wsprintf( m_fSpec.filename ,"%s",oldFileName );
	wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
	if( MoveFile( tmpFile, fname ) == FALSE ){
		DeleteFile( tmpFile);
		MoveFile(savFile,fname);
		delete readBuf;
		return(15);
	}

	// �}�X�^�[�폜
	DeleteFile( savFile );

	delete readBuf;
	return(ret);
}


//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TTextTable::Del( void *key )
//	�����o�Fprotected
//	�@�\�@�F
//	�����@�F�폜����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Del( void *key )
{
	HANDLE hFile,hTmp;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;


	// Open
	ret = Open( hFile, FALSE);
	if( ret ) return(ret);

	// �w��ں��ނ̗L���𒲂ׂ�
	ret = MoveRecord( hFile, key ,&nowrec);

	// �����������
	Close(hFile);

	// ���������ꍇ�폜����
	if( !ret ){
		if( !m_fSpec.reclen ) return(11);

		int readCount,i;
		DWORD  totalSize;
		char tmpFile[MAX_PATH+1];		// *.tmp�t�@�C����
		char savFile[MAX_PATH+1];		// *.sav�t�@�C����
		char fname[MAX_PATH+1];

		// �e���|�����t�@�C���̍쐬(filename.tmp)
		memset( tmpFile, 0, sizeof(tmpFile) );
		memset( savFile, 0, sizeof(savFile) );
		memset( fname, 0, sizeof(fname) );

		// �t�@�C��������g���q���͂���
		memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
		if(!strlen(fname)) return(16);
		if( strtok(fname, ".") ==NULL ) return(17);

		wsprintf( tmpFile, "%s\\%s.tmp",m_fSpec.tmpdir, fname );
		wsprintf( savFile, "%s\\%s.sav",m_fSpec.tmpdir, fname );

		hTmp = CreateFile( tmpFile, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if( hTmp == INVALID_HANDLE_VALUE )			return(12);// open err

		// �}�X�^�[�ăI�[�v��
		ret = Open( hFile, FALSE);
		if( ret ){
			Close(hTmp);
			DeleteFile( tmpFile );
			return(13);
		}

		// ���݂̓o�^ں��ތ���
		if( ( totalSize = GetFileSize(hFile, NULL)) == 0xffffffff ){
			Close(hTmp);
			Close(hFile);
			DeleteFile( tmpFile );
			return(14);
		}

		// �ǂ݂Ƃ�o�b�t�@
		char *readBuf= new char[m_BufSize];

		// �폜ں��ނ܂ł̓ǂ݂Ƃ�񐔁i�P�񂠂���m_BufSize�܂Ƃ߂ēǂށj���Z�o
		readCount = ( (nowrec-1) * m_fSpec.reclen ) /m_BufSize;

		// �폜�O�܂ł̃f�[�^���R�s�[
		// Block Read
		for( i=0; i < readCount; i++){
			memset( readBuf, 0, m_BufSize );
			ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
			WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
		}

		// �]�蕔����ǂ�
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, ( (nowrec-1) * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, ( (nowrec-1) * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );

		// �Y��ں��ނ͔�΂�
		if( SetFilePointer( hFile, m_fSpec.reclen, NULL, FILE_CURRENT) ==0xffffffff ){
			Close(hTmp);
			Close(hFile);
			DeleteFile( tmpFile );
			return(15);
		}

		// �폜��|EOF�܂ł��R�s�[
		readCount = (totalSize - (nowrec*m_fSpec.reclen )) / m_BufSize;

		for( i=0; i < readCount; i++){
			memset( readBuf, 0, m_BufSize );
			ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
			WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
		}

		// �]�蕔����ǂ�
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, (totalSize - ( nowrec * m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, (totalSize - ( nowrec * m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );

		// �t�@�C���N���[�Y
		Close(hFile);
		Close(hTmp);

		// ���l�[��
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

		// �}�X�^�[�폜
		DeleteFile( savFile );
		delete readBuf;

	}

	return(ret);
}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TTextTable::Del2( void *key )
//	�����o�Fprotected
//	�@�\�@�FDel�����ǂ��A�����O�Ƀ}�X�^�t�@�C������ύX���A�������͑�����̃A�N�Z�X���o���Ȃ��悤�ύX
//	�����@�F�폜����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::Del2( void *key )
{
	HANDLE hFile,hTmp;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;

	char fname[MAX_PATH+1];
	char pcName[MAX_COMPUTERNAME_LENGTH + 1 ];
	DWORD pcNameLen = MAX_COMPUTERNAME_LENGTH + 1;
	char newFileName[MAX_PATH+1];
	char newPath[MAX_PATH+1];
	char oldFileName[MAX_PATH+1];
	//PC���擾 --�g���q�Ɏg��--
	memset(pcName,0,sizeof(pcName));
	GetComputerName(pcName,&pcNameLen);

	//���̃t�@�C������ޔ�
	wsprintf( oldFileName, m_fSpec.filename );
	
	// �t�@�C��������g���q���͂���
	memset( fname ,0, sizeof(fname) );
	memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
	if(!strlen(fname)){
		return(26);
	}
	if( strtok(fname, ".") ==NULL ){
		return(27);
	}
	wsprintf( newFileName, "%s.%s", fname ,pcName);				//�V�K�t�@�C����
	wsprintf( newPath, "%s\\%s",m_fSpec.dir,newFileName);	//�S�Ẵp�X

	//���̃t�@�C����
	memset(fname,0,sizeof(fname));
	wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );

	//���t�@�C���͑��݂��邩�H
	ret = CheckFile();
	//���݂����ꍇ
	if(!ret){
		DeleteFile(newPath);
		if( !MoveFile(fname ,newPath) ){
			//���s������
			return TXDB_WORKING;
		}
		else{
			//m_fSpec.filename����������
			wsprintf( m_fSpec.filename , newFileName );
		}
	}
	//�t�@�C�����{���ɂȂ����V�K�쐬
	else if(ret == 10){
		//m_fSpec.filename����������
		wsprintf( m_fSpec.filename , newFileName );
	}
	//��ƒ��̏ꍇ
	else return TXDB_WORKING;
	
	// Open
	ret = Open( hFile, FALSE);
	if( ret ){
		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		MoveFile( newPath,fname );
		return(ret);
	}

	// �w��ں��ނ̗L���𒲂ׂ�
	ret = MoveRecord( hFile, key ,&nowrec);

	// �����������
	Close(hFile);

	// ���������ꍇ�폜����
	if( !ret ){
		if( !m_fSpec.reclen ){
			//���Ƃ̃t�@�C�����ɖ߂�
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(11);
		}

		int readCount,i;
		DWORD  totalSize;
		char tmpFile[MAX_PATH+1];		// *.tmp�t�@�C����
		char savFile[MAX_PATH+1];		// *.sav�t�@�C����
		char fname[MAX_PATH+1];

		// �e���|�����t�@�C���̍쐬(filename.tmp)
		memset( tmpFile, 0, sizeof(tmpFile) );
		memset( savFile, 0, sizeof(savFile) );
		memset( fname, 0, sizeof(fname) );

		// �t�@�C��������g���q���͂���
		memcpy( fname, m_fSpec.filename,strlen(m_fSpec.filename) );
		if(!strlen(fname)){
			//���Ƃ̃t�@�C�����ɖ߂�
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(16);
		}
		if( strtok(fname, ".") ==NULL ){
			//���Ƃ̃t�@�C�����ɖ߂�
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(17);
		}

		wsprintf( tmpFile, "%s\\%s.tmp",m_fSpec.tmpdir, fname );
		wsprintf( savFile, "%s\\%s.sav",m_fSpec.tmpdir, fname );

		hTmp = CreateFile( tmpFile, GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if( hTmp == INVALID_HANDLE_VALUE ){
			//���Ƃ̃t�@�C�����ɖ߂�
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(12);// open err
		}

		// �}�X�^�[�ăI�[�v��
		ret = Open( hFile, FALSE);
		if( ret ){
			Close(hTmp);
			DeleteFile( tmpFile );
			//���Ƃ̃t�@�C�����ɖ߂�
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(13);
		}

		// ���݂̓o�^ں��ތ���
		if( ( totalSize = GetFileSize(hFile, NULL)) == 0xffffffff ){
			Close(hTmp);
			Close(hFile);
			DeleteFile( tmpFile );
			//���Ƃ̃t�@�C�����ɖ߂�
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(14);
		}

		// �ǂ݂Ƃ�o�b�t�@
		char *readBuf= new char[m_BufSize];

		// �폜ں��ނ܂ł̓ǂ݂Ƃ�񐔁i�P�񂠂���m_BufSize�܂Ƃ߂ēǂށj���Z�o
		readCount = ( (nowrec-1) * m_fSpec.reclen ) /m_BufSize;

		// �폜�O�܂ł̃f�[�^���R�s�[
		// Block Read
		for( i=0; i < readCount; i++){
			memset( readBuf, 0, m_BufSize );
			ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
			WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
		}

		// �]�蕔����ǂ�
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, ( (nowrec-1) * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, ( (nowrec-1) * m_fSpec.reclen ) % m_BufSize, &dwRead, NULL );

		// �Y��ں��ނ͔�΂�
		if( SetFilePointer( hFile, m_fSpec.reclen, NULL, FILE_CURRENT) ==0xffffffff ){
			Close(hTmp);
			Close(hFile);
			DeleteFile( tmpFile );
			//���Ƃ̃t�@�C�����ɖ߂�
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(15);
		}

		// �폜��|EOF�܂ł��R�s�[
		readCount = (totalSize - (nowrec*m_fSpec.reclen )) / m_BufSize;

		for( i=0; i < readCount; i++){
			memset( readBuf, 0, m_BufSize );
			ReadFile( hFile, readBuf, m_BufSize, &dwRead, NULL );
			WriteFile( hTmp, readBuf, m_BufSize, &dwRead, NULL );
		}

		// �]�蕔����ǂ�
		memset( readBuf, 0, m_BufSize );
		ReadFile( hFile, readBuf, (totalSize - ( nowrec * m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );
		WriteFile( hTmp, readBuf, (totalSize - ( nowrec * m_fSpec.reclen )) % m_BufSize, &dwRead, NULL );

		// �t�@�C���N���[�Y
		Close(hFile);
		Close(hTmp);

		// ���l�[��
		wsprintf( fname, "%s\\%s",m_fSpec.dir, m_fSpec.filename );
		DeleteFile( savFile);
		if( MoveFile( fname, savFile ) == FALSE ){
			DeleteFile( tmpFile );
			delete readBuf;
			//���Ƃ̃t�@�C�����ɖ߂�
			wsprintf( m_fSpec.filename ,"%s",oldFileName );
			wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
			MoveFile( newPath,fname );
			return(15);
		}

		//���Ƃ̃t�@�C�����ɖ߂�
		wsprintf( m_fSpec.filename ,"%s",oldFileName );
		wsprintf( fname, "%s\\%s",m_fSpec.dir, oldFileName );
		if( MoveFile( tmpFile, fname ) == FALSE ){
			DeleteFile( tmpFile);
			DeleteFile( savFile);
			delete readBuf;
			return(15);
		}

		// �}�X�^�[�폜
		DeleteFile( savFile );
		delete readBuf;
	}
	return(ret);
}



//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TTextTable::MoveRecord( HANDLE hFile, void *key )
//	�����o�Fprivate
//	�@�\�@�F�����Ŏw�肳�ꂽ�L�[��ں��ނ��������A�Y�����R�[�h�̐擪�Ƀt�@�C���|�C���^���ړ�����
//	�����P�F��������t�@�C���̃n���h��
//	�����Q�F��������L�[
//	�����R�F�Y��ں��ނ�ں��ޔԍ��i�o�́j
//	�߂�l�F0=�����A���̑�=�G���[
//						���t�@�C���I�[�v������Ă邱�Ƃ��O��I
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::MoveRecord2( HANDLE hFile,void *key ,long *recNo )
{
	int ret;
	char keybuf[TXDB_MAXKEYLEN+1];
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead;

	*recNo = 0;

	// �L�[�̈ʒu�܂ł��炷
	if( SetFilePointer( hFile, m_fSpec.offset, NULL, FILE_BEGIN ) ==0xffffffff ){
		return(4);
	}

	while(1){
		// �ǂݏo��(key�����̂�)
		memset(keybuf, 0, sizeof(keybuf) );
		if( ReadFile( hFile, keybuf, m_fSpec.keylen, &dwRead, NULL )==FALSE){
			ret = 2;
			break;
		}

		if( !dwRead  ){
			ret = 9;
			break;
		}

		// �����L�[�Ƃ̔�r
		if( !memcmp( key, keybuf, m_fSpec.keylen) ){
			// �Y�����R�[�h�̏ꍇ�́A�|�C���^�����R�[�h�擪�ɖ߂�
			SetFilePointer( hFile, -1*(m_fSpec.offset+m_fSpec.keylen), NULL, FILE_CURRENT );
			ret = 0;

			// �J�����g�s�ԍ��������o�ϐ��ɕۑ�
			m_PrevRecNo = nowrec;
			*recNo = nowrec;
			break;
		}

		// ���R�[�h�ړ�
		if( SetFilePointer( hFile, (m_fSpec.reclen - m_fSpec.keylen), NULL, FILE_CURRENT ) ==0xffffffff){
			ret = 4;
			break;
		}
		nowrec ++;
	}

	return(ret);
}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TTextTable::MoveRecord( HANDLE hFile, void *key )
//	�����o�Fprivate
//	�@�\�@�F�����Ŏw�肳�ꂽ�L�[��ں��ނ��������A�Y�����R�[�h�Ƀt�@�C���|�C���^���ړ�����
//			�Y�����R�[�h�������Ƃ��̓L�[�Ɉ�ԋ߂����O�̃��R�[�h�Ɉړ�
//	�����P�F��������t�@�C���̃n���h��
//	�����Q�F��������L�[
//	�����R�F�Y��ں��ނ�ں��ޔԍ��i�o�́j
//	�߂�l�F0=�����A���̑�=�G���[
//						���t�@�C���I�[�v������Ă邱�Ƃ��O��I
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::MoveRecord( HANDLE hFile,void *key ,long *recNo )
{
	int cmpstat,i;
	char keybuf[TXDB_MAXKEYLEN+1];
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD dwRead,fsize;
	long under,now,over;


	*recNo = 0;

	// �t�@�C���T�C�Y�擾
	fsize = GetFileSize( hFile, NULL );
	// �f�[�^��
	if( !fsize )		return(4);


	// �擪�Ɣ�r
	SetFilePointer( hFile,m_fSpec.offset, NULL,FILE_BEGIN );
	if( ReadFile( hFile, keybuf, m_fSpec.keylen, &dwRead, NULL )==FALSE) return(2);
	cmpstat = memcmp( key, keybuf ,m_fSpec.keylen );
	// �擪���O�ɓ�����ꍇ
	if( cmpstat < 0 ){
		SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
		return(4);
	}
	// �擪�f�[�^�������ꍇ
	if( cmpstat ==0){
		SetFilePointer( hFile, 0, NULL, FILE_BEGIN );
		*recNo = 1;
		return(0);
	}

	// �Ō���Ɣ�r
	SetFilePointer( hFile,-(m_fSpec.reclen-m_fSpec.offset), NULL,FILE_END );
	if( ReadFile( hFile, keybuf, m_fSpec.keylen, &dwRead, NULL )==FALSE) return(2);
	cmpstat = memcmp( key, keybuf, m_fSpec.keylen );
	// �Ō���ɓ�����ꍇ
	if( cmpstat > 0 ){
		SetFilePointer( hFile, 0, NULL, FILE_END );
		*recNo = fsize/m_fSpec.reclen;
		return(4);
	}
	// �Y���f�[�^�������ꍇ
	if( cmpstat ==0){
		SetFilePointer( hFile, -m_fSpec.reclen, NULL, FILE_END );
		*recNo = fsize/m_fSpec.reclen;
		return(0);
	}

	under = 1;
	over = fsize/m_fSpec.reclen;

	// 2������
	while(1){
		// under,over�̒��Ԃ̃��R�[�h���`�F�b�N����
		now=( over+under) / 2;
		SetFilePointer( hFile, ( (now-1) * m_fSpec.reclen) +m_fSpec.offset, NULL, FILE_BEGIN );

		// �ǂݍ���
		if( ReadFile( hFile, keybuf, m_fSpec.keylen, &dwRead, NULL )==FALSE) return(2);

		// ��r
		cmpstat = memcmp( key, keybuf, m_fSpec.keylen );

		// ����
		if( cmpstat ==0){
			SetFilePointer( hFile,-(m_fSpec.keylen+m_fSpec.offset) , NULL, FILE_CURRENT );
			*recNo = now;
			return(0);
		}

		// ���R�[�h�����ԓ_���O�ɂ��鎞
		if( cmpstat < 0 )			over = now;

		// ���R�[�h�����ԓ_����ɂ��鎞
		if( cmpstat > 0 )			under=now;

		// ���[�v��������
		if( over == under || over < under || over-under==1 ||now==0)
			break;
	}

	// �ŏI�`�F�b�N
	if(cmpstat){
		SetFilePointer( hFile,((under-1)*m_fSpec.reclen)+m_fSpec.offset , NULL, FILE_BEGIN );
		for(i=0; i<2 ;i++ ){
			ReadFile( hFile, keybuf, m_fSpec.keylen, &dwRead, NULL );
			cmpstat = memcmp( key, keybuf, m_fSpec.keylen );

			// ����
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
//	�֐����Fvoid TTextTable::Close(HANDLE& hFile)
//	�����o�Fprivate
//	�@�\�@�F�t�@�C���N���[�Y
//	�����@�FClose����t�@�C���n���h��
//---------------------------------------------------------------------------------------------------------------------
void TTextTable::Close(HANDLE& hFile)
{
	CloseHandle( hFile );
}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TTextTable::Open(HANDLE& hFile)
//	�����o�Fprivate
//	�@�\�@�F�t�@�C���I�[�v��
//	�����@�F�I�[�v������t�@�C���n���h��
//	�߂�l�F0=�����A���̑�=�G���[
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
//	�֐����Fint TTextTable::Open(HANDLE& hFile)
//	�����o�Fprivate
//	�@�\�@�F�t�@�C���G���h�Ƀ��R�[�h��ǉ�
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

	//2004.07.27 ����
	ret = FlushFileBuffers( hFile );

	// 2004.07.30 coba
	// FlushFileBuffers()��BOOL�^�Ȃ̂ŁA���������TRUE(��[��)��Ԃ��G���[�ɂȂ��Ă�
	if(ret==FALSE) return(10);

	// Close
	Close(hFile);
//	return(ret);	2004.07.30
	return(0);

}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TTextTable::Open(HANDLE& hFile)
//	�����o�Fpublic
//	�@�\�@�F���ݓo�^����Ă��郌�R�[�h�̍��v����Ԃ�
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::GetCounts(void)
{
	if(m_fSpec.reclen<1) return(-1);

	HANDLE hFile;
	int ret;
	long nowrec=0;					// ���݂̃��R�[�h�ʒu
	DWORD fsize;


	// Open
	ret = Open( hFile, TRUE);
	if( ret ) return(0);

	// �t�@�C���T�C�Y�擾
	fsize = GetFileSize( hFile, NULL );

	// Close
	Close(hFile);

	// �f�[�^��
	return( (int)(fsize /m_fSpec.reclen));


}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TTextTable::CheckFile(void)
//	�����o�Fprivate
//	�@�\�@�F�l�b�g���[�N���Ŏg�p���̂��߂́A�}�X�^�t�@�C���ƃe���|�����t�@�C���̃`�F�b�N�֐�
//---------------------------------------------------------------------------------------------------------------------
int TTextTable::CheckFile(void)
{
	char fname[MAX_PATH+1];
	char workFile[MAX_PATH+1];

	//�t�@�C������
	HANDLE hFind;
	WIN32_FIND_DATA findFile;

	// �t�@�C��������g���q���͂���
	//�}�X�^�t�@�C����
	wsprintf( fname, "%s\\%s", m_fSpec.dir, m_fSpec.filename);

	hFind = FindFirstFile(fname,&findFile);
	//̧�ق�������������
	if(hFind != INVALID_HANDLE_VALUE){
		//�����n���h���۰��
		FindClose(hFind);
		return 0;	//�t�@�C���A��!!
	}
	
	//�}�X�^�t�@�C��������.���ݍ�ƒ����H
	if( strtok(fname, ".") ==NULL ){
		return(17);
	}

	//�t�@�C����.*�Ō���
	wsprintf(workFile,"%s.*",fname);
	hFind = FindFirstFile(workFile,&findFile);
	if(hFind != INVALID_HANDLE_VALUE){
		//��ƒ�����
		FindClose(hFind);
		return -1;
	}	
	
	//�{���ɂȂ�
	return 10;
}

