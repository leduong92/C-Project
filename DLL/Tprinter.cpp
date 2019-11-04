//=======================================================================================================================================
//		FileName : TPrinter.cpp
//===========================================================================

#include <stdafx.h>
#include	<stdio.h>
#include	"Tprinter.h"

#include 	"..\Tool\QR2DLL.H"


#include <cmath>	//Draw text Rotation --


// ASCII
static char BarcodeConvTbl[256]={
	//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//0
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//1
		38,41,38,38,40,43,38,38,38,38,39,42,38,36,37,38,	//2
		 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,38,38,38,38,38,	//3
		38,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,	//4
		25,26,27,28,29,30,31,32,33,34,35,38,38,38,38,38,	//5
		38,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,	//6
		25,26,27,28,29,30,31,32,33,34,35,38,38,38,38,38,	//7
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//8
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//9
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//A
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//B
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//C
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//D
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//E
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38};	//F



// BarCode(Code-39) BASR(5)+SPACE(4)
static char BarcodeTbl[44][11]={
						{1,0,0,1,0,0,0,0,1,0,'1'},	// 00 
						{0,0,1,1,0,0,0,0,1,0,'2'},	// 01 
						{1,0,1,1,0,0,0,0,0,0,'3'},	// 02 
						{0,0,0,1,1,0,0,0,1,0,'4'},	// 03 
						{1,0,0,1,1,0,0,0,0,0,'5'},	// 04 
						{0,0,1,1,1,0,0,0,0,0,'6'},	// 05
						{0,0,0,1,0,0,1,0,1,0,'7'},	// 06
						{1,0,0,1,0,0,1,0,0,0,'8'},	// 07
						{0,0,1,1,0,0,1,0,0,0,'9'},	// 08
						{0,0,0,1,1,0,1,0,0,0,'0'},	// 09
						{1,0,0,0,0,1,0,0,1,0,'A'},	// 10
						{0,0,1,0,0,1,0,0,1,0,'B'},	// 11
						{1,0,1,0,0,1,0,0,0,0,'C'},	// 12
						{0,0,0,0,1,1,0,0,1,0,'D'},	// 13
						{1,0,0,0,1,1,0,0,0,0,'E'},	// 14
						{0,0,1,0,1,1,0,0,0,0,'F'},	// 15
						{0,0,0,0,0,1,1,0,1,0,'G'},	// 16
						{1,0,0,0,0,1,1,0,0,0,'H'},	// 17
						{0,0,1,0,0,1,1,0,0,0,'I'},	// 18
						{0,0,0,0,1,1,1,0,0,0,'J'},	// 19
						{1,0,0,0,0,0,0,1,1,0,'K'},	// 20
						{0,0,1,0,0,0,0,1,1,0,'L'},	// 21
						{1,0,1,0,0,0,0,1,0,0,'M'},	// 22
						{0,0,0,0,1,0,0,1,1,0,'N'},	// 23
						{1,0,0,0,1,0,0,1,0,0,'O'},	// 24
						{0,0,1,0,1,0,0,1,0,0,'P'},	// 25
						{0,0,0,0,0,0,1,1,1,0,'Q'},	// 26
						{1,0,0,0,0,0,1,1,0,0,'R'},	// 27
						{0,0,1,0,0,0,1,1,0,0,'S'},	// 28
						{0,0,0,0,1,0,1,1,0,0,'T'},	// 29
						{1,1,0,0,0,0,0,0,1,0,'U'},	// 30
						{0,1,1,0,0,0,0,0,1,0,'V'},	// 31
						{1,1,1,0,0,0,0,0,0,0,'W'},	// 32
						{0,1,0,0,1,0,0,0,1,0,'X'},	// 33
						{1,1,0,0,1,0,0,0,0,0,'Y'},	// 34
						{0,1,1,0,1,0,0,0,0,0,'Z'},	// 35
						{0,1,0,0,0,0,1,0,1,0,'_'},	// 36
						{1,1,0,0,0,0,1,0,0,0,'.'},	// 37
						{0,1,1,0,0,0,1,0,0,0,' '},	// 38
						{0,1,0,0,1,0,1,0,0,0,'*'},	// 39
						{0,1,0,1,0,1,0,0,0,0,'$'},	// 40
						{0,1,0,1,0,0,0,1,0,0,'!'},	// 41
						{0,1,0,0,0,1,0,1,0,0,'+'},	// 42
						{0,0,0,1,0,1,0,1,0,0,'%'}};	// 43

static char BarcodeConvTbl128A[256]={
	//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,	//0
		80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,	//1
		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,	//2
		16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,	//3
		32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,	//4
		48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,	//5
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//6
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	//7
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//8
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//9
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//A
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//B
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//C
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//D
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//E
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38};	//F

static char BarcodeConvTbl128B[256]={
	//   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
		64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,	//0
		80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,	//1
		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,	//2
		16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,	//3
		32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,	//4
		48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,	//5
		64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,	//6
		80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,	//7
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//8
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//9
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//A
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//B
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//C
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//D
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,	//E
		38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38};	//F

// 
static char BarcodeTbl128[106][6]={
						{2,1,2,2,2,2},	// 00 
						{2,2,2,1,2,2},	// 01
						{2,2,2,2,2,1},
						{1,2,1,2,2,3},
						{1,2,1,3,2,2},
						{1,3,1,2,2,2},
						{1,2,2,2,1,3},
						{1,2,2,3,1,2},
						{1,3,2,2,1,2},
						{2,2,1,2,1,3},
						{2,2,1,3,1,2},
						{2,3,1,2,1,2},	// 01 
						{1,1,2,2,3,2},
						{1,2,2,1,3,2},
						{1,2,2,2,3,1},
						{1,1,3,2,2,2},
						{1,2,3,1,2,2},
						{1,2,3,2,2,1},
						{2,2,3,2,1,1},
						{2,2,1,1,3,2},
						{2,2,1,2,3,1},
						{2,1,3,2,1,2},	// 01 
						{2,2,3,1,1,2},
						{3,1,2,1,3,1},
						{3,1,1,2,2,2},
						{3,2,1,1,2,2},
						{3,2,1,2,2,1},
						{3,1,2,2,1,2},
						{3,2,2,1,1,2},
						{3,2,2,2,1,1},
						{2,1,2,1,2,3},
						{2,1,2,3,2,1},	// 01 
						{2,3,2,1,2,1},
						{1,1,1,3,2,3},
						{1,3,1,1,2,3},
						{1,3,1,3,2,1},
						{1,1,2,3,1,3},
						{1,3,2,1,1,3},
						{1,3,2,3,1,1},
						{2,1,1,3,1,3},
						{2,3,1,1,1,3},
						{2,3,1,3,1,1},	// 01 
						{1,1,2,1,3,3},
						{1,1,2,3,3,1},
						{1,3,2,1,3,1},
						{1,1,3,1,2,3},
						{1,1,3,3,2,1},
						{1,3,3,1,2,1},
						{3,1,3,1,2,1},
						{2,1,1,3,3,1},
						{2,3,1,1,3,1},
						{2,1,3,1,1,3},	// 01 
						{2,1,3,3,1,1},
						{2,1,3,1,3,1},
						{3,1,1,1,2,3},
						{3,1,1,3,2,1},
						{3,3,1,1,2,1},
						{3,1,2,1,1,3},
						{3,1,2,3,1,1},
						{3,3,2,1,1,1},
						{3,1,4,1,1,1},
						{2,2,1,4,1,1},	// 01 
						{4,3,1,1,1,1},
						{1,1,1,2,2,4},
						{1,1,1,4,2,2},
						{1,2,1,1,2,4},
						{1,2,1,4,2,1},
						{1,4,1,1,2,2},
						{1,4,1,2,2,1},
						{1,1,2,2,1,4},
						{1,1,2,4,1,2},
						{1,2,2,1,1,4},	// 01 
						{1,2,2,4,1,1},
						{1,4,2,1,1,2},
						{1,4,2,2,1,1},
						{2,4,1,2,1,1},
						{2,2,1,1,1,4},
						{4,1,3,1,1,1},
						{2,4,1,1,1,2},
						{1,3,4,1,1,1},
						{1,1,1,2,4,2},
						{1,2,1,1,4,2},	// 01 
						{1,2,1,2,4,1},
						{1,1,4,2,1,2},
						{1,2,4,1,1,2},
						{1,2,4,2,1,1},
						{4,1,1,2,1,2},
						{4,2,1,1,1,2},
						{4,2,1,2,1,1},
						{2,1,2,1,4,1},
						{2,1,4,1,2,1},
						{4,1,2,1,2,1},	// 01
						{1,1,1,1,4,3},
						{1,1,1,3,4,1},
						{1,3,1,1,4,1},
						{1,1,4,1,1,3},
						{1,1,4,3,1,1},
						{4,1,1,1,1,3},
						{4,1,1,3,1,1},
						{1,1,3,1,4,1},
						{1,1,4,1,3,1},
						{3,1,1,1,4,1},	// 01 
						{4,1,1,1,3,1},
						{2,1,1,4,1,2},		// 103=STOP-A
						{2,1,1,2,1,4},
						{2,1,1,2,3,2}};



static char BarcodeTblStop[7]={2,3,3,1,1,1,2};



static char *FontName[]={{"Default"},					// 0
						{"ＭＳ 明朝"},					// 1
						{"ＭＳ ゴシック"},				// 2
						{"Century Gothic"},				// 3
						{"Courier New"},				// 4
						{"Courier New Bold"},			// 5
						{"Courier New Bold Italic"},	// 6
						{"Courier New Italic"},			// 7				
						{"Arial"},						// 8
						{"Impact"},						// 9
						{"FreesiaUPC"},					//10						
						{".VnTime"}						//11
						};					



