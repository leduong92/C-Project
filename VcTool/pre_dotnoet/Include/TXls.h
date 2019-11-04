// *verion 0.00
#ifndef TXLS_H
#define TXLS_H
//================================================================================================
//	NOK Fanatic Class Libraly
//	FileName	:TXls.H
//	Class		:TXls 
// 				:MS-Excel97(bif80)�`���t�@�C��/�������݃N���X
//	�쐬��		:2000.01.08	Coded by coba
//				2000.07.4 TAKE �V�[�g�ǉ��Ȃ�тɃV�[�g���Z�b�g�֐���ǉ�
//				2001.06.12 coba �����ݒ�֐�SetFormat()��ǉ�
//				2001.09.12 TAKE �s�����ݒ�SetRowHeight()��ǉ�
//				2001.09.12 TAKE �񕝐ݒ�SetColWidth()��ǉ�
//				2001.09.12 TAKE �Z����������CellJoint()��ǉ�
//				2001.09.12 TAKE �p���ݒ�SetPage()��ǉ�
//				2001.09.12 TAKE �p���ݒ�SetMargin()��ǉ�
//				2001.10.02 TAKE �r��DrawLine()��ǉ�
//				2002.10.15 coba �����̈ʒu�w���ǉ�
//				2003.04.23 coba �t�H���g�̃T�C�Y�w��
//				2003.06.04 coba �t�H���g�w��
//				2003.11.10 coba open���ɃV�[�g�����w��ł���悤�ɕύX OpenXls()
//				2004.05.08 coba �t�H���g�A���_�[���C���w��
//				2004.05.24 coba SetFont()��SetFontSize()��PutPosHorz()��͈͎w��ł���悤�ɏC��
//------------------------------------------------------------------------------------------------
// Dinamic Link Library	:xlscrt2.dll
// inport library		:xlscrt2.lib	<-�����N�ɕK�v
//================================================================================================

#include "windows.h"
#include "xls2.h"

class TXls
{

	private:
		long  m_XlsID;						// �t�@�C���n���h��(�n���h�����H)
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
	
		BOOL SetFontColor( long color,long x,long y);	// �t�H���g�̐F�@2003.04.09�@Y.kogai
		BOOL SetCellColor( long color,long x,long y);	// �Z���̃o�b�N�J���[�@2005.01.26�@Y.kogai

		BOOL SetPageProperty(char *Size , BOOL Portrait=TRUE,double HdrMergin=-1,double FrtMergin=-1);	// �p���ݒ�
		BOOL SetMargin(short Left,short Right,short Top,short Bottom);			// �}�[�W���̐ݒ�
		BOOL DrawLine(long LineStyle,long bt,long sx, long sy, long ex,long ey);
		
		// �G���[�̎擾
		long GetError(void){return(m_Err);};

		// �����̈ʒu�w��
		BOOL PutPosHorz(long horz,long x,long y,long ex=-1,long ey=-1);

	protected:


};
#endif

