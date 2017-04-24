# CAT

[![CAT Build Status](https://travis-ci.org/Talonas/cat.svg?branch=master)](https://travis-ci.org/Talonas/cat)

A simple and lightweight C test framework.

### Asserts

Assert | Description
-------|------------
CAT_ASSERT(expression) | Asserts that condition is true
CAT_ASSERT_STR_EQUAL(actual, expected) | Asserts that _actual_ equals to _expected_
CAT_ASSERT_NOT_NULL(ptr) | Asserts that _ptr_ is not NULL
CAT_ASSERT_NULL(ptr) | Asserts that _ptr_ is NULL

### Functions

Function | Description
-------|------------
CAT_CASE(name) | Registers a test case
CAT_FUNC(name, args) | Declares a test function that can be called using `CAT_FUNC_RUN(name)`
CAT_FUNC_RUN(name, args) | Calls a declared `CAT_FUNC` by name

### Samples
```c
#include "cat.h"

CAT_FUNC(simple_func)
{
	CAT_LOG("simple_func is invoked!\n");
	CAT_ASSERT(1);
}

CAT_FUNC(compare_numbers, int a, int b)
{
	CAT_ASSERT(a == b);
}

CAT_CASE(test1)
{
	CAT_ASSERT(1);
}

CAT_CASE(test2)
{
	CAT_FUNC_RUN(simple_func);
	CAT_ASSERT(1);
}

CAT_CASE(test3)
{
	CAT_ASSERT_STR_EQUAL("TEST", "TEST");
}

CAT_CASE(test4)
{
	CAT_FUNC_RUN(compare_numbers, 2, 2);
	CAT_ASSERT(1);
}
```

Output should be similar to

```bash
Running test cases
  - [PASS] test1
lib-test.c test_function_simple_func:8: simple_func is invoked!
  - [PASS] test2
  - [PASS] test3
  - [PASS] test4

Summary   
	TOTAL      PASSED     FAILED     UNKNOWN   
	4          4          0          0         

Time elapsed: 0.000128 seconds
```
