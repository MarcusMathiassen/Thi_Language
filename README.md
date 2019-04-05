# How to compile
Just run the build script in the root folder.
There are no dependencies. Not yet tested on Windows but should compile file on latest macOS

# Basic example
        extern puts (u8*)  s32
        main()
            i := 0
            defer
                puts("4th")
                i = 1
            defer
                puts("3rd")
                i = 2
            defer puts("2nd")
            puts("1st")
            
Output:

        1st
        2nd
        3rd
        4th
