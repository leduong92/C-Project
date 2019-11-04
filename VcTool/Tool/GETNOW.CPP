//=======================================================================================================
//
//        乻 斈梡 乼
//
//        尰帪崗庢摼僾儘僌儔儉
//        File name : GETNOW.C
//        媄弍杮晹  惗嶻媄弍晹
//        嶌惉幰 丗 彫椦
//        嶌惉擔 丗 1996.1.11
//		  廋惓擔 丗 1996.5.07 take
//					俠俆偱偺儚乕僯儞僌乮pointer mismatch乯傪嶍彍
//				 丗 1996.12.09 Koba
//					儂僗僩憲怣宍幃(?YYMMDDHHMMSS)懳墳
//				 丗 1999.04.08 Coba
//					儂僗僩憲怣宍幃偱偺抳柦揑僶僌偺廋惓 1 仺 '1'
//					2008.04.03 下擂
//					CvDateTime傪捛壛
//					get_yobi傪捛壛 2008.08.18 下擂
//=======================================================================================================


#include "windows.h"
#include "getnow.h"

int UruuCheck(int year);

/*-----------------------------------------------------------------------------
 * 	娭悢柤:		void	get_nowtime(char *buf,short mode, short plus )
 *
 * 	奣梫丂:		尰嵼偺帪崗傪巜掕偟偨暥帤楍宆偱庢摼偡傞丅
 * 	堷悢侾:		奿擺偡傞億僀儞僞
 *  堷悢俀:     庢摼儌乕僪
 *  堷悢俁丗丂丂壛偊傞擔悢乮擔乯
 * 	曉偟抣:		側偟
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

// 擔悢寁嶼
void Calctime( char *buf,char *date, short plus )
{
	time_t 	ltime=0;
	struct	tm	nowtime,*newtime;
    char dummy[20];

	memset( &nowtime,0,sizeof(nowtime));

	// 婎弨擔傪宱夁昩偵偡傞

	// 擭
    memset( dummy, 0 , sizeof(dummy));
	memcpy(dummy,date,4);
	nowtime.tm_year = atol(dummy)-1900;

	// 寧
    memset( dummy, 0 , sizeof(dummy));
	memcpy(dummy,&date[4],2);
	nowtime.tm_mon = atol(dummy)-1;

	// 擔
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
 * 	娭悢柤:		void	get_nowtime(char *buf,short mode, short plus )
 *
 * 	奣梫丂:		尰嵼偺帪崗傪巜掕偟偨暥帤楍宆偱庢摼偡傞丅
 * 	堷悢侾:		奿擺偡傞億僀儞僞
 *  堷悢俀:     庢摼儌乕僪
 *  堷悢俁丗丂丂壛偊傞帪娫
 *  堷悢係丗丂丂壛偊傞帪娫偺扨埵
 * 	曉偟抣:		側偟
 *---------------------------------------------------------------------------*/
