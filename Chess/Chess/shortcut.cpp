/*----------------------------------------------------------
 *				HTBLA-Leonding
 * ---------------------------------------------------------
 * Title:			shortcut.c
 * Author:			P. Bauer
 * Date:			November 08, 2010
 * ----------------------------------------------------------
 * Description:
 * Test driver.
 * ----------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>

#include "shortcut.h"

#define MAX_TEST_FUNCTIONS 256

static int tc_count = 0;
static int tc_fail_count = 0;

static struct TestCase test_cases[MAX_TEST_FUNCTIONS];

const char* version()
{
	return "ShortCut v. 1.1.1";
}

void assert_true(bool bool_expr, struct TestCase *tc, const char *msg)
{
	if (!bool_expr) {
		if (tc->success) {
			tc->success = false;
			tc_fail_count++;
		}
		printf("\n\tFailure: %s: %s", tc->name, msg);
	}
}

void assert_false(bool bool_expr, struct TestCase *tc, const char *msg)
{
	assert_true(!bool_expr, tc, msg);
}

static void assert_string_failure(const char *expected, char *actual, struct TestCase *tc, const char *msg);

void assert_equals(const char *expected, char *actual, struct TestCase *tc, const char *msg)
{
	if (expected == actual) {
		return;
	}

	if (expected == 0 || actual == 0) {
		assert_string_failure(expected, actual, tc, msg);
		return;
	}

	if (strcmp(actual, expected) != 0) {
		assert_string_failure(expected, actual, tc, msg);
		return;
	}
}

#define MAX_MSG_LEN 128
static void assert_string_failure(const char *expected, char *actual, struct TestCase *tc, const char *msg)
{
  char new_msg[MAX_MSG_LEN];

  sprintf(new_msg, "Expected %s, actual %s. %s", expected, actual, msg);
  assert_true(false, tc, new_msg);
}

void assert_equals(int expected, int actual, struct TestCase *tc, const char *msg)
{
	char new_msg[MAX_MSG_LEN];
	sprintf(new_msg, "Expected %d, actual %d. %s", expected, actual, msg);
	assert_true(expected == actual, tc, new_msg);
}

int get_test_count()
{
	return tc_count;
}

bool add_test(void (*test_function)(struct TestCase *tc), const char *test_name)
{
	if (tc_count == MAX_TEST_FUNCTIONS) {
		return false;
	}
	else {
		test_cases[tc_count].success = true;
		test_cases[tc_count].name = test_name;
		test_cases[tc_count].test_function = test_function;
		tc_count++;

		return true;
	}
}

void run_tests()
{
	int i;

	printf("\n%s: Running tests\n", version());

	for (i = 0; i < get_test_count(); i++) {
		printf("Running test %s ...", test_cases[i].name);
		test_cases[i].test_function(&test_cases[i]);
		if (test_cases[i].success) {
			printf("\033[32m OK\033[m");
		}
		else {
			printf("\033[31m ... FAIL\033[m");
		}
		printf("\n");
	}

	printf("\nTotal tests run: %d\n", tc_count);
	if (tc_fail_count > 0) {
		printf("\033[31mTests failed: %d\033[m\n", tc_fail_count);
	}
	else {
		printf("\033[32mAll tests run successfully\033[m\n");
	}
}
