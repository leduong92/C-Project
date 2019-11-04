//=====================================================================================================================
//
//	FileName	:TPgBase.CPP
//	Class		:TPgBase
//=====================================================================================================================

//#include "stdafx.h"
#include "tpgbase.h"

TPgBase::TPgBase()
{
	m_pgConn = NULL;
}

TPgBase::~TPgBase()
{
	if( m_pgConn !=NULL ) PS_DisConnect( m_pgConn);
}

int TPgBase::OpenConnect(char* host,char* db,char* user,char* pass)
{
	if( NULL==host || NULL==db || NULL==user || NULL == pass ) return(-1);
	char con_str[256];

	if(host[0] >='0' && host[0] <='9')
		sprintf( con_str,"hostaddr=%s dbname=%s user=%s password=%s",host,db,user,pass);

		sprintf( con_str,"host=%s dbname=%s user=%s password=%s",host,db,user,pass);

		
	if(PS_Connect(&m_pgConn, con_str)){
		return(-2);
	}

	return(0);
}

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

	ret=this->ChkConnect();
	if( ret ) return(ret);
	ret=PS_Exec(m_pgConn,buf);
	return(ret);
}

int TPgBase::ChkConnect(void)
{
	if(m_pgConn == NULL) return(-100);

	if( PS_IsConnect( m_pgConn) ){
		if( PS_ReConnect(&m_pgConn)) return(-101);
	}
	return(0);
}

