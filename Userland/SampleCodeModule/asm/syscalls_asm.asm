GLOBAL drawStringSysCall
GLOBAL readKeyboardSysCall
GLOBAL isKeyboardEmptySyscall
GLOBAL drawRectSyscall
GLOBAL clearDisplaySyscall
GLOBAL setFunctionKeyMethodSyscall
GLOBAL getTimeSyscall
GLOBAL getTicksSyscall
GLOBAL getRegistersSyscall
GLOBAL getMemSyscall
GLOBAL readErrorSyscall
GLOBAL setTimerFunctionSyscall
GLOBAL deleteTimerFunctionSyscall
GLOBAL mallocSyscall
GLOBAL callocSyscall
GLOBAL freeSyscall
GLOBAL createProcessSyscall
GLOBAL initSemaphoresSyscall
GLOBAL openSemaphoreSyscall
GLOBAL waitSemaphoreSyscall
GLOBAL postSemaphoreSyscall
GLOBAL closeSemaphoreSyscall
GLOBAL semSyscall

section .text



drawStringSysCall:
    push rbp
    mov rbp, rsp

    mov rax, 1 ;ID write str
    mov r10, [rbp+2*8] ;sexto argumento
    mov r11, [rbp+3*8] ;septimo argumento

    int 80h

    mov rsp, rbp
    pop rbp
    ret

readKeyboardSysCall:
    push rbp
    mov rbp, rsp

    mov rax, 0    ;ID read
    int 80h

    mov rsp, rbp
    pop rbp
    ret

isKeyboardEmptySyscall:
    push rbp
    mov rbp, rsp

    mov rax, 11    ;ID bufferEmpty
    int 80h

    mov rsp, rbp
    pop rbp
    ret

clearDisplaySyscall:
    push rbp
    mov rbp, rsp

    mov rax, 2    ;ID clear Display
    int 80h

    mov rsp, rbp
    pop rbp
    ret

drawRectSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 5 ; drawRectangle syscall
    int 80h

    mov rsp, rbp
    pop rbp
    ret

setFunctionKeyMethodSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 13; setFunctionKeyMethod syscall
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getTimeSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 8
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getTicksSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 7
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getRegistersSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 9
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getMemSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 10
    int 80h

    mov rsp, rbp
    pop rbp
    ret

readErrorSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 12 ;ID readError
    int 80h

    mov rsp, rbp
    pop rbp
    ret

setTimerFunctionSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 14 ;ID readError
    int 80h

    mov rsp, rbp
    pop rbp
    ret

deleteTimerFunctionSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 15 ;ID readError
    int 80h

    mov rsp, rbp
    pop rbp
    ret

mallocSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 16 ;sys_somalloc
    int 80h

    mov rsp, rbp
    pop rbp
    ret

callocSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 17 ;sys_socalloc
    int 80h

    mov rsp, rbp
    pop rbp
    ret

freeSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 18 ;sys_sofree
    int 80h

    mov rsp, rbp
    pop rbp
    ret




createProcessSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 20 ;ID createProcess
    int 80h

    mov rsp, rbp
    pop rbp
    ret

openSemaphoreSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 30
    int 80h

    mov rsp, rbp
    pop rbp
    ret

waitSemaphoreSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 31
    int 80h

    mov rsp, rbp
    pop rbp
    ret

postSemaphoreSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 32
    int 80h

    mov rsp, rbp
    pop rbp
    ret

closeSemaphoreSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 33
    int 80h

    mov rsp, rbp
    pop rbp
    ret

semSyscall: ;getValueSemaphore
    push rbp
    mov rbp, rsp

    mov rax, 34
    int 80h

    mov rsp, rbp
    pop rbp
    ret