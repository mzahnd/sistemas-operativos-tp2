#include <stdio.h>

#include <CuTest.h>
#include "Kernel/mem/test_memory_manager.h"

void RunAllTests(void)
{
        CuString *output = CuStringNew();
        CuSuite *suite = CuSuiteNew();

        CuSuiteAddSuite(suite, test_get_memory_manager_suite());

        CuSuiteRun(suite);

        CuSuiteSummary(suite, output);
        CuSuiteDetails(suite, output);

        printf("%s\n", output->buffer);
}

int main(void)
{
        RunAllTests();
        return 0;
}
