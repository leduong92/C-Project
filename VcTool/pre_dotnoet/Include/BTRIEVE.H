#ifndef btrieve_HPP
#define btrieve_HPP
//===================================================================================================================
//
//	FileName	:BTRIEVE.H
//	Class		:Btrieve	(C)1982-1995 Btrieve Technologies, Inc. All Rights Reserved
// 				:BTRIEVEのAPIコールをする基本クラス
//	作成日		:1996.2.16	Coded by M.Ishida
//===================================================================================================================
//	Modefied	:
//	By Koba	96.10.11	ファイルパスをINIファイルから取得
//	By Koba 93.12.25	検索時のキーデータ消失対策			
//	04.01.22 coba	検索時のキー保存時のバグを防ぐため、各キー長をopen時に保存し、その長さを使い
//					キーを保存する
//===================================================================================================================
#include <stdlib.h>
#include <string.h>
#include "btrapi.h"
#include "btrconst.h"


//========================================================================================
//	Constants
//========================================================================================

#define EXIT_WITH_ERROR	 1
#define TRUE			1
#define FALSE			0
#define VERSION_OFFSET		0
#define REVISION_OFFSET	 2
#define PLATFORM_ID_OFFSET 4

#define BTI_KEYSEG_MAX 50
//========================================================================================
//	Structure Type Definitions
//========================================================================================
typedef struct
{
	BTI_SINT recLength;
	BTI_SINT pageSize;
	BTI_SINT indexCount;
	BTI_CHAR reserved[ 4 ];
	BTI_SINT flags;
	BTI_BYTE dupPointers;
	BTI_BYTE notUsed;
	BTI_SINT allocations;
} FILE_SPECS;

typedef struct
{
	BTI_SINT position;
	BTI_SINT length;
	BTI_SINT flags;
	BTI_CHAR reserved[ 4 ];
	BTI_CHAR type;
	BTI_CHAR null;
	BTI_CHAR notUsed[ 2 ];
	BTI_BYTE manualKeyNumber;
	BTI_BYTE acsNumber;
} KEY_SPECS;

//========================================================================================
// クラス定義
//========================================================================================
class Btrieve
{
		/* Btrieve Parameters */
		BTI_BYTE posBlock[ 128 ];
		BTI_BYTE dataBuf[ 4096 ];
		BTI_WORD dataLen;
		BTI_BYTE keyBuf[ 255 ];
		BTI_WORD keyNum,keyNumMax;
		unsigned char fileOpen ;
		unsigned char btrieveLoaded;
		BTI_BYTE	clientID[ 16 ];

	protected:
		BTI_WORD Key1Len,Key2Len;
		BTI_WORD RecLen;
		char	itsFileName[256];
		BTI_BYTE *fileCreatePtr;
		BTI_WORD	fileCreateLen;

	//Define the Public Information
	public: 				
		Btrieve(void); 		//Constructor for this class
		~Btrieve(void);
		BTI_SINT status;

		int Version(short *Ver,short *Rev,short *Plat);
		int StopID(void);
		int StartTran();				//ﾄﾗﾝｻﾞｸｼｮﾝを開始する
		int PauseTran();				//ﾄﾗﾝｻﾞｸｼｮﾝを中断する
		int EndTran();				 	//ﾄﾗﾝｻﾞｸｼｮﾝを終了する
		int SetKey(short Keyno);				// キー番号をセット

	protected:
		int SetFileName( char *FileName, char *ProfileName, LPCTSTR SectionName ,LPCTSTR KeyName);
		int SetDataLen(short Len);


		int Create(void); 			 		//Btriveﾌｧｲﾙを作成
		virtual int CreateSpec(void);		//スペックを定義
		virtual int Open(void);				//Btrive Open

		virtual int Insert(char* Record); 		 //レコードの追加
		virtual int Delete(char* key,char* Record);	 //レコード削除
		virtual int Update(char* key,char* Record);	 //レコードの更新

		virtual int GetFirst(char* Record);		 //最初のレコードを取得
		virtual int GetLast(char* Record);		 //最後のﾚｺｰﾄﾞを取得
		virtual int GetNext(char* Record);		 //次のレコードを取得
		virtual int GetPrev(char* Record);		 //前のレコードを取得
		virtual int GetEqual(char* Record,char* Key) ; //ｷｰﾃﾞｰﾀと同じﾚｺｰﾄﾞを取得
		virtual int GetGE(char* Record,char* Key) ;	 //ｷｰﾃﾞｰﾀと同じor以上
		virtual int GetLE(char* Record,char* key);	 //ｷｰﾃﾞｰﾀと同じor以下
		virtual int GetGT(char* Record,char* Key) ;	 //ｷｰﾃﾞｰﾀより大きいﾃﾞｰﾀ
		virtual int GetLT(char* Record,char* key);	 //ｷｰﾃﾞｰﾀより小さいﾃﾞｰﾀ

		virtual int Close(void);
		int Stop(void);

		// キーバッファ用の構造体ですが実際には使用しません
		struct{
			FILE_SPECS fileSpecs;
			KEY_SPECS	keySpecs[ 2 ];
		} fileCreateBuf;


		// キーデータの消失防止用待避エリア
		char m_Key[1000];

	private:
		BTI_SINT m_KeyLen[BTI_KEYSEG_MAX];
};
#endif
