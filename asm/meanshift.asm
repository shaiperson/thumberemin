global IHT_meanShift_ASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

section .data

section .text

; rdi->r12 <-- densityMap
; esi->r13 <-- mapRows
; edx->r14 <-- mapcols
; ecx->r15 <-- mapstep
; r8->rbx  <-- &window
; r9       <-- iters

; Quedarían conteniendo parámetros, en principio: r12, r13, r14, r15, rbx, r9(desp de volver a traerlo de la pila)
; Quedarían libres entonces, en principio: r8, r10, r11, rdi, rsi, rdx, rcx

IHT_meanShift_ASM:

    push r12
    push r13
    push r14
    push r15
    push rbx

    ; preserve parameters
    mov r12, rdi
    mov r13, rsi
    mov r14, rdx
    mov r15, rcx
    mov rbx, r8

    sub rsp, 8
    mov [rsp], r9 ; preservo parámetro de r9, iters

    call GLOBAL_startTimer

    mov r9, [rsp] ; vuelvo a traer de la pila iters

    call GLOBAL_stopTimer

    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12

    ret
