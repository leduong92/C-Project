#ifndef TARCHIVE_H
#define TARCHIVE_H
//////////////////////////////////////////////////////////////////
// クラス   ::TArchive
// ﾍｯﾀﾞｰﾌｧｲﾙ::TArchive.h
// 機能::ﾌｧｲﾙ圧縮、解凍を実行する
// 
// インポートライブラリ UNLHAVC.LIB
// 作成::2005.01.25 ﾏﾂﾀﾞ
//////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <memory.h>
#include <windows.h>
#include <time.h>
#include "unlha32.h"
//インポートライブラリはUNLHAVC.LIB！！

		int Freeze(char *outFile ,char *arcFile);	//圧縮
		int Melt(char *fname, char *dest,BOOL renew);			//解凍
#ifdef __cplusplus
}
#endif

#endif
