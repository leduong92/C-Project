//=====================================================================================================================
//
//	FileName	:TPgBase.CPP
//	Class		:TPgBase
//				: postgres��DB��{����N���X
//	�쐬��		:2002.05.07	Coded by Coba
//
//=====================================================================================================================
//	�C��
//=====================================================================================================================
#include "tpgbase.h"

//---------------------------------------------------------------------------------------------------------------------
//	TPgBase::TPgBase()
//	�^�C�v�Fpublic
//	�@�\�@�F�f�t�H���g�R���X�g���N�^�B�����o�ϐ��̏�����
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
TPgBase::TPgBase()
{
	m_pgConn = NULL;
}

//---------------------------------------------------------------------------------------------------------------------
//	TPgBase::~TPgBase()
//	�^�C�v�Fpublic
//	�@�\�@�F�f�X�g���N�^�B�I�u�W�F�N�g�⃁�����̉��
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
TPgBase::~TPgBase()
{
	if( m_pgConn !=NULL ) PS_DisConnect( m_pgConn);
}

//---------------------------------------------------------------------------------------------------------------------
// 2010.12.17 coba �ؒf�֐�
//	�^�C�v�Fpublic
//	�@�\�@�F�f�[�^�x�[�X�ؒf
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
void TPgBase::CloseDB(void)
{
	if( m_pgConn !=NULL ){
		PS_DisConnect( m_pgConn);
		m_pgConn = NULL;
	}
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TPgBase::Connect(void)
//	�^�C�v�Fpublic
//	�@�\�@�F�f�[�^�x�[�X�ɐڑ�
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TPgBase::OpenConnect(char* host,char* db,char* user,char* pass ,int port/*=5432*/)
{
	if( NULL==host || NULL==db || NULL==user || NULL == pass ) return(-1);
	char con_str[256];

/*
	// DB�ڑ�������̍쐬:IP���ڽ
	if(host[0] >='0' && host[0] <='9')
		sprintf( con_str,"hostaddr=%s dbname=%s user=%s password=%s",host,db,user,pass);
*/

	// 2010.09.03 matuda ������port��ǉ�
	sprintf( con_str,"host=%s dbname=%s user=%s password=%s port=%ld",host,db,user,pass,port);

	// DB�I�[�v��

	if(PS_Connect(&m_pgConn, con_str)){
		return(-2);
	}

	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TPgBase::Commit(int mode)
//	�^�C�v�Fpublic
//	�@�\�@�F�g�����U�N�V�����̃R�~�b�g
//	�����P�F�R�~�b�g���[�h
//			PGCMT_START(0),		�J�n
//			PGCMT_COMMIT(1),		�I��
//			PGCMT_ROLLBACK(2),	���[���o�b�N
//---------------------------------------------------------------------------------------------------------------------
int TPgBase::Commit(int mode)
{
	if( mode < PGCMT_START || mode > PGCMT_ROLLBACK ) return(-1);

	int ret;
	char buf[40];

	switch(mode){
		case PGCMT_START:
			sprintf(buf,"begin");
			break;

		case PGCMT_COMMIT:
			sprintf(buf,"commit");
			break;

		case PGCMT_ROLLBACK:
			sprintf(buf,"rollback");
			break;
		default:return(-1);
	}

	// DB�̐ڑ����m�F
	ret=this->ChkConnect();
	if( ret ) return(ret);
	ret=PS_Exec(m_pgConn,buf);
	return(ret);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TPgTable::ChkConnect(void)
//	�^�C�v�Fprotected
//	�@�\�@�FDB�ڑ��󋵂𒲂ׁA�؂�Ă�����Đڑ�����
//	�߂�l�F0=���� 
//			-100=�ڑ������� 
//			-101 �Đڑ��Ɏ��s
//---------------------------------------------------------------------------------------------------------------------
int TPgBase::ChkConnect(void)
{
	if(m_pgConn == NULL) return(-100);

	// DB�̐ڑ����m�F
	if( PS_IsConnect( m_pgConn) ){
		if( PS_ReConnect(&m_pgConn)) return(-101);
	}
	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TPgBase::SetEncoding(char* encoding)
//	�^�C�v�Fprotected
//	�@�\�@�F�N���C�A���g���̃G���R�[�f�B���O�̕ύX 2010.05.20 coba
//	�߂�l�F0=���� 
//			-100=�ڑ������� 
//---------------------------------------------------------------------------------------------------------------------
int TPgBase::SetEncoding(char* encoding)
{
	int ret;

	if(m_pgConn == NULL) return(-100);

	// DB�̐ڑ����m�F
	ret=PS_Encoding( m_pgConn,encoding);
	return(ret);
}

//---------------------------------------------------------------------------------------------------------------------
// int TPgBase::GetSrvDateTime(SRV_DATETIME* dat)
//	�^�C�v�Fpublic
//	�@�\�@�F�T�[�o�����̎擾
//	�����F�T�[�o�����\����(�o��)
/*
typedef struct
{
	short y;	// �N
	short m;	// ��
	short d;	// ��
	short h;	// ��
	short mi;	// ��
	short s;	// �b
	char dt[14+1];	// YYYYMMDD
}SRV_DATETIME;
*/
//---------------------------------------------------------------------------------------------------------------------
int TPgBase::GetSrvDateTime(SRV_DATETIME* dat)
{
	int ret;
	PGresult* res;
	char sql[128],buf[64];

	if(m_pgConn == NULL) return(-100);

	// �₢���킹
	strcpy( sql,"select to_char(current_timestamp,'yyyymmddhh24missd') as sdate" );
	ret = PS_Select( m_pgConn,&res, sql);
	if(ret<1){
		PS_Close( res );
		return(-1);
	}
	// �擾
	memset( buf,0,sizeof(buf));
	strcpy( buf,PS_Col( res, 0,"sdate"));
	// �J�[�\������
	PS_Close( res );

	if(strlen(buf)!=15) return(-2);		// ����14+�j��1

	// �o��
	memcpy( dat->dt,buf,14);
	dat->dow=(short)(atoi(buf+14))-1;		// PostgreSQL�͓��j����1
	buf[14]=0;
	dat->s=(short)atoi(buf+12);
	buf[12]=0;
	dat->mi=(short)atoi(buf+10);
	buf[10]=0;
	dat->h=(short)atoi(buf+8);
	buf[8]=0;
	dat->d=(short)atoi(buf+6);
	buf[6]=0;
	dat->m=(short)atoi(buf+4);
	buf[4]=0;
	dat->y=(short)atoi(buf);

	return(0);
}

