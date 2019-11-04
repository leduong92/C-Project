//=======================================================================================================
//	簡易版暗号化処理クラス
//	Class Name EasyEnc.cpp
//	File Name CEasyEnc.cpp
//	Date : 2006.10.12 N.Maeda
//=======================================================================================================

#include "EasyEnc.h"
#include <shlwapi.h>
#include <stdio.h>
#include <winsock.h>
#include <time.h>
#include <string.h>


typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;

	NAME_BUFFER NameBuff [30];

}ASTAT, * PASTAT;

ASTAT Adapter;




//---------------------------------------------------------------------------------------------------------------------
//	CEasyEnc::CEasyEnc(void)
//	Member	:public
//	Function:Constractor
//---------------------------------------------------------------------------------------------------------------------
CEasyEnc::CEasyEnc(void)
{
}

//---------------------------------------------------------------------------------------------------------------------
//	CEasyEnc::~CEasyEnc(void)
//	Member	:public
//	Function:Destractor
//---------------------------------------------------------------------------------------------------------------------
CEasyEnc::~CEasyEnc(void)
{
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CEasyEnc::CheckReg( char*EncCode )
//	Member	:public
//	Function:
// プログラムの最初にレジストリの値が正しいか？チェックする
// レジストリの登録が正しくない場合は、値を削除する。
// （以降のファイルオープンのチェックで引っかかる）
//---------------------------------------------------------------------------------------------------------------------
BOOL CEasyEnc::CheckRegEncKey( char*EncCode )
{
	unsigned char macBuf[6+1];
	unsigned char regMac[6+1];
	char regBuf[1024];
	char buf[256];
	int ret;
	int i;
	unsigned char EncKey[6+1];

	memset(macBuf,0,sizeof(macBuf));
	memset(regBuf,0,sizeof(regBuf));
	memset(EncKey,0,sizeof(EncKey));

	memcpy(EncKey,ENC_REG_CODE,6);

	// Macアドレス取得
	ret= this->GetMac( macBuf );
	if( ret!=0 ){
		MessageBox(NULL,"No LAN!!\nPls connect to LAN.","Err",MB_OK);
		this->DelReg( ENC_REG_KEY , ENC_REG_DATA );
		return FALSE;
	}

	
	// Read from registry
	ret = this->GetReg( ENC_REG_KEY , ENC_REG_DATA , regBuf, sizeof(regBuf));
	if( ret!=TRUE || strlen(regBuf)==0){
		// レジストリ登録無し
		char tt[1024];
		sprintf(tt,"Check No1 !!\n Can't read registry data\n%s\n%s",ENC_REG_KEY , ENC_REG_DATA);
		MessageBox(NULL,tt,"Err",MB_OK);

		return FALSE;
	}

	// レジストリから読み出したMACアドレスをバイナリに変換
	// ついでにレジストリの値をちょっと加工
	memset(regMac,0,sizeof(regMac));
	for(i=0;i<6;i++){
		memset(buf,0,sizeof(buf));
		memcpy( buf ,regBuf+i*3, 2 );
		regMac[i]=(unsigned char)htol(buf,2);
		regMac[i] ^= EncKey[i];
	}


	// レジストリのデータとMACアドレスが一致しているか？チェックする
	if( memcmp(regMac,macBuf,6) ){
		MessageBox(NULL,"Program Key Err\nThis PC can't use for Mixing Infomarion System\nPls setup new key!!","Err",MB_OK);
		this->DelReg( ENC_REG_KEY , ENC_REG_DATA );
		return FALSE;
	}

	return TRUE;
}

BOOL CEasyEnc::SetRegEncKey( char*EncCode )
{
	unsigned char macBuf[6+1];
	unsigned char regMac[6+1];
	char regBuf[1024];
	int ret;
	int i;
	unsigned char EncKey[6+1];

	memset(macBuf,0,sizeof(macBuf));
	memset(regBuf,0,sizeof(regBuf));
	memset(EncKey,0,sizeof(EncKey));

	memcpy(EncKey,ENC_REG_CODE,6);



	// Macアドレス取得
	ret= this->GetMac( macBuf );
	if( ret!=0 ){
		MessageBox(NULL,"Can't check MAC address","Err",MB_OK);
		this->DelReg( ENC_REG_KEY , ENC_REG_DATA );
		return FALSE;
	}

	// レジストリに書き込む値をちょっと加工
	memset(regMac,0,sizeof(regMac));
	for(i=0;i<6;i++){
		regMac[i]=macBuf[i] ^ EncKey[i];
	}

	// 一度レジストリを削除
	this->DelReg( ENC_REG_KEY , ENC_REG_DATA );

	sprintf(regBuf,"%02X-%02X-%02X-%02X-%02X-%02X",regMac[0],regMac[1],regMac[2],regMac[3],regMac[4],regMac[5]);
	ret = this->PutReg( ENC_REG_KEY , ENC_REG_DATA , regBuf );

	time_t 	ltime;
	struct	tm	*nowtime;
    char dummy[20];

    memset( dummy, 0 , sizeof(dummy));

	time(&ltime);
	nowtime=localtime(&ltime);
	sprintf(dummy,"%04d/%02d/%02d %02d:%02d:%02d", nowtime->tm_year+1900,
                      nowtime->tm_mon+1,nowtime->tm_mday,nowtime->tm_hour,
                                   nowtime->tm_min,nowtime->tm_sec	);
	ret = this->PutReg( ENC_REG_KEY , ENC_REG_DATE , dummy );


	return TRUE;
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CEasyEnc::DelRegEncKey( void )
//	Member	:public
//	Function:
//  レジストリ削除処理
//---------------------------------------------------------------------------------------------------------------------
BOOL CEasyEnc::DelRegEncKey( void )
{
	return this->DelReg( ENC_REG_KEY , ENC_REG_DATA );
}

//---------------------------------------------------------------------------------------------------------------------
//	BOOL CEasyEnc::CheckEnc( void )
//	Member	:public
//	Function:
// ファイル操作クラスでレジストリに値が登録されているか？チェックする
//---------------------------------------------------------------------------------------------------------------------
BOOL CEasyEnc::CheckEnc( void )
{
	char regBuf[1024];
	int ret;

	memset(regBuf,0,sizeof(regBuf));
	// レジストリ読み込み
	ret = this->GetReg( ENC_REG_KEY , ENC_REG_DATA , regBuf, sizeof(regBuf));
	if( ret!=TRUE || strlen(regBuf)==0 ){
		// レジストリ登録無し
		return FALSE;
	}
	return TRUE;
}


//---------------------------------------------------------------------------------------------------------------------
//	CEasyEnc::
//	Member	:public
//	Function:
//---------------------------------------------------------------------------------------------------------------------
BOOL CEasyEnc::GetReg( char*szKeyName,char*szDataName,char*buf,int buf_len)
{
	IStream *pStrm;
	DWORD cnt;

	pStrm=SHOpenRegStream(HKEY_LOCAL_MACHINE,szKeyName,szDataName,STGM_READWRITE);
	if( pStrm!=NULL){
		if( S_OK==pStrm->Read(buf,buf_len,&cnt)){
			if(pStrm != NULL) pStrm->Release();
			return TRUE;
		}
	}
	if(pStrm != NULL) pStrm->Release();
	return FALSE;
}


//---------------------------------------------------------------------------------------------------------------------
//	CEasyEnc::
//	Member	:public
//	Function:
//---------------------------------------------------------------------------------------------------------------------
BOOL CEasyEnc::PutReg( char*szKeyName,char*szDataName,char*buf)
{
	IStream *pStrm;
	DWORD cnt;
	int ret;
	
	pStrm=SHOpenRegStream(HKEY_LOCAL_MACHINE,szKeyName,szDataName,STGM_READWRITE);
	if( pStrm!=NULL){
		ret=pStrm->Write(buf,strlen(buf)+1,&cnt);
		if(pStrm != NULL) pStrm->Release();
	}
	
	return TRUE;
}

//---------------------------------------------------------------------------------------------------------------------
//	CEasyEnc::
//	Member	:public
//	Function:
//---------------------------------------------------------------------------------------------------------------------
BOOL CEasyEnc::DelReg( char*szKeyName,char*szDataName)
{

	SHDeleteValue(HKEY_LOCAL_MACHINE,szKeyName,szDataName);

	return TRUE;
}


// ちゃんとしたやつ
int	CEasyEnc::GetMacAddress( void )
{

	NCB Ncb;
	UCHAR uRetCode;
//	char NetName[50];
	LANA_ENUM   lenum;
	int	  i;


	memset( MacString,0,sizeof(MacString));

	memset( &Ncb, 0, sizeof(Ncb) );
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (UCHAR *)&lenum;
	Ncb.ncb_length = sizeof(lenum);
	uRetCode = Netbios( &Ncb );
	if( uRetCode ) return -1;

	for(i=0; i < lenum.length ;i++)
	{
		// NCBRESET
		memset( &Ncb, 0, sizeof(Ncb) );
		Ncb.ncb_command = NCBRESET;
		Ncb.ncb_lana_num = lenum.lana[i];

		uRetCode = Netbios( &Ncb );
		if( uRetCode ) return -1;


		// NCBASTAT
		memset( &Ncb, 0, sizeof (Ncb) );
		Ncb.ncb_command = NCBASTAT;
		Ncb.ncb_lana_num = lenum.lana[i];

		memset( Ncb.ncb_callname,0,sizeof(Ncb.ncb_callname));
		memcpy( Ncb.ncb_callname,  "*               " , 15 );
		Ncb.ncb_buffer = (unsigned char *) &Adapter;
		Ncb.ncb_length = sizeof(Adapter);

		uRetCode = Netbios( &Ncb );
		if ( uRetCode == 0 ){
			MacString[i][0]=Adapter.adapt.adapter_address[0];
			MacString[i][1]=Adapter.adapt.adapter_address[1];
			MacString[i][2]=Adapter.adapt.adapter_address[2];
			MacString[i][3]=Adapter.adapt.adapter_address[3];
			MacString[i][4]=Adapter.adapt.adapter_address[4];
			MacString[i][5]=Adapter.adapt.adapter_address[5];
		}
	}

	return 0;
}

//---------------------------------------------------------------------------------------------------------------------
//	CEasyEnc::
//	Member	:public
//	Function:Get mac address
//  buf[6]
//---------------------------------------------------------------------------------------------------------------------
BOOL CEasyEnc::GetMac( unsigned char*buf )
{
	int ret;

	ret= GetMacAddress();
	if( ret ){
		memcpy( buf , MacString[0] , 6 );
	}
	return ret;

/*
	NCB Ncb;
	UCHAR uRetCode;
//	char NetName[100];


	memset( &Ncb, 0, sizeof(Ncb) );

	Ncb.ncb_command		= NCBRESET;
	Ncb.ncb_lana_num	= 0;


	uRetCode = Netbios( &Ncb );

	if( uRetCode )	return(-1);

	memset( &Ncb, 0, sizeof (Ncb) );

	Ncb.ncb_command		= NCBASTAT;
	Ncb.ncb_lana_num	= 0;

	strcpy( (char*)Ncb.ncb_callname, "*" );

	Ncb.ncb_buffer	= (unsigned char *) &Adapter;
	Ncb.ncb_length	= sizeof(Adapter);

	uRetCode = Netbios( &Ncb );

	if ( uRetCode == 0 )
	{ 
		buf[0]=Adapter.adapt.adapter_address[0];
		buf[1]=Adapter.adapt.adapter_address[1];
		buf[2]=Adapter.adapt.adapter_address[2];
		buf[3]=Adapter.adapt.adapter_address[3];
		buf[4]=Adapter.adapt.adapter_address[4];
		buf[5]=Adapter.adapt.adapter_address[5];

	}
	else	return(-2);

	return(0);
*/
}




long CEasyEnc::htol( char *hexstr , short len )
{
	long ret = 0;
	short i;

	if( len > 8 ) return 0;
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

