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
#include <pthread.h>
#include <string.h> /* memset */
#define _DEFAULT_SOURCE // Enable usleep
#include <unistd.h> /* usleep() */

/* Header to test */
#include "../../Kernel/include/semaphore.h"

/* This file's header */
#include "semaphore.h"

#define TEST_WAIT_WRITE_N_IN_BUFF 10
#define TEST_WAIT_WRITES_BUFF_SIZE (TEST_WAIT_WRITE_N_IN_BUFF * 2 + 1)

#define TEST_SEM_NAME "Louis Reasoner"
#define TEST_SEM_NAME_LEN (14) // Same as strlen(TEST_SEM_NAME)

#define MS_TO_US(n) ((unsigned int)(n)*1000)

typedef struct {
        CuTest *const ct;

        sosem_t sem;

        char buff[TEST_WAIT_WRITES_BUFF_SIZE];
        size_t index;
} test_sosem_wait_t;

/* ---------- sosem_init ---------- */
void test_sosem_init_sem_0(CuTest *const ct);
void test_sosem_init_sem_n(CuTest *const ct);
void test_sosem_init_NULL_sem(CuTest *const ct);
/* ---------- sosem_destroy ---------- */
void test_sosem_destroy_sem(CuTest *const ct);
void test_sosem_destroy_NULL(CuTest *const ct);
/* ---------- sosem_open ---------- */
void test_sosem_open_create_and_reopen(CuTest *const ct);
void test_sosem_open_NULL_args(CuTest *const ct);
void test_sosem_open_long_name(CuTest *const ct);
/* ---------- sosem_close ---------- */
void test_sosem_close_close_recreate(CuTest *const ct);
void test_sosem_close_NULL_args(CuTest *const ct);
/* ---------- sosem_post ---------- */
void test_sosem_post_non_zero_initial_value(CuTest *const ct);
void test_sosem_post_zero_initial_value(CuTest *const ct);
/* ---------- sosem_wait ---------- */
void test_sosem_wait_two_threads(CuTest *const ct);
void *test_sosem_wait_two_threads_threadA(void *arg);
void *test_sosem_wait_two_threads_threadB(void *arg);
void test_sosem_wait_NULL_args(CuTest *const ct);
/* ---------- sosem_getvalue ---------- */
void test_sosem_getvalue_zero_sem(CuTest *const ct);
void test_sosem_getvalue_non_zero_sem(CuTest *const ct);
void test_sosem_getvalue_non_zero_waiting(CuTest *const ct);
void test_sosem_getvalue_NULL_args(CuTest *const ct);

