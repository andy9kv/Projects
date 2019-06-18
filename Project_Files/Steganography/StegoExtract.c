/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * StegoExtract.c: A program for manipulating images                    *
 * Andrew Morato, G00892138                                             *
 * CS262, Lab Section: 205                                              *
 * Project 3, Steganography                                             *
 * Wednesday December 2, 2015                                           *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "image.h"

//Arguments are the stego file and the output file where the embedded payload will be written
int main(int argc, char *argv[]){  
 	struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
 	struct Buffer b = {NULL, 0, 0};
 	 if (argc != 3) 
	{
      printf("\n%s <stego_file> <output_file> \n", argv[0]);
      exit(1);
    }

    ReadImage(argv[1],&img);     // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros
    

    //Extracts and prints b.size to stdout
    b.size = extractBSize(argv[1], &img);
    printf("   Extracted b.size: %d\n", b.size);
    
    //Extracts and prints G Number to stdout
    char Gnum[] = "00000000";
	printf("   Extracted G Number: %s\n", extractGNum(argv[1], &img, Gnum));
	
	//Writes embedded payload to the output file
	int i = 0;
	printf("\nExtracting Payload from %s\n", argv[1]);
	b.data = malloc(b.size * sizeof(byte));//make space for data
	for(i = 0; i < b.size; i++){
		byte temp = extractByte((64+(8*i)), &img);//extracts a byte from the right place in img
		SetByte(i, temp);//sets the extracted byte to b.data
	}

	WriteBinaryFile(argv[2], b); //writes payload to output file

	free(b.data);
	return 0;
}

//--------------------------------------------------------------------------------------
//                                     Helpers
//--------------------------------------------------------------------------------------

//extracts b.size from the appropiate place in the stego file, looping
//32 times, then forms a char* that holds b.size and returns it as an int 
int extractBSize(char *name, struct Image *img){
	printf("\nExtracting b.size from '%s'\n", name);
	int i = 0;
	int bsize = 0;//all 32 bits are 0
	while(i<32){
		if(getBit(img->gray[i], 0) != getBitFromInt(bsize, (i)))//if LSB of GetGray(i) != 0
			bsize = toggleIntBit(bsize, (i));//sets ith bit of b.size = LSB of GetGray(i)
		i++;
	}
	return bsize;
}

//extracts the G number from the appropiate place in the stego file by calling
//extractGDigit 8 times, then forms a char* that holds the entire G number
char *extractGNum(char *name, struct Image *img, char *Gnum){
	printf("\nExtracting G Number from '%s'\n", name);
	int i = 0;
	while(i<8){
		byte add = extractGDigit(((4*i)+32), img);//extract from appropiate place from IMG
		char digit = add + 48; //converts the binary representation of digit to ascii rep
		Gnum[8-(i+1)] = digit;//adds the byte 'add' to the last spot not taken in char array Gnum
		i++;
	}
	return Gnum; //returns Gnum
}

//extract each digit from the stego file. uses a variation of GetGray to allow for
//pointers. returns the extracted digit
byte extractGDigit(int N, struct Image *img){
	int i = 0;
	byte digit = 0;//all 0 byte
	while(i < 4){//4 times because of 4 bits per digit
		if(1 == getBit(img->gray[(N + i)], 0)) //if last bit NthCoverByte == 00000001
			digit = toggleBit(digit, i); //sets the ith bit of the G number digit to 1
		i++;
	}
	return digit;
}

//extracts a byte starting at the Nth bit in img
//combines the byte from the LSB's and returns it 
byte extractByte(int N, struct Image *img){
	int i = 0;
	byte bits = 0;//all 0 byte
	while(i<8){
		if(getBit(img->gray[(N + i)], 0) != 0)//if LSB of Nth byte of img == 1
			bits = toggleBit(bits, i); //sets the ith bit of bits to 1
		i++;
	}
	return bits;
}

//Creates an all-zero byte and matches the last bit
//to the 'N'th bit of 'bits' and returns the new byte
byte getBit(byte bits, int N){
  byte bit = 0; //right now all 0's
  bit = ((bits >> N)  & 0x01);
  return bit;
}

//same as function above but takes bit N from an 'num' instead
byte getBitFromInt(int num, int N){
  byte bit = 0; //right now all 0's
  bit = ((num >> N)  & 0x01);
  return bit;
}

//Toggles the 'N'th bit of 'bits' and returns it
byte toggleBit(byte bits, int N){
  bits ^= 1 << N;
  return bits;
}

//Toggles the 'N'th bit of 'num' and returns it
int toggleIntBit(int num, int N){
  num ^= 1 << N;
  return num;
}