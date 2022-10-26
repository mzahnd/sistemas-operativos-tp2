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

#include "pipes.h"

/* ------------------------------ */

/* ------------------------------ */

int sopipe(int fildes[PIPE_N_FD])
{
        return -1;
}

ssize_t soread(int fd, void *buf, size_t count)
{
        return 0;
}

ssize_t sowrite(int fd, const void *buf, size_t count)
{
        return 0;
}

int soclose(int fd)
{
        return 0;
}