/*
static char *FontName[]={{"Default"},					// 0
						{"ＭＳ 明朝"},					// 1
						{"ＭＳ ゴシック"},				// 2
						{"Century Gothic"},				// 3
						{"Courier New"},				// 4
						{"Courier New Bold"},			// 5
						{"Courier New Bold Italic"},	// 6
						{"Courier New Italic"},			// 7				
						{"Arial"},						// 8
						{"Impact"},						// 9
						{"FreesiaUPC"}					//10						
						};					
*/


//================================================================================================
//	TPrinter::TPrinter(void)
//================================================================================================
TPrinter::TPrinter(void)
{
	flgIPrinter = 0;		
}

//================================================================================================
// TPrinter::TPrinter(int *initStat)
//================================================================================================
TPrinter::TPrinter(int *initStat)
{
	int stat;
	char csDrv[256], csDev[256], csPort[256];

	flgIPrinter = 0;		


	stat = GetDefaultPrinter(csDrv,csDev,csPort);


	if( stat ){
		*initStat = stat;
		return;
	}

	
	hdcPrint = CreateDC(  csDrv,  csDev,  csPort,NULL);
	//hdcPrint = CreateDC(lpPrtDriver,lpPrtDevice,lpPrtPort, NULL);
	if( hdcPrint == NULL ){
		*initStat = -2;
		return ;
	}


	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	


	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );


	IPrinter();
	*initStat = 0;
}

//================================================================================================
//	TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat)
//================================================================================================
TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat)
{
	flgIPrinter = 0;	
	int stat;

	hdcPrint = CreateDC(  theDrv,  theDev,  thePort,NULL);
	//hdcPrint = CreateDC(lpPrtDriver,lpPrtDevice,lpPrtPort, NULL);
	if( hdcPrint == NULL ){
		*initStat = -2;
		return ;
	}

	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );
	
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );


	IPrinter();
	*initStat = 0;
}


//==========================================================================================================
//==========================================================================================================
TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth, int *initStat)
{
	flgIPrinter = 0;		

	*initStat = InitPageSetting(theDrv, theDev, thePort, theLength,theWidth);

	return;
}


//================================================================================================
//================================================================================================
TPrinter::~TPrinter()
{
	if( flgIPrinter != 0 ){
		EndDoc(hdcPrint);		
		DeleteDC(hdcPrint);	
	}
}


//============================================================================
//	Return: int ( 0:OK or 1:NG )
//============================================================================
int TPrinter::IPrinter( void )
{
	DocStart();
	flgIPrinter = 1;						

	return 0;
}

//===========================================================================================================
//	int	TPrinter::InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth)
//===========================================================================================================
int	TPrinter::InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth)
{

	int		stat;				
	HANDLE	phPrinter;			
	DWORD	dwDmSize;		
	DEVMODE	*devNew;		
	char *pszIn;
	DWORD	newdmVal;

	if( OpenPrinter(  theDev , &phPrinter , NULL ) == FALSE ){
		return(-1);
	}

	dwDmSize = DocumentProperties(	NULL ,			// handle to window that displays dialog box
									phPrinter,			// handle to printer object
									 theDrv,				// pointer to device name (Driver name)
								 	NULL ,				// pointer to modified device mode structure
									NULL ,				// pointer to original device mode structure
 									0 );				// mode flg ( 0:Get DEVMODE structure size )

 
	// DEVMODE
	pszIn = new char[dwDmSize+1];
	devNew = (DEVMODE *)pszIn;

	// DEVMODE
	stat = DocumentProperties(	NULL,phPrinter, theDrv,devNew ,NULL ,	DM_OUT_BUFFER );	// mode flg 
	if( stat != IDOK ){
		ClosePrinter( phPrinter );
		delete( pszIn );
		return(-2);
	}

	ClosePrinter( phPrinter );

	devNew -> dmPaperSize = DMPAPER_USER;		
	devNew -> dmPaperLength = theLength;			
	devNew -> dmPaperWidth = theWidth;			

	newdmVal = devNew -> dmFields;
	devNew -> dmFields = newdmVal|DM_PAPERSIZE|DM_PAPERLENGTH|DM_PAPERWIDTH;


	hdcPrint = CreateDC(  theDrv,  theDev,  thePort, devNew );
	if( hdcPrint == NULL ){
		delete( pszIn );
		return(-3);
	}

	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );

	flgIPrinter = 1;						
	IPrinter();

	return(0);
}

//================================================================================================
// TPrinter::TPrinter(int *initStat)
//================================================================================================
TPrinter::TPrinter(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,int *initStat,short pageSize,short orient)
{
	flgIPrinter = 0;

	*initStat = InitPageSettingEx(theDrv, theDev, thePort, pageSize, orient);
	return;
}

//===========================================================================================================
//	int	TPrinter::InitPageSetting(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short theLength, short theWidth)
//===========================================================================================================
int	TPrinter::InitPageSettingEx(LPSTR theDrv,LPSTR theDev,LPSTR thePort ,short thePage ,short theOrient)
{

	int		stat;				
	HANDLE	phPrinter;		
	DWORD	dwDmSize;		
	DEVMODE	*devNew;		
	char *pszIn;
	DWORD	newdmVal;

	if( OpenPrinter(  theDev , &phPrinter , NULL ) == FALSE ){
		return(-1);
	}

	dwDmSize = DocumentProperties(	NULL ,			// handle to window that displays dialog box
									phPrinter,			// handle to printer object
									 theDrv,				// pointer to device name (Driver name)
								 	NULL ,				// pointer to modified device mode structure
									NULL ,				// pointer to original device mode structure
 									0 );				// mode flg ( 0:Get DEVMODE structure size )

 
	// DEVMODE
	pszIn = new char[dwDmSize+1];
	devNew = (DEVMODE *)pszIn;

	// DEVMODE
	stat = DocumentProperties(	NULL,phPrinter, theDrv,devNew ,NULL ,	DM_OUT_BUFFER );	// mode flg 
	if( stat != IDOK ){
		ClosePrinter( phPrinter );
		delete( pszIn );
		return(-2);
	}

	ClosePrinter( phPrinter );

	devNew -> dmPaperSize = thePage;		
	devNew -> dmCopies = 1;			
	devNew -> dmOrientation = theOrient;			
	devNew ->dmColor=DMCOLOR_COLOR;


	//thuy
	/*devNew -> dmPaperSize = thePage;//256 ;
	devNew -> dmPaperWidth = 1000;//2160 ;
	devNew -> dmPaperLength = 1000;//2800 ;
	*/
	//----------------

	// dmFields
	newdmVal = devNew -> dmFields;
	devNew -> dmFields = newdmVal|DM_PAPERSIZE|DM_COPIES|DM_ORIENTATION|DM_COLOR;

	hdcPrint = CreateDC(  theDrv,  theDev,  thePort, devNew );
	if( hdcPrint == NULL ){
		delete( pszIn );
		return(-3);
	}

	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	
	
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){	
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );

	flgIPrinter = 1;					

	IPrinter();

	return(0);
}


//============================================================================
int TPrinter::Print(void)
{
	char	prMes[] = "印刷フォームPrint();が設定(オーバーライド)されていません！！";
	
	if( flgIPrinter == 0 ){
		return 1;	
	}
	return 0;
}

//============================================================================
//	Input:	
//	Return:
//============================================================================
int	TPrinter::PageStart( void )
{
	int stat;

	if( flgIPrinter == 0 ) return 1;	

	stat = StartPage( hdcPrint );


	if( stat != 0 ){
		return GetLastError();
	}
	return 0;

}

//============================================================================
//	Input:
//	Return:
//============================================================================
int	TPrinter::PageEnd( void )
{
	int stat;

	if( flgIPrinter == 0 ) return 1;		
	stat = EndPage( hdcPrint );

	if( stat != 0 ){
		return GetLastError();
	}
	return 0;
}


//============================================================================
//	int TPrinter::DocStart( void )
//	public
//============================================================================
int TPrinter::DocStart( void )
{
	DOCINFO docinfo;			

	
	docinfo.cbSize = sizeof(docinfo);	
	docinfo.lpszDocName=  "NOK Printer";	
	docinfo.lpszOutput =  (LPSTR)NULL;	
	docinfo.lpszDatatype = NULL;		
	docinfo.fwType = 0;					
	StartDoc(hdcPrint,&docinfo);		


	return 0;
}

//============================================================================
//	int	TPrinter::DocEnd( void )
//	public
//============================================================================
int	TPrinter::DocEnd( void )
{
	int stat;

	if( flgIPrinter == 0 ) return 1;	
		stat = EndDoc( hdcPrint );

	if( stat != 0 ){
		return GetLastError();
	}
	return 0;
}

