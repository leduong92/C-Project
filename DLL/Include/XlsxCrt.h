//////////////////////////////////////////////////////////////////////////////
// ExcelCreator Ver3.6

#ifndef __XLSXCRT_H__
#define __XLSXCRT_H__

//////////////////////////////////////////////////////////////////////////////
// �萔

// Excel �o�[�W����
#define _ver2007				2007					// Excel 2007
#define _ver2010				2010					// Excel 2010
#define _ver2013				2013					// Excel 2013				// EC8-rev100
#define _ver2013S				-2013					// Excel 2013 StrictXML		// EC8-rev100
// �J���[�p���b�g
#define _xclAuto				0x80000000				// ����
#define _xclBlack				0x80000008				// ��
#define _xclWhite				0x80000009				// ��
#define _xclRed					0x8000000A				// ��
#define _xclGreen				0x8000000B				// ��
#define _xclBlue				0x8000000C				// ��
#define _xclYellow				0x8000000D				// ��
#define _xclPink				0x8000000E				// �s���N
#define _xclCyan				0x8000000F				// ��
#define _xclDarkRed				0x80000010				// �Z����
#define _xclDarkGreen			0x80000011				// �Z����
#define _xclDrakBlue			0x80000012				// �Z����
#define _xclDrakYellow			0x80000013				// �Z����
#define _xclPurple				0x80000014				// ��
#define _xclBlueGreen			0x80000015				// ��
#define _xclGray25				0x80000016				// 25%�D�F
#define _xclGray50				0x80000017				// 50%�D�F
#define _xclGray				0x80000018				// �O���[
#define _xclPlum				0x80000019				// �v����
#define _xclIvory				0x8000001A				// �A�C�{���[
#define _xclLightBlue			0x8000001B				// �������F
#define _xclDrakPurple			0x8000001C				// �Z����
#define _xclCoral				0x8000001D				// �R�[����
#define _xclOceanBlue			0x8000001E				// �I�[�V�����u���[
#define _xclIceBlue				0x8000001F				// �A�C�X�u���[
#define _xclDrakBlue2			0x80000020				// �Z����
#define _xclPink2				0x80000021				// �s���N
#define _xclYellow2				0x80000022				// ���F
#define _xclCyan2				0x80000023				// ���F
#define _xclPurple2				0x80000024				// ��
#define _xclDarkRed2			0x80000025				// �Z����
#define _xclBlueGreen2			0x80000026				// ��
#define _xclBlue2				0x80000027				// ��
#define _xclSkyBlue				0x80000028				// �X�J�C�u���[
#define _xclLightCyan2			0x80000029				// �������F
#define _xclLightGreen			0x8000002A				// ������
#define _xclLightYellow			0x8000002B				// �������F
#define _xclVeilBlue			0x8000002C				// �x�[���u���[
#define _xclRose				0x8000002D				// ���[�Y
#define _xclLavender			0x8000002E				// ���x���_�[
#define _xclBeige				0x8000002F				// �x�[�W��
#define _xclLightBlue2			0x80000030				// ������
#define _xclAquaBlue			0x80000031				// �A�N�A
#define _xclLime				0x80000032				// ���C��
#define _xclGold				0x80000033				// �S�[���h
#define _xclLightOrange			0x80000034				// �����I�����W
#define _xclOrange				0x80000035				// �I�����W
#define _xclBlueGrey			0x80000036				// �u���[�O���[
#define _xclGray40				0x80000037				// 40%�D�F
#define _xclDarkBlueGreen		0x80000038				// �Z����
#define _xclSeaGreen			0x80000039				// �V�[�O���[��
#define _xclDarkGreen2			0x8000003A				// �Z����
#define _xclOlive				0x8000003B				// �I���[�u
#define _xclBrown				0x8000003C				// ��
#define _xclPlum2				0x8000003D				// �v����
#define _xclIndigo				0x8000003E				// �C���f�B�S
#define _xclGray80				0x8000003F				// 80%�D�F
// �t�H���g�X�^�C��
#define _xfsNormal				0x00					// �m�[�}��
#define _xfsBold				0x01					// �{�[���h
#define _xfsItalic				0x02					// �C�^���b�N
#define _xfsStrike				0x04					// �������
#define _xfsUp					0x08					// ��t��
#define _xfsDown				0x10					// ���t��
// �e�L�X�g�{�b�N�X�t�H���g�X�^�C��
#define _xtsNormal				0x00					// �m�[�}��
#define _xtsBold				0x01					// �{�[���h
#define _xtsItalic				0x02					// �C�^���b�N
// �t�H���g����
#define _xfuNone				0						// �Ȃ�
#define _xfuNormal				1						// ����
#define _xfuDouble				2						// ��d��
// �r���̎��
#define _xbsNone				0x00000000				// �Ȃ�
#define _xbsThin				0x01000000				// ����
#define _xbsMedium				0x02000000				// ����
#define _xbsDashed				0x03000000				// �j��
#define _xbsDotted				0x04000000				// �_��
#define _xbsThick				0x05000000				// �ɑ���
#define _xbsDouble				0x06000000				// ��d��
#define _xbsHair				0x07000000				// �׎���
#define _xbsMediumDashed		0x08000000				// ���j��
#define _xbsDashDot				0x09000000				// �׈�_����
#define _xbsMediumDashDot		0x0A000000				// ����_����
#define _xbsDashDotDot			0x0B000000				// �ד�_����
#define _xbsMediumDashDotDot	0x0C000000				// ����_����
#define _xbsSlantDashDot		0x0D000000				// ����_�΍���
// �p�^�[���̎��
#define _xpnMask				0x1F000000				// �}�X�N
#define _xpnSet(a)				((a)<<24)				// �ݒ�^�擾
#define _xpnGet(a)				(((a)&_xpnMask)>>24)
#define _xpnNone				0x01000000				// 01
#define _xpnSolid				0x02000000				// �b
#define _xpnMediumGray			0x03000000				// �b
#define _xpnDarkGray			0x04000000				// �b
#define _xpnLightGray			0x05000000				// �b
#define _xpnDarkHorizontal		0x06000000				// �b
#define _xpnDarkVertical		0x07000000				// �b
#define _xpnDarkDown			0x08000000				// �b
#define _xpnDarkUp				0x09000000				// �b
#define _xpnDarkGrid			0x0A000000				// 10
#define _xpnDarkTrellis			0x0B000000				// �b
#define _xpnLightHorizontal		0x0C000000				// �b
#define _xpnLightVertical		0x0D000000				// �b
#define _xpnLightDown			0x0E000000				// �b
#define _xpnLightUp				0x0F000000				// �b
#define _xpnLightGrid			0x10000000				// �b
#define _xpnLightTrellis		0x11000000				// �b
#define _xpnGray125				0x12000000				// �b
#define _xpnGray0625			0x13000000				// �b
// �{�b�N�X�̌`��
#define _xbtBox					0						// ����
#define _xbtLtc					1						// �i�q��
#define _xbtOver				2						// �㉡��
#define _xbtUnder				3						// ������
#define _xbtLeft				4						// ���c��
#define _xbtRight				5						// �E�c��
// �����ʒu
#define _xphGeneral				20						// �W��
#define _xphLeft				21						// ���l
#define _xphCenter				22						// ��������
#define _xphRight				23						// �E�l
#define _xphFill				24						// �J��Ԃ�
#define _xphJustify				25						// ���[����
#define _xphCenterContinuous	26						// �I��͈͓��Œ���
#define _xphDistributed			27						// �ϓ�
// �����ʒu
#define _xpvTop					1						// ��l��
#define _xpvCenter				2						// ��������
#define _xpvBottom				3						// ���l��
#define _xpvJustify				4						// ���[����
#define _xpvDistributed			5						// �ϓ�
// ��������
#define _xptHorz				100						// ��
#define _xptVert				101						// �c
#define _xptRvTurn90			102						// �����v����90��
#define _xptTurn90				103						// ���v����90��
// �p���T�C�Y
#define _xpsLetter				1						// ���^�[
#define _xpsRejoer				3						// ���W���[
#define _xpsRegal				5						// ���[�K��
#define _xpsExecutive			7						// �G�O�[�N�e�B�u
#define _xpsA3					8						// A3
#define _xpsA4					9						// A4
#define _xpsA5					11						// A5
#define _xpsB4					12						// B4
#define _xpsB5					13						// B5
// �p������
#define _xorLandscape			0						// ��
#define _xorDefault				1						// �W��
#define _xorPortrait			2						// �c
// �f�[�^�A�����͈�
#define _xarMaxPoint			0						// �ő�͈�
#define _xarEndPoint			1						// �G���h�|�C���g
// �y�[�W�����ʒu
#define _xpcNone				0x00					// �w��Ȃ�
#define _xpcHorz				0x01					// ����
#define _xpcVert				0x02					// ����
// �y�[�W�̕���
#define _xpoRightFromLeft		0						// ������E��
#define _xpoBottomFromTop		1						// �ォ�牺��
// �ϐ��������[�h
#define _xvmValidAll			0x00					// �S�č���
#define _xvmInvalidAnswer		0x01					// �Z���̂�
#define _xvmInvalidTextBox		0x02					// �v�Z���̂�
#define _xvmInvalidCellText		0x04					// �e�L�X�g�{�b�N�X
// ScaleMode
#define _xsmTwips				1						// 1/20 �|�C���g�P��
#define _xsmPoints				2						// �|�C���g�P��
#define _xsmPixels				3						// �s�N�Z���P��
#define _xsmInches				5						// �C���`�P��
#define _xsmMillimeters			6						// �~���P��
#define _xsmCentimeters			7						// �Z���`�P��
// �w�b�_/�t�b�^�[�w��
#define _xhmNormal				0x00					// �m�[�}�����[�h�F���݊�
#define _xhmOddEven				0x01					// ������ʃ��[�h
#define _xhmFirstPage			0x02					// �擪�y�[�W�ʃ��[�h
#define _xhmAll					0x03					// ������擪�y�[�W�ʃ��[�h
// �w�b�_/�t�b�^�[�w��
#define _xhtNormal				0x00					// �m�[�}���E��y�[�W
#define _xhtEven				0x01					// �����y�[�W
#define _xhtFirst				0x02					// �擪�y�[�W
// �G���[���
#define _errDontOpen			1						// �t�@�C�����I�[�v���ł��Ȃ�
#define _errFileHandle			2						// �����ȃt�@�C���n���h��
#define _errNotOpen				3						// ���I�[�v���G���[
#define _errSheetNo				4						// �V�[�g�ԍ��G���[
#define _errWrite				5						// �������ݎ��̃G���[
#define _errAction				6						// ���샂�[�h�G���[
#define _errDataCat				7						// �f�[�^��ʃG���[
#define _errErrorValue			8						// �G���[�l
#define _errClear				20						// �N���A�G���[
#define _errCopy				21						// �R�s�[�G���[
#define _errPaste				22						// �\��t���G���[
#define _errInsert				23						// �}���G���[
#define _errDelete				24						// �폜�G���[
#define _errLength				31						// �����G���[
#define _errLocate				32						// ���W�G���[
#define _errAttr				33						// �����ԍ��G���[
#define _errParam				34						// �p�����[�^�G���[
#define _errNoData				35						// �f�[�^������
#define _errEndOfData			36						// �f�[�^�̏I���
#define _errVarPoint			37						// �w�肵���ϐ������݂��Ȃ�
#define _errBreakCount			38						// ���y�[�W�����������z���܂���
#define _errMemory1				40						// �������[�s���G���[�P
#define _errMemory2				41						// �������[�s���G���[�Q
#define _errOther				50						// �����G���[
#define _errFunction			100						// �v�Z���`���G���[
#define _errCreate				300						// �t�@�C���쐬�G���[
#define _errTempCreate			301						// �ꎞ�t�@�C�����쐬�ł��Ȃ�
#define _errTempOpen			302						// �ꎞ�t�@�C�����I�[�v���ł��Ȃ�
#define _errStream				303						// �X�g���[���G���[
#define _errProtect				304						// �ی삳�ꂽ�V�[�g
#define _errMutex				400						// �~���[�e�b�N�X�G���[
// TextAnchor Type
#define _xacTop					1						// ��l��
#define _xacCenter				2						// ��������
#define _xacBottom				3						// ���l��
// TextVertical Type
#define _xtvEaVert				1						// �c����
#define _xtvHorz				2						// ������
#define _xtvVert				4						// �E��90����]
#define _xtvVert270				5						// ����90����]
#define _xtvWordArtVertRtl		7						// �c����(���p�����܂�)
// TextAlign Type
#define _xalCenter				1						// ��������
#define _xalLeft				5						// ���l��
#define _xalRight				6						// �E�l��
// Shape Line Style
#define _xlsNone				0x00000000				// ���Ȃ�
#define _xlsNormal				0x00000001				// ����
#define _xlsSysDot				0x00000002				// �ۓ_��
#define _xlsSysDash				0x00000003				// �p�_��
#define _xlsDash				0x00000004				// �j��
#define _xlsDashDot				0x00000005				// ��_����
#define _xlsLongDash			0x00000006				// ���j��
#define _xlsLongDashDot			0x00000007				// ����_����
#define _xlsLongDashDotDot		0x00000008				// ����_����
#define _xlsRound				0x00000009				// �׎���
#define _xltSingle				0x00000010				// ����
#define _xltDouble				0x00000020				// ��d��
#define _xltThickThin			0x00000030				// �オ������d��
#define _xltThinThick			0x00000040				// ����������d��
#define _xltTriple				0x00000050				// �O�d��
// Arrow Type
#define _xatNone				0x00000000				// �Ȃ�
#define _xatTriangle			0x00000001				// �O�p�`
#define _xatStealth				0x00000002				// ���C��
#define _xatDiamond				0x00000003				// �Ђ��`
#define _xatOval				0x00000004				// ��
#define _xatArrow				0x00000005				// �s�p
#define _xasSmall				0x00000000				// ������
#define _xasMedium				0x00000010				// ����
#define _xasLarge				0x00000020				// �傫��
#define _xasShort				0x00000000				// �Z��
#define _xasNormal				0x00000040				// ����
#define _xasLong				0x00000080				// ����
// Line Type
#define _xltLine				1						// ����
#define _xltLineInv				2						// LineInv
#define _xltStraightConnector1	96						// �������R�l�N�^
#define _xltBentConnector2		97						// �J�M���R�l�N�^2
#define _xltBentConnector3		98						// �J�M���R�l�N�^3
#define _xltBentConnector4		99						// �J�M���R�l�N�^4
#define _xltBentConnector5		100						// �J�M���R�l�N�^5
#define _xltCurvedConnector2	101						// �Ȑ��R�l�N�^2
#define _xltCurvedConnector3	102						// �Ȑ��R�l�N�^3
#define _xltCurvedConnector4	103						// �Ȑ��R�l�N�^4
#define _xltCurvedConnector5	104						// �Ȑ��R�l�N�^5
// Shape Type
#define _xstTriangle					3				// �񓙕ӎO�p�`
#define _xstRtTriangle					4				// ���p�O�p�`
#define _xstRect						5				// �����`/�����`
#define _xstDiamond						6				// �Ђ��`
#define _xstParallelogram				7				// ���s�l�ӌ`
#define _xstTrapezoid					8				// ��`
#define _xstNonIsoscelesTrapezoid		9				// NonIsoscelesTrapezoid
#define _xstPentagon					10				// �܊p�`
#define _xstHexagon						11				// �Z�p�`
#define _xstHeptagon					12				// ���p�`
#define _xstOctagon						13				// ���p�`
#define _xstDecagon						14				// �\�p�`
#define _xstDodecagon					15				// �\��p�`
#define _xstStar4						16				// �� 4
#define _xstStar5						17				// �� 5
#define _xstStar6						18				// �� 6
#define _xstStar7						19				// �� 7
#define _xstStar8						20				// �� 8
#define _xstStar10						21				// �� 10
#define _xstStar12						22				// �� 12
#define _xstStar16						23				// �� 16
#define _xstStar24						24				// �� 24
#define _xstStar32						25				// �� 32
#define _xstRoundRect					26				// �p�ێl�p�`
#define _xstRound1Rect					27				// 1 �̊p���ۂ߂��l�p�`
#define _xstRound2SameRect				28				// �Б��� 2 �̊p���ۂ߂��l�p�`
#define _xstRound2DiagRect				29				// �Ίp���� 2 �̊p���ۂ߂��l�p�`
#define _xstSnipRoundRect				30				// 1 �̊p���ۂ߂��l�p�`
#define _xstSnip1Rect					31				// 1 �̊p��؂������l�p�`
#define _xstSnip2SameRect				32				// �Б��� 2 �̊p��؂������l�p�`
#define _xstSnip2DiagRect				33				// �Ίp���� 2 �̊p��؂������l�p�`
#define _xstPlaque						34				// �u���[�`
#define _xstEllipse						35				// �~/�ȉ~
#define _xstTeardrop					36				// �܌`
#define _xstHomePlate					37				// �z�[���x�[�X
#define _xstChevron						38				// �R�`
#define _xstPieWedge					39				// PieWedge
#define _xstPie							40				// �p�C
#define _xstBlockArc					41				// �A�[�`
#define _xstDonut						42				// �h�[�i�c
#define _xstNoSmoking					43				// �֎~
#define _xstRightArrow					44				// �E���
#define _xstLeftArrow					45				// �����
#define _xstUpArrow						46				// ����
#define _xstDownArrow					47				// �����
#define _xstStripedRightArrow			48				// �X�g���C�v���
#define _xstNotchedRightArrow			49				// V ���`���
#define _xstBentUpArrow					50				// ���ܖ��
#define _xstLeftRightArrow				51				// ���E���
#define _xstUpDownArrow					52				// �㉺���
#define _xstLeftUpArrow					53				// ��������
#define _xstLeftRightUpArrow			54				// �O�������
#define _xstQuadArrow					55				// �l�������
#define _xstLeftArrowCallout			56				// ����󐁂��o��
#define _xstRightArrowCallout			57				// �E��󐁂��o��
#define _xstUpArrowCallout				58				// ���󐁂��o��
#define _xstDownArrowCallout			59				// ����󐁂��o��
#define _xstLeftRightArrowCallout		60				// ���E��󐁂��o��
#define _xstUpDownArrowCallout			61				// �㉺�u���b�N���t�����o��
#define _xstQuadArrowCallout			62				// �l������󐁂��o��
#define _xstBentArrow					63				// �Ȑܖ��
#define _xstUturnArrow					64				// U �^�[�����
#define _xstCircularArrow				65				// ����
#define _xstLeftCircularArrow			66				// LeftCircularArrow
#define _xstLeftRightCircularArrow		67				// LeftRightCircularArrow
#define _xstCurvedRightArrow			68				// �E�J�[�u���
#define _xstCurvedLeftArrow				69				// ���J�[�u���
#define _xstCurvedUpArrow				70				// ��J�[�u���
#define _xstCurvedDownArrow				71				// ���J�[�u���
#define _xstSwooshArrow					72				// SwooshArrow
#define _xstCube						73				// ������
#define _xstCan							74				// �~��
#define _xstLightningBolt				75				// ���
#define _xstHeart						76				// �n�[�g
#define _xstSun							77				// ���z
#define _xstMoon						78				// ��
#define _xstSmileyFace					79				// �X�}�C��
#define _xstIrregularSeal1				80				// ���� 1
#define _xstIrregularSeal2				81				// ���� 2
#define _xstFoldedCorner				82				// ����
#define _xstBevel						83				// �z��
#define _xstFrame						84				// �t���[��
#define _xstHalfFrame					85				// �t���[�� (����)
#define _xstCorner						86				// L ��
#define _xstDiagStripe					87				// �΂ߎ�
#define _xstChord						88				// ��
#define _xstArc							89				// �~��
#define _xstLeftBracket					90				// ���傩����
#define _xstRightBracket				91				// �E�傩����
#define _xstLeftBrace					92				// ����������
#define _xstRightBrace					93				// �E��������
#define _xstBracketPair					94				// �傩����
#define _xstBracePair					95				// ��������
#define _xstCallout1					105				// �������o�� 1 
#define _xstCallout2					106				// �������o�� 2 
#define _xstCallout3					107				// �������o�� 3 
#define _xstAccentCallout1				108				// �����������o�� 1
#define _xstAccentCallout2				109				// �����������o�� 2
#define _xstAccentCallout3				110				// �����������o�� 3
#define _xstBorderCallout1				111				// �������o�� 1 (�g�t��)
#define _xstBorderCallout2				112				// �������o�� 2 (�g�t��)
#define _xstBorderCallout3				113				// �������o�� 3 (�g�t��)
#define _xstAccentBorderCallout1		114				// �����������o�� 1 (�g�t��)
#define _xstAccentBorderCallout2		115				// �����������o�� 2 (�g�t��)
#define _xstAccentBorderCallout3		116				// �����������o�� 3 (�g�t��)
#define _xstWedgeRectCallout			117				// �l�p�`�����o��
#define _xstWedgeRoundRectCallout		118				// �p�ێl�p�`�����o��
#define _xstWedgeEllipseCallout			119				// �~�`�����o��
#define _xstCloudCallout				120				// �_�`�����o��
#define _xstCloud						121				// �_
#define _xstRibbon						122				// �����{��
#define _xstRibbon2						123				// �ナ�{��
#define _xstEllipseRibbon				124				// ���J�[�u ���{��
#define _xstEllipseRibbon2				125				// ��J�[�u ���{��
#define _xstLeftRightRibbon				126				// LeftRightRibbon
#define _xstVerticalScroll				127				// �c����
#define _xstHorizontalScroll			128				// ������
#define _xstWave						129				// ��g
#define _xstDoubleWave					130				// ���g
#define _xstPlus						131				// �\���`
#define _xstFlowChartProcess			132				// �t���[�`���[�g : ����
#define _xstFlowChartDecision			133				// �t���[�`���[�g : ���f
#define _xstFlowChartInputOutput		134				// �t���[�`���[�g : �f�[�^
#define _xstFlowChartPredefinedProcess	135				// �t���[�`���[�g : ��`�ςݏ��� 
#define _xstFlowChartInternalStorage	136				// �t���[�`���[�g : �����L��
#define _xstFlowChartDocument			137				// �t���[�`���[�g : ����
#define _xstFlowChartMultidocument		138				// �t���[�`���[�g : ��������
#define _xstFlowChartTerminator			139				// �t���[�`���[�g : �[�q
#define _xstFlowChartPreparation		140				// �t���[�`���[�g : ����
#define _xstFlowChartManualInput		141				// �t���[�`���[�g : �葀�����
#define _xstFlowChartManualOperation	142				// �t���[�`���[�g : ����
#define _xstFlowChartConnector			143				// �t���[�`���[�g : �����q
#define _xstFlowChartPunchedCard		144				// �t���[�`���[�g : �J�[�h
#define _xstFlowChartPunchedTape		145				// �t���[�`���[�g : ����E�e�[�v
#define _xstFlowChartSummingJunction	146				// �t���[�`���[�g : �a�ڍ�
#define _xstFlowChartOr					147				// �t���[�`���[�g : �_���a
#define _xstFlowChartCollate			148				// �t���[�`���[�g : �ƍ�
#define _xstFlowChartSort				149				// �t���[�`���[�g : ����
#define _xstFlowChartExtract			150				// �t���[�`���[�g : ���o��
#define _xstFlowChartMerge				151				// �t���[�`���[�g : �g����
#define _xstFlowChartOfflineStorage		152				// �t���[�`���[�g : OfflineStorage
#define _xstFlowChartOnlineStorage		153				// �t���[�`���[�g : �L���f�[�^
#define _xstFlowChartMagneticTape		154				// �t���[�`���[�g : �����A�N�Z�X�L��
#define _xstFlowChartMagneticDisk		155				// �t���[�`���[�g : ���C�f�B�X�N
#define _xstFlowChartMagneticDrum		156				// �t���[�`���[�g : ���ڃA�N�Z�X�L��
#define _xstFlowChartDisplay			157				// �t���[�`���[�g : �\��
#define _xstFlowChartDelay				158				// �t���[�`���[�g : �_���σQ�[�g
#define _xstFlowChartAlternateProcess	159				// �t���[�`���[�g : ��֏���
#define _xstFlowChartOffpageConnector	160				// �t���[�`���[�g : ���y�[�W�����q
#define _xstActionButtonBlank			161				// ActionButtonBlank
#define _xstActionButtonHome			162				// ActionButtonHome
#define _xstActionButtonHelp			163				// ActionButtonHelp
#define _xstActionButtonInformation		164				// ActionButtonInformation
#define _xstActionButtonForwardNext		165				// ActionButtonForwardNext
#define _xstActionButtonBackPrevious	166				// ActionButtonBackPrevious
#define _xstActionButtonEnd				167				// ActionButtonEnd
#define _xstActionButtonBeginning		168				// ActionButtonBeginning
#define _xstActionButtonReturn			169				// ActionButtonReturn
#define _xstActionButtonDocument		170				// ActionButtonDocument
#define _xstActionButtonSound			171				// ActionButtonSound
#define _xstActionButtonMovie			172				// ActionButtonMovie
#define _xstGear6						173				// Gear6
#define _xstGear9						174				// Gear9
#define _xstFunnel						175				// Funnel
#define _xstMathPlus					176				// ���Z�L��
#define _xstMathMinus					177				// ���Z�L��
#define _xstMathMultiply				178				// ��Z�L��
#define _xstMathDivide					179				// ���Z�L��
#define _xstMathEqual					180				// ����
#define _xstMathNotEqual				181				// �s����
#define _xstCornerTabs					182				// CornerTabs
#define _xstSquareTabs					183				// SquareTabs
#define _xstPlaqueTabs					184				// PlaqueTabs
#define _xstChartX						185				// ChartX
#define _xstChartStar					186				// ChartStar
#define _xstChartPlus					187				// ChartPlus
// �Z���N���A���[�h
#define _clAll							0x00            // �S�ăN���A
#define _clValue						0x01            // �l�̂�
#define _clAttr							0x02            // �����̂�

