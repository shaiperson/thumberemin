global IHT_calc3DByteDepthUniformHist_ASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

; general constants
%define CHANNELS 3
%define DIMSIZE 256
%define PLANESIZE 65536
%define SIZEOFUCHAR

; masks
%define ZERO_DWS_1_2 0xFFFFFFFF0000000000000000FFFFFFFF
%define ZERO_DWS_0_1_3 0x00000000FFFFFFFF0000000000000000
%define ZERO_DWS_0_2_3 0x0000000000000000FFFFFFFF00000000

; misc
%define PIXELS_PER_ITER 5

section .data

align 16

coordclc_psz_1_dsz_psz:   dd PLANESIZE, DIMSIZE, 1, PLANESIZE
coordclc_dsz_psz_1_dsz:   dd DIMSIZE, 1, PLANESIZE, DIMSIZE
coordclc_1_dsz_psz_1:     dd 1, PLANESIZE, DIMSIZE, 1

zero_dws_1_2:   dd 0xFFFFFFFF, 0, 0, 0xFFFFFFFF
zero_dws_0_1_3: dd 0, 0, 0xFFFFFFFF, 0
zero_dws_0_2_3: dd 0, 0xFFFFFFFF, 0 , 0

one: dq 1

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
    mov r11, r15
    sal r11, 1
    add r11, r15 ; r11 now contains imgcols * 3

    sub rbx, r11 ; rbx now contains padding

    ; r11 is now free

    ; zeros for interleaving when unpacking
    pxor xmm15, xmm15

    ; vectors for bin-index calculation
    movdqa xmm14, [coordclc_psz_1_dsz_psz]
    movdqa xmm13, [coordclc_dsz_psz_1_dsz]
    movdqa xmm12, [coordclc_1_dsz_psz_1]

    ; masks for rearranging
    movdqa xmm11, [zero_dws_1_2]
    movdqa xmm10, [zero_dws_0_1_3]
    movdqa xmm9, [zero_dws_0_2_3]

    ; i = 0
    xor r9, r9

    .rows_loop:
        xor r10, r10 ; j = 0
        .cols_loop:
            ; xmm0 = x|r4|g4|b4|r3|g3|b3|r2|g2|b2|r1|g1|b1|r0|g0|b0|
            movdqu xmm0, [r12]

            ; unpack byte --> word
            movdqa xmm2, xmm0
            punpcklbw xmm0, xmm15
            punpckhbw xmm2, xmm15

            ; unpack word --> dword
            movdqa xmm1, xmm0
            punpcklwd xmm0, xmm15
            punpckhwd xmm1, xmm15

            movdqa xmm3, xmm2
            punpcklwd xmm2, xmm15
            punpckhwd xmm3, xmm15

            ; at this point:
            ; b1 | r0 | g0 | b0 <-- xmm0
            ; g2 | b2 | r1 | g1 <-- xmm1
            ; r3 | g3 | b3 | r2 <-- xmm2
            ; xx | r4 | g4 | b4 <-- xmm3

            ; calculate coor12nates

            ; WARNING -----------------------------------------------------------
            ; using signed packed dword mul. This is OK assuming highest possible
            ; multiplication value here is PLANESIZE*BYTESIZE = 255*255*255
            ; which is << 2^31-1. This of course assumes image as 8-bit depth.
            ; -------------------------------------------------------------------

            pmulld xmm0, xmm14
            pslld xmm0, 1 ; multiply all by 2

            pmulld xmm1, xmm13
            pslld xmm1, 1 ; multiply all by 2

            pmulld xmm2, xmm12
            pslld xmm2, 1 ; multiply all by 2

            pmulld xmm3, xmm14
            pslld xmm3, 1 ; multiply all by 2

            ; rearrange 'b's for vertical addition
            movdqa xmm4, xmm0 ; copy register
            movdqa xmm5, xmm1 ; copy register
            movdqa xmm6, xmm2 ; copy register

            pand xmm4, xmm11 ; b1 | 0 | 0 | b0
            pand xmm5, xmm10 ; 0 | b2 | 0 | 0
            pand xmm6, xmm9 ; 0 | 0 | b3 | 0

            por xmm4, xmm5
            por xmm4, xmm6 ; b1 | b2 | b3 | b0 <-- xmm4

            ; rearrange 'g's for vertical addition
            movdqa xmm5, xmm0 ; copy register
            movdqa xmm6, xmm1 ; copy register
            movdqa xmm7, xmm2 ; copy register

            pand xmm5, xmm9 ; 0 | 0 | g0 | 0
            pand xmm6, xmm11 ; g2 | 0 | 0 | g1
            pand xmm7, xmm10 ; 0 | g3 | 0 | 0

            por xmm5, xmm6
            por xmm5, xmm7 ; g2 | g3 | g0 | g1 <-- xmm5

            ; rearrange 'r's for vertical addition
            ;;; movdqa xmm4, xmm0 ; copy register
            ;;; movdqa xmm5, xmm1 ; copy register
            ;;; movdqa xmm6, xmm2 ; copy register

            ; --> no need to copy registers in last rearrangement
            ; use directly xmm0, xmm1, xmm2
            pand xmm0, xmm10 ; 0 | r0 | 0 | 0
            pand xmm1, xmm9 ; 0 | 0 | r1 | 0
            pand xmm2, xmm11 ; r3 | 0 | 0 | r2

            por xmm0, xmm1
            por xmm0, xmm2 ; r3 | r0 | r1 | r2 <-- xmm0mv

            ; at this point, xmm4, xmm5 and xmm0 have 'b's, 'g's and 'r's
            ; free registers: xmm6, xmm7

            ; shuffle xmm0 and xmm5 so they're ordered same as xmm4 (this is arbitrary)
            ; mask for xmm0: 1 0 3 2
            ; mask for xmm5, 0 3 2 1
            pshufd xmm0, xmm0, 01001110b
            pshufd xmm5, xmm5, 00111001b

            ; b1+g1+r1 | b2+g2+r2 | b3+g3+r3 | b0+g0+r0 <-- xmm0
            paddd xmm0, xmm4
            paddd xmm0, xmm5

            ; last but not least, xmm3 (b4 g4 r4)
            ; shift left to kill unused highest dword without changing value of
            ; horizontal sum (shift instruction fills in 0s)
            pslldq xmm3, 4
            phaddd xmm3, xmm3
            phaddd xmm3, xmm3 ; xx | xx | xx | b4+g4+r4

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
            movd r11d, xmm3 ; read pixel3
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
