global IHT_calc3DByteDepthBackProject_ASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

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
            ; xmm0 = x|r4|g4|b4|r3|g3|b3|r2 | g2|b2|r1|g1|b1|r0|g0|b0
            movdqu xmm0, [r12]

            movq rdi, xmm0
            mov rsi, rdi
            mov rdx, rdi

            and rdi, 0x0000000000FFFFFF ; rdi <-- 0 0 0 0 0 r0 g0 b0

            shr rsi, 24 ; move pixel1 to lowest 3 bytes
            and rsi, 0x0000000000FFFFFF ; rsi <-- 0 0 0 0 0 r1 g1 b1

            shr rdx, 48 ; rdx <-- 0 0 0 0 0 0 g2 b2 <--> missing r2

            psrldq xmm0, 8

            movq rcx, xmm0 ; copy higher pixels

            mov r8, rcx ; recover r2 to place in rdx
            sal r8, 16 ; position r2 for or with rdx
            or rdx, r8 ; bring r2 along with a bunch of other crap
            and rdx, 0x0000000000FFFFFF ; rdx <-- 0 0 0 0 0 r2 g2 b2

            mov r8, rcx ; copy higher pixels again

            shr rcx, 8 ; move pixel3 to lowest 3 bytes
            and rcx, 0x0000000000FFFFFF ; rcx <-- 0 0 0 0 0 0 r4 g4 b4

            shr r8, 32 ; move pixel4 to lowest 3 bytes
            and r8, 0x0000000000FFFFFF

            ; now write (ordered lowest->highest): rdi, rsi, rdx, rcx, r8

            mov r11w, [r13 + 2*rdi]
            mov [r14 + 0], r11w

            mov r11w, [r13 + 2*rsi]
            mov [r14 + 2], r11w

            mov r11w, [r13 + 2*rdx]
            mov [r14 + 4], r11w

            mov r11w, [r13 + 2*rcx]
            mov [r14 + 6], r11w

            mov r11w, [r13 + 2*r8]
            mov [r14 + 8], r11w

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