//============================================================================
//		Input:	int x;			
//				int y;			
//				int nFont;		
//				int nBold:		
//				int nPoint1;	
//				int nPoint2;	
//				LPSTR Message;	
//		Return:
//============================================================================
void TPrinter::PrMessage( int x, int y, int nFont, int nBold,int nPoint1, int nPoint2, LPSTR Message )
{
	HFONT	holdf,hnewf;
	HDC		hdc;
	int		sx,sy;

	TEXTMETRIC	textmetric;

	hdc = hdcPrint;


	sx = ConvertMmToPixcel( x , 0 );
	sy = ConvertMmToPixcel( y , 1 );

	GetTextMetrics(hdc,&textmetric);

	if( nPoint2 == 0 ) nPoint2 = nPoint1;

	nPoint1 = ConvertMmToPixcel( nPoint1 , 0 );
	nPoint2 = ConvertMmToPixcel( nPoint2 , 1 );

	if( nFont != 0 && nFont <= 8 ){	
		hnewf = CreateFont(	nPoint2,nPoint1,	
							0,					// Escapement
							0,					// Orientation
							nBold,				// Weight()
							0,					// Ital()
							0,					// Underline()
							0,					// StrikeThru()
							ANSI_CHARSET,	//DEFAULT_CHARSET,	//ANSI_CHARSET, //SHIFTJIS_CHARSET,	// Charset()	//Thuy------
							OUT_DEFAULT_PRECIS,	// Precision()
							CLIP_DEFAULT_PRECIS,// ClipPrecision
							DEFAULT_QUALITY,	// Quality
							DEFAULT_PITCH | FF_DONTCARE,	// Pitch
//							DEFAULT_PITCH | FF_ROMAN,	// Pitch
							 FontName[nFont] );	// FontName
		holdf = (HFONT) SelectObject( hdc , hnewf );	

		TextOut(hdc,sx,sy, Message,strlen(Message));

		hnewf = (HFONT) SelectObject( hdc , holdf );
		DeleteObject( hnewf );

	}
	else if( nFont == 11 ){		/* 11: ".VnTime" - Thuy - print fraction label*/
		hnewf = CreateFont(	nPoint2,nPoint1,	
							0,					// Escapement
							0,					// Orientation
							nBold,				// Weight()
							0,					// Ital()
							0,					// Underline()
							0,					// StrikeThru()
							ANSI_CHARSET,	//DEFAULT_CHARSET,	//ANSI_CHARSET, //SHIFTJIS_CHARSET,	// Charset()	//Thuy------
							OUT_DEFAULT_PRECIS,	// Precision()
							CLIP_DEFAULT_PRECIS,// ClipPrecision
							DEFAULT_QUALITY,	// Quality
							DEFAULT_PITCH | FF_DONTCARE,	// Pitch
//							DEFAULT_PITCH | FF_ROMAN,	// Pitch
							 FontName[nFont] );	// FontName
		holdf = (HFONT) SelectObject( hdc , hnewf );	

		TextOut(hdc,sx,sy, Message,strlen(Message));

		hnewf = (HFONT) SelectObject( hdc , holdf );
		DeleteObject( hnewf );

	}
	else{
		TextOut(hdc,sx,sy, Message,strlen(Message));
	}

}

void TPrinter::PrMessage( int x, int y, int nFont,int nPoint1, int nPoint2, LPSTR Message )
{
	//Thuy------------
	//PrMessage( x,  y, nFont, FW_SEMIBOLD, nPoint1, nPoint2, Message );
	PrMessage( x,  y, nFont, FW_MEDIUM, nPoint1, nPoint2, Message );
}


//============================================================================

//============================================================================
void TPrinter::CenterText( int x, int y, int endX,int nFont, int nBold,int nPoint1, int nPoint2, LPSTR Message )
{
	HFONT	holdf,hnewf;
	HDC		hdc;
	int		sx,sy,ex;
	RECT rect;

	TEXTMETRIC	textmetric;

	hdc = hdcPrint;


	sx = ConvertMmToPixcel( x , 0 );	
	ex = ConvertMmToPixcel( endX , 0 );	
	sy = ConvertMmToPixcel( y , 1 );	

	GetTextMetrics(hdc,&textmetric);

	if( nPoint2 == 0 ) nPoint2 = nPoint1;

	nPoint1 = ConvertMmToPixcel( nPoint1 , 0 );
	nPoint2 = ConvertMmToPixcel( nPoint2 , 1 );

	SetRect(&rect	,x
					,sy
					,ex
					,sy+nPoint2);

	if( nFont != 0 && nFont <= 8 ){	
		hnewf = CreateFont(	nPoint2,nPoint1,	
							0,					// Escapement
							0,					// Orientation
							nBold,				// Weight
							0,					// Ital
							0,					// Underline
							0,					// StrikeThru
							ANSI_CHARSET, //SHIFTJIS_CHARSET,	// Charset
							OUT_DEFAULT_PRECIS,	// Precision
							CLIP_DEFAULT_PRECIS,// ClipPrecision
							DEFAULT_QUALITY,	// Quality
							DEFAULT_PITCH | FF_DONTCARE,	// Pitch
//							DEFAULT_PITCH | FF_ROMAN,	// Pitch
							 FontName[nFont] );	// FontName
		holdf =(HFONT) SelectObject( hdc , hnewf );	

//		TextOut(hdc,sx,sy,Message,strlen(Message));
		DrawText(hdc,  Message,-1,&rect,DT_CENTER|DT_SINGLELINE);

		hnewf =(HFONT) SelectObject( hdc , holdf );
		DeleteObject( hnewf );

	}else{
		DrawText(hdc,  Message,-1,&rect,DT_CENTER|DT_SINGLELINE);
	}

}


void TPrinter::PrMsgEx( int x, int y, char* font, DWORD charset,int nBold,int nPoint1, int nPoint2, LPSTR Message )
{
	HFONT	holdf,hnewf;
	HDC		hdc;
	int		sx,sy;


	hdc = hdcPrint;


	sx = ConvertMmToPixcel( x , 0 );	// 印字Ｘ座標
	sy = ConvertMmToPixcel( y , 1 );	// 印字Ｙ座標


	if( nPoint2 == 0 ) nPoint2 = nPoint1;

	nPoint1 = ConvertMmToPixcel( nPoint1 , 0 );
	nPoint2 = ConvertMmToPixcel( nPoint2 , 1 );

		hnewf = CreateFont(	nPoint2,nPoint1,				// フォントサイズ
							0,					// Escapement
							0,					// Orientation
							nBold,				// Weight(フォントの太さ)
							0,					// Ital(イタリック体)
							0,					// Underline(下線)
							0,					// StrikeThru(打ち消し線)
							charset,	// Charset(文字セット)
							OUT_DEFAULT_PRECIS,	// Precision(出力精度)
							CLIP_DEFAULT_PRECIS,// ClipPrecision(クリッピング精度)
							DEFAULT_QUALITY,	// Quality(フォントの出力品質)
							DEFAULT_PITCH | FF_DONTCARE,	// Pitch(フォントのピッチとファミリ)
//							DEFAULT_PITCH | FF_ROMAN,	// Pitch(フォントのピッチとファミリ)
							font );	// FontName
		holdf = (HFONT)SelectObject( hdc , hnewf );	// フォントの切り替え

		TextOut(hdc,sx,sy,Message,strlen(Message));

		hnewf = (HFONT)SelectObject( hdc , holdf );
		DeleteObject( hnewf );


}

//=======================================================================================================================================
// void TPrinter::PrMessage( int x, int y, int nFont, int nBold,int nPoint1, int nPoint2, COLORREF colFont,COLORREF colBack,LPSTR Message )
//=======================================================================================================================================
void TPrinter::PrMessage( int x, int y, int nFont, int nBold,int nPoint1, int nPoint2, COLORREF colFont,COLORREF colBack,LPSTR Message )
{
	COLORREF oldFontCol,oldBackCol;
	
	oldFontCol = SetTextColor( hdcPrint,colFont);


	oldBackCol = SetBkColor(hdcPrint,colBack );

	PrMessage( x, y, nFont, nBold,nPoint1, nPoint2, Message );


	SetTextColor( hdcPrint,oldFontCol);


	SetBkColor(hdcPrint,oldBackCol );
}

//============================================================================
void TPrinter::PrBCode( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message )
{

	HDC	hdc;
	char	mojiBarcode[MAX_BARCODE+1];		
	char	dspBarcode[MAX_BARCODE];		
	int i;

	int		mojisuu;					
	int		sx,ox1,ox2;					
	int		sy,ey;						
	int		l;							
	double		bigBar,smallBar;		

	
	RECT rect;

	
	hdc = hdcPrint;

	
	memset( dspBarcode , 0 , sizeof( dspBarcode ));
	mojisuu = strlen(Message);
	if( mojisuu > MAX_BARCODE-2 )mojisuu = MAX_BARCODE-2;	
	
	memset( mojiBarcode , 0 , sizeof( mojiBarcode ));
	mojiBarcode[0]='*';
	memcpy( &mojiBarcode[1] , Message , mojisuu );
	mojiBarcode[mojisuu+1] = '*';


	for( i = 0 ; i < mojisuu+2 ; i++ ){
		dspBarcode[i] = BarcodeConvTbl[mojiBarcode[i]];
	}


	//	Rectangle( hdc , x-5 , y-5 , x+nWidth*((mojisuu)*13-1)+5 , y+nHeight+5);

//	sx = x;
	sx = ConvertMmToPixcel( x , 0 );	
	sy = ConvertMmToPixcel( y , 1 );	
	ey = ConvertMmToPixcel((y+nHeight),1);	


	smallBar = ConvertMmToPixcel(nWidth,0);
	bigBar = 2*smallBar;
//	bigBar = 2.5*smallBar;

	if( smallBar == 1 ){
	
		ox1 = 0;					
		for( i = 0 ; i < mojisuu+2 ; i++ ){	
			for( l = 0 ; l < 10 ; l++ ){	
				
				ox2 = ox1 + (int)(BarcodeTbl[dspBarcode[i]][l] ? bigBar : smallBar);
				if( l % 2 == 0 ){		
					SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
					FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
				}
				ox1 = ox2;		
			}
		}
	}else{
	
		ox1 = 0;						
		for( i = 0 ; i < mojisuu+2 ; i++ ){	
			for( l = 0 ; l < 10 ; l++ ){	
				
				ox2 = ox1 + (int)(BarcodeTbl[dspBarcode[i]][l] ? bigBar : smallBar);
				if( l % 2 == 0 ){			
					SetRect( &rect,sx+ox1,sy,sx+ox2-1,ey );
					FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
				}
				ox1 = ox2;		
			}
		}
	}

	
	if(strON)	PrBCodeString( x , y+nHeight+1 , sx+ox2 , mojiBarcode );
}

void TPrinter::PrBCode( int x, int y, int nWidth, int nHeight,LPSTR Message )
{
	PrBCode( x, y, nWidth, nHeight,1 , Message );
}

