#ifndef GRIDTDATA_H
#define GRIDTDATA_H

//========================================================================================
//
//	FileName	:GrdData3.h
//	Class		:TGridData
// 				:�O���b�h�E�C���h�E�E�f�[�^�N���Xversion 3
//	�C��		:2000.11.14 coba	�������g�p�ʂ̍팸�ׂ̈̑�����Ńo�[�W�����A�b�v
//									�񐔖�����
//	�C��		:2002.10.24 coba	�s�̑}���E�폜�@�\��ǉ�
//	�C��		:2003.05.23 coba	�s�̍폜���ɁA�ŏI�s���폜����Ȃ��o�O���
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>

#define STRLEN_MAX 100
#define TITLELEN_MAX 40
#define ROW_DEFAULT 256

class TGridData
{
	public:
		TGridData(long rowblock,long totalcol,BOOL title=FALSE);
		~TGridData();

		// �^�C�g���`��
		BOOL SetTitle(int col,char* titleString);
		BOOL GetTitle(int col,char* buf,int bufsize);

		// �f�[�^��� 
		BOOL GetCel(char* buf, unsigned int bufsize,int col,int row );
		char *GetCelEx(int col,int row);

		long GetActiveRow(void);
		// �s�ǉ�
		long AddRow(long uTag=0);
		long AddRow(char* colTitle, long uTag=0);


		// �^�O���擾
		long GetTag( long row){return(m_StrData[row].Tag);};
		long GetCelTag(long col,long row){return( m_StrData[row].celTag[col]);}
		BOOL SetCelTag(int col,int row, long data);
		BOOL SearchCelTag(long* col,long* row,long searchVal,long satrtcol,long startrow,long direction);

		// ���݂̍s�̍��v��Ԃ�
		long GetTotalRows(void){return(m_LastRowNo+1);}
		long GetTotalCols(void){return(m_TotalCols);}

		long m_AllocBlock;				// �A���P�[�g����P��

		// �w�肵���s���L�����𔻒�
		BOOL IsCelEnable( int col, int row);

	protected:
		// �s�w�b�_�̂���Ȃ�
		BOOL m_RowTitleEnable;

		// View���Q��
		BOOL SetCelData(int col,int row, char color,long cTag,char* data,char bkcolor);

		long m_LastRowNo;			// �ێ����Ă���s�̍��v-1
		long m_TotalCols;

		// ���o���i�[
//		char m_Title[COL_LIMIT+1][TITLELEN_MAX+1];		// +1 �̓R�s�[�p�o�b�t�@
		char **m_Title;

		// �f�[�^�i�[�̈�(��P��)
		typedef struct{
			char** d;
			long *celTag;
			long Tag;								// ���[�U�p
			char colTitle[TITLELEN_MAX+1];			// �s��i���O�ƈ�v���ĂȂ��I�I���߂�Ȃ����j
			char *color;				// moji�F	0=���A1=�� 2= ��
			char *bkcolor;				// �w�i�F�F	0=���A1=�����F
		}LISTSTRDATA;


		LISTSTRDATA *m_StrData;
		long m_AllocRows;				// �A���P�[�g�ςݍs��


		// �V�K��̑}��
		long AddCol(long targetCol);

		// ��R�s�[,�y�[�X�g
		BOOL ColCopy( long targetCol ,BOOL cut);
		long ColPaste( long targetCol,BOOL insert);

		BOOL m_CopyBufFull;

		BOOL DataClear(void);

		// �Z���w�i�F�̕ύX
		BOOL SetCelBkColor(int col,int row, char color);

		// �s���o���̕ύX
		BOOL ChangeRowTitle(long rowNo, char* newtitle);

		// �s�̑}���E�폜
		long RowDelete(long row);
		long InsertNewRow(long row,long tag=0,char* title=NULL);

	private:
		// GetCelEx()�p�̃o�b�t�@
		char *m_retBuf;

};
#endif

