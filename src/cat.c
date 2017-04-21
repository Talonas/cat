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
#define COLOR_LIGHT_CYAN "\033[1;36m"
#define COLOR_NORMAL "\033[0m"

enum final_result_type {
	RESULT_TYPE_PASS = 0,
	RESULT_TYPE_FAIL,
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
	const char *message_long;
	char message_short;
	const struct test_item *item;
	int ret;
	int result_type;
};

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
		ret = item->test();
		exit(ret);
	}

	if (pid == -1)
	{
		/* somehow fork failed, run test local'y */
		ret = item->test();
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
test_case_run(const struct test_item *item)
{
	struct test_result result;
	int ret;


	if (state.single_process == 1)
	{
		ret = item->test();
	}
	else
	{
		ret = test_run_in_process(item);
	}

	switch (ret)
	{
	case TEST_PASSED:
		result.message_long = "PASS";
		result.color = COLOR_GREEN;
		state.results[RESULT_TYPE_PASS]++;
		break;
	case TEST_FAIL:
		result.message_long = "FAIL";
		result.color = COLOR_RED;
		state.results[RESULT_TYPE_FAIL]++;
		break;
	default:
		result.message_long = "UNKNOWN";
		result.color = COLOR_RED;
		state.results[RESULT_TYPE_UNKNOWN]++;
		break;
	}

	printf("  - %s[%s]%s %s\n", result.color, result.message_long,
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
}

static void
display_summary(void)
{
	int total = 0;

	total = state.results[RESULT_TYPE_PASS] +
		state.results[RESULT_TYPE_FAIL] +
		state.results[RESULT_TYPE_UNKNOWN];

	printf("\n%-10s\n\t%-10s %-10s %-10s %-10s\n"
		"\t%-10d %-10d %-10d %-10d\n",
		"Summary", "TOTAL", "PASSED", "FAILED", "UNKNOWN",
		total,
		state.results[RESULT_TYPE_PASS],
		state.results[RESULT_TYPE_FAIL],
		state.results[RESULT_TYPE_UNKNOWN]
	);

	printf("\nTime elapsed: %ld.%06ld seconds\n\n",
		(long int)state.time_elapsed.tv_sec,
		(long int)state.time_elapsed.tv_usec);
}

static void
help(void)
{
	printf("Usage: %s [OPTIONS]\n"
		"\n"
		"Options:\n"
		"  -h        Prints this help\n"
		"Test run options:\n"
		"  -p        Run tests in single process\n"
		"", state.name);
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

	_FOREACH_TEST(item, test_case, struct test_item)
	{
		printf("%s\n", item->name);
	}
}

int
main(int argc, char *argv[])
{
	int i;
	int retval = 0;
	const struct test_item *test = NULL;


	state.single_process = 0;
	state.name = argv[0];
	memset(&state.time_elapsed, 0, sizeof(state.time_elapsed));

	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
			case 'p':
				state.single_process = 1;
				break;
			case 'a':
				test_run_all();
				break;
			case 'l':
				show_tests();
				break;
			case 'h':
			default:
				help();
			}
		}
		else
		{
			test = test_search(argv[i]);
			if (test == NULL)
			{
				printf("Test \"%s\" can't be found\n", argv[i]);
			}
			else
			{
				test_case_run(test);
			}
		}
	}

	display_summary();

	if (state.results[RESULT_TYPE_FAIL] != 0 ||
	    state.results[RESULT_TYPE_UNKNOWN] != 0)
	{
		/* Failed */
		retval = -1;
	}

	return retval;
}