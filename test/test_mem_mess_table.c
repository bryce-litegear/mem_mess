


#include "unity.h"

#include "mem_mess_table.h"

void setUp(void)
{
}

void tearDown(void)
{
}

mem_mess_record_t mes_recs[] =
{
    {.token=1},
    {.token=3},
    {.token=4},
    {.token=9},
    {.token=23},
    {.token=42},
};


mem_mess_table_t m_table = {.table=mes_recs, .table_entries = ARR_LEN(mes_recs)};

void test_mem_mess_table_NeedToImplement(void)
{
    TEST_ASSERT_EQUAL_PTR(&mes_recs[0], mem_mes_get_record(&m_table, 1));
    TEST_ASSERT_EQUAL_PTR(NULL, mem_mes_get_record(&m_table, 8));
    TEST_ASSERT_EQUAL_PTR(&mes_recs[2], mem_mes_get_record(&m_table, 4));
    TEST_ASSERT_EQUAL_PTR(&mes_recs[5], mem_mes_get_record(&m_table, 42));
    TEST_ASSERT_EQUAL_PTR(NULL, mem_mes_get_record(&m_table, 100));
}

