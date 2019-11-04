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

#define GP_SCREEN_X					640		// ＧＰの画面の最大Ｘドットサイズ
#define GP_SCREEN_Y					480		// ＧＰの画面の最大Ｙドットサイズ



/////////////////////////////////////////////////////////////////
// Default for API

#define MTOM_DEFAULT_PORTNO					8000			// デフォルトのポート番号

#define	MTOM_DEFAULT_FIND_NODE_TIME			(2*1000)		// [mS], Find node time-out
#define	MTOM_DEFAULT_RESPONSE_TIME			(15*1000)		// [mS], Response time-out
#define	MTOM_DEFAULT_CHAR_TIME				(10*1000)		// [mS], Character time-our
#define	MTOM_DEFAULT_UDP_TIME				50				// [mS], Multi block delay time

#define	MTOM_DEFAULT_N_LAN_NETWORKDATA		1				// Initial network data allocate size



/////////////////////////////////////////////////////////////////

struct tagMtoMSOCK ;
typedef struct tagMtoMSOCK MtoMSOCK,*LPMtoMSOCK ;
typedef BOOL (CALLBACK* MTOMCALLBACK)(LPMtoMSOCK,int);									// ＡＰＩ終了を通知するコールバック関数の型宣言
typedef void (CALLBACK* MTOMEVENTBACK)(LPMtoMSOCK,int,DWORD dwParam1,DWORD dwParam2);	// ＡＰＩからのイベント発生時,通知するコールバック関数の型宣言


/////////////////////////////////////////////////////////////////
// Result of API, finish callback, event callback

#define MTOM_OK						0x00000000		//	処理は正常終了した
#define MTOM_CONTINUE				0x00000001		//	処理は継続中
#define MTOM_PROCESS_FINISH			0x00000002		//	１プロセスが正常終了しました,引き続き,処理を継続します
#define MTOM_USER_STOPED			0x00000003   	//	ユーザーアプリケーションによって処理が中断されました
													//  （MtoMStop()がコールされたかコールバック関数でFALSEが返された)

#define MTOM_EVENT_TOUCH  			0x00000040		//  タッチパネルが押された
#define MTOM_EVENT_CLOSED   		0x00000041 		//  コネクションが切断された
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

typedef struct tagPROTOCOLVERSION {			// 搭載通信プロトコルバージョン情報
		DWORD dwID ;							// 通信プロトコルのＩＤ(バイナリー値)
		BYTE szProtocolName[32] ;				// 通信プロトコルの名			(ASCIIZ 文字列)
		BYTE szProtocolVersion[32] ; 			// 通信プロトコルのバージョン	(ASCIIZ 文字列)
#define MTOM_PROTOCOL_MAX	16					// 最大搭載可能プロトコル数
} PROTOCOLVERSION ,*LPPROTOCOLVERSION ;

typedef struct tagGPVERSION {				// バージョン情報
	BOOL	bGood ;								// バージョン情報が有効かどうか
													// バージョン情報を有効にするにはMtoMDLE_Communication()を一度実行してください
													// TURE:バージョン情報は有効です
													// FALSE:バージョン情報は有効ではありません 

	BYTE szMachineName[32] ;					// 機種名			(ASCIIZ 文字列)
	BYTE szMachineVersion[32] ; 				// 機種バージョン	(ASCIIZ 文字列)
	BYTE szMachineDate[32] ;					// 機種日付			(ASCIIZ 文字列)
	
	DWORD dwProtocolCounter ;					// 搭載通信プロトコル数
	PROTOCOLVERSION Protocl[MTOM_PROTOCOL_MAX] ;// 搭載通信プロトコルバージョン情報

} GPVERSION,*LPGPVERSION ;


typedef struct tagGPTIMERS {
	DWORD  dwResponsTimer ;						// コマンド送信後,レスポンスを受信するまでの時間
												// この値はdwChactorTimerより長い時間でなければなりません
												// (単位 ｍｓ）

	DWORD  dwChactorTimer ;						// キャラクター間タイマー
												// １フレームの受信が開始され,そのフレームが途中で途切れた場合
												// 再びフレームの残りが受信が再会されるまでの最長時間
												// この値はTCPの再送時間及びdwUDPPacketTimerより長い時間でなければなりません
												// (単位 ｍｓ）

	DWORD	dwUDPPacketTimer ;					// UDP/IPを利用した交信で連続して相手ノードにパケットを送信した場合
												// 相手ノードが処理仕切れない場合があります。
												// それを防ぐ為に,相手ノードの処理の延滞時間を指定します
												// ＡＰＩは複数の連続したUDPパケットを送信する場合
												// １パケット送信後,このタイマーで指定された時間Waitしてから次の
												// パケットを送信します
												// (単位 ｍｓ)

												
} GPTIMERS,*LPGPTIMERS ;


typedef struct tagGPNode {					// ノード レコード
	DWORD dwNodeStatus 	;						// ノードの状態
#define B_dwNodeStatus_Nothing		0				// 空のノード  レコードを示す
#define B_dwNodeStatus_Find			1			// ノードは存在する(処理は正常に終了した)
#define B_dwNodeStatus_NotFind		2			// ノードが存在しない(処理は異常終了した)
#define B_dwNodeStatus_NonAction	3			// このノードは処理の対象外なので処理されなかった


	BOOL dwCheckButton ;						// 処理の対象にするかどうか(アプリケーションユーザーがセットする)
													// TRUE:対象とする
													// FALSE:対象としない

	DWORD dwIPAddress ;							// ノードのＩＰアドレス

} GPNODE,*LPGPNODE;

