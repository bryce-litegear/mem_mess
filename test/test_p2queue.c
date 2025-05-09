

#include "unity.h"

#include "p2queue.h"

void setUp(void)
{
}

void tearDown(void)
{
}

p2queue_t *p2q;
uint8_t pbuf[64];
uint8_t pbuf2[128];
p2queue_t p2q2  = (p2queue_t){.queue=pbuf2, .mask = 127};

void test_p2queue(void)
{
    p2queue_t myp2q = p2q_bind(pbuf, sizeof pbuf);
    p2q = &myp2q;
    TEST_ASSERT_EQUAL(63, p2q_space(p2q));
    TEST_ASSERT_EQUAL(127, p2q_space(&p2q2));
    for(uint8_t i=0; i<10; i++)
    {
        p2q_in(p2q, i);
        p2q_in(&p2q2, i);
    }
    TEST_ASSERT_EQUAL(53, p2q_space(p2q));
    TEST_ASSERT_EQUAL(117, p2q_space(&p2q2));

    for(uint8_t i=0; i<123; i++)
    {
        uint8_t temp;
        TEST_ASSERT_TRUE(p2q_in(p2q, i));
        TEST_ASSERT_TRUE(p2q_in(&p2q2, i));
        TEST_ASSERT_TRUE(p2q_out(p2q, &temp));
        TEST_ASSERT_TRUE(p2q_out(&p2q2, &temp));
    }

    TEST_ASSERT_EQUAL(53, p2q_space(p2q));
    TEST_ASSERT_EQUAL(117, p2q_space(&p2q2));

}







