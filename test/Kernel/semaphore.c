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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Header to test */
#include "../../Kernel/include/semaphore.h"

/* This file's header */
#include "semaphore.h"

#define MAX_TEST_SEMAPHORES 32

#define TEST_REAL_SOSEM_NAME_MAX (SOSEM_NAME_MAX + 2)
#define TEST_SEM_NAME_DUMMY_CHAR '#'
#define TEST_SEM_NAME_INIT_CHAR '/'

/* ---------- sosem_open ---------- */
void test_sosem_open_create_fullname(CuTest *const ct);
void test_sosem_open_create_shortname(CuTest *const ct);
void test_sosem_open_create_negative_inital_value(CuTest *const ct);
void test_sosem_open_existing_open_all(CuTest *const ct);
/* ---------- sosem_close ---------- */
void test_sosem_close_non_existing_sem(CuTest *const ct);
void test_sosem_close_all_test_semaphores(CuTest *const ct);
void test_sosem_close_open_close_reopen(CuTest *const ct);
/* ---------- sosem_post ---------- */
void test_sosem_post_non_zero_initial_value(CuTest *const ct);
void test_sosem_post_zero_initial_value(CuTest *const ct);
/* ---------- suite functions ---------- */
void test_add_test_sem(sosem_t *sem);
void test_free_all_test_sem(void);

typedef struct {
        char name[SOSEM_NAME_MAX];
        sosem_t *sem;
} test_sem_t;

static size_t test_n_semaphores = 0;
static test_sem_t *test_semaphores[MAX_TEST_SEMAPHORES] = { 0 };

CuSuite *test_get_semaphore_suite(void)
{
        printf("[INFO] Running memory suite.\n");
        CuSuite *const suite = CuSuiteNew();

        /* sosem_open */
        SUITE_ADD_TEST(suite, test_sosem_open_create_fullname);
        SUITE_ADD_TEST(suite, test_sosem_open_create_shortname);
        SUITE_ADD_TEST(suite, test_sosem_open_create_negative_inital_value);
        SUITE_ADD_TEST(suite, test_sosem_open_existing_open_all);
        /* sosem_close */
        SUITE_ADD_TEST(suite, test_sosem_close_non_existing_sem);
        SUITE_ADD_TEST(suite, test_sosem_close_all_test_semaphores);
        /* sosem_post */
        SUITE_ADD_TEST(suite, test_sosem_post_non_zero_initial_value);
        SUITE_ADD_TEST(suite, test_sosem_post_zero_initial_value);

        printf("[INFO] Done memory suite.\n");

        return suite;
}

/* ---------- sosem_open ---------- */
void test_sosem_open_create_fullname(CuTest *const ct)
{
        int64_t initial_value = 0;
        char ok_semname[TEST_REAL_SOSEM_NAME_MAX] = { 0 };
        char semname[SOSEM_NAME_MAX] = { 0 };

        ok_semname[0] = TEST_SEM_NAME_INIT_CHAR;
        memset(&ok_semname[1], TEST_SEM_NAME_DUMMY_CHAR, SOSEM_NAME_MAX);

        memset(&semname, TEST_SEM_NAME_DUMMY_CHAR, SOSEM_NAME_MAX);

        sosem_t *sem = sosem_open(semname, initial_value);
        CuAssertPtrNotNull(ct, sem);

        CuAssertIntEquals(ct, initial_value, sem->value);
        CuAssertStrEquals(ct, ok_semname, sem->name);

        test_add_test_sem(sem);
}

void test_sosem_open_create_shortname(CuTest *const ct)
{
        int64_t initial_value = 0;
        char ok_semname[TEST_REAL_SOSEM_NAME_MAX] = { 0 };
        char semname[SOSEM_NAME_MAX] = { 0 };

        ok_semname[0] = TEST_SEM_NAME_INIT_CHAR;
        memset(&ok_semname[1], TEST_SEM_NAME_DUMMY_CHAR, SOSEM_NAME_MAX / 2);

        memset(&semname, TEST_SEM_NAME_DUMMY_CHAR, SOSEM_NAME_MAX / 2);

        sosem_t *sem = sosem_open(semname, initial_value);
        CuAssertPtrNotNull(ct, sem);

        CuAssertIntEquals(ct, initial_value, sem->value);
        CuAssertStrEquals(ct, ok_semname, sem->name);

        test_add_test_sem(sem);
}

void test_sosem_open_create_negative_inital_value(CuTest *const ct)
{
        int64_t initial_value = -5;
        char ok_semname[TEST_REAL_SOSEM_NAME_MAX] = { 0 };
        char semname[SOSEM_NAME_MAX] = { 0 };

        ok_semname[0] = TEST_SEM_NAME_INIT_CHAR;
        memset(&ok_semname[1], TEST_SEM_NAME_DUMMY_CHAR, SOSEM_NAME_MAX / 6);

        memset(&semname, TEST_SEM_NAME_DUMMY_CHAR, SOSEM_NAME_MAX / 6);

        sosem_t *sem = sosem_open(semname, initial_value);
        CuAssertPtrNotNull(ct, sem);

        CuAssertIntEquals(ct, 0, sem->value);
        CuAssertStrEquals(ct, ok_semname, sem->name);

        test_add_test_sem(sem);
}

