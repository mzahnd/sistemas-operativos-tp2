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
#ifndef SYSCALLS
#define SYSCALLS

#include <stdint.h>
#include <regi.h>
#include <keyboard_driver.h>
#include <video_driver.h>
#include <timer_driver.h>
#include <date_driver.h>
#include <font.h>
#include <IO_driver.h>
#include <exceptions.h>
#include <scheduler/scheduler.h>
#include <scheduler/process.h>
#include <mem/memory.h>
#include <utils.h>
#include <mem/sys_memory.h> /* sys_somalloc(); sys_socalloc(); sys_sofree() */
#include <sys_semaphore.h> /* sys_sosem_*() */
#include <sys_pipes.h> /* sys_sopipe(); sys_soread(); sys_sowrite(); 
                        * sys_soclose(); sys_sopipe_getinformation() */

void writeStr(registerStruct *registers);
void getDateInfo(uint8_t mode, uint8_t *target);
void syscallCreateProcess(registerStruct *reg);
void syscallRead(registerStruct *reg);

void syscallHandler(registerStruct *registers)
{
        uint64_t option = registers->rax;
        switch (option) {
        //READ KEYBOARD
        case 0:
                //rdi -> puntero a buffer
                //rsi -> uint8_t size
                //rdx -> putero a uint64_t count
                readKeyboard((int *)registers->rdi, (uint8_t)registers->rsi,
                             (uint64_t *)registers->rdx);
                break;

        //WRITE STR
        case 1:
                //rdi -> puntero a buffer
                //rsi -> size
                //rdx -> x
                //rcx -> y
                //r8 -> fontColor
                //r9 -> backgroundColor
                //r10 -> fontSize
                //r11 -> alphaBackground
                writeStr(registers);
                break;

        case 2:
                //rdi color
                clearDisplay((uint64_t)registers->rdi);
                break;
        case 3:
                // rdi xstart , rsi ystart, rdx xend, rcx yend, r8 color
                drawLine((uint64_t)registers->rdi, (uint64_t)registers->rsi,
                         (uint64_t)registers->rdx, (uint64_t)registers->rcx,
                         (uint64_t)registers->r8);
                break;
        case 4:
                // r9 xi, r8 yi, rsi color
                drawPixel((uint64_t)registers->rdi, (uint64_t)registers->rsi,
                          (uint64_t)registers->rdx);
                break;
        case 5:
                // rdi xi, rsi yi , rdx width, rc8 height , r8 color
                drawRectangle((uint64_t)registers->rdi,
                              (uint64_t)registers->rsi,
                              (uint64_t)registers->rdx,
                              (uint64_t)registers->rcx,
                              (uint64_t)registers->r8);
                break;
        case 6:
                // rdi xi, rsi yi, rdx puntero a matriz, rcx width,
                // r8 height, r9 size
                drawMatrix((uint64_t)registers->rdi, (uint64_t)registers->rsi,
                           (uint64_t *)registers->rdx, (uint64_t)registers->rcx,
                           (uint64_t)registers->r8, (uint64_t)registers->r9);
                break;
        case 7:
                *((uint64_t *)registers->rdi) = getTicks();
                break;
        case 8:
                //rdi -> mode
                //rsi -> puntero a entero
                getDateInfo((uint8_t)registers->rdi, (uint8_t *)registers->rsi);
                break;

        case 9: //Obtener los registros
                //rdi -> puntero a vector de uint64_t para guardar los valores
                getRegisters((uint64_t *)registers->rdi);
                break;

        case 10:
                //rdi -> direccion de la que se desea leer
                //rsi -> direccion del buffer para guardar
                //rdx -> totalBytes
                getBytesFromAddress(registers->rdi, (uint64_t *)registers->rsi,
                                    (uint8_t)registers->rdx);
                break;

        case 11:
                //rdi -> puntero a int para devolver si hay algo
                bufferEmpty((uint64_t *)registers->rdi);
                break;

        case 12: //readError
                //rdi -> puntero a int para que devuelva el Error
                readError((uint64_t *)registers->rdi);
                break;

        case 13:
                //rdi -> indice de la tecla de funcion (de 1 (F1) a 10 (F10))
                //rsi -> puntero a la funcion tipo void foo()
                setFunctionKeyMethod(registers->rdi,
                                     (void (*)())registers->rsi);
                break;

        case 14:
                //rdi -> indice en la tabla de metodos del timer tick
                //rsi -> cada cuantos ticks se tiene que ejecutar la funcion
                //rdx -> puntero a la funcion
                setTickMethod(registers->rdi, registers->rsi,
                              (void (*)())registers->rdx);
                break;

        case 15:
                //rdi -> indice del metodo a eliminar
                deleteTickMethod(registers->rdi);
                break;

        case 16: /* somalloc */
                // rdi -> tamaño de memoria solicitado
                // rsi -> result
                sys_somalloc((size_t)registers->rdi, (void **)registers->rsi);
                break;

        case 17: /* socalloc */
                // rdi -> cantidad de bloques solicitados
                // rsi -> tamaño de cada bloque
                // rdx -> result
                sys_socalloc((size_t)registers->rdi, (size_t)registers->rsi,
                             (void **)registers->rdx);
                break;

        case 18: /* sofree */
                // rdi -> puntero al bloque a liberar
                sys_sofree((void *)registers->rdi);
                break;

        // From 20 -> Process management syscalls:
        case 20: // Create Process
                //rdi -> char *: Nombre del proceso
                //rsi -> int (*)(int, char**): Puntero a la funcion principal del procesp
                //rdx -> int: argc
                //rcx -> char**: argv
                //r8 -> unsigned int: foreground Flag
                //r9 -> uiint64_t: stdin
                //r10 -> uint64_t: stdout
                //r11 -> unsigned int*: address to return PID
                syscallCreateProcess(registers);
                break;

        case 21: // Get Scheduler Info
                // rdi -> schInfo_t*: pointer to struct
                getSchedulerInfo((schInfo_t *)registers->rdi);
                break;

        case 22: // waitPID
                //rdi -> unsigned int: pid of process to wait
                waitForPID((uint64_t)registers->rdi);
                break;

        case 30:
                // rdi -> const char *: name
                // rsi -> unsigned int: initial value
                // rdx -> sosem_t **: semaphore pointer
                sys_sosem_open((const char *)registers->rdi,
                               (unsigned int)registers->rsi,
                               (sosem_t **)registers->rdx);
                break;

        case 31:
                // rdi -> semaphore pointer (sosem_t *)
                // rsi -> result (int *)
                sys_sosem_wait((sosem_t *)registers->rdi,
                               (int *)registers->rsi);
                break;

        case 32:
                // rdi -> semaphore pointer (sosem_t *)
                // rsi -> result (int *)
                sys_sosem_post((sosem_t *)registers->rdi,
                               (int *)registers->rsi);
                break;

        case 33:
                // rdi -> semaphore pointer (sosem_t *)
                // rsi -> result (int *)
                sys_sosem_close((sosem_t *)registers->rdi,
                                (int *)registers->rsi);
                break;

        case 34:
                // rdi -> semaphore pointer (sosem_t *)
                // rsi -> semaphore's value storage (int *)
                // rdx -> function result (int *)
                sys_sosem_getvalue((sosem_t *)registers->rdi,
                                   (unsigned int *)registers->rsi,
                                   (int *)registers->rdx);
                break;

        case 35:
                // rdi -> semaphore pointer (sosem_t *)
                // rsi -> initial value
                // rdx -> result (int*)
                sys_sosem_init((sosem_t *)registers->rdi,
                               (unsigned int)registers->rsi,
                               (int *)registers->rdx);
                break;

        case 36:
                // rdi -> semaphore pointer (sosem_t *)
                // rsi -> result (int*)
                sys_sosem_destroy((sosem_t *)registers->rdi,
                                  (int *)registers->rsi);
                break;

        case 37:
                // rdi -> semaphore pointer (sosem_t *)
                // rsi -> result (sosem_info_t **)
                sys_sosem_getinformation((sosem_t *)registers->rdi,
                                         (sosem_info_t **)registers->rsi);
                break;

        // Pipes from 40
        case 40:
                // rdi -> int [PIPE_N_FD]: fd
                // rsi -> int *: result
                sys_sopipe((int *)registers->rdi, (int *)registers->rsi);
                break;

        case 41:
                // rdi -> int: fd
                // rsi -> int *: result
                sys_soclose((int)registers->rdi, (int *)registers->rsi);
                break;

        case 42:
                //rdi -> int: fd
                //rsi -> char *: buf
                //rdx -> size_t: count
                //rcx -> ssize_t *: result
                syscallRead(registers);

                break;

        case 43:
                //rdi -> int: fd
                //rsi -> const char *: buf
                //rdx -> size_t: count
                //rcx -> ssize_t *: result
                sys_sowrite((int)registers->rdi, (const char *)registers->rsi,
                            (size_t)registers->rdx, (ssize_t *)registers->rcx);
                break;

        case 44:
                //rdi -> int: fd
                //rsi -> pipe_info_t **: result
                sys_sopipe_getinformation((int)registers->rdi,
                                          (pipe_info_t **)registers->rsi);
                break;
        }
}

