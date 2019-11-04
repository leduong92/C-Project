//=====================================================================================================================
//
//	FileName	:TPgBase.CPP
//	Class		:TPgBase
//				: postgresのDB基本操作クラス
//	作成日		:2002.05.07	Coded by Coba
//
//=====================================================================================================================
//	修正
//=====================================================================================================================
#include "tpgbase.h"

//---------------------------------------------------------------------------------------------------------------------
//	TPgBase::TPgBase()
//	タイプ：public
//	機能　：デフォルトコンストラクタ。メンバ変数の初期化
//	引数１：なし
//---------------------------------------------------------------------------------------------------------------------
TPgBase::TPgBase()
{
	m_pgConn = NULL;
}

//---------------------------------------------------------------------------------------------------------------------
//	TPgBase::~TPgBase()
//	タイプ：public
//	機能　：デストラクタ。オブジェクトやメモリの解放
//	引数１：なし
//---------------------------------------------------------------------------------------------------------------------
TPgBase::~TPgBase()
{
	if( m_pgConn !=NULL ) PS_DisConnect( m_pgConn);
}

//---------------------------------------------------------------------------------------------------------------------
// 2010.12.17 coba 切断関数
//	タイプ：public
//	機能　：データベース切断
//	引数１：なし
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
//	タイプ：public
//	機能　：データベースに接続
//	引数１：なし
//---------------------------------------------------------------------------------------------------------------------
int TPgBase::OpenConnect(char* host,char* db,char* user,char* pass ,int port/*=5432*/)
{
	if( NULL==host || NULL==db || NULL==user || NULL == pass ) return(-1);
	char con_str[256];

/*
	// DB接続文字列の作成:IPｱﾄﾞﾚｽ
	if(host[0] >='0' && host[0] <='9')
		sprintf( con_str,"hostaddr=%s dbname=%s user=%s password=%s",host,db,user,pass);
*/

	// 2010.09.03 matuda 引数にportを追加
	sprintf( con_str,"host=%s dbname=%s user=%s password=%s port=%ld",host,db,user,pass,port);

	// DBオープン

	if(PS_Connect(&m_pgConn, con_str)){
		return(-2);
	}

	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TPgBase::Commit(int mode)
//	タイプ：public
//	機能　：トランザクションのコミット
//	引数１：コミットモード
//			PGCMT_START(0),		開始
//			PGCMT_COMMIT(1),		終了
//			PGCMT_ROLLBACK(2),	ロールバック
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

	// DBの接続を確認
	ret=this->ChkConnect();
	if( ret ) return(ret);
	ret=PS_Exec(m_pgConn,buf);
	return(ret);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TPgTable::ChkConnect(void)
//	タイプ：protected
//	機能　：DB接続状況を調べ、切れていたら再接続する
//	戻り値：0=成功 
//			-100=接続が無効 
//			-101 再接続に失敗
//---------------------------------------------------------------------------------------------------------------------
int TPgBase::ChkConnect(void)
{
	if(m_pgConn == NULL) return(-100);

	// DBの接続を確認
	if( PS_IsConnect( m_pgConn) ){
		if( PS_ReConnect(&m_pgConn)) return(-101);
	}
	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
//	int TPgBase::SetEncoding(char* encoding)
//	タイプ：protected
//	機能　：クライアント側のエンコーディングの変更 2010.05.20 coba
//	戻り値：0=成功 
//			-100=接続が無効 
//---------------------------------------------------------------------------------------------------------------------
int TPgBase::SetEncoding(char* encoding)
{
	int ret;

	if(m_pgConn == NULL) return(-100);

	// DBの接続を確認
	ret=PS_Encoding( m_pgConn,encoding);
	return(ret);
}

//---------------------------------------------------------------------------------------------------------------------
// int TPgBase::GetSrvDateTime(SRV_DATETIME* dat)
//	タイプ：public
//	機能　：サーバ時刻の取得
//	引数：サーバ時刻構造体(出力)
/*
typedef struct
{
	short y;	// 年
	short m;	// 月
	short d;	// 日
	short h;	// 時
	short mi;	// 分
	short s;	// 秒
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

	// 問い合わせ
	strcpy( sql,"select to_char(current_timestamp,'yyyymmddhh24missd') as sdate" );
	ret = PS_Select( m_pgConn,&res, sql);
	if(ret<1){
		PS_Close( res );
		return(-1);
	}
	// 取得
	memset( buf,0,sizeof(buf));
	strcpy( buf,PS_Col( res, 0,"sdate"));
	// カーソル閉じる
	PS_Close( res );

	if(strlen(buf)!=15) return(-2);		// 時刻14+曜日1

	// 出力
	memcpy( dat->dt,buf,14);
	dat->dow=(short)(atoi(buf+14))-1;		// PostgreSQLは日曜日が1
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

