#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

//===================================================================================================================
//	FileName	:globldef.h 
//===================================================================================================================

//#include "length.h"
#include ".\tool\getnow.h"
#include <getini.h>
#include ".\tool\colordef.h"
#include "DbTnc.h"
//#include "BcrTrans.h"


#define szAppVer " Packing PC"
#define szVersion "1.11d"
//#define INIFILE_NAME "qimtnc.ini"
#define INIFILE_NAME "vnn.ini"
#define STOCK_NAME "Vnn_Stock.ini"
#define LOCATION_CD gszLocCd

#define CORP_NAME "VNN"
#define PRINT_FOOTER "VNN NOK CO.,LTD."
#define MSGTITLE "VNN QIM "
#define COPYRIGHT "Copyright (C) 2002-05 NOK Corp."

// Default font
#define DEF_FONT  "Times New Roman"
#define PR_N_FONT "Century Gothic"

// Default Packing Type
#define TYPE_C	'C'
#define TYPE_M	'M'
#define TYPE_N	'N'

// Status Stock
#define NO_STOCK	"0"
#define TAKE_IN		"1"

enum WindowNo{
	DLG_MAIN,
	TOTAL_WINDOWS,
};


// defined in "appain.cpp"
// It must be set only one at source files.
#ifdef NOTEXTERN
	#define EXT
#else
	#define EXT extern
#endif

// DB - "TncDb.h"
EXT TTncDB* g_dbTnc;
// HandyTerminal
//EXT BcrTrans* g_Bcr;

#endif
