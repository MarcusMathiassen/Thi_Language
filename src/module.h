#ifndef MODULE_H
#define MODULE_H

#include "list.h"
#include "string.h"

typedef struct Module Module;

struct Module
{
    string name;
};

// module name: main
// module loaded modules: includes/foo

#endif
