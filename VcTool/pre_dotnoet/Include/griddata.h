#ifndef GRIDTDATA_H
#define GRIDTDATA_H

//========================================================================================
//
//	FileName	:GridData.h
//	Class		:TGridData
// 				:�O���b�h�E�C���h�E�E�f�[�^�N���X
//	�쐬��		:1999.07.30	Coded by coba
//	�C��		:1999.09.06 coba	�s���𖳐����Ɂi���܂ł͍ő�T�O�j
//									�s�w�b�_�@�\�ǉ�
//									���X�N���[���@�\�ǉ�
//	�C��		:1999.09.09 coba	�����L�[�ł̃Z���Ԉړ��@�\�ǉ�
//									��P�ʂ̃R�s�[�E����t���@�\�ǉ�
//	�C��		:1999.09.10 coba	�s�ꊇ�I�����[�h��ǉ�
//									�s���̐�����p�~
//	�C��		:1999.09.10 coba	�P�s�P�ʂ̍s�A���P�[�g���u���b�N�P�ʂցi�x������j
//	�C��		:2000.06.07 coba	�s��̕ύX�@�\��ǉ�
//
//========================================================================================
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <winbase.h>

#define STRLEN_MAX 100
#define TITLELEN_MAX 40
#define COL_LIMIT 301
//#define COL_LIMIT 256
#define ROW_DEFAULT 256

class TGridData
{
	public:
		TGridData(long rowblock,long totalcol,BOOL title=FALSE);
		~TGridData();

		// �^�C�g���`��
		BOOL SetTitle(int col,char* titleString);

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

		long m_AllocBlock;				// �A���P�[�g����P��

	protected:
		// �s�w�b�_�̂���Ȃ�
		BOOL m_RowTitleEnable;

		// View���Q��
		BOOL SetCelData(int col,int row, char color,long cTag,char* data,char bkcolor);

		long m_LastRowNo;			// �ێ����Ă���s�̍��v-1
		long m_TotalCols;

		// ���o���i�[
		char m_Title[COL_LIMIT+1][TITLELEN_MAX+1];		// +1 �̓R�s�[�p�o�b�t�@

		// �f�[�^�i�[�̈�(��P��)
		typedef struct{
			char d[COL_LIMIT+1][STRLEN_MAX+1];
			long celTag[COL_LIMIT+1];
			long Tag;								// ���[�U�p
			char colTitle[TITLELEN_MAX+1];
			char color[COL_LIMIT+1];				// moji�F	0=���A1=�� 2= ��
			char bkcolor[COL_LIMIT+1];				// �w�i�F�F	0=���A1=�����F
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

	private:
		// GetCelEx()�p�̃o�b�t�@
		char m_retBuf[STRLEN_MAX+1];

};
#endif

