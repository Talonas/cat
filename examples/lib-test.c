#include <stdlib.h>

#include "cat.h"
#include "lib.h"


/**
 * Calls "lib_get_number()" and checks returned value.
 * It can be called using "CAT_FUNC_RUN(lib_get_number);"
 */
CAT_FUNC(lib_get_number)
{
	int ret = lib_get_number();
	CAT_ASSERT(ret == 11);
}

/**
 * Compares int values "a" and "b". Test will pass when they are equal.
 * Otherwise, the test will fail.
 * Can be called using "CAT_FUNC_RUN(compare_numbers, 1, 1);"
 */
CAT_FUNC(compare_numbers, int a, int b)
{
	CAT_ASSERT(a == b);
}


CAT_CASE(sample1)
{
	CAT_ASSERT(1);
}

CAT_CASE(sample2)
{
	CAT_ASSERT(1 == 1);
}

CAT_CASE(sample3)
{
	CAT_FUNC_RUN(lib_get_number);
}

CAT_CASE(sample4)
{
	CAT_FUNC_RUN(compare_numbers, 9, 9);
}

CAT_CASE(lib_get_number_should_return_11)
{
	int ret = lib_get_number();
	CAT_ASSERT(ret == 11);
}

CAT_CASE(lib_get_number_should_be_in_range)
{
	int ret = lib_get_number();
	CAT_ASSERT(ret < 12);
	CAT_ASSERT(ret > 10);
	CAT_ASSERT(ret == 11);
}

CAT_CASE(lib_set_str_should_return_TEST)
{
	char str[5];
	int ret;

	ret = lib_set_str(str, sizeof(str));
	CAT_ASSERT(ret == 0);
	ret = strcmp(str, "TEST");
	CAT_ASSERT(ret == 0);
}

CAT_CASE(lib_set_str_should_not_be_FEST)
{
	char str[5];
	int ret;

	ret = lib_set_str(str, sizeof(str));
	CAT_ASSERT(ret == 0);
	ret = strcmp(str, "FEST");
	CAT_ASSERT(ret != 0);
}
