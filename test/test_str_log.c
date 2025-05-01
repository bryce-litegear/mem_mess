


#include "unity.h"

#include "str_log.h"

const char my_str_1[] = "the first string";
const char *my_str_2 = "the second string";
str_const_t my_str_3 = "the third string";
str_const_t my_str_4 = "the fourth string";
str_const_t my_str_5 = "the fifth string";
str_const_t my_str_6 = "the sixth string";


STR_LOG_DEC(testing);

STR_LOG_DEF(testing, 10);



void setUp(void)
{
    testing_slog.last = NULL;
    testing_slog.oldest = NULL;
    TEST_ASSERT_EQUAL(10, testing_slog.arr_len);
    TEST_ASSERT_EQUAL_PTR(testing_slog_store, testing_slog.log);
}

void tearDown(void)
{
}



void test_str_log(void)
{
    mem_str_log_add(&testing_slog, my_str_1);
    mem_str_log_add(&testing_slog, my_str_1);
    mem_str_log_add(&testing_slog, my_str_2);
    mem_str_log_add(&testing_slog, my_str_1);
    mem_str_log_add(&testing_slog, my_str_2);
    mem_str_log_add(&testing_slog, my_str_2);
    mem_str_log_add(&testing_slog, my_str_2);
    mem_str_log_add(&testing_slog, my_str_3);
    mem_str_log_add(&testing_slog, my_str_3);
    mem_str_log_add(&testing_slog, my_str_2);
    mem_str_log_add(&testing_slog, my_str_1);

    TEST_ASSERT_EQUAL_PTR(my_str_1, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_2, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_1, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_2, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_3, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_2, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_1, mem_str_log_get(&testing_slog));

}

