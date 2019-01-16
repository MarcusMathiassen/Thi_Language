# Thi_Language

# Basic example
        touch main.thi

        # this is a comment
        def main () -> s32
            ret 1

        $ thi main.thi
# Syntax
    Constant variable def <=>        <ident> :: <expr>
    Function def <=> def <ident> (<variable_decl>..) -> <ret_type>
    Variable decl <=>                <ident> : <type>
    Variable decl with assignmen <=> <ident> : <type> = <expr>
    Variable decl type inf <=>       <ident> := <expr>

    Control flow
    if <=> if <cond>
    for <=> for <init>, <cond>, <inc>
    while <=> while <cond>

# Not actually a readme but a todo scratchpad
- [x] integer literals
- [ ] string literals
- [x] functions
- [x] function calls
- [x] multiple compilation files using 'load'
- [x] C stdlib interop

- [x] variables
- [ ] global variables

- [x] s32
- [x] char
- [x] pointer

- [x] subscript access

- [ ] structs
- [ ] field access

- [ ] enums

- [x] if
- [x] for
- [x] do
- [x] defer
- [x] while
- [x] break
- [x] return
- [x] continue

- [ ] switch
- [ ] case
