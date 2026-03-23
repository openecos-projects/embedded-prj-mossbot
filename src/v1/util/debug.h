#ifndef DEBUG_H
#define DEBUG_H

#define PRINT_ARR(arr, str)                     \
    printf("[%s] data [", str);                 \
    for (uint8_t i = 0; i < LENGTH(arr); i++) { \
        printf("%d", arr[i]);                   \
        if (i < LENGTH(arr) - 1) {              \
            printf(", ");                       \
        }                                       \
        else {                                  \
            printf("]");                        \
        }                                       \
    }                                           \
    printf("\n");                               \

#define PRINT_ARR_PTR(arr, len, str)            \
    printf("[%s] data [", str);                 \
    for (uint8_t i = 0; i < len; i++) {         \
        printf("%d", arr[i]);                   \
        if (i < len - 1) {                      \
            printf(", ");                       \
        }                                       \
        else {                                  \
            printf("]");                        \
        }                                       \
    }                                           \
    printf("\n");

#endif
