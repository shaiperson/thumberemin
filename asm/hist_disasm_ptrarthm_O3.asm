global IHT_calc3DByteDepthUniformHist_DISASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

;00000000004670c0 <_Z30IHT_calc3DByteDepthUniformHistPKhPhmmm>:

section .text

; RDI imgdata --> r14 --> rdi
; RSI histdata --> rbx
; RDX imgrows --> r12
; RCX imgcols --> rbp
; R8 imgstep --> r8

IHT_calc3DByteDepthUniformHist_DISASM:
        push   r14
        push   r13
        mov    r14,rdi ; r14 <-- imgdata
        push   r12
        push   rbp
        mov    r12,rdx ; r12 <-- imgrows
        push   rbx
        mov    rbp,rcx ; rbp <-- imgcols
        mov    rbx,rsi ; rbx <-- histdata
        mov    r13,r8 ; r13 <-- imgstep
        call   GLOBAL_startTimer
        test   r12,r12 ; testea si imgrows es 0
        je     L4 ; jump if ZF=1 <--> r12 AND r12 = 0x0 <--> r12 = 0x0
        lea    rdx,[rbp+rbp*2+0x0] ; rdx <-- 3*rbp = 3*imgcols
        mov    rdi,r14 ; rdi <-- imgdata
        xor    r11d,r11d ; r11d <-- 0, básicamente es i (contador de rows)
        nop    DWORD [rax+0x0]

        L1:
        test   rbp,rbp ; testea si imgcols es 0
        lea    rsi,[rdi+rdx*1] ; rsi <-- rdi+rdx = imgdata + 3*imgcols
        mov    rax,rdi ; rax <-- imgdata
        je     L3
        nop    DWORD [rax+0x0]

        L2:
        movzx  r9d,BYTE [rax] ; r9 <-- b
        add    rax,0x3 ; avanzar un pixel
        mov    r10,r9 ; r10 <-- b
        movzx  r9d,BYTE [rax-0x2] ; r9 <-- g
        shl    r10,0x10 ; multiplicar b por 65536
        shl    r9,0x8 ; multiplicar g por 256
        add    r9,r10 ; sumar parte del offset en r9
        movzx  r10d,BYTE [rax-0x1] ; r10 <-- r
        add    r9,r10 ; terminar de sumar offset en r9
        add    WORD [rbx+r9*2],0x1 ; inc en histdata + r9*sizeof(short)
        cmp    rsi,rax ; mientras no haya terminado la fila, seguir iterando
        jne    L2

        L3:
        add    r11,0x1 ; inc r11
        add    rdi,r13 ; sumar un imgstep a imgdata
        cmp    r12,r11 ; comparar imgrows con i
        jne    L1

        L4: ; (terminar)
        pop    rbx
        pop    rbp
        pop    r12
        pop    r13
        pop    r14
        jmp    GLOBAL_stopTimer
        nop    WORD [rax+rax*1+0x0]
