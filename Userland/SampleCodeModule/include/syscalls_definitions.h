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
#ifndef SYSCALL_DEF_H
#define SYSCALL_DEF_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h> /* size_t */
#include <stdatomic.h> /* atomic_* */

/* ---------- scheduler ---------- */
#define MAX_PROCESSES_INFO 32

/* ---------- semaphores ---------- */
#define SEM_MAX_NAME_LEN ((2 << 7) - 1) // 254

#define SEM_MAX_WAITING (2 << 4) // 32

/* ---------- memory manager ---------- */
#define MEM_HEAP_START_ADDR (0x100000 << 8)

// HEAP_SIZE can be externally defined
#ifndef MEM_HEAP_SIZE
#define MEM_HEAP_SIZE (256 * 1024 * 1024) // 256 MiB
#endif

/* ------------------------------ */

/* ---------- scheduler ---------- */
typedef struct processInfo_t {
        char *name;
        unsigned int pid;
        unsigned int ppid;
        unsigned int priority;
        unsigned int status;
        unsigned int foreground;
        void *stack;

} processInfo_t;

typedef struct schInfo_t {
        unsigned int totalReady;
        unsigned int totalProcesses;
        processInfo_t processes[MAX_PROCESSES_INFO];
} schInfo_t;

/* ---------- semaphores ---------- */
typedef struct SEM_INFO {
        char *name;
        size_t len;

        unsigned int value;

        uint64_t *waiting_pid;
        size_t n_waiting;
} sem_info_t;

// Circular queue with locked processes' PID
typedef struct _SEM_PID {
        unsigned int _n_waiting;
        uint64_t _queue[SEM_MAX_WAITING];
        unsigned int _index;
} _sem_pid_t;

typedef struct SEM {
        char name[SEM_MAX_NAME_LEN + 1];

        atomic_uint value;
        atomic_flag lock;
        atomic_uint _n_waiting;

        int binary;

        sem_info_t userland;

        _sem_pid_t _processes;
} sem_t;

/* ---------- memory manager ---------- */
typedef struct {
        unsigned int n_reserved_blocks;

        size_t reserved_size;
        size_t user_size;
        size_t free_size;

        // Allocated memory blocks are between
        // first_address and MEM_HEAP_START_ADDR + MEM_HEAP_SIZE
        void *first_address;
} mem_info_t;

#endif /* SYSCALL_DEF_H */
