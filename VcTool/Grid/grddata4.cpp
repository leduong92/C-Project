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
#include "grddata4.h"

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
	int i,j;

	m_TotalCols=totalcol;


	// 行の合計
	m_LastRowNo=-1;

	// ヘッダのあるなし
	m_RowTitleEnable=title;

	m_AllocBlock = rowBlock;				// アロケートする単位
	if( m_AllocBlock < 1 ) m_AllocBlock = 20;


	// 行objectを１ブロックアロケートする
	LISTSTRDATA *tmp=NULL;
	m_StrData = NULL;


	// allocate
	m_StrData = (LISTSTRDATA*)malloc(sizeof(LISTSTRDATA) * m_AllocBlock);

	if(m_StrData==NULL) return;
	m_AllocRows = m_AllocBlock;				// アロケート済み行数

	m_Title = (char**)malloc( sizeof(char*)*(totalcol+1) );
	for(i=0;i<totalcol+1;i++){
		m_Title[i]=(char*)malloc( sizeof(char) * (TITLELEN_MAX+1) );
		memset( m_Title[i], 0, TITLELEN_MAX+1 );
	}

	// 各行のデータ領域をアロケートする
	for(i=0; i<m_AllocRows;i++){
		m_StrData[i].d = (char**)malloc( sizeof(char*) * (totalcol+1) );

//		m_StrData[i].color = (char*)malloc(totalcol+1);
//		m_StrData[i].bkcolor = (char*)malloc(totalcol+1);
		m_StrData[i].ncolor = (COLORREF*)malloc(sizeof(COLORREF)*(totalcol+1));
		m_StrData[i].nbkcolor = (COLORREF*)malloc(sizeof(COLORREF)*(totalcol+1));
		m_StrData[i].celTag = (long*)malloc(sizeof(long)*(totalcol+1) );
		m_StrData[i].Tag=0;

		for(j=0;j<totalcol+1;j++){
			m_StrData[i].d[j]=(char*)malloc(2);
			memset(m_StrData[i].d[j],0,2);
//			m_StrData[i].color[j] = 0;
//			m_StrData[i].bkcolor[j] = 0;
			m_StrData[i].ncolor[j] = 0;
			m_StrData[i].nbkcolor[j] = RGB(255,255,255);	// 初期値は白
			m_StrData[i].celTag[j] = 0;
		}

	}

	// コピーバッファ空
	m_CopyBufFull =FALSE;
	m_retBuf=NULL;

	this->SortColNo=-1;
}
//---------------------------------------------------------------------------------------------------------------------
//	TGridData::TGridData(long totalcol)
//	タイプ：private
//	機能　：デストラクタ。メモリ解放
//---------------------------------------------------------------------------------------------------------------------
TGridData::~TGridData()
{
	int i,j;

	if( m_StrData != NULL){


		// 各行のデータ領域を解放する
		for(i=0; i<m_AllocRows;i++){
			for(j=0;j<m_TotalCols+1;j++){
//				if( m_StrData[i].d[j]!=NULL)	delete m_StrData[i].d[j];
//				if( m_StrData[i].d[j]!=NULL)	free(m_StrData[i].d[j]);
					free(m_StrData[i].d[j]);
					m_StrData[i].d[j]=NULL;
			}
			free( m_StrData[i].celTag);
//			free( m_StrData[i].bkcolor);
//			free( m_StrData[i].color);
			free( m_StrData[i].nbkcolor);
			free( m_StrData[i].ncolor);
			free(m_StrData[i].d);
		}

		free(m_StrData);

		// 列題領域解放
		for(i=0;i<m_TotalCols+1;i++)
			free(m_Title[i]);

		free( m_Title);
	}
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
//	BOOL TGridData::SetTitle(int col,char* title)
//	タイプ：protected
//	機能　：列ヘッダのデータ設定
//	引数１：設定する列番号
//	引数２：列の題
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::GetTitle(int col,char* buf,int bufsize)
{
	if( col >= TGridData::m_TotalCols || col < 0) return(FALSE);

	memcpy( buf,m_Title[col],  (int)strlen(m_Title[col])>bufsize ? bufsize:strlen(m_Title[col]) );
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
//	引数５：セルの背景色 0=白、1=無効色 -1=変更なし（前の色を使用する）
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SetCelData(int col,int row, char color,long cTag,char* data,char bkcolor)
{
	COLORREF ncolor;
	COLORREF nbkcolor;

	ncolor=( color==0 )?RGB(0,0,0):RGB(255,0,0);
	nbkcolor=( bkcolor==0||(unsigned char)bkcolor==0xff )?RGB(255,255,255):RGB(128,128,128);
	return(SetCelDataEx(col,row,ncolor,cTag,data,nbkcolor));
/*
	if( col >= m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);

	m_StrData[row].celTag[col] = cTag;
	m_StrData[row].color[col] = color;
	if( bkcolor !=-1)
		m_StrData[row].bkcolor[col] = bkcolor;

	if( m_StrData[row].d[col] !=NULL){
		free(m_StrData[row].d[col]);
		m_StrData[row].d[col] = NULL;
	}

	int len;
	if( data==NULL) len=0;
	else len=strlen(data);

	m_StrData[row].d[col]=(char*)malloc(len+1);

	memset( m_StrData[row].d[col],0, len+1);
	if( data !=NULL )
		memcpy( m_StrData[row].d[col],data, len);

	return(TRUE);
*/
}
//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::SetCelData(int col,int row,char* data)
//	タイプ：protected
//	機能　：セルにデータを設定
//	引数１：設定する列番号
//	引数２：設定する行番号
//	引数３：設定データ(文字)
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SetCelData(int col,int row,char* data)
{
	return(SetCelDataEx(col,row,GetCelColEx(col,row),GetCelTag(col,row),data,GetCelBkColEx(col,row)));
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
//	引数５：セルの背景色 0=白、1=無効色 -1=変更なし（前の色を使用する）
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SetCelDataEx(int col,int row, COLORREF color,long cTag,char* data,COLORREF bkcolor)
{
	if( col >= m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);

	m_StrData[row].celTag[col] = cTag;
	m_StrData[row].ncolor[col] = color;

	m_StrData[row].nbkcolor[col] = bkcolor;

	if( m_StrData[row].d[col] !=NULL){
		free(m_StrData[row].d[col]);
		m_StrData[row].d[col] = NULL;
	}

	int len;
	if( data==NULL) len=0;
	else len=strlen(data);

	m_StrData[row].d[col]=(char*)malloc(len+1);

	memset( m_StrData[row].d[col],0, len+1);
	if( data !=NULL )
		memcpy( m_StrData[row].d[col],data, len);

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
	COLORREF nbkcolor;
	nbkcolor=( color==0 )?RGB(255,255,255):RGB(128,128,128);

	return(SetCelBkColorEx(col,row,nbkcolor));
/*
	if( col >= m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);

	m_StrData[row].bkcolor[col] = color;

	return(TRUE);
*/
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
BOOL TGridData::SetCelBkColorEx(int col,int row, COLORREF color)
{
	if( col >= m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);

	m_StrData[row].nbkcolor[col] = color;

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

	int i,j;

	m_LastRowNo++;

	// アロケート済み行数で足りないときは確保
	if( m_LastRowNo >= m_AllocRows ){
		LISTSTRDATA *tmp=NULL;
		// allocate
		m_AllocRows += m_AllocBlock;
		tmp = (LISTSTRDATA*)realloc(m_StrData, sizeof(LISTSTRDATA) * m_AllocRows);

		if(NULL==tmp) return(-1);

		m_StrData = tmp;
		// データ領域のアロケート
		for( i=m_LastRowNo;i<m_AllocRows;i++){
			m_StrData[i].d = (char**)malloc(sizeof(char*) * (m_TotalCols+1) );
//			m_StrData[i].color = (char*)malloc(m_TotalCols+1);
//			m_StrData[i].bkcolor = (char*)malloc(m_TotalCols+1);
			m_StrData[i].ncolor = (COLORREF*)malloc(sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].nbkcolor = (COLORREF*)malloc(sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].celTag = (long*)malloc(sizeof(long)*(m_TotalCols+1) );
			for(j=0;j<m_TotalCols+1;j++){
				m_StrData[i].d[j]=(char*)malloc(1);
				memset(m_StrData[i].d[j],0,1);
//				m_StrData[i].color[j]=0;
//				m_StrData[i].bkcolor[j]=0;
				m_StrData[i].ncolor[j]=0;
				m_StrData[i].nbkcolor[j]=RGB(255,255,255);
				m_StrData[i].celTag[j]=0;
			}
		}
  }

	memset(m_StrData[m_LastRowNo].colTitle,0,TITLELEN_MAX+1);
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
	m_LastRowNo++;
	int i,j;

	// アロケート済み行数で足りないときは確保
	if( m_LastRowNo >= m_AllocRows ){
		LISTSTRDATA *tmp=NULL;
		// allocate
		m_AllocRows += m_AllocBlock;
		tmp = (LISTSTRDATA*)realloc(m_StrData, sizeof(LISTSTRDATA) * m_AllocRows);

		if(NULL==tmp) return(-1);

		m_StrData = tmp;

		// データ領域のアロケート
		for( i=m_LastRowNo;i<m_AllocRows;i++){
			m_StrData[i].d = (char**)malloc(sizeof(char*) * (m_TotalCols+1) );
//			m_StrData[i].color = (char*)malloc(sizeof(char)*(m_TotalCols+1));
//			m_StrData[i].bkcolor = (char*)malloc(sizeof(char)*(m_TotalCols+1));
			m_StrData[i].ncolor = (COLORREF*)malloc(sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].nbkcolor = (COLORREF*)malloc(sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].celTag = (long*)malloc(sizeof(long)*(m_TotalCols+1) );

			for(j=0;j<m_TotalCols+1;j++){
				m_StrData[i].d[j]=(char*)malloc(sizeof(char)*1);
				memset(m_StrData[i].d[j],0,1);
//				m_StrData[i].color[j]=0;
//				m_StrData[i].bkcolor[j]=0;
				m_StrData[i].ncolor[j]=0;
				m_StrData[i].nbkcolor[j]=RGB(255,255,255);
				m_StrData[i].celTag[j]=0;
			}
		}
  }

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
//	BOOL TGridData::ChangeRowTitle(long rowNo, char* newtitle)
//	タイプ：protected
//	機能　：行題の変更
//	引数１：変更する行
//	引数２：変更後の行題
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::GetRowTitle(int rowNo, char* buf,int bufsize)
{
	memset(buf,0,bufsize);

	if( rowNo > m_LastRowNo || rowNo < 0) return(FALSE);

	memcpy(buf,m_StrData[rowNo].colTitle
				, (int)strlen(m_StrData[rowNo].colTitle)>bufsize-1 ? bufsize-1 : strlen(m_StrData[rowNo].colTitle));

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::ChangeRowTitle2(long rowNo, char* newtitle)
//	タイプ：protected
//	機能　：行題の変更
//	引数１：変更する行
//	引数２：変更後の行題
//	戻り値：成功＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::ChangeRowTitle2(long rowNo, char* newtitle)
{
	if( rowNo > m_LastRowNo || rowNo < 0) return(FALSE);

	//if( newtitle==NULL) return(FALSE);

	memset(m_StrData[rowNo].colTitle,0,TITLELEN_MAX+1);
	memcpy(m_StrData[rowNo].colTitle, newtitle, strlen(newtitle)>TITLELEN_MAX ? TITLELEN_MAX : strlen(newtitle));

	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::GetCel(char* buf, int bufsize,int col,int row )
//	タイプ：public
//	機能　：指定した行が有効か調べる
//	引数１：列番号
//	引数２：行番号
//	戻り値：有効＝TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::IsCelEnable( int col, int row)
{
	if( col >= m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);
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
	if( m_retBuf !=NULL ) delete m_retBuf;

	m_retBuf=new char[1];
	memset( m_retBuf, 0, 1);

	if( col > m_TotalCols || col < 0) return(m_retBuf);
	if( row > m_LastRowNo || row < 0) return(m_retBuf);
	if( m_StrData[row].d[col] == NULL ) return(m_retBuf);

	int i,len;

	len = strlen(m_StrData[row].d[col]);
	m_retBuf=new char[len+1];
	memset( m_retBuf, 0, len+1);
	memcpy( m_retBuf,m_StrData[row].d[col], len);
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

	int i,len,j;
//	char* newpt;
	COLORREF* newpt;
	char**newstr;
	long *newtag;

	// バッファにコピー
//	for( i= 0; i <= m_LastRowNo ; i++ ){
	for(i=0; i<m_AllocRows;i++){
		// コピーバッファの領域を確保する
		if( m_StrData[i].d[m_TotalCols] !=NULL){
			free(m_StrData[i].d[m_TotalCols]);
			m_StrData[i].d[m_TotalCols]=NULL;
		}

		if( m_StrData[i].d[targetCol] ==NULL )len=0;
		else len = strlen(m_StrData[i].d[targetCol]);
		m_StrData[i].d[m_TotalCols] = (char*)malloc(sizeof( char) * (len+1) );

		// 文字データとセル情報をコピーする
		memset( m_StrData[i].d[m_TotalCols],0,len+1 );
		memcpy( m_StrData[i].d[m_TotalCols], m_StrData[i].d[targetCol], len );
		m_StrData[i].celTag[m_TotalCols] = m_StrData[i].celTag[targetCol];
//		m_StrData[i].color[m_TotalCols] = m_StrData[i].color[targetCol];
//		m_StrData[i].bkcolor[m_TotalCols] = m_StrData[i].bkcolor[targetCol];
		m_StrData[i].ncolor[m_TotalCols] = m_StrData[i].ncolor[targetCol];
		m_StrData[i].nbkcolor[m_TotalCols] = m_StrData[i].nbkcolor[targetCol];

		// 削除する場合はメモリをずらす
		if( cut ){
			// セルデータ
			for( j=targetCol;j < m_TotalCols; j++ )
				m_StrData[i].d[j]=m_StrData[i].d[j+1];
			newstr = (char**)realloc(m_StrData[i].d, sizeof(char*) * (m_TotalCols) );
			m_StrData[i].d=newstr;

			// セル1タグ
			memmove( &m_StrData[i].celTag[targetCol], &m_StrData[i].celTag[targetCol+1],
					(m_TotalCols-targetCol) * sizeof(long) );
			newtag = (long*)realloc(m_StrData[i].celTag, sizeof(long)*(m_TotalCols));
			m_StrData[i].celTag=newtag;

//			// 色
//			memmove( &m_StrData[i].color[targetCol], &m_StrData[i].color[targetCol+1],
//					(m_TotalCols-targetCol) * sizeof(char) );
//			newpt = (char*)realloc(m_StrData[i].color, m_TotalCols);
//			m_StrData[i].color=newpt;
//			// 色
//			memmove( &m_StrData[i].bkcolor[targetCol], &m_StrData[i].bkcolor[targetCol+1],
//					(m_TotalCols-targetCol) * sizeof(char) );
//			newpt = (char*)realloc(m_StrData[i].bkcolor, m_TotalCols);
//			m_StrData[i].bkcolor=newpt;

			// 色
			memmove( &m_StrData[i].ncolor[targetCol], &m_StrData[i].ncolor[targetCol+1],
					(m_TotalCols-targetCol) * sizeof(COLORREF) );
			newpt = (COLORREF*)realloc(m_StrData[i].ncolor, sizeof(COLORREF)*m_TotalCols);
			m_StrData[i].ncolor=newpt;
			// 色
			memmove( &m_StrData[i].nbkcolor[targetCol], &m_StrData[i].nbkcolor[targetCol+1],
					(m_TotalCols-targetCol) * sizeof(COLORREF) );
			newpt = (COLORREF*)realloc(m_StrData[i].nbkcolor, sizeof(COLORREF)*m_TotalCols);
			m_StrData[i].nbkcolor=newpt;
		
		}
	}

	// 列タイトル
	memcpy( m_Title[m_TotalCols], m_Title[targetCol], TITLELEN_MAX+1 );
	if( cut ){
		for( j=targetCol;j < m_TotalCols; j++ )		m_Title[j]=m_Title[j+1];
		//free(m_Title[m_TotalCols]);

		newstr = (char**)realloc( m_Title, sizeof(char*) * (m_TotalCols) );
		m_Title = newstr;

		m_TotalCols --;
//		memset( m_Title[m_TotalCols-1], 0, TITLELEN_MAX+1  );
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
//	if( m_TotalCols > COL_LIMIT ) return(-1);
	if( !m_CopyBufFull ) return(-1);

	int i,j,len;
//	char* newpt;
	COLORREF* newpt;
	char**newstr;
	long *newtag;
	BOOL allocNew=FALSE;
	long prevCols;

	// 引数チェック
	if( m_LastRowNo < 0)		return(-1);

	// 列が無い場合は、新規に列を作成する
	if( targetCol < 0 ) targetCol = 0;

	if( m_TotalCols < 0 ){
		m_TotalCols =1;
		targetCol = 0;
		prevCols=0;
		allocNew = TRUE;
	}
	// 新規に列の追加が必要な場合
	else if( targetCol >= m_TotalCols){
		prevCols=m_TotalCols;
		// 領域アロケート
		m_TotalCols=targetCol+1;
		allocNew = TRUE;
	}

	// --------------------------------------①新規に列を追加する場合-------------------------------------------
	if( allocNew == TRUE ){
		// 領域アロケート
		m_TotalCols=targetCol+1;
		for(i=0; i<m_AllocRows;i++){
//		for( i= 0; i <= m_LastRowNo ; i++ ){
			// セルデータ領域
			newstr = (char**)realloc(m_StrData[i].d, sizeof(char*) * (m_TotalCols+1) );
			m_StrData[i].d = newstr;
			newtag = (long*)realloc(m_StrData[i].celTag, sizeof(long) * (m_TotalCols+1) );
			m_StrData[i].celTag=newtag;
			newpt = (COLORREF*)realloc(m_StrData[i].ncolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].ncolor=newpt;
			newpt = (COLORREF*)realloc(m_StrData[i].nbkcolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].nbkcolor=newpt;

			// 追加した領域を初期化する
			for(j=prevCols+1; j < m_TotalCols+1;j++){
				m_StrData[i].d[j]=(char*)malloc(1);
				memset(m_StrData[i].d[j], 0, 1 );
//				m_StrData[i].bkcolor[j]=0;
//				m_StrData[i].color[j]=0;
				m_StrData[i].nbkcolor[j]=RGB(255,255,255);
				m_StrData[i].ncolor[j]=0;
				m_StrData[i].celTag[j]=0;
			}

			// アロケート前コピーバッファの内容を目的の列にコピーする
			if( m_StrData[i].d[targetCol] !=NULL){
				free(m_StrData[i].d[targetCol]);
				m_StrData[i].d[targetCol] = NULL;
			}

			if( m_StrData[i].d[prevCols] ==NULL) len=0;
			else len=strlen(m_StrData[i].d[prevCols]);
			m_StrData[i].d[targetCol]=(char*)malloc( sizeof(char) * (len+1));
			memset( m_StrData[i].d[targetCol],0, len+1);
			if(len)		memcpy( m_StrData[i].d[targetCol],m_StrData[i].d[prevCols], len);

//			m_StrData[i].bkcolor[targetCol] = m_StrData[i].bkcolor[prevCols];
//			m_StrData[i].color[targetCol] = m_StrData[i].color[prevCols];
			m_StrData[i].nbkcolor[targetCol] = m_StrData[i].nbkcolor[prevCols];
			m_StrData[i].ncolor[targetCol] = m_StrData[i].ncolor[prevCols];
			m_StrData[i].celTag[targetCol] = m_StrData[i].celTag[prevCols];

			// アロケート前コピーバッファの内容を新規のコピーバッファにコピーする
			if( m_StrData[i].d[m_TotalCols] !=NULL){
				free( m_StrData[i].d[m_TotalCols]);
				m_StrData[i].d[m_TotalCols] = NULL;
			}

			m_StrData[i].d[m_TotalCols]=(char*)malloc( sizeof(char) * (len+1));
			memset( m_StrData[i].d[m_TotalCols],0, len+1);
			if(len)	memcpy( m_StrData[i].d[targetCol],m_StrData[i].d[prevCols], len);

//			m_StrData[i].bkcolor[m_TotalCols] = m_StrData[i].bkcolor[prevCols];
//			m_StrData[i].color[m_TotalCols] = m_StrData[i].color[prevCols];
			m_StrData[i].nbkcolor[m_TotalCols] = m_StrData[i].nbkcolor[prevCols];
			m_StrData[i].ncolor[m_TotalCols] = m_StrData[i].ncolor[prevCols];
			m_StrData[i].celTag[m_TotalCols] = m_StrData[i].celTag[prevCols];

			// アロケート前のコピーバッファをクリアする
			if( m_StrData[i].d[prevCols] !=NULL)  free( m_StrData[i].d[prevCols]);
			m_StrData[i].d[prevCols]=(char*)malloc( sizeof(char));
			m_StrData[i].d[prevCols][0] = 0;

//			m_StrData[i].bkcolor[prevCols] = 0;
//			m_StrData[i].color[prevCols] = 0;
			m_StrData[i].nbkcolor[prevCols] = RGB(255,255,255);
			m_StrData[i].ncolor[prevCols] = 0;
			m_StrData[i].celTag[prevCols] = 0;
		}

		// 列題領域
		newstr = (char**)realloc( m_Title, sizeof(char*) * (m_TotalCols+1) );
		m_Title = newstr;
		for(j=prevCols+1; j < m_TotalCols+1;j++){
			m_Title[j]=(char*)malloc(TITLELEN_MAX+1);
			memset( m_Title[j], 0, TITLELEN_MAX+1);
		}
		memcpy( m_Title[targetCol], m_Title[prevCols],TITLELEN_MAX+1);
		memcpy( m_Title[m_TotalCols], m_Title[prevCols],TITLELEN_MAX+1);
		memset( m_Title[prevCols], 0, TITLELEN_MAX+1 );

		return(targetCol);
	}


	// -------------------------------------------②挿入する場合-------------------------------------------
	if( insert) m_TotalCols++;
	// -------------------------------------------③既存の列に上書きする場合-------------------------------------------
	for(i=0; i<m_AllocRows;i++){
//	for( i= 0; i <= m_LastRowNo ; i++ ){
		// 挿入する場合は領域アロケートして調整
		if( insert ){
			// セルデータ領域
			newstr = (char**)realloc(m_StrData[i].d, sizeof(char*) * (m_TotalCols+1) );
			m_StrData[i].d = newstr;
			newtag = (long*)realloc(m_StrData[i].celTag, sizeof(long) * (m_TotalCols+1) );
			m_StrData[i].celTag=newtag;
			newpt = (COLORREF*)realloc(m_StrData[i].ncolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].ncolor=newpt;
			newpt = (COLORREF*)realloc(m_StrData[i].nbkcolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].nbkcolor=newpt;

			// データを右方向にシフトする
			// セルデータ
			for( j=m_TotalCols;j > targetCol; j-- )	m_StrData[i].d[j]=m_StrData[i].d[j-1];

			// セルタグ
			memmove( &m_StrData[i].celTag[targetCol+1], &m_StrData[i].celTag[targetCol],
							(m_TotalCols-targetCol) * sizeof(long) );
//			// 色
//			memmove( &m_StrData[i].color[targetCol+1], &m_StrData[i].color[targetCol],
//							(m_TotalCols-targetCol) * sizeof(char) );
//			memmove( &m_StrData[i].bkcolor[targetCol+1], &m_StrData[i].bkcolor[targetCol],
//							(m_TotalCols-targetCol) * sizeof(char) );
			// 色
			memmove( &m_StrData[i].ncolor[targetCol+1], &m_StrData[i].ncolor[targetCol],
							(m_TotalCols-targetCol) * sizeof(COLORREF) );
			memmove( &m_StrData[i].nbkcolor[targetCol+1], &m_StrData[i].nbkcolor[targetCol],
							(m_TotalCols-targetCol) * sizeof(COLORREF) );
		}

		// コピーバッファの内容を目的の列にコピーする
		if( m_StrData[i].d[m_TotalCols] ==NULL) len=0;
		else len = strlen(m_StrData[i].d[m_TotalCols]);
		m_StrData[i].d[targetCol]=(char*)malloc(len+1);
		memset( m_StrData[i].d[targetCol],0, len+1);
		if(len)		memcpy( m_StrData[i].d[targetCol],m_StrData[i].d[m_TotalCols], len);

//		m_StrData[i].bkcolor[targetCol] = m_StrData[i].bkcolor[m_TotalCols];
//		m_StrData[i].color[targetCol] = m_StrData[i].color[m_TotalCols];
		m_StrData[i].nbkcolor[targetCol] = m_StrData[i].nbkcolor[m_TotalCols];
		m_StrData[i].ncolor[targetCol] = m_StrData[i].ncolor[m_TotalCols];
		m_StrData[i].celTag[targetCol] = m_StrData[i].celTag[m_TotalCols];
	}

	// 列題領域
	if( insert ){
		newstr = (char**)realloc( m_Title, sizeof(char*) * (m_TotalCols+1) );
		m_Title = newstr;
			for( j=m_TotalCols;j > targetCol; j-- ) m_Title[j]=m_Title[j-1];
		m_Title[targetCol]=(char*)malloc(TITLELEN_MAX+1);
		memset( m_Title[targetCol], 0,TITLELEN_MAX+1);
	}
	memcpy( m_Title[targetCol], m_Title[m_TotalCols],TITLELEN_MAX+1);

	return(targetCol);


}

long TGridData::AddCol( long targetCol )
{

	int i,j;
//	char* newpt;
	COLORREF* newpt;
	char**newstr;
	long *newtag;
	BOOL allocNew=FALSE;
	long prevCols;

	// 引数チェック
	if( m_LastRowNo < 0)		return(-1);
	// 列が無い場合は、新規に列を作成する
	if( targetCol < 0 ) targetCol = 0;

	if( m_TotalCols < 0 ){
		m_TotalCols =1;
		targetCol = 0;
		prevCols=0;
		allocNew = TRUE;
	}
	// 新規に列の追加が必要な場合
	else if( targetCol >= m_TotalCols){
		prevCols=m_TotalCols;
		// 領域アロケート
		m_TotalCols=targetCol+1;
		allocNew = TRUE;
	}

	// --------------------------------------①新規に列を追加する場合-------------------------------------------
	if( allocNew == TRUE ){
		// 領域アロケート
		m_TotalCols=targetCol+1;
//		for( i= 0; i <= m_LastRowNo ; i++ ){
		for(i=0; i<m_AllocRows;i++){
			// セルデータ領域
			newstr = (char**)realloc(m_StrData[i].d, sizeof(char*) * (m_TotalCols+1) );
			m_StrData[i].d = newstr;
			newtag = (long*)realloc(m_StrData[i].celTag, sizeof(long) * (m_TotalCols+1) );
			m_StrData[i].celTag=newtag;
			newpt = (COLORREF*)realloc(m_StrData[i].ncolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].ncolor=newpt;
			newpt = (COLORREF*)realloc(m_StrData[i].nbkcolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].nbkcolor=newpt;

			// 追加した領域を初期化する
			for(j=prevCols+1; j < m_TotalCols+1;j++){
				m_StrData[i].d[j]=(char*)malloc(1);
				memset(m_StrData[i].d[j], 0, 1 );
//				m_StrData[i].bkcolor[j]=0;
//				m_StrData[i].color[j]=0;
				m_StrData[i].nbkcolor[j]=RGB(255,255,255);
				m_StrData[i].ncolor[j]=0;
				m_StrData[i].celTag[j]=0;
			}

			// アロケート前コピーバッファの内容を新規のコピーバッファにコピーする
			if( m_StrData[i].d[m_TotalCols] !=NULL){
				free(m_StrData[i].d[m_TotalCols]);
				m_StrData[i].d[m_TotalCols] = NULL;
			}

			m_StrData[i].d[m_TotalCols]=(char*)malloc(sizeof(char));
			memset( m_StrData[i].d[m_TotalCols],0, 1);

//			m_StrData[i].bkcolor[m_TotalCols] = m_StrData[i].bkcolor[prevCols];
//			m_StrData[i].color[m_TotalCols] = m_StrData[i].color[prevCols];
			m_StrData[i].nbkcolor[m_TotalCols] = m_StrData[i].nbkcolor[prevCols];
			m_StrData[i].ncolor[m_TotalCols] = m_StrData[i].ncolor[prevCols];
			m_StrData[i].celTag[m_TotalCols] = m_StrData[i].celTag[prevCols];

			// アロケート前のコピーバッファをクリアする
			if( m_StrData[i].d[prevCols] !=NULL)  free(m_StrData[i].d[prevCols]);
			m_StrData[i].d[prevCols] = (char*)malloc(1);
			memset(m_StrData[i].d[prevCols],0,1);

//			m_StrData[i].bkcolor[prevCols] = 0;
//			m_StrData[i].color[prevCols] = 0;
			m_StrData[i].nbkcolor[prevCols] = RGB(255,255,255);
			m_StrData[i].ncolor[prevCols] = 0;
			m_StrData[i].celTag[prevCols] = 0;
		}

		// 列題領域
		newstr = (char**)realloc( m_Title, sizeof(char*) * (m_TotalCols+1) );
		m_Title = newstr;
		for(j=prevCols+1; j < m_TotalCols+1;j++){
			m_Title[j]=(char*)malloc(TITLELEN_MAX+1);
			memset( m_Title[j], 0, TITLELEN_MAX+1);
		}
		memcpy( m_Title[m_TotalCols], m_Title[prevCols],TITLELEN_MAX+1);
		memset( m_Title[prevCols], 0, TITLELEN_MAX+1 );

		return(targetCol);
	}


	m_TotalCols++;

//	for( i= 0; i <= m_LastRowNo ; i++ ){
	for(i=0; i<m_AllocRows;i++){
		// 挿入する場合は領域アロケートして調整
		// セルデータ領域
		newstr = (char**)realloc(m_StrData[i].d, sizeof(char*) * (m_TotalCols+1) );
		m_StrData[i].d = newstr;
		newtag = (long*)realloc(m_StrData[i].celTag, sizeof(long) * (m_TotalCols+1) );
		m_StrData[i].celTag=newtag;
		newpt = (COLORREF*)realloc(m_StrData[i].ncolor, sizeof(COLORREF)*(m_TotalCols+1));
		m_StrData[i].ncolor=newpt;
		newpt = (COLORREF*)realloc(m_StrData[i].nbkcolor, sizeof(COLORREF)*(m_TotalCols+1));
		m_StrData[i].nbkcolor=newpt;

		// データを右方向にシフトする
		// セルデータ
		for( j=m_TotalCols;j > targetCol; j-- )	m_StrData[i].d[j]=m_StrData[i].d[j-1];

		// セルタグ
		memmove( &m_StrData[i].celTag[targetCol+1], &m_StrData[i].celTag[targetCol],
							(m_TotalCols-targetCol) * sizeof(long) );
//		// 色
//		memmove( &m_StrData[i].color[targetCol+1], &m_StrData[i].color[targetCol],
//							(m_TotalCols-targetCol) * sizeof(char) );
//		memmove( &m_StrData[i].bkcolor[targetCol+1], &m_StrData[i].bkcolor[targetCol],
//							(m_TotalCols-targetCol) * sizeof(char) );
		// 色
		memmove( &m_StrData[i].ncolor[targetCol+1], &m_StrData[i].ncolor[targetCol],
							(m_TotalCols-targetCol) * sizeof(COLORREF) );
		memmove( &m_StrData[i].nbkcolor[targetCol+1], &m_StrData[i].nbkcolor[targetCol],
							(m_TotalCols-targetCol) * sizeof(COLORREF) );


		m_StrData[i].d[targetCol]=(char*)malloc(1);
		memset( m_StrData[i].d[targetCol],0, 1);

//		m_StrData[i].bkcolor[targetCol] = 0;
//		m_StrData[i].color[targetCol] = 0;
		m_StrData[i].nbkcolor[targetCol] = RGB(255,255,255);
		m_StrData[i].ncolor[targetCol] = 0;
		m_StrData[i].celTag[targetCol] = 0;
	}

	// 列題領域
	newstr = (char**)realloc( m_Title, sizeof(char*) * (m_TotalCols+1) );
	m_Title = newstr;
	for( j=m_TotalCols;j > targetCol; j-- ) m_Title[j]=m_Title[j-1];
	m_Title[targetCol]=(char*)malloc(TITLELEN_MAX+1);
	memset( m_Title[targetCol], 0,TITLELEN_MAX+1);

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

	int i,j;

	for( i= 0; i <= m_LastRowNo ; i++ ){
		// データ領域を初期化する
		for(j=0; j < m_TotalCols+1;j++){
			free(m_StrData[i].d[j]);
			m_StrData[i].d[j]=(char*)malloc(1);
			memset(m_StrData[i].d[j], 0, 1 );
//			m_StrData[i].bkcolor[j]=0;
//			m_StrData[i].color[j]=0;
			m_StrData[i].nbkcolor[j]=RGB(255,255,255);
			m_StrData[i].ncolor[j]=0;
			m_StrData[i].celTag[j]=0;
		}
	}
//	memset( m_StrData, 0, (m_LastRowNo+1) * sizeof(LISTSTRDATA) );

	m_LastRowNo = -1;
	return(TRUE);
}



//---------------------------------------------------------------------------------------------------------------------
//	long TGridData::DelRow(long row)
//	タイプ：protected
//	機能　：行を削除
//	引数１：削除行
//	戻り値：成功＝現在の行数 －１以下＝失敗
//---------------------------------------------------------------------------------------------------------------------
long TGridData::RowDelete(long row)
{
	int i,j;
	int len;

	if( row > m_LastRowNo ) return -1;

	// 1行ずつずらす
	for( i=row;i<m_LastRowNo;i++){
		m_StrData[i].Tag = m_StrData[i+1].Tag;
		memcpy(m_StrData[i].colTitle, m_StrData[i+1].colTitle, TITLELEN_MAX+1);
		for(j=0;j<m_TotalCols+1;j++){
//			memcpy(m_StrData[i].d[j],m_StrData[i+1].d[j],1);
			len = strlen(m_StrData[i+1].d[j]);
			free(m_StrData[i].d[j]);
			m_StrData[i].d[j]=(char*)malloc(len+1);
			memset(m_StrData[i].d[j], 0, len+1 );
			memcpy(m_StrData[i].d[j],m_StrData[i+1].d[j],len);
			m_StrData[i].ncolor[j]	= m_StrData[i+1].ncolor[j];
			m_StrData[i].nbkcolor[j]= m_StrData[i+1].nbkcolor[j];
			m_StrData[i].celTag[j]	= m_StrData[i+1].celTag[j];
		}

	}

	// 削除した最終行をクリア
	for(j=0;j<m_TotalCols+1;j++){
		free(m_StrData[m_LastRowNo].d[j]);
		m_StrData[m_LastRowNo].d[j]=(char*)malloc(1);
		m_StrData[m_LastRowNo].d[j][0]=0;
		m_StrData[m_LastRowNo].nbkcolor[j]=RGB(255,255,255);;
		m_StrData[m_LastRowNo].ncolor[j]=0;
		m_StrData[m_LastRowNo].celTag[j]=0;
//		memset(m_StrData[m_LastRowNo].d[j],0,1);
	}
	m_LastRowNo--;
	return(m_LastRowNo+1);
}

//---------------------------------------------------------------------------------------------------------------------
//	long TGridData::InsertNewRow(long row,long tag,char* title)
//	タイプ：protected
//	機能　：行を挿入
//	引数１：挿入行
//	引数２：行タグ（省略可）
//	引数３：行タイトル（省略可）
//	戻り値：成功＝現在の行数 －１以下＝失敗
//---------------------------------------------------------------------------------------------------------------------
long TGridData::InsertNewRow(long row,long tag,char* title)
{
	int i,j;
	int len;

	if( row < 0 || row > m_LastRowNo || m_LastRowNo<0) return(-1);

	if( this->AddRow()<0 ) return(-2);

	for( i=m_LastRowNo; i > row; i-- ){
		m_StrData[i].Tag = m_StrData[i-1].Tag;
		memcpy(m_StrData[i].colTitle, m_StrData[i-1].colTitle, TITLELEN_MAX+1);
		for(j=0;j<m_TotalCols+1;j++){
			len = strlen(m_StrData[i-1].d[j]);
			free(m_StrData[i].d[j]);
			m_StrData[i].d[j]=(char*)malloc(len+1);
			memset(m_StrData[i].d[j], 0, len+1 );
			memcpy(m_StrData[i].d[j],m_StrData[i-1].d[j],len);
			m_StrData[i].ncolor[j]	= m_StrData[i-1].ncolor[j];
			m_StrData[i].nbkcolor[j]= m_StrData[i-1].nbkcolor[j];
			m_StrData[i].celTag[j]	= m_StrData[i-1].celTag[j];
		}
	}

	m_StrData[row].Tag = tag;
	memset(m_StrData[row].colTitle, 0, TITLELEN_MAX+1);
	if( NULL != title)
		memcpy(m_StrData[row].colTitle, title, (int)strlen(title)>TITLELEN_MAX ? TITLELEN_MAX:strlen(title));
	for(j=0; j < m_TotalCols+1;j++){
		free(m_StrData[row].d[j]);
		m_StrData[row].d[j]=(char*)malloc(1);
		memset(m_StrData[row].d[j], 0, 1 );
		m_StrData[row].nbkcolor[j]=RGB(255,255,255);
		m_StrData[row].ncolor[j]=0;
		m_StrData[row].celTag[j]=0;
	}
	return(m_LastRowNo+1);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::SortRow(long col);
//	タイプ：protected
//	機能　：行並び替え
//	引数１：並び替え対象行
//	戻り値：成功＝TRUE,失敗=FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SortRow(long col)
{
	int i,j;
	int chgFlg=1;
	int cmplen;
	int len1,len2;

	char *tmpD;
	long tmpCelTag;
	COLORREF tmpncolor;
	COLORREF tmpbkcolor;

	this->SortColNo=col;	// ソートした列を覚えておく

	if(m_LastRowNo<2) return(FALSE);

	// 材料名の順番に並び替える
	while(chgFlg){
		chgFlg=0;
		for( i=0 ; i < m_LastRowNo ; i++ ){
			cmplen = strlen(m_StrData[i].d[col])>strlen(m_StrData[i+1].d[col] )?strlen(m_StrData[i].d[col]):strlen(m_StrData[i+1].d[col] );
			if ( memcmp( m_StrData[i].d[col] , m_StrData[i+1].d[col] , cmplen ) > 0 ){

				// 入れ替え
				for(j=0;j<m_TotalCols+1;j++){
					// こっちのソースで問題ないと思うけど。。。。mallocで取得されるサイズはポインタの-4バイトから入る？
//					tmpD=m_StrData[i].d[j];
//					m_StrData[i].d[j]	= m_StrData[i+1].d[j];
//					m_StrData[i+1].d[j]=tmpD;


					len1=strlen(m_StrData[i].d[j]);
					len2=strlen(m_StrData[i+1].d[j]);		// 長さチェック

					tmpD = (char*)malloc(len1+1);
					memcpy( tmpD , m_StrData[i].d[j] , len1+1 );	// 一度退避

					free(m_StrData[i].d[j]);
					m_StrData[i].d[j]=(char*)malloc(len2+1);
					memset( m_StrData[i].d[j] , 0 , len2+1 );
					memcpy( m_StrData[i].d[j] , m_StrData[i+1].d[j] , len2+1 );	// コピー

					free(m_StrData[i+1].d[j]);
					m_StrData[i+1].d[j]=(char*)malloc(len1+1);
					memset( m_StrData[i+1].d[j] , 0 , len1+1 );
					memcpy( m_StrData[i+1].d[j] , tmpD , len1+1 );	// コピー

					free(tmpD);

					tmpncolor=m_StrData[i].ncolor[j];
					m_StrData[i].ncolor[j]	= m_StrData[i+1].ncolor[j];
					m_StrData[i+1].ncolor[j]=tmpncolor;

					tmpbkcolor=m_StrData[i].nbkcolor[j];
					m_StrData[i].nbkcolor[j]	= m_StrData[i+1].nbkcolor[j];
					m_StrData[i+1].nbkcolor[j]=tmpbkcolor;

					tmpCelTag=m_StrData[i].celTag[j];
					m_StrData[i].celTag[j]	= m_StrData[i+1].celTag[j];
					m_StrData[i+1].celTag[j]=tmpCelTag;
				}

				
				chgFlg=1;
			}
		}
	}
	return TRUE;
}

BOOL TGridData::SortRowQuick(long col)
{
	int i,j;
	int chgFlg=1;
	int cmplen;
	int ptr;

	char *tmpD;
	long tmpCelTag;
	COLORREF tmpncolor;
	COLORREF tmpbkcolor;

	this->SortColNo=col;	// ソートした列を覚えておく

	if(m_LastRowNo<2) return(FALSE);


	// 順番に並び替える
	for( ptr=0;ptr<m_LastRowNo-1;ptr++){
		// 一番小さい値を探す
		char *minv;
		char *chkv;
		int min_id;
		min_id=ptr;
		minv=m_StrData[min_id].d[col];
		for( i=ptr+1;i<m_LastRowNo;i++){
			chkv=m_StrData[i].d[col];
			cmplen = strlen(minv)>strlen(chkv )?strlen(minv):strlen(chkv);
			if ( memcmp( minv , chkv , cmplen ) > 0 ){
				minv=chkv;
				min_id=i;
			}
		}
		// 一番小さい値をptrに持ってくる
		// 以降はずらす
		for(j=0;j<m_TotalCols+1;j++){
			tmpD=m_StrData[min_id].d[j];
			tmpncolor=m_StrData[min_id].ncolor[j];
			tmpbkcolor=m_StrData[min_id].nbkcolor[j];
			tmpCelTag=m_StrData[min_id].celTag[j];
			for( i=min_id;i>ptr;i--){
				m_StrData[i].d[j]=		 m_StrData[i-1].d[j];
				m_StrData[i].ncolor[j]=	 m_StrData[i-1].ncolor[j];
				m_StrData[i].nbkcolor[j]=m_StrData[i-1].nbkcolor[j];
				m_StrData[i].celTag[j]=	 m_StrData[i-1].celTag[j];
			}
			m_StrData[i].d[j]=tmpD;
			m_StrData[i].ncolor[j]=tmpncolor;
			m_StrData[i].nbkcolor[j]=tmpbkcolor;
			m_StrData[i].celTag[j]=tmpCelTag;
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::ReSortRow(void);
//	タイプ：protected
//	機能　：行並び替え(前回と同じ条件で！)
//	引数１：無し
//	戻り値：成功＝TRUE,失敗=FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::ReSortRow(void)
{
	if( this->SortColNo < 0) return FALSE;
	return this->SortRow(this->SortColNo);
}
