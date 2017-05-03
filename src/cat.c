#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "cat.h"

#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[2;32m"
#define COLOR_GREEN_BOLD "\033[1;32m"
#define COLOR_LIGHT_CYAN "\033[1;36m"
#define COLOR_YELLOW "\033[1;93m"
#define COLOR_NORMAL "\033[0m"

enum final_result_type {
	RESULT_TYPE_PASS = 0,
	RESULT_TYPE_FAIL,
	RESULT_TYPE_SKIP,
	RESULT_TYPE_UNKNOWN,
	RESULT_TYPE_COUNT,
};

static struct
{
	int results[RESULT_TYPE_COUNT];
	int single_process;
	char *name;
	struct timeval time_elapsed;
} state;

struct test_result
{
	const char *color;
	const char *message;
	const struct test_item *item;
};

struct mocked_func
{
	struct mocked_func *next;
	void *addr;
	unsigned char addr_orig[STUB_SIZE];
};

static int test_run_in_process(const struct test_item *item);
static void test_case_run(const struct test_item *item);
static void test_run_all(void);
static void display_summary(void);
static void help(void);
static const struct test_item *test_search(const char *name);
static void show_tests(void);
static void deinit_mocked_functions(void);
static void run_before_each(void);
static void run_after_each(void);
static void test_run_suites(char **suites, int count);
static void run_test_suite(const char *suite);
static void test_run_tests(char **tests, int count);

static struct mocked_func *mocked_func = NULL;


void
_cat_ptr_data_copy(unsigned char *ptr1, unsigned char *ptr2, size_t size)
{
	size_t i;
	for (i = 0; i < size; i++)
	{
		ptr1[i] = ptr2[i];
	}
}

struct mocked_func *
_cat_search_mocked_func(void *addr)
{
	struct mocked_func *retval = NULL;
	struct mocked_func *mf = NULL;
	int ret;


	mf = mocked_func;

	while (1)
	{
		if (mf == NULL)
		{
			break;
		}

		ret = memcmp(mf->addr, addr, STUB_SIZE);
		if (ret == 0)
		{
			retval = mf;
			goto done;
		}

		mf = mf->next;
	}
done:
	return retval;
}

int
_cat_unprotect_address(void *addr)
{
	int retval = -1;
	int ret;
	unsigned char *pg;
	long psize;


	pg = (unsigned char *)(addr - ((size_t)addr % 4096));

	ret = mprotect(pg, 128, PROT_READ | PROT_WRITE | PROT_EXEC);
	if (ret != 0)
	{
		fprintf(stderr, "ERROR: mprotect() failed: %d: %d: %sn", ret,
			errno, strerror(errno));
		goto done;
	}

	psize = sysconf(_SC_PAGESIZE);
	ret = mprotect(pg + psize, 128, PROT_WRITE|PROT_READ|PROT_EXEC);
	if (ret != 0)
	{
		fprintf(stderr, "ERROR: mprotect() failed: %d: %d: %sn", ret,
			errno, strerror(errno));
		goto done;
	}

	retval = 0;
done:
	return retval;
}

struct mocked_func *
cat_create_mocked_func(void)
{
	struct mocked_func *retval = NULL;
	struct mocked_func *mf = NULL;


	mf = malloc(sizeof(*mf));
	if (mf == NULL)
	{
		fprintf(stderr, "malloc failed\n");
		goto done;
	}

	mf->next = mocked_func;
	mocked_func = mf;

	retval = mf;
done:
	return retval;
}

void
_cat_rewrite_func(void *ptr, void *dst)
{
#ifdef __amd64__
	unsigned char jumpf[16] = {
		0x48, 0xb8,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x50, 0xc3,
	};
	unsigned char *addr = jumpf + 2;
	(*(void **)(addr)) = dst;
	_cat_ptr_data_copy(ptr, jumpf, STUB_SIZE);
#elif __i386__
#define HIJACK_ADDR(x) *(void**)((x) + 1)
	static const char hijack_stub[8] = {
		0x68, 0x00, 0x00, 0x00, 0x00, 0xc3
	};
	_cat_ptr_data_copy(ptr, hijack_stub, STUB_SIZE);
	HIJACK_ADDR(addr) = dst;
#endif
}

void
_cat_mock(void *orig, void *mock)
{
	int ret;
	struct mocked_func *mf = NULL;


	mf = _cat_search_mocked_func(orig);
	if (mf == NULL)
	{
		mf = cat_create_mocked_func();
		if (mf == NULL)
		{
			goto done;
		}
	}
	else
	{
		if (memcmp(mock, mf->addr_orig, STUB_SIZE) == 0)
		{
			goto done;
		}
	}

	ret = _cat_unprotect_address(orig);
	if (ret != 0)
	{
		goto done;
	}

	_cat_ptr_data_copy(mf->addr_orig, orig, STUB_SIZE);
	_cat_rewrite_func(orig, mock);
	mf->addr = orig;

done:
	return;
}

