//Multiplying by a big number

load Mult.asm,
output-file Mult02.out,
compare-to Mult02.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2;

set PC 0,
set RAM[0] 0,  // Set R0
set RAM[1] 1231,  // Set R1
set RAM[2] 23;  // Set R2

// For this case, needs more clock cycles to finish the Multiplication
repeat 300 {
  ticktock;    // Run for 300 clock cycles
}
set RAM[1] 1231,  // Restore arguments in case program used them
set RAM[2] 23,
output;        // Output to file

