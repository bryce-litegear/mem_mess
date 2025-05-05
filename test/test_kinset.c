/**
 * @file test_kinset.c
 * @author Bryce Deary (bdeary@chauvetcinema.com)
 * @brief tests for kinematic set module
 * @date 2025-05-01
 * 
 * @copyright Copyright LiteGear (2025)
 * 
 */
#include "unity.h"
#include <stdio.h>
#include <string.h>
#include "kinset.h"


kin_val_t my_data[5] = {0};
int32_t my_new_data[5] = {203, -5, -423, 19, 63217};

kin_val_t my_pair[2] = {0};
int32_t my_new_pair[2] = {-23, -42};

void setUp(void)
{
    memset(my_data, 0, sizeof my_data);
}

void tearDown(void)
{
}

DEC_KINSET(kin_test, 5);

DEF_KINSET(kin_test, 5, 10) 
{
    [0]=&my_data[0],
    [1]=&my_data[1],
    [2]=&my_data[2],
    [3]=&my_data[3],
    [4]=&my_data[4],
};

DEC_KINSET(data_pair, 2);
DEF_KINSET(data_pair, 2, 13)
{
    [0]=&my_pair[0],
    [1]=&my_pair[1],
};

void print_kinset(kinset_t *kset)
{
    for(int i=0; i < kset->set_size; i++)
    {
        printf("%i, ", kset->val[i]->current);
    }
    printf("\n");
}

void test_mem_kin(void)
{
    TEST_ASSERT_EQUAL(0, kinset_update(&kin_test_set, my_new_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, kinset_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, kinset_advance(&kin_test_set));
}

void test_mem_kin_pair(void)
{
    int32_t final[2];
    TEST_ASSERT_EQUAL(0, kinset_update(&data_pair_set, my_new_pair));
    printf("\n");
    print_kinset(&data_pair_set);

    for(int step = 0; step < GET_KINSET_STEPS(data_pair)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, kinset_advance(&data_pair_set));
        print_kinset(&data_pair_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, kinset_advance(&data_pair_set));
    kinset_get_current(&data_pair_set, final);
    TEST_ASSERT_EQUAL_INT32_ARRAY(my_new_pair, final, sizeof final/sizeof final[0]);
}

void test_mem_kin_1(void)
{
    kin_test_set.steps = 1;
    TEST_ASSERT_GREATER_THAN(0, kinset_update(&kin_test_set, my_new_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_GREATER_THAN(0, kinset_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, kinset_advance(&kin_test_set));
}

void test_mem_kin_21(void)
{
    int32_t my_new_data2[5] = {-2030, 50, 4230, -190, -6321};
    SET_KINSET_STEPS(kin_test, 21);
    TEST_ASSERT_EQUAL(0, kinset_update(&kin_test_set, my_new_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, kinset_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, kinset_advance(&kin_test_set));

    TEST_ASSERT_EQUAL(0, kinset_update(&kin_test_set, my_new_data2));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, kinset_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, kinset_advance(&kin_test_set));    


    TEST_ASSERT_EQUAL(0, kinset_update(&kin_test_set, my_new_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, kinset_advance(&kin_test_set));
        print_kinset(&kin_test_set);        
    }

    TEST_ASSERT_GREATER_THAN(0, kinset_advance(&kin_test_set));    
}

void test_mem_kin_781(void)
{
    int32_t final[5];
    SET_KINSET_STEPS(kin_test, 781);
    TEST_ASSERT_EQUAL(0, kinset_update(&kin_test_set, my_new_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_EQUAL(0, kinset_advance(&kin_test_set));
        if(!(step%25)) print_kinset(&kin_test_set);        
    }
    print_kinset(&kin_test_set);        

    TEST_ASSERT_GREATER_THAN(0, kinset_advance(&kin_test_set));
    kinset_get_current(&kin_test_set, final);
    TEST_ASSERT_EQUAL_INT32_ARRAY(my_new_data, final, sizeof final/sizeof final[0]);
}


void test_mem_kin_not_new(void)
{
    int32_t my_zero_data[5] = {0};

    TEST_ASSERT_GREATER_THAN(0, kinset_update(&kin_test_set, my_zero_data));
    printf("\n");
    print_kinset(&kin_test_set);

    for(int step = 0; step < GET_KINSET_STEPS(kin_test)-1; step++)
    {
        TEST_ASSERT_GREATER_THAN(0, kinset_advance(&kin_test_set));
        print_kinset(&kin_test_set);    
        break;    
    }

    TEST_ASSERT_GREATER_THAN(0, kinset_advance(&kin_test_set));
}
