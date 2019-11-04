#pragma pack(push,8)

//////////////////////////////////////////////////////////////////
// Last mod on 1997-7-25 by cmy
// Mod on 1999-2-8 by cmy for netmask
// Mod on 1999-2-21 by cmy for security


#define	MTOM_VER_1_2				1	// Version 1.2 for netmask
#define	MTOM_VER_1_5				1	// Version 1.5 for security
#define	MTOM_PCCARD					1	// PC CARD feature
#define	MTOM_VER_2_2				1	// Version 2.2
#define	MTOM_VER_3_0				1	// Version 3.0

//////////////////////////////////////////////////////////////////

#define GP_SCREEN_X					640		// �f�o�̉�ʂ̍ő�w�h�b�g�T�C�Y
#define GP_SCREEN_Y					480		// �f�o�̉�ʂ̍ő�x�h�b�g�T�C�Y



/////////////////////////////////////////////////////////////////
// Default for API

#define MTOM_DEFAULT_PORTNO					8000			// �f�t�H���g�̃|�[�g�ԍ�

#define	MTOM_DEFAULT_FIND_NODE_TIME			(2*1000)		// [mS], Find node time-out
#define	MTOM_DEFAULT_RESPONSE_TIME			(15*1000)		// [mS], Response time-out
#define	MTOM_DEFAULT_CHAR_TIME				(10*1000)		// [mS], Character time-our
#define	MTOM_DEFAULT_UDP_TIME				50				// [mS], Multi block delay time

#define	MTOM_DEFAULT_N_LAN_NETWORKDATA		1				// Initial network data allocate size



/////////////////////////////////////////////////////////////////

struct tagMtoMSOCK ;
typedef struct tagMtoMSOCK MtoMSOCK,*LPMtoMSOCK ;
typedef BOOL (CALLBACK* MTOMCALLBACK)(LPMtoMSOCK,int);									// �`�o�h�I����ʒm����R�[���o�b�N�֐��̌^�錾
typedef void (CALLBACK* MTOMEVENTBACK)(LPMtoMSOCK,int,DWORD dwParam1,DWORD dwParam2);	// �`�o�h����̃C�x���g������,�ʒm����R�[���o�b�N�֐��̌^�錾


/////////////////////////////////////////////////////////////////
// Result of API, finish callback, event callback

#define MTOM_OK						0x00000000		//	�����͐���I������
#define MTOM_CONTINUE				0x00000001		//	�����͌p����
#define MTOM_PROCESS_FINISH			0x00000002		//	�P�v���Z�X������I�����܂���,��������,�������p�����܂�
#define MTOM_USER_STOPED			0x00000003   	//	���[�U�[�A�v���P�[�V�����ɂ���ď��������f����܂���
													//  �iMtoMStop()���R�[�����ꂽ���R�[���o�b�N�֐���FALSE���Ԃ��ꂽ)

#define MTOM_EVENT_TOUCH  			0x00000040		//  �^�b�`�p�l���������ꂽ
#define MTOM_EVENT_CLOSED   		0x00000041 		//  �R�l�N�V�������ؒf���ꂽ
#if	MTOM_PRONET
#define MTOM_EVENT_BEL	   			0x00000042 		//  BEL code = PFN data
#endif

#define	MTOM_ERROR					0x00000080		// Error, Detail code means unknown or result code of response
#define	MTOM_ERROR_INVALID			0x00000081		// Invalid API call or parameter
#define	MTOM_ERROR_LAN				0x00000082		// LAN error, Detail code means result of LAN API
#define	MTOM_ERROR_TOUT_RES			0x00000083		// Response time-out error
#define	MTOM_ERROR_TOUT_CHAR		0x00000084		// Character time-out error
#define	MTOM_ERROR_NAK				0x00000085		// NAK was Received, Detail code means NAK code
#if	MTOM_VER_1_5 && MTOM_PRONET
#define	MTOM_ERROR_REMOTE_ACCESS	0x00000086		// Password unmatch
#endif
#if	MTOM_PCCARD
#define	MTOM_ERROR_FILE				0x00000087		// File access error
#endif
#define	MTOM_ERROR_MASK				0xffffff80		// Mask for errors
#define	MTOM_ERROR_BASE				0x00000080		// Base after masked



/////////////////////////////////////////////////////////////////

