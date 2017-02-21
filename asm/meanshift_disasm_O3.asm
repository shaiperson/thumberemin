global IHT_meanShift_DISASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer
extern IHT_round

section .text

; rdi    <-- densityMap
; esi->ebx    <-- mapRows
; edx->ebp    <-- mapcols
; ecx    <-- mapstep
; r8->r15     <-- &w_x
; r9->r14     <-- &w_y
; pila_1 <-- width
; pila_2 <-- height
; pila_3 <-- iters

IHT_meanShift_DISASM:
        push   r15
        push   r14
        mov    r15,r8 ; r15 <-- &w_x
        push   r13
        push   r12
        mov    r14,r9 ; r14 <-- &w_y
        push   rbp
        push   rbx
        mov    ebp,edx ; ebp <-- mapcols
        mov    ebx,esi ; ebx <-- maprows
        sub    rsp,0x58
        mov    eax,DWORD [rsp+0xa0]
        mov    QWORD [rsp+0x40],r8
        mov    r8d,DWORD [rsp+0x98]
        mov    QWORD [rsp+0x28],rdi
        mov    DWORD [rsp+0x20],ecx
        mov    QWORD [rsp+0x48],r9
        mov    r12d,eax
        mov    r13d,DWORD [rsp+0x90]
        mov    DWORD [rsp+0x34],eax
        mov    DWORD [rsp+0x8],r8d
        call   GLOBAL_startTimer
        test   r12d,r12d
        mov    r15d,DWORD [r15]
        mov    r14d,DWORD [r14]
        jle    L1
        mov    eax,r13d
        mov    r8d,DWORD [rsp+0x8]
        movsxd r9,DWORD [rsp+0x20]
        shr    eax,0x1f
        sub    ebp,r13d
        mov    r12d,r14d
        add    eax,r13d
        mov    DWORD [rsp+0x24],ebp
        sar    eax,1
        sub    ebx,r8d
        neg    eax
        pxor   xmm4,xmm4
        mov    DWORD [rsp+0x38],eax
        mov    eax,r8d
        mov    DWORD [rsp+0x30],ebx
        shr    eax,0x1f
        xor    ebx,ebx
        add    eax,r8d
        sar    eax,1
        neg    eax
        mov    DWORD [rsp+0x3c],eax
        movsxd rax,r13d
        add    rax,rax
        sub    r9,rax
        lea    eax,[r13-0x1]
        lea    rbp,[rax+rax*1+0x2]
        mov    eax,r8d
        mov    r14d,eax
        mov    r8,rbp
        mov    ebp,ebx
        mov    ebx,r15d
        mov    r15,r9

L6:


        mov    eax,DWORD [rsp+0x20]
        movsxd rdx,ebx
        imul   eax,r12d
        cdqe
        lea    rsi,[rax+rdx*2]
        add    rsi,QWORD [rsp+0x28]
        test   r14d,r14d
        jle    L2
        movaps xmm2,xmm4
        movaps xmm0,xmm4
        movaps xmm3,xmm4
        xor    edi,edi

L5:

        test   r13d,r13d

        jle    L3
        xor    edx,edx

L4:

        movsx  eax,WORD [rsi+rdx*2]

        pxor   xmm1,xmm1
        cvtsi2ss xmm1,eax
        mov    ecx,eax
        imul   ecx,edx
        add    rdx,0x1
        imul   eax,edi
        cmp    r13d,edx
        addss  xmm3,xmm1
        pxor   xmm1,xmm1
        cvtsi2ss xmm1,ecx
        addss  xmm0,xmm1
        pxor   xmm1,xmm1
        cvtsi2ss xmm1,eax
        addss  xmm2,xmm1
        jg     L4
        add    rsi,r8

L3:

        add    edi,0x1

        add    rsi,r15
        cmp    r14d,edi
        jne    L5

L7:

        divss  xmm0,xmm3

        mov    QWORD [rsp+0x18],r8
        movss  DWORD [rsp+0x14],xmm4
        movss  DWORD [rsp+0x10],xmm2
        movss  DWORD [rsp+0xc],xmm3
        call   IHT_round
        movss  xmm3,DWORD [rsp+0xc]
        movss  xmm2,DWORD [rsp+0x10]
        divss  xmm2,xmm3
        movss  DWORD [rsp+0x8],xmm0
        movaps xmm0,xmm2
        call   IHT_round
        movss  xmm1,DWORD [rsp+0x8]
        mov    r8,QWORD [rsp+0x18]
        cvttss2si eax,xmm1
        movss  xmm4,DWORD [rsp+0x14]
        lea    r10d,[rax+rbx*1]
        add    r10d,DWORD [rsp+0x38]
        mov    eax,0x0
        cmovs  r10d,eax
        mov    eax,DWORD [rsp+0x24]
        mov    ebx,r10d
        cmp    r10d,eax
        cmovg  ebx,eax
        cvttss2si eax,xmm0
        lea    r9d,[rax+r12*1]
        add    r9d,DWORD [rsp+0x3c]
        mov    eax,0x0
        cmovs  r9d,eax
        mov    eax,DWORD [rsp+0x30]
        cmp    r9d,eax
        cmovg  r9d,eax
        add    ebp,0x1
        cmp    DWORD [rsp+0x34],ebp
        mov    r12d,r9d
        jne    L6
        mov    r14d,r9d
        mov    r15d,ebx

L1:

        mov    rax,QWORD [rsp+0x40]

        mov    DWORD [rax],r15d
        mov    rax,QWORD [rsp+0x48]
        mov    DWORD [rax],r14d
        add    rsp,0x58
        pop    rbx
        pop    rbp
        pop    r12
        pop    r13
        pop    r14
        pop    r15
        jmp    GLOBAL_stopTimer

L2:

        movaps xmm2,xmm4

        movaps xmm0,xmm4
        movaps xmm3,xmm4
        jmp    L7
