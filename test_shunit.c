#include "shunit.h"

int main(void) {
    int any_failed = 0;
#undef ON_TEST_FAIL
#define ON_TEST_FAIL any_failed = 1;

    TEST("this should succeed") {
        ASSERT(1 == 1);
        int b = 6, c = 7;
        ASSERT(b < c);
    }
    printf("Any failed? %d\n", any_failed);

    TEST("this should fail") {
        int d = 8, e = 9;
        ASSERT(d >= e);
    }
    printf("Any failed? %d\n", any_failed);

    TEST("eq tests") {
        ASSERT_EQ(1, 1, "%d");
        ASSERT_EQ(3UL, 3UL, "%lu");
        ASSERT_INT_EQ(4, 4);
        ASSERT_INT_EQ((long) -1, (long) -1);
        ASSERT_UINT_EQ(5u, 5u);
        ASSERT_UINT_EQ(15L, 15L);
        int a = 5;
        ASSERT_PTR_EQ(&a, &a);
    }

    TEST("int eq fail")     { ASSERT_INT_EQ(5, 6);      }
    TEST("uint eq fail")    { ASSERT_UINT_EQ(6u, 7u);   }
    TEST("long eq fail")    { ASSERT_INT_EQ(-7L, -8L); }
    TEST("ulong eq fail")   { ASSERT_UINT_EQ((unsigned long) -1, 0L); }
    TEST("ptr eq fail") {
        int a = 7;
        int b = 7;
        ASSERT_PTR_EQ(&a, &b);
    }

    TEST("memory eq tests") {
        int a[] = {1, 2, 3};
        int bb[] = {1, 2, 3};
        int *b = bb;
        ASSERT(a != b);
        ASSERT_MEM_EQ(a, b, 3);

        long c[] = {1, 2, 3, 4};
        long d[] = {1, 2, 3, 5};
        ASSERT(c != d);
        ASSERT_MEM_EQ(c, d, 3);

        char e[] = {'a', 'b', 'c', 'd'};
        char f[] = {'a', 'b', 'c', 'd'};
        ASSERT(e != f);
        ASSERT_MEM_EQ(e, f, 4);
    }

    TEST("differently-sized memory test") {
        int a[] = {1, 2};
        long b[] = {3};
        ASSERT_MEM_EQ(a, b, 2);
    }

    TEST("non-eq memory test 1") {
        int a[] = {1, 2, 3};
        int b[] = {4, 2, 3};
        ASSERT(a != b);
        ASSERT_MEM_EQ(a, b, 3);
    }

    TEST("non-eq memory test 2") {
        int a[] = {1, 2, 3};
        int b[] = {1, 2, 4};
        ASSERT(a != b);
        ASSERT_MEM_EQ(a, b, 3);
    }

    TEST("non-eq memory test 3") {
        int abc[] = {1, 2, 3};
        int b[] = {1, 2, 4};
        ASSERT(abc != b);
        ASSERT_MEM_EQ(abc, b, 3);
    }

    TEST("non-eq memory test 4") {
        int a[] = {1, 2, 3};
        int bcd[] = {1, 2, 4};
        ASSERT(a != bcd);
        ASSERT_MEM_EQ(a, bcd, 3);
    }

    TEST("non-eq memory test 5") {
        long a[] = {6, 7, 8000};
        long b[] = {6, 7, -1};
        ASSERT(a != b);
        ASSERT_MEM_EQ(a, b, 3);
    }

    return any_failed;
}
