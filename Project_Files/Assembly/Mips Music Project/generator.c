/********************************************************************|
| Author: Andrew Morato                                              |
| C program to generate a mips assembly file (.asm) that plays the   |
| tunes that was extracted by 'extract.c'                            |
|********************************************************************/

//Preprocessor Directives
#include "extract.c"
#define VOLUME 127
#define SYSCALL 33
#define INSTR 4
#define PACE 300

//Function Prototypes
void write_string(char*, char*);
void write_int(char*, int);
void write_block(char *, int);


//command line args: <read_filename> <write_filename>
int main(int argc, char *argv[]){

	//establishes the command line file input
	char *input_file = argv[1];
	char *output_file = argv[2];

	//defines several strings to use for the mips code
	char *begin = "li $v0, 33 \n li $a1, 300 \n li $a2, 4 \n li $a3, 127 \n";
	char *load = "li $a0, ";
	char *syscall = "\n syscall \n";

	//reads the given file using extract.c's functions
	struct node *head = read_arduino_file(input_file);

	//writes the necessary beginings to the assembly file
	write_string(output_file, begin);

	struct node *n = head->next;
	//writes the mips program to the output file
	while(n) {
		write_block(output_file, n->val);
		n = n->next;
	}

	//writes the necessary endings to the assembly file
	char *end = "li $v0, 10 \n syscall";
	write_string(output_file, end);

	return 0;
}

//writes to the output file char by char
void write_string(char *output_file, char* add){
	FILE *fp = fopen(output_file, "a");
	fputs(add, fp);
	fclose(fp);
}

//writes an int to the specified file
void write_int(char *output_file, int add){
	FILE *fp = fopen(output_file, "a");
	fprintf(fp,"%d ", add);
	fclose(fp);
}

//writes a block of mips music code
void write_block(char *output_file, int add){
	FILE *fp = fopen(output_file, "a");
	fputs("li $a0, ", fp);
	fprintf(fp,"%d ", add);
	fputs("\n syscall \n", fp);
	fclose(fp);
}