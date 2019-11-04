// 式のチェック結果
#define STATEMENT_OK 0
#define SYNTAX_ERROR 1
#define VARIABLE_NOT_FOND 2
#define VARIABLE_BUFFER_FULL 3
#define DEVIDE_BY_ZERO 4
#define FUNCTION_NOT_FOUND 5

#ifdef __cplusplus
extern "C" {
#endif
/* 計算手順の実行 */
int WINAPI ExpCalculate(LPSTR expression, double* value);
/* 変数のクリア */
void WINAPI ExpClearVariable(void);
#ifdef __cplusplus
};
#endif
