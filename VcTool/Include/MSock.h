// MSock.h : ヘッダー ファイル
// coba注：ベンダ提供のクラスなので、触らないほうがいいです

/////////////////////////////////////////////////////////////////////////////
// MSock スレッド

#include <windows.h>
#include "MtoMAPI.h"

class CMSock
{

// アトリビュート
public:
	LPMtoMSOCK m_pMSock ;
	MTOMCALLBACK m_pfFinish;

	
// オペレーション
public:
	CMSock(DWORD dwProtocolType = B_ProtocolType_LAN);
	~CMSock();

	// LPMtoMSOCK CreateMtoMSocket(DWORD dwProtocolType);
	// int FreeMtoMSoket();
	// int SetMtoMEventCallBack(MTOMEVENTBACK pfEventFunc);

	int OpenMtoMLAN(LPCSTR szIPAddress);
	int CloseMtoM();
	
	int CancelMtoM();
	int MtoMFS_FindNode(LPCTSTR szNetID);
	int MtoM_ResizeGPNetWorkData(DWORD dwNodeCounter);
	DWORD SetTransitionType(DWORD dwTranscitonType);
	DWORD GetTransitionType();
	DWORD MtoMGetLastError(void);

	int MtoMESC_W(WORD wAddress,INT iDataCount,WORD* pwData);
	int MtoMESC_R(WORD wAddress,INT iDataCount,WORD* pwoData);
	int MtoMESC_T(GPCOLOR cDisplayColor,GPCOLOR cBackColor, INT iX1,INT iY1,GPFONTSIZE cFontSize,CHAR* szString)	;
	int MtoMESC_L(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iX2,INT iY2);
	int MtoMESC_B(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iX2,INT iY2);
	int MtoMESC_S(GPCOLOR cDisplayColor,GPCOLOR cBackColor,INT iX1,INT iY1,INT iX2,INT iY2,GPTILE cTile);
	int MtoMESC_C(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius);
	int MtoMESC_A(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius,INT iStartAngle,INT iEndAngle);
	int MtoMESC_G(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius,INT iStartAngle,INT iEndAngle);
	int MtoMESC_P(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPTILE cTile,GPCOLOR cLimitColor,INT iX1,INT iY1);
	int MtoMESC_I(BOOL* pbHave,DWORD *pdwCode);
	int MtoMESC_t(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPFONT cFont,INT iTurn,INT iDirection,INT iHalfcentering,INT iEmphasis,GPCOLOR cSculpture,INT iX1,INT iY1,GPFONTSIZE cFontSize,CHAR* szString);
	int MtoMESC_l(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iArrowPattern,INT iArrowDirection,INT iX1,INT iY1,INT iX2,INT iY2);
	int MtoMESC_b(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iCornerDirection,INT iCornerRadius,INT iX1,INT iY1,INT iX2,INT iY2);
	int MtoMESC_s(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPTILE cTile,INT iCornerDirection,INT iCornerRadius,INT iX1,INT iY1,INT iX2,INT iY2);
	int MtoMESC_c(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPTILE cTile,INT iX1,INT iY1,INT iRadius);
	int MtoMESC_g(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius,INT iStartAngle,INT iEndAngle);
	int MtoMESC_SetContrast(DWORD dwContrast,DWORD dwLight);
	int MtoMESC_GetContrast(DWORD *dwContrast,DWORD *dwLight);


	void ASyncModeEnable() ;	// 非同期モードで交信を行う
	void SyncModeEnable() ;		// 同期モードで交信を行う

	virtual BOOL OnFinished(int iMtoMCode){	return( TRUE ); } ;	// ＡＰＩの途中経過及び終了時、コールバックが必要な時はこのメンバーをオーバーライドし、FinishedCallBackEnable()をコールする
	virtual void OnEventFunc(int iCode,DWORD dwParam1,DWORD dwParam2){};	// ＡＰＩからのイベント通知が必要な時はこのメンバーをオーバーライドしします


	// ネットワーク情報エリアをを求める
	LPGPNETWORKDATA	GetGPNetWorkData(){ return m_pMSock->pGPNetWorkData ; } ;

protected:
	MTOMCALLBACK m_Finished ;
} ;
/////////////////////////////////////////////////////////////////////////////
