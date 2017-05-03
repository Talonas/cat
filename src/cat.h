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


#define CAT_ASSERT_STR_EQUAL(a, b) \
int ret = strcmp(a, b); \
if (ret != 0) \
{ \
	_TEST_LOG_FAILURE(CAT_ASSERT_STR_EQUAL(a, b)); \
	*_ret_MPD8Z7 = TEST_FAIL; \
	return; \
} \
else \
{ \
	*_ret_MPD8Z7 = TEST_PASSED; \
}

#define CAT_ASSERT_STR_NOT_EQUAL(a, b) \
int ret = strcmp(a, b); \
if (ret == 0) \
{ \
	_TEST_LOG_FAILURE(CAT_ASSERT_STR_NOT_EQUAL(a, b)); \
	*_ret_MPD8Z7 = TEST_FAIL; \
	return; \
} \
else \
{ \
	*_ret_MPD8Z7 = TEST_PASSED; \
}


#define CAT_ASSERT_NOT_NULL(ptr) \
if (ptr == NULL) \
{ \
	_TEST_LOG_FAILURE(CAT_ASSERT_NOT_NULL(ptr)); \
	*_ret_MPD8Z7 = TEST_FAIL; \
	return; \
} \
else \
{ \
	*_ret_MPD8Z7 = TEST_PASSED; \
}


#define CAT_ASSERT_NULL(ptr) \
if (ptr != NULL) \
{ \
	_TEST_LOG_FAILURE(CAT_ASSERT_NULL(ptr)); \
	*_ret_MPD8Z7 = TEST_FAIL; \
	return; \
} \
else \
{ \
	*_ret_MPD8Z7 = TEST_PASSED; \
}


#define CAT_SKIP(expression) \
if (expression) \
{ \
	*_ret_MPD8Z7 = TEST_SKIP; \
	return; \
} \
else \
{ \
	*_ret_MPD8Z7 = TEST_PASSED; \
}

#define CAT_CASE(test_name, ...) _TEST_CASE(test_name, __VA_ARGS__)

#define CAT_BEFORE_EACH(name) _TEST_BEFORE_EACH(name)

#define CAT_AFTER_EACH(name) _TEST_AFTER_EACH(name)

#define CAT_FUNC(test_func_name, ...) _TEST_FUNC(\
	test_func_name, ## __VA_ARGS__)

#define CAT_FUNC_RUN(test_func_name, ...) \
	_TEST_FUNC_RUN(test_func_name, ## __VA_ARGS__)

#define CAT_MOCK(orig, mock) _TEST_MOCK(orig, mock)

#define CAT_UNMOCK(orig) _TEST_UNMOCK(orig)

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_wSJuEUMYxZXqHOg1IWKihrfRB7TkGnsl */
