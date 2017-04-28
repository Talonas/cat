# CAT

[![CAT Build Status](https://travis-ci.org/Talonas/cat.svg?branch=master)](https://travis-ci.org/Talonas/cat)

CAT is a lightweight and simple framework for writing tests in C. __CAT__ can be interpreted
as C Awesome Tests, C Automated Tests, cat or whatever you like.

## Supported Platforms

- __Linux__ with a gcc
- __Mac OS X__ - not supported
- __Windows__ - not supported

## Declaring Test Case
```c
CAT_CASE(case)
{
	...
}
```

## Test Skip
```c
CAT_SKIP(expression)
```
Skips the test if evaluates expression.

## Assertions
```c
CAT_ASSERT(expression)
```
Asserts that _condition_ is true. Otherwise the test case will fail

```c
CAT_ASSERT_NULL(ptr)
```
Asserts that _ptr_ is NULL

```c
CAT_ASSERT_NOT_NULL(ptr)
```
Asserts that _ptr_ is not NULL

```c
CAT_ASSERT_STR_EQUAL(actual, expected)
```
Asserts that strings _actual_ equals to _expected_

```c
CAT_ASSERT_STR_NOT_EQUAL(str1, str2)
```
Compares _str1_ with _str2_. Passes when they are not equal. Otherwise the test will fail.

## Functions

```c
CAT_FUNC(name, args)
{
	...
}
```
Declares a test function which name is the first argument _name_. Function will pass if all expressions inside will be passed.
Otherwise, it will fail and the test case will return failure.

```c
CAT_FUNC_RUN(name, args)
```
Calls a test function by _name_

## Usage

### Build

Just create a sample test source file _test.c_
```C
#include "cat.h"

CAT_CASE(test)
{
	CAT_ASSERT(1);
}
```

Then make a executable test using gcc
```bash
gcc -I./src test.c ./src/cat.c -o test
```

Check if it is working
```bash
./test -a

Running test cases
  - [PASS] test

Summary   
	TOTAL      PASSED     FAILED     SKIPPED    UNKNOWN   
	1          1          0          0          0         

Time elapsed: 0.000127 seconds

==================== SUCCESS ====================
```

### Run Options
```bash
Usage: ./test [OPTIONS] [TEST, ...]

Options:
  -h        Prints this help
  -l        Lists all declared unit tests
  -a        Run all tests
  -p        Run tests in single process
```

## Samples

### Basic
```c
#include "cat.h"

CAT_CASE(test1)
{
	CAT_ASSERT(1);
}

CAT_CASE(test2)
{
	void *ptr = malloc(1);
	CAT_ASSERT_NOT_NULL(ptr);
}
```

### Using Test Functions
```c
#include "cat.h"

CAT_FUNC(compare, int a, int b)
{
	CAT_ASSERT(a == b);
}

CAT_CASE(test1)
{
	CAT_FUNC_RUN(compare, 1, 2);
}
```