typedef struct tagPROTOCOLVERSION {			// ���ڒʐM�v���g�R���o�[�W�������
		DWORD dwID ;							// �ʐM�v���g�R���̂h�c(�o�C�i���[�l)
		BYTE szProtocolName[32] ;				// �ʐM�v���g�R���̖�			(ASCIIZ ������)
		BYTE szProtocolVersion[32] ; 			// �ʐM�v���g�R���̃o�[�W����	(ASCIIZ ������)
#define MTOM_PROTOCOL_MAX	16					// �ő哋�ډ\�v���g�R����
} PROTOCOLVERSION ,*LPPROTOCOLVERSION ;

typedef struct tagGPVERSION {				// �o�[�W�������
	BOOL	bGood ;								// �o�[�W������񂪗L�����ǂ���
													// �o�[�W��������L���ɂ���ɂ�MtoMDLE_Communication()����x���s���Ă�������
													// TURE:�o�[�W�������͗L���ł�
													// FALSE:�o�[�W�������͗L���ł͂���܂��� 

	BYTE szMachineName[32] ;					// �@�햼			(ASCIIZ ������)
	BYTE szMachineVersion[32] ; 				// �@��o�[�W����	(ASCIIZ ������)
	BYTE szMachineDate[32] ;					// �@����t			(ASCIIZ ������)
	
	DWORD dwProtocolCounter ;					// ���ڒʐM�v���g�R����
	PROTOCOLVERSION Protocl[MTOM_PROTOCOL_MAX] ;// ���ڒʐM�v���g�R���o�[�W�������

} GPVERSION,*LPGPVERSION ;


typedef struct tagGPTIMERS {
	DWORD  dwResponsTimer ;						// �R�}���h���M��,���X�|���X����M����܂ł̎���
												// ���̒l��dwChactorTimer��蒷�����ԂłȂ���΂Ȃ�܂���
												// (�P�� �����j

	DWORD  dwChactorTimer ;						// �L�����N�^�[�ԃ^�C�}�[
												// �P�t���[���̎�M���J�n����,���̃t���[�����r���œr�؂ꂽ�ꍇ
												// �Ăуt���[���̎c�肪��M���ĉ���܂ł̍Œ�����
												// ���̒l��TCP�̍đ����ԋy��dwUDPPacketTimer��蒷�����ԂłȂ���΂Ȃ�܂���
												// (�P�� �����j

	DWORD	dwUDPPacketTimer ;					// UDP/IP�𗘗p������M�ŘA�����đ���m�[�h�Ƀp�P�b�g�𑗐M�����ꍇ
												// ����m�[�h�������d�؂�Ȃ��ꍇ������܂��B
												// �����h���ׂ�,����m�[�h�̏����̉��؎��Ԃ��w�肵�܂�
												// �`�o�h�͕����̘A������UDP�p�P�b�g�𑗐M����ꍇ
												// �P�p�P�b�g���M��,���̃^�C�}�[�Ŏw�肳�ꂽ����Wait���Ă��玟��
												// �p�P�b�g�𑗐M���܂�
												// (�P�� ����)

												
} GPTIMERS,*LPGPTIMERS ;


typedef struct tagGPNode {					// �m�[�h ���R�[�h
	DWORD dwNodeStatus 	;						// �m�[�h�̏��
#define B_dwNodeStatus_Nothing		0				// ��̃m�[�h  ���R�[�h������
#define B_dwNodeStatus_Find			1			// �m�[�h�͑��݂���(�����͐���ɏI������)
#define B_dwNodeStatus_NotFind		2			// �m�[�h�����݂��Ȃ�(�����ُ͈�I������)
#define B_dwNodeStatus_NonAction	3			// ���̃m�[�h�͏����̑ΏۊO�Ȃ̂ŏ�������Ȃ�����


	BOOL dwCheckButton ;						// �����̑Ώۂɂ��邩�ǂ���(�A�v���P�[�V�������[�U�[���Z�b�g����)
													// TRUE:�ΏۂƂ���
													// FALSE:�ΏۂƂ��Ȃ�

	DWORD dwIPAddress ;							// �m�[�h�̂h�o�A�h���X

} GPNODE,*LPGPNODE;

