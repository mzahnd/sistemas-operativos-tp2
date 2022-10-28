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

void writeStr(registerStruct *registers);
void getDateInfo(uint8_t mode, uint8_t *target);
void syscallCreateProcess(registerStruct *reg) ;

void syscallHandler(registerStruct *registers)
{
        uint64_t option = registers->rax;
        switch (option) {
        //READ KEYBOARD
        case 0:
                //rdi -> puntero a buffer
                //rsi -> uint8_t size
                //rdx -> putero a uint64_t count
                readKeyboard((char *)registers->rdi, (uint8_t)registers->rsi,
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
        case 20: //Create Process
                //rdi -> char *: Nombre del proceso
                //rsi -> int (*)(int, char**): Puntero a la funcion principal del procesp
                //rdx -> int: argc
                //rcx -> char**: argv
                syscallCreateProcess(registers);

        case 30:
                // rdi -> id
                // rsi -> initValue
                // rdx -> *toReturn
                sosem_open((uint32_t) registers->rdi, (uint32_t) registers->rsi, (int*) registers->rdx);
                return 1;
                // break;

        case 31:
                // rdi -> id
                // rsi -> *toReturn
                sosem_wait((uint32_t) registers->rdi, (int*) registers->rsi);
                return 1;
                // break;

        case 32:
                // rdi -> id
                // rsi -> *toReturn
                sosem_post((uint32_t) registers->rdi, (int*) registers->rsi);
                return 1;
                // break;

        case 33:
                // rdi -> id
                // rsi -> *toReturn
                sosem_close((uint32_t) registers->rdi, (int*) registers->rsi);
                return 1;
                // break;

        // case 34:
        //         // rdi -> buffer
        //         printSemaphore((char *) registers->rdi);
        //         return 1;
        //         // break;
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

void syscallCreateProcess(registerStruct *reg) {
        createAndAddProcess((char *)reg->rdi,(int (*)(int, char**))reg->rsi, (int)reg->rdx, (char **)reg->rcx);
}

#endif
