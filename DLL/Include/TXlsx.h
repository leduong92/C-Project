// *verion 0.00
#ifndef TXLSX_H
#define TXLSX_H
//================================================================================================
//	FileName	:TXlsx.H
//	Class		:TXlsx 
// 				:TXls�N���X�݊���xlsx�`������N���X
//	�쐬��		:2013.12.04	Coded by coba
//------------------------------------------------------------------------------------------------
// Dinamic Link Library	:xlsxcrt.dll
// inport library		:xlsxcrt.lib	<-�����N�ɕK�v
//================================================================================================

#include "windows.h"
#include "xlsxCrt.h"

class TXls
{

	private:
		DWORD  m_xid;						// �t�@�C���n���h��(�n���h�����H)
		BOOL m_Open;
		long m_Err;
		void SetErrCode(void);

	public:
		TXls( void );
		~TXls( void );

		// �u�b�N�̃I�[�v���N���[�Y
		BOOL OpenXls(char* filename, BOOL overwrite, long sheetMax=3);
		BOOL OpenXlsByTemplate(char* filename, char* templatefile,BOOL overwrite);
		BOOL ReadXls(char* filename);
		void Close(BOOL save);
		BOOL AddSheet(long Page);
		BOOL DelSheet(long SheetNo,long Page);
		BOOL SetSheetName(char* SheetName);
		BOOL GetSheetName(long SheetNo,char* SheetName,int size);
		BOOL SetSheetNo(long Page);
		BOOL SetActiveSheet(long page);
		BOOL CopySheet( long posSource, long posInsert, LPCTSTR pszSheetName );	//2007.03.08 matsuda

		// �Z���ւ̃f�[�^��������
		BOOL WriteStr(char* data, long x,long y);
		BOOL WriteLong(long data, long x,long y);
		BOOL WriteDbl(double data, long x,long y);
		BOOL WriteVariant(char* data, long x,long y);

		// �Z���̃f�[�^�ǂݍ���
		BOOL ReadStr(char* data, int size, long x,long y);
		BOOL ReadLong(long* data, long x,long y);
		BOOL ReadDbl(double* data, long x,long y);

		// �Z���̏����ݒ�
		BOOL SetFormat( char* format,long x,long y);
		BOOL SetRowHeight(double h,long y);	// �s�����̐ݒ�
		BOOL SetColWidth(long w ,long x);	// �񕝂̐ݒ�
		BOOL CellJoint( BOOL onoff ,long sx, long sy, long ex,long ey);	// �Z���̌���
		BOOL SetFontSize( double fsize,long x,long y,long ex=-1,long ey=-1);
		BOOL SetFont( char* fontname,long x,long y,long ex=-1,long ey=-1);
		BOOL SetFontLine( long ltype,long x,long y);

		BOOL SetColWidthEx(double w ,long x);	// �񕝂̐ݒ� 2008.09.22


		BOOL SetFontColor( long color,long x,long y);	// �t�H���g�̐F�@2003.04.09�@Y.kogai
		BOOL SetCellColor( long color,long x,long y);	// �Z���̃o�b�N�J���[�@2005.01.26�@Y.kogai

		BOOL SetPageProperty(char *Size , BOOL Portrait=TRUE,double HdrMergin=-1,double FrtMergin=-1);	// �p���ݒ�
		BOOL SetMargin(short Left,short Right,short Top,short Bottom);			// �}�[�W���̐ݒ�
		int SetPrnArea(int sx, int sy, int ex,int ey);

		BOOL DrawLine(long LineStyle,long bt,long sx, long sy, long ex,long ey,int color=_xclBlack);

		BOOL SetDefFont(char* fname,double fsize=0.0);
		BOOL SetFontStyle( long style,long x,long y,long ex=-1,long ey=-1);

		// �G���[�̎擾
		long GetError(void){return(m_Err);};

		// �����̈ʒu�w��
		BOOL PutPosHorz(long horz,long x,long y,long ex=-1,long ey=-1);
		BOOL PutPosVert(long vert,long x,long y,long ex=-1,long ey=-1);

		// �t�H���g���k�����đS�̕\��
		BOOL PutFit(BOOL onoff,long x,long y,long ex=-1,long ey=-1);

