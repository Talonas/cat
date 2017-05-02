#include "cat.h"

#include "../src/prod.c"

int
prod_multiple_numbers_mock(int a, int b)
{
	(void)a;
	(void)b;
	return -1;
}

int
prod_static_return_7_mock(void)
{
	return 777;
}

CAT_FUNC(prod_multiple_numbers, int a, int b, int exp)
{
	int ret = prod_multiple_numbers(a, b);
	CAT_ASSERT(ret == exp);
}

CAT_FUNC(prod_static_return_7, int exp)
{
	int ret = prod_static_return_7();
	CAT_ASSERT(ret == exp);
}


CAT_CASE(mock_sample)
{
	/* call non mocked function. Should return 4 */
	CAT_FUNC_RUN(prod_multiple_numbers, 2, 2, 4);

	/* mock function */
	CAT_MOCK(prod_multiple_numbers, prod_multiple_numbers_mock);
	/* mocked func should return -1 */
	CAT_FUNC_RUN(prod_multiple_numbers, 2, 2, -1);

	/* undo mocked func */
	CAT_UNMOCK(prod_multiple_numbers);
	/* original func should return 4 */
	CAT_FUNC_RUN(prod_multiple_numbers, 2, 2, 4);
}

CAT_CASE(mock_static_func)
{
	/* static func should return 7 */
	CAT_FUNC_RUN(prod_static_return_7, 7);

	/* mock static func */
	CAT_MOCK(prod_static_return_7, prod_static_return_7_mock);
	/* mocked func should return 777 */
	CAT_FUNC_RUN(prod_static_return_7, 777);

	/* undo mock */
	CAT_UNMOCK(prod_static_return_7);
	/* original func should return 7 */
	CAT_FUNC_RUN(prod_static_return_7, 7);
}
