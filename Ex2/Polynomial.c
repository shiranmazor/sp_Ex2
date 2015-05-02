// Ex2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "Polynomial.h"
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

//define structures:
struct Polynomial{
	char *name;
	//the value in the fields is the coeff and the index is the degree
	float *coeffs;
	//array of degrees
	int *degrees;
	int p_len;
	Polynomial *next;
};

//linkied list help fields
Polynomial *firstPolynomialPtr;
Polynomial *lastPolynomialPtr;
//help function implementation:

/*
reading string input from user and relloc the char pointer
size - initial size of the pointer
*/
char* getString(FILE* fp, size_t size)
{
	char *str;
	int ch;
	size_t len = 0;
	str = realloc(NULL, sizeof(char)*size);
	if (!str)
		return str;
	while (EOF != (ch=fgetc(fp)) && ch != '\n')
	{
		str[len++] = ch;
		if (len == size)
		{
			str = realloc(str, sizeof(char)*(size += 16));
			if (!str)
				return str;
		}
	}
	str[len++] = '\0';
	//return realloc(str, sizeof(char)*len);
	return str;
}

int split(const char *str, char c, char ***arr)
{
	int count = 1;
	int token_len = 1;
	int i = 0;
	char *p; char *t;
	p = str;
	//1 - counting delimiter\splitted output string
	while (*p != '\0')
	{
		if (*p == c)
			count++;
		p++;
	}
	*arr = (char**)malloc(sizeof(char*)*count);
	if (*arr == NULL)
	{
		perror("memory allocation while spiliting has failed!");
		assert(arr != NULL);
	}
		
	if (*arr == NULL)
		exit(1);

	p = str;
	//2 - allocating arr space
	while (*p != '\0')
	{
		if (*p == c)
		{
			(*arr)[i] = (char*)malloc(sizeof(char)*token_len);
			if ((*arr)[i] == NULL)
			{
				perror("memory allocation while spiliting has failed!");
				assert((*arr)[i] != NULL);
			}
				
			if ((*arr)[i] == NULL)
				exit(1);

			token_len = 0;
			i++;
		}
		p++;
		token_len++;
	}

	(*arr)[i] = (char*)malloc(sizeof(char)*token_len);
	if ((*arr)[i] == NULL)
	{
		perror("memory allocation while spiliting has failed!");
		assert((*arr)[i] != NULL);
	}
		
	if ((*arr)[i] == NULL)
		exit(1);

	i = 0;
	p = str;
	t = ((*arr)[i]);
	//3 - splitting the pointer by delimiter
	while (*p != '\0')
	{
		if (*p != c && *p != '\0')
		{
			*t = *p;
			t++;
		}
		else
		{
			*t = '\0';
			i++;
			t = ((*arr)[i]);
		}
		p++;
	}

	*t = '\0';
	i++;
	t = ((*arr)[i]);
	return count;

}

char* removeSpaces(char* src)
{
	char* i = src;
	char* j = src;
	while (*j !=0)
	{
		*i = *j++;
		if (*i != ' ')
			i++;
	}
	*i = 0;
}

int checkPName(char *name)
{
	//check if start with a letter:
	if (!isalpha(name[0]))
	{
		return 0;
	}
		
	for (int i = 0; i < strlen(name); i++)
	{
		if (isalpha(name[i]) || isdigit(name[i]) || name[i] == ' ')
			continue;
		else
		{
			return 0;
		}
			
	}
	//check if one of the commands are in varible name
	if (strcmp(name, "der") == 0 || strcmp(name, "eval") == 0 || strcmp(name, "quit") == 0 || strcmp(name, "x") == 0
		|| strcmp(name, "der ") == 0 || strcmp(name, "eval ") == 0 || strcmp(name, "quit ") == 0 || strcmp(name, "x ") == 0)
	{
		return 0;
	}

	//name is o.k
	return 1;
}

/*check if the string that given is in the form of polynom with x varible*/

int isPolynomial(char* pString)
{

	for (int i = 0; i < strlen(pString); i++)
	{
		if ((pString[i] ==  'x') || (pString[i] == ' ') || (pString[i] == '^') ||
			(isdigit(pString[i])) || (pString[i] == '.') || (pString[i] == '+') || (pString[i] == '-') || (pString[i] == '\0'))
			continue;
		else // found invalid char
			return 0;
	}
	return 1;
}


