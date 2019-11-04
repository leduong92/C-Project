//=======================================================================================================
//
//        《 汎用 》
//
//        現時刻取得プログラム
//        File name : GETNOW.C
//        技術本部  生産技術部
//        作成者 ： 小林
//        作成日 ： 1996.1.11
//		  修正日 ： 1996.5.07 take
//					Ｃ５でのワーニング（pointer mismatch）を削除
//				 ： 1996.12.09 Koba
//					ホスト送信形式(?YYMMDDHHMMSS)対応
//				 ： 1999.04.08 Coba
//					ホスト送信形式での致命的バグの修正 1 → '1'
//=======================================================================================================


#include "windows.h"
#include "getnow.h"

/*-----------------------------------------------------------------------------
 * 	関数名:		void	get_nowtime(char *buf,short mode, short plus )
 *
 * 	概要　:		現在の時刻を指定した文字列型で取得する。
 * 	引数１:		格納するポインタ
 *  引数２:     取得モード
 *  引数３：　　加える日数（日）
 * 	返し値:		なし
 *---------------------------------------------------------------------------*/
void	get_nowtime( char *buf, short mode, short plus )
{
	time_t 	ltime;
	struct	tm	*nowtime;
    char dummy[20];
	TMSTRCT *outTime;

    memset( dummy, 0 , sizeof(dummy));

	time(&ltime);
	ltime += (time_t)(plus)*24*60*60;
	nowtime=localtime(&ltime);

    if(nowtime->tm_year >= 95){
       nowtime->tm_year += 1900;
    }
    else{
       nowtime->tm_year += 2000;
    }

	sprintf(dummy,"%04d%02d%02d%02d%02d%02d", nowtime->tm_year,
                      nowtime->tm_mon+1,nowtime->tm_mday,nowtime->tm_hour,
                                   nowtime->tm_min,nowtime->tm_sec	);

    switch( mode){
       case HDATEL:
          memcpy( buf, dummy, HDATEL );
          break;

       case LDATEL:
          memcpy( buf, dummy, LDATEL );
          break;

       case MDATEL:
          memcpy( buf, dummy, MDATEL );
          break;

       case SDATEL:
          memcpy( buf, dummy, SDATEL );
          break;

       case HOST_L:
		  if( memcmp( &dummy[2],"93", 2) > 0 )
				buf[0]='0';
		  else
				buf[0]='1';
		  memcpy( &buf[1],&dummy[2],HOST_L);
		  break;

		case TIMEST:
			outTime = (TMSTRCT*)buf;
			outTime->year = nowtime->tm_year;
			outTime->month = nowtime->tm_mon+1;
			outTime->day = nowtime->tm_mday;
			outTime->hour = nowtime->tm_hour;
			outTime->min = nowtime->tm_min;
			outTime->sec = nowtime->tm_sec;
          break;
	   default:
          break;
   }

}

// 日数計算
void Calctime( char *buf,char *date, short plus )
{
	time_t 	ltime=0;
	struct	tm	nowtime,*newtime;
    char dummy[20];

	memset( &nowtime,0,sizeof(nowtime));

	// 基準日を経過秒にする

	// 年
    memset( dummy, 0 , sizeof(dummy));
	memcpy(dummy,date,4);
	nowtime.tm_year = atol(dummy)-1900;

	// 月
    memset( dummy, 0 , sizeof(dummy));
	memcpy(dummy,&date[4],2);
	nowtime.tm_mon = atol(dummy)-1;

	// 日
    memset( dummy, 0 , sizeof(dummy));
	memcpy(dummy,&date[6],2);
	nowtime.tm_mday=atol(dummy);

	ltime = mktime(&nowtime);
	ltime += (time_t)(plus)*24*60*60;

	newtime=localtime(&ltime);
    if(newtime->tm_year >= 95){
       newtime->tm_year += 1900;
    }
    else{
       newtime->tm_year += 2000;
    }

	sprintf(dummy,"%04d%02d%02d%02d%02d%02d", newtime->tm_year,
                      newtime->tm_mon+1,newtime->tm_mday,newtime->tm_hour,
                                   newtime->tm_min,newtime->tm_sec	);

	memcpy( buf, dummy, 14 );

}

