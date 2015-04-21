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
	unsigned *degress;
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

int executeOperation(char* input)
{
	enum operation op;
	//remove spaces	
	removeSpaces(input);

	char **arr = NULL;
	int arr_len = split(input, '=', &arr);
	//arr_len should be 2
	if (arr_len > 0)//found '=' char in command!
	{	
		char* pName = arr[0];
		if (!checkPName(pName))
		{
			printf("illegal variable name\n");
			return 0;
		}
		else
			createPolynomial(pName, arr[1]);
		return 1;
	}
	int arr_len = split(input, '+', &arr);
	if (arr_len > 0)
	{
		char* p1 = arr[0];
		char* p2 = arr[1];

	}
	
	
}

//program functions
int createPolynomial(char *name)
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

