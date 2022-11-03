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
#include <syscalls_asm.h>
#include <semaphoreUser.h>

sem_t *sem_open(const char *name, unsigned int initial_value)
{
        sem_t *sp;
        semaphoreOpenSyscall(name, initial_value, &sp);
        return sp;
}

int sem_close(sem_t *sem)
{
        int ret;
        semaphoreCloseSyscall(sem, &ret);
        return ret;
}
// Create an unnamed semaphore starting with initial_value.
int sem_init(sem_t *sem, unsigned int initial_value)
{
        int ret;
        semaphoreInitSyscall(sem, initial_value, &ret);
        return ret;
}

int sem_destroy(sem_t *sem)

{
        int ret;
        semaphoreDestroySyscall(sem, &ret);
        return ret;
}

int sem_getvalue(sem_t *restrict sem, unsigned int *restrict sval)
{
        int ret;
        semaphoreGetValueSyscall(sem, sval, &ret);
        return ret;
}

int sem_post(sem_t *sem)
{
        int ret;
        semaphorePostSyscall(sem, &ret);
        return ret;
}

int sem_wait(sem_t *sem)
{
        int ret;
        semaphoreWaitSyscall(sem, &ret);
        return ret;
}

sem_info_t *sem_getinformation(sem_t *restrict sem)
{
        sem_info_t *sp;
        semaphoreGetInformationSyscall(sem, &sp);
        return sp;
}
