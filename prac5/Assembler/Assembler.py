instruction_type = ['NULL','A_INSTRUCTION','C_INSTRUCTION','L_INSTRUCTION']

instruction_dest = ['NULL','M','D','MD','A','AM','AD','AMD']

instruction_jump = ['NULL','JGT','JEQ','JGE','JLT','JNE','JLE','JMP']

instruction_comp = ['NULL',
                    '0','1','-1',
                    'A','M','D',
                    '!A','!M','!D',
                    '-A','-M','-D',
                    'A+1','M+1','D+1',
                    'A-1','M-1','D-1',
                    'D+A','D+M',
                    'D-A','D-M','A-D','M-D',
                    'D&A','D&M',
                    'D|A','D|M']

sTable = {
    "R0" : "0",
    "R1" : "1",
    "R2" : "2",
    "R3" : "3",
    "R4" : "4",
    "R5" : "5",
    "R6" : "6",
    "R7" : "7",
    "R8" : "8",
    "R9" : "9",
    "R10" : "10",
    "R11" : "11",
    "R12" : "12",
    "R13" : "13",
    "R14" : "14",
    "R15" : "15",
    "SP" : "0",
    "LCL" : "1",
    "ARG" : "2",
    "THIS" : "3",
    "THAT" : "4",
    "SCREEN" : "16384",
    "KBD" : "24576",
}

dTable = {
        "NULL": "000",
        "M": "001",
        "D": "010",
        "MD": "011",
        "DM": "011",
        "A": "100",
        "AM": "101",
        "AD": "110",
        "AMD": "111"
    }

jTable = {
    "NULL": "000",
    "JGT": "001",
    "JEQ": "010",
    "JGE": "011",
    "JLT": "100",
    "JNE": "101",
    "JLE": "110",
    "JMP": "111"
}

cTable = {
    "NULL": "0000000",
    "0": "0101010",
    "1": "0111111",
    "-1": "0111010",
    "D": "0001100",
    "A": "0110000",
    "!D": "0001101",
    "!A": "0110001",
    "-D": "0001111",
    "-A": "0110011",
    "D+1": "0011111",
    "A+1": "0110111",
    "D-1": "0001110",
    "A-1": "0110010",
    "D+A": "0000010",
    "D-A": "0010011",
    "A-D": "0000111",
    "D&A": "0000000",
    "D|A": "0010101",
    "M": "1110000",
    "!M": "1110001",
    "-M": "1110011",
    "M+1": "1110111",
    "M-1": "1110010",
    "D+M": "1000010",
    "D-M": "1010011",
    "M-D": "1000111",
    "D&M": "1000000",
    "D|M": "1010101"
}

class SymbolTable:
    def __init__(self):
        """
        Symbol table constructor
        """
        pass

    def addSymbol(self, symbol, value):
        """
        Adds a symbol to the symbol table

        @param symbol: The name of the symbol
        @param value: The address for the symbol
        """
        
        sTable[symbol] = value

        pass


    def getSymbol(self, symbol):
        """
        Gets a symbol from the symbol table
        
        @param symbol: The name of the symbol
        @return: The address for the symbol or -1 if the symbol isn't in the table
        """      
        if symbol in sTable:
            return sTable[symbol]  
        
        return -1


