// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * This file is part of sistemas-operativos-tp2
 * Licensed under BSD 3-Clause "New" or "Revised" License.
 * Copyright (c) 2022 Flores Levalle, M.
 *                    López, P.
 *                    Sierra Pérez, C.
 *                    Zahnd, M. E.
 */
#include <stdio.h>

/* Header to test */
#include "../../Kernel/include/semaphore.h"

/* This file's header */
#include "semaphore.h"

/* ---------- sosem_init ---------- */
void test_sosem_init_sem_0(CuTest *const ct);
void test_sosem_init_sem_n(CuTest *const ct);
void test_sosem_init_NULL_sem(CuTest *const ct);
/* ---------- sosem_destroy ---------- */
void test_sosem_destroy_sem(CuTest *const ct);
void test_sosem_destroy_NULL(CuTest *const ct);
/* ---------- sosem_post ---------- */
void test_sosem_post_non_zero_initial_value(CuTest *const ct);
void test_sosem_post_zero_initial_value(CuTest *const ct);
/* ---------- sosem_wait ---------- */
// TODO: Tests for sosem_wait
// void test_sosem_wait_(CuTest *const ct);
// void test_sosem_wait_(CuTest *const ct);
/* ---------- sosem_getvalue ---------- */
void test_sosem_getvalue_zero_sem(CuTest *const ct);
void test_sosem_getvalue_non_zero_sem(CuTest *const ct);
void test_sosem_getvalue_non_zero_waiting(CuTest *const ct);
void test_sosem_getvalue_NULL_args(CuTest *const ct);

CuSuite *test_get_semaphore_suite(void)
{
        printf("[INFO] Running memory suite.\n");
        CuSuite *const suite = CuSuiteNew();

        /* sosem_init */
        SUITE_ADD_TEST(suite, test_sosem_init_sem_0);
        SUITE_ADD_TEST(suite, test_sosem_init_sem_n);
        SUITE_ADD_TEST(suite, test_sosem_init_NULL_sem);
        /* sosem_destroy */
        SUITE_ADD_TEST(suite, test_sosem_destroy_sem);
        SUITE_ADD_TEST(suite, test_sosem_destroy_NULL);
        /* sosem_post */
        SUITE_ADD_TEST(suite, test_sosem_post_non_zero_initial_value);
        SUITE_ADD_TEST(suite, test_sosem_post_zero_initial_value);

        printf("[INFO] Done memory suite.\n");

        return suite;
}

/* ---------- sosem_init ---------- */
void test_sosem_init_sem_0(CuTest *const ct)
{
        sosem_t sem;

        int ret = sosem_init(&sem, 0);

        CuAssertIntEquals(ct, 0, ret);
        CuAssertIntEquals(ct, 0, sem.value);
        CuAssertIntEquals(ct, 0, atomic_flag_test_and_set(&sem.lock));
        CuAssertIntEquals(ct, 0, sem._n_waiting);

        atomic_flag_clear(&sem.lock); // Clear test and *set*
}

void test_sosem_init_sem_n(CuTest *const ct)
{
        sosem_t sem;

        int ret = sosem_init(&sem, 15);

        CuAssertIntEquals(ct, 0, ret);
        CuAssertIntEquals(ct, 15, sem.value);
        CuAssertIntEquals(ct, 0, atomic_flag_test_and_set(&sem.lock));
        CuAssertIntEquals(ct, 0, sem._n_waiting);

        atomic_flag_clear(&sem.lock); // Clear test and *set*
}

void test_sosem_init_NULL_sem(CuTest *const ct)
{
        int ret = sosem_init(NULL, 0);
        CuAssertIntEquals(ct, -1, ret);

        ret = sosem_init(NULL, 15);
        CuAssertIntEquals(ct, -1, ret);
}

/* ---------- sosem_destroy ---------- */
void test_sosem_destroy_sem(CuTest *const ct)
{
        int ret = 0;
        sosem_t sem;
        ret = sosem_init(&sem, 0);
        CuAssertIntEquals(ct, 0, ret);

        ret = sosem_destroy(&sem);
        CuAssertIntEquals(ct, 0, ret);
}

void test_sosem_destroy_NULL(CuTest *const ct)
{
        int ret = sosem_destroy(NULL);
        CuAssertIntEquals(ct, -1, ret);
}

