/*-----------------
Andrew Morato
November 21, 2017
Machine Language (y86)
to C Interpreter
-----------------*/

//Preprocessor Directives
#include <stdio.h>
#include <stdlib.h>

//Macro's
#define MEMORY_BLOCK_SIZE 0x8
#define FIRST_REG regs[(memory[PC + 1] >> 4) & 0xF]
#define SECOND_REG regs[memory[PC + 1] & 0xF]
#define RSP regs[4]

//Function Headers
int Pop(int), Push(int), Call(int), Ret(int), Jump(int);
int IRmov(int), RRmov(int), RMmov(int), MRmov(int);
int OPx(int);
int printall(int), printreg(int),printmem(int);

//Helpers
long int readMem(long int);
void writeMem(long int, long int);

//Flags
char SF = 0x00;
char ZF = 0x00;

//Memory
extern char memory[];
extern int PC;

//Registers
long int regs[16];
char *regname[15] = {"%rax","%rcx","%rdx","%rbx","%rsp","%rbp",
                     "%rsi","%rdi","%r8","%r9","%r10","%r11",
                     "%r12","%r13","%r14"};

/*-----------------------------------------------------------------*/
/*------------------------------Driver-----------------------------*/
/*-----------------------------------------------------------------*/

void execute(int PC) {
   int done = 0;
   char byte; 
   char opcode;
   while (!done) {
   	 byte = memory[PC];
     opcode = (byte >> 4)&0xf;
     switch (opcode) {
	 	case 0: printf("Halting at instruction 0x%x\n", PC); 
			done = 1; break;  //halt
	 	case 1: PC++; break;   //nop
	 	case 2: PC = RRmov(PC); break;
	 	case 3: PC = IRmov(PC); break;
	 	case 4: PC = RMmov(PC); break;
	 	case 5: PC = MRmov(PC); break;
	 	case 6: PC = OPx(PC); break;
	 	case 7: PC = Jump(PC); break;
	 	case 8: PC = Call(PC); break;
	 	case 9: PC = Ret(PC); break;
	 	case 10: PC = Push(PC); break;
	 	case 11: PC = Pop(PC); break;
     	case 12: PC = printall(PC); break;
	 	case 13: PC = printreg(PC); break;
	 	case 14: PC = printmem(PC); break;
	 	default: printf("# unknown op at PC=0x%x\n",PC); PC++;
   	}
   }
}

/*-----------------------------------------------------------------*/
/*---------------------------Instructions--------------------------*/
/*-----------------------------------------------------------------*/

/*D0 rA:F - prints rA*/
int printreg(int PC) {
	printf("%s 0x%lx\n", regname[(memory[PC+1]>>4)&0xF], FIRST_REG);
	return PC + 2;
}

/*C0 - prints all registers*/
int printall(int PC) {
	for(int i = 0; i < (sizeof(regname) / sizeof(char *)); i++)
		printf("%s 0x%lx\n", regname[i], regs[i]);
	return PC + 1;
}