void
_cat_unmock(void *orig)
{
	struct mocked_func *mf = NULL;

	mf = _cat_search_mocked_func(orig);
	if (mf == NULL)
	{
		goto done;
	}

	if (mf->addr == NULL)
	{
		goto done;
	}

	_cat_ptr_data_copy(mf->addr, mf->addr_orig, STUB_SIZE);
	memset(mf->addr_orig, 0, STUB_SIZE);
done:
	return;
}


static int
test_run_in_process(const struct test_item *item)
{
	int ret;
	pid_t pid;
	pid_t retp;


	pid = fork();
	if (pid == 0)
	{
		/* child */
		run_before_each();
		item->test(&ret);
		run_after_each();
		deinit_mocked_functions();
		exit(ret);
	}

	if (pid == -1)
	{
		/* somehow fork failed, run test local'y */
		item->test(&ret);
	}
	else
	{
		/* parent with running child */
		retp = waitpid(pid, &ret, 0);
		if (retp != pid)
		{
			fprintf(stderr, "error: failed waitpid(%d) with %d, %d\n",
				pid, retp, errno);
			/* return value is uknown */
			ret = 1;
		}
		else if (WIFEXITED(ret))
		{
			/* exit normaly. */
			ret = WEXITSTATUS(ret);
		}
		else
		{
			/* exit abnormal */
			ret |= 0x5A000000;
		}
	}

	return ret;
}

static void
run_before_each(void)
{
	const struct test_each_item *item = NULL;

	_FOREACH_TEST(item, test_before_each, struct test_each_item)
	{
		item->func();
	}
}

static void
run_after_each(void)
{
	const struct test_each_item *item = NULL;

	_FOREACH_TEST(item, test_after_each, struct test_each_item)
	{
		item->func();
	}
}

static void
test_case_run(const struct test_item *item)
{
	struct test_result result;
	int ret;


	if (state.single_process == 1)
	{
		run_before_each();
		item->test(&ret);
		run_after_each();
	}
	else
	{
		ret = test_run_in_process(item);
	}

	switch (ret)
	{
	case TEST_PASSED:
		result.message = "PASS";
		result.color = COLOR_GREEN;
		state.results[RESULT_TYPE_PASS]++;
		break;
	case TEST_FAIL:
		result.message = "FAIL";
		result.color = COLOR_RED;
		state.results[RESULT_TYPE_FAIL]++;
		break;
	case TEST_SKIP:
		result.message = "SKIP";
		result.color = COLOR_YELLOW;
		state.results[RESULT_TYPE_SKIP]++;
		break;
	default:
		result.message = "UNKNOWN";
		result.color = COLOR_RED;
		state.results[RESULT_TYPE_UNKNOWN]++;
		break;
	}

	printf("  - %s[%s]%s %s\n", result.color, result.message,
		COLOR_NORMAL, item->name);
	fflush(stdout);
}

static void
test_run_all(void)
{
	const struct test_item *item = NULL;
	struct timeval start;
	struct timeval end;

	printf("\nRunning test cases\n");

	gettimeofday(&start, NULL);

	_FOREACH_TEST(item, test_case, struct test_item)
	{
		test_case_run(item);
	}

	gettimeofday(&end, NULL);
	timersub(&end, &start, &state.time_elapsed);

	deinit_mocked_functions();
}

static void
test_run_tests(char **tests, int count)
{
	int i;
	const struct test_item *test = NULL;
	struct timeval start;
	struct timeval end;


	gettimeofday(&start, NULL);

	for (i = 0; i < count; i++)
	{
		test = test_search(tests[i]);
		if (test == NULL)
		{
			printf("Test \"%s\" can't be found\n", tests[i]);
			continue;
		}
		test_case_run(test);
	}

	gettimeofday(&end, NULL);
	timersub(&end, &start, &state.time_elapsed);
}

static void
test_run_suites(char **suites, int count)
{
	int i;
	struct timeval start;
	struct timeval end;

	gettimeofday(&start, NULL);

	for (i = 0; i < count; i++)
	{
		run_test_suite(suites[i]);
	}

	gettimeofday(&end, NULL);
	timersub(&end, &start, &state.time_elapsed);
}

