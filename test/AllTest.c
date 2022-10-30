#include <stdio.h>

#include <CuTest.h>
#include "Kernel/lib.h"
#include "Kernel/mem/memory.h"
#include "Kernel/semaphore.h"
#include "Kernel/pipes.h"

void RunAllTests(void)
{
        CuString *output = CuStringNew();
        CuSuite *suite = CuSuiteNew();

        CuSuiteAddSuite(suite, test_get_lib_suite());
        CuSuiteAddSuite(suite, test_get_memory_suite());
        CuSuiteAddSuite(suite, test_get_semaphore_suite());
        CuSuiteAddSuite(suite, test_get_pipes_suite());
        printf("[INFO] All suites loaded.\n");

        printf("[INFO] Running tests...\n");
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
