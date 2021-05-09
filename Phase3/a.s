.data

.text
.globl main
main:
    ADD     R1,R2,R3,
    SUB    R4,R2,R3,
    ADD     R5,R1,R4,
    ADDI     R18,R18,3,
for:
    BEQ     R2,R3,lab1,

    ADDI    R2,R2,1,
    ADDI R17,R17,0,
    jump    for,

exit,
lab1:
  ADDI    R6,R1,10,
exit,