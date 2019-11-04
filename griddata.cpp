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
#include "griddata.h"

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
	if( totalcol > COL_LIMIT )
		m_TotalCols=COL_LIMIT;
	else
		m_TotalCols=totalcol;

	memset(m_Title, 0, sizeof(m_Title));

	// �s�̍��v
	m_LastRowNo=-1;

	// �w�b�_�̂���Ȃ�
	m_RowTitleEnable=title;

	m_AllocBlock = rowBlock;				// �A���P�[�g����P��
	if( m_AllocBlock < 1 ) m_AllocBlock = 20;


	// ��object���P�u���b�N�A���P�[�g����
	LISTSTRDATA *tmp=NULL;
	m_StrData = NULL;

	// allocate
	tmp = (LISTSTRDATA*)realloc(m_StrData, sizeof(LISTSTRDATA) * m_AllocBlock);

	if(NULL==tmp) return;
	m_StrData = tmp;
	m_AllocRows = m_AllocBlock;				// �A���P�[�g�ςݍs��

	// �R�s�[�o�b�t�@��
	m_CopyBufFull =FALSE;
}
//---------------------------------------------------------------------------------------------------------------------
//	TGridData::TGridData(long totalcol)
//	�^�C�v�Fprivate
//	�@�\�@�F�f�X�g���N�^�B���������
//---------------------------------------------------------------------------------------------------------------------
TGridData::~TGridData()
{
	if( m_StrData != NULL)
		free(m_StrData);
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
//	BOOL TGridData::SetCel(int col,int row, long cTag,char* data)
//	�^�C�v�Fprotected
//	�@�\�@�F�Z���Ƀf�[�^��ݒ�
//	�����P�F�ݒ肷���ԍ�
//	�����Q�F�ݒ肷��s�ԍ�
//	�����R�F�ݒ�f�[�^(�F�j
//	�����R�F�ݒ�f�[�^(long�l)
//	�����S�F�ݒ�f�[�^(����)
//	�����T�F�Z���̔w�i�F 0=���A1=�����F
//	�߂�l�F������TRUE
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
	if( col >= m_TotalCols || col < 0) return(FALSE);
	if( row > m_LastRowNo || row < 0) return(FALSE);

	m_StrData[row].bkcolor[col] = color;

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

	// �A���P�[�g�ςݍs���ő���Ȃ��Ƃ��͊m��
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
//	�^�C�v�Fprotected
//	�@�\�@�F�V�K�ɍs��ǉ�
//	�����P�F�s�ɂ���^�O(�ȗ���)
//	�߂�l�F�������ǉ������s�̔ԍ��A�|�P�ȉ������s
//---------------------------------------------------------------------------------------------------------------------
long TGridData::AddRow(char *colTitle,long uTag)
{
	// �A���P�[�g�ςݍs���ő���Ȃ��Ƃ��͊m��
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

	int i;

	// �o�b�t�@�ɃR�s�[
	for( i= 0; i <= m_LastRowNo ; i++ ){
		memset( m_StrData[i].d[COL_LIMIT],0,STRLEN_MAX+1 );
		memcpy( m_StrData[i].d[COL_LIMIT], m_StrData[i].d[targetCol], STRLEN_MAX+1 );
		m_StrData[i].celTag[COL_LIMIT] = m_StrData[i].celTag[targetCol];
		m_StrData[i].color[COL_LIMIT] = m_StrData[i].color[targetCol];
		// �폜����ꍇ
		if( cut ){
			// �Z���f�[�^
			memmove( m_StrData[i].d[targetCol], m_StrData[i].d[targetCol+1],
					(m_TotalCols-targetCol-1) * (STRLEN_MAX+1) );
			memset( m_StrData[i].d[m_TotalCols-1], 0, (STRLEN_MAX+1) );

			// �Z���^�O
			memmove( &m_StrData[i].celTag[targetCol], &m_StrData[i].celTag[targetCol+1],
					(m_TotalCols-targetCol-1) * sizeof(long) );
			m_StrData[i].celTag[m_TotalCols-1] =0;

			// �F
			memmove( &m_StrData[i].color[targetCol], &m_StrData[i].color[targetCol+1],
					(m_TotalCols-targetCol-1) * sizeof(char) );
			m_StrData[i].color[m_TotalCols-1] =0;
		}
	}

	// ��^�C�g��
	memcpy( m_Title[COL_LIMIT], m_Title[targetCol], TITLELEN_MAX+1 );
	if( cut ){
		// �Z���^�O
		memmove( &m_StrData[i].celTag[targetCol], &m_StrData[i].celTag[targetCol+1],
					(m_TotalCols-targetCol-1) * sizeof(long) );
		m_StrData[i].celTag[targetCol] = m_StrData[i].celTag[COL_LIMIT];
		// �Z���^�O
		memmove( &m_StrData[i].color[targetCol], &m_StrData[i].color[targetCol+1],
					(m_TotalCols-targetCol-1) * sizeof(char) );
		m_StrData[i].color[targetCol] = m_StrData[i].color[COL_LIMIT];

		memmove( m_Title[targetCol], m_Title[targetCol+1],	(m_TotalCols-targetCol-1) * (TITLELEN_MAX+1) );
		memset( m_Title[m_TotalCols-1], 0, TITLELEN_MAX+1  );
		m_TotalCols --;
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
	if( m_TotalCols > COL_LIMIT ) return(-1);
	if( !m_CopyBufFull ) return(-1);

	// �����`�F�b�N
	if( m_LastRowNo < 0)
		return(-1);

	// �񂪖����ꍇ�́A�V�K�ɗ���쐬���āA�㏑���ŏ�������
	if( targetCol < 0 ) targetCol = 0;
	if( m_TotalCols < 0 ){
		m_TotalCols =1;
		targetCol = 0;
		insert = FALSE;
	}

	int i,offset;

	// �㏑��or���炷�K�v�Ȃ�
	if( targetCol > m_TotalCols || !insert){
		if( targetCol > m_TotalCols)
				targetCol = m_TotalCols;

		if( targetCol >= COL_LIMIT ) return(-1);		// ����ȏ㖳��

		// �R�s�[����
		for( i= 0; i <= m_LastRowNo ; i++ ){
			memcpy( m_StrData[i].d[targetCol], m_StrData[i].d[COL_LIMIT], STRLEN_MAX+1  );
			m_StrData[i].celTag[targetCol] = m_StrData[i].celTag[COL_LIMIT];
			m_StrData[i].color[targetCol] = m_StrData[i].color[COL_LIMIT];
		}
		memcpy( m_Title[targetCol], m_Title[COL_LIMIT], TITLELEN_MAX+1  );

		// ��𑝂₵���ꍇ
		if( targetCol == m_TotalCols)
					m_TotalCols++;
		return(targetCol);
	}

	// ���炷�ꍇ�i��MAX�j
	if( m_TotalCols == COL_LIMIT ) offset =1;
	// !��max
	else{
		offset = 0;
	}

	// �R�s�[����
	for( i= 0; i <= m_LastRowNo ; i++ ){
		memmove( m_StrData[i].d[targetCol+1], m_StrData[i].d[targetCol],	(m_TotalCols-targetCol-offset) * (STRLEN_MAX+1) );
		memcpy( m_StrData[i].d[targetCol], m_StrData[i].d[COL_LIMIT], STRLEN_MAX+1  );

		// �Z���^�O
		memmove( &m_StrData[i].celTag[targetCol+1], &m_StrData[i].celTag[targetCol],
					(m_TotalCols-targetCol-offset) * sizeof(long) );
		m_StrData[i].celTag[targetCol] = m_StrData[i].celTag[COL_LIMIT];
		// �Z���^�O
		memmove( &m_StrData[i].color[targetCol+1], &m_StrData[i].color[targetCol],
					(m_TotalCols-targetCol-offset) * sizeof(char) );
		m_StrData[i].color[targetCol] = m_StrData[i].color[COL_LIMIT];
	}

	memmove( m_Title[targetCol+1], m_Title[targetCol],	(m_TotalCols-targetCol-offset) * (TITLELEN_MAX+1) );
	memcpy( m_Title[targetCol], m_Title[COL_LIMIT], TITLELEN_MAX+1  );

	// ��𑝂₷
	if( !offset ) 
		m_TotalCols++;

	return(targetCol);


}

long TGridData::AddCol( long targetCol )
{
	BOOL insert = TRUE;

	if( m_TotalCols > COL_LIMIT ) return(-1);

	// �����`�F�b�N
	if( m_LastRowNo < 0)		return(-1);

	// �񂪖����ꍇ�́A�V�K�ɗ���쐬���āA�㏑���ŏ�������
	if( targetCol < 0 ) targetCol = 0;
	if( m_TotalCols < 0 ){
		m_TotalCols =1;
		targetCol = 0;
		insert = FALSE;
	}

	int i,offset;

	// ���炷�K�v�Ȃ�
	if( targetCol > m_TotalCols || !insert){
		if( targetCol > m_TotalCols)	targetCol = m_TotalCols;

		if( targetCol >= COL_LIMIT ) return(-1);		// ����ȏ㖳��

		// �N���A����
		for( i= 0; i <= m_LastRowNo ; i++ )	memset( m_StrData[i].d[targetCol], 0, STRLEN_MAX+1  );
		memset( m_Title[targetCol],0 , TITLELEN_MAX+1  );

		// ��𑝂₵���ꍇ
		if( targetCol == m_TotalCols)	m_TotalCols++;
		return(targetCol);
	}

	// ���炷�ꍇ�i��MAX�j
	if( m_TotalCols == COL_LIMIT ) offset =1;
	// !��max
	else	offset = 0;

	// �N���A
	for( i= 0; i <= m_LastRowNo ; i++ ){
		memmove( m_StrData[i].d[targetCol+1], m_StrData[i].d[targetCol],	(m_TotalCols-targetCol-offset) * (STRLEN_MAX+1) );
		memset( m_StrData[i].d[targetCol], 0, STRLEN_MAX+1  );
		// �Z���^�O
		memmove( &m_StrData[i].celTag[targetCol+1], &m_StrData[i].celTag[targetCol],
					(m_TotalCols-targetCol-offset) * sizeof(long) );
		m_StrData[i].celTag[targetCol] = m_StrData[i].celTag[COL_LIMIT];
		// �Z���^�O
		memmove( &m_StrData[i].color[targetCol+1], &m_StrData[i].color[targetCol],
					(m_TotalCols-targetCol-offset) * sizeof(char) );
		m_StrData[i].color[targetCol] = m_StrData[i].color[COL_LIMIT];
	}

	memmove( m_Title[targetCol+1], m_Title[targetCol],	(m_TotalCols-targetCol-offset) * (TITLELEN_MAX+1) );
	memset( m_Title[targetCol],0 , TITLELEN_MAX+1  );
	memcpy( m_Title[targetCol],"??" , 2  );

	// ��𑝂₷
	if( !offset ) 		m_TotalCols++;

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

	memset( m_StrData, 0, (m_LastRowNo+1) * sizeof(LISTSTRDATA) );

	m_LastRowNo = -1;
	return(TRUE);
}



