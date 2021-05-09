.data

.text
.globl main
main:
    ADDI R2,R2,3,
    ADDI R3,R3,5,
    ADD R1,R2,R3,
    SUB R4,R2,R3,
    ADD R5,R1,R4,
loop:
    BEQ     R2,R3,lab,
    SUBI    R10,R5,15,
    ADD     R11,R10,R5,
    ADDI    R2,R2,1,
    ADDI    R31,R31,1,
    jump    loop,
exit,
lab:
  ADDI    R6,R1,10,
exit,
