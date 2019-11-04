//=======================================================================================================
//	可変長バイナリファイル操作クラス
//	Class Name TBinaryTable
//	File Name TBinaryTable.cpp
//	作成者：Coba
//	作成日：2002.02.01
//	修正日：
//=======================================================================================================
#include "TBinaryTable.h"

#pragma pack(push,1)

//---------------------------------------------------------------------------------------------------------------------
//	関数名：TBinaryTable::TBinaryTable(void)
//	メンバ：public
//	機能　：コンストラクタ
//---------------------------------------------------------------------------------------------------------------------
TBinaryTable::TBinaryTable(void)
{
	memset( m_fName, 0, sizeof(m_fName) );
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：TBinaryTable::TBinaryTable(void)
//	メンバ：public
//	機能　：デストラクタ
//---------------------------------------------------------------------------------------------------------------------
TBinaryTable::~TBinaryTable(void)
{
}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::Create(char* fname,BFSPEC *filespec)
//	メンバ：protected
//	機能　：DBファイルの作成。既にある場合は上書きされます
//	引数１：作成するファイル名
//	引数２：構造定義用の構造体”BFSPEC”
//	戻り値：0=成功、その他=引数エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Create(char* fname,BF_SPEC *filespec)
{
	BF_HEAD fHead;					// ファイルヘッダ

	if( NULL == fname || !strlen(fname) ) return(BTB_NAME_FAULT);

	// キー長のチェック
	if( filespec->keylen==0 || filespec->keylen > MAX_KEY_LENGTH) return BTB_KEYLEN_ERR;

	// データ長を超えてはいけない
	if( filespec->keylen+filespec->offset>MAX_RECORDLENGTH) return(BTB_RECLEN_ERR);

	FILE* fp;

	if( NULL==(fp=fopen(fname,"wb")) ) return(BTB_CANT_CREATE);

	time_t 	ltime;
	struct	tm	*nowtime;
    char dummy[20];

	time(&ltime);
	nowtime=localtime(&ltime);
	sprintf(dummy,"%04d%02d%02d", nowtime->tm_year+1900,nowtime->tm_mon+1,nowtime->tm_mday);

	// ヘッダー部を書き込む
	memset( &fHead, 0, sizeof(BF_HEAD));
	memcpy( fHead.code, BF_NOK_NAME,6);			// ファイルチェック用定義名
	memcpy( fHead.ver,BF_NOK_VERSION,5);			// バージョン
	memcpy( fHead.date,dummy,8);					// 作成日
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
//	メンバ：public
//	機能　：操作するファイル名をセットする
//	引数　：構造定義用の構造体”BFSPEC”
//	引数２：ファイル編集時のバッファーサイズ
//	戻り値：0=成功、その他=引数エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Open(char* fname)
{
	int ret;
	FILE* fp;
	BF_HEAD fHead;					// ファイルヘッダ

	if( NULL == fname || !strlen(fname) ) return(BTB_NAME_FAULT);

	// バッファやカーソルの初期化
	memset( m_fName, 0, sizeof(m_fName) );
	memcpy( m_fName, fname,strlen(fname) > MAX_PATH ? MAX_PATH:strlen(fname) );
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));

	// ファイルが存在するか？
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);


	// shared open
	fp = _fsopen( m_fName, "rb",_SH_DENYWR );

	if( fp == NULL ) return(BTB_LOCKED);

	// ファイルヘッダ読み出し
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );
	fclose(fp);

	// 準拠ファイルではない
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 )	|| fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		return(BTB_DIFF_FILE);
	}

	return(BTB_OK);
}


