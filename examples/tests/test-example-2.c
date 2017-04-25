#include "cat.h"

/*
 * Declare test functions
 */

CAT_FUNC(sample_func1)
{
	CAT_ASSERT(1);
}

CAT_FUNC(compare_numbers, int a, int b)
{
	CAT_ASSERT(a == b);
}

CAT_FUNC(open_non_existing_file, const char *filename)
{
	FILE *f = fopen(filename, "r");
	CAT_ASSERT_NULL(f);
}


/*
 * Declare test cases
 */

CAT_CASE(use_simple_test_func)
{
	CAT_FUNC_RUN(sample_func1);
}

CAT_CASE(use_test_func_for_compare_numbers)
{
	CAT_FUNC_RUN(compare_numbers, 1, 1);
}

CAT_CASE(open_non_existing_file)
{
	CAT_FUNC_RUN(open_non_existing_file, "a");
}
