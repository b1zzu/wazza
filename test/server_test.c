
#include "server_test.h"
#include "../src/server.h"

START_TEST(test_server) 
{
    ck_assert_int_eq(1, 2);
}
END_TEST

Suite *server_test_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("server");

    tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_server);

    suite_add_tcase(s, tc_core);

    return s;
}