void test_sosem_open_existing_open_all(CuTest *const ct)
{
        for (size_t i = 0; i < test_n_semaphores; i++) {
                test_sem_t *stored_sem = test_semaphores[i];
                CuAssertPtrNotNull(ct, stored_sem);
                int64_t stored_sem_value = stored_sem->sem->value;

                sosem_t *opened_sem = sosem_open(stored_sem->name, i);

                CuAssertPtrNotNull(ct, opened_sem);
                // Same semaphore
                CuAssertPtrEquals(ct, stored_sem->sem, opened_sem);
                // Value has not been modified while opening
                CuAssertIntEquals(ct, stored_sem_value, opened_sem->value);
                // Name has not been altered while opening
                CuAssertStrEquals(ct, stored_sem->name, opened_sem->name);
        }
}

/* ---------- sosem_close ---------- */
void test_sosem_close_non_existing_sem(CuTest *const ct)
{
        sosem_t dummy_sem = { 0 };
        dummy_sem.value = 123;
        memcpy(&dummy_sem.name, "I do not exist", 14);

        int ret = sosem_close(&dummy_sem);

        CuAssertIntEquals(ct, -1, ret);
}

void test_sosem_close_all_test_semaphores(CuTest *const ct)
{
        for (size_t i = 0; i < test_n_semaphores; i++) {
                test_sem_t *stored_sem = test_semaphores[i];
                CuAssertPtrNotNull(ct, stored_sem);

                int ret = sosem_close(stored_sem->sem);

                CuAssertIntEquals(ct, 0, ret);
        }

        test_free_all_test_sem();
        test_n_semaphores = 0;
}

void test_sosem_close_open_close_reopen(CuTest *const ct)
{
        const char *test_sem_name = "sosem_close_open_close_reopen";
        const int initial_value_1 = 3;
        const int initial_value_2 = 5;
        int ret = 0;

        // Open
        sosem_t *sem = sosem_open(test_sem_name, initial_value_1);
        CuAssertPtrNotNull(ct, sem);

        CuAssertIntEquals(ct, TEST_SEM_NAME_INIT_CHAR, sem->name[0]);
        CuAssertStrEquals(ct, test_sem_name, &(sem->name[0]) + 1);
        CuAssertIntEquals(ct, initial_value_1, sem->value);

        // Close
        ret = sosem_close(sem);
        CuAssertIntEquals(ct, 0, ret);

        // Reopen
        sem = sosem_open(test_sem_name, initial_value_2);
        CuAssertPtrNotNull(ct, sem);

        CuAssertIntEquals(ct, TEST_SEM_NAME_INIT_CHAR, sem->name[0]);
        CuAssertStrEquals(ct, test_sem_name, &(sem->name[0]) + 1);
        CuAssertIntEquals(ct, initial_value_2, sem->value);

        // Close before returning
        ret = sosem_close(sem);
        CuAssertIntEquals(ct, 0, ret);
}

/* ---------- sosem_post ---------- */
void test_sosem_post_non_zero_initial_value(CuTest *const ct)
{
        const char *test_sem_name = "sosem_post";
        const int initial_value = 8;

        sosem_t *sem = sosem_open(test_sem_name, initial_value);
        CuAssertPtrNotNull(ct, sem);

        CuAssertIntEquals(ct, initial_value, sem->value);

        // Post 1
        sosem_post(sem);
        CuAssertIntEquals(ct, initial_value + 1, sem->value);

        // Post 2
        sosem_post(sem);
        CuAssertIntEquals(ct, initial_value + 2, sem->value);

        // Post 3
        sosem_post(sem);
        CuAssertIntEquals(ct, initial_value + 3, sem->value);

        int ret = sosem_close(sem);
        CuAssertIntEquals(ct, 0, ret);
}

void test_sosem_post_zero_initial_value(CuTest *const ct)
{
        const char *test_sem_name = "sosem_post_zero_initial_value";
        const int initial_value = 0;

        sosem_t *sem = sosem_open(test_sem_name, initial_value);
        CuAssertPtrNotNull(ct, sem);

        CuAssertIntEquals(ct, initial_value, sem->value);

        // Post 1
        sosem_post(sem);
        CuAssertIntEquals(ct, initial_value + 1, sem->value);

        // Post 2
        sosem_post(sem);
        CuAssertIntEquals(ct, initial_value + 2, sem->value);

        // Post 3
        sosem_post(sem);
        CuAssertIntEquals(ct, initial_value + 3, sem->value);

        int ret = sosem_close(sem);
        CuAssertIntEquals(ct, 0, ret);
}

/* ---------- suite functions ---------- */
void test_add_test_sem(sosem_t *sem)
{
        test_sem_t *ts = (test_sem_t *)calloc(1, sizeof(test_sem_t));
        if (ts == NULL) {
                perror("calloc");
                exit(1);
        }

        memcpy(ts->name, sem->name, SOSEM_NAME_MAX);
        ts->sem = sem;

        test_semaphores[test_n_semaphores] = ts;
        test_n_semaphores++;
}

/*
 * Assumes all semaphores have been closed externally.
 */
void test_free_all_test_sem(void)
{
        for (size_t i = 0; i < test_n_semaphores; i++) {
                free(test_semaphores[i]);
        }
}
