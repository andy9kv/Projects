/*******************************************************************|
| Author: Andrew Morato                                             |
| C program to take in a list of the form:                          |
|                                                                   |
|   tone(<num1>, <num2>, <num3>);                                   |
|   delay(150);                                                     |
|   tone(<num1>, <num2>, <num3>);                                   |
|   ......                                                          |
|                                                                   |
| and produce an array of integers made up of num2 values. Then the |
| program exports a the list of numbers scaled a given range to a   |
| different file.These kind of lists are the very commonly made     |
| arduino music files                                               |
|*******************************************************************/

//Preprocessor Directives
#include <stdio.h>
#include <stdlib.h>
#define RANGE 90

struct node {
	int val;
	struct node *next;
};

struct node *tail;

//Function Prototypes
void append(char *, char); //writes to output file
void add(int); //appends an int to the linked list
void write_file(char *, struct node *);
struct node* read_arduino_file(char *);
struct node* inRange(struct node*, int);

/*reads a file for arduino tune music and places the frequency/pitch
values in a linked list, returning the head of that list*/
struct node* read_arduino_file(char *input_file){
	struct node *head = malloc(sizeof(struct node));
	tail = head;
	/*reads char by char, parsing the file*/
	FILE *fp;
	fp = fopen(input_file, "r");
	int c = 0, max = 0;
	while((c = fgetc(fp)) != EOF){
		char current = c;
		int num = 0;
		if(current == ','){
			num += ((int) fgetc(fp) - 48) * 100;
			num += ((int) fgetc(fp) - 48) * 10;
			num += (int) fgetc(fp) - 48;
			fgetc(fp);
			add(num);
			if(num > max) {max = num;}
		}
	}
	fclose(fp);
	head = inRange(head, max);
	return head;
}

/*exports the linked lists of numbers to a file*/
void write_file(char *output_file, struct node *head){
	if(head->next != tail){
		struct node *n = head->next;
		FILE *fp = fopen(output_file, "a");
		while(n != tail){
			fprintf(fp,"%d ", n->val);
			n = n->next;
		}
		fclose(fp);
	}
}

//appends an int to the linked list
void add(int val){
	struct node *n = malloc(sizeof(struct node));
	n->val = val;
	tail->next = n;
	tail = n; 
}

/*brings the given integer list within the range of 0 ... macro RANGE. int max
specifies the largest number in the list : (NOT a lossless process)*/
struct node* inRange(struct node *head, int max){
	int div_factor = max / RANGE;
	struct node *n = head->next;
	while(n){ 
		n->val = n->val / div_factor;
		n = n->next;
	}
	return head;
}