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
arrange_bytes: db 0,3,6,9,1,4,7,10,2,5,8,11,12,13,14,15

coordclc_psz_psz_psz_psz: dd PLANESIZE, PLANESIZE, PLANESIZE, PLANESIZE
coordclc_dsz_dsz_dsz_dsz: dd DIMSIZE, DIMSIZE, DIMSIZE, DIMSIZE
coordclc_0_1_dsz_psz:     dd PLANESIZE, DIMSIZE, 1, 0

section .text

; RDI-->R12 imgdata
; RSI-->R13 histdata
; RDX-->R14 imgrows
; RCX-->R15 imgcols
; R8-->RBX imgstep ---> padding = imgstep - imgcols*3

; R9 i
; R10 j

; ---> FREE REGS at least as of beginning of loop <---
; rax
; xmm11

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
    mov r11, r15
    sal r11, 1
    add r11, r15 ; r11 now contains imgcols * 3

    sub rbx, r11 ; rbx now contains padding

    ; zero r11 to use in loop
    xor r11, r11

    ; zeros for interleaving when unpacking
    pxor xmm15, xmm15

    ; vectors for bin-index calculation
    movdqa xmm14, [coordclc_psz_psz_psz_psz]
    movdqa xmm13, [coordclc_dsz_dsz_dsz_dsz]
    movdqa xmm12, [coordclc_0_1_dsz_psz]

    ; mask for pshufb
    movdqa xmm8, [arrange_bytes]

    ; move to xmm10 2 copies of histdata (r13)
    movq xmm10, r13 ; move histdata to lower 64 bits
    pslldq xmm10, 8 ; shift left 64 bits
    movq xmm11, r13 ; copy histdata again. xmm0 is now histdata | histdata
    por xmm10, xmm11

    ; i = 0
    xor r9, r9

    .rows_loop:
        xor r10, r10 ; j = 0
        .cols_loop:
            ; xmm0 = x|r4|g4|b4|r3|g3|b3|r2|g2|b2|r1|g1|b1|r0|g0|b0
            movdqu xmm0, [r12]

            ; xmm0 = x|r4|g4|b4|r3|r2|r1|r0|g3|g2|g1|g0|b3|b2|b1|b0
            pshufb xmm0, xmm8

            ; unpack byte --> word
            movdqa xmm2, xmm0
            punpcklbw xmm0, xmm15
            punpckhbw xmm2, xmm15

            ; multiply by 2
            psllw xmm0, 1
            psllw xmm2, 1

            ; unpack word --> dword
            movdqa xmm1, xmm0
            punpcklwd xmm0, xmm15
            punpckhwd xmm1, xmm15

            movdqa xmm3, xmm2
            punpcklwd xmm2, xmm15
            punpckhwd xmm3, xmm15

            ; at this point:
            ; b3 | b2 | b1 | b0 <-- xmm0
            ; g3 | g2 | g1 | g0 <-- xmm1
            ; r3 | r2 | r1 | r0 <-- xmm2
            ; xx | r4 | g4 | b4 <-- xmm3

            ; calculate coor12nates

            ; WARNING -----------------------------------------------------------
            ; using signed packed dword mul. This is OK assuming highest possible
            ; multiplication value here is PLANESIZE*BYTESIZE = 255*255*255
            ; which is << 2^31-1. This of course assumes image as 8-bit depth.
            ; -------------------------------------------------------------------

            pmulld xmm0, xmm14 ; multiply b's
            pmulld xmm1, xmm13 ; multiply g's
            ; xmm2 r's, they're multiplied by 1
            pmulld xmm3, xmm12 ; multiply pixel4

            ; b1+g1+r1 | b2+g2+r2 | b3+g3+r3 | b0+g0+r0 <-- xmm0
            paddd xmm0, xmm1
            paddd xmm0, xmm2

            ; last but not least, xmm3 (b4 g4 r4)
            ; shift left to kill unused highest dword without changing value of
            ; horizontal sum (shift instruction fills in 0s)
            pslldq xmm3, 4
            phaddd xmm3, xmm3
            phaddd xmm3, xmm3 ; xx | xx | xx | b4+g4+r4

            ; ------------------------------------------------------------------
            ; use to compute histogram bin addresses and increment them
            ; ------------------------------------------------------------------

            ; unpack to quadword for pointer arithmetic
            movdqa xmm9, xmm0
            punpckldq xmm0, xmm15
            punpckhdq xmm9, xmm15

            ; xmm0 and xmm9 now contain bin offsets for pixel0,1,2,3.

            ; add histdata to each
            paddq xmm0, xmm10
            paddq xmm9, xmm10

            ; xmm0 and xmm9 now contain bin addresses for pixel0,1,2,3.

            ; move to r64 to perform memory write
            movq rax, xmm0 ; pixel in xmm0
            inc word [rax]
            psrldq xmm0, 8
            movq rax, xmm0 ; the other pixel in xmm0
            inc word [rax]
            movq rax, xmm9 ; pixel in xmm9
            inc word [rax]
            psrldq xmm9, 8
            movq rax, xmm9 ; the other pixel in xmm9
            inc word [rax]

            movd r11d, xmm3 ; read pixel4
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
