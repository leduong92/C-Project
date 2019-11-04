// MSock.cpp : インプリメンテーション ファイル
// coba注：ベンダ提供のクラスなので、触らないほうがいいです
//
#include "MSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MSock






// ＡＰＩの途中経過及び終了時のコールバック関数をジャンプする
BOOL CALLBACK FinishedJump(LPMtoMSOCK pMSock,int iMtoMCode)
{
	BOOL Ret ;
	CMSock* pCMSock ;
	
	pCMSock = (CMSock* )pMSock->dwUser1 ;

	Ret = pCMSock->OnFinished(iMtoMCode) ;
	return( Ret );
}

// イベント発生時のコールバック関数

void CALLBACK EventFuncJump(LPMtoMSOCK pMSock,int iCode,DWORD dwParam1,DWORD dwParam2)
{
	CMSock* pCMSock ;
	
	pCMSock = (CMSock* )pMSock->dwUser1 ;

	pCMSock->OnEventFunc(iCode,dwParam1,dwParam2) ;
}


	// 非同期モードで交信を行う
void CMSock::ASyncModeEnable()
{
	m_pfFinish = FinishedJump ;
}

	// 同期モードで交信を行う
void CMSock::SyncModeEnable()
{
	m_pfFinish = NULL; 
}


CMSock::CMSock(DWORD dwProtocolType)
{
	if( m_pMSock = ::CreateMtoMSocket(dwProtocolType) ){
		m_pMSock->dwUser1 = (DWORD )this ;
		::SetMtoMEventCallBack(m_pMSock,EventFuncJump) ;

		// デフォルトは同期モード
		SyncModeEnable();
	}
}

CMSock::~CMSock()
{
	::FreeMtoMSoket(m_pMSock);
}

int CMSock::OpenMtoMLAN(LPCSTR szIPAddress)
{
	return( ::OpenMtoMLAN(m_pMSock,m_pfFinish,szIPAddress) );
}


int CMSock::CloseMtoM()
{
	return( ::CloseMtoM(m_pMSock,m_pfFinish)) ;
}


int CMSock::CancelMtoM()
{
	return( ::CancelMtoM(m_pMSock)) ;
}

int CMSock::MtoMFS_FindNode(LPCTSTR szNetID)
{
	return( ::MtoMFS_FindNode(m_pMSock,m_pfFinish,szNetID)) ;
}

int CMSock::MtoM_ResizeGPNetWorkData(DWORD dwNodeCounter)
{
	return( ::MtoM_ResizeGPNetWorkData(m_pMSock,dwNodeCounter)) ;
}

DWORD CMSock::SetTransitionType(DWORD dwTranscitonType)
{
	return( ::SetTransitionType(m_pMSock,dwTranscitonType)) ;
}

DWORD CMSock::GetTransitionType()
{
	return( ::GetTransitionType(m_pMSock)) ;
}

int CMSock::MtoMESC_W(WORD wAddress,INT iDataCount,WORD* pwData)	
{
	return( ::MtoMESC_W(m_pMSock,m_pfFinish,wAddress,iDataCount,pwData)) ;
}

int CMSock::MtoMESC_R(WORD wAddress,INT iDataCount,WORD* pwoData)	
{
	return( ::MtoMESC_R(m_pMSock,m_pfFinish,wAddress,iDataCount,pwoData)) ;
}

int CMSock::MtoMESC_T(GPCOLOR cDisplayColor,GPCOLOR cBackColor, INT iX1,INT iY1,GPFONTSIZE cFontSize,CHAR* szString)	
{
	return( ::MtoMESC_T(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,iX1,iY1,cFontSize,szString)) ;
}

int CMSock::MtoMESC_L(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iX2,INT iY2)
{
	return( ::MtoMESC_L(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cLineStyle,iX1,iY1,iX2,iY2)) ;
}

int CMSock::MtoMESC_B(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iX2,INT iY2)
{
	return( ::MtoMESC_B(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cLineStyle,iX1,iY1,iX2,iY2)) ;
}

