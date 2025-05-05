


#include "unity.h"

#include "str_log.h"

const char my_str_1[] = "the first string";
const char *my_str_2 = "the second string";
str_const_t my_str_3 = "the third string";
str_const_t my_str_4 = "the fourth string";
str_const_t my_str_5 = "the fifth string";
str_const_t my_str_6 = "the sixth string";


STR_LOG_DEC(testing);

STR_LOG_DEF(testing, 7);

void setUp(void)
{
    testing_slog.last = NULL;
    testing_slog.oldest = NULL; 
    testing_slog.lost = 0;
    TEST_ASSERT_EQUAL(7, testing_slog.arr_len);
    TEST_ASSERT_EQUAL_PTR(testing_slog_store, testing_slog.log);
}

void tearDown(void)
{
}



void test_str_log(void)
{
    mem_str_log_add(&testing_slog, my_str_1); // 1
    mem_str_log_add(&testing_slog, my_str_1); // 1
    mem_str_log_add(&testing_slog, my_str_2); // 2
    mem_str_log_add(&testing_slog, my_str_1); // 3
    mem_str_log_add(&testing_slog, my_str_2); // 4
    mem_str_log_add(&testing_slog, my_str_2); // 4
    mem_str_log_add(&testing_slog, my_str_2); // 4
    mem_str_log_add(&testing_slog, my_str_3); // 5
    mem_str_log_add(&testing_slog, my_str_3); // 5
    TEST_ASSERT_EQUAL(0, testing_slog.lost);
    mem_str_log_add(&testing_slog, my_str_2); // 6
    mem_str_log_add(&testing_slog, my_str_1); // 7
    mem_str_log_add(&testing_slog, my_str_5); // this will drop the oldest
    TEST_ASSERT_NOT_EQUAL(0, testing_slog.lost);

    TEST_ASSERT_EQUAL_PTR(my_str_2, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_1, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_2, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_3, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_2, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_1, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_5, mem_str_log_get(&testing_slog));

}

void test_str_log_2(void)
{
    TEST_ASSERT_EQUAL(0, testing_slog.lost);
    mem_str_log_add(&testing_slog, my_str_1);
    mem_str_log_add(&testing_slog, my_str_1);
    mem_str_log_add(&testing_slog, my_str_2);
    mem_str_log_add(&testing_slog, my_str_1);
    TEST_ASSERT_EQUAL_PTR(my_str_1, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL(0, testing_slog.lost);
    mem_str_log_add(&testing_slog, my_str_2);
    mem_str_log_add(&testing_slog, my_str_2);
    TEST_ASSERT_EQUAL_PTR(my_str_2, mem_str_log_get(&testing_slog));
    mem_str_log_add(&testing_slog, my_str_2);
    TEST_ASSERT_EQUAL_PTR(my_str_1, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL(0, testing_slog.lost);
    mem_str_log_add(&testing_slog, my_str_3);
    TEST_ASSERT_EQUAL_PTR(my_str_2, mem_str_log_get(&testing_slog));
    mem_str_log_add(&testing_slog, my_str_3);
    mem_str_log_add(&testing_slog, my_str_2);
    TEST_ASSERT_EQUAL_PTR(my_str_3, mem_str_log_get(&testing_slog));
    mem_str_log_add(&testing_slog, my_str_1);
    mem_str_log_add(&testing_slog, my_str_5);
    TEST_ASSERT_EQUAL(0, testing_slog.lost);
    TEST_ASSERT_EQUAL_PTR(my_str_2, mem_str_log_get(&testing_slog));
    mem_str_log_add(&testing_slog, my_str_4);
    TEST_ASSERT_EQUAL_PTR(my_str_1, mem_str_log_get(&testing_slog));
    mem_str_log_add(&testing_slog, my_str_6);
    TEST_ASSERT_EQUAL(0, testing_slog.lost);
    mem_str_log_add(&testing_slog, my_str_1);
    TEST_ASSERT_EQUAL_PTR(my_str_5, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_4, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(my_str_6, mem_str_log_get(&testing_slog));
    TEST_ASSERT_EQUAL_PTR(testing_slog.oldest, testing_slog.last);
    TEST_ASSERT_NOT_NULL(testing_slog.oldest);
    TEST_ASSERT_EQUAL_PTR(*testing_slog.oldest, my_str_1);
    TEST_ASSERT_EQUAL_PTR(my_str_1, mem_str_log_get(&testing_slog));
    TEST_ASSERT_NULL(testing_slog.last);
    TEST_ASSERT_NULL(testing_slog.oldest);
}

