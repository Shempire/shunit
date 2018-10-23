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

    return any_failed;
}
