typedef struct Polynomial Polynomial;
//program functions

int createPolynomial(char *name, char* polynomialStr);//1
void print(Polynomial *pol);//2
Polynomial* summation(char *name1, char *name2);//3
Polynomial* summationByPolynomials(Polynomial* p1, Polynomial* p2);
Polynomial* subtraction(char *name1, char *name2);//4
Polynomial* multiplication(char *name1, char *name2);//5
Polynomial* derivation(char *name);//6
float evaluation(char *name, float value);//7
int createFromExisting(char* newName, char* pString);//8



//help function
//enum operation getOperation(char* input);
int executeOperation(char* input);
int checkPName(char *name);
int split(char *str, char c, char ***arr);
char* getString(FILE* fp, size_t size);
int isPolynomial(char* pString);
int isNumber(char* str);
Polynomial *getPolynomial(char* name);
char* replace(char *s, char ch, char *repl);
int getMaxDegree(char **arrP, int arr_len);
void ExtractPolynom(Polynomial* pol, char* polyStr);
void cleanMemory(void);
int insertToPolynomArray(Polynomial* pol, int degree, float coeff, int len,int currLen);
int binsearch(int *arr, int size, int key);
void removeSpaces(char* src);
char *trimwhitespace(char *str);