int CMSock::MtoMESC_S(GPCOLOR cDisplayColor,GPCOLOR cBackColor,INT iX1,INT iY1,INT iX2,INT iY2,GPTILE cTile)
{
	return( ::MtoMESC_S(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,iX1,iY1,iX2,iY2,cTile)) ;
}

int CMSock::MtoMESC_C(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius)
{
	return( ::MtoMESC_C(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cLineStyle,iX1,iY1,iRadius)) ;
}

int CMSock::MtoMESC_A(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius,INT iStartAngle,INT iEndAngle)
{
	return( ::MtoMESC_A(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cLineStyle,iX1,iY1,iRadius,iStartAngle,iEndAngle)) ;
}

int CMSock::MtoMESC_G(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius,INT iStartAngle,INT iEndAngle)
{
	return( ::MtoMESC_G(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cLineStyle,iX1,iY1,iRadius,iStartAngle,iEndAngle)) ;
}
int CMSock::MtoMESC_P(GPCOLOR cDisplayColor,GPCOLOR cBackColor, GPTILE cTile,GPCOLOR cLimitColor,INT iX1,INT iY1)
{
	return( ::MtoMESC_P(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cTile,cLimitColor,iX1,iY1)) ;
}
int CMSock::MtoMESC_I(BOOL* pbHave,DWORD *pdwCode)
{
	return( ::MtoMESC_I(m_pMSock,m_pfFinish,pbHave,pdwCode)) ;
}

int CMSock::MtoMESC_t(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPFONT cFont,INT iTurn,INT iDirection,INT iHalfcentering,INT iEmphasis,GPCOLOR cSculpture,INT iX1,INT iY1,GPFONTSIZE cFontSize,CHAR* szString)
{
	return( ::MtoMESC_t(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cFont,iTurn,iDirection,iHalfcentering,iEmphasis,cSculpture,iX1,iY1,cFontSize,szString)) ;
}

int CMSock::MtoMESC_l(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iArrowPattern,INT iArrowDirection,INT iX1,INT iY1,INT iX2,INT iY2)
{
	return( ::MtoMESC_l(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cLineStyle,iArrowPattern,iArrowDirection,iX1,iY1,iX2,iY2)) ;
}
int CMSock::MtoMESC_b(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iCornerDirection,INT iCornerRadius,INT iX1,INT iY1,INT iX2,INT iY2)
{
	return( ::MtoMESC_b(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cLineStyle,iCornerDirection,iCornerRadius,iX1,iY1,iX2,iY2)) ;
}
int CMSock::MtoMESC_s(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPTILE cTile,INT iCornerDirection,INT iCornerRadius,INT iX1,INT iY1,INT iX2,INT iY2)
{
	return( ::MtoMESC_s(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cTile,iCornerDirection,iCornerRadius,iX1,iY1,iX2,iY2)) ;
}

int CMSock::MtoMESC_c(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPTILE cTile,INT iX1,INT iY1,INT iRadius)
{
	return( ::MtoMESC_c(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cTile,iX1,iY1,iRadius)) ;
}

int CMSock::MtoMESC_g(GPCOLOR cDisplayColor,GPCOLOR cBackColor,GPLINESTYLE cLineStyle,INT iX1,INT iY1,INT iRadius,INT iStartAngle,INT iEndAngle)
{
	return( ::MtoMESC_g(m_pMSock,m_pfFinish,cDisplayColor,cBackColor,cLineStyle,iX1,iY1,iRadius,iStartAngle,iEndAngle)) ;
}

int CMSock::MtoMESC_SetContrast(DWORD dwContrast,DWORD dwLight)
{
	return( ::MtoMESC_SetContrast(m_pMSock,m_pfFinish,dwContrast,dwLight)) ;
}

int CMSock::MtoMESC_GetContrast(DWORD *dwContrast,DWORD *dwLight)
{
	return( ::MtoMESC_GetContrast(m_pMSock,m_pfFinish,dwContrast,dwLight));
}

DWORD CMSock::MtoMGetLastError(void)
{
	return( ::MtoMGetLastError(m_pMSock)) ;
}
