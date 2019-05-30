
## Feature/Todo
- [ ] passing structs as parameters to function calls
- [x] dereference '*' syntax parsed correctly
- [ ] implicit upcast type conversion for suitable expressions
- [ ] make sure all basic types are handled correctly
- [ ] struct braced initilization syntax with named parameters
- [ ] support for variable arguments in in not just clibs.
- [ ] solve circular dependencies, and any other dependencies.
- [ ] remove duplicate data tables in assembly output.
- [ ] find a better way to remove unused externs
- [x] sema on AST_IDENT should print an error on undefined identifiers
- [ ] fix 'for in' it's hardcoded atm
- [x] implement pointer increment
- [ ] Remove unused load when in addresof
      ex.
        mov rax, [rbp-8]; load i from 8
        lea rax, [rbp-8]; addrsof
- [ ] X64 colored by statements and expressions.
- [ ] X64 commented by statements and expressions.
- [ ] Remove all unused nodes from the AST
- [x] in Sema, orderless decls are not implemented.
- [x] sizeof char returns 8 instead of 1. Why?  
- [x] variable 'k s64* = &i' doesnt work. But 'k = &i' does. ?
- [x] Handle optional braces
- [x] Handle optional semicolon
- [ ] Change how we parse types. We can just parse them as derefs of Identifiers. And later look
      them up in the symbol table to determine if they are type or not. This will make parsing easier.
      And let us drop 'def' as a keyword.
- [ ] Implement 'union'

## Bugs
- [ ] Typechecker shouldnt change the types of things. Only check. 
- [ ] there seems to be a bug in the ast_to_source printer. Comments get the wrong indentation after a block
- [ ] in Sema, AST_GROUPING seems to cause a crash. Removing any grouping expressions in the source removes the crash.
- [ ] GLFW_KEY_LAST :: GLFW_KEY_MENU is not handled correctly. We need to do constant propogation. Or change all constants into variables with a stack position.
