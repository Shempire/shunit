#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void _shunit_print_mem(
        char *name,
        size_t name_pad,
        void *p,
        size_t sz) {
    size_t name_len = strlen(name);
    char buf[name_pad > name_len ? (name_pad + 1) : 0];
    if (name_pad > name_len) {
        strcpy(buf, name);
        for (size_t i = name_len; i < name_pad; i++) buf[i] = ' ';
        buf[name_pad] = '\0';
        name = buf;
    }

    printf("    %s: ", name);
    unsigned char *pp = (unsigned char *) p;
    for (size_t i = 0; i < sz; i++) {
        if (i > 0) printf(" ");
        printf("%02X", pp[i]);
    }
    printf("\n");
}

static void _shunit_print_mem_cmp(
        char *p1_name,
        char *p2_name,
        void *p1,
        void *p2,
        size_t sz) {
    size_t p1_name_len = strlen(p1_name),
           p2_name_len = strlen(p2_name);
    size_t name_pad = p1_name_len > p2_name_len ? p1_name_len : p2_name_len;
    _shunit_print_mem(p1_name, name_pad, p1, sz);
    _shunit_print_mem(p2_name, name_pad, p2, sz);
}

#define ASSERT_MEM_MATCHES(_p1, _p2, _size)                                 \
    if (sizeof(*(_p1)) != sizeof(*(_p2))) {                                 \
        printf("--- [%s] FAILED at MEM_MATCHES<" #_p1 ", " #_p2 ">: "       \
            "sizeof(*(" #_p1 ")) = %zu; sizeof(*(" #_p2 ")) = %zu "         \
            "(" __FILE__ " line %d)\n", _SHUNIT_TESTING_, sizeof(*(_p1)),   \
            sizeof(*(_p2)), __LINE__);                                      \
        ON_TEST_FAIL;                                                       \
        break;                                                              \
    } else if (memcmp((_p1), (_p2), sizeof(*(_p1)) * (_size)) != 0) {       \
        printf("--- [%s] FAILED at MEM_MATCHES<" #_p1 ", " #_p2 "> "        \
                "for %zu items / %zu bytes (" __FILE__ " line %d)\n",       \
                _SHUNIT_TESTING_, (size_t) (_size),                         \
                (size_t) (_size) * sizeof(*(_p1)), __LINE__);               \
        _shunit_print_mem_cmp(#_p1, #_p2, (_p1), (_p2),                     \
                sizeof(*(_p1)) * (_size));                                  \
        ON_TEST_FAIL;                                                       \
        break;                                                              \
    }
