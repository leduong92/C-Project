/*=============================================================================
iii/Win  Ｃ言語インタ－フェイス用ヘッダファイル
==============================================================================*/
#ifndef IWINCL_PSDLI_H	//二重include防止策
	#define IWINCL_PSDLI_H
	#ifdef __cplusplus
		extern "C" {
	#endif


// 構造体や定数の定義
	//PsTblAccess()のオペレ－ション・モ－ド
	#define		FLD_APPEND   1		//フィールド定義追加
	#define		FLD_UPDATE   2		//フィールド定義更新
	#define		FLD_CLEAR    3		//フィールド定義クリア（全廃棄）
	#define		FLD_DELETE   4		//フィールド削除
	#define		FLD_GET      5		//フィールド定義の取得
	#define		FLD_ORGANIZE 6		//フィールド再編成
	#define		SCRN_INFOGET 7		//画面構成情報取得

	//フィールド定義テーブル
	#pragma pack(1)
	typedef struct _PsFldTbl{
		short		  FldFlag;		//フラッグ:必ず１
		short		  FldY;			//Ｙ座標
		short		  FldX;			//Ｘ座標
		short		  Reserve;		//予約：必ず０
		short		  FldLen;		//文字数
		unsigned char FldName[11];	//名称
		unsigned char FldCode;		//コ－ド
		unsigned char Justify;		//表示形式
		unsigned char FldZs;		//ｾﾛｻﾌﾟﾚｽ
		short 		  FldCol;		//文字色
		unsigned char MayEnter;		//入力許可
		unsigned char MustEnter;	//入力必須
		unsigned char MustFill;		//全桁入力
		unsigned char FldExit;		//CR抜け出し
		unsigned char FldLock;		//入力ﾛｯｸ
		unsigned char ChrType;		//入力字種
		short 		  FldDp;		//小数桁数
		unsigned char Llimit[10];	//下限値
		unsigned char Ulimit[10];	//上限値
		unsigned char ChrEnum[21];	//文字集合・飛び先
		unsigned char FldComma;		//カンマ編集
	}PsFldTbl;
	#pragma pack(8)
	
	//画面構成情報
	#pragma pack(1)
	typedef struct _PsScrnInfo{
		unsigned char ScrnName[75];		//画面名称
		short		  ScrnDex;			//画面デ－タ域の最大Ｘ文字座標
		short		  ScrnDey;			//画面デ－タ域の最大Ｙ文字座標
		short		  ScrnVcx;			//画面定義時の表示域の最大Ｘ文字座標
		short		  ScrnVcy;			//画面定義時の表示域の最大Ｘ文字座標
		short		  ScrnVsx;			//現在の画面表示域の 左上Ｘ文字座標
		short		  ScrnVsy;			//現在の画面表示域の 左上Ｙ文字座標
		short		  ScrnVex;			//現在の画面表示域の 右下Ｘ文字座標
		short		  ScrnVey;			//現在の画面表示域の 右下Ｙ文字座標
		short		  FieldMaxNo;		//フィールド定義最終番号
		unsigned char TopFieldName[11];	//先頭フィールドの名称
		unsigned char TailFieldName[11];//最終フィールドの名称
	}PsScrnInfo;
	#pragma pack(8)


// 関数の宣言
	#define iImport __declspec(dllimport)

	iImport void psinit(void);
	iImport void psscrn(char *file_name);
	iImport void pswndw(char *file_name);
	iImport void pswndwn(int wndw_num);
	iImport void pscls(void);
	iImport void pssave(void);
	iImport void psrstr(void);
	iImport void psquit(void);
	iImport int psinpt(char *fld_name);
	iImport void psfclr(char *fld_name);
	iImport void psaclr(char *bgn_fld,char *end_fld);
	iImport char pschgt(char *fld_name,char *message,char *valid_str);
	iImport int psichk(void);
	iImport void psposn(char *fld_name);
	iImport int pschnd(void);
	iImport char pscode(char *fld_name);
	iImport int psflen(char *fld_name);
	iImport void pserr(int ln_num,int col_num,char *err_str);
	iImport void psalrm(int ln_num,int col_num,char *alm_str);
	iImport int psergt(void);
	iImport void pspath(char *path_name);
	iImport void psafnm(char *file_name);
	iImport int pscinit(void);
	iImport int psslct(int bgn_pos);
	iImport int psslcts(int bgn_pos,int sub_loc,int sub_len,char *sub_str);
	iImport int psofst(char *fld_name);
	iImport int psofstn(char *fld_name,int ary_num);
	iImport void psfnnm(int serial,char *fld_name);
	iImport void psffnm(char *sp_name,char *fld_name);
	iImport void psdset(char *fld_name);
	iImport void psdsetn(char *fld_name,int ary_num);
	iImport void pslock(char *fld_name);
	iImport void psulck(char *fld_name);
	iImport void pslockn(char *fld_name,int ary_num);
	iImport void psulckn(char *fld_name,int ary_num);
	iImport void psalck(char *bgn_fld,char *end_fld);
	iImport void psaulk(char *bgn_fld,char *end_fld);
	iImport void psalckn(char *bgn_fld,int bgn_num,char *end_fld,int end_num);
	iImport void psaulkn(char *bgn_fld,int bgn_num,char *end_fld,int end_num);
	iImport void psfcol(char *fld_name,int col_num);
	iImport void psfcoln(char *fld_name,int ary_num,int col_num);
	iImport void psbrnk(char *fld_name);
	iImport void psubrk(char *fld_name);
	iImport void psbrnkn(char *fld_name,int ary_num);
	iImport void psubrkn(char *fld_name,int ary_num);
	iImport int psslst(void);
	iImport void psfkon(void);
	iImport void psfkof(void);
	iImport void psfken(int key_num);
	iImport void psfkda(int key_num);
	iImport void psfkst(int key_num,char *fk_str,int col_num);
	iImport void PsFkText(int EventNo,char* Text,char Enable);
	iImport void psaben(int key_num);
	iImport void psabda(int key_num);
	iImport int psarno(char *fld_name);
	iImport int psinptn(char *fld_name,int ary_num);
	iImport void psfclrn(char *fld_name,int ary_num);
	iImport void psaclrn(char *bgn_fld,int bgn_num,char *end_fld,int end_num);
	iImport void psposnn(char *fld_name,int ary_num);
	iImport char pscoden(char *fld_name,int ary_num);
	iImport int psarnon(char *fld_name,int ary_num);
	iImport int psflenn(char *fld_name,int ary_num);
	iImport void pssput(char *fld_name,char *put_str);
	iImport void pssget(char *fld_name,char *get_str);
	iImport void psspgt(char *get_str);
	iImport void pssputn(char *fld_name,int ary_num,char *put_str);
	iImport void pssgetn(char *fld_name,int ary_num,char *get_str);
	iImport void pscput(char *fld_name,char put_char);
	iImport char pscget(char *fld_name);
	iImport char pscpgt(void);
	iImport void pscputn(char *fld_name,int ary_num,char put_char);
	iImport char pscgetn(char *fld_name,int ary_num);
	iImport void psiput(char *fld_name,int put_ival);
	iImport int psiget(char *fld_name);
	iImport int psipgt(void);
	iImport void psiputn(char *fld_name,int ary_num,int put_ival);
	iImport int psigetn(char *fld_name,int ary_num);
	iImport void psrput(char *fld_name,double put_dval);
	iImport double psrget(char *fld_name);
	iImport double psrpgt(void);
	iImport void psrputn(char *fld_name,int ary_num,double put_dval);
	iImport double psrgetn(char *fld_name,int ary_num);
	iImport void pslput(char *fld_name,long put_lval);
	iImport long pslget(char *fld_name);
	iImport long pslpgt(void);
	iImport void pslputn(char *fld_name,int ary_num,long put_lval);
	iImport long pslgetn(char *fld_name,int ary_num);
	iImport void psfput(char *fld_name,double put_dval);
	iImport double psfget(char *fld_name);
	iImport double psfpgt(void);
	iImport void psfputn(char *fld_name,int ary_num,double put_dval);
	iImport double psfgetn(char *fld_name,int ary_num);
	iImport void pspush(char *bgn_fld,char *end_fld);
	iImport void pspop(char *bgn_fld,char *end_fld);
	iImport void pspushn(char *bgn_fld,int bgn_num,char *end_fld,int end_num);
	iImport void pspopn(char *bgn_fld,int bgn_num,char *end_fld,int end_num);
	iImport void psspush(char *bgn_fld,char *end_fld,char *fil_str);
	iImport void psspop(char *bgn_fld,char *end_fld,char *fil_str);
	iImport void psspushn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,char *fil_str);
	iImport void psspopn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,char *fil_str);
	iImport void pscpush(char *bgn_fld,char *end_fld,char fil_char);
	iImport void pscpop(char *bgn_fld,char *end_fld,char fil_char);
	iImport void pscpushn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,char fil_char);
	iImport void pscpopn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,char fil_char);
	iImport void psipush(char *bgn_fld,char *end_fld,int fil_ival);
	iImport void psipop(char *bgn_fld,char *end_fld,int fil_ival);
	iImport void psipushn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,int fil_ival);
	iImport void psipopn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,int fil_ival);
	iImport void psrpush(char *bgn_fld,char *end_fld,double fil_dval);
	iImport void psrpop(char *bgn_fld,char *end_fld,double fil_dval);
	iImport void psrpushn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,double fil_dval);
	iImport void psrpopn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,double fil_dval);
	iImport void pslpush(char *bgn_fld,char *end_fld,long fil_lval);
	iImport void pslpop(char *bgn_fld,char *end_fld,long fil_lval);
	iImport void pslpushn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,long fil_lval);
	iImport void pslpopn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,long fil_lval);
	iImport void psfpush(char *bgn_fld,char *end_fld,double fil_dval);
	iImport void psfpop(char *bgn_fld,char *end_fld,double fil_dval);
	iImport void psfpushn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,double fil_dval);
	iImport void psfpopn(char *bgn_fld,int bgn_num,char *end_fld,int end_num,double fil_dval);
	iImport char *PsCmd(char *Para1);
	iImport void psset(char *para1,char *para2,char *para3);
	iImport void pssquery(char *para1,char *para2,char *para3,char *get_data);
	iImport char *psquery(char *para1,char *para2,char *para3);
	iImport int psiquery(char *para1,char *para2,char *para3);
	iImport long pslquery(char *para1,char *para2,char *para3);
	iImport char *lta(long value);
	iImport char *ita(int value);
	iImport void psabort(int AbortLevel);
	iImport int PsHcPrint(char *Fname,char *Option);
	iImport void PsFColor(char *FName,int ForeCol,int BackCol);
	iImport void PsFColorN(char *FName,int ArrayNo,int ForeCol,int BackCol);
	iImport int PsCallFunc(int (__stdcall *pFunc)(void*),void* pArg);

	//変数型がiii/2と異なるもの
	typedef	int	xShort;
	iImport xShort PsTblAccess(xShort OpeMode,xShort TblNo,void *BufPtr,xShort Size);
	iImport xShort PsReadFieldTbl(char *FileName,PsFldTbl** ppBuf,xShort *Cnt);

	#ifdef __cplusplus
		}
	#endif
#endif // IWINCL_PSDLI_H 

