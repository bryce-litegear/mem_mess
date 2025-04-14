


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
    MM_OBJ(reg_buf),

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

// hand packed
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
    MM_OBJ_ARR(my_data_set),
    .indexed8=1,
    .pl_offset=1,
};

void test_mem_mess_setter_indexed8(void)
{
    struct  // bytes
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
    MM_OBJ_ARR(my_data_set),
    .indexed32=1,
    .pl_offset=4,
};

void test_mem_mess_setter_indexed32(void)
{
    struct 
    {
        uint8_t pix[4];
        uint8_t data[sizeof(my_struct)];
    } payload = {.pix[0]=5};

    my_struct temp = {.ui = 23456, .us = 5434, .ub[1] = 72};

    memcpy(payload.data, &temp, sizeof temp);
    
    my_mem_mess_background_Expect(&my_data_set[5]);

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexed32_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[5].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[5].us);
    TEST_ASSERT_EQUAL(temp.ub[0], my_data_set[5].ub[0]);

    my_mem_mess_background_Expect(&my_data_set[2]);
    payload.pix[0] = 2;

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexed32_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[2].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[2].us);
    TEST_ASSERT_EQUAL(temp.ub[0], my_data_set[2].ub[0]);    
}


mem_mess_record_t indexoffset_message = 
{
    MM_OBJ_ARR(my_data_set),    
    .indexed32=1,
    .index_offset=4,
    .pl_offset=8,
};

void test_mem_mess_setter_indexed_offset32(void)
{
    struct 
    {
        uint8_t res[4];
        uint8_t pix[4];
        uint8_t data[sizeof(my_struct)];
    } payload = {.pix[0]=1};

    my_struct temp = {.ui = 2356, .us = 544, .ub[1] = 26};

    memcpy(payload.data, &temp, sizeof temp);
    

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexoffset_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[1].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[1].us);
    TEST_ASSERT_EQUAL(temp.ub[1], my_data_set[1].ub[1]);

    payload.pix[0] = 2;

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexoffset_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[2].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[2].us);
    TEST_ASSERT_EQUAL(temp.ub[0], my_data_set[2].ub[0]);    
}

mem_mess_record_t indexoffset8_message = 
{
    MM_OBJ_ARR(my_data_set),    
    .indexed8=1,
    .index_offset=2,
    .pl_offset=3,
};

void test_mem_mess_setter_indexed_offset8(void)
{
    struct 
    {
        uint8_t res;
        uint8_t res2;
        uint8_t pix;
        uint8_t data[sizeof(my_struct)];
    } payload = {.pix=7};

    my_struct temp = {.ui = 23256, .us = 5244, .ub[1] = 226};

    memcpy(payload.data, &temp, sizeof temp);
    

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexoffset8_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[7].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[7].us);
    TEST_ASSERT_EQUAL(temp.ub[1], my_data_set[7].ub[1]);

    payload.pix = 3;

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexoffset8_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[3].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[3].us);
    TEST_ASSERT_EQUAL(temp.ub[0], my_data_set[3].ub[0]);    
}


mem_mess_record_t indexoffset8_vlen_message = 
{
    MM_OBJ_ARR(my_data_set),    
    .indexed8=1,
    .index_offset=2,
    .len8=1,
    .len_offset=1,
    .pl_offset=3,
};

void test_mem_mess_setter_indexed_offset8_vlen(void)
{
    struct 
    {
        uint8_t res;
        uint8_t len;
        uint8_t pix;
        uint8_t data[sizeof(my_struct)];
    } payload = {.pix=7, .len = sizeof my_data_set[0]};

    my_struct temp = {.ui = 256, .us = 524, .ub[1] = 22};

    memcpy(payload.data, &temp, sizeof temp);
    

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexoffset8_vlen_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[7].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[7].us);
    TEST_ASSERT_EQUAL(temp.ub[1], my_data_set[7].ub[1]);

    payload.pix = 3;

    TEST_ASSERT_EQUAL(0, mem_mess_process(&indexoffset8_vlen_message, (uint8_t*)&payload, sizeof payload));
    TEST_ASSERT_EQUAL(temp.ui, my_data_set[3].ui);
    TEST_ASSERT_EQUAL(temp.us, my_data_set[3].us);
    TEST_ASSERT_EQUAL(temp.ub[0], my_data_set[3].ub[0]);    
}



// unique case of the binary flash programming messages
// 0,size32,startAdd32,[256]

uint8_t flash_target[256];
uint32_t test_add = 0x1300; // page 19
uint32_t test_size = sizeof flash_target;

int32_t flash_process(mem_mess_record_t const *mesrec, uint8_t *payload, uint32_t pl_size)
{
    TEST_ASSERT_EQUAL(sizeof flash_target + 9, pl_size );
    TEST_ASSERT_EQUAL_PTR(flash_target, mesrec->mem_obj_pnt);
    TEST_ASSERT_EQUAL(sizeof flash_target, mesrec->mem_obj_len);
    TEST_ASSERT_EQUAL(0, payload[0]);
    uint32_t size;
    uint32_t add;
    memcpy(&size, &payload[mesrec->len_offset], sizeof size);
    memcpy(&add, &payload[mesrec->index_offset], sizeof add);
    TEST_ASSERT_TRUE(mesrec->len32);
    TEST_ASSERT_TRUE(mesrec->indexed32);
    TEST_ASSERT_EQUAL(test_add, add);
    TEST_ASSERT_EQUAL(test_size, size);
    return (int32_t)add;
}

mem_mess_record_t flash_rec_message =
{
    MM_OBJ(flash_target),
    .indexed32=1,
    .index_offset=5,
    .len32=1,
    .len_offset=1,
    .immediate = flash_process,
    .pl_offset=9,
};

void test_mem_mess_setter_indexed_len_1_4_4(void)
{
    struct 
    {
        uint8_t is_zero;
        uint8_t size[4];
        uint8_t add[4];
        uint8_t data[sizeof(flash_target)];
    } payload = {.is_zero=0};
    memcpy(payload.add, &test_add, sizeof payload.add);
    memcpy(payload.size, &test_size, sizeof payload.size);

    TEST_ASSERT_EQUAL((int32_t)test_add, mem_mess_process(&flash_rec_message, (uint8_t*)&payload, sizeof payload));

}
