//===================================================================================================================
//
//	FileName	: pslib.c
//				Postgres�p�̃��[�e�B���e�B
//	�쐬��		:2001.10.22 coba
//	�X�V��		: 
//===================================================================================================================

//#include "stdafx.h"
#include "pslib.h"

// Postgres�p�֐��ϐ�
//--------------------------------------------------------------------------------------------------------------------
//	int PS_Connect( PGconn** pg)
// �����@:	PostgreSQL�ɐڑ�
// �����P:�@�f�[�^�x�[�X�ڑ��ϐ�
// �߂�l: 0=����
//--------------------------------------------------------------------------------------------------------------------
int PS_Connect( PGconn** pg,char* constr)
{
	if( constr==NULL || strlen(constr) < 1 ) return(-2);

	*pg = PQconnectdb(constr); /* �f�[�^�x�[�X�ւ̐ڑ� */

	/* �f�[�^�x�[�X�ւ̐ڑ����s? */
	if (PQstatus(*pg) == CONNECTION_BAD) return(-1);

	return(0);
}
//--------------------------------------------------------------------------------------------------------------------
//	void PS_DisConnect( PGconn* pg,PGresult* res)
// �����@:DB�ڑ��̊J��
// �����P:�@�f�[�^�x�[�X�ڑ��ϐ�
// �����Q:�@���ʃZ�b�g���ϐ�
//--------------------------------------------------------------------------------------------------------------------
void PS_DisConnect( PGconn* pg)
{
	PQfinish(pg);
}

//--------------------------------------------------------------------------------------------------------------------
//	int IsConnect(PGconn* pg)
// �����@:DB�ڑ����L�����ǂ������ׂ�
// �����P:�@�f�[�^�x�[�X�ڑ��ϐ�
// �߂�l:0=�ڑ�OK
//--------------------------------------------------------------------------------------------------------------------
int PS_IsConnect(PGconn* pg)
{
	if (PQstatus(pg) == CONNECTION_BAD) return(-1);
	return(0);
}

//--------------------------------------------------------------------------------------------------------------------
//	int ReConnect(PGconn** pg)
// �����@:DB�ɍĐڑ�����
// �����P:�@�f�[�^�x�[�X�ڑ��ϐ�
//--------------------------------------------------------------------------------------------------------------------
int PS_ReConnect(PGconn** pg)
{
	PQreset(*pg); /* �f�[�^�x�[�X�ւ̐ڑ� */

	/* �f�[�^�x�[�X�ւ̐ڑ����s? */
	if (PQstatus(*pg) == CONNECTION_BAD) return(-1);
	return(0);
}

//--------------------------------------------------------------------------------------------------------------------
//	int PS_Select( PGconn* pg,PGresult* res, char* sql)
// �����@:select�����s
// �����P:�@�f�[�^�x�[�X�ڑ��ϐ�
// �����Q:�@���ʃZ�b�g���ϐ�
// �����R:�@���s����SELECT��
// �߂�l: 0���������s ����ȊO�����R�[�h��
//--------------------------------------------------------------------------------------------------------------------
int PS_Select( PGconn* pg,PGresult** res, char* sql)
{
	*res = PQexec(pg, sql);/* �₢���킹�̑��M */
	if (PQresultStatus(*res) != PGRES_TUPLES_OK) {
		return(-101);
	}

	return(PQntuples(*res));        /* �߂��Ă����s�� */
}

//--------------------------------------------------------------------------------------------------------------------
// void PS_Close( PGconn* pg,PGresult* res)
// �����@:���ʃZ�b�g�̃N���[�Y
// �����P:�@���ʃZ�b�g���ϐ�
//--------------------------------------------------------------------------------------------------------------------
void PS_Close( PGresult* res)
{
	PQclear(res);
}

//--------------------------------------------------------------------------------------------------------------------
//	int PS_GetColCounts(PGresult* res)
// �����@:�������ʂ̃t�B�[���h����Ԃ�
// �����P:�@���ʃZ�b�g
// �߂�l:�t�B�[���h��
//--------------------------------------------------------------------------------------------------------------------
int PS_GetColCounts(PGresult* res)
{
	return(PQnfields(res));
}

//--------------------------------------------------------------------------------------------------------------------
//	int PS_GetColName( PGresult* res, char* fieldname,int bufsize)
// �����@:�������ʂ̃t�B�[���h�ԍ����w�肵�āA���̃t�B�[���h�����o�b�t�@�Ɏ擾����
// �����P:�@���ʃZ�b�g
// �����Q:�@�t�B�[���h�i��j�ԍ�
// �����S:	�擾�o�b�t�@�̃|�C���^
// �����T:	�o�b�t�@�̃T�C�Y
// �߂�l:����=0,����ȊO=�G���[
//--------------------------------------------------------------------------------------------------------------------
int PS_GetColName( PGresult* res, int colno,char* fieldname,int bufsize)
{
	int len;

	// ��͗L����?
	if( colno >= PQnfields(res) ) return(-1);

	len=strlen(PQfname( res, colno));
	if( len <1 ) return(-2);

	// �񖼃R�s�[
	memcpy( fieldname, PQfname( res, colno), len>bufsize ? bufsize : len);
	return(0);
}

