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
#include "lib.h"
#include "semaphore.h"

/* ------------------------------ */

sosem_t *sosem_open(const char *name, uint64_t initial_value)
{
        return NULL;
}

int sosem_close(sosem_t *sem)
{
        return -1;
}

int sosem_post(sosem_t *sem)
{
        return -1;
}

int sosem_wait(sosem_t *sem)
{
        return -1;
}
