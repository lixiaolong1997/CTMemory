#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "greatest.h"

extern SUITE(test_basic);
extern SUITE(test_rbtree);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(test_basic);
    RUN_SUITE(test_rbtree);

    GREATEST_MAIN_END();

    return 0;
}
