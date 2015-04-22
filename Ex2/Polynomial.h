typedef struct Polynomial Polynomial;
//program functions

int createPolynomial(char *name, char* polynomialStr);//1
int Print(char *name);//2
Polynomial summation(char *name1, char *name2);//3
Polynomial subtraction(char *name1, char *name2);//4
Polynomial multiplication(char *name1, char *name2);//5
Polynomial derivation(char *name);//6
float evaluation(char *name, float value);//7
int createFromExisting(char* newName, char* pString);//8


typedef enum operation
{
	CREATE,
	PRINT,
	SUM,
	SUB,
	MUL,
	DER,
	EVAL,
	OP,
	ERROR

};

//help function
//enum operation getOperation(char* input);
int executeOperation(char* input);
int checkPName(char *name);
int split(const char *str, char c, char ***arr);
char* getString(size_t size);
char* remove_spaces(char* src, char* target);
int isPolynomial(char* pString);
int isNumber(char* str);
int getPolynomial(char* name);