//////////////////////////////////////////////////////////////////////////////
// C/C++
#ifndef   __CREATOROCX_RC_
#ifndef   __EXCELOCX_IDL_
#ifndef   __EXCELCRTOCX_H_

// Excel �o�[�W����
#define ver2007					_ver2007				// Excel 2007
#define ver2010					_ver2010				// Excel 2010
#define ver2013					_ver2013				// Excel 2013			// EC8-rev100
#define ver2013S				_ver2013S				// Excel 2013 StrictXML	// EC8-rev100
// �J���[�p���b�g
#define xclAuto					_xclAuto				// ����
#define xclBlack				_xclBlack				// ��
#define xclWhite				_xclWhite				// ��
#define xclRed					_xclRed					// ��
#define xclGreen				_xclGreen				// ��
#define xclBlue					_xclBlue				// ��
#define xclYellow				_xclYellow				// ��
#define xclPink					_xclPink				// �s���N
#define xclCyan					_xclCyan				// ��
#define xclDarkRed				_xclDarkRed				// �Z����
#define xclDarkGreen			_xclDarkGreen			// �Z����
#define xclDrakBlue				_xclDrakBlue			// �Z����
#define xclDrakYellow			_xclDrakYellow			// �Z����
#define xclPurple				_xclPurple				// ��
#define xclBlueGreen			_xclBlueGreen			// ��
#define xclGray25				_xclGray25				// 25%�D�F
#define xclGray50				_xclGray50				// 50%�D�F
#define xclGray					_xclGray				// �O���[
#define xclPlum					_xclPlum				// �v����
#define xclIvory				_xclIvory				// �A�C�{���[
#define xclLightBlue			_xclLightBlue			// �������F
#define xclDrakPurple			_xclDrakPurple			// �Z����
#define xclCoral				_xclCoral				// �R�[����
#define xclOceanBlue			_xclOceanBlue			// �I�[�V�����u���[
#define xclIceBlue				_xclIceBlue				// �A�C�X�u���[
#define xclDrakBlue2			_xclDrakBlue2			// �Z����
#define xclPink2				_xclPink2				// �s���N
#define xclYellow2				_xclYellow2				// ���F
#define xclCyan2				_xclCyan2				// ���F
#define xclPurple2				_xclPurple2				// ��
#define xclDarkRed2				_xclDarkRed2			// �Z����
#define xclBlueGreen2			_xclBlueGreen2			// ��
#define xclBlue2				_xclBlue2				// ��
#define xclSkyBlue				_xclSkyBlue				// �X�J�C�u���[
#define xclLightCyan2			_xclLightCyan2			// �������F
#define xclLightGreen			_xclLightGreen			// ������
#define xclLightYellow			_xclLightYellow			// �������F
#define xclVeilBlue				_xclVeilBlue			// �x�[���u���[
#define xclRose					_xclRose				// ���[�Y
#define xclLavender				_xclLavender			// ���x���_�[
#define xclBeige				_xclBeige				// �x�[�W��
#define xclLightBlue2			_xclLightBlue2			// ������
#define xclAquaBlue				_xclAquaBlue			// �A�N�A
#define xclLime					_xclLime				// ���C��
#define xclGold					_xclGold				// �S�[���h
#define xclLightOrange			_xclLightOrange			// �����I�����W
#define xclOrange				_xclOrange				// �I�����W
#define xclBlueGrey				_xclBlueGrey			// �u���[�O���[
#define xclGray40				_xclGray40				// 40%�D�F
#define xclDarkBlueGreen		_xclDarkBlueGreen		// �Z����
#define xclSeaGreen				_xclSeaGreen			// �V�[�O���[��
#define xclDarkGreen2			_xclDarkGreen2			// �Z����
#define xclOlive				_xclOlive				// �I���[�u
#define xclBrown				_xclBrown				// ��
#define xclPlum2				_xclPlum2				// �v����
#define xclIndigo				_xclIndigo				// �C���f�B�S
#define xclGray80				_xclGray80				// 80%�D�F
// �t�H���g�X�^�C��
#define xfsNormal				_xfsNormal				// �m�[�}��
#define xfsBold					_xfsBold				// �{�[���h
#define xfsItalic				_xfsItalic				// �C�^���b�N
#define xfsStrike				_xfsStrike				// �������
#define xfsUp					_xfsUp					// ��t��
#define xfsDown					_xfsDown				// ���t��
// �e�L�X�g�{�b�N�X�t�H���g�X�^�C��
#define xtsNormal				_xtsNormal				// �m�[�}��
#define xtsBold					_xtsBold				// �{�[���h
#define xtsItalic				_xtsItalic				// �C�^���b�N
// �t�H���g����
#define xfuNone					_xfuNone				// �Ȃ�
#define xfuNormal				_xfuNormal				// ����
#define xfuDouble				_xfuDouble				// ��d��
// �����ʒu
#define xphGeneral				_xphGeneral				// �W��
#define xphLeft					_xphLeft				// ���l
#define xphCenter				_xphCenter				// ��������
#define xphRight				_xphRight				// �E�l
#define xphFill					_xphFill				// �J��Ԃ�
#define xphJustify				_xphJustify				// ���[����
#define xphCenterContinuous		_xphCenterContinuous	// �I��͈͓��Œ���
#define xphDistributed			_xphDistributed			// �ϓ�
// �����ʒu
#define xpvTop					_xpvTop					// ��l��
#define xpvCenter				_xpvCenter				// ��������
#define xpvBottom				_xpvBottom				// ���l��
#define xpvJustify				_xpvJustify				// ���[����
#define xpvDistributed			_xpvDistributed			// �ϓ�
// ��������
#define xptHorz					_xptHorz				// ��
#define xptVert					_xptVert				// �c
#define xptRvTurn90				_xptRvTurn90			// �����v����90��
#define xptTurn90				_xptTurn90				// ���v����90��
// �p���T�C�Y
#define xpsLetter				_xpsLetter				// ���^�[
#define xpsRejoer				_xpsRejoer				// ���W���[
#define xpsRegal				_xpsRegal				// ���[�K��
#define xpsExecutive			_xpsExecutive			// �G�O�[�N�e�B�u
#define xpsA3					_xpsA3					// A3
#define xpsA4					_xpsA4					// A4
#define xpsA5					_xpsA5					// A5
#define xpsB4					_xpsB4					// B4
#define xpsB5					_xpsB5					// B5
// �p������
#define xorLandscape			_xorLandscape			// ��
#define xorDefault				_xorDefault				// �W��
#define xorPortrait				_xorPortrait			// �c
// �f�[�^�A�����͈�
#define xarMaxPoint				_xarMaxPoint			// �ő�͈�
#define xarEndPoint				_xarEndPoint			// �G���h�|�C���g
// �{�b�N�X�̌`��
#define xbtBox					_xbtBox					// ����
#define xbtLtc					_xbtLtc					// �i�q��
#define xbtOver					_xbtOver				// �㉡��
#define xbtUnder				_xbtUnder				// ������
#define xbtLeft					_xbtLeft				// ���c��
#define xbtRight				_xbtRight				// �E�c��
// �r���̐���
#define xbsNone					_xbsNone				// �Ȃ�
#define xbsThin					_xbsThin				// ����
#define xbsMedium				_xbsMedium				// ����
#define xbsDashed				_xbsDashed				// �j��
#define xbsDotted				_xbsDotted				// �_��
#define xbsThick				_xbsThick				// �ɑ���
#define xbsDouble				_xbsDouble				// ��d��
#define xbsHair					_xbsHair				// �׎���
#define xbsMediumDashed			_xbsMediumDashed		// ���j��
#define xbsDashDot				_xbsDashDot				// �׈�_����
#define xbsMediumDashDot		_xbsMediumDashDot		// ����_����
#define xbsDashDotDot			_xbsDashDotDot			// �ד�_����
#define xbsMediumDashDotDot		_xbsMediumDashDotDot	// ����_����
#define xbsSlantDashDot			_xbsSlantDashDot		// ����_�΍���
// �p�^�[���ԍ�
#define xpnNone					_xpnNone				// 01
#define xpnSolid				_xpnSolid				// �b
#define xpnMediumGray			_xpnMediumGray			// �b
#define xpnDarkGray				_xpnDarkGray			// �b
#define xpnLightGray			_xpnLightGray			// �b
#define xpnDarkHorizontal		_xpnDarkHorizontal		// �b
#define xpnDarkVertical			_xpnDarkVertical		// �b
#define xpnDarkDown				_xpnDarkDown			// �b
#define xpnDarkUp				_xpnDarkUp				// �b
#define xpnDarkGrid				_xpnDarkGrid			// 10
#define xpnDarkTrellis			_xpnDarkTrellis			// �b
#define xpnLightHorizontal		_xpnLightHorizontal		// �b
#define xpnLightVertical		_xpnLightVertical		// �b
#define xpnLightDown			_xpnLightDown			// �b
#define xpnLightUp				_xpnLightUp				// �b
#define xpnLightGrid			_xpnLightGrid			// �b
#define xpnLightTrellis			_xpnLightTrellis		// �b
#define xpnGray125				_xpnGray125				// �b
#define xpnGray0625				_xpnGray0625			// �b
// �y�[�W�����ʒu
#define xpcNone					_xpcNone				// �w��Ȃ�
#define xpcHorz					_xpcHorz				// ����
#define xpcVert					_xpcVert				// ����
// �y�[�W�̕���
#define xpoRightFromLeft		_xpoRightFromLeft		// ������E��
#define xpoBottomFromTop		_xpoBottomFromTop		// �ォ�牺��
// �ϐ��������[�h
#define xvmValidAll				_xvmValidAll			// �������Ȃ�
#define xvmInvalidAnswer		_xvmInvalidAnswer		// �Z���̂�
#define xvmInvalidTextBox		_xvmInvalidTextBox		// �v�Z���̂�
#define xvmInvalidCellText		_xvmInvalidCellText		// �e�L�X�g�{�b�N�X
// ScaleMode
#define xsmTwips				_xsmTwips				// 1/20 �|�C���g�P��
#define xsmPoints				_xsmPoints				// �|�C���g�P��
#define xsmPixels				_xsmPixels				// �s�N�Z���P��
#define xsmInches				_xsmInches				// �C���`�P��
#define xsmMillimeters			_xsmMillimeters			// �~���P��
#define xsmCentimeters			_xsmCentimeters			// �Z���`�P��
// �w�b�_/�t�b�^���[�h
#define xhmNormal				_xhmNormal				// �m�[�}�����[�h�F���݊�
#define xhmOddEven				_xhmOddEven				// ������ʃ��[�h
#define xhmFirstPage			_xhmFirstPage			// �擪�y�[�W�ʃ��[�h
#define xhmAll					_xhmAll					// ������擪�y�[�W�ʃ��[�h
// �w�b�_/�t�b�^�^�C�v
#define xhtNormal				_xhtNormal				// �m�[�}���E��y�[�W
#define	xhtEven					_xhtEven				// �����y�[�W
#define xhtFirst				_xhtFirst				// �擪�y�[�W
// Variant Type
#define vtStr					0						// ������
#define vtDouble				1						// �����l
#define vtInt					2						// ����
#define vtFunc					3						// �v�Z��
#define vtNull					4						// �f�[�^�Ȃ�
// �G���[���
#define errDontOpen				_errDontOpen			// �t�@�C�����I�[�v���ł��Ȃ�
#define errFileHandle			_errFileHandle			// �����ȃt�@�C���n���h��
#define errNotOpen				_errNotOpen				// ���I�[�v���G���[
#define errSheetNo				_errSheetNo				// �V�[�g�ԍ��G���[
#define errWrite				_errWrite				// �������ݎ��̃G���[
#define errAction				_errAction				// ���샂�[�h�G���[
#define errDataCat				_errDataCat				// �f�[�^��ʃG���[
#define errErrorValue			_errErrorValue			// �G���[�l
#define errClear				_errClear				// �N���A�G���[
#define errCopy					_errCopy				// �R�s�[�G���[
#define errPaste				_errPaste				// �\��t���G���[
#define errInsert				_errInsert				// �}���G���[
#define errDelete				_errDelete				// �폜�G���[
#define errLength				_errLength				// �����G���[
#define errLocate				_errLocate				// ���W�G���[
#define errAttr					_errAttr				// �����ԍ��G���[
#define errParam				_errParam				// �p�����[�^�G���[
#define errNoData				_errNoData				// �f�[�^������
#define errEndOfData			_errEndOfData			// �f�[�^�̏I���
#define errVarPoint				_errVarPoint			// �w�肵���ϐ������݂��Ȃ�
#define errBreakCount			_errBreakCount			// ���y�[�W�����������z���܂���
#define errMemory1				_errMemory1				// �������[�s���G���[�P
#define errMemory2				_errMemory2				// �������[�s���G���[�Q
#define errOther				_errOther				// �����G���[
#define errFunction				_errFunction			// �v�Z���`���G���[
#define errCreate				_errCreate				// �t�@�C���쐬�G���[
#define errTempCreate			_errTempCreate			// �ꎞ�t�@�C�����쐬�ł��Ȃ�
#define errTempOpen				_errTempOpen			// �ꎞ�t�@�C�����I�[�v���ł��Ȃ�
#define errStream				_errStream				// �X�g���[���G���[
#define errProtect				_errProtect				// �ی삳�ꂽ�V�[�g
#define errMutex				_errMutex				// �~���[�e�b�N�X�G���[
// TextAnchor Type
#define xacTop					_xacTop					// ��l��
#define xacCenter				_xacCenter				// ��������
#define xacBottom				_xacBottom				// ���l��
// TextVertical Type
#define xtvEaVert				_xtvEaVert				// �c����
#define xtvHorz					_xtvHorz				// ������
#define xtvVert					_xtvVert				// �E��90����]
#define xtvVert270				_xtvVert270				// ����90����]
#define xtvWordArtVertRtl		_xtvWordArtVertRtl		// �c����(���p�����܂�)
// TextAlign Type
#define xalCenter				_xalCenter				// ��������
#define xalLeft					_xalLeft				// ���l��
#define xalRight				_xalRight				// �E�l��
// Shape Line Style
#define xlsNone					_xlsNone				// ���Ȃ�
#define xlsNormal				_xlsNormal				// ����
#define xlsSysDot				_xlsSysDot				// �ۓ_��
#define xlsSysDash				_xlsSysDash				// �p�_��
#define xlsDash					_xlsDash				// �j��
#define xlsDashDot				_xlsDashDot				// ��_����
#define xlsLongDash				_xlsLongDash			// ���j��
#define xlsLongDashDot			_xlsLongDashDot			// ����_����
#define xlsLongDashDotDot		_xlsLongDashDotDot		// ����_����
#define xlsRound				_xlsRound				// �׎���
#define xltSingle				_xltSingle				// ����
#define xltDouble				_xltDouble				// ��d��
#define xltThickThin			_xltThickThin			// �オ������d��
#define xltThinThick			_xltThinThick			// ����������d��
#define xltTriple				_xltTriple				// �O�d��
// Arrow Type
#define xatNone					_xatNone				// �Ȃ�
#define xatTriangle				_xatTriangle			// �O�p�`
#define xatStealth				_xatStealth				// ���C��
#define xatDiamond				_xatDiamond				// �Ђ��`
#define xatOval					_xatOval				// ��
#define xatArrow				_xatArrow				// �s�p
#define xasSmall				_xasSmall				// ������
#define xasMedium				_xasMedium				// ����
#define xasLarge				_xasLarge				// �傫��
#define xasShort				_xasShort				// �Z��
#define xasNormal				_xasNormal				// ����
#define xasLong					_xasLong				// ����
// Line Type
#define xltLine					_xltLine				// ����
#define xltLineInv				_xltLineInv				// LineInv
#define xltStraightConnector1	_xltStraightConnector1	// �������R�l�N�^
#define xltBentConnector2		_xltBentConnector2		// �J�M���R�l�N�^2
#define xltBentConnector3		_xltBentConnector3		// �J�M���R�l�N�^3
#define xltBentConnector4		_xltBentConnector4		// �J�M���R�l�N�^4
#define xltBentConnector5		_xltBentConnector5		// �J�M���R�l�N�^5
#define xltCurvedConnector2		_xltCurvedConnector2	// �Ȑ��R�l�N�^2
#define xltCurvedConnector3		_xltCurvedConnector3	// �Ȑ��R�l�N�^3
#define xltCurvedConnector4		_xltCurvedConnector4	// �Ȑ��R�l�N�^4
#define xltCurvedConnector5		_xltCurvedConnector5	// �Ȑ��R�l�N�^5
// Shape Type
#define xstTriangle						_xstTriangle					// �񓙕ӎO�p�`
#define xstRtTriangle					_xstRtTriangle					// ���p�O�p�`
#define xstRect							_xstRect						// �����`/�����`
#define xstDiamond						_xstDiamond						// �Ђ��`
#define xstParallelogram				_xstParallelogram				// ���s�l�ӌ`
#define xstTrapezoid					_xstTrapezoid					// ��`
#define xstNonIsoscelesTrapezoid		_xstNonIsoscelesTrapezoid		// NonIsoscelesTrapezoid
#define xstPentagon						_xstPentagon					// �܊p�`
#define xstHexagon						_xstHexagon						// �Z�p�`
#define xstHeptagon						_xstHeptagon					// ���p�`
#define xstOctagon						_xstOctagon						// ���p�`
#define xstDecagon						_xstDecagon						// �\�p�`
#define xstDodecagon					_xstDodecagon					// �\��p�`
#define xstStar4						_xstStar4						// �� 4
#define xstStar5						_xstStar5						// �� 5
#define xstStar6						_xstStar6						// �� 6
#define xstStar7						_xstStar7						// �� 7
#define xstStar8						_xstStar8						// �� 8
#define xstStar10						_xstStar10						// �� 10
#define xstStar12						_xstStar12						// �� 12
#define xstStar16						_xstStar16						// �� 16
#define xstStar24						_xstStar24						// �� 24
#define xstStar32						_xstStar32						// �� 32
#define xstRoundRect					_xstRoundRect					// �p�ێl�p�`
#define xstRound1Rect					_xstRound1Rect					// 1 �̊p���ۂ߂��l�p�`
#define xstRound2SameRect				_xstRound2SameRect				// �Б��� 2 �̊p���ۂ߂��l�p�`
#define xstRound2DiagRect				_xstRound2DiagRect				// �Ίp���� 2 �̊p���ۂ߂��l�p�`
#define xstSnipRoundRect				_xstSnipRoundRect				// 1 �̊p���ۂ߂��l�p�`
#define xstSnip1Rect					_xstSnip1Rect					// 1 �̊p��؂������l�p�`
#define xstSnip2SameRect				_xstSnip2SameRect				// �Б��� 2 �̊p��؂������l�p�`
#define xstSnip2DiagRect				_xstSnip2DiagRect				// �Ίp���� 2 �̊p��؂������l�p�`
#define xstPlaque						_xstPlaque						// �u���[�`
#define xstEllipse						_xstEllipse						// �~/�ȉ~
#define xstTeardrop						_xstTeardrop					// �܌`
#define xstHomePlate					_xstHomePlate					// �z�[���x�[�X
#define xstChevron						_xstChevron						// �R�`
#define xstPieWedge						_xstPieWedge					// PieWedge
#define xstPie							_xstPie							// �p�C
#define xstBlockArc						_xstBlockArc					// �A�[�`
#define xstDonut						_xstDonut						// �h�[�i�c
#define xstNoSmoking					_xstNoSmoking					// �֎~
#define xstRightArrow					_xstRightArrow					// �E���
#define xstLeftArrow					_xstLeftArrow					// �����
#define xstUpArrow						_xstUpArrow						// ����
#define xstDownArrow					_xstDownArrow					// �����
#define xstStripedRightArrow			_xstStripedRightArrow			// �X�g���C�v���
#define xstNotchedRightArrow			_xstNotchedRightArrow			// V ���`���
#define xstBentUpArrow					_xstBentUpArrow					// ���ܖ��
#define xstLeftRightArrow				_xstLeftRightArrow				// ���E���
#define xstUpDownArrow					_xstUpDownArrow					// �㉺���
#define xstLeftUpArrow					_xstLeftUpArrow					// ��������
#define xstLeftRightUpArrow				_xstLeftRightUpArrow			// �O�������
#define xstQuadArrow					_xstQuadArrow					// �l�������
#define xstLeftArrowCallout				_xstLeftArrowCallout			// ����󐁂��o��
#define xstRightArrowCallout			_xstRightArrowCallout			// �E��󐁂��o��
#define xstUpArrowCallout				_xstUpArrowCallout				// ���󐁂��o��
#define xstDownArrowCallout				_xstDownArrowCallout			// ����󐁂��o��
#define xstLeftRightArrowCallout		_xstLeftRightArrowCallout		// ���E��󐁂��o��
#define xstUpDownArrowCallout			_xstUpDownArrowCallout			// �㉺�u���b�N���t�����o��
#define xstQuadArrowCallout				_xstQuadArrowCallout			// �l������󐁂��o��
#define xstBentArrow					_xstBentArrow					// �Ȑܖ��
#define xstUturnArrow					_xstUturnArrow					// U �^�[�����
#define xstCircularArrow				_xstCircularArrow				// ����
#define xstLeftCircularArrow			_xstLeftCircularArrow			// LeftCircularArrow
#define xstLeftRightCircularArrow		_xstLeftRightCircularArrow		// LeftRightCircularArrow
#define xstCurvedRightArrow				_xstCurvedRightArrow			// �E�J�[�u���
#define xstCurvedLeftArrow				_xstCurvedLeftArrow				// ���J�[�u���
#define xstCurvedUpArrow				_xstCurvedUpArrow				// ��J�[�u���
#define xstCurvedDownArrow				_xstCurvedDownArrow				// ���J�[�u���
#define xstSwooshArrow					_xstSwooshArrow					// SwooshArrow
#define xstCube							_xstCube						// ������
#define xstCan							_xstCan							// �~��
#define xstLightningBolt				_xstLightningBolt				// ���
#define xstHeart						_xstHeart						// �n�[�g
#define xstSun							_xstSun							// ���z
#define xstMoon							_xstMoon						// ��
#define xstSmileyFace					_xstSmileyFace					// �X�}�C��
#define xstIrregularSeal1				_xstIrregularSeal1				// ���� 1
#define xstIrregularSeal2				_xstIrregularSeal2				// ���� 2
#define xstFoldedCorner					_xstFoldedCorner				// ����
#define xstBevel						_xstBevel						// �z��
#define xstFrame						_xstFrame						// �t���[��
#define xstHalfFrame					_xstHalfFrame					// �t���[�� (����)
#define xstCorner						_xstCorner						// L ��
#define xstDiagStripe					_xstDiagStripe					// �΂ߎ�
#define xstChord						_xstChord						// ��
#define xstArc							_xstArc							// �~��
#define xstLeftBracket					_xstLeftBracket					// ���傩����
#define xstRightBracket					_xstRightBracket				// �E�傩����
#define xstLeftBrace					_xstLeftBrace					// ����������
#define xstRightBrace					_xstRightBrace					// �E��������
#define xstBracketPair					_xstBracketPair					// �傩����
#define xstBracePair					_xstBracePair					// ��������
#define xstCallout1						_xstCallout1					// �������o�� 1 
#define xstCallout2						_xstCallout2					// �������o�� 2 
#define xstCallout3						_xstCallout3					// �������o�� 3 
#define xstAccentCallout1				_xstAccentCallout1				// �����������o�� 1
#define xstAccentCallout2				_xstAccentCallout2				// �����������o�� 2
#define xstAccentCallout3				_xstAccentCallout3				// �����������o�� 3
#define xstBorderCallout1				_xstBorderCallout1				// �������o�� 1 (�g�t��)
#define xstBorderCallout2				_xstBorderCallout2				// �������o�� 2 (�g�t��)
#define xstBorderCallout3				_xstBorderCallout3				// �������o�� 3 (�g�t��)
#define xstAccentBorderCallout1			_xstAccentBorderCallout1		// �����������o�� 1 (�g�t��)
#define xstAccentBorderCallout2			_xstAccentBorderCallout2		// �����������o�� 2 (�g�t��)
#define xstAccentBorderCallout3			_xstAccentBorderCallout3		// �����������o�� 3 (�g�t��)
#define xstWedgeRectCallout				_xstWedgeRectCallout			// �l�p�`�����o��
#define xstWedgeRoundRectCallout		_xstWedgeRoundRectCallout		// �p�ێl�p�`�����o��
#define xstWedgeEllipseCallout			_xstWedgeEllipseCallout			// �~�`�����o��
#define xstCloudCallout					_xstCloudCallout				// �_�`�����o��
#define xstCloud						_xstCloud						// �_
#define xstRibbon						_xstRibbon						// �����{��
#define xstRibbon2						_xstRibbon2						// �ナ�{��
#define xstEllipseRibbon				_xstEllipseRibbon				// ���J�[�u ���{��
#define xstEllipseRibbon2				_xstEllipseRibbon2				// ��J�[�u ���{��
#define xstLeftRightRibbon				_xstLeftRightRibbon				// LeftRightRibbon
#define xstVerticalScroll				_xstVerticalScroll				// �c����
#define xstHorizontalScroll				_xstHorizontalScroll			// ������
#define xstWave							_xstWave						// ��g
#define xstDoubleWave					_xstDoubleWave					// ���g
#define xstPlus							_xstPlus						// �\���`
#define xstFlowChartProcess				_xstFlowChartProcess			// �t���[�`���[�g : ����
#define xstFlowChartDecision			_xstFlowChartDecision			// �t���[�`���[�g : ���f
#define xstFlowChartInputOutput			_xstFlowChartInputOutput		// �t���[�`���[�g : �f�[�^
#define xstFlowChartPredefinedProcess	_xstFlowChartPredefinedProcess	// �t���[�`���[�g : ��`�ςݏ��� 
#define xstFlowChartInternalStorage		_xstFlowChartInternalStorage	// �t���[�`���[�g : �����L��
#define xstFlowChartDocument			_xstFlowChartDocument			// �t���[�`���[�g : ����
#define xstFlowChartMultidocument		_xstFlowChartMultidocument		// �t���[�`���[�g : ��������
#define xstFlowChartTerminator			_xstFlowChartTerminator			// �t���[�`���[�g : �[�q
#define xstFlowChartPreparation			_xstFlowChartPreparation		// �t���[�`���[�g : ����
#define xstFlowChartManualInput			_xstFlowChartManualInput		// �t���[�`���[�g : �葀�����
#define xstFlowChartManualOperation		_xstFlowChartManualOperation	// �t���[�`���[�g : ����
#define xstFlowChartConnector			_xstFlowChartConnector			// �t���[�`���[�g : �����q
#define xstFlowChartPunchedCard			_xstFlowChartPunchedCard		// �t���[�`���[�g : �J�[�h
#define xstFlowChartPunchedTape			_xstFlowChartPunchedTape		// �t���[�`���[�g : ����E�e�[�v
#define xstFlowChartSummingJunction		_xstFlowChartSummingJunction	// �t���[�`���[�g : �a�ڍ�
#define xstFlowChartOr					_xstFlowChartOr					// �t���[�`���[�g : �_���a
#define xstFlowChartCollate				_xstFlowChartCollate			// �t���[�`���[�g : �ƍ�
#define xstFlowChartSort				_xstFlowChartSort				// �t���[�`���[�g : ����
#define xstFlowChartExtract				_xstFlowChartExtract			// �t���[�`���[�g : ���o��
#define xstFlowChartMerge				_xstFlowChartMerge				// �t���[�`���[�g : �g����
#define xstFlowChartOfflineStorage		_xstFlowChartOfflineStorage		// �t���[�`���[�g : OfflineStorage
#define xstFlowChartOnlineStorage		_xstFlowChartOnlineStorage		// �t���[�`���[�g : �L���f�[�^
#define xstFlowChartMagneticTape		_xstFlowChartMagneticTape		// �t���[�`���[�g : �����A�N�Z�X�L��
#define xstFlowChartMagneticDisk		_xstFlowChartMagneticDisk		// �t���[�`���[�g : ���C�f�B�X�N
#define xstFlowChartMagneticDrum		_xstFlowChartMagneticDrum		// �t���[�`���[�g : ���ڃA�N�Z�X�L��
#define xstFlowChartDisplay				_xstFlowChartDisplay			// �t���[�`���[�g : �\��
#define xstFlowChartDelay				_xstFlowChartDelay				// �t���[�`���[�g : �_���σQ�[�g
#define xstFlowChartAlternateProcess	_xstFlowChartAlternateProcess	// �t���[�`���[�g : ��֏���
#define xstFlowChartOffpageConnector	_xstFlowChartOffpageConnector	// �t���[�`���[�g : ���y�[�W�����q
#define xstActionButtonBlank			_xstActionButtonBlank			// ActionButtonBlank
#define xstActionButtonHome				_xstActionButtonHome			// ActionButtonHome
#define xstActionButtonHelp				_xstActionButtonHelp			// ActionButtonHelp
#define xstActionButtonInformation		_xstActionButtonInformation		// ActionButtonInformation
#define xstActionButtonForwardNext		_xstActionButtonForwardNext		// ActionButtonForwardNext
#define xstActionButtonBackPrevious		_xstActionButtonBackPrevious	// ActionButtonBackPrevious
#define xstActionButtonEnd				_xstActionButtonEnd				// ActionButtonEnd
#define xstActionButtonBeginning		_xstActionButtonBeginning		// ActionButtonBeginning
#define xstActionButtonReturn			_xstActionButtonReturn			// ActionButtonReturn
#define xstActionButtonDocument			_xstActionButtonDocument		// ActionButtonDocument
#define xstActionButtonSound			_xstActionButtonSound			// ActionButtonSound
#define xstActionButtonMovie			_xstActionButtonMovie			// ActionButtonMovie
#define xstGear6						_xstGear6						// Gear6
#define xstGear9						_xstGear9						// Gear9
#define xstFunnel						_xstFunnel						// Funnel
#define xstMathPlus						_xstMathPlus					// ���Z�L��
#define xstMathMinus					_xstMathMinus					// ���Z�L��
#define xstMathMultiply					_xstMathMultiply				// ��Z�L��
#define xstMathDivide					_xstMathDivide					// ���Z�L��
#define xstMathEqual					_xstMathEqual					// ����
#define xstMathNotEqual					_xstMathNotEqual				// �s����
#define xstCornerTabs					_xstCornerTabs					// CornerTabs
#define xstSquareTabs					_xstSquareTabs					// SquareTabs
#define xstPlaqueTabs					_xstPlaqueTabs					// PlaqueTabs
#define xstChartX						_xstChartX						// ChartX
#define xstChartStar					_xstChartStar					// ChartStar
#define xstChartPlus					_xstChartPlus					// ChartPlus
// �Z���N���A���[�h
#define clAll							_clAll							// �S�ăN���A
#define clValue							_clValue						// �l�̂�
#define clAttr							_clAttr							// �����̂�

