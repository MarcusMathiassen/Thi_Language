#!/usr/bin/env bash
output=./thi
olvl=
compiler=clang
src=./src/*.c
flags=-Wall\ -Wextra\ -Wno-unused-function\ -Wno-unused-parameter\ -Werror #\ -DNDEBUG
std=c99
$compiler $src -std=$std $olvl $flags -o $output # -g -fsanitize=address -fno-omit-frame-pointer
if [ $? == 0 ]; then
    test_files=$@
    if [ ${#test_files} == 0 ]; then
        test_files="./tests/*.thi"
    fi
    if [ $? == 0 ]; then
        # ./test.bash
        # ./test.bash ./tests/test_window.thi
        ./test.bash ./tests/test_basic_setup.thi
        # ./test.bash ./tests/test_global_variable.thi
    fi
fi
