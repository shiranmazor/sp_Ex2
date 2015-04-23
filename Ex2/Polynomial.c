// Ex2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "Polynomial.h"
#include <string.h>
#include <ctype.h>

//define structures:
struct Polynomial{
	char *name;
	//the value in the fields is the coeff and the index is the degree
	float *coeffs;
};

//help function implementation:

char* getString(FILE* fp, size_t size)
/*
reading string input from user and relloc the char pointer
size - initial size of the pointer
*/
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
		exit(1);

	p = str;
	//2 - allocating arr space
	while (*p != '\0')
	{
		if (*p == c)
		{
			(*arr)[i] = (char*)malloc(sizeof(char)*token_len);
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
		return 0;
	for (int i = 0; i < strlen(name); i++)
	{
		if (isalpha(name[i]) || isdigit(name[i]))
			continue;
		else
			return 0;
	}
	//check if one of the commands are in varible name
	if (strcmp(name, "der") == 0 || strcmp(name, "eval") == 0 || strcmp(name, "quit") == 0)
		return 0;

	//name is o.k
	return 1;
}

/*check if the string that given is in the form of polynom with x varible*/
int isPolynomial(char* pString)
{
	//check if the chars - x only /x and ^/ number digit exist in the string
	//else return 0 - this mean polynom name
	if (strcmp(pString, "x") == 0 || isNumber(pString) == 1)
		return 1;
	if (strchr(pString, 'x') != NULL && strchr(pString, '^') != NULL)
		return 1;
	return 0;
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
	enum operation op;
	removeSpaces(input);

	char **arr = NULL;
	int arr_len = split(input, '=', &arr);
	//arr_len should be 2
	if (arr_len > 0)//found '=' char in command!
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
			if (isPolynomial(pString))
				createPolynomial(pName, pString);
			else
				createFromExisting(pName, pString);
		}
			
		return 1;
	}
	
}

int getPolynomial(char* name)
/*will return the position of the polynom in the database else will return -1*/
{
	return -1;
}

/*return tha maximum degree of the polynom for initializing the coeffs array size*/
int getMaxDegree(char **arrP, int arr_len)
{
	int max_deg = 0;
	int current = 0;
	for (int i = 0; i < arr_len; i++)
	{
		char* item = arrP[i];
		if (strchr(item, '^') != NULL)
		{
			char **arr = NULL;
			int arr_len = split(item, '^', &arr);
			current = *arr[arr_len - 1] - '0';
			if (current > max_deg)
				max_deg = current;
		}
	}
	return max_deg;
}

//program functions
//1:
int createPolynomial(char *name, char* polynomialStr)
{
	if (getPolynomial(polynomialStr) == -1)
	{
		//create new
		Polynomial *p = malloc(sizeof(struct Polynomial));
		p->name = malloc(strlen(name) + 1);
		strcpy(p->name, name);

		//extract coeffs and exponents
		float tmpNum = 1;
		char *newP = replace(polynomialStr, '-', "+-");
		char **arrP = NULL;
		int p_len = split(newP, '+', &arrP);
		int max_deg = getMaxDegree(arrP, p_len);

		//init coeffs array and alocate memory with zero values:
		p->coeffs = (float*)calloc(max_deg+1,sizeof(float));
		//let's fill in the coeffs array
		for (int i = 0; i < p_len; i++)
		{
			if (strchr(arrP[i], '^') != NULL)
			{
				char **tmpArr = NULL;
				int len = split(arrP[i], '^', &tmpArr);
				int index = *tmpArr[len - 1] - '0';
				if (strcmp(tmpArr[0], "x") == 0)
					p->coeffs[index] = 1;
				if (strcmp(tmpArr[0], "-x") == 0)
					p->coeffs[index] = -1;
				else
				{
					//split with x:
					char* start = strtok(*tmpArr, 'x');
					//check if negative:
					if (strchr(start, "-") != NULL)
					{
						int len = split(arrP[i], '-', &tmpArr);
						float num = (float)tmpArr[len - 1][0];
						num = num*(-1);
						p->coeffs[index] = num;
					}
					else
					{
						p->coeffs[index] = (float)*start - '0';
					}
					int i = 5;
				}
				

			}
			else
			{
				float num = atof(arrP[i]);
				if (isNumber(arrP[i]) == 1)
				{
					
					int i = 7;
				}
					
			}
		}
		/*
		float *c;
		c = (float*)malloc(sizeof(float)*2);
		c[0] = (float)3;
		c[1] = (float)4.5;
		for (int i = 0; i < 2; i++)
		{
		float u = c[i];

		}
		*/

		
	}
	else
	{
		//update existing
	}
	return 0;
}
//8:
int createFromExisting(char* newName, char* pString)
{
	return 0;
}

int main(void)
{
	Polynomial *polynomials;
	

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
	free(command);
	return 0;
}

