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

    ; r11 is now free

    ; zeros for interleaving when unpacking
    ; pxor xmm15, xmm15

    ; mask for pshufb for pixel4
    ; movdqa xmm9, [shuffle_pixel4]

    ; mask for pshufb for rearranging
    ; movdqa xmm8, [arrange_bytes]

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
        xor r10, r10 ; j = 0
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

            pslld xmm0, 1 ; xmm0 <-- b's * psize * 2
            pslld xmm1, 1 ; xmm1 <-- g's * dsize * 2
            pslld xmm2, 1 ; xmm2 <-- r's * 2

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

            ; use to compute histogram bin addresses and increment them
            xor r11, r11
            movd r11d, xmm0 ; read pixel0
            add r11, r13
            inc word [r11]

            psrldq xmm0, 4 ; kill pixel0

            xor r11, r11
            movd r11d, xmm0 ; read pixel1
            add r11, r13
            inc word [r11]

            psrldq xmm0, 4 ; kill pixel1

            xor r11, r11
            movd r11d, xmm0 ; read pixel2
            add r11, r13
            inc word [r11]

            psrldq xmm0, 4 ; kill pixel2

            xor r11, r11
            movd r11d, xmm0 ; read pixel3
            add r11, r13
            inc word [r11]

            xor r11, r11
            movd r11d, xmm3 ; read pixel4
            sal r11, 1
            add r11, r13
            inc word [r11]

        add r12, 15 ; 15 = CHANNELS * PIXELS_PER_ITER * sizeof(uchar)
        add r10, 5 ; 5 = PIXELS_PER_ITER
        cmp r10, r15 ; cmp j, imgcols
        jne .cols_loop

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
