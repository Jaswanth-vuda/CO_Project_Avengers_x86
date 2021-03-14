.data
.text
.globl main
main:
     ADDI  R2,R2,1,
     ADDI  R3,R3,4,
for:
    BEQ     R2,R3,lab1,
    SUBI    R1,R5,15,
    ADD     R16,R10,R5,
    ADDI    R2,R2,1,
    jump    for,
exit,
lab1:
  ADDI    R6,R1,10,
exit,