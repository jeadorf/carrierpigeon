#include "stdbool.h"
#include "stdio.h"

// TODO: find better place

bool assert_true(char *msg, bool b)
{
    if (b == false) {
        printf("Failed: '%s'\n", msg);
        return false;
    }
    return true;
}

bool assert_false(char *msg, bool b)
{
    return assert_true(msg, !b);
}

bool assert_equals_char(char *msg, char expected, char actual)
{
    if (expected != actual) {
        printf("Failed: '%s'. Expected '%c', got '%c'.\n",
                    msg, expected, actual);
        return false;
    }
    return true;
}

bool assert_equals_int(char *msg, int expected, int actual)
{
    if (expected != actual) {
        printf("Failed: '%s'. Expected '%d', got '%d'.\n",
                    msg, expected, actual);
        return false;
    }

    return true;
}

bool assert_equals_string(char *msg, char *expected, char *actual)
{
    // TODO: use strcmp here
    int i;
    for (i = 0; *(expected+i) != '\0' && *(actual+i) != '\0'; i++) {
        if (*expected != *actual) {
            printf("Failed: '%s'. Expected '%s', got '%s'.\n",
                        msg, expected, actual);
            return false;
        }
    }
    if (*(expected+i) != *(actual+i)) {
        printf("Failed: '%s'. Expected '%s', got '%s'.\n",
                    msg, expected, actual);
        return false;
    }

    return true;
}


