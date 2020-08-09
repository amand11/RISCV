				*** CS204(computer architecture) - Project ***
				----------------------------------------------
                                            PHASE -  1

Group members (entry number)-
	*Prateek Saini (2018CSB1114)
	*Naveen Yadav  (2018CSB1106)
	*Aman Dhawan   (2018CSB1070)
	*Vikram Singh  (2018CSB1194)
	*Vishal Yadav  (2018CSB1128)

	  

----------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------------------------
Work by Indviduals:-
------------

Prateek Saini ->-overall structure of code(main.cpp)
		-I,R,SB and UJ formats 
		-.data,.text part exection and Documentation.
Naveen Yadav -> I,R format 
Aman Dhawan ->U,S format 
Vikram -> UJ format


Instructions to run program -
	a. Open command prompt, and go to the directory where you have kept the files.
	b. Enter command "g++ main.cpp". (this wil compile the code, and create an executable file with name "a.exe").
	c. Just give command "a.exe". (code will be executed).
	d. Input file will given with name "sample.txt".
	e. Output file with name "example.mc" will be created in same directory.
	f. Any other Input file can be given in file "main.cpp" (in place of sample.txt) in line no. 400.

Algorithm in simple word -
	a. First we read the line by line of the file and by name of the command(like add ,addi etc.) we find its opcode, func3,func7,etc.
	b. we make functions for each format like r,i,u etc. and call these function to arrange our opcode func3 rs1 and rs2 thing in a proper manner
	   for ex.(in r - func7+rs2+rs1+func3+rd+opcode )
	c. After this we get a 32 bit string and we convert it into hexadecimal format and write in our output file.
 	   (Encoding each instructions based on their format. and  writing their hexadecimal value in our output file).	

Output format -
	a. For each instruction, we have printed 2 values in each line, in which first is denoting the PC value (in hex),
	   and second string is machine instruction of that instruction (in hex).
	b. After all instruction's machine code has been printed. we printed END PROGRAM.
	c. After Endofprogram. we have printed the Data memory format with two values separated.
	   Here, first is for memory address and second is values of variables defined in .data section. (both are printed in hex).

Assumptions -
	a. As given by sir, We have given 0x10000000 as the base address of Data memory.
	b. Psuedo instructions are not supported.
	c. -For comment, Input file must not contain comment.
	   -After a label the next instruction if present should start from the same line with a sapce(" ") between : and the instruction.
	   -In instruction - the tokens of instruction must be seperated by a , not " "
	   -For ex.(add x1 x2 x3    must be written      add x1,x2,x3)
	   -After ending of instruction no tap or space must be given
	   -For ex.("add x1,x2,x3"-correct     "add x1,x2,x3     "-incorrect)
	d. We have not covered all (except the 4 error mentioned in the google classroom comments) Syntax error cases. So we assume a syntatically & semantically correct input file.
	e. Only the instructions provided in "reference sheet manual of risc-v", are supported.	 
	f. for lw command  ---   lw register,label like commands are not supportive. 	