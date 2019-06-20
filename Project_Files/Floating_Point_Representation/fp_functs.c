
//Andrew Morato
//CS 367

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fp.h"

//preprocessor directives
#define EXP_LENGTH 6
#define FRAC_LENGTH 8
#define BIAS 31

//other function headers
int denormal(int);
int special(int);

/*Main (tester)*/
int main(int argc, char const *argv[])
{
	/* currently just a filler*/
	return 0;
}

  /*computes our 15-bit representation by getting the closest 2^x to 
  val as possible. Then, represents val by picking which 1.frac bits
  (1, 0.5, 0.25, etc) * 2^x add up to val (or a close approximation)
  Then, x = E, and the chosen frac bits are shifted into our 15-bit*/
  int computeFP(float val){
  	if(val > 4286578688)
  		return -1; //overflow
  	if(val < 0.000000000931323)
  		return 0; //underflow
  	int frac = 0, i = 0, E = 0;
  	//calculates the E's value
  	if(val >= 1)
  		for(i = 0; val >= pow(2, i); i++){}
  	else
  		for(i = 0; val * 2 < pow(2, i); i--){}
  	E = (i - 1);
  	float rest = val - pow(2, E);
  	//calculates frac's value
  	for(i = 1; i < FRAC_LENGTH + 1; i++){
  		frac = (frac << 0x1 | (pow(2, E - i) <= rest));
  		if((pow(2, E - i) <= rest))
  			rest = rest - pow(2, E - i);
  	}
  	return (((E + BIAS) << 8) | frac);
  }

  /*does the exact same thing as computeFP but backwards. Simply,
  multipies 2^(exp - BIAS) with 1.frac and returns the sum*/
  float getFP(int val){
  	if(denormal(val))
  		return 0;
  	if(special(val))
  		return -1;
  	int frac = (val & 0xFF);
  	int E = (((val >> 8) & 0x3F) - BIAS);
  	float sum = pow(2, E);
  	//calculates the value represented by frac
  	for(int i = 1; i < FRAC_LENGTH + 1; i++)
  		if(frac & (0x1 << (FRAC_LENGTH - i)))
  			sum += pow(2, E - i);	
  	return sum;
  }

  /*multiplies both 15-bit representations as you would multiply two
  large numbers by hand. Adds the properly shifted products and returns
  the sum*/
  int multVals(int source1, int source2){
  	if(denormal(source2) | denormal(source2))
  		return 0;
  	if(special(source1) | special(source2))
  		return -1;
  	int exp = (((source1 >> 8) - BIAS) + ((source2 >> 8) - BIAS) + BIAS);
  	int frac = 0;
  	//gets the M of source1 and source2
  	int s1m = (source1 | 0x100) & 0x1FF;
  	int s2m = (source2 | 0x100) & 0x1FF;
  	//multiplies both M's
  	for(int i = 0; i < FRAC_LENGTH + 1; i++)
  		if((s1m >> i) & 0x1)
  			frac += s2m << i;
  	//checks if division by two is needed
  	if(frac >> 17 & 0x1){
  		frac = frac >> 1;
  		exp++;
  	}
  	if(((((source1 >> 8) & 0x3F) < BIAS) & (((source1 >> 8) & 0x3F) < BIAS)) & exp == 0x0)
  		return 0; //checks for underflow
  	if(exp == 0x0)
  		return -1; //checks for overflow
  	return ((exp << 8) | ((frac >> 8) & 0xFF));
  }

  /*Puts both 1.frac's into into the same E base and adds them.
  If a right shift is needed (M >= 2), it adds 1 to E & right
  shifts frac. Then, the right 8 bits are set to frac, and the
  greater E (or E + 1 if M >= 2) is returned*/
  int addVals(int source1, int source2){
  	if(denormal(source2) | denormal(source2))
  		return 0;
  	if(special(source1) | special(source2))
  		return -1;
  	//sets a bunch of necesary variables to facilitate calculations
  	int E = 0, diff = 0, large = 0, small = 0;
  	if(((source1 >> 8) & 0x3F) >= ((source2 >> 8) & 0x3F)){
	  	E = ((source1 >> 8) & 0x3F) - BIAS;
	  	diff = E - (((source2 >> 8) & 0x3F) - BIAS);
	  	large = source1;
	  	small = source2;
	  	if(diff > 8)
	  		return source1;
  	}
	else{
		E = ((source2 >> 8) & 0x3F) - BIAS;
		diff = E - (((source1 >> 8) & 0x3F) - BIAS);
	  	large = source2;
	  	small = source1;
		if(diff > 8)
			return source2;
	}
	//puts both frac's in the same exp base and adds them
  	int frac = ((((large & 0xFF) | 0x100) << diff) + ((small & 0xFF) | 0x100)) >> diff;
  	//checks if division by two is needed
  	if(frac & 0x200){
  		E++;
  		frac = frac >> 1;
  	}  	
  	if((E + BIAS) < ((large >> 8) & 0x3F))
  		return -1; //checks for overflow
  	return (((E + BIAS) << 8) | (frac & 0xFF));
  }

  //are called to check for denormalized or special cases
  int denormal(int val) {return (((val >> 8) & 0x3F) == 0x0);}
  int special(int val) {return ((val & 0x3F00) == 0x3F00);}