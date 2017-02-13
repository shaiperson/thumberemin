global IHT_calc3DByteDepthUniformHist_ASM

; general constants
%define CHANNELS 3
%define DIMSIZE 256
%define PLANESIZE 65536
%define SIZEOFUCHAR

; masks
%define ZERO_DWS_1_2 0xFFFFFFFF0000000000000000FFFFFFFF
%define ZERO_DWS_0_1_3 0x00000000FFFFFFFF0000000000000000
%define ZERO_DWS_0_2_3 0x0000000000000000FFFFFFFF00000000

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

; RDI imgdata
; RSI histdata
; RDX imgrows
; RCX imgcols
; R8 imgstep ---> padding = imgstep - imgcols*3

; R14 i
; R15 j

IHT_calc3DByteDepthUniformHist_ASM:

    ; HARDCODING imgcols * 3 <---- MODIFY if switching to HS(V)
    ; multiply imgcols by 3, number of channels
    ; shift left once and add once
    mov r13, rcx
    sal r13, 1
    add r13, rcx ; r13 now contains imgcols * 3

    sub r8, r13 ; r8 now contains padding

    ; i = 0
    xor r14, r14

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

    ; regigster for shifting once
    ; YAYZ xmm8 is now free


    .rows_loop:
        xor r15, r15 ; j = 0
        .cols_loop:
            inc r15; j += 1

            ; xmm0 = x|r4|g4|b4|r3|g3|b3|r2|g2|b2|r1|g1|b1|r0|g0|b0|
            movdqu xmm0, [rdi]

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

            ; calculate coordinates

            mov rax, rax; >>>>> REMOVE DUMMY >>>>>

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
            por xmm0, xmm2 ; r3 | r0 | r1 | r2 <-- xmm0

            ; at this point, xmm4, xmm5 and xmm0 have 'b's, 'g's and 'r's
            ; free registers: xmm6, xmm7

            ; shuffle xmm0 and xmm5 so they're ordered same as xmm4 (this is arbitrary)
            ; pshufd xmm0, 1 0 3 2
            ; pshufd xmm5, 0 3 2 1

            mov rax, rax; >>>>> REMOVE DUMMY >>>>>

            ; b1+g1+r1 | b2+g2+r2 | b3+g3+r3 | b0+g0+r0 <-- xmm0
            paddd xmm0, xmm4
            paddd xmm0, xmm5

            ; shift left to kill unused highest dword without changing value of
            ; horizontal sum (shift instruction fills in 0s)
            pslldq xmm3, 4 ; SCORE this lets you use an immediate
            phaddd xmm3, xmm3 ; xx | xx | xx | b4+g4+r4
            phaddd xmm3, xmm3 ; xx | xx | xx | b4+g4+r4

            ; move to general purpose registers to use as increments for hist pointer
            ; ...


    ret
