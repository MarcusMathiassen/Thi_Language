# Thi_Language

# Basic example
        touch main.thi

        # this is a comment
        main :: () -> i32 {
            ret 1
        }

        $ thi main.thi
# Syntax
    Constant variable def <=>        <ident> :: <expr>
    Constant variable def <=>        <ident> :: (<params>) -> <ret_type> { <body> }
    Variable decl <=>                <ident> : <type>
    Variable decl with assignmen <=> <ident> : <type> = <expr>
    Variable decl type inf <=>       <ident> := <expr>

    Control flow
    if <=> if <expr> { <body> }
    for <=> for <ident>: <range> { <body> }
    while <=> while <expr> { <body> }

# Not actually a readme but a todo scratchpad

# Minumum features needed for self-hosting
- [x] integer literals
- [ ] string literals
- [x] functions
- [x] function calls
- [x] multiple compilation files using 'load'
- [x] C stdlib interop

- [x] variables
- [ ] global variables

- [x] i32
- [ ] char
- [x] pointer

- [x] subscript access

- [ ] structs
- [ ] field access

- [ ] enums

- [x] if
- [x] for
- [x] while
- [x] break
- [x] return
- [x] continue

- [ ] switch
- [ ] case
