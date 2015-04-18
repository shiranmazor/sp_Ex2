// Ex2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "Polynomial.h"
#include <string.h>

//test
struct Polynomial{
	char *name;
	float *coeffs;
	unsigned *degress;
};


int createPolynomial(char *name)
{
	return 0;
}

char* initCommands(void)
{
	char *commands[10];
	commands[0] = "=";
	commands[1] = "+";
	commands[2] = "-";
	commands[3] = "*";
	commands[4] = "der";
	commands[5] = "eval";
	commands[6] = "quit";
	return commands;
}

int main(void)
{
	Polynomial *polynomials;
	//init commands
	const char *commands = initCommands();

	printf("Welcome to Polynomials!\n");
	printf(" > ");
	char *command = calloc(20, sizeof(char));
	scanf("%s", command);
	while (strcmp(command, "quit") != 0)
	{
		char *c = command;
		const char *cmd_char = commands;
		int r = strchr(cmd_char, *c);
		printf(" > ");
		scanf("%s", command);
	}
	free(command);
	return 0;
}

