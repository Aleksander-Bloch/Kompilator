#include <stdio.h>
#include <stdlib.h>

#define MULTIPLIER 3
#define DIVISOR 2

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
    int stack;
}inst_t;

/*
PRECOMPILATION
- creates array of instructions
- fills proc_ad
- creates array of addresses for choice instructions
- returns array of pointers (inst and choice_add array)
*/
inst_t* precompile(int* inst_num, int proc_ad[], int* main_ad);

int main() {
    // lists addresses of procedures named A-Z
    int proc_ad[26] = {0};
    // main procedure address
    int main_ad = 0;
    // size of inst and choice_add array on heap
    int inst_num = 0;
    inst_t* inst_arr = precompile(&inst_num, proc_ad, &main_ad);
    inst_arr[0].address = main_ad;
    inst_arr[inst_num].type = HALT;

    int i;
    for(i = 0; i < inst_num + 1; ++i) {
        enum instructions temp = inst_arr[i].type;
        if(temp == PUSH_0 || temp == PUSH_1) {
            printf("%d %d\n", temp, inst_arr[i].stack);
        }
        else if(temp == JUMP || temp == CALL) {
            printf("%d %d\n", temp, inst_arr[i].address);
        }
        else {
            printf("%d\n", temp);
        }
    }

    return 0;
}

inst_t* precompile(int* inst_num, int proc_ad[], int* main_ad) {
    // capacity of inst_arr
    int capacity = 1;
    // ith item, token value
    int i, token;
    // flag to indicate the source of bits
    int use_stack, stack_num;
    // depth of nested parentheses expr
    int nested_depth = 0;
    // flag to represent whether expr is part of comment
    int comment = 0;
    // whether expr is part of procedure definition
    int def = 0;


    // initializing the inst_arr with jump to main
    inst_t* inst_arr = (inst_t*)malloc(sizeof(inst_t));
    inst_arr[0].type = JUMP;

    // translating tokens into instructions in vm code
    for(i = 1; (token = getchar()) != EOF; ++i) {
        // make sure there is enough space for next inst
        if(i == capacity) {
            capacity = 1 + capacity * MULTIPLIER / DIVISOR;
            inst_arr = realloc(inst_arr, capacity * sizeof(inst_t));
        }
        if(token == ';') {
            comment = 1;
            i--;
            
        }
        else if(token == '\n') {
            comment = 0;
            i--;
        }
        else if(comment) {
            i--;
        }
        else if(token == ' ' || token == '\t') {
            i--;
        }
        // use stack mode
        else if(token >= 'a' && token <= 'z') {
            use_stack = 1;
            stack_num = token - 'a';
            i--;
        }
        // stdin mode
        else if(token == '$') {
            use_stack = 0;
            i--;
        }
        // processing tokens according to mode
        else if(token == '-') {
            if(use_stack) {
                inst_arr[i].type = PUSH_0;
                inst_arr[i].stack = stack_num;
            }
            else {
                inst_arr[i].type = OUTPUT_0;
            }
        }
        else if(token == '+') {
            if(use_stack) {
                inst_arr[i].type = PUSH_1;
                inst_arr[i].stack = stack_num;
            }
            else {
                inst_arr[i].type = OUTPUT_1;
            }
        }
        else if(token == '{') {
            if(nested_depth == 0 && !def) {
                *main_ad = i;
            }
            nested_depth++;
            i--;
        }
        else if(token == '}') {
            nested_depth--;
            if(nested_depth == 0 && def) {
                def = 0;
                inst_arr[i].type = RETURN;
            }
            else {
                i--;
            }
        }
        else if(token >= 'A' && token <= 'Z') {
            if(nested_depth == 0) {
                proc_ad[token - 'A'] = i;
                def = 1;
                i--;
            }
            else {
                inst_arr[i].type = CALL;
                inst_arr[i].address = proc_ad[token - 'A'];
            }
        }
    }
    *inst_num = i;
    return inst_arr;
}