class Assembler:

    def __init__(self):
        """
        Assembler constructor
        """
        pass


    def buildSymbolTable(self, instructions, symbolTable):
        """
        Assembler first pass; populates symbol table with label locations.

        @param instructions: A list of the assembly language instructions.
        @param symbolTable: The symbol table to populate.
        """
        counter = 0
        for inst in instructions:
            intType = self.parseInstructionType(inst)
            if intType == "A_INSTRUCTION":
                label = self.parseSymbol(inst)
                if sTable.get(label) == None and label.isdigit() == False:
                    counter += 1
                    sTable[label] = 15 + counter
                elif sTable.get(label) == None:
                    sTable[label] = int(label)
        
        for symbol in sTable: 
            print(symbol, sTable[symbol])
        
        pass
    

    def generateMachineCode(self, instructions, symbolTable):
        """
        Assembler second pass; Translates a set of instructions to machine code.

        @param instructions: A list of the assembly language instructions to be converted to machine code.
        @param symbolTable: The symbol table to reference/update.
        @return: A String containing the generated machine code as strings of 16-bit binary instructions, 1-per-line.
        """
        
        instructions2 = []
        counter = 0
        for inst in instructions:
            counter += 1
            intType = self.parseInstructionType(inst)
            if intType == "A_INSTRUCTION":
                counter -= 1
                symb = self.parseSymbol(inst)
                if symb in sTable or symb:
                    newsymb = self.translateSymbol(symb, sTable)
                    instructions2.append("0" + newsymb)
                else:
                    sTable[symb] = counter
            elif intType == "C_INSTRUCTION":
                    dest = self.parseInstructionDest(inst)
                    bDest = self.translateDest(dest)
                    comp = self.parseInstructionComp(inst)
                    bComp = self.translateComp(comp)
                    jump = self.parseInstructionJump(inst)
                    bJump = self.translateJump(jump)
                    instructions2.append("111" + bComp + bDest + bJump)
            else:
                continue
            print(inst)

        for i in instructions2:
            print(i)

        

        
        return instructions2
       

    def parseInstructionType(self, instruction):
        """
        Parses the type of the provided instruction

        @param instruction: The assembly language representation of an instruction.
        @return: The type of the instruction (A_INSTRUCTION, C_INSTRUCTION, L_INSTRUCTION, NULL)
        """
        
        if instruction[0] == "@":
            return "A_INSTRUCTION"
        elif instruction[0] == "(":
            return "L_instruction"
        else:
            return "C_INSTRUCTION"
    

    def parseInstructionDest(self, instruction):
        """
        Parses the destination of the provided C-instruction

        @param instruction: The assembly language representation of a C-instruction.
        @return: The destination of the instruction (see instruction_dest) 
        """
        """ return the substring of the instruction until the = sign is reached, or until the ; sign is reached """
        if (instruction.find("=") != -1 ):
            print("test1")
            return instruction.split("=")[0]
        elif (instruction.find(";") != -1 ):
            print("test2")
            if (instruction[0] == '0'):
                return "NULL"
            return instruction.split(";")[0]
    
        return "NULL"
    

    def parseInstructionJump(self, instruction):
        """
        Parses the jump condition of the provided C-instruction

        @param instruction: The assembly language representation of a C-instruction.
        @return: The jump condition for the instruction (see instruction_jump)
        """
        """ return the substring of the instruction after the ; sign is reached """

        if (instruction.find("=") != -1 ):
            return "NULL"
        elif (instruction.find(";") != -1 ):
            return instruction.split(";")[1]
    
        return "NULL"
    

    def parseInstructionComp(self, instruction):
        """
        Parses the computation/op-code of the provided C-instruction

        @param instruction: The assembly language representation of a C-instruction.
        @return: The computation/op-code of the instruction (see instruction_comp)
        """
        """ return the substring of the instruction after the = sign """
        counter = 0
        for i in instruction:
            counter += 1
            if i == "=":
                return instruction[counter:]
            
        return "NULL"
    
    
    def parseSymbol(self, instruction):
        """
        Parses the symbol of the provided A/L-instruction

        @param instruction: The assembly language representation of a A/L-instruction.
        @return: A string containing either a label name (L-instruction), 
                a variable name (A-instruction), or a constant integer value (A-instruction)
        """

        if instruction[0] == "@":
            return instruction[1:]
        elif instruction[1] == "(":
            return instruction[2:-1]

        return ""
    

    def translateDest(self, dest):
        """
        Generates the binary bits of the dest part of a C-instruction

        @param dest: The destination of the instruction
        @return: A String containing the 3 binary dest bits that correspond to the given dest value.
        """
        return dTable[dest]
    

    def translateJump(self, jump):
        """
        Generates the binary bits of the jump part of a C-instruction

        @param jump: The jump condition for the instruction
        @return: A String containing the 3 binary jump bits that correspond to the given jump value.
        """
        return jTable[jump]
    
    
    def translateComp(self, comp):
        """
        Generates the binary bits of the computation/op-code part of a C-instruction

        @param comp: The computation/op-code for the instruction
        @return: A String containing the 7 binary computation/op-code bits that correspond to the given comp value.
        """
        return cTable[comp]
    
    
    def translateSymbol(self, symbol, symbolTable):
        """
        Generates the binary bits for an A-instruction, parsing the value, or looking up the symbol name.

        @param symbol: A string containing either a label name, a variable name, or a constant integer value
        @param symbolTable: The symbol table for looking up label/variable names
        @return: A String containing the 15 binary bits that correspond to the given sybmol.
        """
        
        print(symbol, " -- ",sTable[symbol], " -- ", int(sTable[symbol]))

        return bin(int(sTable[symbol]))[2:].zfill(15)
    

# A quick-and-dirty parser when run as a standalone script.
if __name__ == "__main__":
    import sys
    if(len(sys.argv) > 1):
        instructions = []
        # Open file
        with open(sys.argv[1], "r") as a_file:
            # Read line-by-line, skip comments and empty line
            for line in a_file:
                if line[0] != '/' and line[0] != "\n":
                    instructions.append(line.strip())
        assembler = Assembler()
        symbolTable = SymbolTable()
        # First pass
        assembler.buildSymbolTable(instructions,symbolTable)
        # Second pass
        code = assembler.generateMachineCode(instructions,symbolTable)
        # Print output
        print(code)
