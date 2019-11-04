//////////////////////////////////////////////////////////////////////////////
// Excel �N���G�[�^�[ Ver3.0 - C/C++ Header
//

#ifndef __XLSCRT3_H__
#define __XLSCRT3_H__

//////////////////////////////////////////////////////////////////////////////
// Excel �N���G�[�^�[ Ver1 �݊�

#ifndef _EXCEL_VER3_
#ifndef __EXCELOCX_IDL_

// ��{��`��
#define XL_DEFAULT_SHEETS			16		// �W���V�[�g��
#define XL97_DEFAULT_SHEETS			3		// �W���V�[�g��

// ��{�F��`��
#define XLP_DEFAULT					0		// ����
#define XLP_BLACK					8		// ��
#define XLP_WHITE					9		// ��
#define XLP_RED						10		// ��
#define XLP_GREEN					11		// ��
#define XLP_BLUE					12		// ��
#define XLP_YELLOW					13		// ��
#define XLP_MAGENTA					14		// ��
#define XLP_CYAN					15		// ��

// �r�����`��
#define XL_NORMAL_LINE				1		// �׎���
#define XL_THICK_LINE				2		// ����
#define XL_BROKEN_LINE				3		// �הj��
#define XL_DOT_LINE					4		// �ד_��
#define XL_THICK2_LINE				5		// �ɑ���
#define XL_DOUBLE_LINE				6		// �ד�d��
#define XL_SLENDER_LINE				7		// �ɍ׎���
#define XL_MEDIUM_DASHED_LINE		8		// ���j��
#define XL_DASH_DOT_LINE			9		// �׈�_����
#define XL_MEDIUM_DASH_DOT_LINE		10		// ����_����
#define XL_DASH_DOT2_LINE			11		// �ד�_����
#define XL_MEDIUM_DASH_DOT2_LINE	12		// ����_����
#define XL_SLANTED_DASH_DOT_LINE	13		// ����_�΍���

// �ʒu��������`��
#define XLAH_DEFAULT				0		// �����ʒu�W��
#define XLAH_LEFT					1		// ���l
#define XLAH_RIGHT					3		// �E�l
#define XLAH_CENTER					2		// ��������
#define XLAV_DEFAULT				2		// �����ʒu�W��
#define XLAV_TOP					0		// ��l
#define XLAV_BOTTOM					2		// ���l
#define XLAV_CENTER					1		// ��������
#define XLAO_DEFAULT				0		// ��]�W��
#define XLAO_180					1		// �c����            (EXCEL95)
#define XLAO_L90					2		// �����v�܂��X�O�K(EXCEL95)
#define XLAO_R90					3		// ���v�܂��X�O�K  (EXCEL95)
#define XL97AO_180					255		// �c����            (EXCEL97)
#define XL97_DIAG_NONE				0		// �ΐ�����          (EXCEL97)
#define XL97_DIAG_DOWN				1		// �E������ΐ�      (EXCEL97)
#define XL97_DIAG_UP				2		// �E�オ��ΐ�      (EXCEL97)
#define XL97_DIAG_BOTH				3		// ����              (EXCEL97)

// �y�[�W���C�A�E�g�p����ޒ�`��
#define XL_PS_LETTER				1		// ���^�[
#define XL_PS_REJOER				3		// ���W���[
#define XL_PS_REGAL					5		// ���[�K��
#define XL_PS_EXECUTIVE				7		// �G�O�[�N�e�B�u
#define XL_PS_A3					8		// �`�R
#define XL_PS_A4					9		// �`�S
#define XL_PS_A5					11		// �`�T
#define XL_PS_B4					12		// �a�S
#define XL_PS_B5					13		// �a�T
#define XL_MMTOINCH					25.4	// �o������v�Z�萔

// �G���[�萔
#define XL_FILE_OPEN_ERR			-1		// �t�@�C�����I�[�v���ł��Ȃ�
#define XL_HANDLE_ERR				-2		// �����ȃt�@�C���n���h��
#define XL_NO_OPEN					-3		// ���I�[�v���G���[
#define XL_NO_SHEET					-4		// �V�[�g�ԍ��G���[
#define XL_WRITE_ERR				-5		// �������ݎ��̃G���[
#define XL_ACTION_ERR				-6		// ���샂�[�h�G���[
#define XL_DATA_CAT_ERR				-7		// �f�[�^��ʃG���[
#define XL_ERRORVAL_ERR				-8		// �G���[�l
#define XL_LENGTH_ERR				-31		// �����G���[
#define XL_POINT_ERR				-32		// ���W�G���[
#define XL_ATTR_ERR					-33		// �����ԍ��G���[
#define XL_PARAM_ERR				-34		// �p�����[�^�G���[
#define XL_NO_DATA					-35		// �f�[�^������
#define XL_END_OF_DATA				-36		// �f�[�^�̏I���
#define XL_VERPOINT_ERR				-37		// �w�肵���ϐ������݂��Ȃ�
#define XL_NOMEMORY1_ERR			-40		// �������[�s���G���[�P
#define XL_NOMEMORY2_ERR			-41		// �������[�s���G���[�Q
#define XL_INTERNAL_ERR				-50		// �����G���[
#define XL_FUNC_ERR					-100	// �v�Z���`���G���[�i-100 �` -199�j
#define XL_FILE_CREATE_ERR			-300	// �쐬�G���[
#define XL_TEMPFILE_CREATE_ERR		-301	// �ꎞ�t�@�C�����쐬�ł��Ȃ�
#define XL_TEMPFILE_OPEN_ERR		-302	// �ꎞ�t�@�C�����I�[�v���ł��Ȃ�
#define XL_STREAM_ERR				-303	// �X�g���[���G���[
#define XL_PROTECT_ERR				-304	// �ی삳�ꂽ�V�[�g
#define XL_MUTEX_ERR				-400	// �~���[�e�b�N�X�G���[

// ������`�\���̒�`
struct CELLATTR_t {
	short FntNo;	// �t�H���g���ԍ�
	short FmtNo;	// �������ԍ�
	short alcH;		// �����ʒu   0:�W�� 1:���l 2:�������� 3:�E�l 4:�J��Ԃ�
					//            5:���[���� 6:�I��͈͓��Œ��� 7:�ϓ�
	short alcK;		// �񕝂Ő܂�Ԃ��ĕ\�� 0:�܂�Ԃ��Ȃ� 1:�܂�Ԃ�
	short alcV;		// �����ʒu   0:��l 1:���� 2:���l 3:���[ 4:�ϓ�
	short alcO;		// ��] 0:������ 1:�c����
					//      2:�����v���90� 3:���v���90�  (EXCEL95�p)
	short sTop;		// ��r���� 1�`13 (8�` EXCEL97�̂ݗL��)
	short sBtm;		// �� �V
	short sLft;		// �� �V
	short sRit;		// �E �V
	short pTop;		// ��r���J���[�p���b�g�ԍ��@8�`63
	short pBtm;		// �� �V
	short pLft;		// �� �V
	short pRit;		// �E �V
	short fPat;		// �h�ׂ��p�^�[��
	short fPatFG;	// �h�׃p�^�[���O�i�J���[�p���b�g�ԍ��@8�`63
	short fPatBG;	// �h�׃p�^�[���w�i�J���[�p���b�g�ԍ��@8�`63
	short fSFit;	// �k�����đS�̂̕\�� 0:���Ȃ� 1:���� (EXCEL97�̂ݗL��)
	short fMCell;	// �Z���̌���         0:���Ȃ� 1:���� (EXCEL97�̂ݗL��)
	short sgDiag;	// �΂ߐ�         0:����   1:�E�΂߉�����
					//                2:�E�Ώオ�� 3:���� (EXCEL97�̂ݗL��)
	short sDiag;	// �΂ߌr���� 0�`13                   (EXCEL97�̂ݗL��)
	short pDiag;	// �΂ߌr���J���[�p���b�g�ԍ�         (EXCEL97�̂ݗL��)
	short sIdnt;	// �C���f���g�l 0�`15     (EXCEL97: alcH�����l�̂ݗL��)
	short alcO97;	// ��] -90� �` 90� �܂� 1ߍ��݂Ŏw��\
					// �c����:255���w�肷��               (EXCEL97�̂ݗL��)
};
typedef struct CELLATTR_t CELLATTR;

