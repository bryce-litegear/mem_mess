


#include "unity.h"

#include "mem_mess_message.h"
#include "mock_user_files.h" 
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

uint32_t reg_buf[2] = {123, 456};


mem_mess_record_t regular_message = 
{
    .background = my_mem_mess_background,
    .mem_pnt = reg_buf,
    .mem_struct_len = sizeof reg_buf,
};


void test_mem_mess_setter(void)
{
    uint32_t reg_payload[] = {3,4,5,6,7,8,9,10,1,2,3,4};
    my_mem_mess_background_Expect(reg_buf);
    reg_buf[0] = 123;
    TEST_ASSERT_EQUAL(0, mem_mess_process(&regular_message, (uint8_t*)reg_payload, sizeof reg_payload));
    TEST_ASSERT_EQUAL(reg_payload[0], reg_buf[0]);
    TEST_ASSERT_EQUAL(reg_payload[1], reg_buf[1]);

    reg_buf[0] = 123;
    my_mem_mess_background_Expect(reg_buf);    
    TEST_ASSERT_EQUAL(0, mem_mess_process(&regular_message, (uint8_t*)&reg_payload[3], sizeof reg_payload));
    TEST_ASSERT_EQUAL(reg_payload[3], reg_buf[0]);
    TEST_ASSERT_EQUAL(reg_payload[4], reg_buf[1]);

}


typedef struct my_struct
{
    uint32_t ui;
    uint16_t us;
    uint8_t ub[2];
} my_struct;

my_struct my_data_set[10];


mem_mess_record_t indexed8_message = 
{
    .background = my_mem_mess_background,
    .mem_pnt = my_data_set,
    .mem_struct_len = sizeof my_data_set[0]+1,
    .byte_indexed=1,
};

void test_mem_mess_setter_indexed8(void)
{
    struct 
    {
        uint8_t pix;
        uint8_t data[sizeof(my_struct)];
    } payload = {.pix=4};

    my_struct temp = {.ui = 123456, .us = 543, .ub[1] = 42};

    memcpy(payload.data, &temp, sizeof temp);
    
    my_mem_mess_background_Expect(&my_data_set[4]);

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexed8_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[4].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[4].us);
    TEST_ASSERT_EQUAL(temp.ub[0], my_data_set[4].ub[0]);

    my_mem_mess_background_Expect(&my_data_set[7]);
    payload.pix = 7;

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexed8_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[7].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[7].us);
    TEST_ASSERT_EQUAL(temp.ub[0], my_data_set[7].ub[0]);    
}


mem_mess_record_t indexed32_message = 
{
    .background = my_mem_mess_background,
    .mem_pnt = my_data_set,
    .mem_struct_len = sizeof my_data_set[0]+4,
    .word_indexed=1,
};

void test_mem_mess_setter_indexed32(void)
{
    struct 
    {
        uint32_t pix;
        uint8_t data[sizeof(my_struct)];
    } payload = {.pix=5};

    my_struct temp = {.ui = 23456, .us = 5434, .ub[1] = 72};

    memcpy(payload.data, &temp, sizeof temp);
    
    my_mem_mess_background_Expect(&my_data_set[5]);

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexed32_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[5].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[5].us);
    TEST_ASSERT_EQUAL(temp.ub[0], my_data_set[5].ub[0]);

    my_mem_mess_background_Expect(&my_data_set[2]);
    payload.pix = 2;

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexed32_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[2].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[2].us);
    TEST_ASSERT_EQUAL(temp.ub[0], my_data_set[2].ub[0]);    
}


mem_mess_record_t indexoffset_message = 
{
    .mem_pnt = my_data_set,
    .mem_struct_len = sizeof my_data_set[0]+8,
    .word_indexed=1,
    .index_offset=4,
};

void test_mem_mess_setter_indexed_offset32(void)
{
    struct 
    {
        uint32_t res;
        uint32_t pix;
        uint8_t data[sizeof(my_struct)];
    } payload = {.pix=1};

    my_struct temp = {.ui = 2356, .us = 544, .ub[1] = 26};

    memcpy(payload.data, &temp, sizeof temp);
    

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexoffset_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[1].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[1].us);
    TEST_ASSERT_EQUAL(temp.ub[1], my_data_set[1].ub[1]);

    payload.pix = 2;

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexoffset_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[2].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[2].us);
    TEST_ASSERT_EQUAL(temp.ub[0], my_data_set[2].ub[0]);    
}
