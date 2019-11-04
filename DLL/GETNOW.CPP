//=======================================================================================================
//        File name : GETNOW.C
//=======================================================================================================

//#include "stdafx.h"

#include "windows.h"
#include "getnow.h"

int UruuCheck(int year);

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

void Calctime( char *buf,char *date, short plus )
{
	time_t 	ltime=0;
	struct	tm	nowtime,*newtime;
    char dummy[20];

	memset( &nowtime,0,sizeof(nowtime));

    memset( dummy, 0 , sizeof(dummy));
	memcpy(dummy,date,4);
	nowtime.tm_year = atol(dummy)-1900;

    memset( dummy, 0 , sizeof(dummy));
	memcpy(dummy,&date[4],2);
	nowtime.tm_mon = atol(dummy)-1;

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

void	get_nowtime2( char *buf, short mode, long plus ,int tani)
{
	time_t 	ltime;
	struct	tm	*nowtime;
    char dummy[20];

    memset( dummy, 0 , sizeof(dummy));

	time(&ltime);

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


void NglogAdd(char *prgname, char *msg, short code,char* dir)
{
	char filename[MAX_PATH+1];
	char now[HDATEL+1];
	char buf[24];
	FILE *fp;
	WIN32_FIND_DATA fd;
	HANDLE hFind;

   
   memset(now,      0 ,sizeof(now));
   get_nowtime( now,SDATEL,0);

	
	if( dir==NULL){
		
		strcpy( filename,".\\log\0\0");
		memset( &fd, 0, sizeof(fd) );
		fd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		hFind = FindFirstFile(filename,&fd);
		
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

  
   if( (fp = fopen( filename,"ab")) == NULL ){
//      printf("\nエラーファイルオープン失敗！！！");
      return;
   }

   
   memset(now, 0 ,sizeof(now));      
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

   fprintf(fp, "%s:",prgname);        
   fprintf(fp, "%s",msg);
   if(code)
	   fprintf(fp,":err=%d\n",code);
   else
	   fprintf(fp, "\n");

   fclose(fp);
}


void Calctime( char *buf,char *date, int plus ,short tani)
{
	time_t 	ltime=0;
	struct	tm	nowtime,*newtime;
    char dummy[20];

	memset( &nowtime,0,sizeof(nowtime));


    memset( dummy, 0 , sizeof(dummy));
	memcpy(dummy,date,4);
	nowtime.tm_year = atol(dummy)-1900;


	memcpy(dummy,&date[4],2);
	dummy[2] = 0;
	nowtime.tm_mon = atol(dummy)-1;


	memcpy(dummy,&date[6],2);
	dummy[2] = 0;
	nowtime.tm_mday=atol(dummy);


	memcpy(dummy,&date[8],2);
	dummy[2] = 0;
	nowtime.tm_hour=atol(dummy);


	memcpy(dummy,&date[10],2);
	dummy[2] = 0;
	nowtime.tm_min=atol(dummy);


	memcpy(dummy,&date[12],2);
	dummy[2] = 0;
	nowtime.tm_sec=atol(dummy);

	// tm->time_t
	ltime = mktime(&nowtime);


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

void CvDateTime(char *src,char *dest,int mode)
{

	if( mode == CVMODE_TIME ){
		if( strlen(src) < 6 ) return;
		memcpy(dest,src,2);
		memcpy(dest+2,":",1);
		memcpy(dest+3,src+2,2);
		memcpy(dest+5,":",1);
		memcpy(dest+6,src+4,2);
	}


	if( mode == CVMODE_DATE ){
		if( strlen(src) < 8 ) return;
		memcpy(dest,src,4);
		memcpy(dest+4,"/",1);
		memcpy(dest+5,src+4,2);
		memcpy(dest+7,"/",1);
		memcpy(dest+8,src+6,2);
	}
	
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

int get_yobi(char *date)
{
	int y,m,d,yobi;
	long count;
	char buf[20];
	int     MTBL[12]=  { 0,31,59,90,120,151,181,212,243,273,304,334 };

	if( strlen(date) < 8 )return -1;
	
	memcpy(buf,date,4);buf[4]=0;
	y=atoi(buf);
	memcpy(buf,date+4,2);buf[2]=0;
	m=atoi(buf);
	memcpy(buf,date+6,2);buf[2]=0;
	d=atoi(buf);

    count=  (y-1)*365L;           
    count+= (y/4L - y/100L + y/400L + MTBL[m-1] + d);
    if( m<3 && UruuCheck(y) )  count--;

	yobi= count%7L;
	return yobi;
}

int UruuCheck(int year)
{
	if (year%400L==0L)  return(1);
	if(year%100L==0L)  return(0);
	if(year%4L==0L)    return(1);
	return(0);
}

int check_date(char* date)
{
	char buf[50];

	
	if( strlen(date) < 8 ) return -1;

	int y,m,d;
	memcpy(buf,date,4);buf[4]=0;
	y = atoi(buf);
	memcpy(buf,date+4,2);buf[2]=0;
	m = atoi(buf);
	memcpy(buf,date+6,2);buf[2]=0;
	d = atoi(buf);

	if( m < 1 || m > 12 ) return -2;

	int nUruu = 0;
	int nMaxDay;

	
	nUruu = UruuCheck(y);
	
	switch(m) {
		case 2:nMaxDay = 28+nUruu;
			break;
		case 4:
		case 6:
		case 9:
		case 11: nMaxDay = 30; break;
		default: nMaxDay = 31; break;
	}
	if( d > nMaxDay ) return -3;

	return 0;
}
int GetDateStock(char* datenew,char* date,char* time,int adddays)
{
	char buf[50];
	int nUruu = 0;
	int nMaxDay,nMonth;

	if( strlen(date) < 8 ) return -1;

	int y,m,d,dNew;
	memcpy(buf,date,4);buf[4]=0;
	y = atoi(buf);
	memcpy(buf,date+4,2);buf[2]=0;
	m = atoi(buf);
	memcpy(buf,date+6,2);buf[2]=0;
	d = atoi(buf);
	
	if( m < 1 || m > 12 ) return -2;
	
	if(atoi(time)>140000)
	{
		nUruu = UruuCheck(y);
	
		switch(m) {
			case 2:nMaxDay = 28+nUruu;
				break;
			case 4:
			case 6:
			case 9:
			case 11: nMaxDay = 30; break;
			default: nMaxDay = 31; break;
		}
		dNew = d + adddays;
		/*if(d == nMaxDay)
		{
			if(m == 12)
			{
				y = y+1;
				m = 1;
				d = 1;
			}
			else
			{
				m = m+1;
				d = 1;
			}
		}
		else
		{
			d = d+1;
		}*/
		if(dNew > nMaxDay)
		{
			if(m == 12)
			{
				y = y+1;
				m = 1;
				d = dNew - adddays;
			}
			else
			{
				m = m+1;
				d = dNew - adddays;
			}
		}
		else
			d = d + adddays;
		//
		//if(d<10)
		memset( datenew, 0 , sizeof(datenew));

		sprintf(datenew,"%04d%02d%02d", y,
                      m,d);
	}
	else
	{
		memset( datenew, 0 , sizeof(datenew));
		memcpy(datenew,date,8);
	}
	return 0;

}
////06-02-2012
int check_fifo(char* lotnew,char* lotold,int a,int b)
{
	int y1,m1,d1;
	int y2,m2,d2;
	char buf[10];
	char buf2[10];

	if( strlen(lotnew) < 5 || strlen(lotold) < 5 ) return -1;

	//lotnew
	memcpy(buf,lotnew,1);buf[1]=0;
	y1 = atoi(buf);
	memcpy(buf,lotnew+1,1);buf[1]=0;
	if(strcmp(buf,"X")==0) 
		m1 = 10; 
	else
	{
		if(strcmp(buf,"Y")==0) 
			m1 = 11;
		else
		{
			if(strcmp(buf,"Z")==0) 
				m1 = 12;
			else
				m1 = atoi(buf);
		}
	}
	memcpy(buf,lotnew+3,2);buf[2]=0;
	d1 = atoi(buf);

	//lotold
	memcpy(buf2,lotold,1);buf2[1]=0;
	y2 = atoi(buf2);
	memcpy(buf2,lotold+1,1);buf2[1]=0;
	
	/*switch( buf2) {
		case "X":m1 = 10; break;
		case "Y":m1 = 11; break;
		case "X":m1 = 12; break;
		default: m1 = atoi(buf); break;
	}*/
	if(strcmp(buf2,"X")==0) 
		m2 = 10; 
	else
	{
		if(strcmp(buf2,"Y")==0) 
			m2 = 11;
		else
		{
			if(strcmp(buf2,"Z")==0) 
				m2 = 12;
			else
				m2 = atoi(buf2);
		}
	}
	memcpy(buf2,lotold+3,2);buf2[2]=0;
	d2 = atoi(buf2);
	
	
	if(y2>y1) 
		return -1;
	else
	{
		if(y2==0 && y1==9)
		{
			if(y1>y2)
				return -1;
		}
		else
		{
			if(y2==y1)
			{
				if(m2>m1)
					return -1;
				else
				{
					if(m2==m1)
					{
						if(d2>d1)
							return -1;
					}
				}
			}
		}
	}
	/*if(m2>m1)
		return -1;
	else
	{
		if(d2>d1) return -1;
	}*/

	return 0;
}
int check_fifo(char* lotnew,char* lotold)
{
	int y1,m1,d1;
	int y2,m2,d2;
	char buf[10];
	char buf1[10];
	char buf2[10];
	char datet[15],ty[4];

	memset( datet, 0 , sizeof(datet));
	memset( ty, 0 , sizeof(ty));

	get_nowtime(datet,8,0);
	memcpy(ty,datet,3);

	if( strlen(lotnew) < 5 || strlen(lotold) < 5 ) return -1;
	
	//lotnew
	memcpy(buf1,lotnew,1);buf1[1]=0;
	memset( buf, 0 , sizeof(buf));
	sprintf(buf,"%s%s",ty,buf1);

	y1 = atoi(buf);
	memcpy(buf1,lotnew+1,1);buf1[1]=0;
	if(strcmp(buf1,"X")==0) 
		m1 = 10; 
	else
	{
		if(strcmp(buf1,"Y")==0) 
			m1 = 11;
		else
		{
			if(strcmp(buf1,"Z")==0) 
				m1 = 12;
			else
				m1 = atoi(buf1);
		}
	}
	memcpy(buf1,lotnew+3,2);buf1[2]=0;
	d1 = atoi(buf1);

	//lotold
	memcpy(buf2,lotold,1);buf2[1]=0;
	memset( buf, 0 , sizeof(buf));
	sprintf(buf,"%s%s",ty,buf2);

	y2 = atoi(buf);
	memcpy(buf2,lotold+1,1);buf2[1]=0;
	
	if(strcmp(buf2,"X")==0) 
		m2 = 10; 
	else
	{
		if(strcmp(buf2,"Y")==0) 
			m2 = 11;
		else
		{
			if(strcmp(buf2,"Z")==0) 
				m2 = 12;
			else
				m2 = atoi(buf2);
		}
	}
	memcpy(buf2,lotold+3,2);buf2[2]=0;
	d2 = atoi(buf2);
	
	
	if(y2>y1) 
		return -1;
	else
	{
		if(y2==0 && y1==9)
		{
			if(y1>y2)
				return -1;
		}
		else
		{
			if(y2==y1)
			{
				if(m2>m1)
					return -1;
				else
				{
					if(m2==m1)
					{
						if(d2>d1)
							return -1;
					}
				}
			}
		}
	}
	return 0;
}

int pos(char *s,char *scantim)
{
     char *p = strstr(s,scantim);
     if(p!=NULL)
        return strlen(s) - strlen(p);
     else
        return -1;
}
int changeabctonum(char* chuoi)
{
	char lotcur1[24]=" ABCDEFGHKLMNPQRSTUVXYZ";
	char str[256],str1[256];
	int i,j,num;

	num=0;
	memset( str, 0 , sizeof(str));
	for(i=0;i<strlen(chuoi);i++)
	{
		for(j=0;j<strlen(lotcur1);j++)
		{
			if(lotcur1[j]==chuoi[i])
			{
				memset( str1, 0 , sizeof(str1));
				sprintf(str1,"%d",j);
				sprintf(str,"%s%s",str,str1);
				break;
			}
		}
	}
	num = atoi(str);
	
	return num;
}