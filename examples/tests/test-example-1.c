#include "cat.h"


CAT_CASE(basic_assert)
{
	CAT_ASSERT(1);
}

CAT_CASE(compare_two_numbers)
{
	CAT_ASSERT(1 == 1);
}

CAT_CASE(compare_two_strings)
{
	CAT_ASSERT_STR_EQUAL("test str", "test str");
}

CAT_CASE(compare_two_non_equal_strings)
{
	CAT_ASSERT_STR_NOT_EQUAL("test str 1", "test str 2");
}

CAT_CASE(assert_pointer_not_null)
{
	void *ptr = "str";
	CAT_ASSERT_NOT_NULL(ptr);
}

CAT_CASE(assert_null_pointer)
{
	void *ptr = NULL;
	CAT_ASSERT_NULL(ptr);
}

CAT_CASE(skip_this_test_on_condition)
{
	int cond = 1;

	CAT_SKIP(cond == 1);
	/* this section should be not reached */
	CAT_ASSERT(1 == 2);
}
