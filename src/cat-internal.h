#ifndef INCLUDE_WUzcop7SdJvkHfKIRl2Tqs3imQBGCLMP
#define INCLUDE_WUzcop7SdJvkHfKIRl2Tqs3imQBGCLMP 1

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>

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
	void (*test)(int *_ret_MPD8Z7);
};

struct test_each_item
{
	struct test_sort_by sort_by;
	const char *name;
	void (*func)(void);
};


#define _TEST_TO_STRING_(x) #x
#define _TEST_TO_STRING(x) _TEST_TO_STRING_(x)

#define _TEST_CASE_TO_NAME(x) test_case_ ## x

#define _TEST_BEFORE_EACH_TO_NAME(x) test_before_each_ ## x

#define _TEST_AFTER_EACH_TO_NAME(x) test_after_each_ ## x

#define _TEST_FUNC_TO_NAME(x) test_function_ ## x

#define _TEST_LOG_FAILURE(expression) \
	CAT_LOG("%s\n",_TEST_TO_STRING(expression))


#define _TESTS_REGISTER(_struct_name, _name, _test) \
static struct test_item _struct_name \
__attribute__((used, section("test_case"))) = \
{ \
	.sort_by.file.name = __FILE__, \
	.sort_by.counter = __COUNTER__, \
	.name = _TEST_TO_STRING_(_name), \
	.test = _test, \
};


#define _TEST_BEFORE_EACH_REGISTER(_struct_name, _name, _func) \
static struct test_each_item _struct_name \
__attribute__((used, section("test_before_each"))) = \
{ \
	.sort_by.file.name = __FILE__, \
	.sort_by.counter = __COUNTER__, \
	.name = _TEST_TO_STRING_(_name), \
	.func = _func, \
};


#define _TEST_AFTER_EACH_REGISTER(_struct_name, _name, _func) \
static struct test_each_item _struct_name \
__attribute__((used, section("test_after_each"))) = \
{ \
	.sort_by.file.name = __FILE__, \
	.sort_by.counter = __COUNTER__, \
	.name = _TEST_TO_STRING_(_name), \
	.func = _func, \
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
static void _TEST_CASE_TO_NAME(test_name)(int *_ret_MPD8Z7); \
_TESTS_REGISTER(test_name ## _case, test_name, \
	_TEST_CASE_TO_NAME(test_name)); \
static void _TEST_CASE_TO_NAME(test_name)(int *_ret_MPD8Z7)


#define _TEST_BEFORE_EACH(name) \
static void _TEST_BEFORE_EACH_TO_NAME(name)(void); \
_TEST_BEFORE_EACH_REGISTER(name ## _before_each, name, \
	_TEST_BEFORE_EACH_TO_NAME(name)); \
static void _TEST_BEFORE_EACH_TO_NAME(name)(void)


#define _TEST_AFTER_EACH(name) \
static void _TEST_AFTER_EACH_TO_NAME(name)(void); \
_TEST_AFTER_EACH_REGISTER(name ## _after_each, name, \
	_TEST_AFTER_EACH_TO_NAME(name)); \
static void _TEST_AFTER_EACH_TO_NAME(name)(void)


#define _TEST_FUNC(test_func_name, ...) \
static void _TEST_FUNC_TO_NAME(test_func_name)(int *_ret_MPD8Z7, \
	## __VA_ARGS__)


#define _TEST_FUNC_RUN(test_func_name, ...) \
{\
	int ret_lYTxjBSLpqAa; \
	_TEST_FUNC_TO_NAME(test_func_name)(&ret_lYTxjBSLpqAa, ## __VA_ARGS__); \
	if (ret_lYTxjBSLpqAa != TEST_PASSED) \
	{ \
		_TEST_LOG_FAILURE(test_func_name); \
		*_ret_MPD8Z7 = TEST_FAIL; \
		return; \
	} \
	*_ret_MPD8Z7 = TEST_PASSED; \
}

#define _TEST_MOCK(orig, mock) _cat_mock(orig, mock)
#define _TEST_UNMOCK(orig) _cat_unmock(orig)

#define STUB_SIZE 5+sizeof(void*)

void _cat_ptr_data_copy(unsigned char *ptr1, unsigned char *ptr2,
	size_t size);
struct mocked_func *_cat_search_mocked_func(void *addr);
int _cat_unprotect_address(void *addr);
struct mocked_func * cat_create_mocked_func(void);
void _cat_rewrite_func(void *ptr, void *dst);
void _cat_mock(void *orig, void *mock);
void _cat_unmock(void *orig);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_WUzcop7SdJvkHfKIRl2Tqs3imQBGCLMP */
