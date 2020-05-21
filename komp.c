#include <stdio.h>

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

typedef struct inst_template {
    enum instructions type;
    int address;
    int stack_num;
}inst_t;

int main() {
    int proc_ad[26] = {0};

    return 0;
}