/*E0 rA:F D(8 Bytes) - prints 8 byte $imm at (rA) + D*/
int printmem(int PC) {
	long int address = (FIRST_REG + readMem(PC + 2));
	printf("0x%lx 0x%lx\n", address, readMem(address));
	return PC + 10;


/*30 F:rB D(8 Bytes) - sets rB to D*/
int IRmov(int PC) {
	SECOND_REG = readMem(PC + 2);
	return PC + 10;
}

/*2fn rA:rB - sets rA to rB if condition fn is met*/
int RRmov(int PC) {
	int cond = (memory[PC] & 0xF);
	if (cond == 0) SECOND_REG = FIRST_REG; //always
	if (cond == 1 && (SF != 0 || ZF != 0)) SECOND_REG = FIRST_REG;
	if (cond == 2 && SF != 0) SECOND_REG = FIRST_REG;
	if (cond == 3 && ZF != 0) SECOND_REG = FIRST_REG;
	if (cond == 4 && ZF == 0) SECOND_REG = FIRST_REG;
	if (cond == 5 && SF == 0) SECOND_REG = FIRST_REG;
	if (cond == 6 && SF == 0 && ZF == 0) SECOND_REG = FIRST_REG;
	if (cond > 6) printf("# unknown move op at PC=0x%x\n", PC);
	return PC + 2;
}

/*40 rA:rB D(8 Bytes) - sets the 8 bytes at (rB) + D to rA*/
int RMmov(int PC) {
	writeMem(readMem(PC + 2) + SECOND_REG, FIRST_REG);
	return PC + 10;
}

/*50 rA:rB D(8 Bytes) - sets rA to the 8 byte $imm at (rB) + D*/
int MRmov(int PC) {
	FIRST_REG = readMem(readMem(PC + 2) + SECOND_REG);
	return PC + 10;
}

/*7fn D(8 Bytes) - sets the PC to D if condition fn is met*/
int Jump(int PC) {
	int cond = (memory[PC] & 0xF), address = readMem(PC + 1);
	if (cond == 0) return address; //jmp
	if (cond == 1 && (SF != 0 || ZF != 0)) return address; //jle
	if (cond == 2 && SF != 0) return address; //jl
	if (cond == 3 && ZF != 0) return address; //je
	if (cond == 4 && ZF == 0) return address; //jne
	if (cond == 5 && SF == 0) return address; //jge
	if (cond == 6 && SF == 0 && ZF == 0) return address; //jg
	if (cond > 6) printf("# unknown jump op at PC=0x%x\n", PC);
	return PC + 9;
}

/*80 D(8 Bytes) - sets the PC to D, saves the return address*/
int Call(int PC) {
	RSP = RSP - MEMORY_BLOCK_SIZE;
	writeMem(RSP, PC + 9);
	return readMem(PC + 1);
}

/*90 - goes back to the previous call*/
int Ret(int PC) {
	RSP = RSP + MEMORY_BLOCK_SIZE;
	return readMem(RSP - MEMORY_BLOCK_SIZE);
}

/*A0 rA:F - pushes (rA) onto the stack and edits rsp*/
int Push(int PC) {
	RSP = RSP - MEMORY_BLOCK_SIZE;
	writeMem(RSP, FIRST_REG);
	return PC + 2;
}

/*BO rA:F - sets (rA) to the stack's top and edits rsp*/
int Pop(int PC) {
	FIRST_REG = readMem(RSP);
	RSP = RSP + MEMORY_BLOCK_SIZE;
	return PC + 2;
}

/*6fn rA:rB - stores operation fn in rB and sets flags accordingly*/
int OPx(int PC) {
	int cond = (memory[PC] & 0xF);
	if (cond == 0) SECOND_REG = SECOND_REG + FIRST_REG; //add
	else if (cond == 1) SECOND_REG = SECOND_REG - FIRST_REG; //sub
	else if (cond == 2) SECOND_REG = SECOND_REG & FIRST_REG; //and
	else if (cond == 3) SECOND_REG = SECOND_REG ^ FIRST_REG; //xor
	else printf("# unknown mathematical op at PC=0x%x\n", PC);
	SF = (SECOND_REG < 0);
	ZF = (SECOND_REG == 0);
	return PC + 2;
}

/*-----------------------------------------------------------------*/
/*------------------------------Helpers----------------------------*/
/*-----------------------------------------------------------------*/

/*returns the 8 bytes, as a long int, beginning at mem[address]*/
long int readMem(long int address) {
	long int constant = 0;
	for (int i = MEMORY_BLOCK_SIZE - 1; i >= 0; i--)
		constant = constant << 8 | (memory[address + i] & 0xFF);
	return constant;
}

/*puts the 8 bytes of constant into mem[address] - mem[address + 8]*/
void writeMem(long int address, long int constant) {
	for (int i = 0; i < MEMORY_BLOCK_SIZE; i++)
		memory[address + i] = ((constant >> (8 * i)) & 0xFF);
}