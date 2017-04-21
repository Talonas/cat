#ifndef INCLUDE_WUzcop7SdJvkHfKIRl2Tqs3imQBGCLMP
#define INCLUDE_WUzcop7SdJvkHfKIRl2Tqs3imQBGCLMP 1

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
#include <stdint.h>

enum test_return
{
	TEST_PASSED = 0xad,
	TEST_FAIL = 0xbd,
	TEST_SKIP = 0xcd
};

struct test_sort_by
{
	union
	{
		const char *name;
		/* some padding for 32bit systems */
		char _dummy0[8];
	} file;
	uint64_t counter;
};

struct test_item
{
	struct test_sort_by sort_by;
	const char *name;
	enum test_return (*test)(void);
};


#define _TEST_TO_STRING_(x) #x
#define _TEST_TO_STRING(x) _TEST_TO_STRING_(x)

#define _TEST_CASE_TO_NAME(x) test_case_ ## x

#define _TEST_FUNC_TO_NAME(x) test_function_ ## x

#define _TEST_LOG_FAILURE(expression) \
	fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, \
		_TEST_TO_STRING(expression))


#define _TESTS_REGISTER(_struct_name, _name, _test) \
static struct test_item _struct_name \
__attribute__((used, section("test_case"))) = \
{ \
	.sort_by.file.name = __FILE__, \
	.sort_by.counter = __COUNTER__, \
	.name = _TEST_TO_STRING_(_name), \
	.test = _test, \
};


#define _FOREACH_TEST(item, section, type_t) \
	for (item = \
	({ \
		extern type_t __start_ ## section; \
		&__start_ ## section; \
	}); \
	item != \
	({ \
		extern type_t __stop_ ## section; \
		&__stop_ ## section; \
	}); \
	item++) \


#define _TEST_CASE(test_name) \
static enum test_return _TEST_CASE_TO_NAME(test_name)(void); \
_TESTS_REGISTER(test_name ## _case, test_name, \
	_TEST_CASE_TO_NAME(test_name)); \
static enum test_return _TEST_CASE_TO_NAME(test_name)(void)


#define _TEST_FUNC(test_func_name) \
static enum test_return _TEST_FUNC_TO_NAME(test_func_name)(void)


#define _TEST_FUNC_ARG(test_func_name, arguments) \
static enum test_return _TEST_FUNC_TO_NAME(test_func_name) arguments


#define _TEST_FUNC_RUN(test_func_name) \
{ \
	enum test_return _TEST_FUNC_TO_NAME(test_func_name)(void); \
	{ \
		enum test_return ret; \
		ret = _TEST_FUNC_TO_NAME(test_func_name)(); \
		if (ret != TEST_PASSED) \
		{ \
			_TEST_LOG_FAILURE(test_func_name); \
			return TEST_FAIL; \
		} \
	} \
}

#define _TEST_FUNC_ARG_RUN(test_func_name, arguments) \
{\
	/* anoying variable name to avoid name hiding */ \
	enum test_return ret_lYTxjBSLpqAa; \
	ret_lYTxjBSLpqAa = _TEST_FUNC_TO_NAME(test_func_name) arguments; \
	if (ret_lYTxjBSLpqAa != TEST_PASSED) \
	{ \
		_TEST_LOG_FAILURE(test_func_name); \
		return TEST_FAIL; \
	} \
}



#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_WUzcop7SdJvkHfKIRl2Tqs3imQBGCLMP */
