#include "memory.h"

CuSuite *test_get_memory_suite(void)
{
        CuSuite *const suite = CuSuiteNew();

        return suite;
}