/*check is the string contains only digit characters*/
int isNumber(char* str)
{
	int i = 0;
	while ( i<strlen(str) )
	{
		if (str[i] <48 || str[i]> 57)
			return 0;
		i++;
	}
	return 1;
}

/* return a new string with every instance of ch replaced by repl */
char* replace(const char *s, char ch, const char *repl) {
	int count = 0;
	const char *t;
	for (t = s; *t; t++)
		count += (*t == ch);

	size_t rlen = strlen(repl);
	char *res = malloc(strlen(s) + (rlen - 1)*count + 1);
	if (res == NULL)
	{
		perror("memory allocation in replace has failed!");
		assert(res != NULL);
	}
		
	char *ptr = res;
	for (t = s; *t; t++) {
		if (*t == ch) {
			memcpy(ptr, repl, rlen);
			ptr += rlen;
		}
		else {
			*ptr++ = *t;
		}
	}
	*ptr = 0;
	return res;
}

int executeOperation(char* input)
{
	char **arr = NULL;
	int arr_len = split(input, '=', &arr);
	//arr_len should be 2
	if (arr_len > 1)//found '=' char in command - create new/from existing
	{	
		//the command should be or cerate new or create from an existing object
		char* pName = arr[0];
		char* pString = arr[1];
		if (!checkPName(pName))
		{
			printf("illegal variable name\n");
			return 0;
		}
		else
		{
			//check if command 1 or 8
			if (isPolynomial(pString) == 1)
				createPolynomial(pName, pString);
			else
				createFromExisting(pName, pString);
		}	
	}
	else if (strstr(input, " ")) //riven or eval
	{
		char **arr2 = NULL;
		int arr2_len = split(input, ' ', &arr2);
		if (arr2_len > 1)
		{
			//if (!checkPName(arr2[1]))
				//return 1;

			if (strcmp(arr2[0], "der") == 0)
			{
				Polynomial* res;
				res = derivation(arr2[1]);
				if (res != NULL)
				{
					print(res);
					free(res);
				}
			}
			else if (strcmp(arr2[0], "eval") == 0 && arr2_len == 3)
			{
				printf("%.2f\n",evaluation(arr2[1], atof(arr2[2])));
			}
			else if (checkPName(arr2[0]) == 1)//printing command with spaces
			{
				//removeSpaces(arr2[0]);
				Polynomial *existPol = getPolynomial(arr2[0]);
				if (existPol != NULL)
				{
					print(existPol);
				}
				else
				{
					printf("unknown polynomial %s\n", arr2[0]);
				}
			}
		}

	}
	else if (strchr(input, '+') != NULL)//+ exist without '=' - sum
	{
		removeSpaces(input);
		char **arrSum = NULL;
		int arr_len = split(input, '+', &arrSum);
		Polynomial *res = summation(arrSum[0], arrSum[1]);
		if (res != NULL)
			print(res);
		free(res);
		free(arrSum);
	}
	else if (strchr(input, '-') != NULL)//- exist without '=' - sub
	{
		removeSpaces(input);
		char **arrSub = NULL;
		int arr_len = split(input, '-', &arrSub);
		Polynomial *res = subtraction(arrSub[0], arrSub[1]);
		if (res != NULL)
			print(res);
		free(res);
		free(arrSub);
	}
	else if (strchr(input, '*') != NULL)// mul
	{
		removeSpaces(input);
		char **arrMul = NULL;
		int arr_len = split(input, '*', &arrMul);
		Polynomial *res = multiplication(arrMul[0], arrMul[1]);
		if (res != NULL)
			print(res);
		free(res);
		free(arrMul);
	}
	else if (checkPName(input) == 1) //print
	{
		//get  polynom in case of printing:
		removeSpaces(input);
		Polynomial *existPol = getPolynomial(input);
		if (existPol != NULL)
		{
			print(existPol);
		}
		else
		{
			printf("unknown polynomial %s\n", input);
		}
	}
	
	free(arr);
	return 1;
}

void reduceSpaces(char * str)
{
	while (strstr(str, "  "))
	{
		
	}
}
/*
Returns polynomial with the provided name, NULL if not exists
*/
Polynomial* getPolynomial(char* name)
{
	Polynomial *currentPolynomial = firstPolynomialPtr;
	if (firstPolynomialPtr == NULL) //no polynomial was defined
		return NULL;	
	do
	{
		if (strcmp((*currentPolynomial).name, name) == 0)			
			return currentPolynomial;

		currentPolynomial = (*currentPolynomial).next;
	} while (currentPolynomial != NULL);


	return NULL;
}

