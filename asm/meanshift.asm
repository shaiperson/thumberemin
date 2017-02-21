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

    ; preserve parameters
    mov r12, rdi
    mov r13d, esi
    mov r14d, edx
    mov r15d, ecx
    mov rbx, r8

    sub rsp, 16 ; alineado
    mov [rsp], r9 ; preservo parámetro de r9, iters

    call GLOBAL_startTimer

    ; acomodo parámetros en r64s
    mov rcx, [rsp] ; vuelvo a traer de la pila iters

    ; máscaras y datos fijos
    movdqa xmm15, [four_dw_4s] ; for incrementing x's
    movdqa xmm14, [four_dw_1s] ; for incrementing y's
    movdqa xmm13, [initial_xs] ; for resetting x's inside loop (without accessing memory)
    pxor xmm12, xmm12 ; zeros for unpacking

    ; move window info into registers
    ; VERIFICAR SI HACEN FALTA ESTOS XORs PARA QUE QUEDE LIMPIA LA PARTE ALTA
    mov r8d, dword [rbx + 0x0] ; r8d <-- w_x (curr_w_x)
    mov r9d, dword [rbx + 0x4] ; r9d <-- w_y (curr_w_y)
    mov r10d, dword [rbx + 0x8] ; r10d <-- width
    mov r11d, dword [rbx + 0xc] ; r11d <-- height

    ; calculate address of first window element (densityMap + curr_w_y*mapstep + curr_w_x*sizeof(short))
    xor rdx, rdx ; limpio para poder usarlo en lea más que nada
    mov edx, r15d ; edx <-- mapstep (temporario)
    imul edx, r9d ; edx <-- mapstep*curr_w_y (temporario)

    mov esi, r8d ; esi <-- w_x (temporario)
    sal esi, 1 ; esi <-- w_x*2

    add edx, esi ; edx (rdx) <-- mapstep*curr_w_y + w_x*sizeof(short)

    lea r12, [r12 + rdx] ; r12 <-- dirección del primer elemento del window

    ; guardo en la pila el comienzo de los datos #aFaltaDeRegistros
    mov [rsp], r12

    ; calculate row increment for data pointer ( mapstep - width*sizeof(short) )
    xor rdi, rdi ; limpio para poder usar como incremento de puntero
    mov edi, r15d ; edi <-- mapstep
    mov edx, r10d ; edx <-- width (temporario)
    sal edx, 1 ; edx <-- width*sizeof(short)
    sub edi, edx ; edi <-- mapstep - width*sizeof(short)

    .iters_loop: ; uses rcx as counter

        ; reseteo puntero al comienzo de la ventana
        mov r12, [rsp]

        ; reset m00, m10, m01 accumulators (note 00..0 is FP 0 as well)
        pxor xmm0, xmm0
        pxor xmm1, xmm1
        pxor xmm2, xmm2

        ; reset y's
        pxor xmm3, xmm4 ; xmm3 <-- 0 | 0 | 0 | 0
        ; clear y counter
        xor rdx, rdx

        .y_loop:
            ; reset x's
            movdqu xmm4, xmm13 ; xmm4 <-- 3 | 2 | 1 | 0
            ; clear x counter
            xor rsi, rsi

            .x_loop:

                movdqu xmm5, [r12] ; xmm5 <-- xx | xx | xx | xx | d3 | d2 | d1 | d0
                punpcklwd xmm5, xmm12 ; xmm5 <-- d3 | d2 | d1 | d0

                cvtdq2ps xmm6, xmm5 ; xmm6 <-- float(xmm5)
                addps xmm0, xmm6 ; accumulate m00

                movdqu xmm6, xmm7 ; xmm6 <-- d3 | d2 | d1 | d0
                pmuludq xmm6, xmm4 ; xmm6 <-- x3*d3 | x2*d2 | x1*d1 | x0*d0
                cvtdq2ps xmm6, xmm6 ; xmm6 <-- float(xmm6)
                addps xmm1, xmm6 ; accumulate m10

                pmuludq xmm5, xmm3 ; xmm5 <-- y3*d3 | y2*d2 | y1*d1 | y0*d0
                cvtdq2ps xmm5, xmm5 ; xmm5 <-- float(xmm5)
                addps xmm2, xmm5 ; accumulate m01

            paddd xmm4, xmm15 ; increment x's
            add r12, 8 ; increment by sizeof(short)*PIXELS_PER_ITER
            add rsi, 4 ; increment x-counter by PIXELS_PER_ITER
            cmp esi, r10d ; cmp x-counter, width
            jne .x_loop

        paddd xmm4, xmm14 ; increment y's
        add r12, rdi ; add row-window increment
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
    ; cvtss2si UN_R64, xmm1 ; UN_R64 <-- int(m10/m00) = shifted_centroid_x
    ; cvtps2dq UN_R64, xmm2 ; UN_R64 <-- int(m01/m00) = shifted_centroid_y

    ; at this point
    ; xmm1 <-- shifted_centroid_x | shifted_centroid_x | shifted_centroid_x | shifted_centroid_x
    ; xmm2 <-- shifted_centroid_y | shifted_centroid_y | shifted_centroid_y | shifted_centroid_y

    ; loop .iters_loop
    dec ecx
    jnz .iters_loop

    ; lógica de updatear window

    call GLOBAL_stopTimer

    add rsp, 16
    pop rbx
    pop r15
    pop r14
    pop r13
    pop r12

    ret