//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TBinaryTable::First(  unsigned short* reclen )
//	メンバ：protected
//	機能　：最初のレコードに移動し、読み込みに必要なデータ長を返す
//	引数　：レコードサイズ格納ポインタ（出力）
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::First(  unsigned short* reclen )
{
	return(this->Move(reclen,0));
}
//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TBinaryTable::Last(  unsigned short* reclen )
//	メンバ：protected
//	機能　：最終のレコードに移動し、読み込みに必要なデータ長を返す
//	引数　：レコードサイズ格納ポインタ（出力）
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Last(  unsigned short* reclen )
{
	return(this->Move(reclen,1));
}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TBinaryTable::Next(  unsigned short* reclen )
//	メンバ：protected
//	機能　：(前のoperationに対する)次のレコード情報を取得し、必要なデータ長を返す
//	引数　：レコードサイズ格納ポインタ（出力）
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Next(  unsigned short* reclen )
{
	return(this->Move(reclen,2));

}
//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TBinaryTable::Prev(  unsigned short* reclen )
//	メンバ：protected
//	機能　：(前のoperationに対する)次のレコード情報を取得し、必要なデータ長を返す
//	引数　：レコードサイズ格納ポインタ（出力）
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Prev(  unsigned short* reclen )
{
	return(this->Move(reclen,3));

}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int Move(unsigned short reclen,int operation)
//	メンバ：private
//	機能　：指定された方向にカレントポジションを移動し、そのレコードのレコード長を返す
//	引数　：レコードサイズ格納ポインタ（出力）
//	引数２：進行方向 0=最初のレコード、1=最後のレコード、2=次のレコード,3=前のレコード
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Move(unsigned short *reclen,int operation)
{
	int ret;
	long move;

	BF_HEAD fHead;					// ファイルヘッダ

	// バッファ初期化
	memset( &fHead, 0, sizeof(BF_HEAD));
	*reclen = 0;

	// 引数エラー
	if( operation < 0 || operation >3 )return(BTB_OPE_ERR);

	// Next()の時、次のレコードがあるのか？
	else if( operation == 2 ){	
		if( m_Pblock.rec_pos==0 )return(BTB_POS_ERR);
		if(m_Pblock.next_pos ==0 ) return(BTB_EOF);
	}
	else if( operation == 3 ){
		if( m_Pblock.rec_pos==0 )return(BTB_POS_ERR);
		if(m_Pblock.prev_pos ==0 ) return(BTB_EOF);
	}

	// ファイルが存在するか？
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	FILE* fp;

	// shared open
	fp = _fsopen( m_fName, "rb",_SH_DENYWR );

	if( fp == NULL ) return(BTB_LOCKED);

	// ファイルヘッダ読み出し
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );
	// 準拠ファイルではない
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 )	|| fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// レコードが存在しない
	if( fHead.first_pos < sizeof(BF_HEAD) ){
		fclose(fp);
		return(BTB_EOF);
	}

	// オペレーション別に移動量を決める
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

	// 対象のレコードに移動し、ポジションブロックを読み出す
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
//	関数名：int TBinaryTable::SearchFile(char* fname)
//	メンバ：private
//	機能　：指定されたファイルの検索
//	引数１：検索ファイル名
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::SearchFile(char* fname)
{

	// ファイルのチェック
	if( NULL == m_fName || !strlen(m_fName) ) return(BTB_NAME_FAULT);

	struct _finddata_t c_file;
	long hFile;

	if( (hFile = _findfirst( fname, &c_file )) == -1L ) return(BTB_NOFILE);
	_findclose( hFile );

	return(BTB_OK);

}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::Find( void *key ,unsigned short* reclen)
//	メンバ：protected
//	機能　：検索キーに合致するレコードを検索
//	引数１：検索キー
//	引数２：（見つかった場合）そのレコードの長さ
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Find( void *key ,unsigned short* reclen)
{
	return(this->SearchKey(key,0,reclen));
}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::GE( void *key ,unsigned short* reclen)
//	メンバ：protected
//	機能　：検索キーに合致するかそれ以上のレコードを検索
//	引数１：検索キー
//	引数２：（見つかった場合）そのレコードの長さ
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::GE( void *key ,unsigned short* reclen)
{
	return(this->SearchKey(key,1,reclen));
}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::GT( void *key ,unsigned short* reclen)
//	メンバ：protected
//	機能　：検索キーにより大きいレコードを検索
//	引数１：検索キー
//	引数２：（見つかった場合）そのレコードの長さ
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::GT( void *key ,unsigned short* reclen)
{
	return(this->SearchKey(key,3,reclen));
}
//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::GE( void *key ,unsigned short* reclen)
//	メンバ：protected
//	機能　：検索キーに合致するかそれ以下のレコードを検索
//	引数１：検索キー
//	引数２：（見つかった場合）そのレコードの長さ
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::LE( void *key ,unsigned short* reclen)
{
	return(this->SearchKey(key,2,reclen));
}

