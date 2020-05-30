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

typedef struct choice_template {
    // flag for distinguishing between if and else statements
    int occurred_cond;
    int* address_else;
}choice_t;

typedef struct choice_array {
    int capacity;
    int choice_num;
    choice_t* proc_choices;
}choice_arr;

/*
PRECOMPILATION
- creates array of instructions
- fills proc_ad
- creates array of addresses for choice instructions
- returns array of pointers (inst and choice_add array)
*/
inst_t* precompile(int* inst_num, int proc_ad[], int* main_ad,
choice_arr proc_cond[], int* proc_cond_num);

int main() {
    // lists addresses of procedures named A-Z
    int proc_ad[26] = {0};
    // main procedure address
    int main_ad = 0;
    // size of inst and choice_add array on heap
    int inst_num = 0;

    // array of arrays of choices for every procedure
    choice_arr proc_cond[27] = {{0, 0, NULL}};

    // number of procedures with conditionals
    int proc_cond_num = 0;


    inst_t* inst_arr = precompile(&inst_num, proc_ad, &main_ad, proc_cond, &proc_cond_num);
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

inst_t* precompile(int* inst_num, int proc_ad[], int* main_ad,
choice_arr proc_cond[], int* proc_cond_num) {
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
    // current procedure name
    char proc_name;
    // recognition of procedure with conditionals
    int has_cond = 0;


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
            nested_depth++;
            if(nested_depth == 1 && !def) {
                *main_ad = i;
                i--;
            }
            else if(nested_depth > 1) {
                if(!has_cond) {
                    has_cond = 1;
                    (*proc_cond_num)++;
                }
                if(use_stack) {
                    inst_arr[i].type = POP_BRANCH;
                    inst_arr[i].stack = stack_num;
                }
                else {
                    inst_arr[i].type = INPUT_BRANCH;
                }
            }
            else {
                i--;
            }
            
        }
        else if(token == '}') {
            nested_depth--;
            if(nested_depth == 0) {
                has_cond = 0;
                if(def) {
                    def = 0;
                    inst_arr[i].type = RETURN;
                }
            }
            else {
                i--;
            }
        }
        else if(token >= 'A' && token <= 'Z') {
            if(nested_depth == 0) {
                proc_ad[token - 'A'] = i;
                def = 1;
                proc_name = token;
                i--;
            }
            else {
                inst_arr[i].type = CALL;
                inst_arr[i].address = proc_ad[token - 'A'];
                // TODO: add function that enlarges the capacity
                if(token == proc_name) {
                    inst_arr[++i].type = JUMP;
                }
            }
        }
    }
    *inst_num = i;
    return inst_arr;
}