global IHT_calc3DByteDepthBackProject_ASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

section .data

align 16
; low --> high
leave_3_low_bytes_r64: db 0xFF, 0xFF, 0xFF, 0, 0, 0, 0, 0

section .text

; rdi-->r12 imgdata
; rsi-->r13 histdata
; rdx-->r14 resdata
; rcx-->r15 imgrows
; r8-->rax imgcols
; r9-->rbx imgstep

; r9 i
; r10 j

IHT_calc3DByteDepthBackProject_ASM:

    push r12 ; aligned
    push r13 ; unaligned
    push r14 ; aligned
    push r15 ; unaligned
    push rbx ; aligned

    mov r12, rdi ; r12 <-- imgdata
    mov r13, rsi ; r13 <-- histdata
    mov r14, rdx ; r14 <-- resdata
    mov r15, rcx ; r15 <-- imgrows
    mov rbx, r9 ; rbx <-- imgstep

    call GLOBAL_startTimer

    mov rax, r8 ; rax <-- imgcols
    lea r11, [rax+rax*2] ; r11 now contains imgcols * 3
    sub rbx, r11 ; rbx now contains padding

    mov rdi, [leave_3_low_bytes_r64]

    ; i = 0
    xor r9, r9

    .rows_loop:
        xor r10, r10
        .cols_loop:
            ; xmm0 = x|r4|g4|b4|r3|g3|b3|r2|g2|b2|r1|g1|b1|r0|g0|b0
            movdqu xmm0, [r12]

            movq r11, xmm0 ; r11 <-- g2 b2 r1 g1 b1 r0 g0 b0
            and r11, rdi ; r11 <-- 0 0 0 0 0 r0 g0 b0
            movdqu xmm1, [r13 + 2*r11] ; leave in position 0

            psrldq xmm0, 3

            movq r11, xmm0
            and r11, rdi ; r11 <-- 0 0 0 0 0 r1 g1 b1
            movdqu xmm2, [r13 + 2*r11]
            pslldq xmm2, 2 ; place in position 1

            psrldq xmm0, 3

            movq r11, xmm0
            and r11, rdi ; r11 <-- 0 0 0 0 0 r2 g2 b2
            movdqu xmm3, [r13 + 2*r11]
            pslldq xmm3, 4 ; place in position 2

            psrldq xmm0, 3

            movq r11, xmm0
            and r11, rdi ; r11 <-- 0 0 0 0 0 r3 g3 b3
            movdqu xmm4, [r13 + 2*r11]
            pslldq xmm4, 6 ; place in position 3

            psrldq xmm0, 3

            movq r11, xmm0
            and r11, rdi ; r11 <-- 0 0 0 0 0 r4 g4 b4
            movdqu xmm5, [r13 + 2*r11]
            pslldq xmm5, 8 ; place in position 4

            por xmm5, xmm1
            por xmm5, xmm2
            por xmm5, xmm3
            por xmm5, xmm4

            ; escribo en res
            movdqu [r14], xmm5

        add r12, 15 ; advance imgdata, 15 = CHANNELS * PIXELS_PER_ITER
        add r14, 10 ; advance resdata POR AHÍ MEJOR índices y usar lea en el loop?
        add r10, 5 ; 5 = PIXELS_PER_ITER
        cmp r10, rax ; cmp j, imgcols
        jne .cols_loop

    add r12, rbx ; imgdata += padding | RESDATA SE SUPONE CONTINUO
    add r9, 1 ; i += 1
    cmp r9, r15
    jne .rows_loop

    call GLOBAL_stopTimer

    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12

    ret
