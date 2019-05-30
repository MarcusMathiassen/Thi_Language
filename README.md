# How to compile
Run the build script in the root folder.
There are no dependencies. Not yet tested on Windows, but should compile fine on latest macOS.

## Program:
        extern puts (u8*)  s32
        main()
            defer
                puts("4th")
            defer
                puts("3rd")
            defer puts("2nd")
            puts("1st")
            
## Output:

        1st
        2nd
        3rd
        4th

More examples can be found in the 'tests' folder.
