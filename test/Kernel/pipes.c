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
#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h> /* memset */
#include <unistd.h> /* usleep() */

/* Header to test */
#include "../../Kernel/include/pipes.h"

/* This file's header */
#include "pipes.h"

#define TEST_WRITE_CHAR '#'
#define TEST_WRITE_MESSAGE "You shall not pass"

#define MS_TO_US(n) ((unsigned int)(n)*1000)

typedef struct {
        CuTest *const ct;

        char *buf;
        size_t len;
        int fd;
} test_sowrite_soread_ipc_t;

/* ---------- sopipe & soclose ---------- */
void test_sopipe_soclose_create_and_close(CuTest *const ct);
/* ---------- sowrite & soread ---------- */
void test_sowrite_soread_fill_buffer(CuTest *const ct);
void test_sowrite_soread_overflow_buffer(CuTest *const ct);
void test_sowrite_soread_small_writes(CuTest *const ct);
void test_sowrite_soread_ipc_full_buff(CuTest *const ct);
void *test_sowrite_soread_ipc_thread_reader(void *arg);
void *test_sowrite_soread_ipc_thread_writer(void *arg);

CuSuite *test_get_pipes_suite(void)
{
        CuSuite *const suite = CuSuiteNew();

        /* sopipe & soclose */
        SUITE_ADD_TEST(suite, test_sopipe_soclose_create_and_close);
        /* sowrite & soread */
        SUITE_ADD_TEST(suite, test_sowrite_soread_fill_buffer);
        SUITE_ADD_TEST(suite, test_sowrite_soread_small_writes);
        SUITE_ADD_TEST(suite, test_sowrite_soread_ipc_full_buff);

        printf("[INFO] Loaded pipes suite.\n");

        return suite;
}

/* ---------- sopipe & soclose ---------- */
void test_sopipe_soclose_create_and_close(CuTest *const ct)
{
        int ret = 0;
        int fd[PIPE_N_FD] = { 0 };
        int fd2[PIPE_N_FD] = { 0 };

        ret = sopipe(fd);
        CuAssertIntEquals(ct, 0, ret);

        // First file descriptor ever created: expects index 3
        // Remember: fd 0 "is" stdin, 1 stdout and 2 stderr
        CuAssertIntEquals(ct, 3, fd[PIPE_FD_READ]);
        CuAssertIntEquals(ct, 3, fd[PIPE_FD_WRITE]);

        ret = sopipe(fd2);
        CuAssertIntEquals(ct, 0, ret);

        CuAssertIntEquals(ct, 4, fd[PIPE_FD_READ]);
        CuAssertIntEquals(ct, 4, fd[PIPE_FD_WRITE]);

        // fd
        ret = soclose(fd[PIPE_FD_READ]);
        CuAssertIntEquals(ct, 0, ret);
        ret = soclose(fd[PIPE_FD_WRITE]);
        CuAssertIntEquals(ct, 0, ret);

        // fd2
        ret = soclose(fd2[PIPE_FD_READ]);
        CuAssertIntEquals(ct, 0, ret);
        ret = soclose(fd2[PIPE_FD_WRITE]);
        CuAssertIntEquals(ct, 0, ret);
}

/* ---------- sowrite & soread ---------- */
void test_sowrite_soread_fill_buffer(CuTest *const ct)
{
        int ret = 0;
        int fd[PIPE_N_FD] = { 0 };
        char write_buf[PIPE_BUFFER_SIZE] = { 0 };
        char read_buf[PIPE_BUFFER_SIZE] = { 0 };

        ret = sopipe(fd);
        CuAssertIntEquals(ct, 0, ret);

        memset(&write_buf, TEST_WRITE_CHAR, PIPE_BUFFER_SIZE);

        ret = sowrite(fd[PIPE_FD_WRITE], write_buf, PIPE_BUFFER_SIZE);
        CuAssertIntEquals(ct, PIPE_BUFFER_SIZE, ret);
        ret = soread(fd[PIPE_FD_READ], read_buf, PIPE_BUFFER_SIZE);
        CuAssertIntEquals(ct, PIPE_BUFFER_SIZE, ret);

        ret = memcmp(write_buf, read_buf, PIPE_BUFFER_SIZE);
        CuAssertIntEquals(ct, 0, ret);

        // fd
        ret = soclose(fd[PIPE_FD_READ]);
        CuAssertIntEquals(ct, 0, ret);
        ret = soclose(fd[PIPE_FD_WRITE]);
        CuAssertIntEquals(ct, 0, ret);
}

