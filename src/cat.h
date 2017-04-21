#ifndef INCLUDE_wSJuEUMYxZXqHOg1IWKihrfRB7TkGnsl
#define INCLUDE_wSJuEUMYxZXqHOg1IWKihrfRB7TkGnsl 1

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "cat-internal.h"


#define CAT_LOG printf("%s %s:%d: ", __FILE__, __FUNCTION__, \
	__LINE__); printf


#define CAT_ASSERT(expression) \
if (!(expression)) \
{ \
	_TEST_LOG_FAILURE(CAT_ASSERT(expression)); \
	return TEST_FAIL; \
}


#define CAT_ASSERT_FINAL(expression) \
if (expression) \
{ \
	return TEST_PASSED; \
} \
else \
{ \
	_TEST_LOG_FAILURE(CAT_ASSERT_FINAL(expression)); \
	return TEST_FAIL; \
}


#define CAT_CASE(test_name) _TEST_CASE(test_name)

#define CAT_FUNC(test_func_name) _TEST_FUNC(test_func_name)
#define CAT_FUNC_ARGS(test_func_name, arguments) _TEST_FUNC_ARG(\
	test_func_name, arguments)

#define CAT_FUNC_RUN(test_func_name) _TEST_FUNC_RUN(test_func_name)
#define CAT_FUNC_ARGS_RUN(test_func_name, arguments) \
	_TEST_FUNC_ARG_RUN(test_func_name, arguments)


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_wSJuEUMYxZXqHOg1IWKihrfRB7TkGnsl */