typedef struct tagGPNetWorkData {			// �l�b�g���[�N�̏��
	DWORD dwNetID ;								// ��ē��񎞂̑Ώۃl�b�g�h�c(�f�t�H���g�͎��l�b�g�h�c�j
	DWORD dwNodeCounter ;						// �m�[�h ���R�[�h�̐�
	struct tagGPNode Node[1]	;				// �X�̃m�[�h�̏��(�m�[�h ���R�[�h)
												// �i�z��̗v�f����1�Œ�`����Ă��܂������ۂ͓��I�ɕω����܂�,dwNodeCounter
												//   �v�f���͏��dwNodeCounter���Q�Ƃ���悤�ɂ��Ă�������)
} GPNETWORKDATA,*LPGPNETWORKDATA ;



		// �l�����l�p�\�P�b�g�̌^��`
struct tagMtoMSOCK {
	DWORD dwStockSize ;							// �\�P�b�g�̗L���f�[�^�T�C�Y ( sizeof(MtoMSOCK) )

	DWORD dwProtocolType ;						// �v���g�R���̎��
#define B_ProtocolType_SIO	1						// �v���g�R���̎�ނ͂l�����l �r�h�n
#define B_ProtocolType_LAN	2						// �v���g�R���̎�ނ͂l�����l �k�`�m

	DWORD dwMtoM_Mode ;							// ���݂̃\�P�b�g�̓��샂�[�h
#define B_MtoM_Mode_ESC		0					// �`�惂�[�h	(�\�P�b�g�쐬���̃f�t�H���g)
#define B_MtoM_Mode_DLE		1					// �f�[�^�]�����[�h	(MtoMDLE_Communication()�`MtoMDLE_Quit()�܂�)


	DWORD  dwUser1 ;							// �\�P�b�g���[�U�[���g�p���Ă悢�R�Q�r�b�g�̃��[�N
													// �ʏ�R�[���o�b�N��,�\�P�b�g�����ʂ���ׂ̂��̂��\�P�b�g���[�U�[���Z�b�g����
													// ���̒l���`�o�h���ύX���鎖�͂���܂���

	DWORD  dwUser2 ;							// �\�P�b�g���[�U�[���g�p���Ă悢�R�Q�r�b�g�̃��[�N���̂Q
													// ���̒l���`�o�h���ύX���鎖�͂���܂���


	
	GPVERSION  Version ;						// �o�[�W�������

	MTOMEVENTBACK  pfEventFunc ;				// �C�x���g������,�R�[���o�b�N����֐�
												// NULL:�R�[���o�b�N���Ȃ�(�f�t�H���g)

	MTOMEVENTBACK  pfSyncProcess;				// �������[�h�Ŏ��s��,�����̐i�s�󋵂�ʒm���鎞,�R�[���o�b�N����֐�
												// NULL:�R�[���o�b�N���Ȃ�(�f�t�H���g)

												// �����̐i�݋�������v���O���X�o�[�p�f�[�^
	DWORD  dwTotalProcess ;						// �S�s����
	DWORD  dwFinishedProcess ;					// �I�������s����
												// ��  �����̐i�݋�������l�͂����܂ł��r�W���A���I�Ȃ��̂�
												//     ���̒l�����ɏ����̏I����ۏ؂�����̂ł͂���܂���


	DWORD dwGPNetWorkDataSize ;					// �l�b�g���[�N���̃T�C�Y
	LPGPNETWORKDATA pGPNetWorkData ;			// �l�b�g���[�N���G���A�������|�C���^�[


	GPTIMERS GpTimers ;							// �e��^�C�}�[�̃^�C�}�[�l
		


