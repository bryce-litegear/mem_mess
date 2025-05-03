/**
 * @file test_mem_mess_table.c
 * @author Bryce Deary (bdeary@chauvetcinema.com)
 * @brief test of the table object and the record finder function only.
 * @date 2025-05-03
 * 
 * Copyright Chauvet Cinema Group (2025)
 * 
 */


#include "unity.h"

#include "mem_mess_table.h"

void setUp(void)
{
}

void tearDown(void)
{
}

// test table only initializes the token column, real use would also include
// associated memory variables and behaviors.
// note that the table is sorted by token number
mem_mess_record_t mes_recs[] =
{
    {.token=1},
    {.token=3},
    {.token=4},
    {.token=9},
    {.token=23},
    {.token=31},
    {.token=37},
    {.token=42},
};

/* setup the table object with the default record getter function*/
mem_mess_table_t m_table = BIND_MESS_RECS(mes_recs);

void test_mem_mess_table(void)
{
    TEST_ASSERT_EQUAL_PTR(&mes_recs[0], mem_mes_get_record(&m_table, 1));
    TEST_ASSERT_EQUAL_PTR(NULL, m_table.get_record(&m_table, 8));
    TEST_ASSERT_EQUAL_PTR(&mes_recs[2], mem_mes_get_record(&m_table, 4));
    TEST_ASSERT_EQUAL_PTR(&mes_recs[7], m_table.get_record(&m_table, 42));
    TEST_ASSERT_EQUAL_PTR(&mes_recs[6], m_table.get_record(&m_table, 37));
    TEST_ASSERT_EQUAL_PTR(NULL, mem_mes_get_record(&m_table, 100));
}

/* setup the table object with the default record getter function*/
mem_mess_table_t m_s_table = BIND_SORTED_MESS_RECS(mes_recs);

void test_sorted_mem_mess_table(void)
{
    TEST_ASSERT_EQUAL_PTR(&mes_recs[0], mem_mes_get_record(&m_s_table, 1));
    TEST_ASSERT_EQUAL_PTR(NULL, m_s_table.get_record(&m_s_table, 8));
    TEST_ASSERT_EQUAL_PTR(&mes_recs[2], mem_mes_get_record(&m_s_table, 4));
    TEST_ASSERT_EQUAL_PTR(&mes_recs[7], m_s_table.get_record(&m_s_table, 42));
    TEST_ASSERT_EQUAL_PTR(NULL, mem_mes_get_record(&m_s_table, 100));
}



