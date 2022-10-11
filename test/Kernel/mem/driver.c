#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Header to test */
#include "../../../Kernel/include/mem/driver.h"

/* This file's header */
#include "driver.h"

#define TEST_MEMORY_SIZE 1024
#define WRITTEN_VALUE '#'

void test_somemset(CuTest *const ct);
void test_somemcpy(CuTest *const ct);

CuSuite *test_get_driver_suite(void)
{
        CuSuite *const suite = CuSuiteNew();

        /* somemset */
        SUITE_ADD_TEST(suite, test_somemset);
        /* somemcpy */
        SUITE_ADD_TEST(suite, test_somemcpy);

        return suite;
}

void test_somemset(CuTest *const ct)
{
        uint8_t *test_buffer =
                (uint8_t *)calloc(TEST_MEMORY_SIZE, sizeof(uint8_t));
        if (test_buffer == NULL) {
                perror("malloc");
                exit(1);
        }

        // Also test out of bounds writting (to either side)
        uint8_t *shifted_test_buffer_addr = (uint8_t *)test_buffer + 128;
        uint8_t *returned_ptr = (uint8_t *)somemset(
                shifted_test_buffer_addr, WRITTEN_VALUE, TEST_MEMORY_SIZE / 4);

        CuAssertPtrNotNull(ct, returned_ptr);
        CuAssertPtrEquals(ct, shifted_test_buffer_addr, returned_ptr);

        // Test that no writting has been performed before
        // shifted_test_buffer_addr
        for (uint8_t *ptr = test_buffer; ptr < shifted_test_buffer_addr;
             ptr++) {
                CuAssertIntEquals(ct, 0, *ptr);
        }

        // Test that no writting has been performed after
        // shifted_test_buffer_addr
        // plus the given test_buffer size
        for (uint8_t *ptr =
                     (uint8_t *)shifted_test_buffer_addr + TEST_MEMORY_SIZE / 4;
             ptr < (uint8_t *)test_buffer + TEST_MEMORY_SIZE; ptr++) {
                CuAssertIntEquals(ct, 0, *ptr);
        }

        uint8_t *ok_buffer =
                (uint8_t *)calloc(TEST_MEMORY_SIZE / 4 + 1, sizeof(char));
        if (ok_buffer == NULL) {
                perror("malloc");
                exit(1);
        }

        CuAssertStrEquals(ct,
                          (char *)memset(ok_buffer, WRITTEN_VALUE,
                                         TEST_MEMORY_SIZE / 4),
                          (char *)shifted_test_buffer_addr);

        free(ok_buffer);
        free(test_buffer);
}

void test_somemcpy(CuTest *const ct)
{
        uint8_t *test_buffer =
                (uint8_t *)calloc(TEST_MEMORY_SIZE, sizeof(uint8_t));
        if (test_buffer == NULL) {
                perror("malloc");
                exit(1);
        }

        uint8_t *ok_buffer =
                (uint8_t *)calloc(TEST_MEMORY_SIZE / 4 + 1, sizeof(char));
        if (ok_buffer == NULL) {
                perror("malloc");
                exit(1);
        }

        memset(ok_buffer, WRITTEN_VALUE, TEST_MEMORY_SIZE / 4);

        // Test out of bounds writting (to either side)
        uint8_t *shifted_test_buffer_addr =
                (uint8_t *)test_buffer + TEST_MEMORY_SIZE / 8;
        uint8_t *returned_ptr = (uint8_t *)somemcpy(
                shifted_test_buffer_addr, ok_buffer, TEST_MEMORY_SIZE / 4);

        CuAssertPtrNotNull(ct, returned_ptr);
        CuAssertPtrEquals(ct, shifted_test_buffer_addr, returned_ptr);

        // Test that no writting has been performed before
        // shifted_test_buffer_addr
        for (uint8_t *ptr = test_buffer; ptr < shifted_test_buffer_addr;
             ptr++) {
                CuAssertIntEquals(ct, 0, *ptr);
        }

        // Test that no writting has been performed after
        // shifted_test_buffer_addr
        // plus the given test_buffer size
        for (uint8_t *ptr =
                     (uint8_t *)shifted_test_buffer_addr + TEST_MEMORY_SIZE / 4;
             ptr < (uint8_t *)test_buffer + TEST_MEMORY_SIZE; ptr++) {
                CuAssertIntEquals(ct, 0, *ptr);
        }

        CuAssertStrEquals(ct, (char *)ok_buffer,
                          (char *)shifted_test_buffer_addr);

        free(ok_buffer);
        free(test_buffer);
}
