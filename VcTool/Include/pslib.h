#ifndef PS_LIB_H
#define PS_LIB_H
//===================================================================================================================
//
//	FileName	: pslib.c
//				Postgres�p�̃��[�e�B���e�B
//	�쐬��		:2001.10.22 
//	�X�V��		:2002.04.18 coba ������Ƃ����ĂȂ���
//===================================================================================================================
// 2010.05.19 coba �N���C�A���g�̃G���R�[�f�B���O��ݒ肷��֐���ǉ�
// 2011.02.01 matsuda �t�B�[���h������t�B�[���h�ԍ����擾�ł��Ȃ��P�[�X�������������߃t�B�[���h�ԍ����ڎw��̊֐���ǉ�(PS_ColbyNo)
//===================================================================================================================
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libpq-fe.h" /* libpq �w�b�_�t�@�C�� */

// DB�ڑ�
int PS_Connect( PGconn** pg, char* constr);
void PS_DisConnect( PGconn* pg);

int PS_IsConnect(PGconn* pg);
int PS_ReConnect(PGconn** pg);

// select�̔��s����ь��ʂ̊J��
int PS_Select( PGconn* pg,PGresult** res, char* sql);
void PS_Close( PGresult* res);

// �������ʏ��̎擾
int PS_GetColCounts(PGresult* res);
int PS_GetColName( PGresult* res, int colno,char* fieldname,int bufsize);

// �������ʂ̃t�B�[���h�f�[�^�̎Q��
int PS_GetCol( PGresult* res, int rowno,int colno,char* buf,int bufsize);
int PS_GetByField( PGresult* res, int rowno,char* fieldname,char* buf,int bufsize);
char *PS_Col( PGresult* res, int rowno,char* fieldname);
char *PS_ColEx( PGresult* res, int rowno,int colno,char* fieldname,int bufsize);
char *PS_ColbyNo( PGresult* res, int rowno,int fieldno);		//2011.02.01 matsuda 

// SQL�̔��s
int PS_Exec( PGconn* pg, char* sql);

// 2010.05.19 coba �G���R�[�f�B���O�̐ݒ�
int PS_Encoding(PGconn* pg,char* encoding);

#ifdef __cplusplus
}
#endif

#endif
