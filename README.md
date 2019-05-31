## What it is
    A C like programming language written in C99.
    Featuring python like syntax with optional braces and semicolons.

## How to compile
Run a build script in the root folder:
    build_dev or build_prod

## Dependencies (these are not 'min' versions, just the versions i use)
Assembler: nasm (version 2.14.02)
Linker: ld (BUILD 18:45:16 Apr  4 2019)

Tested on macOS. Unsure about Windows.

## Usage
    
        thi source.thi

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
