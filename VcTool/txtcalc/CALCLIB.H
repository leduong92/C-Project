// ���̃`�F�b�N����
#define STATEMENT_OK 0
#define SYNTAX_ERROR 1
#define VARIABLE_NOT_FOND 2
#define VARIABLE_BUFFER_FULL 3
#define DEVIDE_BY_ZERO 4
#define FUNCTION_NOT_FOUND 5

#ifdef __cplusplus
extern "C" {
#endif
/* �v�Z�菇�̎��s */
int WINAPI ExpCalculate(LPSTR expression, double* value);
/* �ϐ��̃N���A */
void WINAPI ExpClearVariable(void);
#ifdef __cplusplus
};
#endif