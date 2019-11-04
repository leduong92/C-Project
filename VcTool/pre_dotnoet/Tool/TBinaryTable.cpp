//=======================================================================================================
//	�ϒ��o�C�i���t�@�C������N���X
//	Class Name TBinaryTable
//	File Name TBinaryTable.cpp
//	�쐬�ҁFCoba
//	�쐬���F2002.02.01
//	�C�����F
//=======================================================================================================
#include "TBinaryTable.h"

#pragma pack(push,1)

//---------------------------------------------------------------------------------------------------------------------
//	�֐����FTBinaryTable::TBinaryTable(void)
//	�����o�Fpublic
//	�@�\�@�F�R���X�g���N�^
//---------------------------------------------------------------------------------------------------------------------
TBinaryTable::TBinaryTable(void)
{
	memset( m_fName, 0, sizeof(m_fName) );
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����FTBinaryTable::TBinaryTable(void)
//	�����o�Fpublic
//	�@�\�@�F�f�X�g���N�^
//---------------------------------------------------------------------------------------------------------------------
TBinaryTable::~TBinaryTable(void)
{
}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::Create(char* fname,BFSPEC *filespec)
//	�����o�Fprotected
//	�@�\�@�FDB�t�@�C���̍쐬�B���ɂ���ꍇ�͏㏑������܂�
//	�����P�F�쐬����t�@�C����
//	�����Q�F�\����`�p�̍\���́hBFSPEC�h
//	�߂�l�F0=�����A���̑�=�����G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Create(char* fname,BF_SPEC *filespec)
{
	BF_HEAD fHead;					// �t�@�C���w�b�_

	if( NULL == fname || !strlen(fname) ) return(BTB_NAME_FAULT);

	// �L�[���̃`�F�b�N
	if( filespec->keylen==0 || filespec->keylen > MAX_KEY_LENGTH) return BTB_KEYLEN_ERR;

	// �f�[�^���𒴂��Ă͂����Ȃ�
	if( filespec->keylen+filespec->offset>MAX_RECORDLENGTH) return(BTB_RECLEN_ERR);

	FILE* fp;

	if( NULL==(fp=fopen(fname,"wb")) ) return(BTB_CANT_CREATE);

	time_t 	ltime;
	struct	tm	*nowtime;
    char dummy[20];

	time(&ltime);
	nowtime=localtime(&ltime);
	sprintf(dummy,"%04d%02d%02d", nowtime->tm_year+1900,nowtime->tm_mon+1,nowtime->tm_mday);

	// �w�b�_�[������������
	memset( &fHead, 0, sizeof(BF_HEAD));
	memcpy( fHead.code, BF_NOK_NAME,6);			// �t�@�C���`�F�b�N�p��`��
	memcpy( fHead.ver,BF_NOK_VERSION,5);			// �o�[�W����
	memcpy( fHead.date,dummy,8);					// �쐬��
	fHead.keylen=filespec->keylen;
	fHead.offset = filespec->offset;
	fHead.crlf[0] = 0x0d;
	fHead.crlf[1] = 0x0a;

	if( fwrite(&fHead, sizeof(BF_HEAD),1,fp)!=1){
		fclose(fp);
		memset( &fHead, 0, sizeof(BF_HEAD));
		return(BTB_CANT_CREATE);
	}

	memcpy( m_fName, fname,strlen(fname) > MAX_PATH ? MAX_PATH:strlen(fname) );
	fclose(fp);

	return(BTB_OK);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::Open(char* fname)
//	�����o�Fpublic
//	�@�\�@�F���삷��t�@�C�������Z�b�g����
//	�����@�F�\����`�p�̍\���́hBFSPEC�h
//	�����Q�F�t�@�C���ҏW���̃o�b�t�@�[�T�C�Y
//	�߂�l�F0=�����A���̑�=�����G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Open(char* fname)
{
	int ret;
	FILE* fp;
	BF_HEAD fHead;					// �t�@�C���w�b�_

	if( NULL == fname || !strlen(fname) ) return(BTB_NAME_FAULT);

	// �o�b�t�@��J�[�\���̏�����
	memset( m_fName, 0, sizeof(m_fName) );
	memcpy( m_fName, fname,strlen(fname) > MAX_PATH ? MAX_PATH:strlen(fname) );
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));

	// �t�@�C�������݂��邩�H
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);


	// shared open
	fp = _fsopen( m_fName, "rb",_SH_DENYWR );

	if( fp == NULL ) return(BTB_LOCKED);

	// �t�@�C���w�b�_�ǂݏo��
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );
	fclose(fp);

	// �����t�@�C���ł͂Ȃ�
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 )	|| fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		return(BTB_DIFF_FILE);
	}

	return(BTB_OK);
}


