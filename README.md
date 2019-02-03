# Thi_Language

# Basic example
        touch main.thi

        # this is a comment
        main()
            return 1

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
- [x] string literals
- [x] char literals
- [x] functions
- [x] function calls
- [x] multiple compilation files using 'load'
- [x] C stdlib interop

- [x] 'link' for adding linking options for the linker

- [x] variables

- [x] s32
- [x] char
- [x] pointer

- [ ] subscript access

- [x] structs
- [ ] field access

- [x] enums

- [x] if
- [x] for
- [x] 'do' lets you do single line statements 
- [x] defer
- [x] while
- [x] break
- [x] return
- [x] continue

- [x] switch
- [x] case