//////////////////////////////////////////////////////////////////////////////
// ExcelCreator API

#ifdef __cplusplus
extern "C" {  // Assume C declarations for C++
#endif // __cplusplus

    // �C�x���g�֐�
    typedef void (CALLBACK* XLSX_ONERRORPROC)(long *pError, LPARAM lp);
    // �C���X�^���X����
    BOOL PASCAL XlsxCreate(LPDWORD pID);
    BOOL PASCAL XlsxRelease(DWORD dwID);
    // Xlsx.
    BOOL PASCAL XlsxAbout(DWORD dwID);
    BOOL PASCAL XlsxCreateBook(DWORD dwID, LPCSTR pszFileName, int nVersion, int nSheetMax, int* pBookID);
    BOOL PASCAL XlsxCreateBookW(DWORD dwID, LPCWSTR pszFileName, int nVersion, int nSheetMax, int* pBookID);
    BOOL PASCAL XlsxOpenBook(DWORD dwID, LPCSTR pszFileName, LPCSTR pszOverlay, int* pBookID);
    BOOL PASCAL XlsxOpenBookW(DWORD dwID, LPCWSTR pszFileName, LPCWSTR pszOverlay, int* pBookID);
    BOOL PASCAL XlsxReadBook(DWORD dwID, LPCSTR pszFileName, int* pBookID);
    BOOL PASCAL XlsxReadBookW(DWORD dwID, LPCWSTR pszFileName, int* pBookID);
    BOOL PASCAL XlsxCloseBook(DWORD dwID, BOOL bSave);
    BOOL PASCAL XlsxSelectBook(DWORD dwID, int newVal);
    BOOL PASCAL XlsxPutDefFontName(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxPutDefFontNameW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxPutDefFontPoint(DWORD dwID, double newVal);
    BOOL PASCAL XlsxPutSheetNo(DWORD dwID, int newVal);
    BOOL PASCAL XlsxPutSheetName(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxPutSheetNameW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxGetSheetName(DWORD dwID, LPSTR pVal);
    BOOL PASCAL XlsxGetSheetNameW(DWORD dwID, LPWSTR pVal);
    int PASCAL XlsxGetSheetCount(DWORD dwID);
    BOOL PASCAL XlsxGetSheetName2(DWORD dwID, int nSheetNo, LPSTR pVal);
    BOOL PASCAL XlsxGetSheetName2W(DWORD dwID, int nSheetNo, LPWSTR pVal);
    int PASCAL XlsxGetSheetNo2(DWORD dwID, LPCSTR pszSheetName);
    int PASCAL XlsxGetSheetNo2W(DWORD dwID, LPCWSTR pszSheetName);
    BOOL PASCAL XlsxPutActiveSheet(DWORD dwID, int newVal);
    int PASCAL XlsxGetActiveSheet(DWORD dwID);
    BOOL PASCAL XlsxAddSheet(DWORD dwID, int nCount, int nInsertPos);
    BOOL PASCAL XlsxDelSheet(DWORD dwID, int nSheetNo, int nCount);
    BOOL PASCAL XlsxCopySheet(DWORD dwID, int nSource, int nInsertPos, LPCSTR pszSheetName);
    BOOL PASCAL XlsxCopySheetW(DWORD dwID, int nSource, int nInsertPos, LPCWSTR pszSheetName);
    BOOL PASCAL XlsxHeader(DWORD dwID, LPCSTR pszLeft, LPCSTR pszCenter, LPCSTR pszRight);
    BOOL PASCAL XlsxFooter(DWORD dwID, LPCSTR pszLeft, LPCSTR pszCenter, LPCSTR pszRight);
    BOOL PASCAL XlsxHeader2(DWORD dwID, LPCSTR pszLeft, LPCSTR pszCenter, LPCSTR pszRight, int nMode);
    BOOL PASCAL XlsxFooter2(DWORD dwID, LPCSTR pszLeft, LPCSTR pszCenter, LPCSTR pszRight, int nMode);
    BOOL PASCAL XlsxHeaderW(DWORD dwID, LPCWSTR pszLeft, LPCWSTR pszCenter, LPCWSTR pszRight);
    BOOL PASCAL XlsxFooterW(DWORD dwID, LPCWSTR pszLeft, LPCWSTR pszCenter, LPCWSTR pszRight);
    BOOL PASCAL XlsxHeader2W(DWORD dwID, LPCWSTR pszLeft, LPCWSTR pszCenter, LPCWSTR pszRight, int nMode);
    BOOL PASCAL XlsxFooter2W(DWORD dwID, LPCWSTR pszLeft, LPCWSTR pszCenter, LPCWSTR pszRight, int nMode);
    BOOL PASCAL XlsxPutHeaderFooterMode(DWORD dwID, int newVal);
    BOOL PASCAL XlsxPageMode(DWORD dwID, int nPaperSize, int nScale, int nOrientation, int nHeaderMargin, int nFooterMargin);
    BOOL PASCAL XlsxSetMargin(DWORD dwID, int nLeft, int nTop, int nRight, int nBottom);
    BOOL PASCAL XlsxSizeFree(DWORD dwID, double dblWidth, double dblHeight, LPCSTR pszPaperName, LPCSTR pszPrinterName);
    BOOL PASCAL XlsxSizeFreeW(DWORD dwID, double dblWidth, double dblHeight, LPCWSTR pszPaperName, LPCWSTR pszPrinterName);
    BOOL PASCAL XlsxPrintArea(DWORD dwID, int sx, int sy, int ex, int ey);
    BOOL PASCAL XlsxPrintTitle(DWORD dwID, int sy, int ey, int sx, int ex);
    BOOL PASCAL XlsxPutCenter(DWORD dwID, int newVal);
    BOOL PASCAL XlsxPutPageOrder(DWORD dwID, int newVal);
	BOOL PASCAL XlsxPutModeGrid(DWORD dwID, BOOL newVal);
    BOOL PASCAL XlsxPutZoom(DWORD dwID, int newVal);
	BOOL PASCAL XlsxPutKeyWord(DWORD dwID, LPCSTR newVal);
	BOOL PASCAL XlsxPutKeyWordW(DWORD dwID, LPCWSTR newVal);
	BOOL PASCAL XlsxPutVarInsertMode(DWORD dwID, int newVal);
	BOOL PASCAL XlsxPutScaleMode(DWORD dwID, int newVal);
	int PASCAL XlsxGetScaleMode(DWORD dwID);
    BOOL PASCAL XlsxRowDelete(DWORD dwID, int sy, int nCount);
    BOOL PASCAL XlsxRowClear(DWORD dwID, int sy, int nCount);
    BOOL PASCAL XlsxRowInsert(DWORD dwID, int sy, int nCount);
    BOOL PASCAL XlsxRowCopy(DWORD dwID, int sy, int dy);
    BOOL PASCAL XlsxRowPaste(DWORD dwID, int dy);
    BOOL PASCAL XlsxColumnDelete(DWORD dwID, int sx, int nCount);
    BOOL PASCAL XlsxColumnClear(DWORD dwID, int sx, int nCount);
    BOOL PASCAL XlsxColumnInsert(DWORD dwID, int sx, int nCount);
    BOOL PASCAL XlsxColumnCopy(DWORD dwID, int sx, int dx);
    BOOL PASCAL XlsxColumnPaste(DWORD dwID, int dx);
    BOOL PASCAL XlsxGetMaxData(DWORD dwID, int* ex, int* ey, int pt);
    BOOL PASCAL XlsxGetMaxArea(DWORD dwID, int* ex, int* ey, int pt);
	BOOL PASCAL XlsxVarCheck(DWORD dwID, LPCSTR pszVarName);
	BOOL PASCAL XlsxVarCheckW(DWORD dwID, LPCWSTR pszVarName);
	BOOL PASCAL XlsxGetVarNamePos(DWORD dwID, LPCSTR pszVarName, int* cx, int* cy, int nIndex);
	BOOL PASCAL XlsxGetVarNamePosW(DWORD dwID, LPCWSTR pszVarName, int* cx, int* cy, int nIndex);
	BOOL PASCAL XlsxGetNamePos(DWORD dwID, LPCSTR pszName, int* cx, int* cy);
	BOOL PASCAL XlsxGetNamePosW(DWORD dwID, LPCWSTR pszName, int* cx, int* cy);
	BOOL PASCAL XlsxInitFormulaAnswer(DWORD dwID, BOOL bInit);
	BOOL PASCAL XlsxImageFileNonDeleteMode(DWORD dwID, BOOL bInit);
	BOOL PASCAL XlsxFullCalcOnLoad(DWORD dwID, BOOL bLoad);
	BOOL PASCAL XlsxChangeRefSheetAddress(DWORD dwID, BOOL bLoad);
    int PASCAL XlsxGetErrorNo(DWORD dwID);
    BOOL PASCAL XlsxGetErrorMessage(DWORD dwID, LPSTR pVal);
    BOOL PASCAL XlsxGetErrorMessageW(DWORD dwID, LPWSTR pVal);
    // Xlsx.Cell/Pos
    BOOL PASCAL XlsxCell(DWORD dwID, LPCSTR pszCell, int xoff, int yoff);
    BOOL PASCAL XlsxCellW(DWORD dwID, LPCWSTR pszCell, int xoff, int yoff);
    BOOL PASCAL XlsxPos(DWORD dwID, int sx, int sy, int ex, int ey);
    // Xlsx.Cell/Pos.Value
    BOOL PASCAL XlsxPutValue(DWORD dwID, VARIANT newVal);
    BOOL PASCAL XlsxGetValue(DWORD dwID, VARIANT* pVal);
    BOOL PASCAL XlsxPutValue2(DWORD dwID, VARIANT newVal, int xfno);
    BOOL PASCAL XlsxPutLong(DWORD dwID, long newVal);
    long PASCAL XlsxGetLong(DWORD dwID);
    BOOL PASCAL XlsxPutLong2(DWORD dwID, long newVal, int xfno);
    BOOL PASCAL XlsxPutDouble(DWORD dwID, double newVal);
    double PASCAL XlsxGetDouble(DWORD dwID);
    BOOL PASCAL XlsxPutDouble2(DWORD dwID, double newVal, int xfno);
    BOOL PASCAL XlsxPutString(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxGetString(DWORD dwID, LPSTR pVal);
    BOOL PASCAL XlsxPutString2(DWORD dwID, LPCSTR newVal, int xfno);
    BOOL PASCAL XlsxPutStringW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxGetStringW(DWORD dwID, LPWSTR pVal);
    BOOL PASCAL XlsxPutString2W(DWORD dwID, LPCWSTR newVal, int xfno);
    BOOL PASCAL XlsxPutInt64(DWORD dwID, INT64 newVal);
    INT64 PASCAL XlsxGetInt64(DWORD dwID);
    BOOL PASCAL XlsxPutInt642(DWORD dwID, INT64 newVal, int xfno);
    BOOL PASCAL XlsxPutFunc(DWORD dwID, LPCSTR pszFunc, VARIANT vAnswer);
    BOOL PASCAL XlsxPutFuncW(DWORD dwID, LPCWSTR pszFunc, VARIANT vAnswer);
    BOOL PASCAL XlsxPutColWidth(DWORD dwID, double newVal);
    double PASCAL XlsxGetColWidth(DWORD dwID);
    BOOL PASCAL XlsxPutRowHeight(DWORD dwID, double newVal);
    double PASCAL XlsxGetRowHeight(DWORD dwID);
    BOOL PASCAL XlsxPutBreak(DWORD dwID, BOOL newVal);
    int PASCAL XlsxGetAttrNo(DWORD dwID);
    BOOL PASCAL XlsxCellClear(DWORD dwID);
    BOOL PASCAL XlsxCellCopy(DWORD dwID, LPCSTR pszCell);
    BOOL PASCAL XlsxCellCopyW(DWORD dwID, LPCWSTR pszCell);
    BOOL PASCAL XlsxCellPaste(DWORD dwID);
    BOOL PASCAL XlsxCellClear2(DWORD dwID, int nClearMode);
    // Xlsx.Cell/Pos.Attr
    BOOL PASCAL XlsxAttrPutFormat(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxAttrPutFormatW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxAttrPutFontName(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxAttrPutFontNameW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxAttrPutFontPoint(DWORD dwID, double newVal);
    BOOL PASCAL XlsxAttrPutFontColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutFontStyle(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutFontULine(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutLineTop(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutLineBottom(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutLineLeft(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutLineRight(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutLineLeftUp(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutLineRightUp(DWORD dwID, int nBorderStyle, int nColor);
    BOOL PASCAL XlsxAttrPutPattern(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutForeColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutBackColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutPosHorz(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutPosVert(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutPosTurn(DWORD dwID, int newVal);
    BOOL PASCAL XlsxAttrPutOverReturn(DWORD dwID, BOOL newVal);
    BOOL PASCAL XlsxAttrPutFit(DWORD dwID, BOOL newVal);
    BOOL PASCAL XlsxAttrPutJoint(DWORD dwID, BOOL newVal);
    BOOL PASCAL XlsxAttrPutBox(DWORD dwID, int nBorderStyle, int nBoxType, int nColor);
    // Xlsx.Cell/Pos.Drawing
	BOOL PASCAL XlsxDrawingInit(DWORD dwID);
    BOOL PASCAL XlsxDrawingPutLineColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutLineWeight(DWORD dwID, double newVal);
    BOOL PASCAL XlsxDrawingPutLineStyle(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutArrowTypeS(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutArrowTypeE(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutFillColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutFontName(DWORD dwID, LPCSTR newVal);
    BOOL PASCAL XlsxDrawingPutFontNameW(DWORD dwID, LPCWSTR newVal);
    BOOL PASCAL XlsxDrawingPutFontPoint(DWORD dwID, double newVal);
    BOOL PASCAL XlsxDrawingPutFontColor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutFontStyle(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutFontULine(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutTextAnchor(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutTextVertical(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingPutTextAlign(DWORD dwID, int newVal);
    BOOL PASCAL XlsxDrawingAddImage(DWORD dwID, LPCSTR pszFileName, double sxoff, double syoff, 
		double exoff, double eyoff, double width, double height);
    BOOL PASCAL XlsxDrawingAddImageW(DWORD dwID, LPCWSTR pszFileName, double sxoff, double syoff, 
		double exoff, double eyoff, double width, double height);
    BOOL PASCAL XlsxDrawingAddLine(DWORD dwID, int nLineType, double sxoff, double syoff, 
		double exoff, double eyoff, double width, double height);
    BOOL PASCAL XlsxDrawingAddShape(DWORD dwID, int nShapeType, double sxoff, double syoff, 
		double exoff, double eyoff, double width, double height);
    BOOL PASCAL XlsxDrawingAddTextBox(DWORD dwID, int nShapeType, LPCSTR pszText, 
		double sxoff, double syoff, double exoff, double eyoff, double width, double height);
    BOOL PASCAL XlsxDrawingAddTextBoxW(DWORD dwID, int nShapeType, LPCWSTR pszText, 
		double sxoff, double syoff, double exoff, double eyoff, double width, double height);
	// Xlsx.�C�x���g
    BOOL PASCAL XlsxSetOnErrorProc(DWORD dwID, XLSX_ONERRORPROC proc, LPARAM lParam);

#ifdef  __cplusplus
}
#endif //__cplusplus

//////////////////////////////////////////////////////////////////////////////

#endif	// __EXCELCRTOCX_H_
#endif	// __EXCELOCX_IDL_
#endif	// __CREATOROCX_RC_

//////////////////////////////////////////////////////////////////////////////

#endif	// __XLSXCRT_H
