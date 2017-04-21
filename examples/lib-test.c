#include <stdlib.h>

#include "cat.h"
#include "lib.h"


CAT_CASE(sample1)
{
	CAT_ASSERT_FINAL(1);
}

CAT_CASE(sample2)
{
	CAT_ASSERT(10 > 1);
	CAT_ASSERT_FINAL(1);
}

CAT_CASE(lib_get_number_should_return_11)
{
	int ret;
	ret = lib_get_number();
	CAT_ASSERT_FINAL(ret == 11);
}

CAT_CASE(lib_get_number_should_be_in_range)
{
	int ret;
	ret = lib_get_number();
	CAT_ASSERT(ret < 12);
	CAT_ASSERT(ret > 10);
	CAT_ASSERT_FINAL(ret == 11);
}

CAT_CASE(lib_set_str_should_return_TEST)
{
	char str[5];
	int ret;

	ret = lib_set_str(str, sizeof(str));
	CAT_ASSERT(ret == 0);
	ret = strcmp(str, "TEST");
	CAT_ASSERT_FINAL(ret == 0);
}

CAT_CASE(lib_set_str_should_not_be_FEST)
{
	char str[5];
	int ret;

	ret = lib_set_str(str, sizeof(str));
	CAT_ASSERT(ret == 0);
	ret = strcmp(str, "FEST");
	CAT_ASSERT_FINAL(ret != 0);
}
