# Thi_Language

# Syntax

    ':' definition operator.
    '::' constant definition operator.
    '=' assignment operator.
    ':=' type inferred assignment operator.

    '+' plus
    '-' minus
    '/' division
    '*' multiplication
    '%' modulo
    '!' not
    '&' or
    '&&' logical or
    '!!' logical not
    '|' AND
    '||' logical and

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
- [-] string literals
- [x] functions
- [x] function calls
- [ ] multiple compilation files using 'load'
- [x] C stdlib interop

- [x] variables
- [ ] global variables

- [x] i32
- [ ] char
- [ ] pointer

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
