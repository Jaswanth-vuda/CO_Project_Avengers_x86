# CO_Project_Avengers_x86

# About Instructions,Registers and Memory :
 Added instructuions are  ADD,SUB,ADDI,SUBI,BNE,jump,BEQ,LW,SW,MULI
 Every instruction should be written in capitals like ADD,SUB but jump instruction has to be written in small, example jump.
 
 After each instruction there should be a comma for termination of instruction, for .data and .text and .globl main and labels should not write commas and spaces
 
 For exitting the file  write command "exit" in new line with a comma example:      exit,
#
Number of registers are 32; and we declared registers as int R[32]
 for accesing registers write Rn  where n value varies from 0 to 31
 example:  R1 refers to register 1->R[1];
 #
Memory:we declared memory for 4kb as char mem[4096];
 we included only one directive ".word" for storing the integer values to memory or data segment


# -> General syntax for labels and data,text segments:

".text" has to be written only after ".data" each one in new line and after these words there should not be any spaces and commas

 At the end of the file ,like "jr $ ra" in MIPS, there sholud be a command for exitting the file, that command is exit follwed by comma. that is "exit,"

 after each label declaration if there is no instructions to execute write command exit, in new line.


-> syntax for each instructions:
ARRAY: .word 10,20,30,40,
ADD R1,R2,R3,
SUB R4,R5,R6,
SUBI R3,R4,9,
ADDI R5,R1,3,
LW R2,0(ARRAY),
SW R2,4(ARRAY),
BNE R2,R3,label,
BEQ R4,R5,label1,
MULI R1,R2,R3,
jump label,
label1:
   ADD R3,R5,R7,
exit,
label:
  SUBI R5,R6,R7,
exit,
![Syntax](https://user-images.githubusercontent.com/73153529/111679105-f7b73780-8846-11eb-9ae0-7c555561129b.png)


for running  the file simulator.cpp:
    1. Use command g++ simulator.cpp 
    2. It will ask the file name to be opened to read the file
    3. You have to write the file name followed by its extension example(a.asm,sample.s)
                  (".asm" and ".s" files are only allowed)
    4. Before running the simulator.cpp,
            you should create a file with extension .asm or .s and write your code in it and save it and then run the file with this file name.

# contribution 
we done maximum work together(everything from first to last line of the code that we have written)
CS19B022 Decoding and Data segment 
CS19B042 execution, syntax ,and ,file reading.
we written with each others ideas .