static void
display_summary(void)
{
	int total = 0;
	char *color = COLOR_GREEN_BOLD;
	char *message = "PASSED";


	total = state.results[RESULT_TYPE_PASS] +
		state.results[RESULT_TYPE_FAIL] +
		state.results[RESULT_TYPE_SKIP] +
		state.results[RESULT_TYPE_UNKNOWN];

	printf("\n%-10s\n\t%-10s %-10s %-10s %-10s %-10s\n"
		"\t%-10d %-10d %-10d %-10d %-10d\n",
		"Summary", "TOTAL", "PASSED", "FAILED", "SKIPPED", "UNKNOWN",
		total,
		state.results[RESULT_TYPE_PASS],
		state.results[RESULT_TYPE_FAIL],
		state.results[RESULT_TYPE_SKIP],
		state.results[RESULT_TYPE_UNKNOWN]
	);

	printf("\nTime elapsed: %ld.%06ld seconds\n\n",
		(long int)state.time_elapsed.tv_sec,
		(long int)state.time_elapsed.tv_usec);

	if (state.results[RESULT_TYPE_FAIL] != 0 ||
	    state.results[RESULT_TYPE_UNKNOWN] != 0)
	{
		color = COLOR_RED;
		message = "FAILED";
	}

	printf("%s========================= %s "
		"==========================%s\n",
		color, message, COLOR_NORMAL);
}

static void
help(void)
{
	printf("Usage: %s [OPTIONS] [TEST, ...]\n"
		"\n"
		"Options:\n"
		"  -h                Prints this help\n"
		"  -l                Lists all declared unit tests\n"
		"Run options:\n"
		"  -a                Run all tests\n"
		"  -p                Run tests in single process\n"
		"  -s [SUITE, ...]   Run test suites\n"
		"", state.name);
	exit(1);
}

static const struct test_item *
test_search(const char *name)
{
	const struct test_item *item = NULL;
	const struct test_item *retval = NULL;
	int ret;


	_FOREACH_TEST(item, test_case, struct test_item)
	{
		ret = strcmp(name, item->name);
		if (ret == 0)
		{
			retval = item;
			break;
		}
	}

	return retval;
}

static void
show_tests(void)
{
	const struct test_item *item = NULL;
	int ret;


	printf("Tests:\n");

	_FOREACH_TEST(item, test_case, struct test_item)
	{
		ret = strlen(item->suite);
		if (ret != 0)
		{
			printf("  - %s [%s]\n", item->name, item->suite);
		}
		else
		{
			printf("  - %s []\n", item->name);
		}
	}
}

static void
deinit_mocked_functions(void)
{
	struct mocked_func *mf = mocked_func;
	struct mocked_func *mf_next;

	while (mf != NULL)
	{
		mf_next = mf->next;
		free(mf);
		mf = mf_next;
	}
}

/* register empty */
CAT_BEFORE_EACH(dummy_ulWrxnsWbplaOTpA)
{
	return;
}

/* register empty */
CAT_AFTER_EACH(dummy_2TDU1qc96gy8Vgfp)
{
	return;
}

static void
run_test_suite(const char *suite)
{
	const struct test_item *item = NULL;
	struct timeval start;
	struct timeval end;
	int ret;


	printf("\nRunning test suite \"%s\"\n", suite);

	gettimeofday(&start, NULL);

	_FOREACH_TEST(item, test_case, struct test_item)
	{
		ret = strcmp(suite, item->suite);
		if (ret != 0)
		{
			continue;
		}

		test_case_run(item);
	}

	gettimeofday(&end, NULL);
	timersub(&end, &start, &state.time_elapsed);

	deinit_mocked_functions();
}

int
main(int argc, char *argv[])
{
	int i;
	int retval = 0;
	int index;
	int mode = 0;


	state.single_process = 0;
	state.name = argv[0];
	memset(&state.time_elapsed, 0, sizeof(state.time_elapsed));

	for (i = 1; i < argc; i++)
	{
		if (mode != 0)
		{
			break;
		}

		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'p':
				state.single_process = 1;
				break;
			case 'a':
				mode = 1;
				break;
			case 'l':
				show_tests();
				break;
			case 's':
				mode = 2;
				index = i + 1;
				break;
			case 'h':
			default:
				help();
			}
		}
		else
		{
			mode = 3;
			index = i;
			break;
		}
	}

	switch (mode)
	{
	case 1:
		test_run_all();
		break;
	case 2:
		test_run_suites(argv + index, argc - index);
		break;
	case 3:
		test_run_tests(argv + index, argc - index);
		break;
	default:
		break;
	}

	switch (mode)
	{
	case 1:
	case 2:
	case 3:
		display_summary();
		break;
	}

	if (state.results[RESULT_TYPE_FAIL] != 0 ||
	    state.results[RESULT_TYPE_UNKNOWN] != 0)
	{
		/* Failed */
		retval = -1;
	}

	return retval;
}
