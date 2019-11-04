#ifndef GRIDTDATA_H
#define GRIDTDATA_H

//========================================================================================
//
//	FileName	:GrdData3.h
//	Class		:TGridData
// 				:�O���b�h�E�C���h�E�E�f�[�^�N���Xversion 3
//	�C��		:2000.11.14 coba	�������g�p�ʂ̍팸�ׂ̈̑�����Ńo�[�W�����A�b�v
//									�񐔖�����
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

		// �s�̑}���E�폜
		long RowDelete(long row);
		long InsertNewRow(long row,long tag=0,char* title=NULL);

		// �^�O���擾
		long GetTag( long row){return(m_StrData[row].Tag);};
		long GetCelTag(long col,long row){return( m_StrData[row].celTag[col]);}
		BOOL SetCelTag(int col,int row, long data);
		BOOL SearchCelTag(long* col,long* row,long searchVal,long satrtcol,long startrow,long direction);

		// �F���
		COLORREF GetCelColEx(long col,long row){return( m_StrData[row].ncolor[col]);}
		COLORREF GetCelBkColEx(long col,long row){return( m_StrData[row].nbkcolor[col]);}
		
		// �Z���w�i�F�̕ύX
		BOOL SetCelBkColor(int col,int row, char color);
		BOOL SetCelBkColorEx(int col,int row, COLORREF color);

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
		BOOL SetCelDataEx(int col,int row, COLORREF color,long cTag,char* data,COLORREF bkcolor);

		BOOL SetCelData(int col,int row, char* data);	// Tag,�F�͂��̂܂�

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
//			char *color;				// moji�F	0=���A1=�� 2= ��
//			char *bkcolor;				// �w�i�F�F	0=���A1=�����F
			COLORREF *ncolor;
			COLORREF *nbkcolor;
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

		// �s���o���̕ύX
		BOOL ChangeRowTitle(long rowNo, char* newtitle);
		BOOL ChangeRowTitle2(long rowNo, char* newtitle);	//2003.05.11 Y.Kogai


		// ��w��A�s���ёւ�(����y���[�h�j
		BOOL SortRow(long col);
		BOOL ReSortRow(void);
		BOOL SortRowQuick(long col);

	private:
		int SortColNo;			// �O��\�[�g��������o���Ă���


	private:

		
		// GetCelEx()�p�̃o�b�t�@
		char *m_retBuf;

};
#endif