/*return tha maximum degree of the polynom for initializing the coeffs array size*/
int getMaxDegree(char **arrP, int arr_len)
{
	int max_deg = 0;
	int current = 0;
	char **arr = NULL;
	for (int i = 0; i < arr_len; i++)
	{
		char* item = arrP[i];
		if (strchr(item, '^') != NULL)
		{
			int arr_len = split(item, '^', &arr);
			current = (int)atof(arr[arr_len - 1]);
			if (current > max_deg)
				max_deg = current;
		}
		else if (strchr(item, 'x') != NULL)
		{
			current = 1;
			if (current > max_deg)
				max_deg = current;
		}
			
	}
	free(arr);
	return max_deg;
}

/*extract polynomial coeffs and exponents and fill in polynomial pointer*/
void ExtractPolynom(Polynomial* pol, char* polyStr)
{
	//extract coeffs and exponents

	char *newP = replace(polyStr, '-', "+-");
	free(polyStr);
	char **arrP = NULL;
	int p_len = split(newP, '+', &arrP);
	int max_deg = getMaxDegree(arrP, p_len);
	pol->p_len = p_len;

	//init coeffs and degree arrays and alocate memory with zero values:
	pol->coeffs = (float*)malloc(p_len * sizeof(float));
	pol->degrees = (int*)malloc(p_len *sizeof(int));
	int curLen = 0;
	//let's fill in the coeffs array 
	//insert order - always the lowest degree will come to the first place
	for (int i = 0; i < p_len; i++)
	{
		int degree = 0;
		int coeff = 0;
		//i - represent the place 
		if (strchr(arrP[i], '^') != NULL)
		{
			//degree > 1
			char **tmpArr = NULL;
			int len = split(arrP[i], '^', &tmpArr);
			degree = atof(tmpArr[len - 1]);//degree
			
			if (strcmp(tmpArr[0], "x") == 0)
				coeff = 1;
			else if (strcmp(tmpArr[0], "-x") == 0)
				coeff = -1;
			else
			{
				//split with x:
				char **tmpArr2 = NULL;
				int len = split(tmpArr[0], 'x', &tmpArr2);
				float num = atof(tmpArr2[0]);
				coeff = num;
				free(tmpArr2);
			}
			free(tmpArr);
		}
		else//don't have '^' in it- degree =0/1
		{
			//in this case the item is - 'dx/x/d', d - digits
			if (strchr(arrP[i], 'x') == NULL)
			{
				degree = 0;
				coeff = atof(arrP[i]);
			}
			else if (strcmp(arrP[i], "-x") == 0)
			{
				degree = 1;
				coeff = -1;
			}
				
			else if (strcmp(arrP[i], "x") == 0)
			{
				degree = 1;
				coeff = 1;
			}
			else  
			{
				degree = 1;
				char **tmpArr = NULL;
				int len = split(arrP[i], 'x', &tmpArr);
				coeff = atof(tmpArr[0]);
				free(tmpArr);
			}

		}
		//insert coeff and degree in increase order by degree
		
		if (i == 0)
		{
			curLen = insertToPolynomArray(pol, degree, coeff, p_len, curLen);
		}
		else
			curLen = insertToPolynomArray(pol, degree, coeff, p_len, curLen);
		
	}

	free(arrP);
}

int binsearch(int *arr, int size, int key)
{
	int low = 0, high = size - 1;

	if (key > arr[high]) return size;

	while (high > low) {
		int mid = (low + high) / 2;
		if (arr[mid] > key) high = mid;
		else low = mid + 1;
	}

	return low;
}

int insertToPolynomArray(Polynomial* pol,int degree, float coeff, int len, int currLen)
{
	if (currLen == 0)
	{
		//first time
		pol->degrees[0] = degree;
		pol->coeffs[0] = coeff;
		currLen++;
	}
	else
	{
		int pos = binsearch(pol->degrees, currLen, degree);
		int i;
		for (i = currLen - 1; i >= pos; --i)
		{
			pol->degrees[i + 1] = pol->degrees[i];
			pol->coeffs[i + 1] = pol->coeffs[i];
		}
		pol->degrees[pos] = degree;
		pol->coeffs[pos] = coeff;
		currLen++;
	}
	return currLen;
	
}

