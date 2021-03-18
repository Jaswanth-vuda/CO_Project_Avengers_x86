.data
arr: .word 30,10,20,
.text
.globl main
main:

    ADDI     R3,R3,3,
    ADDI     R4,R4,3,

for:
    BEQ     R2,R3,lab1,
    LW      R1,0(arr),
for1:
    BEQ    R5,R4,lab2,
    MULI   R16,R5,4,
    LW     R30,R16(arr),
    SGT    R29,R30,R1,
    ADD     R11,R10,R5,
    ADDI    R5,R5,1,
    jump for1,
lab2:
    ADDI R5,R0,0,
    SUBI  R3,R3,1,
    MULI R16,R3,4,
    LW  R30,R16(arr),
    SW R30,R29(arr),
    SW R1,R16(arr),
    SUBI R4,R4,1,
    jump    for,

exit,
lab1:
  ADDI    R6,R1,10,
exit,