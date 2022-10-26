// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * This file is part of sistemas-operativos-tp2
 * Licensed under BSD 3-Clause "New" or "Revised" License.
 * Copyright (c) 2022 Flores Levalle, M.
 *                    López, P.
 *                    Sierra Pérez, C.
 *                    Zahnd, M. E.
 *
 *
 * This implementation is based on Michael Cotterell (@mepcotterell) Gist's
 * on semaphores.
 * See: https://gist.github.com/mepcotterell/6f0a779befe388ab822764255e3776ae
 *
 * In case you do not have the manual entry for stdatomic.h is available online
 * https://en.cppreference.com/w/c/atomic
 */
#include "lib.h"

#include "semaphore.h"

/* ------------------------------ */

#define acquire(lock) while (atomic_flag_test_and_set(lock))
#define release(lock) atomic_flag_clear(lock)

/* ------------------------------ */

int sosem_init(sosem_t *sem, unsigned int initial_value)
{
        if (sem == NULL)
                return -1;

        atomic_store(&sem->value, initial_value);
        atomic_flag_clear(&sem->lock);
        atomic_store(&sem->_n_waiting, 0);

        return 0;
}

/* ------------------------------ */

int sosem_destroy(sosem_t *sem)
{
        if (sem == NULL)
                return -1;

        // Free all waiting processes (if any)
        atomic_store(&sem->value, MAX_VALUE);
        return 0;
}

/* ------------------------------ */

int sosem_post(sosem_t *sem)
{
        if (sem == NULL)
                return -1;

        return atomic_fetch_add(&sem->value, 1);
}

/* ------------------------------ */

int sosem_wait(sosem_t *sem)
{
        if (sem == NULL)
                return -1;

        // We are guaranteed that atomic_* functions are atomic, but we do not
        // have any guarantees about them being executed atomically as a block
        acquire(&(sem->lock));
        atomic_fetch_add(&sem->_n_waiting, 1);

        while (atomic_load(&sem->value) == 0)
                ;
        atomic_fetch_sub(&sem->value, 1);

        atomic_fetch_sub(&sem->_n_waiting, 1);
        release(&(sem->lock));

        return 0;
}

/* ------------------------------ */

int sosem_getvalue(sosem_t *restrict sem, unsigned int *restrict sval)
{
        if (sem == NULL || sval == NULL)
                return -1;

        if (atomic_load(&sem->_n_waiting) > 0)
                *sval = 0;
        else
                *sval = atomic_load(&sem->value);

        return 0;
}