void getDateInfo(uint8_t mode, uint8_t *target)
{
        switch (mode) {
        case 0:
                *target = getSeconds();
                break;
        case 1:
                *target = getMinutes();
                break;
        case 2:
                *target = getHours();
                break;
        case 3:
                *target = getDay();
                break;
        case 4:
                *target = getMonth();
                break;
        case 5:
                *target = getYear();
                break;
        }
}

void writeStr(registerStruct *registers)
{
        uint64_t xOffset = 0;
        char *buffer = (char *)registers->rdi;
        for (uint64_t i = 0; i < registers->rsi && buffer[i] != 0; i++) {
                char ch = ((char *)registers->rdi)[i];
                drawChar(registers->rdx + xOffset, registers->rcx, ch,
                         registers->r10, registers->r8, registers->r9,
                         registers->r11);
                xOffset += CHAR_WIDTH * registers->r10;
        }
        //drawChar(0, 0, 'A',1, 0xFFFFFF, 0, 0);
}

void syscallCreateProcess(registerStruct *reg)
{
        uint64_t *addressToReturn = (uint64_t *)reg->r11;
        uint64_t result = createAndAddProcess((char *)reg->rdi,
                                              (int (*)(int, char **))reg->rsi,
                                              (int)reg->rdx, (char **)reg->rcx,
                                              reg->r8, reg->r9, reg->r10);
        *addressToReturn = result;
}

void syscallRead(registerStruct *reg) {
        int fd = (int)reg->rdi;
        if (fd == 0) { //STDIN
                int currentProcessStdin = getCurrentStdin();
                if (currentProcessStdin == -1) {
                        return; // Not initialized
                }
                if (currentProcessStdin == 0) {
                        char* buff =  (int *)reg->rsi;
                        uint64_t size = (uint64_t)reg->rdx; 
                        uint64_t* resultPtr = (uint64_t *)reg->rcx;
                        readKeyboard(buff, size, resultPtr);
                        return;
                }
                // if the stdin pipe of the process is not 0, it has a pipe where it wants to read from
                fd = currentProcessStdin;
        }
        sys_soread((int)reg->rdi, (char *)reg->rsi,
                (size_t)reg->rdx, (ssize_t *)reg->rcx);
}

#endif /* SYSCALLS */
