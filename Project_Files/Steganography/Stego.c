/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * Stego.c: A program for manipulating images                           *
 * Andrew Morato, G00892138                                             *
 * CS262, Lab Section: 205                                              *
 * Project 3, Steganography                                             *
 * Wednesday December 2, 2015                                           *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "image.h"

int main(int argc, char *argv[]) //Args are input output and payload
{  
  int i = 0, j = 0, k = 0, cover_bits = 0, bits = 0;
  struct Buffer b = {NULL, 0, 0};
  struct Image img = {0, NULL, NULL, NULL, NULL, 0, 0};
   
  if (argc != 4) 
    {
      printf("\n%s <cover_file> <stego_file> <file_to_hide> \n", argv[0]);
      exit(1);
    }
  ReadImage(argv[1],&img);       // read image file into the image buffer img
                                 // the image is an array of unsigned chars (bytes) of NofR rows
                                 // NofC columns, it should be accessed using provided macros
  ReadBinaryFile(argv[3],&b);    // Read binary data
 

  // hidden information ./
  // first four bytes is the size of the hidden file
  // next 4 bytes is the G number (4 bits per digit)
  if (!GetColor)
    cover_bits = img.NofC*img.NofR;
  else 
    cover_bits = 3*img.NofC*img.NofR;    
  bits = (8 + b.size)*8;
  if (bits > cover_bits)
    {
      printf("Cover file is not large enough %d (bits) > %d (cover_bits)\n",bits,cover_bits);
      exit(1);
    }

  //embeds four size bytes for the Buffer's size field
  printf("\nEmbedding b.size to '%s'\n", argv[2]);
  printf("   b.size: %d\n", b.size);
  while(j<32){ //32 iterations needed to embed b.size's 32 bits
    byte bit = getBitFromInt(b.size, j);//gets the jth bit of b.size
    embed(bit, j, &img);//embeds the jth bit of b.size in the last bit of GetGray(j)
    j++;
  }

  //embeds the eight digits of my G# using 4 bits per digit
  int GNum = 892138;
  printf("\nEmbedding G Number to '%s'\n", argv[2]);
  printf("   G Number: 00892138\n");
  while(k<8){////8 digits in G number, call embedGDigit for each
    int digit = GNum%10;//gets last G number digit
    embedGDigit(digit, (32+(k*4)), &img);//embeds the G number digit in the right place
    GNum = GNum / 10;//shifts to the next digit in the G number
    k++;
  }
     
  //embeds the information (payload) into the cover one byte at the time.
  //Only least significant bits are changed. Uses function embed()
  printf("\nEmbedding payload to '%s'\n", argv[2]);
  for (i=0; i<b.size; i++){
    int k = 0;
    while(k<8){
      //embeds the kth bit from byte i from b.data. The 64 accounts for b.size and Gnum
      embed(getBit(GetByte(i), k), (64+(8*i)+k), &img);//finds the right spot to embed
      k++;
    }
  }
  WriteImage(argv[2],img);  // output stego file (cover_file + file_to_hide)
  return 0;
}

//--------------------------------------------------------------------------------------
//                                     Helpers
//--------------------------------------------------------------------------------------

/*Takes the last bit of the 'N'th byte of the cover and sets it
  equal to the last bit of 'check' and then uses a variation of SetGray(N) to embed.
  'check' is the bit we want to embed, either 00000001 or 00000000, comes from getBit*/
void embed(byte check, int N, struct Image *img){
  byte NthCoverByte = img->gray[N];//replaces GetGray(i)
  if(check != getBit(NthCoverByte, 0)) //if check != last bit NthCoverByte
    img->gray[N] = toggleBit(NthCoverByte, 0); //sets Nth Gray with its last bit toggled
}

//Takes in an int from 0-9, aka 0000 - 1001
//Used to embed G number digits. Gets 4 bits of 'num' and calls embed() with each
void embedGDigit(int num, int N, struct Image *img){
  int i = 0;
  while(i < 4){//4 bits per G number digit
    byte bit = getBitFromInt(num, i);
    embed(bit, N, img);
    N++;//to embed into the next spot in the cover
    i++;
  }
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