#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Feel free to #undef this and #define it to something else
#define ON_TEST_FAIL do {} while (0)

#define TEST(_name)                                                         \
    for (                                                                   \
        char *_SHUNIT_TESTING_ =                                            \
            (printf("\n--- [%s] starting\n", (_name)), (_name));            \
        _SHUNIT_TESTING_;                                                   \
        _SHUNIT_TESTING_ = NULL, printf("--- [%s] succeeded\n", (_name))    \
    )

#define ASSERT(_cond)                                                       \
    if (!(_cond)) {                                                         \
        printf("--- [%s] FAILED at <" #_cond "> "                           \
                "(" __FILE__ " line %d)\n", _SHUNIT_TESTING_, __LINE__);    \
        ON_TEST_FAIL;                                                       \
        break;                                                              \
    }
