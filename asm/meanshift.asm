global IHT_meanShift_ASM

extern GLOBAL_startTimer
extern GLOBAL_stopTimer

section .data

section .text

IHT_meanShift_ASM:

    call GLOBAL_startTimer

    call GLOBAL_stopTimer
    
    ret
