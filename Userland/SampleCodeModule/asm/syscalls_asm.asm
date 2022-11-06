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
GLOBAL memGetInformationSyscall
GLOBAL createProcessSyscall
GLOBAL pipeCloseSyscall
GLOBAL pipeGetInformationSyscall
GLOBAL pipePipeSyscall
GLOBAL pipeReadSyscall
GLOBAL pipeWriteSyscall
GLOBAL semaphoreCloseSyscall
GLOBAL semaphoreDestroySyscall
GLOBAL semaphoreGetInformationSyscall
GLOBAL semaphoreGetValueSyscall
GLOBAL semaphoreInitSyscall
GLOBAL semaphoreOpenSyscall
GLOBAL semaphorePostSyscall
GLOBAL semaphoreWaitSyscall
GLOBAL getSchedulerInfoSyscall
GLOBAL waitPIDSyscall

GLOBAL psSyscall
GLOBAL niceSyscall
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

    mov rax, 16 ;ID sys_somalloc
    int 80h

    mov rsp, rbp
    pop rbp
    ret

callocSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 17 ;ID sys_socalloc
    int 80h

    mov rsp, rbp
    pop rbp
    ret

freeSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 18 ;ID sys_sofree
    int 80h

    mov rsp, rbp
    pop rbp
    ret

memGetInformationSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 19 ;ID sys_somem_getinformation
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

pipeCloseSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 41 ;ID sys_soclose
    int 80h

    mov rsp, rbp
    pop rbp
    ret

pipeGetInformationSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 44 ;ID sys_sopipe_getinformation
    int 80h

    mov rsp, rbp
    pop rbp
    ret

pipePipeSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 40 ;ID sys_sopipe
    int 80h

    mov rsp, rbp
    pop rbp
    ret

pipeReadSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 42 ;ID sys_soread
    int 80h

    mov rsp, rbp
    pop rbp
    ret

pipeWriteSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 43 ;ID sys_sowrite
    int 80h

    mov rsp, rbp
    pop rbp
    ret

semaphoreCloseSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 33 ;ID sys_sosem_close
    int 80h

    mov rsp, rbp
    pop rbp
    ret

semaphoreDestroySyscall:
    push rbp
    mov rbp, rsp

    mov rax, 36 ;ID sys_sosem_destroy
    int 80h

    mov rsp, rbp
    pop rbp
    ret

semaphoreGetInformationSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 37 ;ID sys_sosem_getinformation
    int 80h

    mov rsp, rbp
    pop rbp
    ret

semaphoreGetValueSyscall: 
    push rbp
    mov rbp, rsp

    mov rax, 34 ;ID sys_sosem_getvalue
    int 80h

    mov rsp, rbp
    pop rbp
    ret

semaphoreInitSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 35 ;ID sys_sosem_init
    int 80h

    mov rsp, rbp
    pop rbp
    ret

semaphoreOpenSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 30 ;ID sys_sosem_open
    int 80h

    mov rsp, rbp
    pop rbp
    ret

semaphorePostSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 32 ;ID sys_sosem_post
    int 80h

    mov rsp, rbp
    pop rbp
    ret

semaphoreWaitSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 31 ;ID sys_sosem_wait
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getSchedulerInfoSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 21 ;ID getSchedulerInfo
    int 80h

    mov rsp, rbp
    pop rbp
    ret

waitPIDSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 22; ID waitForPID
    int 80h

    mov rsp, rbp
    pop rbp
    ret

psSyscall:  ;printProcessList
    push rbp
    mov rbp, rsp

    mov rax, 17
    int 80h

    mov rsp, rbp
    pop rbp
    ret

niceSyscall:    ;changeProcessPriority
    push rbp
    mov rbp, rsp

    mov rax, 18
    int 80h

    mov rsp, rbp
    pop rbp
    ret