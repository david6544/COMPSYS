// Calculates R1 + R2 - R3 and stores the result in R0.
// (R0, R1, R2, R3 refer to RAM[0], RAM[1], RAM[2], and RAM[3], respectively.)

// Put your code here.


    // D = RAM 0
    @R1
    D = M

    // D = D + RAM 2
    @R2
    D = D + M

    // D = D - RAM 3
    @R3
    D = D - M

    // R0 = D
    @R0
    M = D

(END)
    @END 
    0;JMP


// End of your code.