/* ---------- sosem_post ---------- */
void test_sosem_post_non_zero_initial_value(CuTest *const ct)
{
        const int initial_value = 8;
        int ret = 0;
        sosem_t sem;

        ret = sosem_init(&sem, initial_value);
        CuAssertIntEquals(ct, 0, ret);

        CuAssertIntEquals(ct, initial_value, sem.value);

        // Post 1
        sosem_post(&sem);
        CuAssertIntEquals(ct, initial_value + 1, sem.value);

        // Post 2
        sosem_post(&sem);
        CuAssertIntEquals(ct, initial_value + 2, sem.value);

        // Post 3
        sosem_post(&sem);
        CuAssertIntEquals(ct, initial_value + 3, sem.value);

        ret = sosem_destroy(&sem);
        CuAssertIntEquals(ct, 0, ret);
}

void test_sosem_post_zero_initial_value(CuTest *const ct)
{
        const int initial_value = 0;
        int ret = 0;
        sosem_t sem;

        ret = sosem_init(&sem, initial_value);
        CuAssertIntEquals(ct, 0, ret);

        CuAssertIntEquals(ct, initial_value, sem.value);

        // Post 1
        sosem_post(&sem);
        CuAssertIntEquals(ct, initial_value + 1, sem.value);

        // Post 2
        sosem_post(&sem);
        CuAssertIntEquals(ct, initial_value + 2, sem.value);

        // Post 3
        sosem_post(&sem);
        CuAssertIntEquals(ct, initial_value + 3, sem.value);

        ret = sosem_destroy(&sem);
        CuAssertIntEquals(ct, 0, ret);
}

/* ---------- sosem_getvalue ---------- */
void test_sosem_getvalue_zero_sem(CuTest *const ct)
{
        const int initial_value = 0;
        int ret = 0;
        sosem_t sem;
        unsigned int sval = 0;

        ret = sosem_init(&sem, initial_value);
        CuAssertIntEquals(ct, 0, ret);

        ret = sosem_getvalue(&sem, &sval);
        CuAssertIntEquals(ct, 0, ret);

        CuAssertIntEquals(ct, initial_value, sval);

        // Post + wait
        ret = sosem_post(&sem);
        CuAssertIntEquals(ct, 0, ret);
        ret = sosem_wait(&sem);
        CuAssertIntEquals(ct, 0, ret);

        // Should be zero again
        ret = sosem_getvalue(&sem, &sval);
        CuAssertIntEquals(ct, 0, ret);

        CuAssertIntEquals(ct, initial_value, sval);

        ret = sosem_destroy(&sem);
        CuAssertIntEquals(ct, 0, ret);
}

void test_sosem_getvalue_non_zero_sem(CuTest *const ct)
{
        const int initial_value = 8;
        int ret = 0;
        sosem_t sem;
        unsigned int sval = 0;

        ret = sosem_init(&sem, initial_value);
        CuAssertIntEquals(ct, 0, ret);

        ret = sosem_getvalue(&sem, &sval);
        CuAssertIntEquals(ct, 0, ret);

        CuAssertIntEquals(ct, 0, sval);

        // Post + wait
        ret = sosem_post(&sem);
        CuAssertIntEquals(ct, 0, ret);
        ret = sosem_wait(&sem);
        CuAssertIntEquals(ct, 0, ret);

        // Should be initial_value again
        ret = sosem_getvalue(&sem, &sval);
        CuAssertIntEquals(ct, initial_value, ret);

        CuAssertIntEquals(ct, 0, sval);

        ret = sosem_destroy(&sem);
        CuAssertIntEquals(ct, 0, ret);
}

void test_sosem_getvalue_non_zero_waiting(CuTest *const ct)
{
        // TODO
}

void test_sosem_getvalue_NULL_args(CuTest *const ct)
{
        int ret = 0;

        unsigned int dummy;

        ret = sosem_getvalue(NULL, &dummy);
        CuAssertIntEquals(ct, -1, ret);
        ret = sosem_getvalue((sosem_t *)&dummy, NULL);
        CuAssertIntEquals(ct, -1, ret);

        ret = sosem_getvalue(NULL, NULL);
        CuAssertIntEquals(ct, -1, ret);
}