void	get_nowtime2( char *buf, short mode, long plus ,int tani)
{
	time_t 	ltime;
	struct	tm	*nowtime;
    char dummy[20];

    memset( dummy, 0 , sizeof(dummy));

	time(&ltime);

	// 帪娫偺懌偟崬傒
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
 * 	娭悢柤:		void NglogAdd(char *prgname ,char*  mode, short code )
 *
 * 	奣梫丂:		僄儔乕儊僢僙乕僕傪崱擔偺擔晅偺僄儔乕抬操偵彂偒崬傓丅
 * 	堷悢侾:		僄儔乕傪堷偒婲偙偟偨僾儘僌儔儉柤
 *  堷悢俀:     儊僢僙乕僕
 *  堷悢俁丗丂丂僄儔乕僐乕僪乮僐乕僪偑柍偄偲偒偼 0 傪憲傞偲旕昞帵偵側傝傑偡乯
 *  堷悢係丗丂丂曐懚僨傿儗僋僩儕丅徣棯壜
 * 	曉偟抣:		側偟
 *---------------------------------------------------------------------------*/
void NglogAdd(char *prgname, char *msg, short code,char* dir)
{
	char filename[MAX_PATH+1];
	char now[HDATEL+1];
	char buf[24];
	FILE *fp;
	WIN32_FIND_DATA fd;
	HANDLE hFind;

   // 僼傽僀儖柤偺僙僢僩乮僼傽僀儖柤偼崱擔偺擔晅.err乯
   memset(now,      0 ,sizeof(now));
   get_nowtime( now,SDATEL,0);

	// 僨傿儗僋僩儕巜掕側偟
	if( dir==NULL){
		// 棉ㄚ改貍饘煃鮽祦A柍偗傟偽嶌惉偡傞
		strcpy( filename,".\\log\0\0");
		memset( &fd, 0, sizeof(fd) );
		fd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		hFind = FindFirstFile(filename,&fd);
		// 柍偄応崌偼僨傿儗僋僩儕嶌惉
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

   // 僼傽僀儖僆乕僾儞
   if( (fp = fopen( filename,"ab")) == NULL ){
//      printf("\n僄儔乕僼傽僀儖僆乕僾儞幐攕両両両");
      return;
   }

   // 僄儔乕撪梕彂偒崬傒
   memset(now, 0 ,sizeof(now));        // 敪惗擔帪
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

   fprintf(fp, "%s:",prgname);        // 僾儘僌儔儉柤
   fprintf(fp, "%s",msg);
   if(code)
	   fprintf(fp,":err=%d\r\n",code);	// 2010.11.03 coba \r捛壛
   else
	   fprintf(fp, "\r\n");				// 2010.11.03 coba \r捛壛

   fclose(fp);
}

// 擔悢寁嶼
/*-----------------------------------------------------------------------------
 * 	娭悢柤:		void Calctime( char *buf,char *date, short plus ,short plus)
 *
 * 	奣梫丂:		尰嵼偺帪崗傪巜掕偟偨暥帤楍宆偱庢摼偡傞丅
 * 	堷悢侾:		奿擺偡傞億僀儞僞
 *  堷悢俀:     婎弨擔乮YYYYMMDDHHMMSS乯
 *  堷悢俁丗丂丂壛偊傞帪娫
 *  堷悢係丗丂丂壛偊傞帪娫偺扨埵
 * 	曉偟抣:		側偟
 *---------------------------------------------------------------------------*/
void Calctime( char *buf,char *date, int plus ,short tani)
{
	time_t 	ltime=0;
	struct	tm	nowtime,*newtime;
    char dummy[20];

	memset( &nowtime,0,sizeof(nowtime));

	// 婎弨擔傪宱夁昩偵偡傞

	// 擭
    memset( dummy, 0 , sizeof(dummy));
	memcpy(dummy,date,4);
	nowtime.tm_year = atol(dummy)-1900;

	// 寧
	memcpy(dummy,&date[4],2);
	dummy[2] = 0;
	nowtime.tm_mon = atol(dummy)-1;

	// 擔
	memcpy(dummy,&date[6],2);
	dummy[2] = 0;
	nowtime.tm_mday=atol(dummy);

	// 帪
	memcpy(dummy,&date[8],2);
	dummy[2] = 0;
	nowtime.tm_hour=atol(dummy);

	// 暘
	memcpy(dummy,&date[10],2);
	dummy[2] = 0;
	nowtime.tm_min=atol(dummy);

	// 昩
	memcpy(dummy,&date[12],2);
	dummy[2] = 0;
	nowtime.tm_sec=atol(dummy);

	// tm->time_t
	ltime = mktime(&nowtime);

	// 擔悢傪偲傞
	// 帪娫偺懌偟崬傒
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

//---------------------------------------------------------------------------------------------------------------------
//	void CvDateTime(char *src,char *dest,int mode)
//	婡擻丂丗20080305161102側偳偺暥帤楍偵 '/' 傗丄':'傪擖傟偨宍幃偵曄姺
//	堷悢侾丗曄姺尦偵側傞暥帤楍
//	堷悢俀丗曄姺屻丄奿擺偡傞僶僢僼傽
//	堷悢俁丗奺儌乕僪
//	2008.04.03 下擂
//---------------------------------------------------------------------------------------------------------------------
void CvDateTime(char *src,char *dest,int mode)
{
	//帪崗宆
	if( mode == CVMODE_TIME ){
		if( strlen(src) < 6 ) return;
		memcpy(dest,src,2);
		memcpy(dest+2,":",1);
		memcpy(dest+3,src+2,2);
		memcpy(dest+5,":",1);
		memcpy(dest+6,src+4,2);
	}

	//擔晅宆
	if( mode == CVMODE_DATE ){
		if( strlen(src) < 8 ) return;
		memcpy(dest,src,4);
		memcpy(dest+4,"/",1);
		memcpy(dest+5,src+4,2);
		memcpy(dest+7,"/",1);
		memcpy(dest+8,src+6,2);
	}
	
	//擔晅帪崗宆
	if( mode == CVMODE_DATETIME ){
		if( strlen(src) < 14 ) return;
		memcpy(dest,src,4);
		memcpy(dest+4,"/",1);
		memcpy(dest+5,src+4,2);
		memcpy(dest+7,"/",1);
		memcpy(dest+8,src+6,2);
		memcpy(dest+10," ",1);
		memcpy(dest+11,src+8,2);
		memcpy(dest+13,":",1);
		memcpy(dest+14,src+10,2);
		memcpy(dest+16,":",1);
		memcpy(dest+17,src+12,2);
	}
}
//---------------------------------------------------------------------------------------------------------------------
//	int get_yobi(char *date,char *dest)
//	婡擻丂丗梛擔傪庢摼偡傞
//	堷悢侾丗梛擔傪嶼弌偡傞擔
//	栠傝抣丗梛擔傪昞偡悢抣 0:擔梛乣6:搚梛擔
//	仜梛擔傪昞帵偟偨偄応崌偵偼丄YOBI_TBLJP[]偵栠傝抣傪擖傟偰偔偩偝偄
//	2008.08.18 下擂
//---------------------------------------------------------------------------------------------------------------------
int get_yobi(char *date)
{
	int y,m,d,yobi;
	long count;
	char buf[20];
	int     MTBL[12]=  { 0,31,59,90,120,151,181,212,243,273,304,334 };

	//8僶僀僩偼昁恵
	if( strlen(date) < 8 )return -1;
	
	memcpy(buf,date,4);buf[4]=0;
	y=atoi(buf);
	memcpy(buf,date+4,2);buf[2]=0;
	m=atoi(buf);
	memcpy(buf,date+6,2);buf[2]=0;
	d=atoi(buf);

	//惣楋侾擭侾寧侾擔偐傜偺捠嶼擔悢傪挷傋傞
    count=  (y-1)*365L;            //擭*俁俇俆
    count+= (y/4L - y/100L + y/400L + MTBL[m-1] + d);
    if( m<3 && UruuCheck(y) )  count--;

	yobi= count%7L;
	return yobi;
}
//---------------------------------------------------------------------------------------------------------------------
//	int uruu(int year)
//	偆傞偆擭联
//---------------------------------------------------------------------------------------------------------------------
int UruuCheck(int year)
{
	if (year%400L==0L)  return(1);
	if(year%100L==0L)  return(0);
	if(year%4L==0L)    return(1);
	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	int check_date(char* date)
//	婡擻丂丗擔晅偺懨摉惈僠僃僢僋
//	堷悢侾丗僠僃僢僋偡傞擔晅
//	栠傝抣丗侽丗惓忢側擔晅 偦傟埲奜丗晄惓側擔晅
//---------------------------------------------------------------------------------------------------------------------
int check_date(char* date)
{
	char buf[50];

	//寘懌傜偢
	if( strlen(date) < 8 ) return -1;

	int y,m,d;
	memcpy(buf,date,4);buf[4]=0;
	y = atoi(buf);
	memcpy(buf,date+4,2);buf[2]=0;
	m = atoi(buf);
	memcpy(buf,date+6,2);buf[2]=0;
	d = atoi(buf);

	if( m < 1 || m > 12 ) return -2;//寧偑晄惓

	int nUruu = 0;
	int nMaxDay;

	//塠擭偺妋擣
	nUruu = UruuCheck(y);
	//偦偺寧偺嵟戝擔晅傪寛掕
	switch(m) {
		case 2:nMaxDay = 28+nUruu;
			break;
		case 4:
		case 6:
		case 9:
		case 11: nMaxDay = 30; break;
		default: nMaxDay = 31; break;
	}
	if( d > nMaxDay ) return -3;	//擔晅偑晄惓

	return 0;	//惓忢
}

//---------------------------------------------------------------------------------------------------------------------
// 2013.08.06 coba
// int SubDays(int *days,char* date1,char* date2)
// 擔晅偺堷偒嶼(擔) date1-date2
// 堷悢2-堷悢1偺寢壥傪堷悢1偵僙僢僩
// 堷悢侾:擔晅偺嵎(擔)
// 堷悢俀:擔晅1
// 堷悢俁:擔晅2
// 栠傝抣:0=OK
//---------------------------------------------------------------------------------------------------------------------
int SubDays(int *days,const char* date1,const char* date2)
{
	int y,m,d;
	long count1,count2;
	char buf[20];
	int     MTBL[12]=  { 0,31,59,90,120,151,181,212,243,273,304,334 };

	if( strlen(date1) < 8 || strlen(date2) < 8 ) return(-1);

	//惣楋侾擭侾寧侾擔偐傜偺捠嶼擔悢傪挷傋傞:date1
	memcpy(buf,date1,4);buf[4]=0;
	y=atoi(buf);
	memcpy(buf,date1+4,2);buf[2]=0;
	m=atoi(buf);
	memcpy(buf,date1+6,2);buf[2]=0;
	d=atoi(buf);
    count1=  (y-1)*365L;            //擭*俁俇俆
    count1+= (y/4L - y/100L + y/400L + MTBL[m-1] + d);
    if( m<3 && UruuCheck(y) )  count1--;

	//惣楋侾擭侾寧侾擔偐傜偺捠嶼擔悢傪挷傋傞:date2
	memcpy(buf,date2,4);buf[4]=0;
	y=atoi(buf);
	memcpy(buf,date2+4,2);buf[2]=0;
	m=atoi(buf);
	memcpy(buf,date2+6,2);buf[2]=0;
	d=atoi(buf);
    count2=  (y-1)*365L;            //擭*俁俇俆
    count2+= (y/4L - y/100L + y/400L + MTBL[m-1] + d);
    if( m<3 && UruuCheck(y) )  count2--;
//printf("%d,%d\n",count1,count2);
	// 嵎偺擔悢
	*days=(int)(count1-count2);

	return(0);
}

//---------------------------------------------------------------------------------------------------------------------
// 2013.08.06 coba
// int SubDays(int *days,char* date1,char* date2)
// 擔晅偺堷偒嶼(昩) date1-date2
// 堷悢2-堷悢1偺寢壥傪堷悢1偵僙僢僩
// 堷悢侾:擔晅偺嵎(昩)
// 堷悢俀:擔晅1
// 堷悢俁:擔晅2
// 栠傝抣:0=OK
//---------------------------------------------------------------------------------------------------------------------
int SubSeconds(long* seconds,const char* date1,const char* date2)
{
	int y,m,d;
	int hour, minit, second;
	long day1, day2, days;
	long time1, time2;
	//long count1,count2;
	char buf[20];
	int     MTBL[12]=  { 0,31,59,90,120,151,181,212,243,273,304,334 };

	if( strlen(date1) < 14 || strlen(date2) < 14 ) 
		return(-1);

	//惣楋侾擭侾寧侾擔偐傜偺捠嶼擔悢傪挷傋傞:date1
	memcpy(buf,date1,4);buf[4]=0;
	y=atoi(buf);
	memcpy(buf,date1+4,2);buf[2]=0;
	m=atoi(buf);
	memcpy(buf,date1+6,2);buf[2]=0;
	d=atoi(buf);
    day1=  (y-1)*365L;            //擭*俁俇俆
    day1+= (y/4L - y/100L + y/400L + MTBL[m-1] + d);
    if( m<3 && UruuCheck(y) )  
		day1--;

	//惣楋侾擭侾寧侾擔偐傜偺捠嶼擔悢傪挷傋傞:date2
	memcpy(buf,date2,4);buf[4]=0;
	y=atoi(buf);
	memcpy(buf,date2+4,2);buf[2]=0;
	m=atoi(buf);
	memcpy(buf,date2+6,2);buf[2]=0;
	d=atoi(buf);
    day2=  (y-1)*365L;            //擭*俁俇俆
    day2+= (y/4L - y/100L + y/400L + MTBL[m-1] + d);
    if( m<3 && UruuCheck(y) )  
		day2--;

	//擔晅偺寁嶼
	days = day1 - day2;

	//hh:mm:ss傪昩偵曄姺偡傞:date1
	memcpy(buf,date1+8,2);buf[2]=0;
	hour=atoi(buf);
	memcpy(buf,date1+10,2);buf[2]=0;
	minit=atoi(buf);
	memcpy(buf,date1+12,2);buf[2]=0;
	second=atoi(buf);
	time1 = hour*60*60 + minit*60 + second;

	//hh:mm:ss傪昩偵曄姺偡傞:date2
	memcpy(buf,date2+8,2);buf[2]=0;
	hour=atoi(buf);
	memcpy(buf,date2+10,2);buf[2]=0;
	minit=atoi(buf);
	memcpy(buf,date2+12,2);buf[2]=0;
	second=atoi(buf);
	time2 = hour*3600 + minit*60 + second;

	//昩悢傪寁嶼
	*seconds = days*24*60*60 +(time1 - time2);

	return(0);
}
//---------------------------------------------------------------------------------------------------------------------
//	void CvDateTimeUS(char *src,char *dest,int mode)
//	婡擻丂丗20080305161102側偳偺暥帤楍偵 '/' 傗丄':'傪擖傟偨宍幃偵曄姺:US斉 DD-MM-YYYY
//	堷悢侾丗曄姺尦偵側傞暥帤楍
//	堷悢俀丗曄姺屻丄奿擺偡傞僶僢僼傽
//	堷悢俁丗奺儌乕僪
//	2015.05.09 coba
//---------------------------------------------------------------------------------------------------------------------
void CvDateTimeUS(char *src,char *dest,int mode)
{
	//帪崗宆
	if( mode == CVMODE_TIME ){
		if( strlen(src) < 6 ) return;
		memcpy(dest,src,2);
		memcpy(dest+2,":",1);
		memcpy(dest+3,src+2,2);
		memcpy(dest+5,":",1);
		memcpy(dest+6,src+4,2);
	}

	//擔晅宆
	if( mode == CVMODE_DATE ){
		if( strlen(src) < 8 ) return;
		memcpy(dest,src+6,2);
		memcpy(dest+2,"-",1);
		memcpy(dest+3,src+4,2);
		memcpy(dest+5,"-",1);
		memcpy(dest+6,src,4);
	}
	
	//擔晅帪崗宆
	if( mode == CVMODE_DATETIME ){
		if( strlen(src) < 14 ) return;
		memcpy(dest,src+6,2);
		memcpy(dest+2,"-",1);
		memcpy(dest+3,src+4,2);
		memcpy(dest+5,"-",1);
		memcpy(dest+6,src,4);
		memcpy(dest+10," ",1);
		memcpy(dest+11,src+8,2);
		memcpy(dest+13,":",1);
		memcpy(dest+14,src+10,2);
		memcpy(dest+16,":",1);
		memcpy(dest+17,src+12,2);
	}
}