void test_sowrite_soread_small_writes(CuTest *const ct)
{
        int ret = 0;
        int fd[PIPE_N_FD] = { 0 };
        char write_buf[PIPE_BUFFER_SIZE] = { 0 };
        char read_buf[PIPE_BUFFER_SIZE] = { 0 };

        ret = sopipe(fd);
        CuAssertIntEquals(ct, 0, ret);

        // One byte

        // Let's check it's not writting out of bounds
        const int offset = 10;

        write_buf[offset] = TEST_WRITE_CHAR;

        ret = sowrite(fd[PIPE_FD_WRITE], &write_buf[offset], 1);
        CuAssertIntEquals(ct, 1, ret);
        ret = soread(fd[PIPE_FD_READ], &read_buf[offset], 1);
        CuAssertIntEquals(ct, 1, ret);

        ret = memcmp(write_buf, read_buf, PIPE_BUFFER_SIZE);
        CuAssertIntEquals(ct, 0, ret);

        // (PIPE_BUFFER_SIZE / 8) bytes 10 times
        for (int i = 0; i < 10; i++) {
                // Change character every time to make sure we are reading
                // properly
                memset(&write_buf, TEST_WRITE_CHAR + i, PIPE_BUFFER_SIZE / 8);

                ret = sowrite(fd[PIPE_FD_WRITE], write_buf,
                              PIPE_BUFFER_SIZE / 8);
                CuAssertIntEquals(ct, PIPE_BUFFER_SIZE / 8, ret);
                ret = soread(fd[PIPE_FD_READ], read_buf, PIPE_BUFFER_SIZE / 8);
                CuAssertIntEquals(ct, PIPE_BUFFER_SIZE / 8, ret);

                ret = memcmp(write_buf, read_buf, PIPE_BUFFER_SIZE / 8);
                CuAssertIntEquals(ct, 0, ret);
        }

        ret = soclose(fd[PIPE_FD_READ]);
        CuAssertIntEquals(ct, 0, ret);
        ret = soclose(fd[PIPE_FD_WRITE]);
        CuAssertIntEquals(ct, 0, ret);
}

void test_sowrite_soread_ipc_full_buff(CuTest *const ct)
{
        pthread_t thread_writer = 0;
        pthread_t thread_reader = 0;

        int fd[PIPE_N_FD] = { 0 };
        char write_buf[PIPE_BUFFER_SIZE] = { 0 };
        char read_buf[PIPE_BUFFER_SIZE] = { 0 };
        int ret = 0;

        test_sowrite_soread_ipc_t reader_struct = {
                .ct = ct, .buf = read_buf, .len = PIPE_BUFFER_SIZE, .fd = 0
        };
        test_sowrite_soread_ipc_t writer_struct = {
                .ct = ct, .buf = write_buf, .len = PIPE_BUFFER_SIZE, .fd = 0
        };

        ret = sopipe(fd);
        CuAssertIntEquals(ct, 0, ret);

        reader_struct.fd = fd[PIPE_FD_READ];
        writer_struct.fd = fd[PIPE_FD_WRITE];

        pthread_create(&thread_reader, NULL,
                       test_sowrite_soread_ipc_thread_reader,
                       (void *)&reader_struct);
        usleep(MS_TO_US(500));
        pthread_create(&thread_writer, NULL,
                       test_sowrite_soread_ipc_thread_writer,
                       (void *)&writer_struct);

        pthread_join(thread_reader, NULL);
        pthread_join(thread_writer, NULL);

        ret = memcmp(write_buf, read_buf, 1 + strlen(TEST_WRITE_MESSAGE));
        /* CuAssertIntEquals(ct, 0, ret); */
        CuAssertStrEquals(ct, TEST_WRITE_MESSAGE, read_buf);

        ret = soclose(fd[PIPE_FD_READ]);
        CuAssertIntEquals(ct, 0, ret);
        ret = soclose(fd[PIPE_FD_WRITE]);
        CuAssertIntEquals(ct, 0, ret);
}

void *test_sowrite_soread_ipc_thread_reader(void *arg)
{
        test_sowrite_soread_ipc_t *rs = (test_sowrite_soread_ipc_t *)arg;
        int ret = 0;

        ret = soread(rs->fd, rs->buf, rs->len);
        CuAssertIntEquals(rs->ct, 1 + strlen(TEST_WRITE_MESSAGE), ret);

        return NULL;
}

void *test_sowrite_soread_ipc_thread_writer(void *arg)
{
        int ret = 0;
        test_sowrite_soread_ipc_t *ws = (test_sowrite_soread_ipc_t *)arg;

        ret = sowrite(ws->fd, TEST_WRITE_MESSAGE,
                      1 + strlen(TEST_WRITE_MESSAGE));
        CuAssertIntEquals(ws->ct, 1 + strlen(TEST_WRITE_MESSAGE), ret);

        return NULL;
}
