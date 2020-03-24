    A C-like programming language written in C99.
    Featuring indent-based scoping and optional braces and semicolons.

## It is   
- beautiful
- optional with regards to braces and semicolons
- opinionated
- borderline esoteric
- outputs x64 Assembly
- and not recommended for use

## Usage
    thi hello.thi
    # -> 'Hello'

## How to compile
    ./dev
    
## Dependencies (these are not 'min' versions, just the versions i use)

Assembler: nasm (version 2.14.02)

Linker: ld (BUILD 18:45:16 Apr  4 2019)

Tested on macOS. Unsure about Windows.

## Sample:
        # This is a comment
        extern puts (u8*)  s32
        extern printf (u8*, ...)  s32
        
        def main(argc int, argv char**) s32
            defer
                puts("4th")
            defer
                printf("%f\n", getFloat())
            defer { puts("2nd") }

            puts("1st")

        # Order of declarations do not matter.
        def getFloat() f64
            return 53.2
            
## Output:

        1st
        2nd
        53.2
        4th

More examples can be found in the 'tests' folder.

## Note
This is currently a work in progress and not in any way stable.