CuSuite *test_get_semaphore_suite(void)
{
        CuSuite *const suite = CuSuiteNew();

        /* sosem_init */
        SUITE_ADD_TEST(suite, test_sosem_init_sem_0);
        SUITE_ADD_TEST(suite, test_sosem_init_sem_n);
        SUITE_ADD_TEST(suite, test_sosem_init_NULL_sem);
        /* sosem_destroy */
        SUITE_ADD_TEST(suite, test_sosem_destroy_sem);
        SUITE_ADD_TEST(suite, test_sosem_destroy_NULL);
        /* sosem_open */
        SUITE_ADD_TEST(suite, test_sosem_open_create_and_reopen);
        SUITE_ADD_TEST(suite, test_sosem_open_NULL_args);
        SUITE_ADD_TEST(suite, test_sosem_open_long_name);
        /* sosem_close */
        SUITE_ADD_TEST(suite, test_sosem_close_close_recreate);
        SUITE_ADD_TEST(suite, test_sosem_close_NULL_args);
        /* sosem_wait */
        SUITE_ADD_TEST(suite, test_sosem_wait_two_threads);
        SUITE_ADD_TEST(suite, test_sosem_wait_NULL_args);
        /* sosem_post */
        SUITE_ADD_TEST(suite, test_sosem_post_non_zero_initial_value);
        SUITE_ADD_TEST(suite, test_sosem_post_zero_initial_value);
        /* sosem_getvalue */
        SUITE_ADD_TEST(suite, test_sosem_getvalue_zero_sem);
        SUITE_ADD_TEST(suite, test_sosem_getvalue_non_zero_sem);
        SUITE_ADD_TEST(suite, test_sosem_getvalue_non_zero_waiting);
        SUITE_ADD_TEST(suite, test_sosem_getvalue_NULL_args);

        printf("[INFO] Loaded semaphores suite.\n");

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

/* ---------- sosem_open ---------- */
void test_sosem_open_create_and_reopen(CuTest *const ct)
{
        const int initial_value = 6;
        int ret = 0;
        char ok_name[1 + SEM_MAX_NAME_LEN] = { 0 };
        sosem_t *sem_1 = NULL;
        sosem_t *sem_2 = NULL;

        memcpy(&ok_name[0], TEST_SEM_NAME, 1 + TEST_SEM_NAME_LEN);

        sem_1 = sosem_open(TEST_SEM_NAME, initial_value);
        CuAssertPtrNotNull(ct, sem_1);

        // Name
        // Note that the comparison is performed ONLY agains the first
        // TEST_SEM_NAME_LEN (plus the string terminator) because the semaphore
        // does not guarantee ereasing the rest of its name buffer
        ret = memcmp(ok_name, sem_1->name, 1 + TEST_SEM_NAME_LEN); //-V512
        CuAssertIntEquals(ct, 0, ret);

        // Value
        CuAssertIntEquals(ct, initial_value, sem_1->value);

        sem_2 = sosem_open(TEST_SEM_NAME, initial_value + 2);
        CuAssertPtrNotNull(ct, sem_2);

        // Same semaphore
        CuAssertPtrEquals(ct, sem_1, sem_2);

        // Name
        // Note that the comparison is performed ONLY agains the first
        // TEST_SEM_NAME_LEN (plus the string terminator) because the semaphore
        // does not guarantee ereasing the rest of its name buffer
        ret = memcmp(ok_name, sem_2->name, 1 + TEST_SEM_NAME_LEN); //-V512
        CuAssertIntEquals(ct, 0, ret);

        // Value
        CuAssertIntEquals(ct, initial_value, sem_2->value);

        sosem_close(sem_1);
        sosem_close(sem_2);
}

void test_sosem_open_NULL_args(CuTest *const ct)
{
        const int initial_value = 0;
        sosem_t *sem = NULL;

        sem = sosem_open(NULL, initial_value);
        CuAssertPtrEquals(ct, NULL, sem);
}

void test_sosem_open_long_name(CuTest *const ct)
{
        const int initial_value = 8;
        int ret = 0;
        char ok_name[1 + SEM_MAX_NAME_LEN] = { 0 };
        sosem_t *sem = NULL;

        ok_name[0] = TEST_SEM_NAME[0];

        sem = sosem_open(&ok_name[0], initial_value);
        CuAssertPtrNotNull(ct, sem);

        CuAssertStrEquals(ct, ok_name, sem->name);
        CuAssertIntEquals(ct, initial_value, sem->value);

        ret = sosem_close(sem);
        CuAssertIntEquals(ct, 0, ret);
}

/* ---------- sosem_close ---------- */
void test_sosem_close_close_recreate(CuTest *const ct)
{
        const int initial_value_1 = 3;
        const int initial_value_2 = initial_value_1 + 2;
        int ret = 0;
        char ok_name[1 + SEM_MAX_NAME_LEN] = { 0 };
        sosem_t *sem = NULL;

        memcpy(&ok_name[0], TEST_SEM_NAME, 1 + TEST_SEM_NAME_LEN);

        // First time: create and close
        sem = sosem_open(TEST_SEM_NAME, initial_value_1);
        CuAssertPtrNotNull(ct, sem);

        CuAssertStrEquals(ct, ok_name, sem->name);
        CuAssertIntEquals(ct, initial_value_1, sem->value);

        ret = sosem_close(sem);
        CuAssertIntEquals(ct, 0, ret);

        // Second time: create again with a new initial value (same name)
        sem = sosem_open(TEST_SEM_NAME, initial_value_2);
        CuAssertPtrNotNull(ct, sem);

        CuAssertStrEquals(ct, ok_name, sem->name);
        CuAssertIntEquals(ct, initial_value_2, sem->value);

        ret = sosem_close(sem);
        CuAssertIntEquals(ct, 0, ret);
}

void test_sosem_close_NULL_args(CuTest *const ct)
{
        int ret = 0;

        ret = sosem_close(NULL);
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

/* ---------- sosem_wait ---------- */
void test_sosem_wait_two_threads(CuTest *const ct)
{
        pthread_t threadA = 0;
        pthread_t threadB = 0;

        const int initial_value = 1; // For threadA
        char ok_buff[TEST_WAIT_WRITES_BUFF_SIZE] = { 0 };
        int ret = 0;
        test_sosem_wait_t output = { .ct = ct };

        // Excepted output: AAA...BBB...
        memset(&ok_buff, 'A', TEST_WAIT_WRITE_N_IN_BUFF);
        memset(&(ok_buff[TEST_WAIT_WRITE_N_IN_BUFF]), 'B',
               TEST_WAIT_WRITE_N_IN_BUFF);
        ok_buff[TEST_WAIT_WRITES_BUFF_SIZE - 1] = '\0';

        // Initialize structure
        ret = sosem_init(&output.sem, initial_value);
        CuAssertIntEquals(ct, 0, ret);

        output.index = 0;
        memset(&output.buff, 0, TEST_WAIT_WRITES_BUFF_SIZE);

        // Create and join threads
        pthread_create(&threadA, NULL, test_sosem_wait_two_threads_threadA,
                       (void *)&output);
        usleep(MS_TO_US(500));
        pthread_create(&threadB, NULL, test_sosem_wait_two_threads_threadB,
                       (void *)&output);

        pthread_join(threadA, NULL);
        pthread_join(threadB, NULL);

        // Force NULL-terminating string
        output.buff[TEST_WAIT_WRITES_BUFF_SIZE - 1] = '\0';

        // Test it!
        CuAssertIntEquals(ct, TEST_WAIT_WRITES_BUFF_SIZE - 1, output.index);
        CuAssertStrEquals(ct, ok_buff, output.buff);

        sosem_destroy(&output.sem);
}

/**
 * Slowly writes letter A in the shared buffer TEST_WAIT_WRITE_N_IN_BUFF times.
 *
 * Another thread should be trying to write the buffer at the same time.
 */
void *test_sosem_wait_two_threads_threadA(void *arg)
{
        test_sosem_wait_t *out = (test_sosem_wait_t *)arg;
        int ret = 0;

        ret = sosem_wait(&out->sem);
        CuAssertIntEquals(out->ct, 0, ret);

        for (size_t i = 0; i < TEST_WAIT_WRITE_N_IN_BUFF; i++) {
                out->buff[out->index++] = 'A';
                usleep(MS_TO_US(500));
        }

        ret = sosem_post(&out->sem);
        CuAssertIntEquals(out->ct, 0, ret);

        return NULL;
}

/**
 * Tries to write letter B TEST_WAIT_WRITE_N_IN_BUFF times in the shared buffer
 *
 * Another thread should be trying to write the buffer at the same time.
 */
void *test_sosem_wait_two_threads_threadB(void *arg)
{
        test_sosem_wait_t *out = (test_sosem_wait_t *)arg;
        int ret = 0;

        char t_buff[TEST_WAIT_WRITE_N_IN_BUFF] = { 0 };
        memset(&t_buff, 'B', TEST_WAIT_WRITE_N_IN_BUFF);

        ret = sosem_wait(&out->sem);
        CuAssertIntEquals(out->ct, 0, ret);

        memcpy(out->buff + out->index, &t_buff, TEST_WAIT_WRITE_N_IN_BUFF);
        out->index += TEST_WAIT_WRITE_N_IN_BUFF;

        ret = sosem_post(&out->sem);
        CuAssertIntEquals(out->ct, 0, ret);

        return NULL;
}

void test_sosem_wait_NULL_args(CuTest *const ct)
{
        int ret = 0;

        ret = sosem_wait(NULL);
        CuAssertIntEquals(ct, -1, ret);
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
        CuAssertIntEquals(ct, initial_value, sval);

        // Post + wait
        ret = sosem_post(&sem);
        CuAssertIntEquals(ct, 0, ret);
        ret = sosem_wait(&sem);
        CuAssertIntEquals(ct, 0, ret);

        // Should be initial_value again
        ret = sosem_getvalue(&sem, &sval);
        CuAssertIntEquals(ct, 0, ret);
        CuAssertIntEquals(ct, initial_value, sval);

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
