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
GLOBAL initSemaphoreSyscall
GLOBAL destroySemaphoreSyscall
GLOBAL getSemaphoreInformationSyscall
GLOBAL getSchedulerInfoSyscall
GLOBAL pipeOpenSyscall
GLOBAL pipeCloseSyscall
GLOBAL pipeReadSyscall
GLOBAL pipeWriteSyscall
GLOBAL pipeInfoSyscall

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

    mov rax, 30 ;ID sys_sosem_open
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

waitSemaphoreSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 31 ;ID sys_sosem_wait
    int 80h

    mov rsp, rbp
    pop rbp
    ret

postSemaphoreSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 32 ;ID sys_sosem_post
    int 80h

    mov rsp, rbp
    pop rbp
    ret

closeSemaphoreSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 33 ;ID sys_sosem_close
    int 80h

    mov rsp, rbp
    pop rbp
    ret

semSyscall: 
    push rbp
    mov rbp, rsp

    mov rax, 34 ;ID sys_sosem_getvalue
    int 80h

    mov rsp, rbp
    pop rbp
    ret

initSemaphoreSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 35 ;ID sys_sosem_init
    int 80h

    mov rsp, rbp
    pop rbp
    ret

destroySemaphoreSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 36 ;ID sys_sosem_destroy
    int 80h

    mov rsp, rbp
    pop rbp
    ret

getSemaphoreInformationSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 37 ;ID sys_sosem_getinformation
    int 80h

    mov rsp, rbp
    pop rbp
    ret

pipeOpenSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 40; ID pipeOpen
    int 80h

    mov rsp, rbp
    pop rbp
    ret

pipeCloseSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 41 ;ID Pipe Close
    int 80h

    mov rsp, rbp
    pop rbp
    ret

pipeReadSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 42 ;ID PipeRead
    int 80h

    mov rsp, rbp
    pop rbp
    ret

pipeWriteSyscall:
    push rbp
    mov rbp, rsp

    mov rax, 43 ;ID PipeWrite 
    int 80h

    mov rsp, rbp
    pop rbp
    ret

pipeInfoSyscall:    ;getValuePipes
    push rbp
    mov rbp, rsp

    mov rax, 44 ; ID pipe Info
    int 80h

    mov rsp, rbp
    pop rbp
    ret
