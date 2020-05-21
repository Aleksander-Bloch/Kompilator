#include <stdio.h>

/*
VIRTUAL MACHINE COMPILER

This program translates artificial programming
language into virtual machine code.

Author: Aleksander Bloch <alekbloch21@gmail.com>
Last modified: 21-05-2000
*/

// Fixed values added to names of instructions
enum instructions {
    PUSH_0,
    PUSH_1,
    OUTPUT_0,
    OUTPUT_1,
    POP_BRANCH,
    INPUT_BRANCH,
    JUMP,
    CALL,
    RETURN,
    HALT
};

// Basic template for instruction with 0,1,2 args
typedef struct inst_template {
    enum instructions type;
    int address;
    int stack_num;
}inst_t;

/*
PRECOMPILATION
- creates array of instructions
- fills proc_ad
- creates array of addresses for choice instructions
- returns array of pointers (inst and choice_add array)
*/
inst_t** precompile(int* inst_num, int* choice_add_num,
int* proc_ad, int* main_ad);

int main() {
    // lists addresses of procedures named A-Z
    int proc_ad[26] = {0};
    // main procedure address
    int main_ad = -1;
    // size of inst and choice_add array on heap
    int inst_num = 0, choice_add_num = 0;

    return 0;
}

inst_t** precompile(int* inst_num, int* choice_add_num,
int* proc_ad, int* main_ad) {
    ;
}