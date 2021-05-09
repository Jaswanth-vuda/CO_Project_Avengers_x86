.data
word:    .word 10,20,30,40,50,60,
.text
.globl main
main:
    LW      R1,0(word),
    LW      R7,5(word),
    ADDI    R2,R1,10,
    ADD     R3,R2,R1,
    SW      R3,4(word),
exit,