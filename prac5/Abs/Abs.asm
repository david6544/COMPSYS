// Calculates the absolute value of R1 and stores the result in R0.
// (R0, R1 refer to RAM[0], and RAM[1], respectively.)

// Put your code here.

    // Reading in R1
    @R1
    D=M

    // Store the number in R0
    @R0
    M=D

    // Jump to the end if the number is >= 0
    @END
    D;JGE

    // Else we place the negative of the value in R0
    @R0
    M=-D

(END)
    0;JMP