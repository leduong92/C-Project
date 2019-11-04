#include "GetNetwork.h"
#include <winsock2.h>
#include <iphlpapi.h>
//-------------------------------------------------------------------------------
//
//  int GetNICInfo( char *pIPAddress ,char* pMacAddress)
//  引数はそれぞれ十分な領域が確保された文字列ポインタを渡すこと。
//
//-------------------------------------------------------------------------------
int GetNICInfo( char *pIPAddress ,char* pMacAddress)
{
	ULONG  ulOutBufLen;
	DWORD	dwRet;
	BYTE*		pBuff;
	PIP_ADAPTER_INFO	pAdapterInfo;

	ulOutBufLen = 0;
	// 必要バッファサイズ取得
	GetAdaptersInfo(NULL, &ulOutBufLen);
	// 必要なバッファサイズで取得用の構造体を用意する。
	pBuff = new BYTE[ulOutBufLen];
	pAdapterInfo = (PIP_ADAPTER_INFO)pBuff;
	// 実際にネットワークアダプタの情報を取得する。
	dwRet = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	// ネットワークアダプターの情報取得をチェック
	if(dwRet == ERROR_SUCCESS) {
		// 取得が成功したら最後のアダプタ情報を採用
		while (1) {
			// 現状NICの正しいMacアドレスを割り出すことが出来ない。
			// 最後に取得されたアダプタを採用。必ずしもNICのMacアドレスとは限らない！
			if (pAdapterInfo->Next == NULL) {// MIB_IF_TYPE_ETHERNETでは判定不可！
				
				char buffer[13];
				
				//まず,IP
				sprintf(buffer, "%s",pAdapterInfo->IpAddressList.IpAddress.String);
				strcpy(pIPAddress, buffer);

				if(pMacAddress !=NULL){
					//次にMAC
					sprintf(buffer, "%02x%02x%02x%02x%02x%02x",
					pAdapterInfo->Address[0],
					pAdapterInfo->Address[1],
					pAdapterInfo->Address[2],
					pAdapterInfo->Address[3],
					pAdapterInfo->Address[4],
					pAdapterInfo->Address[5]);
					strcpy(pMacAddress, buffer);
				}
				delete pBuff;
				return true;
			}
		
			pAdapterInfo = pAdapterInfo->Next;
		}
		
	}

	delete pBuff;
		
	return false;	
}