//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TBinaryTable::First(  unsigned short* reclen )
//	�����o�Fprotected
//	�@�\�@�F�ŏ��̃��R�[�h�Ɉړ����A�ǂݍ��݂ɕK�v�ȃf�[�^����Ԃ�
//	�����@�F���R�[�h�T�C�Y�i�[�|�C���^�i�o�́j
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::First(  unsigned short* reclen )
{
	return(this->Move(reclen,0));
}
//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TBinaryTable::Last(  unsigned short* reclen )
//	�����o�Fprotected
//	�@�\�@�F�ŏI�̃��R�[�h�Ɉړ����A�ǂݍ��݂ɕK�v�ȃf�[�^����Ԃ�
//	�����@�F���R�[�h�T�C�Y�i�[�|�C���^�i�o�́j
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Last(  unsigned short* reclen )
{
	return(this->Move(reclen,1));
}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TBinaryTable::Next(  unsigned short* reclen )
//	�����o�Fprotected
//	�@�\�@�F(�O��operation�ɑ΂���)���̃��R�[�h�����擾���A�K�v�ȃf�[�^����Ԃ�
//	�����@�F���R�[�h�T�C�Y�i�[�|�C���^�i�o�́j
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Next(  unsigned short* reclen )
{
	return(this->Move(reclen,2));

}
//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TBinaryTable::Prev(  unsigned short* reclen )
//	�����o�Fprotected
//	�@�\�@�F(�O��operation�ɑ΂���)���̃��R�[�h�����擾���A�K�v�ȃf�[�^����Ԃ�
//	�����@�F���R�[�h�T�C�Y�i�[�|�C���^�i�o�́j
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Prev(  unsigned short* reclen )
{
	return(this->Move(reclen,3));

}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint Move(unsigned short reclen,int operation)
//	�����o�Fprivate
//	�@�\�@�F�w�肳�ꂽ�����ɃJ�����g�|�W�V�������ړ����A���̃��R�[�h�̃��R�[�h����Ԃ�
//	�����@�F���R�[�h�T�C�Y�i�[�|�C���^�i�o�́j
//	�����Q�F�i�s���� 0=�ŏ��̃��R�[�h�A1=�Ō�̃��R�[�h�A2=���̃��R�[�h,3=�O�̃��R�[�h
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Move(unsigned short *reclen,int operation)
{
	int ret;
	long move;

	BF_HEAD fHead;					// �t�@�C���w�b�_

	// �o�b�t�@������
	memset( &fHead, 0, sizeof(BF_HEAD));
	*reclen = 0;

	// �����G���[
	if( operation < 0 || operation >3 )return(BTB_OPE_ERR);

	// Next()�̎��A���̃��R�[�h������̂��H
	else if( operation == 2 ){	
		if( m_Pblock.rec_pos==0 )return(BTB_POS_ERR);
		if(m_Pblock.next_pos ==0 ) return(BTB_EOF);
	}
	else if( operation == 3 ){
		if( m_Pblock.rec_pos==0 )return(BTB_POS_ERR);
		if(m_Pblock.prev_pos ==0 ) return(BTB_EOF);
	}

	// �t�@�C�������݂��邩�H
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	FILE* fp;

	// shared open
	fp = _fsopen( m_fName, "rb",_SH_DENYWR );

	if( fp == NULL ) return(BTB_LOCKED);

	// �t�@�C���w�b�_�ǂݏo��
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );
	// �����t�@�C���ł͂Ȃ�
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 )	|| fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// ���R�[�h�����݂��Ȃ�
	if( fHead.first_pos < sizeof(BF_HEAD) ){
		fclose(fp);
		return(BTB_EOF);
	}

	// �I�y���[�V�����ʂɈړ��ʂ����߂�
	if( operation == 0 ){		// First()
		memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
		move = fHead.first_pos-sizeof(BF_HEAD);
	}
	else if( operation == 1 ){	// Last
		memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
		move = fHead.last_pos-sizeof(BF_HEAD);
	}
	else if( operation == 2 ){	// Next()
		move = m_Pblock.next_pos-sizeof(BF_HEAD);
	}
	else{
		move = m_Pblock.prev_pos-sizeof(BF_HEAD);
	}

	// �Ώۂ̃��R�[�h�Ɉړ����A�|�W�V�����u���b�N��ǂݏo��
	ret = fseek( fp, move,SEEK_CUR);
	if( ret ){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	ret = (int)fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp );
	fclose(fp);

	if( ret !=1 ) return(BTB_IO_ERR);

	*reclen=m_Pblock.rec_len;

	return(BTB_OK);
}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TBinaryTable::SearchFile(char* fname)
//	�����o�Fprivate
//	�@�\�@�F�w�肳�ꂽ�t�@�C���̌���
//	�����P�F�����t�@�C����
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::SearchFile(char* fname)
{

	// �t�@�C���̃`�F�b�N
	if( NULL == m_fName || !strlen(m_fName) ) return(BTB_NAME_FAULT);

	struct _finddata_t c_file;
	long hFile;

	if( (hFile = _findfirst( fname, &c_file )) == -1L ) return(BTB_NOFILE);
	_findclose( hFile );

	return(BTB_OK);

}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::Find( void *key ,unsigned short* reclen)
//	�����o�Fprotected
//	�@�\�@�F�����L�[�ɍ��v���郌�R�[�h������
//	�����P�F�����L�[
//	�����Q�F�i���������ꍇ�j���̃��R�[�h�̒���
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Find( void *key ,unsigned short* reclen)
{
	return(this->SearchKey(key,0,reclen));
}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::GE( void *key ,unsigned short* reclen)
//	�����o�Fprotected
//	�@�\�@�F�����L�[�ɍ��v���邩����ȏ�̃��R�[�h������
//	�����P�F�����L�[
//	�����Q�F�i���������ꍇ�j���̃��R�[�h�̒���
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::GE( void *key ,unsigned short* reclen)
{
	return(this->SearchKey(key,1,reclen));
}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::GT( void *key ,unsigned short* reclen)
//	�����o�Fprotected
//	�@�\�@�F�����L�[�ɂ��傫�����R�[�h������
//	�����P�F�����L�[
//	�����Q�F�i���������ꍇ�j���̃��R�[�h�̒���
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::GT( void *key ,unsigned short* reclen)
{
	return(this->SearchKey(key,3,reclen));
}
//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::GE( void *key ,unsigned short* reclen)
//	�����o�Fprotected
//	�@�\�@�F�����L�[�ɍ��v���邩����ȉ��̃��R�[�h������
//	�����P�F�����L�[
//	�����Q�F�i���������ꍇ�j���̃��R�[�h�̒���
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::LE( void *key ,unsigned short* reclen)
{
	return(this->SearchKey(key,2,reclen));
}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::GT( void *key ,unsigned short* reclen)
//	�����o�Fprotected
//	�@�\�@�F�����L�[�ɂ�菬�������R�[�h������
//	�����P�F�����L�[
//	�����Q�F�i���������ꍇ�j���̃��R�[�h�̒���
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::LT( void *key ,unsigned short* reclen)
{
	return(this->SearchKey(key,4,reclen));
}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TBinaryTable::SearchKey( void *key ,int operation, unsigned short* reclen);
//	�����o�Fprivate
//	�@�\�@�F���R�[�h����/�L�[�ɍ��v���郌�R�[�h������
//	�����P�F�����L�[
//	�����Q�F��r���@ 0=Equal 1=GE,2=LE 3=GT,4=LT
//	�����R�F�i���������ꍇ�j���̃��R�[�h�̒���
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::SearchKey( void *key ,int operation, unsigned short* reclen)
{

	BF_HEAD fHead;					// �t�@�C���w�b�_

	// �o�b�t�@������
	memset( &fHead, 0, sizeof(BF_HEAD));
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
	*reclen = 0;

	// �����G���[
	if( key==NULL )return(BTB_OPE_ERR);

	int ret;
	long move;
	FILE* fp;
	char keybuf[MAX_KEY_LENGTH];

	// �t�@�C�������݂��邩�H
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	// shared open
	fp = _fsopen( m_fName, "rb",_SH_DENYWR );

	if( fp == NULL ) return(BTB_LOCKED);

	// �t�@�C���w�b�_�ǂݏo��
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );

	// �����t�@�C���ł͂Ȃ�
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 )	|| fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// ���R�[�h�����݂��Ȃ�
	if( fHead.first_pos < sizeof(BF_HEAD) ){
		fclose(fp);
		return(BTB_EOF);
	}

	move = fHead.first_pos;

	do{
		// �ړ�	
		ret = fseek( fp, move,SEEK_SET);
		if( ret ){
			ret= BTB_IO_ERR;
			break;
		}

		// �|�W�V�����u���b�N�ǂݍ���
		ret = (int)fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp );
		if( ret!=1 ){
			ret= BTB_IO_ERR;
			break;
		}

		// �L�[�ʒu�ֈړ�
		ret = fseek( fp, fHead.offset, SEEK_CUR);
		if( ret ){
			ret= BTB_IO_ERR;
			break;
		}

		// �L�[�ǂݍ���
		ret = (int)fread( keybuf, fHead.keylen,1,fp );
		if( ret!=1 ){
			ret= BTB_IO_ERR;
			break;
		}

		// �L�[��r
		ret = memcmp(keybuf, key, fHead.keylen);

		// LT�I�y���[�V�����B���̏ꍇ�A���݂̑O�̃��R�[�h���r���S
		if( ret >=0 && operation == 4 ){
			if( !m_Pblock.prev_pos){	// ���R�[�h����
				ret= BTB_NOTFIND;
				break;
			}
			// �ړ�	
			if( fseek( fp, m_Pblock.prev_pos,SEEK_SET)){
				ret= BTB_IO_ERR;
				break;
			}
			// �|�W�V�����u���b�N�ǂݍ���
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1){
				ret= BTB_IO_ERR;
				break;
			}
			ret= BTB_OK;
			*reclen=m_Pblock.rec_len;
			break;
		}

		// �L�[����		Equal�n�I�y���[�V����
		else if( !ret && operation <3){
			ret= BTB_OK;
			*reclen=m_Pblock.rec_len;
			break;
		}

		// �L�[ > �����L�[
		else if( ret > 0 ){
			// Equal�̏ꍇ�A�L�[��������Ȃ�
			if( operation == 0){
				ret= BTB_NOTFIND;
				break;
			}
			// Greater�n�I�y���[�V�����Bkeybuf>key�Ȃ̂ŁA���ꂪ�r���S
			else if( operation == 1 || operation ==3 ){
				ret= BTB_OK;
				*reclen=m_Pblock.rec_len;
				break;
			}
		}

		// ���̃��R�[�h��
		move = m_Pblock.next_pos;

		if(move < sizeof(BF_HEAD) ){
			ret= BTB_NOTFIND;break;
		}

	}while(1);

	fclose(fp);
	return(ret);

}
//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::ReadRecord(void* buf, unsigned short bufsize)
//	�����o�Fprotected
//	�@�\�@�F���ă��R�[�h�̃f�[�^��ǂݏo��
//	�����P�F�擾�o�b�t�@
//	�����Q�F�擾�o�b�t�@�T�C�Y
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::ReadRecord(void* buf, unsigned short bufsize)
{
	// �����G���[
	if( buf==NULL )return(BTB_OPE_ERR);

	int ret;
	BF_HEAD fHead;					// �t�@�C���w�b�_
	BF_POSBLOCK pb;

	// �o�b�t�@������
	memset( &fHead, 0, sizeof(BF_HEAD));

	// �t�@�C�������݂��邩�H
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	FILE* fp;

	// shared open
	fp = _fsopen( m_fName, "rb",_SH_DENYWR );

	if( fp == NULL ) return(BTB_LOCKED);

	// �t�@�C���w�b�_�ǂݏo��
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );
	// �����t�@�C���ł͂Ȃ�
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 )	|| fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// ���R�[�h�����݂��Ȃ�
	if( fHead.first_pos < sizeof(BF_HEAD) ){
		fclose(fp);
		return(BTB_EOF);
	}

	// �J�����g�u���b�N���s��
	if( m_Pblock.rec_pos==0){
		fclose(fp);
		return(BTB_POS_ERR);
	}

	// �Ώۂ̃��R�[�h�Ɉړ����A�|�W�V�����u���b�N��ǂݏo��
	ret = fseek( fp, m_Pblock.rec_pos,SEEK_SET);
	if( ret ){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	ret = (int)fread( &pb, sizeof(BF_POSBLOCK),1,fp );
	if( ret !=1 ){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	// �폜���ꂽ���R�[�h�̏ꍇ
	if( pb.del_sign){
		fclose(fp);
		memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
		return(BTB_POS_ERR);
	}

	// �f�[�^�o�b�t�@���s��
	if( pb.rec_len > bufsize){ 
		fclose(fp);
		return(BTB_DBUFLEN_ERR);
	}

	if( fread( (char*)buf, pb.rec_len,1,fp )!=1){
		fclose(fp);
		memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
		return(BTB_IO_ERR);
	}

	fclose(fp);
	memcpy( &m_Pblock, &pb, sizeof(BF_POSBLOCK));
	return(BTB_OK);
}

//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TBinaryTable::Add( void* data, unsigned short reclen)
//	�����o�Fprotected
//	�@�\�@�F�V�K�Ƀf�[�^��ǉ�����
//	�����P�F�}���f�[�^
//	�����P�F�}���f�[�^��
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Add( void* data, unsigned short reclen)
{
	int ret;
	long move;
	unsigned long wr_pos;
	FILE* fp;
	char keybuf[MAX_KEY_LENGTH];
	BF_POSBLOCK pb;
	BF_HEAD fHead;					// �t�@�C���w�b�_

	// �o�b�t�@������
	memset( &fHead, 0, sizeof(BF_HEAD));
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
	memset( &pb, 0, sizeof(BF_POSBLOCK));

	// �����G���[
	if( data==NULL )return(BTB_OPE_ERR);

	// �t�@�C�������݂��邩�H
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	// open
	fp = fopen( m_fName, "r+b" );
	if( fp == NULL ) return(BTB_LOCKED);

	// �t�@�C���w�b�_�ǂݏo��
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );

	// �����t�@�C���ł͂Ȃ�
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 )	|| fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// ���R�[�h���̃`�F�b�N
	if( reclen < fHead.keylen+fHead.offset){
		fclose(fp);
		return(BTB_RECLEN_ERR);
	}

	// �������݈ʒu�iEOF�j�̌��o
	ret = fseek( fp, 0,SEEK_END);
	wr_pos=(unsigned long)ftell(fp);

	// ���R�[�h����ꍇ�́A�}���ʒu��T��
	if( fHead.first_pos >0){
		move = fHead.first_pos;
		ret = BTB_IO_ERR;
		do{
			// �ړ�	
			if( 0 !=(ret = fseek( fp, move,SEEK_SET)) ){;
				ret= BTB_IO_ERR;break;
			}

			// �|�W�V�����u���b�N�ǂݍ���
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
				ret= BTB_IO_ERR;
				break;
			}

			// �L�[�ʒu�ֈړ�
			if( fseek( fp, fHead.offset, SEEK_CUR) ){
				ret= BTB_IO_ERR;
				break;
			}

			// �L�[�ǂݍ���
			if( fread( keybuf, fHead.keylen,1,fp ) !=1 ){
				ret= BTB_IO_ERR;
				break;
			}

			// �L�[��r
			ret = memcmp(keybuf, (char*)data+fHead.offset, fHead.keylen);
			// �������R�[�h����������G���[
			if( !ret){
				ret= BTB_DUP;
				break;
			}
			// �������傫�ȃ��R�[�h���������炻�̑O�ɑ}��	 �L�[ > �}���L�[
			else if( ret > 0 ){
				pb.next_pos = m_Pblock.rec_pos;
				pb.prev_pos = m_Pblock.prev_pos;
				ret= BTB_OK;
				break;
			}

			// ���̃��R�[�h��
			move = m_Pblock.next_pos;
			// �Ō�܂ł�������A�������ŏI���R�[�h�ɂȂ�
			if(move < sizeof(BF_HEAD) ){
				pb.next_pos = 0;
				pb.prev_pos = m_Pblock.rec_pos;
				ret= BTB_OK;
				break;
			}

		}while(1);

		// �G���[
		if( ret !=BTB_OK){
			fclose(fp);
			return(ret);
		}
	}

	// �������ŏ��̃��R�[�h�ɂȂ�ꍇ
	if( pb.prev_pos == 0 ){
		fHead.first_pos=wr_pos;
	}
	// �}������O�̃��R�[�h��ҏW
	else{
		// �ړ�	
		if( fseek( fp, pb.prev_pos,SEEK_SET) ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// �|�W�V�����u���b�N�ǂݍ���
		if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// �f�[�^��������
		m_Pblock.next_pos=wr_pos;
		if( fseek( fp, -1*(long)sizeof(BF_POSBLOCK),SEEK_CUR) ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		if( fwrite(&m_Pblock, sizeof(BF_POSBLOCK),1,fp)!=1){
			fclose(fp);
			return(BTB_IO_ERR);
		}
	}

	// �������Ō�̃��R�[�h�ɂȂ�ꍇ
	if( pb.next_pos == 0 ){
		fHead.last_pos=wr_pos;
	}
	else{
		// �ړ�	
		if(fseek( fp, pb.next_pos,SEEK_SET)){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// �|�W�V�����u���b�N�ǂݍ���
		if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// �f�[�^��������
		m_Pblock.prev_pos=wr_pos;
		if( fseek( fp, -1*(long)sizeof(BF_POSBLOCK),SEEK_CUR) ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		if( fwrite(&m_Pblock, sizeof(BF_POSBLOCK),1,fp)!=1){
			fclose(fp);
			return(BTB_IO_ERR);
		}
	}

	// �w�b�_�X�V
	ret = fseek( fp, 0,SEEK_SET);
	if( fwrite(&fHead, sizeof(BF_HEAD),1,fp)!=1){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	// �|�W�V�����u���b�N��������
	pb.rec_pos = wr_pos;
	pb.rec_len = reclen;
	ret = fseek( fp, 0,SEEK_END);
	if( fwrite(&pb, sizeof(BF_POSBLOCK),1,fp)!=1){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	// �f�[�^��������
	if( fwrite(data, reclen,1,fp)!=1){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	// �}���������R�[�h����Ăɂ���
	fclose(fp);
	memcpy( &m_Pblock, &pb, sizeof(BF_POSBLOCK));
	return(BTB_OK);
}



//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TBinaryTable::Upd( char* data, unsigned short reclen )
//	�����o�Fprotected
//	�@�\�@�F
//	�����P�F�X�V����f�[�^
//	�����Q�F�X�V����f�[�^�̒���
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Upd( void* data, unsigned short reclen )
{
	int ret;
	long move;
	FILE* fp;
	char keybuf[MAX_KEY_LENGTH];
	BF_POSBLOCK pb;
	BF_HEAD fHead;					// �t�@�C���w�b�_
	unsigned long wr_pos;

	// �o�b�t�@������
	memset( &fHead, 0, sizeof(BF_HEAD));
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
	memset( &pb, 0, sizeof(BF_POSBLOCK));

	// �����G���[
	if( data==NULL )return(BTB_OPE_ERR);

	// �t�@�C�������݂��邩�H
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	// open
	fp = fopen( m_fName, "r+b" );
	if( fp == NULL ) return(BTB_LOCKED);

	// �t�@�C���w�b�_�ǂݏo��
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );

	// �����t�@�C���ł͂Ȃ�
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 ) || fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// ���R�[�h����
	if( fHead.first_pos ==0){
		fclose(fp);
		return(BTB_EOF);
	}

	// ���R�[�h���̃`�F�b�N
	if( reclen < fHead.keylen+fHead.offset){
		fclose(fp);
		return(BTB_RECLEN_ERR);
	}

	// ���R�[�h����������
	move = fHead.first_pos;
	ret = BTB_IO_ERR;
	do{
		// �ړ�	
		if( 0 !=(ret = fseek( fp, move,SEEK_SET)) ){;
			ret= BTB_IO_ERR;break;
		}

		// �|�W�V�����u���b�N�ǂݍ���
		if( fread( &pb, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			ret= BTB_IO_ERR;break;
		}

		// �L�[�ʒu�ֈړ�
		if( fseek( fp, fHead.offset, SEEK_CUR) ){
			ret= BTB_IO_ERR;break;
		}

		// �L�[�ǂݍ���
		if( fread( keybuf, fHead.keylen,1,fp ) !=1 ){
			ret= BTB_IO_ERR;break;
		}

		// �L�[��r
		ret = memcmp(keybuf, (char*)data+fHead.offset, fHead.keylen);
		// �L�[����
		if( !ret){
			ret= BTB_OK;
			break;
		}
		// �L�[��������Ȃ�
		else if( ret > 0 ){
			ret= BTB_NOTFIND;break;
		}

		// ���̃��R�[�h��
		move = pb.next_pos;
		// �Ō�܂ł�������A�������ŏI���R�[�h�ɂȂ�
		if(move < sizeof(BF_HEAD) ){
			ret= BTB_NOTFIND;break;
		}
	}while(1);

	// �G���[
	if( ret !=BTB_OK){
		fclose(fp);
		return(ret);
	}

	// ���R�[�h���ɕω��������ꍇ�͂��̂܂�
	if( reclen == pb.rec_len ){
		if( fseek( fp, pb.rec_pos+sizeof(BF_POSBLOCK),SEEK_SET) ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// �f�[�^��������
		if( fwrite((char*)data, reclen,1,fp)!=1){
			fclose(fp);
			return(BTB_IO_ERR);
		}
	}
	// ���R�[�h���ɕω����������ꍇ�́A��x�폜���Ēǉ�����
	else{
		// �������݈ʒu�iEOF�j�̌��o
		ret = fseek( fp, 0,SEEK_END);
		wr_pos=(unsigned long)ftell(fp);

		// prev���R�[�h�̕ҏW
		if( pb.prev_pos ){
			if( fseek( fp, pb.prev_pos,SEEK_SET) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// �|�W�V�����u���b�N�ǂݍ���
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// �f�[�^��������
			m_Pblock.next_pos=wr_pos;
			if( fseek( fp, -1*(long)sizeof(BF_POSBLOCK),SEEK_CUR) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			if( fwrite(&m_Pblock, sizeof(BF_POSBLOCK),1,fp)!=1){
				fclose(fp);
				return(BTB_IO_ERR);
			}
		}
		// �������擪���R�[�h�ł���ꍇ
		else{
			// �w�b�_�X�V
			fHead.first_pos=wr_pos;
			ret = fseek( fp, 0,SEEK_SET);
			if( fwrite(&fHead, sizeof(BF_HEAD),1,fp)!=1){
				fclose(fp);
				return(BTB_IO_ERR);
			}
		}

		// next���R�[�h�̕ҏW
		if( pb.next_pos ){
			if( fseek( fp, pb.next_pos,SEEK_SET) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// �|�W�V�����u���b�N�ǂݍ���
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// �f�[�^��������
			m_Pblock.prev_pos=wr_pos;
			if( fseek( fp, -1*(long)sizeof(BF_POSBLOCK),SEEK_CUR) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			if( fwrite(&m_Pblock, sizeof(BF_POSBLOCK),1,fp)!=1){
				fclose(fp);
				return(BTB_IO_ERR);
			}
		}
		// �������ŏI���R�[�h�ł���ꍇ
		else{
			// �w�b�_�X�V
			fHead.last_pos=wr_pos;
			ret = fseek( fp, 0,SEEK_SET);
			if( fwrite(&fHead, sizeof(BF_HEAD),1,fp)!=1){
				fclose(fp);
				return(BTB_IO_ERR);
			}
		}

		// �������R�[�h�̍폜
		ret = fseek( fp, pb.rec_pos,SEEK_SET);
		pb.del_sign=1;
		if( fwrite(&pb, sizeof(BF_POSBLOCK),1,fp)!=1){
			fclose(fp);
			return(BTB_IO_ERR);
		}

		// ���R�[�h�ǉ�
		pb.rec_pos=wr_pos;
		pb.del_sign=0;
		pb.rec_len=reclen;
		ret = fseek( fp, 0,SEEK_END);
		if( fwrite(&pb, sizeof(BF_POSBLOCK),1,fp)!=1){	// �|�W�V�����u���b�N��������
			fclose(fp);
			return(BTB_IO_ERR);
		}
		if( fwrite((char*)data, reclen,1,fp)!=1){	// �f�[�^��������
			fclose(fp);
			return(BTB_IO_ERR);
		}
	}

	fclose(fp);
	memcpy( &m_Pblock, &pb, sizeof(BF_POSBLOCK));
	return(BTB_OK);
}


//---------------------------------------------------------------------------------------------------------------------
//	�֐����Fint TBinaryTable::Del( void *key )
//	�����o�Fprotected
//	�@�\�@�F�L�[�ɍ��v���郌�R�[�h���폜����
//	�����@�F�폜����L�[
//	�߂�l�F0=�����A���̑�=�G���[
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Del( void *key )
{
	int ret;
	long move;
	FILE* fp;
	char keybuf[MAX_KEY_LENGTH];
	BF_POSBLOCK pb;
	BF_HEAD fHead;					// �t�@�C���w�b�_

	// �o�b�t�@������
	memset( &fHead, 0, sizeof(BF_HEAD));
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
	memset( &pb, 0, sizeof(BF_POSBLOCK));

	// �����G���[
	if( key==NULL )return(BTB_OPE_ERR);


	// �t�@�C�������݂��邩�H
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	// open
	fp = fopen( m_fName, "r+b" );

	if( fp == NULL ) return(BTB_LOCKED);

	// �t�@�C���w�b�_�ǂݏo��
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );

	// �����t�@�C���ł͂Ȃ�
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 ) || fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// ���R�[�h����
	if( fHead.first_pos ==0){
		fclose(fp);
		return(BTB_EOF);
	}

	// ���R�[�h����������
	move = fHead.first_pos;
	ret = BTB_IO_ERR;
	do{
		// �ړ�	
		if( 0 !=(ret = fseek( fp, move,SEEK_SET)) ){;
			ret= BTB_IO_ERR;break;
		}

		// �|�W�V�����u���b�N�ǂݍ���
		if( fread( &pb, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			ret= BTB_IO_ERR;break;
		}

		// �L�[�ʒu�ֈړ�
		if( fseek( fp, fHead.offset, SEEK_CUR) ){
			ret= BTB_IO_ERR;break;
		}

		// �L�[�ǂݍ���
		if( fread( keybuf, fHead.keylen,1,fp ) !=1 ){
			ret= BTB_IO_ERR;break;
		}

		// �L�[��r
		ret = memcmp(keybuf, key, fHead.keylen);
		// �L�[����
		if( !ret){
			ret= BTB_OK;
			break;
		}
		// �L�[��������Ȃ�
		else if( ret > 0 ){
			ret= BTB_NOTFIND;break;
		}

		// ���̃��R�[�h��
		move = pb.next_pos;
		// �Ō�܂ł�������A�������ŏI���R�[�h�ɂȂ�
		if(move < sizeof(BF_HEAD) ){
			ret= BTB_NOTFIND;break;
		}
	}while(1);

	// �G���[
	if( ret !=BTB_OK){
		fclose(fp);
		return(ret);
	}

	// �������擪���R�[�h�̏ꍇ�́A�����R�[�h��擪�ɂ���
	if( pb.prev_pos == 0 ){	
		fHead.first_pos=pb.next_pos;
		if( pb.next_pos){
			// ���̃��R�[�h���������A���ꂪ�擪�ƂȂ�
			if( fseek( fp, pb.next_pos,SEEK_SET) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// �|�W�V�����u���b�N�ǂݍ���
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// �f�[�^��������(�擪���R�[�h����)
			m_Pblock.prev_pos=0;
			if( fseek( fp, -1*(long)sizeof(BF_POSBLOCK),SEEK_CUR) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			if( fwrite(&m_Pblock, sizeof(BF_POSBLOCK),1,fp)!=1){
				fclose(fp);
				return(BTB_IO_ERR);
			}
		}
		// �f�[�^���O���ɂȂ�
		else{
			fHead.last_pos=0;
			memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));	// �|�W�V�����u���b�N�𖳌��ɂ���
		}
	}
	// �폜�Ώۂ́A�O(prev)���R�[�h��ҏW
	else{
		// �ړ�	
		if( fseek( fp, pb.prev_pos,SEEK_SET) ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// �|�W�V�����u���b�N�ǂݍ���
		if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// �f�[�^��������
		m_Pblock.next_pos=pb.next_pos;
		if( fseek( fp, -1*(long)sizeof(BF_POSBLOCK),SEEK_CUR) ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		if( fwrite(&m_Pblock, sizeof(BF_POSBLOCK),1,fp)!=1){
			fclose(fp);
			return(BTB_IO_ERR);
		}
	}

	// �������Ō�̃��R�[�h�̏ꍇ�́A�O���R�[�h���ŏI�Ɋ��蓖�Ă�
	if( pb.next_pos == 0 ){
		fHead.last_pos=pb.prev_pos;
		if( pb.prev_pos){
			// ���̃��R�[�h���������A���ꂪ�擪�ƂȂ�
			if( fseek( fp, pb.prev_pos,SEEK_SET) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// �|�W�V�����u���b�N�ǂݍ���
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// �f�[�^��������(�ŏI���R�[�h����)
			m_Pblock.next_pos=0;
			if( fseek( fp, -1*(long)sizeof(BF_POSBLOCK),SEEK_CUR) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			if( fwrite(&m_Pblock, sizeof(BF_POSBLOCK),1,fp)!=1){
				fclose(fp);
				return(BTB_IO_ERR);
			}
		}
		// �f�[�^���O���ɂȂ�
		else{
			fHead.first_pos=0;
			memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));	// �|�W�V�����u���b�N�𖳌��ɂ���
		}
	}
	// ����ȊO�́Anext���R�[�h�̕ҏW
	else{
		// �ړ�	
		if(fseek( fp, pb.next_pos,SEEK_SET)){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// �|�W�V�����u���b�N�ǂݍ���
		if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// �f�[�^��������
		m_Pblock.prev_pos=pb.prev_pos;
		if( fseek( fp, -1*(long)sizeof(BF_POSBLOCK),SEEK_CUR) ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		if( fwrite(&m_Pblock, sizeof(BF_POSBLOCK),1,fp)!=1){
			fclose(fp);
			return(BTB_IO_ERR);
		}
	}

	// �w�b�_�X�V
	ret = fseek( fp, 0,SEEK_SET);
	if( fwrite(&fHead, sizeof(BF_HEAD),1,fp)!=1){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	// �|�W�V�����u���b�N��������
	pb.del_sign = 1;				// �폜
	fseek( fp, pb.rec_pos,SEEK_SET);
	if( fwrite(&pb, sizeof(BF_POSBLOCK),1,fp)!=1){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	fclose(fp);
	return(BTB_OK);
}
#pragma pack(pop)

