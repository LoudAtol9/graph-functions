#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

struct stack
{
    void* info;
    struct stack* next;
};

#endif /*STACK_h*/