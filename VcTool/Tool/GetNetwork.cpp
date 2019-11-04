#include "GetNetwork.h"
#include <winsock2.h>
#include <iphlpapi.h>
//-------------------------------------------------------------------------------
//
//  int GetNICInfo( char *pIPAddress ,char* pMacAddress)
//  �����͂��ꂼ��\���ȗ̈悪�m�ۂ��ꂽ������|�C���^��n�����ƁB
//
//-------------------------------------------------------------------------------
int GetNICInfo( char *pIPAddress ,char* pMacAddress)
{
	ULONG  ulOutBufLen;
	DWORD	dwRet;
	BYTE*		pBuff;
	PIP_ADAPTER_INFO	pAdapterInfo;

	ulOutBufLen = 0;
	// �K�v�o�b�t�@�T�C�Y�擾
	GetAdaptersInfo(NULL, &ulOutBufLen);
	// �K�v�ȃo�b�t�@�T�C�Y�Ŏ擾�p�̍\���̂�p�ӂ���B
	pBuff = new BYTE[ulOutBufLen];
	pAdapterInfo = (PIP_ADAPTER_INFO)pBuff;
	// ���ۂɃl�b�g���[�N�A�_�v�^�̏����擾����B
	dwRet = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
	// �l�b�g���[�N�A�_�v�^�[�̏��擾���`�F�b�N
	if(dwRet == ERROR_SUCCESS) {
		// �擾������������Ō�̃A�_�v�^�����̗p
		while (1) {
			// ����NIC�̐�����Mac�A�h���X������o�����Ƃ��o���Ȃ��B
			// �Ō�Ɏ擾���ꂽ�A�_�v�^���̗p�B�K������NIC��Mac�A�h���X�Ƃ͌���Ȃ��I
			if (pAdapterInfo->Next == NULL) {// MIB_IF_TYPE_ETHERNET�ł͔���s�I
				
				char buffer[13];
				
				//�܂�,IP
				sprintf(buffer, "%s",pAdapterInfo->IpAddressList.IpAddress.String);
				strcpy(pIPAddress, buffer);

				if(pMacAddress !=NULL){
					//����MAC
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
