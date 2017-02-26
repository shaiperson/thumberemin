global IHT_calc3DByteDepthUniformHist_ASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

; general constants
%define CHANNELS 3
%define DIMSIZE 256
%define PLANESIZE 65536
%define SIZEOFUCHAR

; misc
%define PIXELS_PER_ITER 5

section .text

; RDI-->R12 imgdata
; RSI-->R13 histdata
; RDX-->R14 imgrows
; RCX-->R15 imgcols
; R8-->RBX imgstep ---> padding = imgstep - imgcols*3

; R9 i
; R10 j

IHT_calc3DByteDepthUniformHist_ASM:

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

    ; HARDCODING imgcols * 3 <---- MODIFY if switching to HS(V)
    ; multiply imgcols by 3, number of channels
    ; shift left once and add once
    lea r11, [r15+r15*2] ; r11 now contains imgcols * 3

    sub rbx, r11 ; rbx now contains padding

    ; i = 0
    xor r9, r9

    .rows_loop:
        xor r10, r10 ; j = 0
        .cols_loop:
            ; xmm0 = x|r4|g4|b4|r3|g3|b3|r2|g2|b2|r1|g1|b1|r0|g0|b0
            movdqu xmm0, [r12]

            movq r11, xmm0 ; r11 <-- g2 b2 r1 g1 b1 r0 g0 b0
            and r11, 0x0000000000FFFFFF ; r11 <-- 0 0 0 0 0 r0 g0 b0
            inc word [r13 + 2*r11] ; probar también con shift, sumar el puntero y ahí escribir

            psrldq xmm0, 3

            movq r11, xmm0
            and r11, 0x0000000000FFFFFF
            inc word [r13 + 2*r11]

            psrldq xmm0, 3

            movq r11, xmm0
            and r11, 0x0000000000FFFFFF
            inc word [r13 + 2*r11]

            psrldq xmm0, 3

            movq r11, xmm0
            and r11, 0x0000000000FFFFFF
            inc word [r13 + 2*r11]

            psrldq xmm0, 3

            movq r11, xmm0
            and r11, 0x0000000000FFFFFF
            inc word [r13 + 2*r11]

        add r12, 15 ; 15 = CHANNELS * PIXELS_PER_ITER * sizeof(uchar)
        add r10, PIXELS_PER_ITER ; 5 = PIXELS_PER_ITER
        lea rdx, [r15 - PIXELS_PER_ITER] ; rdx <-- vecotorization limit
        cmp r10, rdx ; cmp j, vectorization limit
        jle .cols_loop

        .end_of_row:
        cmp r10, r15
        je .next_row

            mov edx, dword [r12] ; rdx <-- x | x | x | x | x | r | g | b
            and rdx, 0x0000000000FFFFFF ; rdx <-- 0 | 0 | 0 | 0 | 0 | r | g | b
            inc word [r13 + 2*rdx] ; bin += 1

        inc r10
        add r12, CHANNELS
        jmp .end_of_row

    .next_row:
    add r12, rbx ; i += padding
    add r9, 1 ; only add ONE row
    cmp r9, r14 ; cmp i, imgrows
    jne .rows_loop

    call GLOBAL_stopTimer

    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12

    ret
