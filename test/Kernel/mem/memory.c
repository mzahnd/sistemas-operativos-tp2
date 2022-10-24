#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Header to test */
#define MEM_HEAP_SIZE (2 * 1024 * 1024) // 2 MiB. For faster tests
#include "../../../Kernel/include/mem/memory.h"

/* This file's header */
#include "memory.h"

#define MAX_TEST_ALLOCATIONS 512

#define TEST_ALLOC_SIZE 1024
#define TEST_SMALL_ALLOC_SIZE 1
#define WRITTEN_VALUE '#'

/* ---------- somalloc ---------- */
void test_somalloc_simple_alloc(CuTest *const ct);
void test_somalloc_multiple_alloc(CuTest *const ct);
void test_somalloc_write(CuTest *const ct);
void test_somalloc_small_simple_alloc(CuTest *const ct);
void test_somalloc_small_multiple_alloc(CuTest *const ct);
void test_somalloc_small_write(CuTest *const ct);
/* ---------- socalloc ---------- */
void test_socalloc_simple_alloc(CuTest *const ct);
void test_socalloc_multiple_alloc(CuTest *const ct);
void test_socalloc_write(CuTest *const ct);
void test_socalloc_small_simple_alloc(CuTest *const ct);
void test_socalloc_small_multiple_alloc(CuTest *const ct);
void test_socalloc_small_write(CuTest *const ct);
/* ---------- sofree ---------- */
void test_sofree_free_old_allocs(CuTest *const ct);
void test_sofree_free_all_heap(CuTest *const ct);
void test_sofree_right_join_middle(CuTest *const ct);
void test_sofree_left_join_middle(CuTest *const ct);
void test_sofree_free_between_blocks(CuTest *const ct);

// Pointers to allocated memory using so(m|c|re)alloc functions. Freed with
// sofree()
static size_t n_allocated = 0;
static uint8_t *allocated[MAX_TEST_ALLOCATIONS] = { 0 };

/* In memory.c */
typedef struct MEMORY_BLOCK {
        struct MEMORY_BLOCK *next;
        size_t size; /* Total block size */
        size_t user_size; /* User available */
} memory_block;

#define BYTE_ALIGNMENT 32
#define BYTE_ALIGNMENT_MASK (0x001f)
static const size_t memblock_size =
        ((sizeof(memory_block) + (size_t)(BYTE_ALIGNMENT - 1)) &
         ~((size_t)BYTE_ALIGNMENT_MASK));