	// �ȉ��v���g�R���ʂ̃��[�N���Â�
	DWORD dwTransitionType ;					// ��M���@
#define B_dwTransitionType_Only1			0		// �P�m�[�h�w��,�R�l�N�V�������J�݂����m�[�h�ɑ΂��Č�M���s���܂�
													// �f�t�H���g
													// (TCP/IP�𗘗p���܂�)
								

#define B_dwTransitionType_BroadCast 		1		// �s���葽���̃m�[�h�ɑ΂��Č�M����(���X�|���X�`�F�b�N�͂��Ȃ�)
													// ���̕��@�͑���m�[�h�̏����X�s�[�h����؍l�����Ă��Ȃ��̂�,�A�����Č�M�����
													// ����m�[�h�������d�؂�Ȃ��ꍇ������܂�
													// (UDP/IP�̈�ē���𗘗p���܂�)


#define B_dwTransitionType_Specific 		2		// ����m�[�h(�l�b�g���[�N�̏��G���A�ŏ����ΏۂɑI�΂�Ă���m�[�h�ɑ΂���)��M����
													// ���X�|���X�̓l�b�g���[�N�̏��G���A�ōŏ��ɏ����ΏۂƂȂ��Ă���m�[�h�����
													// ���X�|���X������L���Ƃ��Ĉ����܂�
													// �܂�,�擪�̃m�[�h��,�S�m�[�h�̑�\�Ƃ��Ďg�p����܂�
													// �{��M���@�͕����m�[�h�ɑ΂��Ă̕`��R�}���h�Ȃ̂Ɏg�p���܂�
													// (UDP/IP�̈�ē���𗘗p���܂�)


#define B_dwTransitionType_SpecificCheck	3		// ����m�[�h(�l�b�g���[�N�̏��G���A�ŏ����ΏۂɑI�΂�Ă���m�[�h�ɑ΂���)��M����
													// B_dwTransitionType_Specific�Ƃ̈Ⴂ��,�����R�}���h���M��,
													// ���X�|���X�͂܂�,�l�b�g���[�N�̏��G���A�ōŏ��ɏ����ΏۂƂȂ��Ă���m�[�h�����
													// ���X�|���X������L���Ƃ��Ĉ���,���̌�,�c��̃m�[�h�ɑ΂���,
													// �P�m�[�h�Â����̐��ۂ��m�F���܂�
													// �{��M���@�͕����m�[�h�ɑ΂��Ă̌����ȏ����̊��҂���ꍇ,
													// �Ⴆ�΃t�@�C���]���ȂǂɎg�p���܂�
													// (UDP/IP�̈�ē���𗘗p���܂�)

#if	MTOM_PRONET
#define B_dwTransitionType_PFN				4		// Pro Face Net�p���[�h(UDP/IP)
													// �擪��Bell�R�[�h�̓d����M��Callback�֐����Ăяo���܂�
													// Pro Face Net�p�̊֐��ȊO���Ă񂾎���B_dwTransitionType_BroadCast�Ɠ��l
#endif

	DWORD dwIPAddress ;							// ��M����m�[�h�̂h�o�A�h���X
	DWORD dwPortNo ;							// ��M����m�[�h�̃|�[�g�ԍ�
	DWORD dwLocalIPAddress ;					// ���ǃm�[�h�̂h�o�A�h���X
	DWORD dwLocalPortNo ;						// ���ǃm�[�h�̃|�[�g�ԍ�


	void	*pDLLWork;	

	DWORD	dwErrorCode;						// Details of error
} ;


typedef BYTE GPCOLOR ;							// �F�w��
typedef BYTE GPLINESTYLE ;						// ����
typedef BYTE GPFONTSIZE ;						// �t�H���g�T�C�Y
typedef BYTE GPTILE ;							// �^�C���p�^�[��
typedef BYTE GPFONT ;							// �t�H���g�̎��


#define W_ESC_T_MAX		80			// ESC T�� ��x�ɕ\���o����ŕ�����
#define W_ESC__t_MAX	80			// ESC t�� ��x�ɕ\���o����ŕ�����
#define W_ESC_R_MAX		64			// ESC R�̍ő�A�N�Z�X��
#define W_ESC_W_MAX		64			// ESC W�̍ő�A�N�Z�X��


#if	MTOM_PRONET

// BEL code parameter(PFN data)

struct	MtoMExData	{
	DWORD	dwOptions;
#define	B_MtoM_ExOptions_None		0x00000000
#define	B_MtoM_ExOptions_Wait		0x00000001	// Random wait
#if	MTOM_VER_1_5
#define	B_MtoM_ExOptions_IPAdrList	0x00000002	// IP address list
#endif

	DWORD	dwDataSize;		// Send/Receive data length
	BYTE	*pbData;		// Send/Receive data buffer
	DWORD	dwIPAdr;		// Send/Receive IP address
	DWORD	dwPortNo;		// Send/Receive Port address
#if	MTOM_VER_1_5
	DWORD	dwIPAdrListCnt;	// Send number of IP address list
	DWORD	*dwIPAdrList;	// Send IP address list
#endif
};

#endif


#define W_EXPORT extern __declspec(dllexport)

