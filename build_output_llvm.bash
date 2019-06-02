#!/usr/bin/env bash
/usr/local/Cellar/llvm/8.0.0_1/bin/llc ./output.ll
flags=-lSystem #\ -lglew\ -lglfw3\ -framework\ OpenGL\ -framework\ CoreVideo\ -framework\ IOKit\ -framework\ Cocoa\ -framework\ Carbon\ -framework\ CoreFoundation
as output.s -o output.o
ld -macosx_version_min 10.14 -o output output.o -e _main $flags
./output
echo $?
# rm ./output
rm ./output.o
