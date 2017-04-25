#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "prod.h"


/**
 * Badly written function which has no args check and it leads to
 * possible crash.
 */
char *
prod_merge_two_strings(const char *str1, const char *str2,
	const char *separator)
{
	char *new_str = NULL;
	size_t new_str_len;


	new_str_len = strlen(str1) + strlen(str2) + strlen(separator) + 1;
	new_str = malloc(new_str_len);

	sprintf(new_str, "%s%s%s", str1, separator, str2);

	new_str[new_str_len] = '\0';

	return new_str;
}

int
prod_get_number_seven(void)
{
	return 7;
}

int
prod_multiple_numbers(int a, int b)
{
	return a * b;
}

/**
 * A function with defect =]
 */
int
prod_compose_numbers(int a, int b)
{
	return a + b - 1;
}
