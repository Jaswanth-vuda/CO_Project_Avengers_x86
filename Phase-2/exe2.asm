.data
.text
.globl main
main:
    ADDI R2,R2,4,
    ADDI R3,R3,5,
    ADD R1,R2,R3,
    SUB R4,R2,R3,
    ADD R5,R1,R4,
if:
    BNE     R2,R3,else,
    SUBI    R10,R5,15,
    ADD     R11,R10,R5,
exit,
else:
  ADDI    R6,R1,10,