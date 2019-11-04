//========================================================================================
//
//	FileName	:GridData.cpp
//	Class		:TListaData
// 				:グリッドウインドウ・データクラス
//	作成日		:1999.07.30	Coded by coba
//	修正		:1999.09.06 coba	行数を無制限に（今までは最大５０）
//									行ヘッダ機能追加
//									横スクロール機能追加
//========================================================================================
#include "griddata.h"

//---------------------------------------------------------------------------------------------------------------------
//	TGridData::TGridData(long totalcol,BOOL title=FALSE)
//	タイプ：private
//	機能　：コンストラクタ。各種初期化
//	引数１：行のブロックサイズ（この単位でメモリをアロケートする）
//	引数２：列数の初期値
//	引数３：
//---------------------------------------------------------------------------------------------------------------------
TGridData::TGridData(long rowBlock,long totalcol, BOOL title)
{
	if( totalcol > COL_LIMIT )
		m_TotalCols=COL_LIMIT;
	else
		m_TotalCols=totalcol;

	memset(m_Title, 0, sizeof(m_Title));

	// 行の合計
	m_LastRowNo=-1;

	// ヘッダのあるなし
	m_RowTitleEnable=title;

	m_AllocBlock = rowBlock;				// アロケートする単位
	if( m_AllocBlock < 1 ) m_AllocBlock = 20;


	// 列objectを１ブロックアロケートする
	LISTSTRDATA *tmp=NULL;
	m_StrData = NULL;

	// allocate
	tmp = (LISTSTRDATA*)realloc(m_StrData, sizeof(LISTSTRDATA) * m_AllocBlock);

	if(NULL==tmp) return;
	m_StrData = tmp;
	m_AllocRows = m_AllocBlock;				// アロケート済み行数

	// コピーバッファ空
	m_CopyBufFull =FALSE;
}
//---------------------------------------------------------------------------------------------------------------------
//	TGridData::TGridData(long totalcol)
//	タイプ：private
//	機能　：デストラクタ。メモリ解放
//---------------------------------------------------------------------------------------------------------------------
TGridData::~TGridData()
{
	if( m_StrData != NULL)
		free(m_StrData);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::SetTitle(int col,char* title)
//	タイプ：protected
//	機能　：列ヘッダのデータ設定
//	引数１：設定する列番号
//	引数２：列の題
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SetTitle(int col,char* title)
{
	if( col >= TGridData::m_TotalCols || col < 0) return(FALSE);

	memset( m_Title[col], 0, TITLELEN_MAX+1 );
	memcpy( m_Title[col],  title, strlen(title)>TITLELEN_MAX ? TITLELEN_MAX:strlen(title) );
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::SetCel(int col,int row, long cTag,char* data)
//	タイプ：protected
//	機能　：セルにデータを設定
//	引数１：設定する列番号
//	引数２：設定する行番号
//	引数３：設定データ(色）
//	引数３：設定データ(long値)
//	引数４：設定データ(文字)
//	引数５：セルの背景色 0=白、1=無効色
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SetCelData(int col,int row, char color,long cTag,char* data,char bkcolor)
{
	if( col >= m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);

	m_StrData[row].celTag[col] = cTag;
	m_StrData[row].color[col] = color;
	m_StrData[row].bkcolor[col] = bkcolor;
	memset( m_StrData[row].d[col],0, STRLEN_MAX+1);
	memcpy( m_StrData[row].d[col],data, strlen(data)>STRLEN_MAX ? STRLEN_MAX:strlen(data));

	return(TRUE);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::SetCelTag(int col,int row, long data)
//	タイプ：public
//	機能　：セルにデータを設定
//	引数１：設定する列番号
//	引数２：設定する行番号
//	引数３：設定データ
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SetCelTag(int col,int row, long data)
{
	if( col >= m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);

	m_StrData[row].celTag[col] = data;

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::SetCelBkColor(int col,int row, int color)
//	タイプ：public
//	機能　：セルに背景色のセット
//	引数１：設定する列番号
//	引数２：設定する行番号
//	引数３：０＝白　１＝灰色
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SetCelBkColor(int col,int row, char color)
{
	if( col >= m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);

	m_StrData[row].bkcolor[col] = color;

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::SearchCelTag(long* col,long* row,long searchVal,long satrtcol,long startrow,long direction)
//	タイプ：public
//	機能　：セルタグの検索
//	引数１：見つかった列番号
//	引数２：見つかった行番号
//	引数３：検索データ
//	引数４：検索開始列
//	引数５：検索開始行
//	引数６：検索方向 0=横（最後のセルまで検索）、1=縦(指定列内のみ検索)
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SearchCelTag(long* col,long* row,long searchVal,long startcol,long startrow,long direction)
{
	// 引数エラー
	if( startcol >= m_TotalCols || startcol < 0) return(FALSE);
	if( startrow > m_LastRowNo || startrow < 0) return(FALSE);

	int i,j,k;

	// 横方向検索
	if( !direction ){
		for( i= startrow; i <= m_LastRowNo;i++){		// 次の行へ
			// 一巡目はstartcolの次から
			if( i==startrow) k = startcol+1;
			else k=0;

			// 舐め舐め検索
			for( j=k; j< m_TotalCols;j++){
				// 発見
				if(m_StrData[i].celTag[j] == searchVal){
					*col = j;*row=i;
					return(TRUE);
				}
			}
		}
		// 未発見
		return(FALSE);
	}

	// 縦方向検索
	else{
		// 舐め舐め検索
		for( j=startrow+1; j<= m_LastRowNo;j++){
			// 発見
			if(m_StrData[j].celTag[startcol] == searchVal){
				*col = startcol;*row=j;
				return(TRUE);
			}
		}
		return(FALSE);
	}

	return(FALSE);

}

//---------------------------------------------------------------------------------------------------------------------
//	long TGridData::AddRow(long uTag)
//	タイプ：protected
//	機能　：新規に行を追加
//	引数１：行につけるタグ(省略可)
//	戻り値：成功＝追加した行の番号、－１以下＝失敗
//---------------------------------------------------------------------------------------------------------------------
long TGridData::AddRow(long uTag)
{

	// アロケート済み行数で足りないときは確保
	if( m_LastRowNo+1 >= m_AllocRows ){
		LISTSTRDATA *tmp=NULL;
		// allocate
		m_AllocRows += m_AllocBlock;
		tmp = (LISTSTRDATA*)realloc(m_StrData, sizeof(LISTSTRDATA) * m_AllocRows);

		if(NULL==tmp) return(-1);

		m_StrData = tmp;
	}

	m_LastRowNo++;
	memset( &m_StrData[m_LastRowNo], 0, sizeof(LISTSTRDATA) );
	m_StrData[m_LastRowNo].Tag = uTag;
	return(m_LastRowNo);
}

//---------------------------------------------------------------------------------------------------------------------
//	long TGridData::AddRow(char *colTitle,long uTag)
//	タイプ：protected
//	機能　：新規に行を追加
//	引数１：行につけるタグ(省略可)
//	戻り値：成功＝追加した行の番号、－１以下＝失敗
//---------------------------------------------------------------------------------------------------------------------
long TGridData::AddRow(char *colTitle,long uTag)
{
	// アロケート済み行数で足りないときは確保
	if( m_LastRowNo+1 >= m_AllocRows ){
		LISTSTRDATA *tmp=NULL;
		// allocate
		m_AllocRows += m_AllocBlock;
		tmp = (LISTSTRDATA*)realloc(m_StrData, sizeof(LISTSTRDATA) * m_AllocRows);

		if(NULL==tmp) return(-1);

		m_StrData = tmp;
	}

	m_LastRowNo++;
	memset( &m_StrData[m_LastRowNo], 0, sizeof(LISTSTRDATA) );
	m_StrData[m_LastRowNo].Tag = uTag;
	memset(m_StrData[m_LastRowNo].colTitle,0,TITLELEN_MAX+1);
	memcpy(m_StrData[m_LastRowNo].colTitle, colTitle, strlen(colTitle)>TITLELEN_MAX ? TITLELEN_MAX : strlen(colTitle));
	return(m_LastRowNo);
}


//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::ChangeRowTitle(long rowNo, char* newtitle)
//	タイプ：protected
//	機能　：行題の変更
//	引数１：変更する行
//	引数２：変更後の行題
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::ChangeRowTitle(long rowNo, char* newtitle)
{
	if( rowNo > m_LastRowNo || rowNo < 0) return(FALSE);

	if( newtitle==NULL) return(FALSE);

	memset(m_StrData[rowNo].colTitle,0,TITLELEN_MAX+1);
	memcpy(m_StrData[rowNo].colTitle, newtitle, strlen(newtitle)>TITLELEN_MAX ? TITLELEN_MAX : strlen(newtitle));

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::GetCel(char* buf, int bufsize,int col,int row )
//	タイプ：public
//	機能　：セルからデータを取得
//	引数１：取得バッファ
//	引数２：取得バッファサイズ
//	引数３：取得する列番号
//	引数４：取得する行番号
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::GetCel(char* buf, unsigned int bufsize,int col,int row )
{
	if( col > m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);

	memcpy( buf,m_StrData[row].d[col], 
			strlen(m_StrData[row].d[col]) > bufsize ? bufsize : strlen(m_StrData[row].d[col])+1);
	return(TRUE);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::GetCel(char* buf, int bufsize,int col,int row )
//	タイプ：public
//	機能　：セルからデータを取得してデータポインタを返す
//	引数３：取得する列番号
//	引数４：取得する行番号
//	戻り値：成功＝データポインタ 失敗=NULL
//---------------------------------------------------------------------------------------------------------------------
char* TGridData::GetCelEx(int col,int row )
{
	memset( m_retBuf, 0, sizeof(m_retBuf));

	if( col > m_TotalCols || col < 0) return(m_retBuf);
	if( row > m_LastRowNo || row < 0) return(m_retBuf);

	int i,len;

	memcpy( m_retBuf,m_StrData[row].d[col], 
			strlen(m_StrData[row].d[col]) > sizeof(m_retBuf)-1 ? sizeof(m_retBuf)-1 : strlen(m_StrData[row].d[col])+1);

	len = strlen(m_retBuf);

	for(i=len;i>0;i--){
		if( m_retBuf[i-1]!=0x20) break;
		if( m_retBuf[i-1]==0x20) m_retBuf[i-1] = 0;
	}

	return(m_retBuf);
}
long TGridData::GetActiveRow(void)
{
	return(m_LastRowNo);
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::GetCel(char* buf, int bufsize,int col,int row )
//	タイプ：protected
//	機能　：指定列のデータを全てバッファにコピーする
//	引数１：コピー対象の列番号
//	引数２：削除する場合=TRUE
//	戻り値：成功＝TRUE、失敗＝FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::ColCopy( long targetCol ,BOOL cut)
{
	// 引数チェック
	if( targetCol < 0 || targetCol >= m_TotalCols || m_TotalCols <=0 || m_LastRowNo < 0)
		return(FALSE);

	int i;

	// バッファにコピー
	for( i= 0; i <= m_LastRowNo ; i++ ){
		memset( m_StrData[i].d[COL_LIMIT],0,STRLEN_MAX+1 );
		memcpy( m_StrData[i].d[COL_LIMIT], m_StrData[i].d[targetCol], STRLEN_MAX+1 );
		m_StrData[i].celTag[COL_LIMIT] = m_StrData[i].celTag[targetCol];
		m_StrData[i].color[COL_LIMIT] = m_StrData[i].color[targetCol];
		// 削除する場合
		if( cut ){
			// セルデータ
			memmove( m_StrData[i].d[targetCol], m_StrData[i].d[targetCol+1],
					(m_TotalCols-targetCol-1) * (STRLEN_MAX+1) );
			memset( m_StrData[i].d[m_TotalCols-1], 0, (STRLEN_MAX+1) );

			// セルタグ
			memmove( &m_StrData[i].celTag[targetCol], &m_StrData[i].celTag[targetCol+1],
					(m_TotalCols-targetCol-1) * sizeof(long) );
			m_StrData[i].celTag[m_TotalCols-1] =0;

			// 色
			memmove( &m_StrData[i].color[targetCol], &m_StrData[i].color[targetCol+1],
					(m_TotalCols-targetCol-1) * sizeof(char) );
			m_StrData[i].color[m_TotalCols-1] =0;
		}
	}

	// 列タイトル
	memcpy( m_Title[COL_LIMIT], m_Title[targetCol], TITLELEN_MAX+1 );
	if( cut ){
		// セルタグ
		memmove( &m_StrData[i].celTag[targetCol], &m_StrData[i].celTag[targetCol+1],
					(m_TotalCols-targetCol-1) * sizeof(long) );
		m_StrData[i].celTag[targetCol] = m_StrData[i].celTag[COL_LIMIT];
		// セルタグ
		memmove( &m_StrData[i].color[targetCol], &m_StrData[i].color[targetCol+1],
					(m_TotalCols-targetCol-1) * sizeof(char) );
		m_StrData[i].color[targetCol] = m_StrData[i].color[COL_LIMIT];

		memmove( m_Title[targetCol], m_Title[targetCol+1],	(m_TotalCols-targetCol-1) * (TITLELEN_MAX+1) );
		memset( m_Title[m_TotalCols-1], 0, TITLELEN_MAX+1  );
		m_TotalCols --;
	}

	// コピーバッファOK
	m_CopyBufFull =TRUE;
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::ColPaste( long targetCol,BOOL insert)
//	タイプ：protected
//	機能　：バッファの内容を指定された列にコピーする
//	引数１：コピー先の列番号
//	戻り値：成功＝実際にコピーした列番号、失敗=-1
//---------------------------------------------------------------------------------------------------------------------
long TGridData::ColPaste( long targetCol,BOOL insert)
{
	if( m_TotalCols > COL_LIMIT ) return(-1);
	if( !m_CopyBufFull ) return(-1);

	// 引数チェック
	if( m_LastRowNo < 0)
		return(-1);

	// 列が無い場合は、新規に列を作成して、上書きで処理する
	if( targetCol < 0 ) targetCol = 0;
	if( m_TotalCols < 0 ){
		m_TotalCols =1;
		targetCol = 0;
		insert = FALSE;
	}

	int i,offset;

	// 上書きorずらす必要なし
	if( targetCol > m_TotalCols || !insert){
		if( targetCol > m_TotalCols)
				targetCol = m_TotalCols;

		if( targetCol >= COL_LIMIT ) return(-1);		// これ以上無理

		// コピーする
		for( i= 0; i <= m_LastRowNo ; i++ ){
			memcpy( m_StrData[i].d[targetCol], m_StrData[i].d[COL_LIMIT], STRLEN_MAX+1  );
			m_StrData[i].celTag[targetCol] = m_StrData[i].celTag[COL_LIMIT];
			m_StrData[i].color[targetCol] = m_StrData[i].color[COL_LIMIT];
		}
		memcpy( m_Title[targetCol], m_Title[COL_LIMIT], TITLELEN_MAX+1  );

		// 列を増やした場合
		if( targetCol == m_TotalCols)
					m_TotalCols++;
		return(targetCol);
	}

	// ずらす場合（列MAX）
	if( m_TotalCols == COL_LIMIT ) offset =1;
	// !列max
	else{
		offset = 0;
	}

	// コピーする
	for( i= 0; i <= m_LastRowNo ; i++ ){
		memmove( m_StrData[i].d[targetCol+1], m_StrData[i].d[targetCol],	(m_TotalCols-targetCol-offset) * (STRLEN_MAX+1) );
		memcpy( m_StrData[i].d[targetCol], m_StrData[i].d[COL_LIMIT], STRLEN_MAX+1  );

		// セルタグ
		memmove( &m_StrData[i].celTag[targetCol+1], &m_StrData[i].celTag[targetCol],
					(m_TotalCols-targetCol-offset) * sizeof(long) );
		m_StrData[i].celTag[targetCol] = m_StrData[i].celTag[COL_LIMIT];
		// セルタグ
		memmove( &m_StrData[i].color[targetCol+1], &m_StrData[i].color[targetCol],
					(m_TotalCols-targetCol-offset) * sizeof(char) );
		m_StrData[i].color[targetCol] = m_StrData[i].color[COL_LIMIT];
	}

	memmove( m_Title[targetCol+1], m_Title[targetCol],	(m_TotalCols-targetCol-offset) * (TITLELEN_MAX+1) );
	memcpy( m_Title[targetCol], m_Title[COL_LIMIT], TITLELEN_MAX+1  );

	// 列を増やす
	if( !offset ) 
		m_TotalCols++;

	return(targetCol);


}

long TGridData::AddCol( long targetCol )
{
	BOOL insert = TRUE;

	if( m_TotalCols > COL_LIMIT ) return(-1);

	// 引数チェック
	if( m_LastRowNo < 0)		return(-1);

	// 列が無い場合は、新規に列を作成して、上書きで処理する
	if( targetCol < 0 ) targetCol = 0;
	if( m_TotalCols < 0 ){
		m_TotalCols =1;
		targetCol = 0;
		insert = FALSE;
	}

	int i,offset;

	// ずらす必要なし
	if( targetCol > m_TotalCols || !insert){
		if( targetCol > m_TotalCols)	targetCol = m_TotalCols;

		if( targetCol >= COL_LIMIT ) return(-1);		// これ以上無理

		// クリアする
		for( i= 0; i <= m_LastRowNo ; i++ )	memset( m_StrData[i].d[targetCol], 0, STRLEN_MAX+1  );
		memset( m_Title[targetCol],0 , TITLELEN_MAX+1  );

		// 列を増やした場合
		if( targetCol == m_TotalCols)	m_TotalCols++;
		return(targetCol);
	}

	// ずらす場合（列MAX）
	if( m_TotalCols == COL_LIMIT ) offset =1;
	// !列max
	else	offset = 0;

	// クリア
	for( i= 0; i <= m_LastRowNo ; i++ ){
		memmove( m_StrData[i].d[targetCol+1], m_StrData[i].d[targetCol],	(m_TotalCols-targetCol-offset) * (STRLEN_MAX+1) );
		memset( m_StrData[i].d[targetCol], 0, STRLEN_MAX+1  );
		// セルタグ
		memmove( &m_StrData[i].celTag[targetCol+1], &m_StrData[i].celTag[targetCol],
					(m_TotalCols-targetCol-offset) * sizeof(long) );
		m_StrData[i].celTag[targetCol] = m_StrData[i].celTag[COL_LIMIT];
		// セルタグ
		memmove( &m_StrData[i].color[targetCol+1], &m_StrData[i].color[targetCol],
					(m_TotalCols-targetCol-offset) * sizeof(char) );
		m_StrData[i].color[targetCol] = m_StrData[i].color[COL_LIMIT];
	}

	memmove( m_Title[targetCol+1], m_Title[targetCol],	(m_TotalCols-targetCol-offset) * (TITLELEN_MAX+1) );
	memset( m_Title[targetCol],0 , TITLELEN_MAX+1  );
	memcpy( m_Title[targetCol],"??" , 2  );

	// 列を増やす
	if( !offset ) 		m_TotalCols++;

	return(targetCol);


}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::DataClear(void)
//	タイプ：private
//	機能　：現在保持している全データをクリアする
//	戻り値：TRUE=クリア実施
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::DataClear(void)
{
	if( m_LastRowNo < 0 ) return(FALSE);			// 1行も保持していない
	if( m_TotalCols < 1 ) return(FALSE);			// 1列も保持していない

	memset( m_StrData, 0, (m_LastRowNo+1) * sizeof(LISTSTRDATA) );

	m_LastRowNo = -1;
	return(TRUE);
}



