// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// Put your code here.
// Checking if the start of the array is past the 2nd postiion

    @R1
    D=M
    D=D-1
    D=D-1
    @END
    D;JLT
    D=D+1
    D=D+1
    @R0
    M=D
    @R2
    D=M
    @END
    D;JLT


(MAIN_LOOP)

    @R1
    A=M
    D=M


    @R0 
    D=D-M

    @IF_SMALLER
    D;JLT

    @NEXT_ELEMENT
    0;JMP


(IF_SMALLER)
    @R1
    A=M
    D=M
    @R0
    M=D

(NEXT_ELEMENT)

    @R1
    M=M+1

    @R2
    M=M-1

    @R2
    D=M
    @MAIN_LOOP
    D;JGT 

(END)
    @END
    0;JMP