//--------------------------------------------------------------------------------------------------------------------
//	int PS_GetCol( PGresult* res, int rowno,int colno,char* buf,int bufsize)
// �����@:�s�ԍ��Ɨ�ԍ����w�肵�āA�t�B�[���h�f�[�^���o�b�t�@�Ɏ擾����
// �����P:�@���ʃZ�b�g
// �����Q:�@�擾�������s�̔ԍ�
// �����R:�@�擾��������̔ԍ�
// �����S:	�擾�o�b�t�@�̃|�C���^
// �����T:	�o�b�t�@�̃T�C�Y
//--------------------------------------------------------------------------------------------------------------------
int PS_GetCol( PGresult* res, int rowno,int colno,char* buf,int bufsize)
{
	int len;
	// ��͗L����?
	if( colno >= PQnfields(res) ) return(-1);

	// �s�͗L����?
	if( rowno >= PQntuples(res) ) return(-2);

	// �f�[�^�̒������擾
	len = PQgetlength(res,rowno,colno);
	if( len < 1 ) return(-4);

	memcpy( buf,PQgetvalue(res, rowno,colno),len>bufsize ? bufsize : len);
	return(0);
}

//--------------------------------------------------------------------------------------------------------------------
//	int PS_GetCol( PGresult* res, int rowno,int colno,char* buf,int bufsize)
// �����@:�s�ԍ��ƃt�B�[���h�����w�肵�āA�t�B�[���h�f�[�^���o�b�t�@�Ɏ擾����(select * ��������)
// �����P:�@���ʃZ�b�g
// �����Q:�@�擾�������s�̔ԍ�
// �����R:�@�擾��������̖��O
// �����S:	�擾�o�b�t�@�̃|�C���^
// �����T:	�o�b�t�@�̃T�C�Y
//--------------------------------------------------------------------------------------------------------------------
int PS_GetByField( PGresult* res, int rowno,char* fieldname,char* buf,int bufsize)
{
	int f_index;
	int len;

	// �t�B�[���h�̃C���f�b�N�X��T��
	if( (f_index=PQfnumber(res,fieldname))==-1) return(-1);

	// �s�͗L����?
	if( rowno >= PQntuples(res) ) return(-2);        /* �߂��Ă����s�� */

	// NULL?
	if( PQgetisnull(res,rowno,f_index)==1 )return(-3);

	// �f�[�^�̒������擾
	len = PQgetlength(res,rowno,f_index);
	if( len < 1 ) return(-4);

	memcpy( buf,PQgetvalue(res, rowno,f_index),len>bufsize ? bufsize : len);
	return(0);
}

//--------------------------------------------------------------------------------------------------------------------
// char *PS_Col( PGresult* res, int rowno,char* fieldname)
// �����@:�s�ƃt�B�[���h�����w�肵�āA�t�B�[���h�f�[�^�̎擾(select * ��������)
// �����P:�@���ʃZ�b�g
// �����Q:�@�擾�������s�̔ԍ�
// �����R:�@�擾�������t�B�[���h��
//--------------------------------------------------------------------------------------------------------------------
char *PS_Col( PGresult* res, int rowno,char* fieldname)
{
	int f_index;

	// �t�B�[���h�̃C���f�b�N�X��T��
	if( (f_index=PQfnumber(res,fieldname))==-1) return("");

	// �s�͗L����?
	if( rowno >= PQntuples(res) ) return("");        /* �߂��Ă����s�� */

	// NULL?
	if( PQgetisnull(res,rowno,f_index)==1 )return("");

	return( PQgetvalue(res, rowno,f_index));
}

//--------------------------------------------------------------------------------------------------------------------
// char *PS_ColEx( PGresult* res, int rowno,int colno,char* fieldname,int bufsize)
// �����@:�s�Ɨ���w�肵�ăt�B�[���h�f�[�^�y�уt�B�[���h���̎擾
// �����P:�@���ʃZ�b�g
// �����Q:�@�擾�������s�̔ԍ�
// �����R:�@�擾��������̔ԍ�
// �����S:�@�t�B�[���h��(�o��)
// �����T:�@��L�o�b�t�@�T�C�Y
// �߂�l�F �擾����������
//--------------------------------------------------------------------------------------------------------------------
char *PS_ColEx( PGresult* res, int rowno,int colno,char* fieldname,int bufsize)
{
	int len;

	// ��͗L����?
	if( colno >= PQnfields(res) ) return("");        /* �߂��Ă����s�� */

	// �s�͗L����?
	if( rowno >= PQntuples(res) ) return("");        /* �߂��Ă����s�� */

	// �񖼃R�s�[
	len=strlen(PQfname( res, colno));

	memcpy( fieldname, PQfname( res, colno), len>bufsize ? bufsize : len);


	// NULL?
	if( PQgetisnull(res,rowno,colno)==1 )return("");

	return( PQgetvalue(res, rowno,colno));
}
//--------------------------------------------------------------------------------------------------------------------
//	int PS_Exec( PGconn* pg, char* sql)
// �����@:update or insert�����s
// �����P:�@�f�[�^�x�[�X�ڑ��ϐ�
// �����R:�@���s����insert or update��
// �߂�l: 0=����  ����ȊO�����s
//--------------------------------------------------------------------------------------------------------------------
int PS_Exec( PGconn* pg, char* sql)
{
	PGresult* res;

	res = PQexec(pg, sql);/* �₢���킹�̑��M */
	if( res == NULL )return(1);
	if (PQresultStatus(res) != PGRES_COMMAND_OK){
		PQclear(res);
		return(2);
	}


	// 1�����e�����󂯂Ȃ��������̓G���[�Ƃ���
	if ( atoi(PQcmdTuples(res)) <=0 ){
		PQclear(res);
		return(3);
	}
	PQclear(res);
	return(0);
}
