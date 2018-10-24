#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Feel free to #undef this and #define it to something else
#define ON_TEST_FAIL do {} while (0)

/**
 * Define a test block.
 */
#define TEST(_name)                                                         \
    for (                                                                   \
        char *_SHUNIT_TESTING_ =                                            \
            (printf("\n--- [%s] starting\n", (_name)), (_name));            \
        _SHUNIT_TESTING_;                                                   \
        _SHUNIT_TESTING_ = NULL, printf("--- [%s] succeeded\n", (_name))    \
    )

/////////////////////////////////////////////////////////////////////////////

#define _SHUNIT_FAIL_PRINT(_msg, ...)                                       \
    printf("--- [%s] FAILED at " _msg " (" __FILE__ " line %d)\n",          \
            _SHUNIT_TESTING_,                                               \
            __VA_ARGS__,                                                    \
            __LINE__)

#define _SHUNIT_FAIL_END  ON_TEST_FAIL; break;

#define _SHUNIT_FAIL(_msg, ...)                                             \
    _SHUNIT_FAIL_PRINT(_msg, __VA_ARGS__);                                  \
    _SHUNIT_FAIL_END;

/////////////////////////////////////////////////////////////////////////////

// NOTE: For these, we need to make sure not to evaluate the macro parameters
// more than once (since evaluating them could have other side-effects)!

/**
 * Assert that some condition is true.
 */
#define ASSERT(_cond)                                                       \
    if (!(_cond)) {                                                         \
        _SHUNIT_FAIL("<%s>", #_cond);                                       \
    }

/**
 * Assert that 2 signed integers are equal.
 */
#define ASSERT_INT_EQ(_a, _b)                                               \
    {                                                                       \
        long long _a_value = (_a), _b_value = (_b);                         \
        if (_a_value != _b_value) {                                          \
            _SHUNIT_FAIL("EQ<" #_a ", " #_b ">: %lld != %lld",              \
                    _a_value, _b_value);                                    \
        }                                                                   \
    }

/**
 * Assert that 2 unsigned integers are equal.
 */
#define ASSERT_UINT_EQ(_a, _b)                                              \
    {                                                                       \
        unsigned long long _a_value = (_a), _b_value = (_b);                \
        if (_a_value != _b_value) {                                         \
            _SHUNIT_FAIL("EQ<" #_a ", " #_b ">: %llu != %llu",              \
                    _a_value, _b_value);                                    \
        }                                                                   \
    }

/**
 * Assert that 2 pointers are equal.
 */
#define ASSERT_PTR_EQ(_a, _b)                                               \
    {                                                                       \
        void *_a_value = (_a), *_b_value = (_b);                            \
        if (_a_value != _b_value) {                                         \
            _SHUNIT_FAIL("PTR_EQ<" #_a ", " #_b ">: %p != %p",              \
                    _a_value, _b_value);                                    \
        }                                                                   \
    }

inline static void _shunit_print_mem(
        char *name,
        size_t name_pad,
        void *p,
        size_t sz,
        size_t cnt) {
    size_t name_len = strlen(name);
    char buf[name_pad > name_len ? (name_pad + 1) : 0];
    if (name_pad > name_len) {
        strcpy(buf, name);
        for (size_t i = name_len; i < name_pad; i++) buf[i] = ' ';
        buf[name_pad] = '\0';
        name = buf;
    }

    printf("    %s:  ", name);
    unsigned char *pp = (unsigned char *) p;
    for (size_t i = 0; i < (sz * cnt); i++) {
        if (i > 0) {
            printf(" ");
            if (i % sz == 0) {
                printf(" ");
            }
        }
        printf("%02X", pp[i]);
    }
    printf("\n");
}

inline static void _shunit_print_mem_cmp(
        char *p1_name,
        char *p2_name,
        void *p1,
        void *p2,
        size_t sz,
        size_t cnt) {
    size_t p1_name_len = strlen(p1_name),
           p2_name_len = strlen(p2_name);
    size_t name_pad = p1_name_len > p2_name_len ? p1_name_len : p2_name_len;
    _shunit_print_mem(p1_name, name_pad, p1, sz, cnt);
    _shunit_print_mem(p2_name, name_pad, p2, sz, cnt);
}

/**
 * Assert that 2 chunks of memory are equal.
 *
 * Note that this still evaluates each parameter exactly once, since "sizeof"
 * is compile-time.
 */
#define ASSERT_MEM_EQ(_p1, _p2, _size)                                      \
    if (sizeof(*(_p1)) != sizeof(*(_p2))) {                                 \
        void *_p1_value = (_p1), *_p2_value = (_p2);                        \
        (void) _p1_value; (void) _p2_value;                                 \
        _SHUNIT_FAIL("MEM_EQ<" #_p1 ", " #_p2 ">: "                         \
                "sizeof(*(" #_p1 ")) = %zu; sizeof(*(" #_p2 ")) = %zu",     \
                sizeof(*(_p1)), sizeof(*(_p2)));                            \
    } else {                                                                \
        void *_p1_value = (_p1), *_p2_value = (_p2);                        \
        if (memcmp(_p1_value, _p2_value, sizeof(*(_p1)) * (_size)) != 0) {  \
            _SHUNIT_FAIL_PRINT("MEM_EQ<" #_p1 ", " #_p2 "> "                \
                    "for %zu items / %zu bytes",                            \
                    (size_t) (_size), (size_t) (_size) * sizeof(*(_p1)));   \
            _shunit_print_mem_cmp(#_p1, #_p2, _p1_value, _p2_value,         \
                    sizeof(*(_p1)), (_size));                               \
            _SHUNIT_FAIL_END;                                               \
        }                                                                   \
    }
