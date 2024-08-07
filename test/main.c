/**
 * Run all unit tests leveraging Check: https://libcheck.github.io/check/
 */

#include <check.h>
#include "server_test.h"

START_TEST(test)
{
    // This is an empty test that can be used to try out C code without having to crate
    //  a new test, test case, test suite and add everything to the test runner.
    //
    // Just write here the code you want to test and run `make check`.
}
END_TEST

/**
 * Executed with `make check` or compiled with `make wazza_test` and execute with `./wazza_test`.
 *
 * This scripts runs all unit tests defined in the _test.c files.
 *
 * Attention: Each test needs to be explicitly added to the test suite and new test suites need to be
 *              explicitly added to runner in this main function.
 */
int main(void)
{
    int number_failed;
    SRunner *sr;
    Suite *s;
    TCase *tc_core;

    s = suite_create("test");
    tc_core = tcase_create("core");
    tcase_add_test(tc_core, test);
    suite_add_tcase(s, tc_core);

    sr = srunner_create(s);
    srunner_add_suite(sr, server_test_suite());

    // Run all test suites added to runner "sr",
    // collect the number of failed tests,
    // and free the memory used by runner.
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? 0 : 1;
}
