typedef struct Polynomial Polynomial;
//program functions

int createPolynomial(char *name, char* polynomialStr);
int Print(char *name);
Polynomial summation(char *name1, char *name2);
Polynomial subtraction(char *name1, char *name2);
Polynomial multiplication(char *name1, char *name2);
Polynomial derivation(char *name);
float evaluation(char *name, float value);


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
int legalPolynom(char* pString);
int checkPName(char *name);
int split(const char *str, char c, char ***arr);
char* getString(size_t size);
char* remove_spaces(char* src, char* target);