//program functions
//1:
int createPolynomial(char *name, char* polynomialStr)
{
	removeSpaces(name);
	removeSpaces(polynomialStr);
	Polynomial *pol = getPolynomial(name);

	if (pol == NULL) //polynom with that name doesn't exsist
	{
		//create new
		pol = malloc(sizeof(struct Polynomial));
		if (pol == NULL)
		{
			perror("failed  to allocate memory in createPolynomial function");
			assert(pol != NULL);

		}
	
		if (firstPolynomialPtr == NULL)
		{
			firstPolynomialPtr = pol;
			lastPolynomialPtr = firstPolynomialPtr;
		}
		else
		{
			(*lastPolynomialPtr).next = pol;
			lastPolynomialPtr = pol;
		}

		lastPolynomialPtr->next = NULL;
	
		pol->name = malloc(strlen(name) + 1);
		if (pol->name == NULL)
		{
			perror("failed to allocate memory in createing polynomial name");
			assert(pol->name != NULL);
		}
		strcpy(pol->name, name);
		printf("created %s\n", name);
			
	}
	else
	{
		printf("updated %s\n",name);
		//update existing - pol contains the existing polynomial
	}

	//extract coeffs and exponents
	ExtractPolynom(pol, polynomialStr);
	return 1;
}

//2:print
void print(Polynomial *pol)
{
	float *arrC = pol->coeffs;
	int *arrD = pol->degrees;
	if (pol->p_len == 0)
		printf("%.2f", 0);
	if (pol->p_len == 1 && arrC[0] == 0)
		printf("%.2f", arrC[0]);
	int first = 1;

	for (int i = 0; i < pol->p_len; i++)
	{
		if (arrC[i] == 0)
			continue;
		if (arrD[i] == 0)
		{
			//print number only
			printf("%.2f", arrC[i]);
			first = 0;
		}
		else if (arrD[i] == 1)
		{
			if (first == 1)
			{
				if (arrC[i] == 1)
					printf("x");
				else if (arrC[i] == -1)
					printf("- x");
				else
					printf("%.2fx", arrC[i]);
				first = 0;
			}
			else
			{
				if (arrC[i] > 0)
				if (arrC[i] == 1)
					printf(" + x");
				else
					printf(" + %.2fx", arrC[i]);
				else
				if (arrC[i] == -1)
					printf(" - x");
				else
					printf(" %.2fx", arrC[i]);
			}
		}
		else// deg > 1
		{
			//i>2
			if (first == 1)
			{
				if (arrC[i] == 1)
					printf("x^%d", arrD[i]);
				else if (arrC[i] == -1)
					printf("-x^%d", i);
				else
					printf("%.2fx^%d", arrC[i], arrD[i]);
				first = 0;
			}
			else
			{
				if (arrC[i] > 0)
				if (arrC[i] == 1)
					printf(" + x^%d", arrD[i]);
				else
					printf(" + %.2fx^%d", arrC[i], arrD[i]);
				else
				if (arrC[i] == -1)
					printf(" - x^%d", arrD[i]);
				else
					printf(" %.2fx^%d", arrC[i], arrD[i]);

			}
		}
	}
	printf("\n");
}

Polynomial *summationByPolynomials(Polynomial* p1, Polynomial* p2)
{
	Polynomial *res = NULL;
	res = malloc(sizeof(Polynomial));
	if (res == NULL)
	{
		perror("Error when allocating memory in summation");
		assert(res != NULL);
	}
	float* coeffs = (float*)calloc(p1->p_len+p2->p_len, sizeof(float));
	int* degs = (int*)calloc(p1->p_len + p2->p_len, sizeof(int));
	
	//perform merge sort:
	int i = 0; int j = 0; int k = 0;
	while (i < p1->p_len && j < p2->p_len)
	{
		if (p1->degrees[i] < p2->degrees[j])
		{
			degs[k] = p1->degrees[i];
			coeffs[k] = p1->coeffs[i];
			i++;
		}
		else
		{
			if (p1->degrees[i] == p2->degrees[j])
			{
				degs[k] = p1->degrees[i];
				coeffs[k] = p1->coeffs[i] + p2->coeffs[j];
				i++;
				j++;
			}
			else
			{
				degs[k] = p2->degrees[j];
				coeffs[k] = p2->coeffs[j];
				j++;
			}
		}
		k++;
	}
	while (i < p1->p_len)
	{
		degs[k] = p1->degrees[i];
		coeffs[k] = p1->coeffs[i];
		i++;
		k++;
	}
	while (j < p2->p_len)
	{
		degs[k] = p2->degrees[j];
		coeffs[k] = p2->coeffs[j];
		j++;
		k++;
	}
	//insert to res:
	res->p_len = k;
	res->coeffs = (float*)calloc(res->p_len, sizeof(float));
	res->degrees = (int*)calloc(res->p_len, sizeof(int));
	for (int i = 0; i < k; i++)
	{
		res->coeffs[i] = coeffs[i];
		res->degrees[i] = degs[i];
	}
	return res;
}

