#include "driver.h"

CuSuite *test_get_driver_suite(void)
{
        CuSuite *const suite = CuSuiteNew();

        return suite;
}
