global IHT_calc3DByteDepthBackProject_ASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

section .text

IHT_calc3DByteDepthBackProject_ASM:
    push r12 ; aligned
    push r13 ; unaligned
    push r14 ; aligned
    push r15 ; unaligned
    push rbx ; aligned

    mov r12, rdi
    mov r13, rsi
    mov r14, rdx
    mov r15, rcx
    mov rbx, r8

    call GLOBAL_startTimer

    call GLOBAL_stopTimer

    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12

    ret
