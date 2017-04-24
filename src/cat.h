#ifndef INCLUDE_wSJuEUMYxZXqHOg1IWKihrfRB7TkGnsl
#define INCLUDE_wSJuEUMYxZXqHOg1IWKihrfRB7TkGnsl 1

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "cat-internal.h"


#define CAT_LOG printf("%s %s:%d ", __FILE__, __FUNCTION__, \
	__LINE__); printf


#define CAT_ASSERT(expression) \
if (!(expression)) \
{ \
	_TEST_LOG_FAILURE(CAT_ASSERT(expression)); \
	*_ret_MPD8Z7 = TEST_FAIL; \
	return; \
} \
else \
{ \
	*_ret_MPD8Z7 = TEST_PASSED; \
}


#define CAT_CASE(test_name) _TEST_CASE(test_name)

#define CAT_FUNC(test_func_name, ...) _TEST_FUNC(\
	test_func_name, ## __VA_ARGS__)

#define CAT_FUNC_RUN(test_func_name, ...) \
	_TEST_FUNC_RUN(test_func_name, ## __VA_ARGS__)


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_wSJuEUMYxZXqHOg1IWKihrfRB7TkGnsl */
