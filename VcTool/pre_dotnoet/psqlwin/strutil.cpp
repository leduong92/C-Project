//===================================================================================================================
//
//	FileName	: strutil.c
//	�쐬��		:2001.10.22 coba 
//	�X�V��		: 
//===================================================================================================================
#include "strutil.h"



//--------------------------------------------------------------------------------------------------------------------
//	�֐����Fint htoi( char *hexstr , short len )
//	�@�\�@�F16�i�i������j��10�i(short�^)�ɕϊ�����
//	�����@�F�ϊ����f�[�^�i�[�|�C���^
//	�Ԓl�@�F�ϊ���f�[�^
//--------------------------------------------------------------------------------------------------------------------
int	htoi( char *hexstr , int len )
{
	int ret = 0;
	int i;

	if( len > 4 ) return 0;
	for( i = 0 ; i < len ; i++ ){
		ret *= 16;
		if( hexstr[i] < 'A' ) ret += hexstr[i] - '0';
		else
		if( hexstr[i] < 'a' ) ret += hexstr[i] - 'A'+10;
		else
			ret += hexstr[i] - 'a'+10;
	}
	return(ret);
}