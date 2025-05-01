
#include "unity.h"
#include <stdio.h>
#include <string.h>

#include "mem_mess_memory.h"

kin_val_t my_data[5] = {0};
int32_t my_new_data[5] = {203, -5, -423, 19, 63217};

void setUp(void)
{
    memset(my_data, 0, sizeof my_data);
}

void tearDown(void)
{
}

DEC_KINSET(kin_test, 5) ;;

DEF_KINSET(kin_test, 5, 10) 
{
    [0]=&my_data[0],
    [1]=&my_data[1],
    [2]=&my_data[2],
    [3]=&my_data[3],
    [4]=&my_data[4]
};

void print_kinset(kinset_t *kset)
{
    for(int i=0; i < GET_KINSET_SIZE(kin_test); i++)
    {
        printf("%i, ", kset->val[i]->current);
    }
    printf("\n");
}

void test_mem_kin(void)
{
    TEST_ASSERT_EQUAL(0, mem_kinset_update(&kin_test_set, my_new_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, mem_kin_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, mem_kin_advance(&kin_test_set));
}

void test_mem_kin_1(void)
{
    kin_test_set.steps = 1;
    TEST_ASSERT_GREATER_THAN(0, mem_kinset_update(&kin_test_set, my_new_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_GREATER_THAN(0, mem_kin_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, mem_kin_advance(&kin_test_set));
}

void test_mem_kin_21(void)
{
    int32_t my_new_data2[5] = {-2030, 50, 4230, -190, -6321};
    SET_KINSET_STEPS(kin_test, 21);
    TEST_ASSERT_EQUAL(0, mem_kinset_update(&kin_test_set, my_new_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, mem_kin_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, mem_kin_advance(&kin_test_set));

    TEST_ASSERT_EQUAL(0, mem_kinset_update(&kin_test_set, my_new_data2));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, mem_kin_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, mem_kin_advance(&kin_test_set));    


    TEST_ASSERT_EQUAL(0, mem_kinset_update(&kin_test_set, my_new_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, mem_kin_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, mem_kin_advance(&kin_test_set));    
}

void test_mem_kin_781(void)
{
    SET_KINSET_STEPS(kin_test, 781);
    TEST_ASSERT_EQUAL(0, mem_kinset_update(&kin_test_set, my_new_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, mem_kin_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, mem_kin_advance(&kin_test_set));
}


void test_mem_kin_not_new(void)
{
    int32_t my_zero_data[5] = {0};

    TEST_ASSERT_GREATER_THAN(0, mem_kinset_update(&kin_test_set, my_zero_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_GREATER_THAN(0, mem_kin_advance(&kin_test_set));
        print_kinset(&kin_test_set);    
        break;    
    }

    TEST_ASSERT_GREATER_THAN(0, mem_kin_advance(&kin_test_set));
}


