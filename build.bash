#!/usr/bin/env bash
output=./bin/thi
compiler=clang
olvl=
src=./src/*.c
flags=-Wall\ -Wextra\ -Wimplicit-fallthrough
std=c99

$compiler $src -std=$std $olvl $flags -o $output

if [ $? == 0 ]; then
    # ./test.bash ./tests/test_basic_setup.thi ./tests/test_hex.thi ./tests/test_logical_and.thi ./tests/test_logical_or.thi ./tests/test_not.thi ./tests/test_math.thi ./tests/test_math_plus.thi ./tests/test_math_minus.thi ./tests/test_math_div.thi ./tests/test_math_mul.thi
    ./test.bash ./tests/test_math.thi
    # ./test.bash ./tests/test_basic_setup.thi
    # ./test.bash ./tests/test_ternary.thi
    # ./test.bash ./tests/test_hex.thi
    # ./test.bash ./tests/test_macro_definition.thi
    # ./test.bash ./tests/test_asterisk_equal.thi
    # ./test.bash ./tests/test_logical_or.thi
    # ./test.bash ./tests/test_for.thi
    # ./test.bash ./tests/test_ternary.thi
    # ./test.bash ./tests/test_function_call.thi
    # ./test.bash ./tests/test_float.thi
    # ./test.bash ./tests/test_struct.thi
    # ./test.bash  ./tests/test_variable_assignment.thi
    # ./test.bash  ./tests/test_foreign.thi
    # ./test.bash ./tests/test_anonymous_scope.thi ./tests/test_asterisk_equal.thi ./tests/test_basic_setup.thi ./tests/test_equal_to.thi ./tests/test_foreign.thi ./tests/test_function_call.thi ./tests/test_function_call_2.thi ./tests/test_fwslash_equal.thi ./tests/test_greater.thi ./tests/test_hex.thi ./tests/test_hex_2.thi ./tests/test_hex_3.thi ./tests/test_less.thi ./tests/test_load.thi ./tests/test_load_2.thi ./tests/test_logical_and.thi ./tests/test_logical_or.thi ./tests/test_math.thi ./tests/test_math_div.thi ./tests/test_math_minus.thi ./tests/test_math_mul.thi ./tests/test_math_plus.thi ./tests/test_minus_equal.thi ./tests/test_neg.thi ./tests/test_not.thi ./tests/test_not_equal_to.thi ./tests/test_order_independance.thi ./tests/test_plus_equal.thi ./tests/test_variable.thi ./tests/test_variable_assignment.thi ./tests/test_variable_declaration.thi ./tests/test_variable_typedefered.thi
fi
 
