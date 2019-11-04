#ifndef GETNOW_H
#define GETNOW_H
//=============================================================================
//        《 汎用 》
//        現時刻取得プログラムヘッダー
//        File name : GETNOW.H
//        技術本部  生産技術部
//        作成者 ： 小林
//        作成日 ： 1996.1.11
//=============================================================================
// 2010.11.03 coba Nglogadd()の改行コードに\r追加
// 2013.08.06 coba 日付の引き算SubDays()を追加
// 2015.02.23 sanno	日付の引き算(秒)SubSeconds()を追加
// 2015.05.09 coba	CvDateTime()の海外(DD-MM-YYYY)を追加
//=============================================================================
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// getnowtimeの第４引数の定数
#define MD_MIN 0
#define MD_HOUR 1
#define MD_SEC 2
#define MD_DAY 3

#ifndef A_MONTH
#define A_MONTH 30
#endif
#ifndef A_YEAR
#define A_YEAR  12
#endif
#ifndef HDATEL
#define HDATEL	14
#endif
#ifndef LDATEL
#define LDATEL	12
#endif
#ifndef MDATEL
#define MDATEL	10
#endif
#ifndef SDATEL
#define SDATEL	8
#endif
#ifndef HOST_L
#define HOST_L 13
#endif

//2008.04.03 ﾏﾂﾀﾞ
#ifndef CVMODE_TIME
#define CVMODE_TIME 6
#endif
#ifndef CVMODE_DATE
#define CVMODE_DATE 8
#endif
#ifndef CVMODE_DATETIME
#define CVMODE_DATETIME 14
#endif

// getnowtimeの第2引数の定数
#define TIMEST	0			// 数値構造体で返す場合
// 時間取得用の構造体
typedef struct{
	short year;
	short month;
	short day;
	short hour;
	short min;
	short sec;
} TMSTRCT;


void get_nowtime( char *buf,short mode,short plus);
void get_nowtime2( char *buf, short mode, long plus ,int tani);
void NglogAdd(char *prgname, char *msg, short code,char* dir=NULL);
void Calctime( char *buf,char *date, short plus );
void Calctime( char *buf,char *date, int plus ,short tani);
void CvDateTime(char *src,char *dest,int mode);		//2008.04.03 ﾏﾂﾀﾞ
void CvDateTimeUS(char *src,char *dest,int mode);	//2015.05.10 coba
int get_yobi(char *date);
int check_date(char *date);		//2008.11.20 matsuda

static char    YOBI_TBLJP[][4]= { "日","月","火","水","木","金","土" };
static char    YOBI_TBLEN[][4]= { "SUN","MON","TUE","WED","THU","FRI","SAT" };

// 日付の引き算(日) date1-date2 2013.08.06
int SubDays(int *days,const char* date1,const char* date2);
// 日付の引き算(秒)	date1-date2 //2015.02.23 sanno
int SubSeconds( long* seconds, const char* date1, const char* date2 );

#endif
