#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LEN_MAX 1000

int main()
{
	// Input file opening read mode
	FILE* input;
	fopen_s(&input, "input.txt", "r");
	if (!input) exit(EXIT_FAILURE);

	// Output file openning write mode
	FILE* output;
	fopen_s(&output, "output.txt", "w");
	if (!output) exit(EXIT_FAILURE);

	// Buffer string
	fseek(input, 0, SEEK_SET);
	char* string = (char*)calloc(STR_LEN_MAX, sizeof(char));
	if (!string) exit(EXIT_FAILURE);

	// Read from input file
	while (!feof(input))
	{
		fgets(string, STR_LEN_MAX, input);
		StackPush(string);
		memset(string, 0, STR_LEN_MAX);
	}

	// Write in output file
	while (!StackIsEmpty())
	{
		fputs(StackGet(), output);
		StackPop();
	}

	// Free memory
	StackDestroy();
	free(string);
	fclose(input);
	fclose(output);
	return 0;
}