// �v�Z���ʊi�[�\���̒�`
struct FUNCANSER_t {
	double dt;		// ���ʋ敪�� 0,1,2 �̂Ƃ��̌v�Z�����ʒl
	short fkbn;		// ���ʋ敪   0:���l 1:BOOL�^ 2:ERROR�^ 3:������^
	short slen;		// ���ʋ敪�� 3 �̂Ƃ��̌v�Z�����ʕ�����
	char str[256];	// ���ʋ敪�� 3 �̂Ƃ��̌v�Z�����ʕ�����
};
typedef struct FUNCANSER_t FUNCANSER;

#ifdef __cplusplus
extern "C" {  // Assume C declarations for C++
#endif
// ���[�N�V�[�g����
int PASCAL XL_CreateT(char* szFileName, char* szTFileName);
int PASCAL XM_CreateT(char* szFileName, char* szTFileName);
int PASCAL XL_Create(char* szFileName, int SheetMax);
int PASCAL XL_Create97(char* szFileName, int SheetMax);
int PASCAL XL_SelSheetNo(int SheetNo);
int PASCAL XL_SheetName(char* szSheetName);
int PASCAL XL_SetColWidth(int sx, int ex, int wid);
int PASCAL XL_SetRowHeight(int y, double PointHigt);
int PASCAL XL_Close();
int PASCAL XL_Cancel();
// �����ݒ�
int PASCAL XL_GetAttrNo(long Kei, long Keip, int alc, int FntNo, int FmtNo);
int PASCAL XL_GetAttrNo2(CELLATTR* pCell);
int PASCAL XL_InitAttr(CELLATTR* pCell);
int PASCAL XL_FontNo(char* szFontName, double Point, int col, int styl, int UnderLine);
int PASCAL XL_FmtNo(char* szFmt);
int PASCAL XL_SetAttr(int x, int y, int xfno);
int PASCAL XL_SetAttrExt(int sx, int sy, int ex, int ey, int xfno);
int PASCAL XL_SetAttr2(int x, int y, CELLATTR* pCell);
int PASCAL XL_SetAttrExt2(int sx, int sy, int ex, int ey, CELLATTR* pCell);
int PASCAL XL_SetAttrMode(int fAttrMode);
int PASCAL XL_SetRefSheet(char* szSheets);
int PASCAL XL_SetGridMode(int fGridMode);
// �ȈՏ�������
int PASCAL XL_WrtStr(char* str);
int PASCAL XL_WrtStr2(char* str, int xfno);
int PASCAL XL_WrtInt(long dt);
int PASCAL XL_WrtInt2(long dt, int xfno);
int PASCAL XL_WrtDouble(double dt);
int PASCAL XL_WrtDouble2(double dt, int xfno);
int PASCAL XL_WrtFunc(char* str);
int PASCAL XL_WrtFunc2(char* str, int xfno);
int PASCAL XL_MovePos(int x, int y);
int PASCAL XL_WrtCR();
// �W����������
int PASCAL XL_WriteStr(int x, int y, char* str);
int PASCAL XL_WriteStr2(int x, int y, char* str, int xfno);
int PASCAL XL_WriteInt(int x, int y, long dt);
int PASCAL XL_WriteInt2(int x, int y, long dt, int xfno);
int PASCAL XL_WriteDouble(int x, int y, double dt);
int PASCAL XL_WriteDouble2(int x, int y, double dt, int xfno);
int PASCAL XL_WriteFunc(int x, int y, char* str);
int PASCAL XL_WriteFunc2(int x, int y, char* str, int xfno);
int PASCAL XL_WriteFunc3(int x, int y, char* str, int xfno, FUNCANSER* pAns);
// ���^�r���`��
int PASCAL XL_BoxLine(int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XL_LtcLine(int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XL_UnderLine(int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XL_OverLine(int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XL_LeftLine(int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XL_RightLine(int sx, int sy, int ex, int ey, int LStyl);
// �r���ݒ�
long PASCAL XL_KeiNo(int Top, int Bottom, int Left, int Right);
long PASCAL XL_KeiPNo(int Top, int Bottom, int Left, int Right);
long PASCAL XL_KBoxNo(int LStyl);
long PASCAL XL_KTopNo(int LStyl);
long PASCAL XL_KBtmNo(int LStyl);
long PASCAL XL_KLftNo(int LStyl);
long PASCAL XL_KRitNo(int LStyl);
long PASCAL XL_KBoxPNo(int LStyl);
long PASCAL XL_KTopPNo(int LStyl);
long PASCAL XL_KBtmPNo(int LStyl);
long PASCAL XL_KLftPNo(int LStyl);
long PASCAL XL_KRitPNo(int LStyl);
// ���[�N�V�[�g����i�}���`�t�@�C���^�}���`�V�[�g�Ή��j
int PASCAL XM_Create(char* szFileName, int SheetMax);
int PASCAL XM_Create97(char* szFileName, int SheetMax);
int PASCAL XM_SheetName(int hXLFile, int SheetNo, char* szSheetName);
int PASCAL XM_SetColWidth(int hXLFile, int SheetNo, int sx, int ex, int wid);
int PASCAL XM_SetRowHeight(int hXLFile, int SheetNo, int y, double PointHigt);
int PASCAL XM_WriteBin(int hXLFile, int SheetNo, int id, int len, void* dt);
int PASCAL XM_Close(int hXLFile);
int PASCAL XM_Cancel(int hXLFile);
// �����ݒ�i�}���`�t�@�C���^�}���`�V�[�g�Ή��j
int PASCAL XM_GetAttrNo(int hXLFile, long Kei, long Keip, int alc, int FntNo, int FmtNo);
int PASCAL XM_GetAttrNo2(int hXLFile, CELLATTR* pCell);
int PASCAL XM_InitAttr(CELLATTR* pCell);
int PASCAL XM_FontNo(int hXLFile, char* szFontName, double Point, int col, int styl, int UnderLine);
int PASCAL XM_FmtNo(int hXLFile, char* szFmt);
int PASCAL XM_SetAttr(int hXLFile, int SheetNo, int x, int y, int xfno);
int PASCAL XM_SetAttrExt(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int xfno);
int PASCAL XM_SetAttr2(int hXLFile, int SheetNo, int x, int y, CELLATTR* pCell);
int PASCAL XM_SetAttrExt2(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, CELLATTR* pCell);
int PASCAL XM_SetRefSheet(int hXLFile, int SheetNo, char* szSheets);
int PASCAL XM_SetGridMode(int hXLFile, int SheetNo, int fGridMode);
// �W���������݁i�}���`�t�@�C���^�}���`�V�[�g�Ή��j
int PASCAL XM_WriteStr(int hXLFile, int SheetNo, int x, int y, char* str);
int PASCAL XM_WriteStr2(int hXLFile, int SheetNo, int x, int y, char* str, int xfno);
int PASCAL XM_WriteInt(int hXLFile, int SheetNo, int x, int y, long dt);
int PASCAL XM_WriteInt2(int hXLFile, int SheetNo, int x, int y, long dt, int xfno);
int PASCAL XM_WriteDouble(int hXLFile, int SheetNo, int x, int y, double dt);
int PASCAL XM_WriteDouble2(int hXLFile, int SheetNo, int x, int y, double dt, int xfno);
int PASCAL XM_WriteFunc(int hXLFile, int SheetNo, int x, int y, char* str);
int PASCAL XM_WriteFunc2(int hXLFile, int SheetNo, int x, int y, char* str, int xfno);
int PASCAL XM_WriteFunc3(int hXLFile, int SheetNo, int x, int y, char* str, int xfno, FUNCANSER* pAns);
// �r���ݒ�i�}���`�t�@�C���^�}���`�V�[�g�Ή��j
int PASCAL XM_BoxLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XM_LtcLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XM_UnderLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XM_OverLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XM_LeftLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
int PASCAL XM_RightLine(int hXLFile, int SheetNo, int sx, int sy, int ex, int ey, int LStyl);
                       
#ifdef  __cplusplus
}
#endif

#endif	// __EXCELOCX_IDL_
#endif	// _EXCEL_VER3_

//////////////////////////////////////////////////////////////////////////////
// Excel �N���G�[�^ Ver2 Header

// Define�l

#define XL_VER95            95              // Excel95
#define XL_VER97            97              // Excel97
#define XL_VER2000          2000            // Excel2000
#define XL_VER2002          2002            // Excel2002
#define XL_VER2003          2003            // Excel2003

#define XL_XCDEFAULT        0               // �J���[�p���b�g ����
#define XL_XCBLACK          8               //                ��
#define XL_XCWHITE          9               //                ��
#define XL_XCRED            10              //                ��
#define XL_XCGREEN          11              //                ��
#define XL_XCBLUE           12              //                ��
#define XL_XCYELLOW         13              //                ��
#define XL_XCPINK           14              //                �s���N
#define XL_XCCYAN           15              //                ��

#define XL_XSNORMAL         0x00            // �t�H���g�X�^�C�� �m�[�}��
#define XL_XSBOLD           0x01            //                  �{�[���h
#define XL_XSITALIC         0x02            //                  �C�^���b�N
#define XL_XSSTRIKE         0x04            //                  �������
#define XL_XSUP             0x08            //                  ��t��
#define XL_XSDOWN           0x10            //                  ���t��

#define XL_FUNONE           0               // �t�H���g���� �Ȃ�
#define XL_FUNORMAL         1               //              ����
#define XL_FUDOUBLE         2               //              ��d��

#define XL_LSNONE           0x00000000      // �r���Ȃ�
#define XL_LSNORMAL         0x01000000      // �r�� ����
#define XL_LSTHICK          0x02000000      //      ����
#define XL_LSBROKEN         0x03000000      //      �j��
#define XL_LSDOT            0x04000000      //      �_��
#define XL_LSTHICK2         0x05000000      //      �ɑ���
#define XL_LSDOUBLE         0x06000000      //      ��d��
#define XL_LSSLENDER        0x07000000      //      �׎���
#define XL_LSMIDBROKEN      0x08000000      //      ���j��
#define XL_LSSLNCHAIN1      0x09000000      //      �׈�_����
#define XL_LSMIDCHAIN1      0x0A000000      //      ����_����
#define XL_LSSLNCHAIN2      0x0B000000      //      �ד�_����
#define XL_LSMIDCHAIN2      0x0C000000      //      ����_����
#define XL_LSMIDCHAINS      0x0D000000      //      ����_�΍���

#define XL_PN01             0x01000000      // �p�^�[���ԍ� �P
#define XL_PN02             0x02000000      // �p�^�[���ԍ� �Q
#define XL_PN03             0x03000000      // �p�^�[���ԍ� �R
#define XL_PN04             0x04000000      // �p�^�[���ԍ� �S
#define XL_PN05             0x05000000      // �p�^�[���ԍ� �T
#define XL_PN06             0x06000000      // �p�^�[���ԍ� �U
#define XL_PN07             0x07000000      // �p�^�[���ԍ� �V
#define XL_PN08             0x08000000      // �p�^�[���ԍ� �W
#define XL_PN09             0x09000000      // �p�^�[���ԍ� �X
#define XL_PN10             0x0A000000      // �p�^�[���ԍ� 10
#define XL_PN11             0x0B000000      // �p�^�[���ԍ� 11
#define XL_PN12             0x0C000000      // �p�^�[���ԍ� 12
#define XL_PN13             0x0D000000      // �p�^�[���ԍ� 13
#define XL_PN14             0x0E000000      // �p�^�[���ԍ� 14
#define XL_PN15             0x0F000000      // �p�^�[���ԍ� 15
#define XL_PN16             0x10000000      // �p�^�[���ԍ� 16
#define XL_PN17             0x11000000      // �p�^�[���ԍ� 17
#define XL_PN18             0x12000000      // �p�^�[���ԍ� 18

#define XL_BTBOX            0               // �{�b�N�X�̌`�� ����
#define XL_BTLTC            1               //                �i�q��
#define XL_BTOVER           2               //                �㉡��
#define XL_BTUNDER          3               //                ������
#define XL_BTLEFT           4               //                ���c��
#define XL_BTRIGHT          5               //                �E�c��

#define XL_PHNORMAL         20              // �����ʒu �W��
#define XL_PHLEFT           21              //          ���l
#define XL_PHCENTER         22              //          ��������
#define XL_PHRIGHT          23              //          �E�l
#define XL_PHLOOP           24              //          �J��Ԃ�
#define XL_PHBOTHEVEN       25              //          ���[����
#define XL_PHSELCENTER      26              //          �I��͈͓��Œ���
#define XL_PHEVEN           27              //          �ϓ�

#define XL_PVUP             0               // �����ʒu ��l��
#define XL_PVCENTER         1               //          ��������
#define XL_PVDOWN           2               //          ���l��
#define XL_PVBOTHEVEN       3               //          ���[����
#define XL_PVEVEN           4               //          �ϓ�

#define XL_PTHORZ           100             // �������� ��
#define XL_PTVERT           101             //          �c
#define XL_PTRVTURN90       102             //          �����v���ɂX�O��
#define XL_PTTURN90         103             //          ���v���ɂX�O��

#define XL_PSLETTER         1               // �p���T�C�Y ���^�[
#define XL_PSREJOER         3               //            ���W���[
#define XL_PSREGAL          5               //            ���[�K��
#define XL_PSEXECUTIVE      7               //            �G�O�[�N�e�B�u
#define XL_PSA3             8               //            �`�R
#define XL_PSA4             9               //            �`�S
#define XL_PSA5             11              //            �`�T
#define XL_PSB4             12              //            �a�S
#define XL_PSB5             13              //            �a�T

#define XL_ORLANDSCAPE      0               // �p������ ��
#define XL_ORPORTRAIT       2               //          �c

#define XL_MAXPOINT         0               // �f�[�^�A�����͈� �ő�͈�
#define XL_ENDPOINT         1               //                  �ŏI�|�C���g

#define XL_PCNONE			0x00			// �y�[�W���� �w��Ȃ�
#define XL_PCHORZ			0x01			//            ����
#define XL_PCVERT			0x02			//            ����

#define XL_VMVALIDALL		0x00			// �ϐ����������[�h �S�č���
#define XL_VMVALIDANSWER	0x01			//                  �Z���̂�
#define XL_VMVALIDTEXTBOX	0x02			//                  �v�Z���̂�
#define XL_VMVALIDCELLTEXT	0x04			//                  �e�L�X�g�{�b�N�X

#define XL_PORIGHTFROMLEFT	0				// �y�[�W���� ������E��
#define XL_POBOTTOMFROMTOP	1				//            �ォ�牺��

#define XL_OPOPEN           0               // �I�[�v�����[�h �I�[�v��
#define XL_OPCREATE         1               //                �N���G�C�g

#define XL_KCNONE           0               // �����R�[�h ���g�p
#define XL_KCNECINTERNAL    1               //  �V        �m�d�b����
#define XL_KCJIPSE          2               //  �V        JIPS(E)
#define XL_KCIBM            3               //  �V        IBM�W��
#define XL_KCJEF9           4               //  �V        JEF9
#define XL_KCJEF12          5               //  �V        JEF12
#define XL_KCKEIS           6               //  �V        KEIS
#define XL_KCMELCOM         7               //  �V        MELCOM
#define XL_KCHAYAC          8               //  �V        HAYAC
#define XL_KCJIS            9               //  �V        JIS C6226

#define XL_DTCSV            0               // �f�[�^�`�� �b�r�u
#define XL_DTBINARY         1               //  �V        �o�C�i��
#define XL_EBCDIC           2               //  �V        �ʐM�f�[�^

#define ERR_DONTOPEN        1               // �t�@�C�����I�[�v���ł��Ȃ�
#define ERR_FILEHANDLE      2               // �����ȃt�@�C���n���h��
#define ERR_NOTOPEN         3               // ���I�[�v���G���[
#define ERR_SHEETNO         4               // �V�[�g�ԍ��G���[
#define ERR_WRITE           5               // �������ݎ��̃G���[
#define ERR_ACTION          6               // ���샂�[�h�G���[
#define ERR_DATACAT         7               // �f�[�^��ʃG���[
#define ERR_ERRORVALUE      8               // �G���[�l
#define ERR_CLEAR	        20              // �N���A�G���[
#define ERR_COPY	        21              // �R�s�[�G���[
#define ERR_PASTE	        22              // �\��t���G���[
#define ERR_INSERT	        23              // �}���G���[
#define ERR_DELETE	        24              // �폜�G���[
#define ERR_LENGTH          31              // �����G���[
#define ERR_LOCATE          32              // ���W�G���[
#define ERR_ATTR            33              // �����ԍ��G���[
#define ERR_PARAM           34              // �p�����[�^�G���[
#define ERR_NODATA          35              // �f�[�^������
#define ERR_ENDOFDATA       36              // �f�[�^�̏I���
#define ERR_VERPOINT        37              // �w�肵���ϐ������݂��Ȃ�
#define ERR_BREAKCOUNT      38              // ���y�[�W���������𒴂�����
#define ERR_MEMORY1         40              // �������[�s���G���[�P
#define ERR_MEMORY2         41              // �������[�s���G���[�Q
#define ERR_OTHER           50              // �����G���[
#define ERR_FUNCTION        100             // �v�Z���`���G���[
#define ERR_FILE_CREATE		300				// �쐬�G���[
#define ERR_TEMPFILE_CREATE	301				// �ꎞ�t�@�C�����쐬�ł��Ȃ�
#define ERR_TEMPFILE_OPEN	302				// �ꎞ�t�@�C�����I�[�v���ł��Ȃ�
#define ERR_STREAM			303				// �X�g���[���G���[
#define ERR_PROTECT			304				// �ی삳�ꂽ�V�[�g
#define ERR_MUTEX			400				// �~���[�e�b�N�X�G���[

// C/C++ �p�� Define �l

#ifndef   __CREATOROCX_RC_
#ifndef   __EXCELOCX_IDL_
#ifndef   __EXCELCRTOCX_H_

// Excel�o�[�W����
#define ver95           XL_VER95            // Excel95
#define ver97           XL_VER97            // Excel97
#define ver2000         XL_VER2000          // Excel2000
#define ver2002         XL_VER2002          // Excel2002
#define ver2003         XL_VER2003          // Excel2003
// �J���[�p���b�g
#define xcDefault       XL_XCDEFAULT        // �J���[�p���b�g ����
#define xcBlack         XL_XCBLACK          //                ��
#define xcWhite         XL_XCWHITE          //                ��
#define xcRed           XL_XCRED            //                ��
#define xcGreen         XL_XCGREEN          //                ��
#define xcBlue          XL_XCBLUE           //                ��
#define xcYellow        XL_XCYELLOW         //                ��
#define xcPink          XL_XCPINK           //                �s���N
#define xcCyan          XL_XCCYAN           //                ��
// �t�H���g�X�^�C��
#define xsDefault       XL_XSNORMAL         // �t�H���g�X�^�C�� �m�[�}��
#define xsNormal        XL_XSNORMAL         //                  �m�[�}��
#define xsBold          XL_XSBOLD           //                  �{�[���h
#define xsItalic        XL_XSITALIC         //                  �C�^���b�N
#define xsStrike        XL_XSSTRIKE         //                  �������
#define xsUp            XL_XSUP             //                  ��t��
#define xsDown          XL_XSDOWN           //                  ���t��
// �t�H���g����
#define fuDefault       XL_FUNONE           // �t�H���g���� �Ȃ�
#define fuNone          XL_FUNONE           //              �Ȃ�
#define fuNormal        XL_FUNORMAL         //              ����
#define fuDouble        XL_FUDOUBLE         //              ��d��
// �r���̐���
#define lsNone          XL_LSNONE           // �r�� �Ȃ�
#define lsDefault       XL_LSNONE           //      �Ȃ�
#define lsNormal        XL_LSNORMAL         //      ����
#define lsThick         XL_LSTHICK          //      ����
#define lsBroken        XL_LSBROKEN         //      �j��
#define lsDot           XL_LSDOT            //      �_��
#define lsThick2        XL_LSTHICK2         //      �ɑ���
#define lsDouble        XL_LSDOUBLE         //      ��d��
#define lsSlender       XL_LSSLENDER        //      �׎���
#define lsMidBroken     XL_LSMIDBROKEN      //      ���j��
#define lsSlnChain1     XL_LSSLNCHAIN1      //      �׈�_����
#define lsMidChain1     XL_LSMIDCHAIN1      //      ����_����
#define lsSlnChain2     XL_LSSLNCHAIN2      //      �ד�_����
#define lsMidChain2     XL_LSMIDCHAIN2      //      ����_����
#define lsMidChains     XL_LSMIDCHAINS      //      ����_�΍���
// �p�^�[���ԍ�
#define pn01            XL_PN01             // �p�^�[���ԍ� �P
#define pn02            XL_PN02             // �p�^�[���ԍ� �Q
#define pn03            XL_PN03             // �p�^�[���ԍ� �R
#define pn04            XL_PN04             // �p�^�[���ԍ� �S
#define pn05            XL_PN05             // �p�^�[���ԍ� �T
#define pn06            XL_PN06             // �p�^�[���ԍ� �U
#define pn07            XL_PN07             // �p�^�[���ԍ� �V
#define pn08            XL_PN08             // �p�^�[���ԍ� �W
#define pn09            XL_PN09             // �p�^�[���ԍ� �X
#define pn10            XL_PN10             // �p�^�[���ԍ� 10
#define pn11            XL_PN11             // �p�^�[���ԍ� 11
#define pn12            XL_PN12             // �p�^�[���ԍ� 12
#define pn13            XL_PN13             // �p�^�[���ԍ� 13
#define pn14            XL_PN14             // �p�^�[���ԍ� 14
#define pn15            XL_PN15             // �p�^�[���ԍ� 15
#define pn16            XL_PN16             // �p�^�[���ԍ� 16
#define pn17            XL_PN17             // �p�^�[���ԍ� 17
#define pn18            XL_PN18             // �p�^�[���ԍ� 18
// �{�b�N�X�̌`��
#define btBox           XL_BTBOX            // �{�b�N�X�̌`�� ����
#define btLtc           XL_BTLTC            //                �i�q��
#define btOver          XL_BTOVER           //                �㉡��
#define btUnder         XL_BTUNDER          //                ������
#define btLeft          XL_BTLEFT           //                ���c��
#define btRight         XL_BTRIGHT          //                �E�c��
// �����ʒu
#define phNormal        XL_PHNORMAL         // �����ʒu �W��
#define phLeft          XL_PHLEFT           //          ���l
#define phCenter        XL_PHCENTER         //          ��������
#define phRight         XL_PHRIGHT          //          �E�l
#define phLoop          XL_PHLOOP           //          �J��Ԃ�
#define phBothEven      XL_PHBOTHEVEN       //          ���[����
#define phSelCenter     XL_PHSELCENTER      //          �I��͈͓��Œ���
#define phEven          XL_PHEVEN           //          �ϓ�
// �����ʒu
#define pvUp            XL_PVUP             // �����ʒu ��l��
#define pvCenter        XL_PVCENTER         //          ��������
#define pvDown          XL_PVDOWN           //          ���l��
#define pvBothEven      XL_PVBOTHEVEN       //          ���[����
#define pvEven          XL_PVEVEN           //          �ϓ�
// ��������
#define ptHorz          XL_PTHORZ           // �������� ��
#define ptVert          XL_PTVERT           //          �c
#define ptRvTurn90      XL_PTRVTURN90       //          �����v���ɂX�O��
#define ptTurn90        XL_PTTURN90         //          ���v���ɂX�O��
// �p���T�C�Y
#define psLetter        XL_PSLETTER         // �p���T�C�Y ���^�[
#define psRejoer        XL_PSREJOER         //            ���W���[
#define psRegal         XL_PSREGAL          //            ���[�K��
#define psExecutive     XL_PSEXECUTIVE      //            �G�O�[�N�e�B�u
#define psA3            XL_PSA3             //            �`�R
#define psA4            XL_PSA4             //            �`�S
#define psA5            XL_PSA5             //            �`�T
#define psB4            XL_PSB4             //            �a�S
#define psB5            XL_PSB5             //            �a�T
// �p������
#define orLandscape     XL_ORLANDSCAPE      // �p������ ��
#define orPortrait      XL_ORPORTRAIT       //          �c
// �f�[�^�A�����͈�
#define xlMaxPoint      XL_MAXPOINT         // �f�[�^�A�����͈� �ő�͈�
#define xlEndPoint      XL_ENDPOINT         //                     �ŏI�|�C���g
// �y�[�W����
#define pcNone			XL_PCNONE			// �y�[�W���� �w��Ȃ�
#define pcHorz			XL_PCHORZ			//            ����
#define pcVert			XL_PCVERT			//            ����
// �y�[�W����
#define poRightFromLeft	XL_PORIGHTFROMLEFT	// �y�[�W���� ������E��
#define poBottomFromTop	XL_POBOTTOMFROMTOP	//            �ォ�牺��
// �ϐ����������[�h
#define vmValidAll			XL_VMVALIDALL		// �ϐ����������[�h �S�č���
#define vmInvalidAnswer		XL_VMVALIDANSWER	//                  �Z���̂�
#define vmInvalidTextBox	XL_VMVALIDTEXTBOX	//                  �v�Z���̂�
#define vmInvalidCellText	XL_VMVALIDCELLTEXT	//                  �e�L�X�g�{�b�N�X

// �I�[�v�����[�h
#define opOpen          XL_OPOPEN           // �I�[�v�����[�h �I�[�v��
#define opCreate        XL_OPCREATE         //                �N���G�C�g

#define vtStr           0                   // ������
#define vtDouble        1                   // �����l
#define vtInt           2                   // ����
#define vtFunc          3                   // �v�Z��
#define vtNull          4                   // �f�[�^�Ȃ�

#define kcNone          XL_KCNONE           // �����R�[�h ���g�p
#define kcNECInternal   XL_KCNECINTERNAL    //  �V        �m�d�b����
#define kcJIPSE         XL_KCJIPSE          //  �V        JIPS(E)
#define kcIBM           XL_KCIBM            //  �V        IBM�W��
#define kcJEF9          XL_KCJEF9           //  �V        JEF9
#define kcJEF12         XL_KCJEF12          //  �V        JEF12
#define kcKEIS          XL_KCKEIS           //  �V        KEIS
#define kcMELCOM        XL_KCMELCOM         //  �V        MELCOM
#define kcHAYAC         XL_KCHAYAC          //  �V        HAYAC
#define kcJIS           XL_KCJIS            //  �V        JIS C6226

#define dtCSV           XL_DTCSV            // �f�[�^�`�� �b�r�u
#define dtBinary        XL_DTBINARY         //  �V        �o�C�i��
#define dtEBCDIC        XL_EBCDIC           //  �V        �ʐM�f�[�^

#define errDontOpen     ERR_DONTOPEN        // �t�@�C�����I�[�v���ł��Ȃ�
#define errFileHandle   ERR_FILEHANDLE      // �����ȃt�@�C���n���h��
#define errNotOpen      ERR_NOTOPEN         // ���I�[�v���G���[
#define errSheetNo      ERR_SHEETNO         // �V�[�g�ԍ��G���[
#define errWrite        ERR_WRITE           // �������ݎ��̃G���[
#define errAction       ERR_ACTION          // ���샂�[�h�G���[
#define errDataCat      ERR_DATACAT         // �f�[�^��ʃG���[
#define errErrorValue	ERR_ERRORVALUE      // �G���[�l
#define errClear		ERR_CLEAR			// �N���A�G���[
#define errCopy			ERR_COPY			// �R�s�[�G���[
#define errPaste		ERR_PASTE			// �\��t���G���[
#define errInsert		ERR_INSERT			// �}���G���[
#define errDelete		ERR_DELETE			// �폜�G���[
#define errLength       ERR_LENGTH          // �����G���[
#define errLocate       ERR_LOCATE          // ���W�G���[
#define errAttr         ERR_ATTR            // �����ԍ��G���[
#define errParam        ERR_PARAM           // �p�����[�^�G���[
#define errNoData       ERR_NODATA          // �f�[�^������
#define errEndOfData    ERR_ENDOFDATA       // �f�[�^�̏I���
#define errVerPoint     ERR_VERPOINT        // �w�肵���ϐ������݂��Ȃ�
#define errBreakCount   ERR_BREAKCOUNT      // ���y�[�W�����������z����
#define errMemory1      ERR_MEMORY1         // �������[�s���G���[�P
#define errMemory2      ERR_MEMORY2         // �������[�s���G���[�Q
#define errOther        ERR_OTHER           // �����G���[
#define errFunction     ERR_FUNCTION        // �v�Z���`���G���[
#define errFileCreate	ERR_FILE_CREATE		// �쐬�G���[
#define errTempCreate	ERR_TEMPFILE_CREATE	// �ꎞ�t�@�C�����쐬�ł��Ȃ�
#define errTempOpen		ERR_TEMPFILE_OPEN	// �ꎞ�t�@�C�����I�[�v���ł��Ȃ�
#define errStream		ERR_STREAM			// �X�g���[���G���[
#define errProtect		ERR_PROTECT			// �ی삳�ꂽ�V�[�g
#define errMutex		ERR_MUTEX			// �~���[�e�b�N�X�G���[

#endif // __EXCELCRTOCX_H_

// �֐��̒�`
#ifdef __cplusplus
extern "C" {  // Assume C declarations for C++
#endif
    // �N���G�[�^�֐�
    BOOL PASCAL XlsPutFormat(LPSTR Format, LPSTR Cell);
    BOOL PASCAL XlsPutFontName(LPSTR FontName, LPSTR Cell);
    BOOL PASCAL XlsPutFontPoint(double FontPoint, LPSTR Cell);
    BOOL PASCAL XlsPutFontColor(long FontColor, LPSTR Cell);
    BOOL PASCAL XlsPutFontStyle(long FontStyle, LPSTR Cell);
    BOOL PASCAL XlsPutFontULine(long FontULine, LPSTR Cell);
    BOOL PASCAL XlsPutLineTop(long LineTop, LPSTR Cell);
    BOOL PASCAL XlsPutLineBottom(long LineBottom, LPSTR Cell);
    BOOL PASCAL XlsPutLineLeft(long LineLeft, LPSTR Cell);
    BOOL PASCAL XlsPutLineRight(long LineRight, LPSTR Cell);
    BOOL PASCAL XlsPutLineLeftUp(long LineLeftUp, LPSTR Cell);
    BOOL PASCAL XlsPutLineRightUp(long LineRightUp, LPSTR Cell);
    BOOL PASCAL XlsPutPattern(long PatNo, LPSTR Cell);
    BOOL PASCAL XlsPutBackColor(long BackColor, LPSTR Cell);
    BOOL PASCAL XlsPutPosHorz(long PosHorz, LPSTR Cell);
    BOOL PASCAL XlsPutPosVert(long PosVert, LPSTR Cell);
    BOOL PASCAL XlsPutPosTurn(long PosTurn, LPSTR Cell);
    BOOL PASCAL XlsPutOverReturn(BOOL OverReturn, LPSTR Cell);
    BOOL PASCAL XlsPutFit(BOOL Fit, LPSTR Cell);
    BOOL PASCAL XlsPutJoint(BOOL Joint, LPSTR Cell);

    BOOL PASCAL XlsPutDouble(double Double, LPSTR Cell);
    BOOL PASCAL XlsGetDouble(double *Double, LPSTR Cell);
    BOOL PASCAL XlsPutLong(long Long, LPSTR Cell);
    BOOL PASCAL XlsGetLong(long *Long, LPSTR Cell);
    BOOL PASCAL XlsPutStr(LPSTR Str, LPSTR Cell);
    BOOL PASCAL XlsGetStr(LPSTR Str, int StrSize, LPSTR Cell);
    BOOL PASCAL XlsPutFunc(LPSTR Func, LPSTR Ans, LPSTR Cell);
    BOOL PASCAL XlsPutBox(long LineStyle, long BoxType, LPSTR Cell);
    BOOL PASCAL XlsPutValue(LPSTR Value, LPSTR Cell);
    BOOL PASCAL XlsGetValue(LPSTR pVal, int nSize, LPSTR Cell);
    BOOL PASCAL XlsPutValueV(VARIANT Value, LPSTR Cell);
    BOOL PASCAL XlsGetValueV(VARIANT* pVal, LPSTR Cell);
    BOOL PASCAL XlsPutFuncV(LPSTR Func, VARIANT Ans, LPSTR Cell);

    BOOL PASCAL XlsPutColWidth(long ColWidth, LPSTR Cell);
    BOOL PASCAL XlsPutColWidthEx(double ColWidth, LPSTR Cell);
    BOOL PASCAL XlsPutColWidth2(long ColWidth, LPSTR Cell);
    BOOL PASCAL XlsGetColWidth2(long* ColWidth, LPSTR Cell);
    BOOL PASCAL XlsPutRowHeight(double RowHeight, LPSTR Cell);
    BOOL PASCAL XlsPutRowHeight2(double RowHeight, LPSTR Cell);
    BOOL PASCAL XlsGetRowHeight2(double* RowHeight, LPSTR Cell);
    BOOL PASCAL XlsPutRefSheet(LPSTR RefSheet);
    BOOL PASCAL XlsPutSheetName(LPSTR SheetName);
    BOOL PASCAL XlsPutDefFontName(LPSTR FontName);
    BOOL PASCAL XlsPutDefFontPoint(double FontPoint);
    BOOL PASCAL XlsMove(LPSTR Cell);

    BOOL PASCAL XlsCreateBook(LPSTR FileName, long Ver, long SheetMax, long *ID);
    BOOL PASCAL XlsReadBook(LPSTR FileName, long *ID);
    BOOL PASCAL XlsOpenBook(LPSTR FileName, LPSTR OverlayName, long *ID);
    BOOL PASCAL XlsCloseBook(BOOL flg);
    BOOL PASCAL XlsSelectBook(long Book);
    BOOL PASCAL XlsSheetNo(long SheetNo);
    BOOL PASCAL XlsPageMode(long PageSize, long Scale, long Orientation, double HdrMergin, double FtrMergin);
    BOOL PASCAL XlsSetMargin(int Left, int Right, int Top, int Bottom);
    BOOL PASCAL XlsAddSheet(long SheetCnt);
    BOOL PASCAL XlsDelSheet(long SheetNo, long SheetCnt);
    BOOL PASCAL XlsModeGrid(BOOL ModeGrid);

    BOOL PASCAL XlsGetErrorNo(long *ErrorNo);
    BOOL PASCAL XlsGetErrorMessage(LPSTR ErrorMessage, int ErrorMessageSize);

    LPSTR PASCAL CELL(LPSTR Cell, long x, long y);
    LPSTR PASCAL CELL2(LPSTR Cell, long x, long y);
    LPSTR PASCAL POS(long sx, long sy, long ex, long ey);
    LPSTR PASCAL POS2(long sx, long sy, long ex, long ey);

    BOOL PASCAL XlsGetSheetName(long lSheetNo, LPSTR pszSheetName, int nSize);

    int PASCAL XlsGetSheetCount();
    BOOL PASCAL XlsPutBreak(BOOL bBreak, LPSTR pszCell);
    BOOL PASCAL XlsSetHeader(LPCTSTR pszLeft, LPCTSTR pszCenter, LPCTSTR pszRight);
    BOOL PASCAL XlsSetFooter(LPCTSTR pszLeft, LPCTSTR pszCenter, LPCTSTR pszRight);
	BOOL PASCAL XlsCopySheet(long lSource, long lInsertPos, LPCTSTR pszSheetName);
    int PASCAL XlsGetActiveSheet();
    BOOL PASCAL XlsSetActiveSheet(long lSheetNo);
	int PASCAL XlsSetKeyWord(LPCTSTR pszKeyWord);
	BOOL PASCAL XlsSetZoom(long lZoom);
	BOOL PASCAL XlsGetLastPos(long* ex, long* ey, BOOL bDataOnly, int pt);
	BOOL PASCAL XlsRowDelete(long sy, long lCount);
	BOOL PASCAL XlsRowClear(long sy, long lCount);
	BOOL PASCAL XlsRowInsert(long sy, long lCount);
	BOOL PASCAL XlsRowCopy(long sy, long dy);
	BOOL PASCAL XlsRowPaste(long dy);
	BOOL PASCAL XlsColumnDelete(long sx, long lCount);
	BOOL PASCAL XlsColumnClear(long sx, long lCount);
	BOOL PASCAL XlsColumnInsert(long sx, long lCount);
	BOOL PASCAL XlsColumnCopy(long sx, long dx);
	BOOL PASCAL XlsColumnPaste(long dx);
	BOOL PASCAL XlsClear(LPCTSTR pszCurCell);
	BOOL PASCAL XlsCopy(LPCTSTR pszCurCell, LPCTSTR pszCell);
	BOOL PASCAL XlsPaste(LPCTSTR pszCurCell);
	long PASCAL XlsGetAttrNo(LPCTSTR pszCurCell);
    BOOL PASCAL XlsPutDouble2(double Double, LPSTR Cell, long xfno);
    BOOL PASCAL XlsPutLong2(long Long, LPSTR Cell, long xfno);
    BOOL PASCAL XlsPutStr2(LPSTR Str, LPSTR Cell, long xfno);
    BOOL PASCAL XlsPutValue2(VARIANT Value, LPSTR Cell, long xfno);
	BOOL PASCAL XlsGetSheetName2(long lSheetNo, LPSTR pszSheetName);
	int PASCAL XlsGetSheetNo2(LPCTSTR pszSheetName);
	BOOL PASCAL XlsVarCheck(LPCTSTR pszVarName);
	int PASCAL XlsSetSizeFree(double dblWidth, double dblHeight, LPCTSTR pszPaperName, LPCTSTR pszPrinterName);
	int PASCAL XlsSetPrintArea(int sx, int sy, int ex, int ey);
	int PASCAL XlsPutCenter(long lCenter);
	int PASCAL XlsPutPageOrder(long lPageOrder);
	void PASCAL XlsPutVarInsertMode(long lInsertMode);
	BOOL PASCAL XlsGetVarNamePos(LPCTSTR pszVarName, int* cx, int* cy, int nIndex);
	BOOL PASCAL XlsGetNamePos(LPCTSTR pszName, int* cx, int* cy);

    // �r���[�A�֐�
    HWND PASCAL ViewCreateWnd(HWND hwndParent, RECT *prc, UINT id);
    BOOL PASCAL ViewRelease(HWND hwnd);
    BOOL PASCAL ViewCreateClass(HINSTANCE hInst);
    BOOL PASCAL ViewReleaseClass();
    BOOL PASCAL ViewPutZoom(HWND hwnd, long nZoom);
    BOOL PASCAL ViewPutUrlName(HWND hwnd, LPSTR pUrlName);
    BOOL PASCAL ViewPutModeGrid(HWND hwnd, BOOL ModeGrid);
    BOOL PASCAL ViewPutSheetNo(HWND hwnd, long SheetNo);
    BOOL PASCAL ViewGetErrorNo(long *ErrorNo);
    BOOL PASCAL ViewGetErrorMessage(LPSTR ErrorMessage, int ErrorMessageSize);
    BOOL PASCAL ViewPutContextMenu(HWND hwnd, BOOL bContextMenu);
    BOOL PASCAL ViewPutOpenEnable(HWND hwnd, BOOL bOpenEnable);
    BOOL PASCAL ViewPutSaveEnable(HWND hwnd, BOOL bSaveEnable);
    BOOL PASCAL ViewPutPrintEnable(HWND hwnd, BOOL bPrintEnable);
    BOOL PASCAL ViewSetPrint(HWND hwnd);
    BOOL PASCAL ViewPrint(HWND hwnd, long nSheetNoStart,long nSheetNoEnd);
    BOOL PASCAL ViewPreView(HWND hwnd, long x, long y, long width, long height);

    // �R���o�[�g�֐�
    int  PASCAL EXCEL_Conv(char *);
    long PASCAL ConvGetCnt();
    BOOL PASCAL ConvGetErrorNo(long *ErrorNo);
    BOOL PASCAL ConvGetErrorMessage(LPSTR ErrorMessage, int ErrorMessageSize);
#ifdef  __cplusplus
}
#endif

// �N���G�[�^�֐�
typedef BOOL (CALLBACK* XLSPUTFORMATPROC)(LPSTR Format, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFONTNAMEPROC)(LPSTR FontName, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFONTPOINTPROC)(double FontPoint, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFONTCOLORPROC)(long FontColor, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFONTSTYLEPROC)(long FontStyle, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFONTULINEPROC)(long FontULine, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINETOPPROC)(long LineTop, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINEBOTTOMPROC)(long LineBottom, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINELEFTPROC)(long LineLeft, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINERIGHTPROC)(long LineRight, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINELEFTUPPROC)(long LineLeftUp, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLINERIGHTUPPROC)(long LineRightUp, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTPATTERNPROC)(long PatNo, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTBACKCOLORPROC)(long BackColor, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTPOSHORZPROC)(long PosHorz, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTPOSVERTPROC)(long PosVert, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTPOSTURNPROC)(long PosTurn, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTOVERRETURNPROC)(BOOL OverReturn, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFITPROC)(BOOL Fit, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTJOINTPROC)(BOOL Joint, LPSTR Cell);

typedef BOOL (CALLBACK* XLSPUTDOUBLEPROC)(double Double, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETDOUBLEPROC)(double *Double, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTLONGPROC)(long Long, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETLONGPROC)(long *Long, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTSTRPROC)(LPSTR Str, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETSTRPROC)(LPSTR Str, int StrSize, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETSTRWPROC)(WCHAR* Str, int StrSize, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTFUNCPROC)(LPSTR Func, VARIANT Ans, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTBOXPROC)(long LineStyle, long BoxType, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTVALUEPROC)(LPSTR Value, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETVALUEPROC)(LPSTR pVal, int ValueSize, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTVALUEVPROC)(VARIANT Value, LPSTR Cell);
typedef BOOL (CALLBACK* XLSGETVALUEVPROC)(VARIANT* pVal, LPSTR Cell);

typedef BOOL (CALLBACK* XLSPUTCOLWIDTHPROC)(long ColWidth, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTROWHEIGHTPROC)(double RowHeight, LPSTR Cell);
typedef BOOL (CALLBACK* XLSPUTREFSHEETPROC)(LPSTR RefSheet);
typedef BOOL (CALLBACK* XLSPUTSHEETNAMEPROC)(LPSTR SheetName);
typedef BOOL (CALLBACK* XLSPUTDEFFONTNAMEPROC)(LPSTR FontName);
typedef BOOL (CALLBACK* XLSPUTDEFFONTPOINTPROC)(double FontPoint);
typedef BOOL (CALLBACK* XLSMOVEPROC)(LPSTR Cell);

typedef BOOL (CALLBACK* XLSCREATEBOOKPROC)(LPSTR FileName, long Ver, long SheetMax, long *ID);
typedef BOOL (CALLBACK* XLSREADBOOKPROC)(LPSTR FileName, long *ID);
typedef BOOL (CALLBACK* XLSOPENBOOKPROC)(LPSTR FileName, LPSTR OverlayName, long *ID);
typedef BOOL (CALLBACK* XLSCLOSEBOOKPROC)(BOOL flg);
typedef BOOL (CALLBACK* XLSSELECTBOOKPROC)(long Book);
typedef BOOL (CALLBACK* XLSSHEETNOPROC)(long SheetNo);
typedef BOOL (CALLBACK* XLSPAGEMODEPROC)(long PageSize, long Scale, long Orientation, double HdrMergin, double FtrMergin);
typedef BOOL (CALLBACK* XLSSETMARGIN)(int Left, int Right, int Top, int Bottom);
typedef BOOL (CALLBACK* XLSADDSHEETPROC)(long SheetCnt);
typedef BOOL (CALLBACK* XLSDELSHEETPROC)(long SheetNo, long SheetCnt);
typedef BOOL (CALLBACK* XLSMODEGRIDPROC)(BOOL ModeGrid);

typedef BOOL (CALLBACK* XLSGETERRORNOPROC)(long *ErrorNo);
typedef BOOL (CALLBACK* XLSGETERRORMESSAGEPROC)(LPSTR ErrorMessage, int ErrorMessageSize);

typedef LPSTR (CALLBACK* CELLPROC)(LPSTR Cell, long x, long y);
typedef LPSTR (CALLBACK* POSPROC)(long sx, long sy, long ex, long ey);

typedef BOOL (CALLBACK* XLSGETSHEETNAMEPROC)(int sno, LPSTR pstr, int strsize);

typedef BOOL (CALLBACK* XLSGETSHEETCOUNTPROC)();
typedef BOOL (CALLBACK* XLSPUTBREAKPROC)(BOOL Break, LPSTR Cell);
typedef BOOL (CALLBACK* XLSSETHEADERPROC)(LPCTSTR pszLeft, LPCTSTR pszCenter, LPCTSTR pszRight);
typedef BOOL (CALLBACK* XLSSETFOOTERPROC)(LPCTSTR pszLeft, LPCTSTR pszCenter, LPCTSTR pszRight);
typedef BOOL (CALLBACK* XLSCOPYSHEETPROC)(long lSource, long lInsertPos, LPCTSTR pszSheetName);
typedef int (CALLBACK* XLSGETACTIVESHEETPROC)();
typedef BOOL (CALLBACK* XLSSETACTIVESHEETPROC)(long lSheetNo);
typedef BOOL (CALLBACK* XLSSETKEYWORDPROC)(LPCTSTR pszKeyWord);

typedef BOOL (CALLBACK* XLSSETZOOMPROC)(long lZoom);
typedef BOOL (CALLBACK* XLSGETLASTPOSPROC)(long* ex, long* ey, BOOL bDataOnly);
typedef BOOL (CALLBACK* XLSROWDELETEPROC)(long sy, long lCount);
typedef BOOL (CALLBACK* XLSROWCLEARPROC)(long sy, long lCount);
typedef BOOL (CALLBACK* XLSROWINSERTPROC)(long sy, long lCount);
typedef BOOL (CALLBACK* XLSROWCOPYPROC)(long sy, long dy);
typedef BOOL (CALLBACK* XLSROWPASTEPROC)(long dy);
typedef BOOL (CALLBACK* XLSCOLUMNDELETEPROC)(long sx, long lCount);
typedef BOOL (CALLBACK* XLSCOLUMNCLEARPROC)(long sx, long lCount);
typedef BOOL (CALLBACK* XLSCOLUMNINSERTPROC)(long sx, long lCount);
typedef BOOL (CALLBACK* XLSCOLUMNCOPYPROC)(long sx, long dx);
typedef BOOL (CALLBACK* XLSCOLUMNPASTEPROC)(long dx);
typedef BOOL (CALLBACK* XLSCLEARPROC)(LPCTSTR pszCurCell);
typedef BOOL (CALLBACK* XLSCOPYPROC)(LPCTSTR pszCurCell, LPCTSTR pszCell);
typedef BOOL (CALLBACK* XLSPASTEPROC)(LPCTSTR pszCurCell);
typedef long (CALLBACK* XLSGETATTRNOPROC)(LPCTSTR pszCurCell);
typedef BOOL (CALLBACK* XLSPUTDOUBLE2PROC)(double Double, LPSTR Cell, long xfno);
typedef BOOL (CALLBACK* XLSPUTLONG2PROC)(long Long, LPSTR Cell, long xfno);
typedef BOOL (CALLBACK* XLSSTR2PROC)(LPSTR Str, LPSTR Cell, long xfno);
typedef BOOL (CALLBACK* XLSVALUE2PROC)(VARIANT Value, LPSTR Cell, long xfno);
typedef BOOL (CALLBACK* XLSGETSHEETNAME2PROC)(long lSheetNo, LPSTR pszSheetName);
typedef int (CALLBACK* XLSGETSHEETNO2PROC)(LPCTSTR pszSheetName);

// �r���[�A�֐�
typedef HWND (CALLBACK* VIEWCREATEWNDPROC)(HWND hwndParent,RECT *prc,UINT id);
typedef BOOL (CALLBACK* VIEWCREATECLASSPROC)(HINSTANCE hInst);
typedef BOOL (CALLBACK* VIEWRELEASECLASSPROC)();
typedef BOOL (CALLBACK* VIEWPUTZOOMPROC)(HWND hwnd,long nZoom);
typedef BOOL (CALLBACK* VIEWPUTURLNAMEPROC)(HWND hwnd,LPSTR pUrlName);
typedef BOOL (CALLBACK* VIEWPUTMODEGRIDPROC)(HWND hwnd,BOOL ModeGrid);
typedef BOOL (CALLBACK* VIEWPUTSHEETNOPROC)(HWND hwnd,long SheetNo);
typedef BOOL (CALLBACK* VIEWGETERRORNOPROC)(long *ErrorNo);
typedef BOOL (CALLBACK* VIEWGETERRORMESSAGEPROC)(LPSTR ErrorMessage, int ErrorMessageSize);
typedef BOOL (CALLBACK* VIEWPUTCONTEXTMENUPROC)(HWND hwnd,BOOL bContextMenu);
typedef BOOL (CALLBACK* VIEWPUTOPENENABLEPROC)(HWND hwnd,BOOL bOpenEnable);
typedef BOOL (CALLBACK* VIEWPUTSAVEENABLEPROC)(HWND hwnd,BOOL bSaveEnable);
typedef BOOL (CALLBACK* VIEWPUTPRINTENABLEPROC)(HWND hwnd,BOOL bPrintEnable);
typedef BOOL (CALLBACK* VIEWSETPRINTPROC)(HWND hwnd);
typedef BOOL (CALLBACK* VIEWPRINTPROC)(HWND hwnd,long nSheetNoStart,long nSheetNoEnd);
typedef BOOL (CALLBACK* VIEWPREVIEWPROC)(HWND hwnd,long x,long y,long width,long height);

// �R���o�[�g�֐�
typedef int  (CALLBACK* EXCEL_CONVPROC)(char *);
typedef long (CALLBACK* CONVGETCNTPROC)();
typedef BOOL (CALLBACK* CONVGETERRORNOPROC)(long *ErrorNo);
typedef BOOL (CALLBACK* CONVGETERRORMESSAGEPROC)(LPSTR ErrorMessage, int ErrorMessageSize);

#endif	// __EXCELOCX_IDL_
#endif	// __CREATOROCX_RC_

#endif	// __XLSCRT3_H
