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
 * Test provistos por la cátedra.
 */
#ifndef TESTS_H
#define TESTS_H

#include <tests/test_util.h>
#include <stdio.h>
#include <syscalls_asm.h>

int test_mm(int argc, char *argv[]);
int test_processes(int argc, char *argv[]);
int test_prio(int argc, char *argv[]);
int test_sync(int argc, char *argv[]);
int test_no_sync(int argc, char *argv[]);

#endif