//---------------------------------------------------------------------------------------------------------------------
//	int TBinaryTable::GT( void *key ,unsigned short* reclen)
//	メンバ：protected
//	機能　：検索キーにより小さいレコードを検索
//	引数１：検索キー
//	引数２：（見つかった場合）そのレコードの長さ
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::LT( void *key ,unsigned short* reclen)
{
	return(this->SearchKey(key,4,reclen));
}

//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TBinaryTable::SearchKey( void *key ,int operation, unsigned short* reclen);
//	メンバ：private
//	機能　：レコード検索/キーに合致するレコードを検索
//	引数１：検索キー
//	引数２：比較方法 0=Equal 1=GE,2=LE 3=GT,4=LT
//	引数３：（見つかった場合）そのレコードの長さ
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::SearchKey( void *key ,int operation, unsigned short* reclen)
{

	BF_HEAD fHead;					// ファイルヘッダ

	// バッファ初期化
	memset( &fHead, 0, sizeof(BF_HEAD));
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
	*reclen = 0;

	// 引数エラー
	if( key==NULL )return(BTB_OPE_ERR);

	int ret;
	long move;
	FILE* fp;
	char keybuf[MAX_KEY_LENGTH];

	// ファイルが存在するか？
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	// shared open
	fp = _fsopen( m_fName, "rb",_SH_DENYWR );

	if( fp == NULL ) return(BTB_LOCKED);

	// ファイルヘッダ読み出し
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );

	// 準拠ファイルではない
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 )	|| fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// レコードが存在しない
	if( fHead.first_pos < sizeof(BF_HEAD) ){
		fclose(fp);
		return(BTB_EOF);
	}

	move = fHead.first_pos;

	do{
		// 移動	
		ret = fseek( fp, move,SEEK_SET);
		if( ret ){
			ret= BTB_IO_ERR;
			break;
		}

		// ポジションブロック読み込み
		ret = (int)fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp );
		if( ret!=1 ){
			ret= BTB_IO_ERR;
			break;
		}

		// キー位置へ移動
		ret = fseek( fp, fHead.offset, SEEK_CUR);
		if( ret ){
			ret= BTB_IO_ERR;
			break;
		}

		// キー読み込み
		ret = (int)fread( keybuf, fHead.keylen,1,fp );
		if( ret!=1 ){
			ret= BTB_IO_ERR;
			break;
		}

		// キー比較
		ret = memcmp(keybuf, key, fHead.keylen);

		// LTオペレーション。この場合、現在の前のレコードがビンゴ
		if( ret >=0 && operation == 4 ){
			if( !m_Pblock.prev_pos){	// レコード無し
				ret= BTB_NOTFIND;
				break;
			}
			// 移動	
			if( fseek( fp, m_Pblock.prev_pos,SEEK_SET)){
				ret= BTB_IO_ERR;
				break;
			}
			// ポジションブロック読み込み
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1){
				ret= BTB_IO_ERR;
				break;
			}
			ret= BTB_OK;
			*reclen=m_Pblock.rec_len;
			break;
		}

		// キー発見		Equal系オペレーション
		else if( !ret && operation <3){
			ret= BTB_OK;
			*reclen=m_Pblock.rec_len;
			break;
		}

		// キー > 検索キー
		else if( ret > 0 ){
			// Equalの場合、キーが見つからない
			if( operation == 0){
				ret= BTB_NOTFIND;
				break;
			}
			// Greater系オペレーション。keybuf>keyなので、これがビンゴ
			else if( operation == 1 || operation ==3 ){
				ret= BTB_OK;
				*reclen=m_Pblock.rec_len;
				break;
			}
		}

		// 次のレコードへ
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
//	メンバ：protected
//	機能　：ｶﾚﾝﾄレコードのデータを読み出す
//	引数１：取得バッファ
//	引数２：取得バッファサイズ
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::ReadRecord(void* buf, unsigned short bufsize)
{
	// 引数エラー
	if( buf==NULL )return(BTB_OPE_ERR);

	int ret;
	BF_HEAD fHead;					// ファイルヘッダ
	BF_POSBLOCK pb;

	// バッファ初期化
	memset( &fHead, 0, sizeof(BF_HEAD));

	// ファイルが存在するか？
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	FILE* fp;

	// shared open
	fp = _fsopen( m_fName, "rb",_SH_DENYWR );

	if( fp == NULL ) return(BTB_LOCKED);

	// ファイルヘッダ読み出し
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );
	// 準拠ファイルではない
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 )	|| fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// レコードが存在しない
	if( fHead.first_pos < sizeof(BF_HEAD) ){
		fclose(fp);
		return(BTB_EOF);
	}

	// カレントブロックが不正
	if( m_Pblock.rec_pos==0){
		fclose(fp);
		return(BTB_POS_ERR);
	}

	// 対象のレコードに移動し、ポジションブロックを読み出す
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

	// 削除されたレコードの場合
	if( pb.del_sign){
		fclose(fp);
		memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
		return(BTB_POS_ERR);
	}

	// データバッファ長不足
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
//	関数名：int TBinaryTable::Add( void* data, unsigned short reclen)
//	メンバ：protected
//	機能　：新規にデータを追加する
//	引数１：挿入データ
//	引数１：挿入データ長
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Add( void* data, unsigned short reclen)
{
	int ret;
	long move;
	unsigned long wr_pos;
	FILE* fp;
	char keybuf[MAX_KEY_LENGTH];
	BF_POSBLOCK pb;
	BF_HEAD fHead;					// ファイルヘッダ

	// バッファ初期化
	memset( &fHead, 0, sizeof(BF_HEAD));
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
	memset( &pb, 0, sizeof(BF_POSBLOCK));

	// 引数エラー
	if( data==NULL )return(BTB_OPE_ERR);

	// ファイルが存在するか？
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	// open
	fp = fopen( m_fName, "r+b" );
	if( fp == NULL ) return(BTB_LOCKED);

	// ファイルヘッダ読み出し
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );

	// 準拠ファイルではない
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 )	|| fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// レコード長のチェック
	if( reclen < fHead.keylen+fHead.offset){
		fclose(fp);
		return(BTB_RECLEN_ERR);
	}

	// 書き込み位置（EOF）の検出
	ret = fseek( fp, 0,SEEK_END);
	wr_pos=(unsigned long)ftell(fp);

	// レコードある場合は、挿入位置を探す
	if( fHead.first_pos >0){
		move = fHead.first_pos;
		ret = BTB_IO_ERR;
		do{
			// 移動	
			if( 0 !=(ret = fseek( fp, move,SEEK_SET)) ){;
				ret= BTB_IO_ERR;break;
			}

			// ポジションブロック読み込み
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
				ret= BTB_IO_ERR;
				break;
			}

			// キー位置へ移動
			if( fseek( fp, fHead.offset, SEEK_CUR) ){
				ret= BTB_IO_ERR;
				break;
			}

			// キー読み込み
			if( fread( keybuf, fHead.keylen,1,fp ) !=1 ){
				ret= BTB_IO_ERR;
				break;
			}

			// キー比較
			ret = memcmp(keybuf, (char*)data+fHead.offset, fHead.keylen);
			// 同じレコードがあったらエラー
			if( !ret){
				ret= BTB_DUP;
				break;
			}
			// 自分より大きなレコードがあったらその前に挿入	 キー > 挿入キー
			else if( ret > 0 ){
				pb.next_pos = m_Pblock.rec_pos;
				pb.prev_pos = m_Pblock.prev_pos;
				ret= BTB_OK;
				break;
			}

			// 次のレコードへ
			move = m_Pblock.next_pos;
			// 最後までいったら、自分が最終レコードになる
			if(move < sizeof(BF_HEAD) ){
				pb.next_pos = 0;
				pb.prev_pos = m_Pblock.rec_pos;
				ret= BTB_OK;
				break;
			}

		}while(1);

		// エラー
		if( ret !=BTB_OK){
			fclose(fp);
			return(ret);
		}
	}

	// 自分が最初のレコードになる場合
	if( pb.prev_pos == 0 ){
		fHead.first_pos=wr_pos;
	}
	// 挿入する前のレコードを編集
	else{
		// 移動	
		if( fseek( fp, pb.prev_pos,SEEK_SET) ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// ポジションブロック読み込み
		if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// データ書き換え
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

	// 自分が最後のレコードになる場合
	if( pb.next_pos == 0 ){
		fHead.last_pos=wr_pos;
	}
	else{
		// 移動	
		if(fseek( fp, pb.next_pos,SEEK_SET)){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// ポジションブロック読み込み
		if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// データ書き換え
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

	// ヘッダ更新
	ret = fseek( fp, 0,SEEK_SET);
	if( fwrite(&fHead, sizeof(BF_HEAD),1,fp)!=1){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	// ポジションブロック書き込み
	pb.rec_pos = wr_pos;
	pb.rec_len = reclen;
	ret = fseek( fp, 0,SEEK_END);
	if( fwrite(&pb, sizeof(BF_POSBLOCK),1,fp)!=1){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	// データ書き込み
	if( fwrite(data, reclen,1,fp)!=1){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	// 挿入したレコードをｶﾚﾝﾄにする
	fclose(fp);
	memcpy( &m_Pblock, &pb, sizeof(BF_POSBLOCK));
	return(BTB_OK);
}



//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TBinaryTable::Upd( char* data, unsigned short reclen )
//	メンバ：protected
//	機能　：
//	引数１：更新するデータ
//	引数２：更新するデータの長さ
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Upd( void* data, unsigned short reclen )
{
	int ret;
	long move;
	FILE* fp;
	char keybuf[MAX_KEY_LENGTH];
	BF_POSBLOCK pb;
	BF_HEAD fHead;					// ファイルヘッダ
	unsigned long wr_pos;

	// バッファ初期化
	memset( &fHead, 0, sizeof(BF_HEAD));
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
	memset( &pb, 0, sizeof(BF_POSBLOCK));

	// 引数エラー
	if( data==NULL )return(BTB_OPE_ERR);

	// ファイルが存在するか？
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	// open
	fp = fopen( m_fName, "r+b" );
	if( fp == NULL ) return(BTB_LOCKED);

	// ファイルヘッダ読み出し
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );

	// 準拠ファイルではない
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 ) || fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// レコード無し
	if( fHead.first_pos ==0){
		fclose(fp);
		return(BTB_EOF);
	}

	// レコード長のチェック
	if( reclen < fHead.keylen+fHead.offset){
		fclose(fp);
		return(BTB_RECLEN_ERR);
	}

	// レコードを検索する
	move = fHead.first_pos;
	ret = BTB_IO_ERR;
	do{
		// 移動	
		if( 0 !=(ret = fseek( fp, move,SEEK_SET)) ){;
			ret= BTB_IO_ERR;break;
		}

		// ポジションブロック読み込み
		if( fread( &pb, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			ret= BTB_IO_ERR;break;
		}

		// キー位置へ移動
		if( fseek( fp, fHead.offset, SEEK_CUR) ){
			ret= BTB_IO_ERR;break;
		}

		// キー読み込み
		if( fread( keybuf, fHead.keylen,1,fp ) !=1 ){
			ret= BTB_IO_ERR;break;
		}

		// キー比較
		ret = memcmp(keybuf, (char*)data+fHead.offset, fHead.keylen);
		// キー発見
		if( !ret){
			ret= BTB_OK;
			break;
		}
		// キーが見つからない
		else if( ret > 0 ){
			ret= BTB_NOTFIND;break;
		}

		// 次のレコードへ
		move = pb.next_pos;
		// 最後までいったら、自分が最終レコードになる
		if(move < sizeof(BF_HEAD) ){
			ret= BTB_NOTFIND;break;
		}
	}while(1);

	// エラー
	if( ret !=BTB_OK){
		fclose(fp);
		return(ret);
	}

	// レコード長に変化が無い場合はそのまま
	if( reclen == pb.rec_len ){
		if( fseek( fp, pb.rec_pos+sizeof(BF_POSBLOCK),SEEK_SET) ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// データ書き込み
		if( fwrite((char*)data, reclen,1,fp)!=1){
			fclose(fp);
			return(BTB_IO_ERR);
		}
	}
	// レコード長に変化があった場合は、一度削除して追加する
	else{
		// 書き込み位置（EOF）の検出
		ret = fseek( fp, 0,SEEK_END);
		wr_pos=(unsigned long)ftell(fp);

		// prevレコードの編集
		if( pb.prev_pos ){
			if( fseek( fp, pb.prev_pos,SEEK_SET) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// ポジションブロック読み込み
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// データ書き換え
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
		// 自分が先頭レコードである場合
		else{
			// ヘッダ更新
			fHead.first_pos=wr_pos;
			ret = fseek( fp, 0,SEEK_SET);
			if( fwrite(&fHead, sizeof(BF_HEAD),1,fp)!=1){
				fclose(fp);
				return(BTB_IO_ERR);
			}
		}

		// nextレコードの編集
		if( pb.next_pos ){
			if( fseek( fp, pb.next_pos,SEEK_SET) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// ポジションブロック読み込み
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// データ書き換え
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
		// 自分が最終レコードである場合
		else{
			// ヘッダ更新
			fHead.last_pos=wr_pos;
			ret = fseek( fp, 0,SEEK_SET);
			if( fwrite(&fHead, sizeof(BF_HEAD),1,fp)!=1){
				fclose(fp);
				return(BTB_IO_ERR);
			}
		}

		// 既存レコードの削除
		ret = fseek( fp, pb.rec_pos,SEEK_SET);
		pb.del_sign=1;
		if( fwrite(&pb, sizeof(BF_POSBLOCK),1,fp)!=1){
			fclose(fp);
			return(BTB_IO_ERR);
		}

		// レコード追加
		pb.rec_pos=wr_pos;
		pb.del_sign=0;
		pb.rec_len=reclen;
		ret = fseek( fp, 0,SEEK_END);
		if( fwrite(&pb, sizeof(BF_POSBLOCK),1,fp)!=1){	// ポジションブロック書き込み
			fclose(fp);
			return(BTB_IO_ERR);
		}
		if( fwrite((char*)data, reclen,1,fp)!=1){	// データ書き込み
			fclose(fp);
			return(BTB_IO_ERR);
		}
	}

	fclose(fp);
	memcpy( &m_Pblock, &pb, sizeof(BF_POSBLOCK));
	return(BTB_OK);
}


//---------------------------------------------------------------------------------------------------------------------
//	関数名：int TBinaryTable::Del( void *key )
//	メンバ：protected
//	機能　：キーに合致するレコードを削除する
//	引数　：削除するキー
//	戻り値：0=成功、その他=エラー
//---------------------------------------------------------------------------------------------------------------------
int TBinaryTable::Del( void *key )
{
	int ret;
	long move;
	FILE* fp;
	char keybuf[MAX_KEY_LENGTH];
	BF_POSBLOCK pb;
	BF_HEAD fHead;					// ファイルヘッダ

	// バッファ初期化
	memset( &fHead, 0, sizeof(BF_HEAD));
	memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));
	memset( &pb, 0, sizeof(BF_POSBLOCK));

	// 引数エラー
	if( key==NULL )return(BTB_OPE_ERR);


	// ファイルが存在するか？
	ret= this->SearchFile(m_fName);
	if(ret ) return(ret);

	// open
	fp = fopen( m_fName, "r+b" );

	if( fp == NULL ) return(BTB_LOCKED);

	// ファイルヘッダ読み出し
	ret = (int)fread( &fHead, sizeof(BF_HEAD),1,fp );

	// 準拠ファイルではない
	if( ret !=1 || memcmp( fHead.code,BF_NOK_NAME,6 ) || fHead.crlf[0]!=0x0d || fHead.crlf[1]!=0x0a ){
		fclose(fp);
		return(BTB_DIFF_FILE);
	}

	// レコード無し
	if( fHead.first_pos ==0){
		fclose(fp);
		return(BTB_EOF);
	}

	// レコードを検索する
	move = fHead.first_pos;
	ret = BTB_IO_ERR;
	do{
		// 移動	
		if( 0 !=(ret = fseek( fp, move,SEEK_SET)) ){;
			ret= BTB_IO_ERR;break;
		}

		// ポジションブロック読み込み
		if( fread( &pb, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			ret= BTB_IO_ERR;break;
		}

		// キー位置へ移動
		if( fseek( fp, fHead.offset, SEEK_CUR) ){
			ret= BTB_IO_ERR;break;
		}

		// キー読み込み
		if( fread( keybuf, fHead.keylen,1,fp ) !=1 ){
			ret= BTB_IO_ERR;break;
		}

		// キー比較
		ret = memcmp(keybuf, key, fHead.keylen);
		// キー発見
		if( !ret){
			ret= BTB_OK;
			break;
		}
		// キーが見つからない
		else if( ret > 0 ){
			ret= BTB_NOTFIND;break;
		}

		// 次のレコードへ
		move = pb.next_pos;
		// 最後までいったら、自分が最終レコードになる
		if(move < sizeof(BF_HEAD) ){
			ret= BTB_NOTFIND;break;
		}
	}while(1);

	// エラー
	if( ret !=BTB_OK){
		fclose(fp);
		return(ret);
	}

	// 自分が先頭レコードの場合は、次レコードを先頭にする
	if( pb.prev_pos == 0 ){	
		fHead.first_pos=pb.next_pos;
		if( pb.next_pos){
			// 次のレコードを検索し、それが先頭となる
			if( fseek( fp, pb.next_pos,SEEK_SET) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// ポジションブロック読み込み
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// データ書き換え(先頭レコード扱い)
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
		// データが０件になる
		else{
			fHead.last_pos=0;
			memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));	// ポジションブロックを無効にする
		}
	}
	// 削除対象の、前(prev)レコードを編集
	else{
		// 移動	
		if( fseek( fp, pb.prev_pos,SEEK_SET) ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// ポジションブロック読み込み
		if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// データ書き換え
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

	// 自分が最後のレコードの場合は、前レコードを最終に割り当てる
	if( pb.next_pos == 0 ){
		fHead.last_pos=pb.prev_pos;
		if( pb.prev_pos){
			// 次のレコードを検索し、それが先頭となる
			if( fseek( fp, pb.prev_pos,SEEK_SET) ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// ポジションブロック読み込み
			if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
				fclose(fp);
				return(BTB_IO_ERR);
			}
			// データ書き換え(最終レコード扱い)
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
		// データが０件になる
		else{
			fHead.first_pos=0;
			memset( &m_Pblock, 0, sizeof(BF_POSBLOCK));	// ポジションブロックを無効にする
		}
	}
	// それ以外は、nextレコードの編集
	else{
		// 移動	
		if(fseek( fp, pb.next_pos,SEEK_SET)){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// ポジションブロック読み込み
		if( fread( &m_Pblock, sizeof(BF_POSBLOCK),1,fp ) !=1 ){
			fclose(fp);
			return(BTB_IO_ERR);
		}
		// データ書き換え
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

	// ヘッダ更新
	ret = fseek( fp, 0,SEEK_SET);
	if( fwrite(&fHead, sizeof(BF_HEAD),1,fp)!=1){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	// ポジションブロック書き込み
	pb.del_sign = 1;				// 削除
	fseek( fp, pb.rec_pos,SEEK_SET);
	if( fwrite(&pb, sizeof(BF_POSBLOCK),1,fp)!=1){
		fclose(fp);
		return(BTB_IO_ERR);
	}

	fclose(fp);
	return(BTB_OK);
}
#pragma pack(pop)

