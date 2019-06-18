/*-----------------
Andrew Morato
November 21, 2017
CS 367 Project 3
-----------------*/

//Preprocessor Directives
#include <stdio.h>
#include <stdlib.h>
#include "memory_system.h"

//Macro's
#define VPN_TAG 5
#define PPN_TAG 13
#define VPN_INDEX 4
#define PPN_INDEX 5
#define PPN_BLK 2

//Function Headers
int updateFirst(int, int, int);
int updateSecond(int, int, int);

//Data Fields
int TLB[16] = {0}; //32b = JUNK-VALID-TAG(VPN)-PPN
/*int[4] = [TIME-V-TAG(PPN)1][DATA1][TIME-V-TAG(PPN)2][DATA2]*/
int cache[32][4] = {0};


void initialize() { }


/*Decomposes the virtual address, updates the TLB if necessary,
and returns the properly formatted physical address*/
int get_physical_address(int virt_address) {
    if(((virt_address >> 18) & 0x3FFF) != 0) {
        log_entry(ILLEGALVIRTUAL, virt_address);
        return -1;
    }
    //decomposes the VA
    int PA = (virt_address & 0x1FF);
    int index = ((virt_address >> OFFSET_BITS) & 0xF);
    int tag = ((virt_address >> (OFFSET_BITS + VPN_INDEX)) & 0x1F);
    int log = ADDRESS_FROM_TLB; 
    //checks valid bit & tag, updates the TLB
    if ((!((TLB[index] >> (VPN_TAG + PPN_BITS))) & 0x1) ||
       (((TLB[index] >> PPN_BITS) & 0x1f) != tag)) {
        TLB[index] = 0x10000 | (tag << PPN_BITS) 
        | get_page_table_entry((tag << VPN_INDEX) | index);
        log = ADDRESS_FROM_PAGETABLE;
      }
    //packs the physical address
    PA = ((((TLB[index] & 0x7FF) << OFFSET_BITS) | PA) & 0xFFFFF);
    log_entry(log, PA);
    return PA;
}

/*Decomposes the PA, launches control flow to properly handle all
cases, uses helpers to update the cache, and returns the byte*/
char get_byte(int phys_address) {
    //decomposes the PA
    int blk = (phys_address & 0x3);
    int index = ((phys_address >> PPN_BLK) & 0x1F);
    int tag = ((phys_address >> (PPN_BLK + PPN_INDEX)) & 0x1FFF);
    int word = get_word(phys_address); int log = DATA_FROM_MEMORY;
    int cache_index = 0; int valid = 0;
    if ((cache[index][0] >> PPN_TAG) & 0x1) { //first set, if valid
        //if first tag is equal
        if ((cache[index][0] & 0x1FFF) == tag) { 
            cache_index = 1; 
            log = DATA_FROM_CACHE;
        }
        else { valid = 1; }
    }
    if(!cache_index){ //if the first tag matched
    if ((cache[index][2] >> PPN_TAG) & 0x1) { //second set, if valid
        //if second tag is equal
        if ((cache[index][2] & 0x1FFF) == tag) { 
            cache_index = 3;
            log = DATA_FROM_CACHE;
        }
        //1st tag doesn't match or is invalid & 2nd tag doesn't match
        else {
            if(valid){ //both valid, tag's dont match -> place oldest
                if (!(cache[index][0] & 0x4000)){ //first is older
                    cache_index = updateFirst(index, word, tag); }
                else {cache_index = updateSecond(index, word, tag);}
            }
             //first is invalid, second is valid -> place in first
            else {cache_index = updateFirst(index, word, tag); }
        }
    }
    else { //second is invalid
        //first is valid, second is not -> place in second
        if (valid) {cache_index = updateSecond(index, word, tag); }
        //both are invalid -> place in first
        else {cache_index = updateFirst(index, word, tag); }
    }}
    unsigned char byte = ((cache[index][cache_index] >> (blk * 8)) & 0xFF);
    log_entry(log, byte);
    return byte;
}

/*updates the first value at index in the cache (time as well)*/
int updateFirst(int index, int word, int tag){
    cache[index][1] = word;
    cache[index][0] = (0x6000 | (tag & 0x1FFF));
    cache[index][2] = (cache[index][2] & 0x3FFF);
    return 1;
}

/*updates the second value at index in the cache (time as well)*/
int updateSecond(int index, int word, int tag){
    cache[index][3] = word;
    cache[index][0] = (cache[index][0] & 0x3FFF);
    cache[index][2] = (0x6000 | (tag & 0x1FFF));
    return 3;
}