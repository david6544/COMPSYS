// Sorts the array of length R2 whose first element is at RAM[R1] in ascending order in place. Sets R0 to True (-1) when complete.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)


// Put your code here.
	@outer_iterator
	M=0

(OUTER_LOOP)
	@inner_iterator
	M=0

(INNER)
	// *(R1+inner_iterator)
	@R1
	D=M

    // iterating through the array
	@inner_iterator
	A=M+D
	D=A

    //temporarily store the address of the inner_iterator
	@first_val
	M=D
	@second_val
	M=D+1

    //grabbing the data at that address
    @first_val
	A=M
	D=M

    //if negative
    @IFNEG
    D;JLT
    //if positive
    @IFPOS
    D;JGT

(COMPARE)
    // check if (first_val > second_val)
    @first_val
	A=M
	D=M
	@second_val
	A=M
	D=D-M
    //swapping if greater than 0
	@SWAP
	D;JGT

(CHECKER)
    // check if (inner_iterator >= R2-outer_iterator-1)
	@outer_iterator
	D=M

    // changing length of  bounds
	@R2
    D=D+1
	D=M-D

    //iterating inner
	@inner_iterator
	M=M+1
	D=D-M

    //jumping to the inner loop
	@INNER
	D;JGT

	// check if (outer_iterator >= R2-1)
	@outer_iterator
	M=M+1
	D=M

    //decrementing the length of the bounds
	@R2
	D=M-D
	D=D-1

    //back to the outer loop
	@OUTER_LOOP
	D;JGT

(END)
	@R0
	M=-1
	@END
    0;JMP


(SWAP)
	@first_val
	A=M
	D=M
    //temporary value to store Firstval
	@R4
	M=D

    //swaps them
	@second_val
	A=M
	D=M
	@first_val
	A=M
	M=D

    //uses temp1 to put secondval as firstval
	@R4
	D=M
	@second_val
	A=M
	M=D

    @CHEKCER
    0;JMP



(IFNEG)
    //grabbing the data at second value
    @second_val
    A=M
    D=M

    //checking if greater than 0
    @CHECKER
    D;JGT

    //comparing the vals otherwise
    @COMPARE
    0;JMP



    // if the value is positive
(IFPOS)
    //grabbing the data at second value
    @second_val
    A=M
    D=M
    
    //swapping if less than 0
    @SWAP
    D;JLT

    // Otherwise comparing
    @COMPARE
    0;JMP

