//=====================================================================================================================
//
//	FileName	:tpgtable.cpp
//	Class		:TPgTable
//				postgres�e�[�u��������N���X
//	�쐬��		:2002.05.07	Coded by coba (Oracle�ł̈ڐA)
//========================================================================================
//	�C��
//
//========================================================================================
#include "tpgtable.h"


//---------------------------------------------------------------------------------------------------------------------
//	TPgTable::TPgTable()
//	�^�C�v�FPublic
//	�@�\�@�F�f�t�H���g�R���X�g���N�^�B�����o�ϐ��̏�����
//---------------------------------------------------------------------------------------------------------------------
TPgTable::TPgTable()
{
	m_rowNo=0;
	m_rowMax=0;
	m_selOK=FALSE;
	m_pgDB = NULL;
	m_pgRes = NULL;
}
//---------------------------------------------------------------------------------------------------------------------
//	TPgTable::~TPgTable()
//	�^�C�v�FPublic
//	�@�\�@�F�f�X�g���N�^�B�I�u�W�F�N�g�⃁�����̉��
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
TPgTable::~TPgTable()
{
	this->Reset();
}

//---------------------------------------------------------------------------------------------------------------------
//	TPgTable::TPgTable()
//	�^�C�v�FPublic
//	�@�\�@�F�R���X�g���N�^�B�����o�ϐ��̏�����
//	�����P�FDB�ڑ��I�u�W�F�N�g(�ڑ��ς݂ł��邱��)
//---------------------------------------------------------------------------------------------------------------------
int TPgTable::SetDB(PGconn** pg)
{
	m_pgDB = pg;
	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TPgTable::ChkConnect(void)
//	�^�C�v�Fprotected
//	�@�\�@�FDB�ڑ��󋵂𒲂ׁA�؂�Ă�����Đڑ�����
//	�߂�l�F0=���� 
//			-100=�ڑ������� 
//			-101 �Đڑ��Ɏ��s
//---------------------------------------------------------------------------------------------------------------------
int TPgTable::ChkConnect(void)
{
	if(m_pgDB == NULL) return(-100);

	// DB�̐ڑ����m�F
	if( PS_IsConnect(*m_pgDB) ){
		if( PS_ReConnect(m_pgDB)) return(-101);
	}
	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TPgTable::Next(void)
//	�^�C�v�FPublic
//	�@�\�@�F�J�����g���R�[�h�̈ړ�
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TPgTable::MoveNext(void)
{

	if(m_pgDB == NULL) return(-100);

	if( m_selOK==FALSE || m_rowMax <=0)return(-1);

	if( ++m_rowNo >= m_rowMax){
		this->Reset();
		return(-1);
	}

	Attach();
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TPgTable::Next(void)
//	�^�C�v�FPublic
//	�@�\�@�F�J�����g���R�[�h�̈ړ�
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TPgTable::MovePrev(void)
{

	if(m_pgDB == NULL) return(-100);

	if( m_selOK==FALSE || m_rowMax <=0)return(-1);

	if( --m_rowNo <0 ){
		this->Reset();
		return(-1);
	}

	Attach();
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TPgTable::Next(void)
//	�^�C�v�FPublic
//	�@�\�@�F�J�����g���R�[�h�̈ړ�
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TPgTable::MoveFirst(void)
{

	if(m_pgDB == NULL) return(-100);

	if( m_selOK==FALSE || m_rowMax <=0)return(-1);

	m_rowNo=0;

	Attach();
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TPgTable::Next(void)
//	�^�C�v�FPublic
//	�@�\�@�F�J�����g���R�[�h�̈ړ�
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
int TPgTable::MoveLast(void)
{

	if(m_pgDB == NULL) return(-100);

	if( m_selOK==FALSE || m_rowMax <=0)return(-1);

	m_rowNo=m_rowMax-1;

	Attach();
	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TPgTable::Sel(char* sql)
//	�^�C�v�Fprotected
//	�@�\�@�Fselect���s
//	�����P�F���s����select��
//	�߂�l�F0=���� 
//			-100=�ڑ������� 
//			-101 �Đڑ��Ɏ��s
//---------------------------------------------------------------------------------------------------------------------
int TPgTable::Sel(char* sql)
{
	int ret;


	// ����
	this->Reset();

	// DB�̐ڑ����m�F
	ret=this->ChkConnect();
	if( ret ) return(ret);

	ret = PS_Select( *m_pgDB,&m_pgRes, sql);

	// �������sor�Y���Ȃ�
	if( ret <=0 ){
		this->Reset();
		//PS_Close( m_pgRes);
		return(-1);
	}

	m_rowMax = ret;
	m_selOK=TRUE;
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int TPgTable::Exec(char* sql)
//	�^�C�v�Fprotected
//	�@�\�@�Fsql�̎��s(insert,select delete)
//	�����P�F���s����sql��

//---------------------------------------------------------------------------------------------------------------------
int TPgTable::Exec(char* sql)
{
	int ret;

	// DB�̐ڑ����m�F
	ret=this->ChkConnect();
	if( ret ) return(ret);
	ret=PS_Exec(*m_pgDB,sql);
	return(ret);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TPgTable::Next(void)
//	�^�C�v�Fprivate
//	�@�\�@�Fselect�p�̃I�u�W�F�N�g�⃁�����̉��
//	�����P�F�Ȃ�
//---------------------------------------------------------------------------------------------------------------------
void TPgTable::Reset(void)
{
	if(m_pgRes !=NULL){
		PS_Close( m_pgRes);
		m_pgRes=NULL;
	}
	m_rowNo=0;
	m_rowMax=0;
	m_selOK=FALSE;
}

//---------------------------------------------------------------------------------------------------------------------
//	char* TPgTable::Columns(char* fldname)
//	�^�C�v�Fprotected
//	�@�\�@�F�t�B�[���h�f�[�^�̎擾
//---------------------------------------------------------------------------------------------------------------------
char* TPgTable::Columns(char* fldname)
{
	return(PS_Col( m_pgRes, m_rowNo,fldname));
}
//---------------------------------------------------------------------------------------------------------------------
//	void CMGenyakuSiyoMst::Attach(void)
//	�^�C�v�Fpublic
//	�@�\�@�F�J�����g�f�[�^�擾(�_�~�[)
//---------------------------------------------------------------------------------------------------------------------
void TPgTable::Attach(void)
{
}

