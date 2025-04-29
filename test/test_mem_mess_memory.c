
#include "unity.h"
#include <stdio.h>

#include "mem_mess_memory.h"

void setUp(void)
{
}

void tearDown(void)
{
}

DEC_KIN_SET(kin_test, 5) ;;

kin_val_t my_data[5] = {0};
int32_t my_new_data[5] = {200, 5, 4, 9, 3000};

DEF_KIN_SET(kin_test, 5, 9) 
{
    [0]=&my_data[0],
    [1]=&my_data[1],
    [2]=&my_data[2],
    [3]=&my_data[3],
    [4]=&my_data[4]
};

void print_kin_set(kin_set_t *kset)
{
    for(int i=0; i < KIN_SET_SIZE(kin_test); i++)
    {
        printf("%i, ", kset->val[i]->current);
    }
    printf("\n");
}

void test_mem_kin(void)
{
    TEST_ASSERT_EQUAL(0, mem_kin_start_move(&kin_test_set, my_new_data));
    print_kin_set(&kin_test_set);

    for(int step = 0; step < KIN_SET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, mem_kin_move(&kin_test_set));
        print_kin_set(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, mem_kin_move(&kin_test_set));
}



