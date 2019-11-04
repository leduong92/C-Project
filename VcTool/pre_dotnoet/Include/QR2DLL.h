
// 以下の ifdef ブロックは DLL から簡単にエクスポートさせるマクロを作成する標準的な方法です。 
// この DLL 内のすべてのファイルはコマンドラインで定義された QR2DLL_EXPORTS シンボル
// でコンパイルされます。このシンボルはこの DLL が使用するどのプロジェクト上でも未定義でなけ
// ればなりません。この方法ではソースファイルにこのファイルを含むすべてのプロジェクトが DLL 
// からインポートされたものとして QR2DLL_API 関数を参照し、そのためこの DLL はこのマク 
// ロで定義されたシンボルをエクスポートされたものとして参照します。
#ifdef QR2DLL_EXPORTS
#define QR2DLL_API __declspec(dllexport)
#else
#define QR2DLL_API __declspec(dllimport)
#endif

QR2DLL_API long __stdcall QRmodel2(long *lngOriginal, long lngOriginalLength, long lngQRversion, long lngErrorLevel,long *lngResult);


long AlphaToNumber(long *Alpha);
long NumberToAlpha(long *Num10, long *Alpha);
long F_CharaCalc(long QR_Version , long Desc);
long F_Digit2ToNum10(long *lngOriginal , long lngOrder, long lngStrings);
long F_EightCheck(long *OriginalData ,long QR_Version, long *lngLocate, long lngOriginalLength);
long F_EnglishToCode(long *chrDummy, long lngLocate);
long F_EnglishToDigit2(long *chrDummy, long lngLocate, long lngStrings, long *lngResult, long *lngResultLocate);
long F_ErrorFunctionImage(long *ErrorAlpha,long CodeNumeric);
long F_FormInformation(long *EstimationField, long ErrorLevel , long MaskRef, long lngSell);
long F_LocateMark(long *Locate , long lngDummy);
long F_LocateMarkPut(long lngX , long lngY , long *QR_Field, long lngSell);
long F_Num10ToDigit2(long Num10 , long *Result , long *lngLocate , long ResultDigit);
long F_NumeralToDigit2(long *chrDummy, long *lngLocate, long lngStrings, long *lngResult, long *lngResultLocate);
long F_QR_FieldMaking(long *QR_Field,long lngSell);
long F_QR_MASC(long *QRfield , long ErrorLevel,long lngSell);
long F_QR_MascEstimation(long *EstimationField, long lngSell);
long F_QR_OriginalToDigit2(long *OriginalData , long lngLength, long QR_Version ,long *ResultData);
long F_STR_EightPlus(long *OriginalData, long *lngLocate , long *ResultData , long *lngResultLocate , long lngLength , long lngVerQlty);
long F_STR_EnglishPlus(long *OriginalData, long *lngLocate, long *ResultData, long *lngResultLocate, long lngLength, long lngVerQlty);
long F_STR_NumericPlus(long *OriginalData, long *lngLocate, long *ResultData, long *lngResultLocate, long lngLength , long lngVerQlty);
long F_VerCharaQty(long QR_Version, long Desc );
long F_VersionInformation(long *EstimationField , long lngSell);
bool IsChineseChara(long *chrDummy, long lngLocate);
bool IsEightAll(long *chrDummy, long lngLocate);
bool IsEightExclusive(long *chrDummy, long lngLocate);
bool IsEnglishNumber(long *chrDummy, long lngLocate);
bool IsEnglishOnly(long *chrDummy, long lngLocate);
bool IsNumberOnly(long *chrDummy, long lngLocate);
long QR_DataBitBaseA(long QR_Version, long ErrorLevel);
long QR_DataBitBaseB(long QR_Version, long ErrorLevel);
long QR_ErrorBitTable(long QR_Version,long ErrorLevel);
long QR_RSBlockBaseA(long QR_Version, long ErrorLevel);
long QR_RSBlockBaseB(long QR_Version, long ErrorLevel);
void DimClearForLong(long *Dim, long Digit);