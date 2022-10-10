
#include "test_memory_manager.h"

CuSuite *test_get_memory_manager_suite(void)
{
        CuSuite *const suite = CuSuiteNew();

        CuSuiteAddSuite(suite, test_get_driver_suite());
        CuSuiteAddSuite(suite, test_get_memory_suite());

        return suite;
}
