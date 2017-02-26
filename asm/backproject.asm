global IHT_calc3DByteDepthBackProject_ASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

%define PIXELS_PER_ITER 5
%define CHANNELS 3

section .data

align 16

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

    ; i = 0
    xor r9, r9

    .rows_loop:
        xor r10, r10
        .cols_loop:

            ; xmm0 = x|r4|g4|b4|r3|g3|b3|r2|g2|b2|r1|g1|b1|r0|g0|b0
            movdqu xmm0, [r12]

            movd r11d, xmm0 ; r11 <-- g2 b2 r1 g1 b1 r0 g0 b0
            and r11, 0x0000000000FFFFFF ; r11 <-- 0 0 0 0 0 r0 g0 b0
            movzx rdi, word [r13 + 2*r11] ; leave in position 0

            psrldq xmm0, 3 ; crush p0

            movd r11d, xmm0
            and r11, 0x0000000000FFFFFF ; r11 <-- 0 0 0 0 0 r1 g1 b1
            movzx rsi, word [r13 + 2*r11]
            sal rsi, 16 ; place in position 1

            psrldq xmm0, 3 ; crush p1

            movd r11d, xmm0
            and r11, 0x0000000000FFFFFF ; r11 <-- 0 0 0 0 0 r2 g2 b2
            movzx rdx, word [r13 + 2*r11]
            sal rdx, 32 ; place in position 2

            psrldq xmm0, 3 ; crush p2

            movd r11d, xmm0
            and r11, 0x0000000000FFFFFF ; r11 <-- 0 0 0 0 0 r3 g3 b3
            movzx rcx, word [r13 + 2*r11]
            sal rcx, 48 ; place in position 3

            ; junt pixel0,1,2,3 bin values
            or rcx, rdi
            or rcx, rsi
            or rcx, rdx ; rcx <-- p3 p2 p1 p0
            movq xmm1, rcx ; xmm1 <-- 0 | 0 | 0 | 0 | p3 | p2 | p1 | p0

            psrldq xmm0, 3 ; crush p3

            movd r11d, xmm0 ;
            and r11, 0x0000000000FFFFFF ; r11 <-- 0 0 0 0 0 r4 g4 b4
            movzx rdi, word [r13 + 2*r11] ; rdi ya está libre acá porque se hicieron los or para acomodar rcx
            movd xmm2, edi
            pslldq xmm2, 8 ; place in position 4

            por xmm1, xmm2

            ; escribo en res
            movdqu [r14], xmm1

        add r12, 15 ; advance imgdata, 15 = CHANNELS * sizeof(uchar) * PIXELS_PER_ITER
        add r14, 10 ; advance resdata POR AHÍ MEJOR índices y usar lea en el loop?
        add r10, PIXELS_PER_ITER ; 5 = PIXELS_PER_ITER
        lea rdi, [rax - PIXELS_PER_ITER] ; cmp j, vectoriation limit
        cmp r10, rdi
        jle .cols_loop ; iterate of lower or equal

        .end_of_row:
        cmp r10, rax
        je .next_row

            mov edi, dword [r12] ; rdi <-- x | x | x | x | x | r | g | b
            and rdi, 0x0000000000FFFFFF ; rdi <-- 0 | 0 | 0 | 0 | 0 | r | g | b
            mov di, word [r13 + 2*rdi] ; rdi <-- 0 | 0 | 0 | 0 | 0 | 0 | val val
            mov [r14], di

        inc r10
        add r12, CHANNELS ; increment imgdata by CHANNELS * sizeof(uchar)
        add r14, 2 ; increment resdata by SIZEOF(short)
        jmp .end_of_row

    .next_row:
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
