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
	float *coeffs;
	unsigned *degrees;
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

//program functions
//1:
int createPolynomial(char *name, char* polynomialStr)
{
	if (getPolynomial(polynomialStr) == -1)
	{
		//create new
		/*
			char *name;
	float *coeffs;
	unsigned *degrees;
	*/
		Polynomial *p = malloc(sizeof(struct Polynomial));
		p->name = malloc(strlen(name) + 1);
		float* coeffs;
		unsigned* degrees;
		strcpy(p->name, name);
		//extract coeffs and exponents
		float tmpNum = 1;
		for (int i = 0; i < strlen(polynomialStr); i++)
		{
			if (strcmp(polynomialStr[i], "x") == 0)
			{
				if (strcmp(polynomialStr[i + 1], "^") == 0 && isNumber(polynomialStr[i + 2]))
			}
		}

		
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

