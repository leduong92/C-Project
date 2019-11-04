#include "winprint.h"
#define TEXTHIGHT 20
extern HWND hwnd;
extern HDC tmpDC;

static	char sLine[30][141];			// 仮想の原稿用紙
static	short NowLine;
static 	char s[5012];
char *TT1="    NOK生産技術部 配送管理システム  For Windows95 Version β";
char *TT2="          Copyright (C) 1996-1997 NOK技術本部生産技術部";

//=======================================================================================================
//	int WinPrintf(const char *, ...);
// 強制的にprintfを使う関数（使わない方がいいと思われます。応急処置です）
// こういった処理はふつうはやりません。
// hwnd を外部変数宣言しておいて下さい。そうしないとどこに表示するのかわからなくなります。
// あと、再描画用のデバイスコンテキスト(tmpDC)も、設定しておいて下さい。これはメインウインドウ作成時に
// 同時に作っといて下さい。
// 画面サイズは30*140です。上３行はタイトル用にしてあります。
// 制御文字のサポートはありません（黒塗りスペースで表示されるみたいです）。意識的に改行したいときはご面倒ですが
// 命令を２回に分けて下さい。
// １４０字を超える場合は、自動改行処理を行う予定。全角文字が化けるかもしれません。（確率1/2)
//========================================================================================================
int WinPrintf(const char *format, ...)
  {
    va_list ap;
	HDC hdc;
	short txtlen,i;
	char tmpLine[141+300];
	short nowlen,nowptr;


	// <printf>
	memset(s,0,sizeof(s));
    va_start (ap, format);	/* Variable argument begin */
	vsprintf(s,format,ap);
    va_end (ap);		/* Variable argument end */

	// 画面表示
	hdc=GetDC(hwnd);
	// タイトル
/*	小林君　ごめんよ　ふたをさせてもらう　！！
	HDC hdcd;
	TextOut(hdc,0,20,TT2,strlen(TT2));

	TextOut(tmpDC,0,0,TT1,strlen(TT1));
	TextOut(tmpDC,0,20,TT2,strlen(TT2));
*/
	// すべての文字を表示。（自動改行）
	nowlen=0;
	nowptr=0;
	txtlen=strlen(s);

	do{
		// 残りの文字数とか改行とかを調べる
		nowlen = txtlen - nowptr;
		if( nowlen > 140 ) nowlen = 140;

		// 今回表示する行のデータをコピー
		memset(tmpLine,0x20,141);
		memcpy( tmpLine, &s[nowptr],nowlen);
		tmpLine[140]=0;


		if(NowLine >=30 ){
			memmove(sLine,sLine[1],141*29);
			memcpy(sLine[29],tmpLine,140);
		}
		else{
			memcpy(sLine[NowLine],tmpLine,140);
			NowLine++;
		}

		//原稿
		memset(tmpLine,0x20,sizeof(tmpLine));
		for(i=0;i<30;i++){
			TextOut(hdc,0,60+(i*TEXTHIGHT),tmpLine,340);
			TextOut(hdc,0,60+(i*TEXTHIGHT),sLine[i],140);
			TextOut(tmpDC,0,60+(i*TEXTHIGHT),tmpLine,340);
			TextOut(tmpDC,0,60+(i*TEXTHIGHT),sLine[i],140);
		}

		// 条件更新
		nowptr += nowlen;

	}while( txtlen != nowptr);

    ReleaseDC(hwnd,hdc);
	return (0);	/* According to ANSI */
  }


// 仮想原稿用紙の初期化
void WpInit(void)
{
	memset(&sLine,0x20,sizeof(sLine));
	NowLine=0;

}