/*-----------------------------------------------------------------------------
 * 	関数名:		void	get_nowtime(char *buf,short mode, short plus )
 *
 * 	概要　:		現在の時刻を指定した文字列型で取得する。
 * 	引数１:		格納するポインタ
 *  引数２:     取得モード
 *  引数３：　　加える時間
 *  引数４：　　加える時間の単位
 * 	返し値:		なし
 *---------------------------------------------------------------------------*/
void	get_nowtime2( char *buf, short mode, long plus ,int tani)
{
	time_t 	ltime;
	struct	tm	*nowtime;
    char dummy[20];

    memset( dummy, 0 , sizeof(dummy));

	time(&ltime);

	// 時間の足し込み
	switch( tani ){
		case MD_SEC:
			ltime += (time_t)plus;
			break;
		case MD_MIN :
			ltime += (time_t)(plus)*60;
			break;
		case MD_HOUR:
			ltime += (time_t)(plus)*60*60;
			break;
		case MD_DAY:
			ltime += (time_t)(plus)*24*60*60;
			break;
		default:
			break;
		
	}

	nowtime=localtime(&ltime);

    if(nowtime->tm_year >= 95){
       nowtime->tm_year += 1900;
    }
    else{
       nowtime->tm_year += 2000;
    }

	sprintf(dummy,"%04d%02d%02d%02d%02d%02d", nowtime->tm_year,
                      nowtime->tm_mon+1,nowtime->tm_mday,nowtime->tm_hour,
                                   nowtime->tm_min,nowtime->tm_sec	);

    switch( mode){
       case HDATEL:
          memcpy( buf, dummy, HDATEL );
          break;

       case LDATEL:
          memcpy( buf, dummy, LDATEL );
          break;

       case MDATEL:
          memcpy( buf, dummy, MDATEL );
          break;

       case SDATEL:
          memcpy( buf, dummy, SDATEL );
          break;

       case HOST_L:
		  if( memcmp( &dummy[2],"93", 2) > 0 )
				buf[0]='0';
		  else
				buf[0]='1';
		  memcpy( &buf[1],&dummy[2],HOST_L);
		  break;

	   default:
          break;
   }

}

/*-----------------------------------------------------------------------------
 * 	関数名:		void NglogAdd(char *prgname ,char*  mode, short code )
 *
 * 	概要　:		エラーメッセージを今日の日付のエラーﾌｧｲﾙに書き込む。
 * 	引数１:		エラーを引き起こしたプログラム名
 *  引数２:     メッセージ
 *  引数３：　　エラーコード（コードが無いときは 0 を送ると非表示になります）
 *  引数４：　　保存ディレクトリ。省略可
 * 	返し値:		なし
 *---------------------------------------------------------------------------*/