//3:sum:
Polynomial* summation(char *name1, char *name2)
{
	
	Polynomial* p1 = getPolynomial(name1);
	Polynomial* p2 = getPolynomial(name2);
	if (p1 == NULL)
		printf("unknown polynomial %s\n", name1);
	else if (p2 == NULL)
		printf("unknown polynomial %s\n", name2);
	else
	{
		return summationByPolynomials(p1,p2);
	}
	return NULL;
}
//4:sub
Polynomial* subtraction(char *name1, char *name2)
{
	Polynomial *res = NULL;
	Polynomial* p1 = getPolynomial(name1);
	Polynomial* p2 = getPolynomial(name2);
	if (p1 == NULL)
		printf("unknown polynomial %s\n", name1);
	else if (p2 == NULL)
		printf("unknown polynomial %s\n", name2);
	else
	{
		res = malloc(sizeof(struct Polynomial));
		if (res == NULL)
		{
			perror("Failed to allocate memory in subtraction");
			assert(res != NULL);
		}
		float* coeffs = (float*)calloc(p1->p_len + p2->p_len, sizeof(float));
		int* degs = (int*)calloc(p1->p_len + p2->p_len, sizeof(int));

		//perform merge sort:
		int i = 0; int j = 0; int k = 0;
		while (i < p1->p_len && j < p2->p_len)
		{
			if (p1->degrees[i] < p2->degrees[j])
			{
				degs[k] = p1->degrees[i];
				coeffs[k] = p1->coeffs[i];
				i++;
			}
			else
			{
				if (p1->degrees[i] == p2->degrees[j])
				{
					degs[k] = p1->degrees[i];
					coeffs[k] = p1->coeffs[i] - p2->coeffs[j];
					i++;
					j++;
				}
				else
				{
					degs[k] = p2->degrees[j];
					coeffs[k] = p2->coeffs[j] * (-1);
					j++;
				}
			}
			k++;
		}
		while (i < p1->p_len)
		{
			degs[k] = p1->degrees[i];
			coeffs[k] = p1->coeffs[i];
			i++;
			k++;
		}
		while (j < p2->p_len)
		{
			degs[k] = p2->degrees[j];
			coeffs[k] = p2->coeffs[j]*(-1);
			j++;
			k++;
		}
		//insert to res:
		res->p_len = k;
		res->coeffs = (float*)calloc(res->p_len, sizeof(float));
		res->degrees = (int*)calloc(res->p_len, sizeof(int));
		for (int i = 0; i < k; i++)
		{
			res->coeffs[i] = coeffs[i];
			res->degrees[i] = degs[i];
		}

	}
	return res;
}
//5:mul:
Polynomial* multiplication(char *name1, char *name2)
{
	Polynomial *res = NULL;
	Polynomial* p1 = getPolynomial(name1);
	Polynomial* p2 = getPolynomial(name2);
	if (p1 == NULL)
		printf("unknown polynomial %s\n", name1);
	else if (p2 == NULL)
		printf("unknown polynomial %s\n", name2);
	else
	{
		res = (Polynomial*) malloc(sizeof(Polynomial));
		res->p_len = p1->p_len * p2->p_len;
		res->coeffs = (float*)calloc(res->p_len, sizeof(float));
		res->degrees = (int*)calloc(res->p_len, sizeof(int));

		Polynomial **polArr = malloc(p1->p_len * sizeof(Polynomial));

		for (int i = 0; i < p1->p_len; i++)
		{
			polArr[i] = (Polynomial*) malloc(sizeof(Polynomial));
			if (polArr[i] == NULL)
			{
				perror("failed to allocate memory in mul func");
				assert(polArr[i] != NULL);
			}
			
			polArr[i]->p_len = p2->p_len;
			polArr[i]->coeffs = calloc(polArr[i]->p_len, sizeof(float));
			polArr[i]->degrees = calloc(polArr[i]->p_len, sizeof(int));
			

			for (int j = 0; j < p2->p_len; j++)
			{
				if (p1->coeffs[i] == 0)
				{
					break; //todo update p_len accordingly
					polArr[i]->p_len = j;
				}
				polArr[i]->coeffs[j] = p1->coeffs[i] * p2->coeffs[j];
				polArr[i]->degrees[j] = p1->degrees[i] + p2->degrees[j];
			}
		}
		
		for (int i = 0; i < p1->p_len; i++)
		{
			res = summationByPolynomials(res, polArr[i]);
		}

		free(polArr);
	}
	
	return res;
}
//6:deriven
Polynomial* derivation(char *name)
{
	Polynomial *res = NULL;
	Polynomial* p1 = getPolynomial(name);
	if (p1 == NULL)
		printf("unknown polynomial %s\n", name);
	else
	{
		res = malloc(sizeof(Polynomial));
		if (res == NULL)
		{
			perror("failed to allocated memory in driven");
			assert(res != NULL);
		}
		res->p_len = p1->p_len;
		res->coeffs = calloc(res->p_len, sizeof(float));
		res->degrees = calloc(res->p_len, sizeof(int));
		
		for (int i = 0; i < res->p_len; i++)
		{
			res->coeffs[i] = p1->coeffs[i] * p1->degrees[i];//(i + 1);
			res->degrees[i] = p1->degrees[i] - 1;
		}
	}
	return res;
}
//7:
float evaluation(char *name, float value)
{
	float res = 0;
	Polynomial* p1 = getPolynomial(name);
	if (p1 == NULL)
		printf("unknown polynomial %s\n", name);
	else
	{
		for (int i = 0; i < p1->p_len; i++)
		{
			res += p1->coeffs[i] * pow(value, p1->degrees[i]);
		}
	}
	return res;
}
//8:
int createFromExisting(char* newName, char* pString)
{
	Polynomial *polToUpdate = getPolynomial(newName);
	Polynomial *polResult;

	if (strncmp("der ", pString, 4) == 0)
	{
		char *polName = malloc(sizeof(char)*strlen(pString - 4 + 1));
		strncpy(polName, pString + 4, strlen(pString) - 4+1);
		polResult = derivation(polName);
		free(polName);
	}
	else if (strstr(pString, "+"))
	{
		char **arr;
		split(pString, '+', &arr);
		polResult = summation(arr[0], arr[1]);
	}
	else if (strstr(pString, "-"))
	{
		char **arr;
		split(pString,'-', &arr);
		polResult = subtraction(arr[0], arr[1]);
	}
	else if (strstr(pString, "*"))
	{
		char **arr;
		split(pString, '*', &arr);
		polResult = multiplication(arr[0], arr[1]);
	}
	else
	{

		return 1;
	}
	
	if (polToUpdate == NULL)
	{
		//polResult = malloc(sizeof(Polynomial));
		polResult->name = malloc(strlen(newName) + 1);
		if (polResult->name == NULL)
		{
			perror("Failed to allocate memory in create from existing function");
			assert(polResult->name != NULL);
		}
		strcpy(polResult->name, newName);
		lastPolynomialPtr->next = polResult;
		lastPolynomialPtr = polResult;
		lastPolynomialPtr->next = NULL;
		printf("created %s\n", newName);
	}
	else
	{
		polToUpdate->p_len = polResult->p_len;
		free(polToUpdate->coeffs);
		polToUpdate->coeffs = calloc(polResult->p_len,sizeof(float));
		for (int i = 0; i < polToUpdate->p_len; i++)
		{
			polToUpdate->coeffs[i] = polResult->coeffs[i];
		}
		printf("updated %s\n", newName);
		free(polResult);
	}
	


	return 0;
}

void cleanMemory(void)
{
	Polynomial *currentPolynomial = firstPolynomialPtr;
	if (firstPolynomialPtr == NULL) //no polynomial was defined
		return;
	while (currentPolynomial != NULL)
	{
		Polynomial* tmp = currentPolynomial;
		
		free(currentPolynomial->coeffs);
		free(currentPolynomial->degrees);
		free(currentPolynomial->name);

		currentPolynomial = (*currentPolynomial).next;
		free(tmp);
	}

}
int main(void)
{
	lastPolynomialPtr = firstPolynomialPtr;

	printf("Welcome to Polynomials!\n");
	printf(" > ");
	char *command = getString(stdin, 10);

	while (strcmp(command, "quit") != 0)
	{
		//int r = strchr(cmd_char, *c);
		executeOperation(command);
		printf(" > ");
		command = getString(stdin,10);
	}
	cleanMemory();
	free(command);
	return 0;
}

