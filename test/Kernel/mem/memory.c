#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Header to test */
#include "../../../Kernel/include/mem/memory.h"

/* This file's header */
#include "memory.h"

#define MAX_ALLOCATIONS 512

#define TEST_ALLOC_SIZE 1024
#define WRITTEN_VALUE '#'

/* ---------- somalloc ---------- */
void test_somalloc_simple_alloc(CuTest *const ct);
void test_somalloc_multiple_alloc(CuTest *const ct);
void test_somalloc_write(CuTest *const ct);
/* ---------- socalloc ---------- */
void test_socalloc_simple_alloc(CuTest *const ct);
void test_socalloc_multiple_alloc(CuTest *const ct);
void test_socalloc_write(CuTest *const ct);
/* ---------- sofree ---------- */
void test_sofree(CuTest *const ct);

// Pointers to allocated memory using so(m|c|re)alloc functions. Freed with
// sofree()
static size_t n_allocated = 0;
static uint8_t *allocated[MAX_ALLOCATIONS] = { 0 };

CuSuite *test_get_memory_suite(void)
{
        CuSuite *const suite = CuSuiteNew();

        /* somalloc */
        SUITE_ADD_TEST(suite, test_somalloc_simple_alloc);
        SUITE_ADD_TEST(suite, test_somalloc_multiple_alloc);
        SUITE_ADD_TEST(suite, test_somalloc_write);

        /* socalloc */
        SUITE_ADD_TEST(suite, test_socalloc_simple_alloc);
        SUITE_ADD_TEST(suite, test_socalloc_multiple_alloc);
        SUITE_ADD_TEST(suite, test_socalloc_write);

        /* sofree */
        SUITE_ADD_TEST(suite, test_sofree);

        return suite;
}

/* ---------- somalloc ---------- */
void test_somalloc_simple_alloc(CuTest *const ct)
{
        uint8_t *somalloc_ptr =
                (uint8_t *)somalloc(TEST_ALLOC_SIZE * sizeof(uint8_t));

        CuAssertPtrNotNull(ct, somalloc_ptr);

        allocated[n_allocated++] = somalloc_ptr;
}

void test_somalloc_multiple_alloc(CuTest *const ct)
{
        uint8_t *somalloc_ptr_1 =
                (uint8_t *)somalloc(TEST_ALLOC_SIZE * sizeof(uint8_t));
        uint8_t *somalloc_ptr_2 =
                (uint8_t *)somalloc(TEST_ALLOC_SIZE * sizeof(uint8_t));

        CuAssertPtrNotNull(ct, somalloc_ptr_1);
        CuAssertPtrNotNull(ct, somalloc_ptr_2);

        CuAssertTrue(ct, somalloc_ptr_1 != somalloc_ptr_2);

        allocated[n_allocated++] = somalloc_ptr_1;
        allocated[n_allocated++] = somalloc_ptr_2;
}

void test_somalloc_write(CuTest *const ct)
{
        char ok_str[TEST_ALLOC_SIZE] = { 0 };
        uint8_t *somalloc_ptr =
                (uint8_t *)somalloc(TEST_ALLOC_SIZE * sizeof(uint8_t));

        CuAssertPtrNotNull(ct, somalloc_ptr);

        memset(&ok_str, WRITTEN_VALUE, TEST_ALLOC_SIZE - 1);
        memset(somalloc_ptr, WRITTEN_VALUE, TEST_ALLOC_SIZE - 1);

        CuAssertStrEquals(ct, ok_str, (char *)somalloc_ptr);

        allocated[n_allocated++] = somalloc_ptr;
}

/* ---------- socalloc ---------- */

void test_socalloc_simple_alloc(CuTest *const ct)
{
        uint8_t *socalloc_ptr =
                (uint8_t *)socalloc(TEST_ALLOC_SIZE, sizeof(uint8_t));

        CuAssertPtrNotNull(ct, socalloc_ptr);

        allocated[n_allocated++] = socalloc_ptr;
}

void test_socalloc_multiple_alloc(CuTest *const ct)
{
        uint8_t *socalloc_ptr_1 =
                (uint8_t *)socalloc(TEST_ALLOC_SIZE, sizeof(uint8_t));
        uint8_t *socalloc_ptr_2 =
                (uint8_t *)socalloc(TEST_ALLOC_SIZE, sizeof(uint8_t));

        CuAssertPtrNotNull(ct, socalloc_ptr_1);
        CuAssertPtrNotNull(ct, socalloc_ptr_2);

        CuAssertTrue(ct, socalloc_ptr_1 != socalloc_ptr_2);

        allocated[n_allocated++] = socalloc_ptr_1;
        allocated[n_allocated++] = socalloc_ptr_2;
}

void test_socalloc_write(CuTest *const ct)
{
        char ok_str[TEST_ALLOC_SIZE] = { 0 };
        uint8_t *socalloc_ptr =
                (uint8_t *)socalloc(TEST_ALLOC_SIZE, sizeof(uint8_t));

        CuAssertPtrNotNull(ct, socalloc_ptr);

        memset(&ok_str, WRITTEN_VALUE, TEST_ALLOC_SIZE - 1);
        memset(socalloc_ptr, WRITTEN_VALUE, TEST_ALLOC_SIZE - 1);

        CuAssertStrEquals(ct, ok_str, (char *)socalloc_ptr);

        allocated[n_allocated++] = socalloc_ptr;
}

/* ---------- sofree ---------- */
void test_sofree(CuTest *const ct)
{
        for (int i = 0; i < n_allocated; i++) {
                sofree(allocated[i]);
        }
}