void NglogAdd(char *prgname, char *msg, short code,char* dir)
{
	char filename[MAX_PATH+1];
	char now[HDATEL+1];
	char buf[24];
	FILE *fp;
	WIN32_FIND_DATA fd;
	HANDLE hFind;

   // ファイル名のセット（ファイル名は今日の日付.err）
   memset(now,      0 ,sizeof(now));
   get_nowtime( now,SDATEL,0);

	// ディレクトリ指定なし
	if( dir==NULL){
		// ﾃﾞｨﾚｸﾄﾘを検索し、無ければ作成する
		strcpy( filename,".\\log\0\0");
		memset( &fd, 0, sizeof(fd) );
		fd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		hFind = FindFirstFile(filename,&fd);
		// 無い場合はディレクトリ作成
		if( hFind == INVALID_HANDLE_VALUE){
			if(!CreateDirectory(filename,NULL)){
				FindClose(hFind);
				return;
			}
		}

		FindClose(hFind);
		sprintf(filename, ".\\log\\%s.err",now);
	}
	else{
		sprintf(filename, "%s\\%s.err",dir,now);
	}

   // ファイルオープン
   if( (fp = fopen( filename,"ab")) == NULL ){
//      printf("\nエラーファイルオープン失敗！！！");
      return;
   }

   // エラー内容書き込み
   memset(now, 0 ,sizeof(now));        // 発生日時
   memset(buf, 0 ,sizeof(buf));
   get_nowtime( now,HDATEL,0);
   memcpy(buf,&now[2],2);
   strcat(buf,"/");
   memcpy(&buf[3],&now[4],2);
   strcat(buf,"/");
   memcpy(&buf[6],&now[6],2);
   strcat(buf," ");
   memcpy(&buf[9],&now[8],2);
   strcat(buf,":");
   memcpy(&buf[12],&now[10],2);
   strcat(buf,":");
   memcpy(&buf[15],&now[12],2);
   fprintf(fp, "%s:",buf);

   fprintf(fp, "%s:",prgname);        // プログラム名
   fprintf(fp, "%s",msg);
   if(code)
	   fprintf(fp,":err=%d\n",code);
   else
	   fprintf(fp, "\n");

   fclose(fp);
}

// 日数計算
/*-----------------------------------------------------------------------------
 * 	関数名:		void Calctime( char *buf,char *date, short plus ,short plus)
 *
 * 	概要　:		現在の時刻を指定した文字列型で取得する。
 * 	引数１:		格納するポインタ
 *  引数２:     基準日（YYYYMMDDHHMMSS）
 *  引数３：　　加える時間
 *  引数４：　　加える時間の単位
 * 	返し値:		なし
 *---------------------------------------------------------------------------*/
void Calctime( char *buf,char *date, int plus ,short tani)
{
	time_t 	ltime=0;
	struct	tm	nowtime,*newtime;
    char dummy[20];

	memset( &nowtime,0,sizeof(nowtime));

	// 基準日を経過秒にする

	// 年
    memset( dummy, 0 , sizeof(dummy));
	memcpy(dummy,date,4);
	nowtime.tm_year = atol(dummy)-1900;

	// 月
	memcpy(dummy,&date[4],2);
	dummy[2] = 0;
	nowtime.tm_mon = atol(dummy)-1;

	// 日
	memcpy(dummy,&date[6],2);
	dummy[2] = 0;
	nowtime.tm_mday=atol(dummy);

	// 時
	memcpy(dummy,&date[8],2);
	dummy[2] = 0;
	nowtime.tm_hour=atol(dummy);

	// 分
	memcpy(dummy,&date[10],2);
	dummy[2] = 0;
	nowtime.tm_min=atol(dummy);

	// 秒
	memcpy(dummy,&date[12],2);
	dummy[2] = 0;
	nowtime.tm_sec=atol(dummy);

	// tm->time_t
	ltime = mktime(&nowtime);

	// 日数をとる
	// 時間の足し込み
	switch( tani ){
		case MD_SEC:
			ltime += (time_t)plus;
			break;
		case MD_MIN :
			ltime += (time_t)(plus)*60;
			break;
		case MD_HOUR:
			ltime += (time_t)(plus)*60*60;
			break;
		case MD_DAY:
			ltime += (time_t)(plus)*24*60*60;
			break;
		default:
			break;
		
	}

	// time_t -> tm
	newtime=localtime(&ltime);
    if(newtime->tm_year >= 95){
       newtime->tm_year += 1900;
    }
    else{
       newtime->tm_year += 2000;
    }

	sprintf(dummy,"%04d%02d%02d%02d%02d%02d", newtime->tm_year,
                      newtime->tm_mon+1,newtime->tm_mday,newtime->tm_hour,
                                   newtime->tm_min,newtime->tm_sec	);

	memcpy( buf, dummy, 14 );

}
