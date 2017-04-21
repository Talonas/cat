#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lib.h"

int
lib_init(void)
{
	return 0;
}

void
lib_deinit(void)
{
	return;
}

int
lib_get_number(void)
{
	if (1 == 2)
	{
		printf("Hello!\n");
	}
	return 11;
}

int
lib_set_str(char *val, size_t val_size)
{
	const char *name = "TEST";
	int retval = -1;


	if (val_size == 0)
	{
		val_size = 199;
		goto done;
	}

	memset(val, 0, val_size);
	memcpy(val, name, strlen(name));

	retval = 0;
done:
	return retval;
}
