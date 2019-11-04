//========================================================================================
//
//	FileName	:GridData.cpp
//	Class		:TListaData
// 				:�O���b�h�E�C���h�E�E�f�[�^�N���X
//	�쐬��		:1999.07.30	Coded by coba
//	�C��		:1999.09.06 coba	�s���𖳐����Ɂi���܂ł͍ő�T�O�j
//									�s�w�b�_�@�\�ǉ�
//									���X�N���[���@�\�ǉ�
//========================================================================================
#include "grddata4.h"

//---------------------------------------------------------------------------------------------------------------------
//	TGridData::TGridData(long totalcol,BOOL title=FALSE)
//	�^�C�v�Fprivate
//	�@�\�@�F�R���X�g���N�^�B�e�평����
//	�����P�F�s�̃u���b�N�T�C�Y�i���̒P�ʂŃ��������A���P�[�g����j
//	�����Q�F�񐔂̏����l
//	�����R�F
//---------------------------------------------------------------------------------------------------------------------
TGridData::TGridData(long rowBlock,long totalcol, BOOL title)
{
	int i,j;

	m_TotalCols=totalcol;


	// �s�̍��v
	m_LastRowNo=-1;

	// �w�b�_�̂���Ȃ�
	m_RowTitleEnable=title;

	m_AllocBlock = rowBlock;				// �A���P�[�g����P��
	if( m_AllocBlock < 1 ) m_AllocBlock = 20;


	// �sobject���P�u���b�N�A���P�[�g����
	LISTSTRDATA *tmp=NULL;
	m_StrData = NULL;


	// allocate
	m_StrData = (LISTSTRDATA*)malloc(sizeof(LISTSTRDATA) * m_AllocBlock);

	if(m_StrData==NULL) return;
	m_AllocRows = m_AllocBlock;				// �A���P�[�g�ςݍs��

	m_Title = (char**)malloc( sizeof(char*)*(totalcol+1) );
	for(i=0;i<totalcol+1;i++){
		m_Title[i]=(char*)malloc( sizeof(char) * (TITLELEN_MAX+1) );
		memset( m_Title[i], 0, TITLELEN_MAX+1 );
	}

	// �e�s�̃f�[�^�̈���A���P�[�g����
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
			m_StrData[i].nbkcolor[j] = RGB(255,255,255);	// �����l�͔�
			m_StrData[i].celTag[j] = 0;
		}

	}

	// �R�s�[�o�b�t�@��
	m_CopyBufFull =FALSE;
	m_retBuf=NULL;

	this->SortColNo=-1;
}
//---------------------------------------------------------------------------------------------------------------------
//	TGridData::TGridData(long totalcol)
//	�^�C�v�Fprivate
//	�@�\�@�F�f�X�g���N�^�B���������
//---------------------------------------------------------------------------------------------------------------------
TGridData::~TGridData()
{
	int i,j;

	if( m_StrData != NULL){


		// �e�s�̃f�[�^�̈���������
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

		// ���̈���
		for(i=0;i<m_TotalCols+1;i++)
			free(m_Title[i]);

		free( m_Title);
	}
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::SetTitle(int col,char* title)
//	�^�C�v�Fprotected
//	�@�\�@�F��w�b�_�̃f�[�^�ݒ�
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F��̑�
//	�߂�l�F������TRUE
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
//	�^�C�v�Fprotected
//	�@�\�@�F��w�b�_�̃f�[�^�ݒ�
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F��̑�
//	�߂�l�F������TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::GetTitle(int col,char* buf,int bufsize)
{
	if( col >= TGridData::m_TotalCols || col < 0) return(FALSE);

	memcpy( buf,m_Title[col],  (int)strlen(m_Title[col])>bufsize ? bufsize:strlen(m_Title[col]) );
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::SetCel(int col,int row, long cTag,char* data)
//	�^�C�v�Fprotected
//	�@�\�@�F�Z���Ƀf�[�^��ݒ�
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F�ݒ肷��s�ԍ�
//	�����R�F�ݒ�f�[�^(�F�j
//	�����R�F�ݒ�f�[�^(long�l)
//	�����S�F�ݒ�f�[�^(����)
//	�����T�F�Z���̔w�i�F 0=���A1=�����F -1=�ύX�Ȃ��i�O�̐F���g�p����j
//	�߂�l�F������TRUE
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
//	�^�C�v�Fprotected
//	�@�\�@�F�Z���Ƀf�[�^��ݒ�
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F�ݒ肷��s�ԍ�
//	�����R�F�ݒ�f�[�^(����)
//	�߂�l�F������TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SetCelData(int col,int row,char* data)
{
	return(SetCelDataEx(col,row,GetCelColEx(col,row),GetCelTag(col,row),data,GetCelBkColEx(col,row)));
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::SetCel(int col,int row, long cTag,char* data)
//	�^�C�v�Fprotected
//	�@�\�@�F�Z���Ƀf�[�^��ݒ�
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F�ݒ肷��s�ԍ�
//	�����R�F�ݒ�f�[�^(�F�j
//	�����R�F�ݒ�f�[�^(long�l)
//	�����S�F�ݒ�f�[�^(����)
//	�����T�F�Z���̔w�i�F 0=���A1=�����F -1=�ύX�Ȃ��i�O�̐F���g�p����j
//	�߂�l�F������TRUE
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
//	�^�C�v�Fpublic
//	�@�\�@�F�Z���Ƀf�[�^��ݒ�
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F�ݒ肷��s�ԍ�
//	�����R�F�ݒ�f�[�^
//	�߂�l�F������TRUE
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
//	�^�C�v�Fpublic
//	�@�\�@�F�Z���ɔw�i�F�̃Z�b�g
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F�ݒ肷��s�ԍ�
//	�����R�F�O�����@�P���D�F
//	�߂�l�F������TRUE
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
//	�^�C�v�Fpublic
//	�@�\�@�F�Z���ɔw�i�F�̃Z�b�g
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F�ݒ肷��s�ԍ�
//	�����R�F�O�����@�P���D�F
//	�߂�l�F������TRUE
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
//	�^�C�v�Fpublic
//	�@�\�@�F�Z���^�O�̌���
//	�����P�F����������ԍ�
//	�����Q�F���������s�ԍ�
//	�����R�F�����f�[�^
//	�����S�F�����J�n��
//	�����T�F�����J�n�s
//	�����U�F�������� 0=���i�Ō�̃Z���܂Ō����j�A1=�c(�w�����̂݌���)
//	�߂�l�F������TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::SearchCelTag(long* col,long* row,long searchVal,long startcol,long startrow,long direction)
{
	// �����G���[
	if( startcol >= m_TotalCols || startcol < 0) return(FALSE);
	if( startrow > m_LastRowNo || startrow < 0) return(FALSE);

	int i,j,k;

	// ����������
	if( !direction ){
		for( i= startrow; i <= m_LastRowNo;i++){		// ���̍s��
			// �ꏄ�ڂ�startcol�̎�����
			if( i==startrow) k = startcol+1;
			else k=0;

			// �r���r�ߌ���
			for( j=k; j< m_TotalCols;j++){
				// ����
				if(m_StrData[i].celTag[j] == searchVal){
					*col = j;*row=i;
					return(TRUE);
				}
			}
		}
		// ������
		return(FALSE);
	}

	// �c��������
	else{
		// �r���r�ߌ���
		for( j=startrow+1; j<= m_LastRowNo;j++){
			// ����
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
//	�^�C�v�Fprotected
//	�@�\�@�F�V�K�ɍs��ǉ�
//	�����P�F�s�ɂ���^�O(�ȗ���)
//	�߂�l�F�������ǉ������s�̔ԍ��A�|�P�ȉ������s
//---------------------------------------------------------------------------------------------------------------------
long TGridData::AddRow(long uTag)
{

	int i,j;

	m_LastRowNo++;

	// �A���P�[�g�ςݍs���ő���Ȃ��Ƃ��͊m��
	if( m_LastRowNo >= m_AllocRows ){
		LISTSTRDATA *tmp=NULL;
		// allocate
		m_AllocRows += m_AllocBlock;
		tmp = (LISTSTRDATA*)realloc(m_StrData, sizeof(LISTSTRDATA) * m_AllocRows);

		if(NULL==tmp) return(-1);

		m_StrData = tmp;
		// �f�[�^�̈�̃A���P�[�g
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
//	�^�C�v�Fprotected
//	�@�\�@�F�V�K�ɍs��ǉ�
//	�����P�F�s�ɂ���^�O(�ȗ���)
//	�߂�l�F�������ǉ������s�̔ԍ��A�|�P�ȉ������s
//---------------------------------------------------------------------------------------------------------------------
long TGridData::AddRow(char *colTitle,long uTag)
{
	m_LastRowNo++;
	int i,j;

	// �A���P�[�g�ςݍs���ő���Ȃ��Ƃ��͊m��
	if( m_LastRowNo >= m_AllocRows ){
		LISTSTRDATA *tmp=NULL;
		// allocate
		m_AllocRows += m_AllocBlock;
		tmp = (LISTSTRDATA*)realloc(m_StrData, sizeof(LISTSTRDATA) * m_AllocRows);

		if(NULL==tmp) return(-1);

		m_StrData = tmp;

		// �f�[�^�̈�̃A���P�[�g
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
//	�^�C�v�Fprotected
//	�@�\�@�F�s��̕ύX
//	�����P�F�ύX����s
//	�����Q�F�ύX��̍s��
//	�߂�l�F������TRUE
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
//	�^�C�v�Fprotected
//	�@�\�@�F�s��̕ύX
//	�����P�F�ύX����s
//	�����Q�F�ύX��̍s��
//	�߂�l�F������TRUE
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
//	�^�C�v�Fprotected
//	�@�\�@�F�s��̕ύX
//	�����P�F�ύX����s
//	�����Q�F�ύX��̍s��
//	�߂�l�F������TRUE
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
//	�^�C�v�Fpublic
//	�@�\�@�F�w�肵���s���L�������ׂ�
//	�����P�F��ԍ�
//	�����Q�F�s�ԍ�
//	�߂�l�F�L����TRUE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::IsCelEnable( int col, int row)
{
	if( col >= m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::GetCel(char* buf, int bufsize,int col,int row )
//	�^�C�v�Fpublic
//	�@�\�@�F�Z������f�[�^���擾
//	�����P�F�擾�o�b�t�@
//	�����Q�F�擾�o�b�t�@�T�C�Y
//	�����R�F�擾�����ԍ�
//	�����S�F�擾����s�ԍ�
//	�߂�l�F������TRUE
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
//	�^�C�v�Fpublic
//	�@�\�@�F�Z������f�[�^���擾���ăf�[�^�|�C���^��Ԃ�
//	�����R�F�擾�����ԍ�
//	�����S�F�擾����s�ԍ�
//	�߂�l�F�������f�[�^�|�C���^ ���s=NULL
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
//	�^�C�v�Fprotected
//	�@�\�@�F�w���̃f�[�^��S�ăo�b�t�@�ɃR�s�[����
//	�����P�F�R�s�[�Ώۂ̗�ԍ�
//	�����Q�F�폜����ꍇ=TRUE
//	�߂�l�F������TRUE�A���s��FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::ColCopy( long targetCol ,BOOL cut)
{
	// �����`�F�b�N
	if( targetCol < 0 || targetCol >= m_TotalCols || m_TotalCols <=0 || m_LastRowNo < 0)
		return(FALSE);

	int i,len,j;
//	char* newpt;
	COLORREF* newpt;
	char**newstr;
	long *newtag;

	// �o�b�t�@�ɃR�s�[
//	for( i= 0; i <= m_LastRowNo ; i++ ){
	for(i=0; i<m_AllocRows;i++){
		// �R�s�[�o�b�t�@�̗̈���m�ۂ���
		if( m_StrData[i].d[m_TotalCols] !=NULL){
			free(m_StrData[i].d[m_TotalCols]);
			m_StrData[i].d[m_TotalCols]=NULL;
		}

		if( m_StrData[i].d[targetCol] ==NULL )len=0;
		else len = strlen(m_StrData[i].d[targetCol]);
		m_StrData[i].d[m_TotalCols] = (char*)malloc(sizeof( char) * (len+1) );

		// �����f�[�^�ƃZ�������R�s�[����
		memset( m_StrData[i].d[m_TotalCols],0,len+1 );
		memcpy( m_StrData[i].d[m_TotalCols], m_StrData[i].d[targetCol], len );
		m_StrData[i].celTag[m_TotalCols] = m_StrData[i].celTag[targetCol];
//		m_StrData[i].color[m_TotalCols] = m_StrData[i].color[targetCol];
//		m_StrData[i].bkcolor[m_TotalCols] = m_StrData[i].bkcolor[targetCol];
		m_StrData[i].ncolor[m_TotalCols] = m_StrData[i].ncolor[targetCol];
		m_StrData[i].nbkcolor[m_TotalCols] = m_StrData[i].nbkcolor[targetCol];

		// �폜����ꍇ�̓����������炷
		if( cut ){
			// �Z���f�[�^
			for( j=targetCol;j < m_TotalCols; j++ )
				m_StrData[i].d[j]=m_StrData[i].d[j+1];
			newstr = (char**)realloc(m_StrData[i].d, sizeof(char*) * (m_TotalCols) );
			m_StrData[i].d=newstr;

			// �Z��1�^�O
			memmove( &m_StrData[i].celTag[targetCol], &m_StrData[i].celTag[targetCol+1],
					(m_TotalCols-targetCol) * sizeof(long) );
			newtag = (long*)realloc(m_StrData[i].celTag, sizeof(long)*(m_TotalCols));
			m_StrData[i].celTag=newtag;

//			// �F
//			memmove( &m_StrData[i].color[targetCol], &m_StrData[i].color[targetCol+1],
//					(m_TotalCols-targetCol) * sizeof(char) );
//			newpt = (char*)realloc(m_StrData[i].color, m_TotalCols);
//			m_StrData[i].color=newpt;
//			// �F
//			memmove( &m_StrData[i].bkcolor[targetCol], &m_StrData[i].bkcolor[targetCol+1],
//					(m_TotalCols-targetCol) * sizeof(char) );
//			newpt = (char*)realloc(m_StrData[i].bkcolor, m_TotalCols);
//			m_StrData[i].bkcolor=newpt;

			// �F
			memmove( &m_StrData[i].ncolor[targetCol], &m_StrData[i].ncolor[targetCol+1],
					(m_TotalCols-targetCol) * sizeof(COLORREF) );
			newpt = (COLORREF*)realloc(m_StrData[i].ncolor, sizeof(COLORREF)*m_TotalCols);
			m_StrData[i].ncolor=newpt;
			// �F
			memmove( &m_StrData[i].nbkcolor[targetCol], &m_StrData[i].nbkcolor[targetCol+1],
					(m_TotalCols-targetCol) * sizeof(COLORREF) );
			newpt = (COLORREF*)realloc(m_StrData[i].nbkcolor, sizeof(COLORREF)*m_TotalCols);
			m_StrData[i].nbkcolor=newpt;
		
		}
	}

	// ��^�C�g��
	memcpy( m_Title[m_TotalCols], m_Title[targetCol], TITLELEN_MAX+1 );
	if( cut ){
		for( j=targetCol;j < m_TotalCols; j++ )		m_Title[j]=m_Title[j+1];
		//free(m_Title[m_TotalCols]);

		newstr = (char**)realloc( m_Title, sizeof(char*) * (m_TotalCols) );
		m_Title = newstr;

		m_TotalCols --;
//		memset( m_Title[m_TotalCols-1], 0, TITLELEN_MAX+1  );
	}

	// �R�s�[�o�b�t�@OK
	m_CopyBufFull =TRUE;
	return(TRUE);
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::ColPaste( long targetCol,BOOL insert)
//	�^�C�v�Fprotected
//	�@�\�@�F�o�b�t�@�̓��e���w�肳�ꂽ��ɃR�s�[����
//	�����P�F�R�s�[��̗�ԍ�
//	�߂�l�F���������ۂɃR�s�[������ԍ��A���s=-1
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

	// �����`�F�b�N
	if( m_LastRowNo < 0)		return(-1);

	// �񂪖����ꍇ�́A�V�K�ɗ���쐬����
	if( targetCol < 0 ) targetCol = 0;

	if( m_TotalCols < 0 ){
		m_TotalCols =1;
		targetCol = 0;
		prevCols=0;
		allocNew = TRUE;
	}
	// �V�K�ɗ�̒ǉ����K�v�ȏꍇ
	else if( targetCol >= m_TotalCols){
		prevCols=m_TotalCols;
		// �̈�A���P�[�g
		m_TotalCols=targetCol+1;
		allocNew = TRUE;
	}

	// --------------------------------------�@�V�K�ɗ��ǉ�����ꍇ-------------------------------------------
	if( allocNew == TRUE ){
		// �̈�A���P�[�g
		m_TotalCols=targetCol+1;
		for(i=0; i<m_AllocRows;i++){
//		for( i= 0; i <= m_LastRowNo ; i++ ){
			// �Z���f�[�^�̈�
			newstr = (char**)realloc(m_StrData[i].d, sizeof(char*) * (m_TotalCols+1) );
			m_StrData[i].d = newstr;
			newtag = (long*)realloc(m_StrData[i].celTag, sizeof(long) * (m_TotalCols+1) );
			m_StrData[i].celTag=newtag;
			newpt = (COLORREF*)realloc(m_StrData[i].ncolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].ncolor=newpt;
			newpt = (COLORREF*)realloc(m_StrData[i].nbkcolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].nbkcolor=newpt;

			// �ǉ������̈������������
			for(j=prevCols+1; j < m_TotalCols+1;j++){
				m_StrData[i].d[j]=(char*)malloc(1);
				memset(m_StrData[i].d[j], 0, 1 );
//				m_StrData[i].bkcolor[j]=0;
//				m_StrData[i].color[j]=0;
				m_StrData[i].nbkcolor[j]=RGB(255,255,255);
				m_StrData[i].ncolor[j]=0;
				m_StrData[i].celTag[j]=0;
			}

			// �A���P�[�g�O�R�s�[�o�b�t�@�̓��e��ړI�̗�ɃR�s�[����
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

			// �A���P�[�g�O�R�s�[�o�b�t�@�̓��e��V�K�̃R�s�[�o�b�t�@�ɃR�s�[����
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

			// �A���P�[�g�O�̃R�s�[�o�b�t�@���N���A����
			if( m_StrData[i].d[prevCols] !=NULL)  free( m_StrData[i].d[prevCols]);
			m_StrData[i].d[prevCols]=(char*)malloc( sizeof(char));
			m_StrData[i].d[prevCols][0] = 0;

//			m_StrData[i].bkcolor[prevCols] = 0;
//			m_StrData[i].color[prevCols] = 0;
			m_StrData[i].nbkcolor[prevCols] = RGB(255,255,255);
			m_StrData[i].ncolor[prevCols] = 0;
			m_StrData[i].celTag[prevCols] = 0;
		}

		// ���̈�
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


	// -------------------------------------------�A�}������ꍇ-------------------------------------------
	if( insert) m_TotalCols++;
	// -------------------------------------------�B�����̗�ɏ㏑������ꍇ-------------------------------------------
	for(i=0; i<m_AllocRows;i++){
//	for( i= 0; i <= m_LastRowNo ; i++ ){
		// �}������ꍇ�͗̈�A���P�[�g���Ē���
		if( insert ){
			// �Z���f�[�^�̈�
			newstr = (char**)realloc(m_StrData[i].d, sizeof(char*) * (m_TotalCols+1) );
			m_StrData[i].d = newstr;
			newtag = (long*)realloc(m_StrData[i].celTag, sizeof(long) * (m_TotalCols+1) );
			m_StrData[i].celTag=newtag;
			newpt = (COLORREF*)realloc(m_StrData[i].ncolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].ncolor=newpt;
			newpt = (COLORREF*)realloc(m_StrData[i].nbkcolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].nbkcolor=newpt;

			// �f�[�^���E�����ɃV�t�g����
			// �Z���f�[�^
			for( j=m_TotalCols;j > targetCol; j-- )	m_StrData[i].d[j]=m_StrData[i].d[j-1];

			// �Z���^�O
			memmove( &m_StrData[i].celTag[targetCol+1], &m_StrData[i].celTag[targetCol],
							(m_TotalCols-targetCol) * sizeof(long) );
//			// �F
//			memmove( &m_StrData[i].color[targetCol+1], &m_StrData[i].color[targetCol],
//							(m_TotalCols-targetCol) * sizeof(char) );
//			memmove( &m_StrData[i].bkcolor[targetCol+1], &m_StrData[i].bkcolor[targetCol],
//							(m_TotalCols-targetCol) * sizeof(char) );
			// �F
			memmove( &m_StrData[i].ncolor[targetCol+1], &m_StrData[i].ncolor[targetCol],
							(m_TotalCols-targetCol) * sizeof(COLORREF) );
			memmove( &m_StrData[i].nbkcolor[targetCol+1], &m_StrData[i].nbkcolor[targetCol],
							(m_TotalCols-targetCol) * sizeof(COLORREF) );
		}

		// �R�s�[�o�b�t�@�̓��e��ړI�̗�ɃR�s�[����
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

	// ���̈�
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

	// �����`�F�b�N
	if( m_LastRowNo < 0)		return(-1);
	// �񂪖����ꍇ�́A�V�K�ɗ���쐬����
	if( targetCol < 0 ) targetCol = 0;

	if( m_TotalCols < 0 ){
		m_TotalCols =1;
		targetCol = 0;
		prevCols=0;
		allocNew = TRUE;
	}
	// �V�K�ɗ�̒ǉ����K�v�ȏꍇ
	else if( targetCol >= m_TotalCols){
		prevCols=m_TotalCols;
		// �̈�A���P�[�g
		m_TotalCols=targetCol+1;
		allocNew = TRUE;
	}

	// --------------------------------------�@�V�K�ɗ��ǉ�����ꍇ-------------------------------------------
	if( allocNew == TRUE ){
		// �̈�A���P�[�g
		m_TotalCols=targetCol+1;
//		for( i= 0; i <= m_LastRowNo ; i++ ){
		for(i=0; i<m_AllocRows;i++){
			// �Z���f�[�^�̈�
			newstr = (char**)realloc(m_StrData[i].d, sizeof(char*) * (m_TotalCols+1) );
			m_StrData[i].d = newstr;
			newtag = (long*)realloc(m_StrData[i].celTag, sizeof(long) * (m_TotalCols+1) );
			m_StrData[i].celTag=newtag;
			newpt = (COLORREF*)realloc(m_StrData[i].ncolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].ncolor=newpt;
			newpt = (COLORREF*)realloc(m_StrData[i].nbkcolor, sizeof(COLORREF)*(m_TotalCols+1));
			m_StrData[i].nbkcolor=newpt;

			// �ǉ������̈������������
			for(j=prevCols+1; j < m_TotalCols+1;j++){
				m_StrData[i].d[j]=(char*)malloc(1);
				memset(m_StrData[i].d[j], 0, 1 );
//				m_StrData[i].bkcolor[j]=0;
//				m_StrData[i].color[j]=0;
				m_StrData[i].nbkcolor[j]=RGB(255,255,255);
				m_StrData[i].ncolor[j]=0;
				m_StrData[i].celTag[j]=0;
			}

			// �A���P�[�g�O�R�s�[�o�b�t�@�̓��e��V�K�̃R�s�[�o�b�t�@�ɃR�s�[����
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

			// �A���P�[�g�O�̃R�s�[�o�b�t�@���N���A����
			if( m_StrData[i].d[prevCols] !=NULL)  free(m_StrData[i].d[prevCols]);
			m_StrData[i].d[prevCols] = (char*)malloc(1);
			memset(m_StrData[i].d[prevCols],0,1);

//			m_StrData[i].bkcolor[prevCols] = 0;
//			m_StrData[i].color[prevCols] = 0;
			m_StrData[i].nbkcolor[prevCols] = RGB(255,255,255);
			m_StrData[i].ncolor[prevCols] = 0;
			m_StrData[i].celTag[prevCols] = 0;
		}

		// ���̈�
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
		// �}������ꍇ�͗̈�A���P�[�g���Ē���
		// �Z���f�[�^�̈�
		newstr = (char**)realloc(m_StrData[i].d, sizeof(char*) * (m_TotalCols+1) );
		m_StrData[i].d = newstr;
		newtag = (long*)realloc(m_StrData[i].celTag, sizeof(long) * (m_TotalCols+1) );
		m_StrData[i].celTag=newtag;
		newpt = (COLORREF*)realloc(m_StrData[i].ncolor, sizeof(COLORREF)*(m_TotalCols+1));
		m_StrData[i].ncolor=newpt;
		newpt = (COLORREF*)realloc(m_StrData[i].nbkcolor, sizeof(COLORREF)*(m_TotalCols+1));
		m_StrData[i].nbkcolor=newpt;

		// �f�[�^���E�����ɃV�t�g����
		// �Z���f�[�^
		for( j=m_TotalCols;j > targetCol; j-- )	m_StrData[i].d[j]=m_StrData[i].d[j-1];

		// �Z���^�O
		memmove( &m_StrData[i].celTag[targetCol+1], &m_StrData[i].celTag[targetCol],
							(m_TotalCols-targetCol) * sizeof(long) );
//		// �F
//		memmove( &m_StrData[i].color[targetCol+1], &m_StrData[i].color[targetCol],
//							(m_TotalCols-targetCol) * sizeof(char) );
//		memmove( &m_StrData[i].bkcolor[targetCol+1], &m_StrData[i].bkcolor[targetCol],
//							(m_TotalCols-targetCol) * sizeof(char) );
		// �F
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

	// ���̈�
	newstr = (char**)realloc( m_Title, sizeof(char*) * (m_TotalCols+1) );
	m_Title = newstr;
	for( j=m_TotalCols;j > targetCol; j-- ) m_Title[j]=m_Title[j-1];
	m_Title[targetCol]=(char*)malloc(TITLELEN_MAX+1);
	memset( m_Title[targetCol], 0,TITLELEN_MAX+1);

	return(targetCol);


}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL TGridData::DataClear(void)
//	�^�C�v�Fprivate
//	�@�\�@�F���ݕێ����Ă���S�f�[�^���N���A����
//	�߂�l�FTRUE=�N���A���{
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::DataClear(void)
{
	if( m_LastRowNo < 0 ) return(FALSE);			// 1�s���ێ����Ă��Ȃ�
	if( m_TotalCols < 1 ) return(FALSE);			// 1����ێ����Ă��Ȃ�

	int i,j;

	for( i= 0; i <= m_LastRowNo ; i++ ){
		// �f�[�^�̈������������
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
//	�^�C�v�Fprotected
//	�@�\�@�F�s���폜
//	�����P�F�폜�s
//	�߂�l�F���������݂̍s�� �|�P�ȉ������s
//---------------------------------------------------------------------------------------------------------------------
long TGridData::RowDelete(long row)
{
	int i,j;
	int len;

	if( row > m_LastRowNo ) return -1;

	// 1�s�����炷
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

	// �폜�����ŏI�s���N���A
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
//	�^�C�v�Fprotected
//	�@�\�@�F�s��}��
//	�����P�F�}���s
//	�����Q�F�s�^�O�i�ȗ��j
//	�����R�F�s�^�C�g���i�ȗ��j
//	�߂�l�F���������݂̍s�� �|�P�ȉ������s
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
//	�^�C�v�Fprotected
//	�@�\�@�F�s���ёւ�
//	�����P�F���ёւ��Ώۍs
//	�߂�l�F������TRUE,���s=FALSE
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

	this->SortColNo=col;	// �\�[�g��������o���Ă���

	if(m_LastRowNo<2) return(FALSE);

	// �ޗ����̏��Ԃɕ��ёւ���
	while(chgFlg){
		chgFlg=0;
		for( i=0 ; i < m_LastRowNo ; i++ ){
			cmplen = strlen(m_StrData[i].d[col])>strlen(m_StrData[i+1].d[col] )?strlen(m_StrData[i].d[col]):strlen(m_StrData[i+1].d[col] );
			if ( memcmp( m_StrData[i].d[col] , m_StrData[i+1].d[col] , cmplen ) > 0 ){

				// ����ւ�
				for(j=0;j<m_TotalCols+1;j++){
					// �������̃\�[�X�Ŗ��Ȃ��Ǝv�����ǁB�B�B�Bmalloc�Ŏ擾�����T�C�Y�̓|�C���^��-4�o�C�g�������H
//					tmpD=m_StrData[i].d[j];
//					m_StrData[i].d[j]	= m_StrData[i+1].d[j];
//					m_StrData[i+1].d[j]=tmpD;


					len1=strlen(m_StrData[i].d[j]);
					len2=strlen(m_StrData[i+1].d[j]);		// �����`�F�b�N

					tmpD = (char*)malloc(len1+1);
					memcpy( tmpD , m_StrData[i].d[j] , len1+1 );	// ��x�ޔ�

					free(m_StrData[i].d[j]);
					m_StrData[i].d[j]=(char*)malloc(len2+1);
					memset( m_StrData[i].d[j] , 0 , len2+1 );
					memcpy( m_StrData[i].d[j] , m_StrData[i+1].d[j] , len2+1 );	// �R�s�[

					free(m_StrData[i+1].d[j]);
					m_StrData[i+1].d[j]=(char*)malloc(len1+1);
					memset( m_StrData[i+1].d[j] , 0 , len1+1 );
					memcpy( m_StrData[i+1].d[j] , tmpD , len1+1 );	// �R�s�[

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

	this->SortColNo=col;	// �\�[�g��������o���Ă���

	if(m_LastRowNo<2) return(FALSE);


	// ���Ԃɕ��ёւ���
	for( ptr=0;ptr<m_LastRowNo-1;ptr++){
		// ��ԏ������l��T��
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
		// ��ԏ������l��ptr�Ɏ����Ă���
		// �ȍ~�͂��炷
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
//	�^�C�v�Fprotected
//	�@�\�@�F�s���ёւ�(�O��Ɠ��������ŁI)
//	�����P�F����
//	�߂�l�F������TRUE,���s=FALSE
//---------------------------------------------------------------------------------------------------------------------
BOOL TGridData::ReSortRow(void)
{
	if( this->SortColNo < 0) return FALSE;
	return this->SortRow(this->SortColNo);
}
