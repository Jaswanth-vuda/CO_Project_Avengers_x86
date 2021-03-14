.data
word:    .word 10,
.text
.globl main
main:
    LW      R1,0(word),
    ADDI    R2,R1,10,
    ADD     R3,R2,R1,
    SW      R3,0(word),
exit,