W_EXPORT LPMtoMSOCK WINAPI CreateMtoMSocket(DWORD dwProtocolType);
W_EXPORT int WINAPI OpenMtoMLAN(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,LPCSTR szIPAddress);
W_EXPORT int WINAPI CloseMtoM(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish);
W_EXPORT int WINAPI FreeMtoMSoket(LPMtoMSOCK pMSock);
W_EXPORT int WINAPI FreeMtoMSocket(LPMtoMSOCK pMSock);
W_EXPORT int WINAPI SetMtoMEventCallBack(LPMtoMSOCK pMSock,MTOMEVENTBACK pfEventFunc);
W_EXPORT int WINAPI CancelMtoM(LPMtoMSOCK pMSock);
W_EXPORT int WINAPI MtoMFS_FindNode(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,LPCTSTR szNetID);
W_EXPORT int WINAPI MtoM_ResizeGPNetWorkData(LPMtoMSOCK pMSock,DWORD dwNodeCounter);
W_EXPORT DWORD WINAPI SetTransitionType(LPMtoMSOCK pMSock,DWORD dwTransitionType);
W_EXPORT DWORD WINAPI GetTransitionType(LPMtoMSOCK pMSock);
W_EXPORT int WINAPI MtoMGetLastError(LPMtoMSOCK pMSock);

W_EXPORT int WINAPI MtoMESC_W(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,WORD wAddress,INT iDataCount,WORD* pwData)	;
W_EXPORT int WINAPI MtoMESC_R(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,WORD wAddress,INT iDataCount,WORD* pwoData)	;
W_EXPORT int WINAPI MtoMESC_T(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor, INT iX1,INT iY1,GPFONTSIZE cFontSize,CHAR* szString)	;
W_EXPORT int WINAPI MtoMESC_L(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iX2,INT iY2);
W_EXPORT int WINAPI MtoMESC_B(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iX2,INT iY2);
W_EXPORT int WINAPI MtoMESC_S(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,INT iX1,INT iY1,INT iX2,INT iY2,GPTILE cTile);
W_EXPORT int WINAPI MtoMESC_C(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius);
W_EXPORT int WINAPI MtoMESC_A(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius,INT iStartAngle,INT iEndAngle);
W_EXPORT int WINAPI MtoMESC_G(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius,INT iStartAngle,INT iEndAngle);
W_EXPORT int WINAPI MtoMESC_P(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor, GPTILE cTile,GPCOLOR cLimitColor,INT iX1,INT iY1);
W_EXPORT int WINAPI MtoMESC_I(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,BOOL* pbHave,DWORD *pdwCode);
W_EXPORT int WINAPI MtoMESC_t(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPFONT cFont,INT iTurn,INT iDirection,INT iHalfcentering,INT iEmphasis,GPCOLOR cSculptrue,INT iX1,INT iY1,GPFONTSIZE cFontSize,CHAR* szString);
W_EXPORT int WINAPI MtoMESC_l(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iArrowPattern,INT iArrowDirection,INT iX1,INT iY1,INT iX2,INT iY2);
W_EXPORT int WINAPI MtoMESC_b(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iCornerDirection,INT iCornerRadius,INT iX1,INT iY1,INT iX2,INT iY2);
W_EXPORT int WINAPI MtoMESC_s(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPTILE cTile,INT iCornerDirection,INT iCornerRadius,INT iX1,INT iY1,INT iX2,INT iY2);
W_EXPORT int WINAPI MtoMESC_c(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPTILE cTile,INT iX1,INT iY1,INT iRadius);
W_EXPORT int WINAPI MtoMESC_g(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPTILE cTile,INT iX1,INT iY1,INT iRadius,INT iStartAngle,INT iEndAngle);
W_EXPORT int WINAPI MtoMESC_SetContrast(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,DWORD dwContrast,DWORD dwLight) ;
W_EXPORT int WINAPI MtoMESC_GetContrast(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish,DWORD *dwContrast,DWORD *dwLight) ;

#if	MTOM_PRONET
W_EXPORT int WINAPI MtoMBEL_Command(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish, MtoMExData *pExData);
#endif
#if	MTOM_VER_1_2
W_EXPORT DWORD WINAPI MtoMGetNetMask(LPMtoMSOCK pMSock);
#endif
#if	MTOM_VER_1_5 && MTOM_PRONET
W_EXPORT int WINAPI MtoMFS_Logon(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish, LPCTSTR pRemoteAccessPassword);
W_EXPORT int WINAPI MtoMFS_Logoff(LPMtoMSOCK pMSock,MTOMCALLBACK pfFinish);
#endif


#pragma pack(pop)
