#include <stdlib.h>

#include "cat.h"
#include "prod.h"


CAT_CASE(get_number_seven)
{
	int ret = prod_get_number_seven();
	CAT_ASSERT(ret == 7);
}

CAT_CASE(test_7x7_should_be_49)
{
	int ret = prod_multiple_numbers(7, 7);
	CAT_ASSERT(ret == 49);
}

CAT_CASE(merge_strings_success)
{
	char *new_str = NULL;

	new_str = prod_merge_two_strings("test", "string", " ");
	CAT_ASSERT_NOT_NULL(new_str);
	CAT_ASSERT_STR_EQUAL(new_str, "test string");

	free(new_str);
	new_str = NULL;
}

CAT_CASE(merge_strings_separator_null)
{
	/* this test case will crash */
	char *new_str = NULL;

	new_str = prod_merge_two_strings("test", "string", NULL);
	CAT_ASSERT_NOT_NULL(new_str);
	CAT_ASSERT_STR_EQUAL(new_str, "teststring");

	free(new_str);
	new_str = NULL;
}

CAT_CASE(compose_numbers)
{
	int ret = prod_compose_numbers(2, 2);
	CAT_ASSERT(ret == 4);
}
