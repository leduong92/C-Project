#ifndef _BTITYPES_H_INCLUDED
/*************************************************************************
**
**  Copyright 1982-1995 Btrieve Technologies, Inc. All Rights Reserved
**
*************************************************************************/
/*************************************************************************
   BTITYPES.H
     This module contains platform-independent data types used by the
     Btrieve and Scalable SQL C/C++ Interface for MS Windows, OS2, DOS,
     Extended DOS, NetWare NLM, Win32s, and MS Windows NT.

     You must define one of the following to your compiler in order to
     compile for a particular platform:

        BTI_DOS     - DOS Real Mode                           16-bit Apps
        BTI_WIN     - MS WINDOWS                              16-bit Apps
        BTI_OS2     - OS2                                     16-bit Apps
        BTI_NLM     - NetWare NLM                             32-bit Apps
        BTI_WIN_32  - Windows NT and Win32s                   32-bit Apps
        BTI_OS2_32  - OS2                                     32-bit Apps
        BTI_DOS_32R - Extended DOS with Rational + bstub.exe  32-bit Apps
        BTI_DOS_32P - Extended DOS with Phar Lap 6.0          32-bit Apps
        BTI_DOS_32B - Extended DOS with Borland PowerPack     32-bit Apps
        BTI_DOS_16B - Extended DOS with Borland PowerPack     16-bit Apps

     If you do not specify one of the above switches to your compiler,
     an error directive in btitypes.h will halt the compilation.
*************************************************************************/
#define BTI_WIN_32

#if !defined(BTI_WIN) && !defined(BTI_OS2) && !defined(BTI_DOS) && \
    !defined(BTI_NLM) && !defined(BTI_DOS_32R) && !defined(BTI_DOS_32P) && \
    !defined(BTI_DOS_32B) && !defined(BTI_WIN_32) && !defined(BTI_OS2_32) && \
    !defined(BTI_DOS_16B)
#error You must define one of the following: BTI_WIN, BTI_OS2, BTI_DOS, BTI_NLM, BTI_DOS_32R, BTI_DOS_32P, BTI_DOS_32B, BTI_WIN_32, BTI_OS2_32, BTI_DOS_16B
#endif

   /* ONLY FOR NLM APPS THAT PASS PARAMETERS IN REGISTERS */
   #if defined (__WATCOMC__) && defined (BTI_NLM)
      /***********************************************************************
         Declare one of the  following cdecl pragmas IF AND ONLY IF your
         NLM application is using register-based parameter-passing.  Most
         users will not have to modify this file.
      ***********************************************************************/
      /********************* WATCOM Compiler Version <= 8.5 ******************
       #define USING_REGISTERS
       #pragma aux cdecl "*" parm caller []\
               value struct float struct routine [eax]\
               modify [eax ecx edx fs gs 8087];
      ***********************************************************************/
      /********************* WATCOM Compiler Version >= 9.01 *****************
      #define USING_REGISTERS
      #pragma aux cdecl "*" parm caller []\
              modify [eax ecx edx gs 8087];
      ***********************************************************************/
   #endif

  /* Platform-Dependent Definitions for BTI_FAR and BTI_PASCAL */
   #if defined(BTI_WIN) 
      #define BTI_FAR       far
      #define BTI_PASCAL    pascal
   #endif

   #if defined(BTI_WIN_32)
     #if defined(__WATCOMC__)
        #pragma aux BTRCALL "*"
        #pragma aux BTRCALLID "*"
        #pragma aux WBTRVINT "*"
        #pragma aux WBRQSHELLINIT "*"
        #pragma aux WBTRVSTOP "*"
        #pragma aux BTRCALLBACK "*"
        #pragma aux RQSHELLINIT "*"
     #endif
        #include <windows.h>
        #define BTI_FAR      
        #define BTI_PASCAL APIENTRY
   #endif

   #if defined(BTI_NLM) || defined(BTI_DOS_32R) || defined(BTI_DOS_32P) 
      #define BTI_FAR
      #define BTI_PASCAL 
   #endif

   #if defined(BTI_DOS_32B)
      #define BTI_FAR
      #define BTI_PASCAL pascal
   #endif

   #if defined(BTI_DOS_16B)
      #define BTI_FAR       far
      #define BTI_PASCAL    pascal
   #endif

   #if defined(BTI_OS2)
      #define BTI_FAR       far
      #define BTI_PASCAL    pascal
   #endif

   #if defined(BTI_DOS)
      #define BTI_FAR       far
      #define BTI_PASCAL   
   #endif

   #if defined(BTI_OS2_32)
      #define BTI_FAR
      #define INCL_BASE
      #include <os2.h>
      #define BTI_PASCAL APIENTRY
   #endif

   /* Platform-Independent Data Types */
   typedef long               BTI_LONG;
   typedef unsigned long      BTI_ULONG;
   typedef short int          BTI_SINT;
   typedef unsigned short     BTI_WORD;
   typedef int                BTI_INT;
   typedef char               BTI_CHAR;
   typedef unsigned char      BTI_BYTE;
   typedef void               BTI_VOID;
   typedef BTI_CHAR  BTI_FAR* BTI_CHAR_PTR;
   typedef BTI_BYTE  BTI_FAR* BTI_BUFFER_PTR;
   typedef BTI_SINT  BTI_FAR* BTI_SINT_PTR;
   typedef BTI_INT   BTI_FAR* BTI_INT_PTR;
   typedef BTI_WORD  BTI_FAR* BTI_WORD_PTR;
   typedef BTI_LONG  BTI_FAR* BTI_LONG_PTR;
   typedef BTI_ULONG BTI_FAR* BTI_ULONG_PTR;
   typedef BTI_VOID  BTI_FAR* BTI_VOID_PTR;

   #if defined(USING_REGISTERS)
   #define BTI_API extern BTI_SINT BTI_FAR BTI_PASCAL cdecl
   #endif

   #if !defined(USING_REGISTERS)
   #define BTI_API extern BTI_SINT BTI_FAR BTI_PASCAL
   #endif

   /* Callback Functions Data Types for MS Windows Applications */
   #if defined(BTI_WIN) || defined(BTI_WIN_32)

   typedef struct
   {
      BTI_WORD iSessionID;
   } NWSQL_YIELD_T;

   typedef struct
   {
      BTI_WORD  iOpCode;
      BTI_BYTE  bClientIDlastFour[ 4 ];
   } BTRV_YIELD_T;

   typedef struct
   {
      BTI_WORD type;
      BTI_WORD size;
      union
      {
          NWSQL_YIELD_T sYield;
          BTRV_YIELD_T  bYield;
      } u;
   } BTI_CB_INFO_T;

   typedef BTI_SINT ( BTI_FAR BTI_PASCAL *BTI_CB_FUNC_PTR_T ) (
                                        BTI_CB_INFO_T BTI_FAR *bCallbackInfo,
                                        BTI_VOID_PTR           bUserData );

   #endif

#define _BTITYPES_H_INCLUDED
#endif
