#ifndef MODULE_H
#define MODULE_H

#include "string.h"
#include "list.h"

typedef struct Module Module;

struct Module {
    string name;
};

// module name: main
// module loaded modules: includes/foo

#endif