//================================================================================================
//	void TPrinter::PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code)
//================================================================================================
void TPrinter::PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code)
{
	HDC	hdc;
	char mojiBarcode[MAX_BARCODE+1];		
	char dspBarcode[MAX_BARCODE];		// BarcodeTbl[no]
	int i,j,mojisuu;
	int	sx,ox1,ox2;					
	int	sy,ey;						
	int	chkD;						// code128
	int barWidth;
	
	if(code < 0 || code > 2 ) code=1;	
	
	RECT rect;
	
	hdc = hdcPrint;
	
	memset( dspBarcode , 0 , sizeof( dspBarcode ));
	memset( mojiBarcode , 0 , sizeof( mojiBarcode ));
	mojisuu = strlen(Message);
	if( mojisuu > MAX_BARCODE)mojisuu = MAX_BARCODE;
	memcpy( mojiBarcode, Message , mojisuu );

	
	for( i = 0 ; i < mojisuu ; i++ ){
		switch(code){
			case 2:		// code A
				if(mojiBarcode[i] > 0 )
					dspBarcode[i] = mojiBarcode[i];
				break;

			case 0:
				dspBarcode[i] = BarcodeConvTbl128A[mojiBarcode[i]];
				break;
			default:
				dspBarcode[i] = BarcodeConvTbl128B[mojiBarcode[i]];
				break;
		}
	}

	sx = ConvertMmToPixcel( x , 0 );	
	sy = ConvertMmToPixcel( y , 1 );	
	ey = ConvertMmToPixcel((y+nHeight),1);	
	barWidth =  ConvertMmToPixcel( nWidth , 0 );	

	ox1 = 0;					
	
	for( i=0; i<6; i++ ){ 
		if( i % 2 == 0 ){	
			ox2=ox1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
		}
		else{		
			ox2=ox1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
		}
		ox1=ox2;
	}
	
	for( i=0; i<mojisuu; i++ ){

		for( j=0; j<6; j++ ){ 
			if( j % 2 == 0 ){
				ox2=ox1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
				SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
				FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
			}
			else{			
				ox2=ox1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
			}
			ox1=ox2;
		}
	}

	
	chkD=0;
	for( i=0; i<mojisuu; i++ ){
		chkD+=( dspBarcode[i]*(i+1) );
	}
	chkD=(chkD+STARTCODE_128+code)%103;
	for( j=0; j<6; j++ ){ 
		if( j % 2 == 0 ){	
			ox2=ox1+(BarcodeTbl128[chkD][j]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
		}
		else{			
			ox2=ox1+(BarcodeTbl128[chkD][j]*barWidth);
		}
		ox1=ox2;
	}
	
	
	for( j=0; j<7; j++ ){ 
		if( j % 2 == 0 ){	
			ox2=ox1+(BarcodeTblStop[j]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
		}
		else{			
			ox2=ox1+(BarcodeTblStop[j]*barWidth);
		}
		ox1=ox2;
	}


	if(strON)	
		// display text at Top of barcode 128
		//PrBCodeString( x+150 , y-20 , sx+ox2 , mojiBarcode );		
		PrBCodeString( x , y+nHeight+1 , sx+ox2 , mojiBarcode );



	


}

//============================================================================
//============================================================================
void	TPrinter::PrBCodeString( int x, int y, int endX, LPSTR Message )
{
	int i;
	int mojisuu;
	char	moji[4];
	int	 endXmm,txtSpace;

	memset( moji , 0 , sizeof( moji ));
	mojisuu = strlen( Message );

	
	endXmm = endX * 10 * PrinterSizeX / PrinterResX;
	txtSpace = (endXmm-x-12) / (mojisuu-1);	

	//Thuy
	txtSpace=12;

	for( i = 0 ; i < mojisuu ; i++ ){
		moji[0] = Message[i];

		//Thuy - display text for barcode 128 -
		//PrMessage( x+i*txtSpace , y , 1 ,FW_NORMAL, 12 , 30 , moji );
		PrMessage( x+i*txtSpace , y , 3 ,FW_THIN, 7 , 18 , moji );
	}
}



//Thuy-- Box non stick
//================================================================================================
void TPrinter::PrBCode128_NonStick( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code)
{
	HDC	hdc;
	char mojiBarcode[MAX_BARCODE+1];		
	char dspBarcode[MAX_BARCODE];		// BarcodeTbl[no]
	int i,j,mojisuu;
	int	sx,ox1,ox2;					
	int	sy,ey;						
	int	chkD;						// code128
	int barWidth;
	
	if(code < 0 || code > 2 ) code=1;	
	
	RECT rect;
	
	hdc = hdcPrint;
	
	memset( dspBarcode , 0 , sizeof( dspBarcode ));
	memset( mojiBarcode , 0 , sizeof( mojiBarcode ));
	mojisuu = strlen(Message);
	if( mojisuu > MAX_BARCODE)mojisuu = MAX_BARCODE;
	memcpy( mojiBarcode, Message , mojisuu );

	
	for( i = 0 ; i < mojisuu ; i++ ){
		switch(code){
			case 2:		// code A
				if(mojiBarcode[i] > 0 )
					dspBarcode[i] = mojiBarcode[i];
				break;

			case 0:
				dspBarcode[i] = BarcodeConvTbl128A[mojiBarcode[i]];
				break;
			default:
				dspBarcode[i] = BarcodeConvTbl128B[mojiBarcode[i]];
				break;
		}
	}

	sx = ConvertMmToPixcel( x , 0 );	
	sy = ConvertMmToPixcel( y , 1 );	
	ey = ConvertMmToPixcel((y+nHeight),1);	
	barWidth =  ConvertMmToPixcel( nWidth , 0 );	

	ox1 = 0;					
	
	for( i=0; i<6; i++ ){ 
		if( i % 2 == 0 ){	
			ox2=ox1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
		}
		else{		
			ox2=ox1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
		}
		ox1=ox2;
	}
	
	for( i=0; i<mojisuu; i++ ){

		for( j=0; j<6; j++ ){ 
			if( j % 2 == 0 ){
				ox2=ox1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
				SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
				FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
			}
			else{			
				ox2=ox1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
			}
			ox1=ox2;
		}
	}

	
	chkD=0;
	for( i=0; i<mojisuu; i++ ){
		chkD+=( dspBarcode[i]*(i+1) );
	}
	chkD=(chkD+STARTCODE_128+code)%103;
	for( j=0; j<6; j++ ){ 
		if( j % 2 == 0 ){	
			ox2=ox1+(BarcodeTbl128[chkD][j]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
		}
		else{			
			ox2=ox1+(BarcodeTbl128[chkD][j]*barWidth);
		}
		ox1=ox2;
	}
	
	
	for( j=0; j<7; j++ ){ 
		if( j % 2 == 0 ){	
			ox2=ox1+(BarcodeTblStop[j]*barWidth);
			SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
		}
		else{			
			ox2=ox1+(BarcodeTblStop[j]*barWidth);
		}
		ox1=ox2;
	}


	if(strON)	
		// display text at Top of barcode 128
		PrBCodeString( x+150 , y-20 , sx+ox2 , mojiBarcode );		
		//PrBCodeString( x , y+nHeight+1 , sx+ox2 , mojiBarcode );

}





//================================================================================================
// Thuy -----------
//	void TPrinter::PrBCode128( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code)
//================================================================================================
void TPrinter::PrBCode128_Rotation( int x, int y, int nWidth, int nHeight,int strON ,LPSTR Message ,int code)
{
	HDC	hdc;
	char mojiBarcode[MAX_BARCODE+1];		
	char dspBarcode[MAX_BARCODE];		// BarcodeTbl[no]
	int i,j,mojisuu;
	int	sx,ox1,ox2;			int ex;		//thuy		
	int	sy,ey;			int oy1,oy2;	//thuy
	int	chkD;						// code128
	int barWidth;
	
	if(code < 0 || code > 2 ) code=1;	
	
	RECT rect;
	
	hdc = hdcPrint;
	
	memset( dspBarcode , 0 , sizeof( dspBarcode ));
	memset( mojiBarcode , 0 , sizeof( mojiBarcode ));
	mojisuu = strlen(Message);
	if( mojisuu > MAX_BARCODE)mojisuu = MAX_BARCODE;
	memcpy( mojiBarcode, Message , mojisuu );

	
	for( i = 0 ; i < mojisuu ; i++ ){
		switch(code){
			case 2:		// code A
				if(mojiBarcode[i] > 0 )
					dspBarcode[i] = mojiBarcode[i];
				break;

			case 0:
				dspBarcode[i] = BarcodeConvTbl128A[mojiBarcode[i]];
				break;
			default:
				dspBarcode[i] = BarcodeConvTbl128B[mojiBarcode[i]];
				break;
		}
	}

	sx = ConvertMmToPixcel( x , 0 );	
	sy = ConvertMmToPixcel( y , 1 );	
	ey = ConvertMmToPixcel((y+nHeight),1);	
	barWidth =  ConvertMmToPixcel( nWidth , 0 );	

	//thuy
	ex = ConvertMmToPixcel((x+nHeight),1);	




	//thuy 
	oy1=0;	ox2=0;

	ox1 = 0;					
	
	for( i=0; i<6; i++ ){ 
		if( i % 2 == 0 ){	
			//thuy
			//ox2=ox1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
			oy2=oy1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);

			//SetRect( &rect,sx+ox1,sy,sx+ox2,ey );	//ROOT
			//SetRect( &rect,sx,sy+ox1,sx,ey+ox2 );	
			SetRect( &rect, sx, sy+oy1, ex, sy+oy2 );

			FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
		}
		else{		
			//ox2=ox1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
			oy2=oy1+(BarcodeTbl128[STARTCODE_128+code][i]*barWidth);
		}
		//ox1=ox2;
		oy1=oy2;
	}
	
	for( i=0; i<mojisuu; i++ ){

		for( j=0; j<6; j++ ){ 
			if( j % 2 == 0 ){
				//ox2=ox1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
				oy2=oy1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);

				//SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
				//SetRect( &rect,sx,sy+ox1,sx,ey+ox2 );
				SetRect( &rect, sx, sy+oy1, ex, sy+oy2 );
				
				FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
			}
			else{			
				//ox2=ox1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
				oy2=oy1+(BarcodeTbl128[dspBarcode[i]][j]*barWidth);
			}
			//ox1=ox2;
			oy1=oy2;
		}
	}

	
	chkD=0;
	for( i=0; i<mojisuu; i++ ){
		chkD+=( dspBarcode[i]*(i+1) );
	}
	chkD=(chkD+STARTCODE_128+code)%103;
	for( j=0; j<6; j++ ){ 
		if( j % 2 == 0 ){	
			//ox2=ox1+(BarcodeTbl128[chkD][j]*barWidth);
			oy2=oy1+(BarcodeTbl128[chkD][j]*barWidth);

			//SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			//SetRect( &rect,sx,sy+ox1,sx,ey+ox2 );
			SetRect( &rect, sx, sy+oy1, ex, sy+oy2 );

			FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
		}
		else{			
			//ox2=ox1+(BarcodeTbl128[chkD][j]*barWidth);
			oy2=oy1+(BarcodeTbl128[chkD][j]*barWidth);
		}
		//ox1=ox2;
		oy1=oy2;
	}
	
	
	for( j=0; j<7; j++ ){ 
		if( j % 2 == 0 ){	
			//ox2=ox1+(BarcodeTblStop[j]*barWidth);
			oy2=oy1+(BarcodeTblStop[j]*barWidth);

			//SetRect( &rect,sx+ox1,sy,sx+ox2,ey );
			//SetRect( &rect,sx,sy+ox1,sx,ey+ox2 );
			SetRect( &rect, sx, sy+oy1, ex, sy+oy2 );
			
			FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
		}
		else{			
			//ox2=ox1+(BarcodeTblStop[j]*barWidth);
			oy2=oy1+(BarcodeTblStop[j]*barWidth);
		}
		//ox1=ox2;
		oy1=oy2;
	}


	// Hien thi text
	if(strON)	
		// display text at Top of barcode 128
		//PrBCodeString( x , y+nHeight+1 , sx+ox2 , mojiBarcode );
		//PrBCodeString( x+140 , y-20 , sx+ox2 , mojiBarcode );		
		PrBCodeString_Rotation( x+nHeight+2 , y , sy+oy2, mojiBarcode );		
		


		





	//Thuy
	/*
	CDC * pDC = CDC::FromHandle(hdc);
	//CDC* pDC=(CDC*)hdc;
	DrawRotatedText(pDC, mojiBarcode, &rect, 180, 0);*/

}


//============================================================================
//============================================================================
void	TPrinter::PrBCodeString_Rotation( int x, int y, int endX, LPSTR Message )
{
	int i;
	int mojisuu;
	char	moji[4];
	int	 endXmm,txtSpace;
	int sy;

	//thuy
	//int endYmm;


	memset( moji , 0 , sizeof( moji ));
	mojisuu = strlen( Message );

	
	//endXmm = endX * 10 * PrinterSizeX / PrinterResX;
	endXmm = endX * 10 * PrinterSizeY / PrinterResY;

	//txtSpace = (endXmm-x-12) / (mojisuu-1);	
	txtSpace = (endXmm-y-12) / (mojisuu-1);
	


	//Thuy
	txtSpace=15;

	sy=y+310;

	for( i = 0 ; i < mojisuu ; i++ ){
		moji[0] = Message[i];

		//Thuy - display text for barcode 128 -
		//PrMessage( x+i*txtSpace , y , 1 ,FW_NORMAL, 12 , 30 , moji );
		//PrMessage( x+i*txtSpace , y , 3 ,FW_THIN, 7 , 18 , moji );
		//PrMessage( x , y+i*txtSpace , 1 ,FW_THIN, 7 , 18 , moji );
		//PrMessage( x , y+i*txtSpace, 1 ,FW_NORMAL, 7 , 18 , moji );



		//PrMessage_Rotation( x+10, y+i*txtSpace, 1 ,FW_NORMAL,7 , 18 , moji , 0);	//rotation 90
		PrMessage_Rotation( x, sy-i*txtSpace , 1 ,FW_NORMAL,9 , 18 , moji , 900);	//rotation 90


		
		
	}
}


//==================================================================================================
// PrMessage - text rotation 180
//==================================================================================================
void TPrinter::PrMessage_Rotation( int x, int y, int nFont, int nBold,int nPoint1, int nPoint2, LPSTR Message, int Esca)
{
	HFONT	holdf,hnewf;
	HDC		hdc;
	int		sx,sy;

	TEXTMETRIC	textmetric;

	hdc = hdcPrint;


	sx = ConvertMmToPixcel( x , 0 );
	sy = ConvertMmToPixcel( y , 1 );

	GetTextMetrics(hdc,&textmetric);

	if( nPoint2 == 0 ) nPoint2 = nPoint1;

	nPoint1 = ConvertMmToPixcel( nPoint1 , 0 );
	nPoint2 = ConvertMmToPixcel( nPoint2 , 1 );

	if( nFont != 0 && nFont <= 8 ){	
		hnewf = CreateFont(	nPoint2,nPoint1,	
							Esca,					// Escapement
							0,					// Orientation
							nBold,				// Weight()
							0,					// Ital()
							0,					// Underline()
							0,					// StrikeThru()
							ANSI_CHARSET,	//DEFAULT_CHARSET,	//ANSI_CHARSET, //SHIFTJIS_CHARSET,	// Charset()	//Thuy------
							OUT_DEFAULT_PRECIS,	// Precision()
							CLIP_DEFAULT_PRECIS,// ClipPrecision
							DEFAULT_QUALITY,	// Quality
							DEFAULT_PITCH | FF_DONTCARE,	// Pitch
//							DEFAULT_PITCH | FF_ROMAN,	// Pitch
							 FontName[nFont] );	// FontName
		holdf = (HFONT) SelectObject( hdc , hnewf );	

		TextOut(hdc,sx,sy, Message,strlen(Message));

		hnewf = (HFONT) SelectObject( hdc , holdf );
		DeleteObject( hnewf );

	}else{
		TextOut(hdc,sx,sy, Message,strlen(Message));
	}

}






//==================================================================================================
//	void TPrinter::DrawCircle( int x,int y,int r, BOOL paint)
//==================================================================================================
void TPrinter::DrawCircle( int x,int y,int r, BOOL paint)
{
	int px,py,rx,ry;

	px = ConvertMmToPixcel( x , 0 );	
	rx = ConvertMmToPixcel( r , 0 );	

	py = ConvertMmToPixcel( y , 1 );	
	ry = ConvertMmToPixcel( r , 1 );	

	HPEN	hOldpen,hPen;				// Handle of pen
	HBRUSH hbrush,hold;

	hPen   = CreatePen( PS_SOLID ,3, RGB(0,0,0));
	hOldpen   = (HPEN) SelectObject(hdcPrint,hPen);
	hbrush = CreateSolidBrush(RGB(0,0,0));
	hold   = (HBRUSH) SelectObject(hdcPrint,hbrush);

	
	if( paint ){
		Ellipse( hdcPrint , px-rx ,py-ry,px+rx,py+ry);
	}
	
	else{
		Arc( hdcPrint , px-rx ,py-ry,px+rx,py+ry,px,py-ry,px,py-ry);
	}
	hPen   = (HPEN) SelectObject(hdcPrint,hOldpen);
	hbrush   =  (HBRUSH) SelectObject(hdcPrint,hold); //(HPEN) SelectObject(hdcPrint,hold);
	DeleteObject( hPen );
	DeleteObject( hbrush );
}

//============================================================================
//============================================================================
void TPrinter::DrawCircle( int x, int y ,int rx ,int ry , int LineType ,int width)
{
	HDC	hdc;
	HPEN	hPen,hOldPen;				// Handle of pen
	int theStyle,theWidth;
	int sx,sy;
	int ex,ey;

	
	hdc = hdcPrint;

	sx = ConvertMmToPixcel( x-rx , 0 );	
	sy = ConvertMmToPixcel( y-ry , 1 );	
	ex = ConvertMmToPixcel( x+rx , 0 );	
	ey = ConvertMmToPixcel( y+ry , 1 );	

	switch( LineType ){
		case 1:						
			theStyle = PS_DASH;
			theWidth = 1;
			break;
		case 2:						
			theStyle = PS_DOT;
			theWidth = 1;
			break;

		default:
			theStyle = PS_SOLID;
			theWidth = width;
			break;
	}

	hPen   = CreatePen( theStyle ,width, RGB(0,0,0));
	hOldPen   = (HPEN)SelectObject(hdc,hPen);

	
	Arc( hdc , sx , sy , ex , ey, sx+(ex-sx)/2 , sy+(ey-sy)/2 , sx+(ex-sx)/2, sy+(ey-sy)/2);

	SelectObject(hdc,hOldPen);

	DeleteObject( hPen );

}

//============================================================================
//============================================================================
void TPrinter::DrawLine( int sx,int sy,int ex, int ey,int LineType ,int LineWidth)
{
	HDC	hdc;
	HPEN	hOldpen,hPen;				// Handle of pen
	int theStyle,theWidth;
	int x1,x2,y1,y2;

	switch( LineType ){
		case 1:					
			theStyle = PS_DASH;
			theWidth = 1;
			break;
		case 2:					
			theStyle = PS_DOT;
			theWidth = 1;
			break;

		default:
			theStyle = PS_SOLID;
			theWidth = LineWidth;
			break;
	}

	
	hdc = hdcPrint;
	x1 = ConvertMmToPixcel( sx , 0 );
	x2 = ConvertMmToPixcel( ex , 0 );
	y1 = ConvertMmToPixcel( sy , 1 );
	y2 = ConvertMmToPixcel(ey,1);	

	hPen   = CreatePen( theStyle ,theWidth, RGB(0,0,0));
	hOldpen   = (HPEN)SelectObject(hdc,hPen);

	
	MoveToEx( hdc , x1 , y1 , NULL );
	LineTo(   hdc , x2 , y2 );


	hPen   = (HPEN)SelectObject(hdc,hOldpen);
	DeleteObject( hPen );
}



//============================================================================
// Thuy - Draw Box_Non_Stick Form Label --

void TPrinter::DrawBoxNonStickForm( )
{

HPEN hpen, hpenOld;
HBRUSH hbrush, hbrushOld;
//PAINTSTRUCT pntS;

//HWND hwnd = hdcPrint;                            // Retrieve the frame handle
HDC hdc = hdcPrint ;//BeginPaint(hdcPrint, &pntS);                        // Prepare the specified window for painting
hpen = CreatePen(PS_SOLID, 7, RGB(0, 0, 0));

// Create a red brush.
hbrush = CreateSolidBrush(RGB(255, 255, 255));

// Select the new pen and brush, and then draw.
hpenOld = (HPEN) SelectObject(hdc, hpen);
hbrushOld = (HBRUSH) SelectObject(hdc, hbrush);



int x,y,w,h, cxClient , cyClient,cwClient,chClient;
int x1, y1, x2, y2 , cx1, cy1, cx2, cy2 ;

x = 39; //36;
y = 580 + 185; //580 + 178;
w = 1305;//3000;
h = 1312;//1307;


cxClient = ConvertMmToPixcel( x , 0 );
cyClient = ConvertMmToPixcel( y , 0 );
cwClient = ConvertMmToPixcel( w , 0 );
chClient = ConvertMmToPixcel( h , 0 );


//Draw rectangle form
Rectangle(hdc, cxClient,cyClient,cwClient ,chClient);	//(x,y,w,h)

//Display text form
	//PrMessage(x+5 ,y+5	, 8,FW_NORMAL,10,27,"CUSTOMER");		//CUSTOMER
	PrMessage(x+5 ,y+7	, 8,FW_NORMAL,10,27,"CUSTOMER");		//CUSTOMER
	PrMessage(x+5 ,y+90	, 8,FW_NORMAL,10,27,"PART NO.");		//PART NO.
	PrMessage(x+5 ,y+235	, 8,FW_NORMAL,10,27,"QUANTITY");	//QUANTITY
	PrMessage(x+400 ,y+240	, 8,FW_NORMAL,10,27,"ITEM");		//ITEM
	PrMessage(w-210 ,y+240	, 8,FW_NORMAL,10,27,"LINE NO.");	//LINE NO.
	PrMessage(x+400 ,y+350	, 8,FW_NORMAL,10,27,"JOB NO.");		//JOB NO.
	//PrMessage(x+635 ,y+350	, 8,FW_NORMAL,10,27,"LOT NO.");		//LOT NO.
	PrMessage(x+805 ,y+350	, 8,FW_NORMAL,10,27,"LOT NO.");		//LOT NO.
	PrMessage(w-210 ,y+320	, 8,FW_NORMAL,10,27,"LOCATION");	//LOCATION
	PrMessage(w-210 ,y+390	, 8,FW_NORMAL,10,27,"INSPECTION");	//INSPECTION

	PrMessage(x+5 ,y+552	, 4,FW_BOLD,30,62,"NOK");	//NOK

//----------------



//Draw Line1 - horizontal Line
x1 = x ; y1 = y+85 ; x2 = w ; y2 = y1 ;
cx1 = ConvertMmToPixcel( x1 , 0 );
cy1 = ConvertMmToPixcel( y1 , 0 );
cx2 = ConvertMmToPixcel( x2 , 0 );
cy2 = ConvertMmToPixcel( y2 , 0 );

MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
LineTo(hdc, cx2, cy2);			//to point = X2 * Y2


//Draw Line1 - Verizontal Line
x1 = w-185 ; y1 = y ; x2 = x1 ; y2 = y1+85 ;
cx1 = ConvertMmToPixcel( x1 , 0 );
cy1 = ConvertMmToPixcel( y1 , 0 );
cx2 = ConvertMmToPixcel( x2 , 0 );
cy2 = ConvertMmToPixcel( y2 , 0 );

MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
LineTo(hdc, cx2, cy2);			//to point = X2 * Y2


//Draw Line2 - horizontal Line
x1 = x ; y1 = y+230 ; x2 = w ; y2 = y1 ;
cx1 = ConvertMmToPixcel( x1 , 0 );
cy1 = ConvertMmToPixcel( y1 , 0 );
cx2 = ConvertMmToPixcel( x2 , 0 );
cy2 = ConvertMmToPixcel( y2 , 0 );

MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
LineTo(hdc, cx2, cy2);			//to point = X2 * Y2


//Draw Line3 - horizontal Line
x1 = x ; y1 = y+305 ; x2 = x +395 ; y2 = y1 ;
cx1 = ConvertMmToPixcel( x1 , 0 );
cy1 = ConvertMmToPixcel( y1 , 0 );
cx2 = ConvertMmToPixcel( x2 , 0 );
cy2 = ConvertMmToPixcel( y2 , 0 );

MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
LineTo(hdc, cx2, cy2);			//to point = X2 * Y2


	//Draw Line3 - Verizontal Line
	x1 = x+395 ; y1 = y+230 ; x2 = x1 ; y2 = y1+315 ;
	cx1 = ConvertMmToPixcel( x1 , 0 );
	cy1 = ConvertMmToPixcel( y1 , 0 );
	cx2 = ConvertMmToPixcel( x2 , 0 );
	cy2 = ConvertMmToPixcel( y2 , 0 );

	MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
	LineTo(hdc, cx2, cy2);			//to point = X2 * Y2

	//Draw Line3-1 - Verizontal Line
	x1 = w -220 ; y1 = y+230 ; x2 = x1 ; y2 = y1+315 ;
	cx1 = ConvertMmToPixcel( x1 , 0 );
	cy1 = ConvertMmToPixcel( y1 , 0 );
	cx2 = ConvertMmToPixcel( x2 , 0 );
	cy2 = ConvertMmToPixcel( y2 , 0 );

	MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
	LineTo(hdc, cx2, cy2);			//to point = X2 * Y2


	//Draw Line3-1 - horizontal Line
	//x1 = w -280 ; y1 = y+305 ; x2 = w ; y2 = y1 ;
	x1 = w -220 ; y1 = y+305 ; x2 = w ; y2 = y1 ;
	cx1 = ConvertMmToPixcel( x1 , 0 );
	cy1 = ConvertMmToPixcel( y1 , 0 );
	cx2 = ConvertMmToPixcel( x2 , 0 );
	cy2 = ConvertMmToPixcel( y2 , 0 );

	MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
	LineTo(hdc, cx2, cy2);			//to point = X2 * Y2


//Draw Line4 - horizontal Line
x1 = x+395 ; y1 = y+340 ; x2 = w-220 ; y2 = y1 ;
cx1 = ConvertMmToPixcel( x1 , 0 );
cy1 = ConvertMmToPixcel( y1 , 0 );
cx2 = ConvertMmToPixcel( x2 , 0 );
cy2 = ConvertMmToPixcel( y2 , 0 );

MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
LineTo(hdc, cx2, cy2);			//to point = X2 * Y2


//Draw Line5 - horizontal Line
x1 = x ; y1 = y+380 ; x2 = x +395 ; y2 = y1 ;
cx1 = ConvertMmToPixcel( x1 , 0 );
cy1 = ConvertMmToPixcel( y1 , 0 );
cx2 = ConvertMmToPixcel( x2 , 0 );
cy2 = ConvertMmToPixcel( y2 , 0 );

MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
LineTo(hdc, cx2, cy2);			//to point = X2 * Y2

//Draw Line5-1 - horizontal Line
x1 = w-220 ; y1 = y+380 ; x2 = w ; y2 = y1 ;
cx1 = ConvertMmToPixcel( x1 , 0 );
cy1 = ConvertMmToPixcel( y1 , 0 );
cx2 = ConvertMmToPixcel( x2 , 0 );
cy2 = ConvertMmToPixcel( y2 , 0 );

MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
LineTo(hdc, cx2, cy2);			//to point = X2 * Y2

//Draw Line6 - horizontal Line
x1 = x ; y1 = y+470 ; x2 = x +395 ; y2 = y1 ;
cx1 = ConvertMmToPixcel( x1 , 0 );
cy1 = ConvertMmToPixcel( y1 , 0 );
cx2 = ConvertMmToPixcel( x2 , 0 );
cy2 = ConvertMmToPixcel( y2 , 0 );

MoveToEx(hdc, cx1, cy1, NULL);	//from point = X1 * Y1
LineTo(hdc, cx2, cy2);			//to point = X2 * Y2


// Do not forget to clean up.
SelectObject(hdc, hpenOld);
DeleteObject(hpen);
SelectObject(hdc, hbrushOld);
DeleteObject(hbrush);



}

//============================================================================




//==================================================================================================
//	void TPrinter::DrawRect( int sx, int sy, int ex, int ey,COLORREF theColor)
//==================================================================================================
void TPrinter::DrawRect( int sx, int sy, int ex, int ey,COLORREF theColor)
{

	RECT rect;

	HBRUSH hBrush,hOldBrush;

	SetRect( &rect,ConvertMmToPixcel(sx,0),ConvertMmToPixcel(sy,1),
				   ConvertMmToPixcel(ex,0),ConvertMmToPixcel(ey,1 ));

	
	hBrush = CreateSolidBrush( theColor);

	hOldBrush =(HBRUSH) SelectObject( hdcPrint , hBrush );
	FillRect( hdcPrint,&rect,hBrush );

	// GDI
	hBrush =(HBRUSH) SelectObject( hdcPrint , hOldBrush );
	DeleteObject( hBrush );
}

//============================================================================
//============================================================================
int	TPrinter::ConvertMmToPixcel( int mm , int xy )
{
	double val;

	if( xy == 0 ){			
		val =  (double)mm * PrinterResX/(PrinterSizeX*10);
//val=mm;
	}else{					
		val =  (double)mm * PrinterResY/(PrinterSizeY*10);
	}


	return (int)val;
}

int TPrinter::GetDefaultPrinter(char *theDrv, char *theDev, char *thePort)
{
	static PAGESETUPDLG psd;	
	DEVNAMES *dvn; 				
	int size;
	char lpDev[256];
	char lpDrv[256];
	char lpPort[256];

	// PAGESETUP
	memset( &psd,0, sizeof(psd));
	psd.Flags = PSD_RETURNDEFAULT;	
 	psd.hDevNames=NULL;
	psd.hDevMode=NULL;
	psd.lStructSize=sizeof(PAGESETUPDLG);
	psd.hwndOwner=NULL;

	
	if(!PageSetupDlg( &psd )) return(-1);

	// DEVNAME
	dvn=(DEVNAMES*)GlobalLock(psd.hDevNames);
	size=dvn->wDeviceOffset;


	memset(lpDev,0,sizeof(lpDev) );
	strncpy( lpDev,(char*)dvn+dvn->wDeviceOffset,sizeof(lpDev) );

	memset(lpDrv,0,sizeof(lpDrv) );
	strncpy( lpDrv,(char*)dvn+dvn->wDriverOffset, sizeof(lpDrv) );

	memset(lpPort,0,sizeof(lpPort) );
	strncpy( lpPort,(char*)dvn+dvn->wOutputOffset,sizeof(lpPort) );


	GlobalFree(psd.hDevMode);
	GlobalUnlock(psd.hDevNames);
	GlobalFree(psd.hDevNames);
	GlobalFree(psd.hPageSetupTemplate);
	GlobalUnlock(dvn);
	GlobalFree(dvn);

	
	strncpy(theDrv,lpDrv,256);
	strncpy(theDev,lpDev,256);
	strncpy(thePort,lpPort,256);
	return(0);

}

//Print QR code
#define MAX_TEXT_LENGTH 7089
#define MAX_VER 7
static const short QR_VER_TBL[]={
	0,		// Ver0:
	34,		// Ver1:34byte
	63,		// Ver2:63byte
	91,		// Ver3:91byte
	149,	// Ver4:149byte
	202,	// Ver5:202yte
	255,	// Ver6:255yte
};

int TPrinter::PrintQRCode(char *txt,long lngErrorLevel,long lngPrintQlty ,int x ,int y)
{
	char strDummy[MAX_TEXT_LENGTH];		
    long lngLoopDummy;		
    long lngReturn;			
    long lngSell;			
    long lngOriginalLength;	
 	long lngResult[MAX_TEXT_LENGTH*4+20];	
	long lngOriginal[MAX_TEXT_LENGTH];		
	long lngX,lngY;		
    double dblPitch;	
	int ver;
	long lngQRversion=0;

	int   sx,sy;

	//int lvlUp;
    
	memset(strDummy,0,sizeof(strDummy));
	memcpy(strDummy,txt,strlen(txt)>sizeof(strDummy) ? sizeof(strDummy) : strlen(txt));


	if( lngPrintQlty < 0 || lngPrintQlty > 3) 	lngPrintQlty = 2;


	if(lngErrorLevel < 0 || lngErrorLevel > 3)	lngErrorLevel = 2;	// L(7%),M(15%),Q(25%),H(30%)

    

	memset(lngResult,0,sizeof(lngResult));


    lngOriginalLength = strlen(strDummy);
	//QR
	if(lngOriginalLength < 0 || lngOriginalLength > MAX_TEXT_LENGTH) return -1;	//Length 
    
	
	for(ver=0;ver < MAX_VER;ver++){
		if(lngOriginalLength < QR_VER_TBL[ver]) break;
	}
	if(!ver) return -2;		
	lngQRversion = ver;
    
    lngSell = lngQRversion * 4 + 17;

	
	memset(lngOriginal,0,sizeof(lngOriginal));
    
	
	for( lngLoopDummy = 0;lngLoopDummy < lngOriginalLength; lngLoopDummy++){
		memcpy(&lngOriginal[lngLoopDummy],&strDummy[lngLoopDummy],1);	
//		lngOriginal[lngLoopDummy] = (unsigned char)strDummy[lngLoopDummy];
	}

	// Ver1:34byte
	// Ver2:63byte
	// Ver3:91byte
	// Ver4:149byte
	// Ver5:202yte
	// Ver6:255yte
    
	while(lngQRversion <= 40){

		lngReturn = QRmodel2(&lngOriginal[0], lngOriginalLength, lngQRversion, lngErrorLevel, &lngResult[0]);
	
		if(!lngReturn) break;

		lngQRversion += 1;
		lngSell = lngQRversion * 4 + 17;
		memset(lngResult,0,sizeof(lngResult));
		if( lngQRversion==40 ) return -3;
	}



/*
	lngReturn = QRmodel2(&lngOriginal[0], lngOriginalLength, lngQRversion, lngErrorLevel, &lngResult[0]);
	//ダメだったらVerをあげてもう一度トライ
	if(lngReturn){
		lngQRversion += 1;
	    lngSell = lngQRversion * 4 + 17;
		memset(lngResult,0,sizeof(lngResult));
		lngReturn = QRmodel2(&lngOriginal[0], lngOriginalLength, lngQRversion, lngErrorLevel, &lngResult[0]);
		if(lngReturn){
			lngQRversion += 1;
			lngSell = lngQRversion * 4 + 17;
			memset(lngResult,0,sizeof(lngResult));
			lngReturn = QRmodel2(&lngOriginal[0], lngOriginalLength, lngQRversion, lngErrorLevel, &lngResult[0]);
			if(lngReturn){
				lngQRversion += 1;
				lngSell = lngQRversion * 4 + 17;
				memset(lngResult,0,sizeof(lngResult));
				lngReturn = QRmodel2(&lngOriginal[0], lngOriginalLength, lngQRversion, lngErrorLevel, &lngResult[0]);
				if(lngReturn)	return -3;			//DLLバージョン不適合エラー
			}
		}
	}
*/
	
	switch(lngPrintQlty){
		case 0:	dblPitch = 0.16 * 56.7;	break;
		case 1:	dblPitch = 0.19 * 56.7;	break;
		case 2:	dblPitch = 0.25 * 56.7;	break;
		case 3:	dblPitch = 0.32 * 56.7;	break;		
		default:	dblPitch = 0.25 * 56.7;	break;	
	}

	if(lngReturn) return -4;

	HDC	hdc;
	
	RECT rect;
	
	hdc = hdcPrint;

	
	sx = ConvertMmToPixcel(x,0);
	sy = ConvertMmToPixcel(y,1);

	
	if(lngReturn==0){
		for( lngY = 0;lngY <= lngSell-1;lngY++){
			for( lngX = 0 ; lngX <= lngSell - 1; lngX++){
				if( lngResult[lngX * lngSell + lngY] >= 1 ){
					
					SetRect( &rect,	sx + (int)(lngX * dblPitch),			sy + (int)(lngY * dblPitch),
									sx + (int)(lngX * dblPitch+dblPitch),	sy + (int)(lngY * dblPitch+dblPitch) );

					FillRect( hdc,&rect,(HBRUSH)GetStockObject(BLACK_BRUSH) );
				}
			}
		}				
	}
	
	return 0;
}

//=============================================================================
//	int	TPrinter::PrintQRCode(	char *txt,long lngQRversion,long lngErrorLevel, long lngPrintQlty, int	 x,	int  y)
//=============================================================================
int TPrinter::PrintQRCode(	char *txt,long lngQRversion,long lngErrorLevel, long lngPrintQlty, int	 x,	int  y)
{
	return (PrintQRCode(txt, lngErrorLevel,lngPrintQlty, x, y));
}


//Dong dau Inspection-QCOK
void TPrinter::PrStump( int x, int y ,int theForm , LPSTR Day ,LPSTR Inspector)
{
	HDC	hdc;
	HPEN	hPen,hOldPen;				// Handle of pen
	int sx,sy;
	int ex,ey;

	
	hdc = hdcPrint;

		sx = ConvertMmToPixcel( x , 0 );
		sy = ConvertMmToPixcel( y , 1 );
		ex = ConvertMmToPixcel( x+200 , 0 );	
		ey = ConvertMmToPixcel( y+140 , 1 );	

	if(theForm == 0 ){
		PrMessage( x+30,y+10, 8, 17, 34, " 合　格 ");	
		PrMessage( x+12,y+55, 8, 16, 32, Day );		
		PrMessage( x+45,y+95, 8, 17, 34, Inspector );		
	}
	else{
		//thuy -- PrMessage(left,top,font,width,height, text) ---

		//PrMessage( x+27,y+11, 8, 17, 40, "QCOK" );//huy2
		PrMessage( x+55,y+17, 8,FW_BOLD, 13, 40, "QCOK" );//huy2
		//PrMessage( x+30,y+18, 1, 20, 30, "QCOK" );					
		PrMessage( x+10,y+58, 8, FW_SEMIBOLD, 14, 42, Day );	//huy2
		//PrMessage( x+10,y+55, 1, 16, 32, Day );
		PrMessage( x+27,y+100, 8, FW_BOLD, 13, 36, Inspector );	//huy2
		//PrMessage( x+25,y+95, 1, 14, 28, Inspector );
 

	}

	hPen   = CreatePen( PS_SOLID , 3 , RGB(0,0,0));
	hOldPen = (HPEN)SelectObject(hdc,hPen);

	

	/*
	//Draw one cycle
	if( sx % 2 ) sx--;
	if( sy % 2 ) sy--;
	if( ex % 2 ) ex++;
	if( ey % 2 ) ey++;
	Arc( hdc , sx , sy , ex , ey, sx+(ex-sx)/2 , sy+(ey-sy)/2 , sx+(ex-sx)/2, sy+(ey-sy)/2);
	*/	


	/*
	//Draw one horizontal Line
	MoveToEx( hdc , sx+3 , sy+(ey-sy)/2-15 , NULL);
	LineTo( hdc , ex-3 , sy+(ey-sy)/2-15);
	
	//Draw one horizontal Line
	MoveToEx( hdc , sx+3 , sy+(ey-sy)/2+15 , NULL);
	LineTo( hdc , ex-3 , sy+(ey-sy)/2+15);
	*/

	SelectObject(hdc,hOldPen);

	DeleteObject( hPen );


 }


void TPrinter::PrStump_Rotation(int x, int y ,int theForm , LPSTR Day ,LPSTR Inspector)
{
	HDC	hdc;
	HPEN	hPen,hOldPen;				// Handle of pen
	int sx,sy;
	int ex,ey;

	
	hdc = hdcPrint;

		sx = ConvertMmToPixcel( x , 0 );
		sy = ConvertMmToPixcel( y , 1 );
		ex = ConvertMmToPixcel( x+200 , 0 );	
		ey = ConvertMmToPixcel( y+140 , 1 );	

	if(theForm == 0 ){
		PrMessage( x+30,y+10, 8, 17, 34, " 合　格 ");	
		PrMessage( x+12,y+55, 8, 16, 32, Day );		
		PrMessage( x+45,y+95, 8, 17, 34, Inspector );		
	}
	else{
		//thuy -- PrMessage(left,top,font,width,height, text) ---

		//PrMessage( x+27,y+11, 8, 17, 40, "QCOK" );//thuy2
		PrMessage_Rotation( x+50,y+100, 8,FW_BOLD, 10, 40, "QCOK",1800 );//
		//PrMessage( x+30,y+18, 1, 20, 30, "QCOK" );					
		PrMessage_Rotation( x+80,y+58, 8, FW_SEMIBOLD, 11, 42, Day ,1800);	//
		//PrMessage( x+10,y+55, 1, 16, 32, Day );
		PrMessage_Rotation( x+70,y+17, 8, FW_BOLD, 10, 36, Inspector,1800 );	//
		//PrMessage( x+25,y+95, 1, 14, 28, Inspector );
 

		//PrMessage( x+30,y+18, 1, 15, 30, "Inspection" );	//Thuy
		//PrMessage( x+10,y+55, 1, 16, 32, Day );			//Thuy	
		//PrMessage( x+25,y+95, 1, 14, 28, Inspector );		//Thuy
	}

	hPen   = CreatePen( PS_SOLID , 3 , RGB(0,0,0));
	hOldPen = (HPEN)SelectObject(hdc,hPen);

	SelectObject(hdc,hOldPen);

	DeleteObject( hPen );
}

void TPrinter::DrawBitmap( int x, int y ,int nWidth,int nHeight, LPBITMAPINFO bmpinfo )
{
	HDC	hdc;

//	bmpinfo=GetBitmap(bmp);

	int sx,sy;
    char *szBuffer;
	int ClrPalette;
	int		xSize,ySize;			
//	BITMAP bm;

	xSize  = ConvertMmToPixcel( nWidth , 0 );
	ySize  = ConvertMmToPixcel( nHeight , 1 );

	sx = ConvertMmToPixcel( x , 0 );
	sy = ConvertMmToPixcel( y , 1 );

	
	hdc = hdcPrint;

/*
	GetObject( bmp,sizeof(BITMAP),&bm);
	memdc=CreateCompatibleDC(hdc);

	SelectObject(memdc,bmp);
	SetStretchBltMode( hdc,STRETCH_DELETESCANS);

	StretchBlt(hdc,sx,sy,xSize,ySize,memdc,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
	DeleteDC(memdc);
	DeleteObject(bmp);
*/
	
    //bmpinfo = (LPBITMAPINFO)LockResource(hResource);
    //bmpinfo = (LPBITMAPINFO)hResource;

	if(bmpinfo->bmiHeader.biClrUsed != 0)	
        ClrPalette = bmpinfo->bmiHeader.biClrUsed;
    else {
        if (bmpinfo->bmiHeader.biBitCount == 24)
            ClrPalette = 0;
        else 
            ClrPalette = 1 << (bmpinfo->bmiHeader.biBitCount);		//bmiHeader.biBitCount::1
    }

 
	szBuffer = (char *)bmpinfo + sizeof(BITMAPINFOHEADER) + ClrPalette * sizeof(RGBQUAD);

	
    StretchDIBits(
		hdc,
		sx,
		sy,
        xSize,//bmpinfo->bmiHeader.biWidth * 5,
        ySize,//bmpinfo->bmiHeader.biHeight * 5,
        0,
		0,
        bmpinfo->bmiHeader.biWidth,
        bmpinfo->bmiHeader.biHeight,
        szBuffer,
        bmpinfo,
        DIB_RGB_COLORS, SRCCOPY
	);

}





//====== Simple text rotation ================
// pDC : pointer to your device-context

// str : the text

// rect: the rectangle

// nOptions: can be a combination of ETO_CLIPPED and ETO_OPAQUE

// (see documentation of ExtTextOut for more details)

void TPrinter::DrawRotatedText(CDC* pDC, const CString str, CRect rect, double angle, UINT nOptions)
{
   // convert angle to radian

   double pi = 3.141592654;
   double radian = pi * 2 / 360 * angle;
 
   // get the center of a not-rotated text

   CSize TextSize = pDC->GetTextExtent(str);
   CPoint center;
   center.x = TextSize.cx / 2;
   center.y = TextSize.cy / 2;
 
   // now calculate the center of the rotated text

   CPoint rcenter;
   rcenter.x = long(cos(radian) * center.x - sin(radian) * center.y);
   rcenter.y = long(sin(radian) * center.x + cos(radian) * center.y);
 
   // finally draw the text and move it to the center of the rectangle

   pDC->SetTextAlign(TA_BASELINE);
   pDC->SetBkMode(TRANSPARENT);
   pDC->ExtTextOut(rect.left + rect.Width() / 2 - rcenter.x, 
                   rect.top + rect.Height() / 2 + rcenter.y,
                   nOptions, rect, str, NULL);
}


//Thuy - 2011-12-16
//GetDefaultPrinter -- Quit Print(1)
//int TPrinter::GetDefaultPrinterGlo(char *theDrv, char *theDev,char *thePort, short thePage ,short theOrient, HDC hdc)
int TPrinter::GetDefaultPrinterGlo(char *theDrv, char *theDev,char *thePort, short thePage ,short theOrient)
{
	int		stat;				
	HANDLE	phPrinter;		
	DWORD	dwDmSize;		
	DEVMODE	*devNew;		
	char *pszIn;
	DWORD	newdmVal;

	

	BOOL ret = GetDefaultPrinter( theDrv, theDev, thePort); 


	if( OpenPrinter(  theDev , &phPrinter , NULL ) == FALSE ){
		return(-1);
	}

	dwDmSize = DocumentProperties(	NULL ,			// handle to window that displays dialog box
									phPrinter,			// handle to printer object
									 theDrv,				// pointer to device name (Driver name)
								 	NULL ,				// pointer to modified device mode structure
									NULL ,				// pointer to original device mode structure
 									0 );				// mode flg ( 0:Get DEVMODE structure size )

 
	// DEVMODE
	pszIn = new char[dwDmSize+1];
	devNew = (DEVMODE *)pszIn;

	// DEVMODE
	stat = DocumentProperties(	NULL,phPrinter, theDrv,devNew ,NULL ,	DM_OUT_BUFFER );	// mode flg 
	if( stat != IDOK ){
		ClosePrinter( phPrinter );
		delete( pszIn );
		return(-2);
	}

	ClosePrinter( phPrinter );

	devNew -> dmPaperSize = thePage;		
	devNew -> dmCopies = 1;			
	devNew -> dmOrientation = theOrient;			
	devNew ->dmColor=DMCOLOR_COLOR;


	// dmFields
	newdmVal = devNew -> dmFields;
	devNew -> dmFields = newdmVal|DM_PAPERSIZE|DM_COPIES|DM_ORIENTATION|DM_COLOR;

	//Thuy 
	hdcPrint = CreateDC(  theDrv,  theDev,  thePort, devNew ); 	//hdc;  //CreateDC(  theDrv,  theDev,  thePort, devNew );

	if( hdcPrint == NULL ){
		delete( pszIn );
		return(-3);
	}

	PrinterResX = GetDeviceCaps( hdcPrint , HORZRES );	
	PrinterResY = GetDeviceCaps( hdcPrint , VERTRES );	
	
	stat = GetMapMode( hdcPrint );
	if( stat != MM_TEXT ){	
		SetMapMode( hdcPrint , MM_TEXT );
	}

	PrinterSizeX = GetDeviceCaps( hdcPrint , HORZSIZE );
	PrinterSizeY = GetDeviceCaps( hdcPrint , VERTSIZE );

	flgIPrinter = 1;					

	IPrinter();

	 return ret;

}
char *TPrinter::trim(char *chuoi)
{
   char *p;
   while (p = strstr(chuoi, "  "))
     memmove(p, p+1, strlen(chuoi) - (p - chuoi));
   if (chuoi[0] == ' ')
     memmove(chuoi, chuoi+1, strlen(chuoi) - 1);
   if (chuoi[strlen(chuoi)-1] == ' ')
     chuoi[strlen(chuoi)-1] = 0;
   return chuoi;
}
//===== End of Text =====