		// �s�E��̑���
		BOOL RowDelete(long sy, long lCount);
		BOOL RowClear(long sy, long lCount);
		BOOL RowInsert(long sy, long lCount);
		BOOL RowCopy(long sy, long dy);
		BOOL RowPaste(long dy);
		BOOL ColumnDelete(long sx, long lCount);
		BOOL ColumnClear(long sx, long lCount);
		BOOL ColumnInsert(long sx, long lCount);
		BOOL ColumnCopy(long sx, long dx);
		BOOL ColumnPaste(long dx);

		//���
		void XlsViewPrint(BOOL default_pr,char *filename,int startsheet,int endsheet);
	protected:


};

// xls2�݊��̂��߂̃}�N����` coab�ǉ�
// �t�H���g����
#define fuDefault				_xfuNone				// �Ȃ�
#define fuNone					_xfuNone				// �Ȃ�
#define fuNormal				_xfuNormal				// ����
#define fuDouble				_xfuDouble				// ��d��

// �J���[�p���b�g
#define xcDefault				_xclAuto				// ����
#define xcBlack					_xclBlack				// ��
#define xcWhite					_xclWhite				// ��
#define xcRed					_xclRed					// ��
#define xcGreen					_xclGreen				// ��
#define xcBlue					_xclBlue				// ��
#define xcYellow				_xclYellow				// ��
#define xcPink					_xclPink				// �s���N
#define xcCyan					_xclCyan				// ��

// �r���̐���
#define lsNone          _xbsNone           		// �r�� �Ȃ�
#define lsDefault       _xbsNone           		//      �Ȃ�
#define lsNormal        _xbsThin  	       		//      ����
#define lsThick         _xbsMedium         		//      ����
#define lsBroken        _xbsDashed         		//      �j��
#define lsDot           _xbsDotted         		//      �_��
#define lsThick2        _xbsThick          		//      �ɑ���
#define lsDouble        _xbsDouble        		//      ��d��
#define lsSlender       _xbsHair       			//      �׎���
#define lsMidBroken     _xbsMediumDashed    	//      ���j��
#define lsSlnChain1     _xbsDashDot      		//      �׈�_����
#define lsMidChain1     _xbsMediumDashDot   	//      ����_����
#define lsSlnChain2     _xbsDashDotDot     		//      �ד�_����
#define lsMidChain2     _xbsMediumDashDotDot    //      ����_����
#define lsMidChains     _xbsSlantDashDot		//      ����_�΍���

// �{�b�N�X�̌`��
#define btBox           _xbtBox            // �{�b�N�X�̌`�� ����
#define btLtc           _xbtLtc            //                �i�q��
#define btOver          _xbtOver           //                �㉡��
#define btUnder         _xbtUnder          //                ������
#define btLeft          _xbtLeft           //                ���c��
#define btRight         _xbtRight          //                �E�c��

// �����ʒu
#define phNormal        _xphGeneral         // �����ʒu �W��
#define phLeft          _xphLeft           	//          ���l
#define phCenter        _xphCenter         	//          ��������
#define phRight         _xphRight          	//          �E�l
#define phLoop          _xphFill           	//          �J��Ԃ�
#define phBothEven      _xphJustify       	//          ���[����
#define phSelCenter     _xphCenterContinuous  //        �I��͈͓��Œ���
#define phEven          _xphDistributed     //          �ϓ�

// �����ʒu
#define pvUp            _xpvTop             // �����ʒu ��l��
#define pvCenter        _xpvCenter         	//          ��������
#define pvDown          _xpvBottom          //          ���l��
#define pvBothEven      _xpvJustify      	//          ���[����
#define pvEven          _xpvDistributed		//          �ϓ�

// �t�H���g�X�^�C��
#define xsDefault       _xfsDefault         // �t�H���g�X�^�C�� �m�[�}��
#define xsNormal        _xfsNormal         //                  �m�[�}��
#define xsBold          _xfsBold           //                  �{�[���h
#define xsItalic        _xfsItalic         //                  �C�^���b�N
#define xsStrike        _xfsStrike         //                  �������
#define xsUp            _xfsUp             //                  ��t��
#define xsDown          _xfsDown           //                  ���t��

#endif

