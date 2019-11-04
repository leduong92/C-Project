//////////////////////////////////////////////////////////////////
// クラス   ::TArchive
// ﾍｯﾀﾞｰﾌｧｲﾙ::TArchive.h
// 機能::ﾌｧｲﾙ圧縮、解凍を実行する
// 
// インポートライブラリ UNLHAVC.LIB
// 作成::2005.01.25 ﾏﾂﾀﾞ
//////////////////////////////////////////////////////////////////

#include "TArchive.h"


//================================================================
//  関数名：int Freeze(char *outFile, char *arcFile)
//  機能　：ﾌｧｲﾙを圧縮する
//  引数１：出力ファイル名
//  引数２：圧縮ファイル名(ファイル名とファイル名の間に1バイトｽﾍﾟｰｽを入れること)
//================================================================
int Freeze(char *outFile, char *arcFile)
{
	char command[1024];
	char buf[256];
	int ret;

	memset(buf,0,sizeof(buf));
	memset(command,0,sizeof(command));

	sprintf(command,"a %s %s",outFile,arcFile);
	
	ret = Unlha(NULL, command ,buf, sizeof(buf)-1);
	if(ret) return -1;

	return 0;
}
//================================================================
//  関数名：int Melt(char *fname, char *dest,,BOOL renew)
//  引数１：解凍対象ﾌｧｲﾙ
//  引数２：解凍先ﾌｫﾙﾀﾞ
//	引数３：自動上書き可否 TRUE : 自動上書き FALSE:自動上書きしない
//================================================================
int Melt(char *fname, char *dest,BOOL renew)
{
	char command[1024];
	char buf[256];
	int ret;

	memset(buf,0,sizeof(buf));
	memset(command,0,sizeof(command));

	if(renew == TRUE) 	sprintf(command,"e %s -u0 -y1 %s\\",fname,dest);
	else sprintf(command,"e %s %s\\",fname,dest);
	
	ret = Unlha(NULL, command ,buf, sizeof(buf)-1);
	if(ret) return -1;

	return 0;
}
