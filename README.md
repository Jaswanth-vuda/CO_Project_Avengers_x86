# CO_Project_Avengers_x86


 Added instructuions are  ADD,SUB,ADDI,SUBI,BNE,jump,BEQ,LW,SW
# every instruction should be written in capitals like ADD,SUB but jump instruction has to be written in small example jump.
 
 After each instruction there should be comma for termination of instruction, for .data and .text and .globl main and labels should not write commas and spaces
 
 # For exiting the file  write command exit in new line with comma example:      exit,
 
# Number registers are 32; and we declares registers as int R[32]
# for accesing registers write Rn  where n value varies from 0 to 31
example R1 refers to register 1->R[1];
# memory we declared memory for 4kb as char mem[4096];
  # we included only one directive .word for storing the integer values to memory


# general syntax for labels and data,text segments

# .text has to written only after .data each one in new line and after these words there should not be any spaces and commas

# At the end of the file like jr $ ra there sholud be commant for exitting the file that command is exit follwed by comma. that is exit,

# after each label declaration if there is no instructions to execute write command exit, in new line.


# syntax for each instructions
ARRAY: .word 10,20,30,40,
ADD R1,R2,R3,
SUB R4,R5,R6,
SUBI R3,R4,9,
ADDI R5,R1,3,
LW R2,0(ARRAY),
SW R2,4(ARRAY),
BNE R2,R3,label,
BEQ R4,R5,label1,
jump label,

for running  the file simulator.cpp
use command g++ simulator.cpp
it will asks the file to be entered to read the file
you have to write the file name followed with its extension example(a.asm,sample.s)
.asm and .s files are only allowed
Before runnung the simulator.cpp
you should create a file with extension .asm or .s and write your code in it and save it. then run the file with this file name.

# contribution 
we done maximum work together 
CS19B022 Decoding and Data segment 
CS19B042 execution, syntax ,and ,file reading.
we written code  at same time with each other ideas .
