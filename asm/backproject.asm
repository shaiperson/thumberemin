global IHT_calc3DByteDepthBackProject_ASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

section .data

align 16
; low --> high

; ----------- PIXELS 0, 1, 2, 3
leave_4_bs_AND_MASK: db 0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0, 0, 0, 0, 0, 0
leave_4_gs_AND_MASK: db 0, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0, 0, 0, 0, 0
leave_4_rs_AND_MASK: db 0, 0, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0, 0, 0, 0

shuffle_4_bs_shifted: db 2, 1, 0, 5, 4, 14, 3, 7, 8, 10, 6, 11, 12, 13, 9, 15
shuffle_4_gs_shifted: db 0, 1, 2, 3, 5, 4, 6, 9, 8, 7, 13, 11, 12, 10, 14, 15
shuffle_4_rs_shifted: db 2, 1, 0, 3, 5, 0, 6, 7, 8, 9, 10, 12, 11, 13, 14, 15

; ----------- PIXEL 4
leave_only_pixel_4: db 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0
shuffle_pixel4_shifted: db 0, 1, 12, 3, 4, 13, 6, 7, 14, 9, 10, 11, 2, 5, 8, 15

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

    movdqa xmm15, [leave_4_bs_AND_MASK]
    movdqa xmm14, [leave_4_gs_AND_MASK]
    movdqa xmm13, [leave_4_rs_AND_MASK]

    movdqa xmm12, [shuffle_4_bs_shifted]
    movdqa xmm11, [shuffle_4_gs_shifted]
    movdqa xmm10, [shuffle_4_rs_shifted]

    movdqa xmm9, [leave_only_pixel_4]
    movdqa xmm8, [shuffle_pixel4_shifted]

    ; i = 0
    xor r9, r9

    .rows_loop:
        xor r10, r10
        .cols_loop:

            ; xmm0 = x|r4|g4|b4|r3|g3|b3|r2|g2|b2|r1|g1|b1|r0|g0|b0
            movdqu xmm0, [r12]

            ; >>>>>>> get pixel0,1,2,3 offsets in xmm0 >>>>>>>

            ; WARNING -----------------------------------------------------------
            ; using signed packed dword mul. This is OK assuming highest possible
            ; multiplication value here is PLANESIZE*BYTESIZE = 255*255*255
            ; which is << 2^31-1. This of course assumes image as 8-bit depth.
            ; -------------------------------------------------------------------

            ; parecerían estar libres xmm4-7

            movdqa xmm1, xmm0
            movdqa xmm2, xmm0
            movdqa xmm3, xmm0

            pand xmm0, xmm15 ; xmm0 <-- b's and 0s
            pand xmm1, xmm14 ; xmm1 <-- g's and 0s
            pand xmm2, xmm13 ; xmm2 <-- r's and 0s

            pshufb xmm0, xmm12 ; xmm0 <-- b's * psize
            pshufb xmm1, xmm11 ; xmm1 <-- g's * dsize
            pshufb xmm2, xmm10 ; xmm2 <-- r's

            ; b1+g1+r1 | b2+g2+r2 | b3+g3+r3 | b0+g0+r0 <-- xmm0
            paddd xmm0, xmm1
            paddd xmm0, xmm2

            ; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

            ; >>>>>>> get pixel4 offset in xmm3 low qword >>>>>>>

            pand xmm3, xmm9
            pshufb xmm3, xmm8 ; xmm3 <-- 0 | r4 | g4*size | b4*psize <-- SIN MULTIPLICAR POR 2 (SIZEOF(SHORT))
            phaddd xmm3, xmm3
            phaddd xmm3, xmm3

            ; <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

            xor r11, r11

            movd r11d, xmm0 ; r11 <-- pixel0 offset
            movzx rdi, word [r13 + 2*r11] ; rdi <-- hist_bin[pixel0]
            psrldq xmm0, 4

            movd r11d, xmm0 ; r11 <-- pixel1 offset
            movzx rsi, word [r13 + 2*r11] ; rsi <-- hist_bin[pixel1]
            psrldq xmm0, 4

            movd r11d, xmm0 ; r11 <-- pixel2 offset
            movzx rdx, word [r13 + 2*r11] ; rdx <-- hist_bin[pixel2]
            psrldq xmm0, 4

            movd r11d, xmm0 ; r11 <-- pixel3 offset
            movzx rcx, word [r13 + 2*r11] ; rcx <-- hist_bin[pixel3]

            movd r11d, xmm3 ; r11 <-- pixel4 offset
            movzx r8, word [r13 + 2*r11] ; r8 <-- hist_bin[pixel4]

            ; shifts para alinear los shorts del hist en orden correcto
            sal rsi, 16 ; pixel1 va en posición 1
            sal rdx, 32 ; pixel2 va en posición 2
            sal rcx, 48 ; pixel3 va en posición 3

            ; or para juntarlos en rdi
            or rdi, rsi
            or rdi, rdx
            or rdi, rcx

            ; primero mov de pixel4
            movd xmm0, r8d
            pslldq xmm0, 8 ; muevo a posición 4, qword bajo queda zeroed)

            ; movq de pixels0,1,2,3 a qword bajo de xmm0, ZEROEA QWORD ALTO POR GATO
            movq xmm7, rdi
            por xmm0, xmm7

            ; escribo en res
            movdqu [r14], xmm0

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
