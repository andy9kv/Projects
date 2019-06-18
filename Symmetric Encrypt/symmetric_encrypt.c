
/*
Andrew Morato
October 13, 2015
Symmetric Encryption
Symmetric Decryption
*/

//Preprocessor Directives
#include <stdio.h>
#include <stdlib.h>

//Function Prototypes
char crypt(char, int); //encrypts and decrypts 
void append(char *, char); //writes to output file

/*cmd line args: ./a.out <key: int> <input file> <output file>*/
int main(int argc, char *argv[]){
	//assigns command line arguments to variables
	char *key = argv[1];
	char *input_file = argv[2];
	char *output_file = argv[3];
	//turns key into a usable int
	int numKey = 0;
	numKey = atoi(key);

	//reads the file, does the work while reading each char
	FILE *fp;
	fp = fopen(input_file, "r");
	int c = 0;
	while((c = fgetc(fp)) != EOF){
		char current = c;
		char changed = ' ';
		changed = crypt(current, numKey);
		append(output_file, changed);
	}
	fclose(fp);

	return 0;
}

//takes care of the logic for encrypting/decrypting
char crypt(char convert, int key){
	srandom(key); //random seeding
	char crypted = ' '; //the crypted char to return
	int crypt = 0; //used to do the math in this method
	int crypt1 = 0; //used to do the math in this method
	int r = random() % 97; //random number

	// change all displayable characters to range [0. . . 96]
	if(convert == '\t')
		crypt = 0;
	else if(convert == '\n') 
		crypt = 1;
	else 
		crypt = convert - 30;
	crypt1 = crypt ^ r; // bitwise xor

	//turns all output values into displayable characters
	if(crypt1 == 0) 
		crypted = '\t';
	else if(crypt1 == 1) 
		crypted = '\n';
	else 
		crypted = crypt1 + 30;
	return crypted;
}

//writes to the output file char by char
void append(char *output_file, char add){
	FILE *fp;
	//opens file with a for append
	fp = fopen(output_file, "a");
	fputc(add, fp);
	fclose(fp);
}