typedef struct tagGPNetWorkData {			// ネットワークの情報
	DWORD dwNetID ;								// 一斉同報時の対象ネットＩＤ(デフォルトは自ネットＩＤ）
	DWORD dwNodeCounter ;						// ノード レコードの数
	struct tagGPNode Node[1]	;				// 個々のノードの情報(ノード レコード)
												// （配列の要素数が1で定義されていますが実際は動的に変化します,dwNodeCounter
												//   要素数は常にdwNodeCounterを参照するようにしてください)
} GPNETWORKDATA,*LPGPNETWORKDATA ;



		// ＭｔｏＭ用ソケットの型定義
struct tagMtoMSOCK {
	DWORD dwStockSize ;							// ソケットの有効データサイズ ( sizeof(MtoMSOCK) )

	DWORD dwProtocolType ;						// プロトコルの種類
#define B_ProtocolType_SIO	1						// プロトコルの種類はＭｔｏＭ ＳＩＯ
#define B_ProtocolType_LAN	2						// プロトコルの種類はＭｔｏＭ ＬＡＮ

	DWORD dwMtoM_Mode ;							// 現在のソケットの動作モード
#define B_MtoM_Mode_ESC		0					// 描画モード	(ソケット作成時のデフォルト)
#define B_MtoM_Mode_DLE		1					// データ転送モード	(MtoMDLE_Communication()～MtoMDLE_Quit()まで)


	DWORD  dwUser1 ;							// ソケットユーザーが使用してよい３２ビットのワーク
													// 通常コールバック時,ソケットを識別する為のものをソケットユーザーがセットする
													// この値をＡＰＩが変更する事はありません

	DWORD  dwUser2 ;							// ソケットユーザーが使用してよい３２ビットのワークその２
													// この値をＡＰＩが変更する事はありません


	
	GPVERSION  Version ;						// バージョン情報

	MTOMEVENTBACK  pfEventFunc ;				// イベント発生時,コールバックする関数
												// NULL:コールバックしない(デフォルト)

	MTOMEVENTBACK  pfSyncProcess;				// 同期モードで実行中,処理の進行状況を通知する時,コールバックする関数
												// NULL:コールバックしない(デフォルト)

												// 処理の進み具合を示すプログレスバー用データ
	DWORD  dwTotalProcess ;						// 全行程数
	DWORD  dwFinishedProcess ;					// 終了した行程数
												// 注  処理の進み具合を示す値はあくまでもビジュアル的なもので
												//     この値を元に処理の終了を保証するものではありません


	DWORD dwGPNetWorkDataSize ;					// ネットワーク情報のサイズ
	LPGPNETWORKDATA pGPNetWorkData ;			// ネットワーク情報エリアを示すポインター


	GPTIMERS GpTimers ;							// 各種タイマーのタイマー値
		


	// 以下プロトコル別のワークがつづく
	DWORD dwTransitionType ;					// 交信方法
#define B_dwTransitionType_Only1			0		// １ノード指定,コネクションを開設したノードに対して交信を行います
													// デフォルト
													// (TCP/IPを利用します)
								

#define B_dwTransitionType_BroadCast 		1		// 不特定多数のノードに対して交信する(レスポンスチェックはしない)
													// この方法は相手ノードの処理スピードを一切考慮していないので,連続して交信すると
													// 相手ノードが処理仕切れない場合があります
													// (UDP/IPの一斉同報を利用します)


#define B_dwTransitionType_Specific 		2		// 特定ノード(ネットワークの情報エリアで処理対象に選ばれているノードに対して)交信する
													// レスポンスはネットワークの情報エリアで最初に処理対象となっているノードからの
													// レスポンスだけを有効として扱われます
													// つまり,先頭のノードが,全ノードの代表として使用されます
													// 本交信方法は複数ノードに対しての描画コマンドなのに使用します
													// (UDP/IPの一斉同報を利用します)


#define B_dwTransitionType_SpecificCheck	3		// 特定ノード(ネットワークの情報エリアで処理対象に選ばれているノードに対して)交信する
													// B_dwTransitionType_Specificとの違いは,処理コマンド送信後,
													// レスポンスはまず,ネットワークの情報エリアで最初に処理対象となっているノードからの
													// レスポンスだけを有効として扱い,その後,残りのノードに対して,
													// １ノードづつ処理の正否を確認します
													// 本交信方法は複数ノードに対しての厳密な処理の期待する場合,
													// 例えばファイル転送などに使用します
													// (UDP/IPの一斉同報を利用します)

#if	MTOM_PRONET
#define B_dwTransitionType_PFN				4		// Pro Face Net用モード(UDP/IP)
													// 先頭がBellコードの電文受信時Callback関数を呼び出します
													// Pro Face Net用の関数以外を呼んだ時はB_dwTransitionType_BroadCastと同様
#endif

	DWORD dwIPAddress ;							// 交信相手ノードのＩＰアドレス
	DWORD dwPortNo ;							// 交信相手ノードのポート番号
	DWORD dwLocalIPAddress ;					// 自局ノードのＩＰアドレス
	DWORD dwLocalPortNo ;						// 自局ノードのポート番号


	void	*pDLLWork;	

	DWORD	dwErrorCode;						// Details of error
} ;


typedef BYTE GPCOLOR ;							// 色指定
typedef BYTE GPLINESTYLE ;						// 線種
typedef BYTE GPFONTSIZE ;						// フォントサイズ
typedef BYTE GPTILE ;							// タイルパターン
typedef BYTE GPFONT ;							// フォントの種類


#define W_ESC_T_MAX		80			// ESC Tで 一度に表示出来る最文字数
#define W_ESC__t_MAX	80			// ESC tで 一度に表示出来る最文字数
#define W_ESC_R_MAX		64			// ESC Rの最大アクセス数
#define W_ESC_W_MAX		64			// ESC Wの最大アクセス数


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