CuSuite *test_get_memory_suite(void)
{
        printf("[INFO] Running memory suite.\n");
        CuSuite *const suite = CuSuiteNew();

        printf("[INFO] MEM_HEAP_SIZE: %ld\n", MEM_HEAP_SIZE);
        printf("[INFO] sizeof(memory_block): %ld\n", sizeof(memory_block));

        /* somalloc */
        SUITE_ADD_TEST(suite, test_somalloc_simple_alloc);
        SUITE_ADD_TEST(suite, test_somalloc_multiple_alloc);
        SUITE_ADD_TEST(suite, test_somalloc_write);
        SUITE_ADD_TEST(suite, test_somalloc_small_simple_alloc);
        SUITE_ADD_TEST(suite, test_somalloc_small_multiple_alloc);
        SUITE_ADD_TEST(suite, test_somalloc_small_write);

        /* socalloc */
        SUITE_ADD_TEST(suite, test_socalloc_simple_alloc);
        SUITE_ADD_TEST(suite, test_socalloc_multiple_alloc);
        SUITE_ADD_TEST(suite, test_socalloc_write);
        SUITE_ADD_TEST(suite, test_socalloc_small_simple_alloc);
        SUITE_ADD_TEST(suite, test_socalloc_small_multiple_alloc);
        SUITE_ADD_TEST(suite, test_socalloc_small_write);

        /* sofree */
        SUITE_ADD_TEST(suite, test_sofree_free_old_allocs);
        SUITE_ADD_TEST(suite, test_sofree_free_all_heap);
        SUITE_ADD_TEST(suite, test_sofree_right_join_middle);
        SUITE_ADD_TEST(suite, test_sofree_left_join_middle);
        SUITE_ADD_TEST(suite, test_sofree_free_between_blocks);

        printf("[INFO] Done memory suite.\n");
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

void test_somalloc_small_simple_alloc(CuTest *const ct)
{
        uint8_t *somalloc_ptr =
                (uint8_t *)somalloc(TEST_SMALL_ALLOC_SIZE * sizeof(uint8_t));

        CuAssertPtrNotNull(ct, somalloc_ptr);

        allocated[n_allocated++] = somalloc_ptr;
}

void test_somalloc_small_multiple_alloc(CuTest *const ct)
{
        uint8_t *somalloc_ptr_1 =
                (uint8_t *)somalloc(TEST_SMALL_ALLOC_SIZE * sizeof(uint8_t));
        uint8_t *somalloc_ptr_2 =
                (uint8_t *)somalloc(TEST_SMALL_ALLOC_SIZE * sizeof(uint8_t));

        CuAssertPtrNotNull(ct, somalloc_ptr_1);
        CuAssertPtrNotNull(ct, somalloc_ptr_2);

        CuAssertTrue(ct, somalloc_ptr_1 != somalloc_ptr_2);

        allocated[n_allocated++] = somalloc_ptr_1;
        allocated[n_allocated++] = somalloc_ptr_2;
}

void test_somalloc_small_write(CuTest *const ct)
{
        uint8_t ok_str[TEST_SMALL_ALLOC_SIZE] = { 0 };
        uint8_t *somalloc_ptr =
                (uint8_t *)somalloc(TEST_SMALL_ALLOC_SIZE * sizeof(uint8_t));

        CuAssertPtrNotNull(ct, somalloc_ptr);

        memset(&ok_str, WRITTEN_VALUE, TEST_SMALL_ALLOC_SIZE);
        memset(somalloc_ptr, WRITTEN_VALUE, TEST_SMALL_ALLOC_SIZE);

        for (int i = 0; i < TEST_SMALL_ALLOC_SIZE; i++) {
                CuAssertIntEquals(ct, (int)ok_str[i], (int)somalloc_ptr[i]);
        }

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

void test_socalloc_small_simple_alloc(CuTest *const ct)
{
        uint8_t *socalloc_ptr =
                (uint8_t *)socalloc(TEST_SMALL_ALLOC_SIZE, sizeof(uint8_t));

        CuAssertPtrNotNull(ct, socalloc_ptr);

        allocated[n_allocated++] = socalloc_ptr;
}

void test_socalloc_small_multiple_alloc(CuTest *const ct)
{
        uint8_t *socalloc_ptr_1 =
                (uint8_t *)socalloc(TEST_SMALL_ALLOC_SIZE, sizeof(uint8_t));
        uint8_t *socalloc_ptr_2 =
                (uint8_t *)socalloc(TEST_SMALL_ALLOC_SIZE, sizeof(uint8_t));

        CuAssertPtrNotNull(ct, socalloc_ptr_1);
        CuAssertPtrNotNull(ct, socalloc_ptr_2);

        CuAssertTrue(ct, socalloc_ptr_1 != socalloc_ptr_2);

        allocated[n_allocated++] = socalloc_ptr_1;
        allocated[n_allocated++] = socalloc_ptr_2;
}

void test_socalloc_small_write(CuTest *const ct)
{
        uint8_t ok_str[TEST_SMALL_ALLOC_SIZE] = { 0 };
        uint8_t *socalloc_ptr =
                (uint8_t *)socalloc(TEST_SMALL_ALLOC_SIZE, sizeof(uint8_t));

        CuAssertPtrNotNull(ct, socalloc_ptr);

        memset(&ok_str, WRITTEN_VALUE, TEST_SMALL_ALLOC_SIZE);
        memset(socalloc_ptr, WRITTEN_VALUE, TEST_SMALL_ALLOC_SIZE);

        for (int i = 0; i < TEST_SMALL_ALLOC_SIZE; i++) {
                CuAssertIntEquals(ct, (int)ok_str[i], (int)socalloc_ptr[i]);
        }

        allocated[n_allocated++] = socalloc_ptr;
}

/* ---------- sofree ---------- */
void test_sofree_free_old_allocs(CuTest *const ct)
{
        for (int i = 0; i < n_allocated; i++) {
                sofree(allocated[i]);
                allocated[i] = 0;
        }
        n_allocated = 0;
}

void test_sofree_free_all_heap(CuTest *const ct)
{
        uint8_t ok_buffer[MEM_HEAP_SIZE] = { 0 };

        size_t extra_space_for_header = memblock_size + BYTE_ALIGNMENT -
                                        (MEM_HEAP_SIZE & BYTE_ALIGNMENT_MASK);
        size_t wanted_size = MEM_HEAP_SIZE - extra_space_for_header;

        uint8_t *somalloc_all_heap = (uint8_t *)somalloc(wanted_size);
        CuAssertPtrNotNull(ct, somalloc_all_heap);
        sofree(somalloc_all_heap);

        // Again
        uint8_t *somalloc_all_heap_2 = (uint8_t *)somalloc(wanted_size);
        CuAssertPtrNotNull(ct, somalloc_all_heap_2);

        // Try to write it
        memset(somalloc_all_heap_2, WRITTEN_VALUE, wanted_size);
        memset(ok_buffer, WRITTEN_VALUE, wanted_size);

        if (memcmp(somalloc_all_heap_2, ok_buffer, wanted_size) != 0) {
                CuFail(ct, "Writting the hole heap failed");
        }

        sofree(somalloc_all_heap_2);
}

void test_sofree_right_join_middle(CuTest *const ct)
{
        uint8_t ok_buffer[MEM_HEAP_SIZE / 2] = { 0 };

        size_t extra_space_for_header =
                memblock_size + BYTE_ALIGNMENT -
                (MEM_HEAP_SIZE / 4 & BYTE_ALIGNMENT_MASK);

        uint8_t *somalloc_ptr_right_bound =
                (uint8_t *)somalloc(MEM_HEAP_SIZE / 4);
        uint8_t *somalloc_ptr_mid_right =
                (uint8_t *)somalloc(MEM_HEAP_SIZE / 4);
        uint8_t *somalloc_ptr_mid_left = (uint8_t *)somalloc(MEM_HEAP_SIZE / 4);
        uint8_t *somalloc_ptr_left_bound = (uint8_t *)somalloc(
                MEM_HEAP_SIZE / 4 - 4 * extra_space_for_header);

        CuAssertPtrNotNull(ct, somalloc_ptr_right_bound);
        CuAssertPtrNotNull(ct, somalloc_ptr_mid_right);
        CuAssertPtrNotNull(ct, somalloc_ptr_mid_left);
        CuAssertPtrNotNull(ct, somalloc_ptr_left_bound);

        // Notice this is swapped compared to test_sofree_left_join_middle
        sofree(somalloc_ptr_mid_right);
        sofree(somalloc_ptr_mid_left);

        uint8_t *somalloc_ptr_mid = (uint8_t *)somalloc(MEM_HEAP_SIZE / 2);
        CuAssertPtrNotNull(ct, somalloc_ptr_mid);

        // Try to write it
        memset(somalloc_ptr_mid, WRITTEN_VALUE, MEM_HEAP_SIZE / 2);
        memset(ok_buffer, WRITTEN_VALUE, MEM_HEAP_SIZE / 2);

        if (memcmp(somalloc_ptr_mid, ok_buffer, MEM_HEAP_SIZE / 2) != 0) {
                CuFail(ct, "Writting the hole heap failed");
        }

        sofree(somalloc_ptr_right_bound);
        sofree(somalloc_ptr_mid);
        sofree(somalloc_ptr_left_bound);
}

void test_sofree_left_join_middle(CuTest *const ct)
{
        uint8_t ok_buffer[MEM_HEAP_SIZE / 2] = { 0 };

        size_t extra_space_for_header =
                memblock_size + BYTE_ALIGNMENT -
                (MEM_HEAP_SIZE / 4 & BYTE_ALIGNMENT_MASK);

        uint8_t *somalloc_ptr_right_bound =
                (uint8_t *)somalloc(MEM_HEAP_SIZE / 4);
        uint8_t *somalloc_ptr_mid_right =
                (uint8_t *)somalloc(MEM_HEAP_SIZE / 4);
        uint8_t *somalloc_ptr_mid_left = (uint8_t *)somalloc(MEM_HEAP_SIZE / 4);
        uint8_t *somalloc_ptr_left_bound = (uint8_t *)somalloc(
                MEM_HEAP_SIZE / 4 - 4 * extra_space_for_header);

        CuAssertPtrNotNull(ct, somalloc_ptr_right_bound);
        CuAssertPtrNotNull(ct, somalloc_ptr_mid_right);
        CuAssertPtrNotNull(ct, somalloc_ptr_mid_left);
        CuAssertPtrNotNull(ct, somalloc_ptr_left_bound);

        // Notice this is swapped compared to test_sofree_right_join_middle
        sofree(somalloc_ptr_mid_left);
        sofree(somalloc_ptr_mid_right);

        uint8_t *somalloc_ptr_mid = (uint8_t *)somalloc(MEM_HEAP_SIZE / 2);
        CuAssertPtrNotNull(ct, somalloc_ptr_mid);

        // Try to write it
        memset(somalloc_ptr_mid, WRITTEN_VALUE, MEM_HEAP_SIZE / 2);
        memset(ok_buffer, WRITTEN_VALUE, MEM_HEAP_SIZE / 2);

        if (memcmp(somalloc_ptr_mid, ok_buffer, MEM_HEAP_SIZE / 2) != 0) {
                CuFail(ct, "Writting the hole heap failed");
        }

        sofree(somalloc_ptr_left_bound);
        sofree(somalloc_ptr_mid);
        sofree(somalloc_ptr_right_bound);
}

void test_sofree_free_between_blocks(CuTest *const ct)
{
        uint8_t ok_buffer[MEM_HEAP_SIZE / 2] = { 0 };

        size_t extra_space_for_header =
                memblock_size + BYTE_ALIGNMENT -
                (MEM_HEAP_SIZE / 4 & BYTE_ALIGNMENT_MASK);

        uint8_t *somalloc_ptr_right_bound =
                (uint8_t *)somalloc(MEM_HEAP_SIZE / 4);
        uint8_t *somalloc_ptr_mid_right =
                (uint8_t *)somalloc(MEM_HEAP_SIZE / 4);
        uint8_t *somalloc_ptr_mid_left = (uint8_t *)somalloc(MEM_HEAP_SIZE / 4);
        uint8_t *somalloc_ptr_left_bound = (uint8_t *)somalloc(
                MEM_HEAP_SIZE / 4 - 4 * extra_space_for_header);

        CuAssertPtrNotNull(ct, somalloc_ptr_right_bound);
        CuAssertPtrNotNull(ct, somalloc_ptr_mid_right);
        CuAssertPtrNotNull(ct, somalloc_ptr_mid_left);
        CuAssertPtrNotNull(ct, somalloc_ptr_left_bound);

        sofree(somalloc_ptr_mid_left);
        sofree(somalloc_ptr_mid_right);

        uint8_t *somalloc_ptr_mid = (uint8_t *)somalloc(MEM_HEAP_SIZE / 2);
        CuAssertPtrNotNull(ct, somalloc_ptr_mid);

        // Try to write it
        memset(somalloc_ptr_mid, WRITTEN_VALUE, MEM_HEAP_SIZE / 2);
        memset(ok_buffer, WRITTEN_VALUE, MEM_HEAP_SIZE / 2);

        if (memcmp(somalloc_ptr_mid, ok_buffer, MEM_HEAP_SIZE / 2) != 0) {
                CuFail(ct, "Writting the hole heap failed");
        }

        /* ================================================================
         * Up to this point is very similar to test_sofree_left_join_middle 
         * ================================================================ 
         */

        // Notice the order: first frees both bounds, and lastly the middle
        // Reference: '(*)' means "about to be freed"
        // | user owned (*) | user owned | user owned |
        // | free left bound | user owned | user owned (*) |
        // | free left bound | user owned (*) | free right bound |
        // | free left bound | free middle | free right bound |
        //                          \-> transitions into full heap freed
        sofree(somalloc_ptr_left_bound);
        sofree(somalloc_ptr_right_bound);
        sofree(somalloc_ptr_mid);

        // Alloc the whole heap to make sure it has been properly freed
        extra_space_for_header = memblock_size + BYTE_ALIGNMENT -
                                 (MEM_HEAP_SIZE & BYTE_ALIGNMENT_MASK);
        size_t wanted_size = MEM_HEAP_SIZE - extra_space_for_header;

        uint8_t *somalloc_all_heap = (uint8_t *)somalloc(wanted_size);
        CuAssertPtrNotNull(ct, somalloc_all_heap);

        sofree(somalloc_all_heap);
}
