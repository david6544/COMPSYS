// This file is based on part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: Mult.asm

// Multiplies R1 and R2 and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

@R2
D=M
@NEGATIVE_LOOP
D;JLT

    // Loop and add R1 to itself, R2 Times, until R2 is 0
(POSITIVE_LOOP)
    // Breaking the loop
    @R2
    D=M
    @END
    D;JEQ

    //Setting R1
    @R1
    D=M

    //Adding onto R0
    @R0
    M=M+D

    //Iterate down R2
    @R2
    M=M-1

    @POSITIVE_LOOP
    0;JMP

(NEGATIVE_LOOP) //Same as above, but for negative numbers
    @R2
    D=M
    @END
    D;JEQ

    @R1
    D=M

    @R0
    M=M-D

    @R2
    M=M+1

    @NEGATIVE_LOOP
    0;JMP


(END)
    @END
    0;JMP
