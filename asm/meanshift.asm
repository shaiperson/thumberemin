global IHT_meanShift_ASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

%define PIXELS_PER_ITER 4

section .data

align 16
; low --> high

four_dw_4s: dd 4, 4, 4, 4
four_dw_1s: dd 1, 1, 1, 1
initial_xs: dd 0, 1, 2, 3

section .text

; rdi->r12 <-- &densityMap
; esi->r13d <-- mapRows
; edx->r14d <-- mapcols
; ecx->r15d <-- mapstep
; r8->rbx  <-- &window
; r9       <-- iters (size_t)

; Quedarían libres entonces, en principio: r8, r9, r10, r11, rdi, rsi, rdx

IHT_meanShift_ASM:

    push r12
    push r13
    push r14
    push r15
    push rbx
    push rbp
    sub rsp, 24 ; alineado

    ; preserve parameters
    mov r12, rdi
    mov r13d, esi
    mov r14d, edx
    mov r15d, ecx
    mov rbx, r8

    mov [rsp], r9 ; preservo parámetro de r9, iters

    call GLOBAL_startTimer

    ; acomodo parámetros en r64s
    mov rcx, [rsp] ; vuelvo a traer de la pila iters

    ; move window info into registers
    ; VERIFICAR SI HACEN FALTA ESTOS XORs PARA QUE QUEDE LIMPIA LA PARTE ALTA
    mov r8d, dword [rbx + 0x0] ; r8d <-- w_x (curr_w_x)
    mov r9d, dword [rbx + 0x4] ; r9d <-- w_y (curr_w_y)
    mov r10d, dword [rbx + 0x8] ; r10d <-- width
    mov r11d, dword [rbx + 0xc] ; r11d <-- height

    ; store in stack width/2 and height/2
    mov dword [rsp + 8], r10d ; [rsp + 8] <-- width
    mov dword [rsp + 12], r11d ; [rsp + 12] <-- height
    sar dword [rsp + 8], 1 ; [rsp + 8] <-- width/2
    sar dword [rsp + 12], 1 ; [rsp + 12] <-- height/2

    ; store a dword 0 in stack for conditional movs for min and max
    mov dword [rsp + 16], 0

    ; calculate maprows-height and mapcols-width
    sub r13d, r11d ; r13d <-- maprows-height
    sub r14d, r10d ; r14d <-- mapcols-width

    ; máscaras y datos fijos
    cvtdq2ps xmm15, [four_dw_4s] ; for incrementing x's
    cvtdq2ps xmm14, [four_dw_1s] ; for incrementing y's
    cvtdq2ps xmm13, [initial_xs] ; for resetting x's inside loop (without accessing memory)ç
    pxor xmm12, xmm12 ; zeros for unpacking

    ; calculate row increment for data pointer ( mapstep - width*sizeof(short) )
    mov edi, r15d ; edi <-- mapstep
    mov edx, r10d ; edx <-- width (temporario)
    sal edx, 1 ; edx <-- width*sizeof(short)
    sub edi, edx ; edi <-- mapstep - width*sizeof(short)

    .iters_loop: ; uses rcx as counter

        mov eax, r15d ; eax <-- mapstep
        imul eax, r9d ; eax <-- mapstep*curr_w_y

        mov ebp, r8d ; ebp <-- curr_w_x
        sal ebp, 1 ; ebp <-- 2*curr_w_x

        add rax, rbp

        lea rax, [r12 + rax] ; rax <-- densityMap + curr_w_y*mapstep + curr_w_x*sizeof(short)

        ; reset m00, m10, m01 accumulators (note 00..0 is FP 0 as well)
        pxor xmm0, xmm0
        pxor xmm1, xmm1
        pxor xmm2, xmm2

        ; reset y's
        pxor xmm3, xmm3 ; xmm3 <-- 0 | 0 | 0 | 0

        ; clear y counter
        xor rdx, rdx

        .y_loop:
            ; reset x's
            movaps xmm4, xmm13 ; xmm4 <-- 3 | 2 | 1 | 0
            ; clear x counter
            xor rsi, rsi

            .x_loop:

                movdqu xmm5, [rax] ; xmm5 <-- xx | xx | xx | xx | d3 | d2 | d1 | d0
                punpcklwd xmm5, xmm12 ; xmm5 <-- d3 | d2 | d1 | d0

                cvtdq2ps xmm6, xmm5 ; xmm6 <-- float(d3) | float(d2) | float(d1) | float(d0)
                addps xmm0, xmm6 ; accumulate for m00

                movaps xmm7, xmm6 ; xmm7 <-- float(d3) | float(d2) | float(d1) | float(d0)
                mulps xmm7, xmm4 ; xmm7 <-- x3*d3 | x2*d2 | x1*d1 | x0*d0
                addps xmm1, xmm7 ; accumulate for m10

                mulps xmm6, xmm3 ; xmm6 <-- y3*d3 | y2*d2 | y1*d1 | y0*d0
                addps xmm2, xmm6 ; accumulate for m01

            addps xmm4, xmm15 ; increment x's
            add rax, 8 ; increment by sizeof(short)*PIXELS_PER_ITER
            add rsi, 4 ; increment x-counter by PIXELS_PER_ITER
            lea ebp, [r10d - PIXELS_PER_ITER] ; vectorization limit in row = width - PIXELS_PER_ITER
            cmp esi, ebp ; cmp x-counter against vectorization limit
            jle .x_loop ; iterate if lower

            ; sequential processing of las bit of row
            .end_of_row:
            cmp esi, r10d
            je .next_row

                movzx rbp, word [rax] ; rbp <-- value
                cvtsi2ss xmm5, ebp ; xmm5[31:0] <-- value

                cvtsi2ss xmm6, esi ; xmm6[31:0] <-- x counter
                cvtsi2ss xmm7, edx ; xmm6[31:0] <-- y counter

                addss xmm0, xmm5 ; accumulate for m00

                mulss xmm6, xmm5 ; xmm6[31:0] <-- x*value
                addss xmm1, xmm6 ; accumulate for m10

                mulss xmm7, xmm5 ; xmm7[31:0] <-- y*value
                addss xmm2, xmm7 ; accumulate for m01

            inc rsi
            jmp .end_of_row

        .next_row:
        addps xmm3, xmm14 ; increment y's
        add rax, rdi ; add row-window increment
        inc rdx ; increment y-counter
        cmp edx, r11d ; cmp y-counter, height
        jne .y_loop

        ; lógica de updatear curr_x y curr_y

        haddps xmm0, xmm0
        haddps xmm0, xmm0 ; xmm0 <-- m00 | m00 | m00 | m00

        divps xmm1, xmm0 ; xmm1 <-- casi_m10_3 / m00 | casi_m10_2 / m00 | casi_m10_1 / m00 | casi_m10_0 / m00
        divps xmm2, xmm0 ; xmm2 <-- casi_m01_3 / m00 | casi_m01_2 / m00 | casi_m01_1 / m00 | casi_m01_0 / m00

        ; por distributividad de la división
        haddps xmm1, xmm1 ; distributividad de la división
        haddps xmm1, xmm1 ; xmm1 <-- m10/m00 | m10/m00 | m10/m00 | m10/m00

        ; por distributividad de la división
        haddps xmm2, xmm2
        haddps xmm2, xmm2 ; xmm2 <-- m01/m00 | m01/m00 | m01/m00 | m01/m00

        ; (esto O3 también lo hace)
        ; VER LO DE MXCSR REGISTER para el redondeo de esto cosa - que sea igual a std::round y listo
        cvtss2si rbp, xmm1 ; rbp <-- int(m10/m00) = shifted_centroid_x
        cvtss2si rax, xmm2 ; rax <-- int(m01/m00) = shifted_centroid_y

        ; at this point
        ; rbp <-- shifted_centroid_x
        ; rax <-- shifted_centroid_y

        add ebp, r8d ; ebp <-- centroid_x
        add eax, r9d ; eax <-- centroid_y

        sub ebp, [rsp + 8] ; ebp <-- tl_x
        sub eax, [rsp + 12] ; eax <-- tl_y

        ; A PARTIR DE AHORA *SON SIGNED*

        cmp ebp, 0
        cmovl ebp, [rsp + 16] ; ebp <-- max(tl_x, 0)

        cmp ebp, r14d
        cmovg ebp, r14d ; ebp <-- min(max(tl_x, 0), mapcols-width)

        cmp eax, 0
        cmovl eax, [rsp + 16] ; eax <-- max(tl_y, 0)

        cmp eax, r13d
        cmovg eax, r13d ; eax <-- min(max(tl_y, 0), maprows-height)

        ; update curr_w_x and curr_w_y
        mov r8d, ebp ; curr_w_x <-- min(max(tl_x, 0), mapcols-width)
        mov r9d, eax ; curr_w_y <-- min(max(tl_y, 0), maprows-height)

        ; loop .iters_loop
    dec ecx
    jnz .iters_loop

    ; update final position of window
    mov [rbx + 0], r8d ; window->x <-- curr_w_x
    mov [rbx + 4], r9d ; window->y <-- curr_w_y

    call GLOBAL_stopTimer

    add rsp, 24
    pop rbp
    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12

    ret
