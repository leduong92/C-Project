#ifndef NOK_WINSHELLTOLL_H
#define NOK_WINSHELLTOLL_H
//===================================================================================================================
//
//	FileName	: shtool.h
//				windowsシェル関係のちょっとしたユーティリティ
//				あまりにも古いwindowsでは動かないかも(shellのバージョン4.00(初期の95とか)は？)
//	作成日		:2003.07.16 by coba 
//===================================================================================================================
//	修正履歴
//===================================================================================================================
#ifdef __cplusplus
extern "C"
{
#endif
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

// デスクトップ、スタートメニューなどの特殊フォルダのパスを取得
BOOL GetSpecialFolder( HWND hwnd,int target,char* buf,int bufsize);

// ショートカットの作成
BOOL CreateShellLink( const char *LinkFile, const char *TargetFile, const char *Arguments, const char *WorkDir, int ShowCmd );

#ifdef __cplusplus
}
#endif

#endif
