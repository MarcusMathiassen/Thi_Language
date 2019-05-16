#!/usr/bin/env bash
output=./thi
olvl=
compiler=clang
src=./src/*.c
flags=-Wall\ -Wextra\ -Wno-unused-function # -Werror #-DNDEBUG
std=c99

$compiler $src -std=$std $olvl $flags -o $output # -g -fsanitize=address -fno-omit-frame-pointer

test_files=$@
if [ ${#test_files} == 0 ]; then
    test_files="./tests/*.thi"
fi

if [ $? == 0 ]; then
    # ./test.bash
    # ./test.bash ./tests/test_order_independance.thi
    # ./test.bash ./tests/test_pointer.thi
    # ./test.bash ./tests/test_var_args.thi
    # ./test.bash ./ tests/test_basic_setup.thi
    # ./test.bash ./ tests/test_typeof.thi
    # ./test.bash ./ tests/test_sizeof.thi
    # ./test.bash ./ tests/test_sizeof_2.thi
    # ./test.bash ./tests/test_window.thi
    # ./test.bash ./tests/test_advanced_pointer.thi
    # ./test.bash ./tests/test_subscript_access.thi
    ./test.bash ./tests/test_subscript_access_2.thi
    # ./test.bash ./tests/test_char.thi
    # ./test.bash ./tests/test_file.thi
    # ./test.bash ./tests/test_field_access.thi
    # ./test.bash ./tests/test_struct.thi
    # ./test.bash ./tests/test_array.thi
    # ./test.bash ./tests/test_struct_2.thi
    # ./test.bash ./tests/test_basic_array.thi
    # ./test.bash ./tests/test_pointer.thi
    # ./test.bash ./tests/test_var_args.thi
    # ./test.bash ./tests/test_advanced_pointer.thi
    # ./test.bash ./tests/test_function_call_struct.thi
    # ./test.bash ./tests/test_window.thi
    # ./test.bash ./tests/test_math.thi
    # ./test.bash ./tests/test_switch.thi
    # ./test.bash ./tests/test_no_main.thi
    # ./test.bash ./tests/test_ternary.thi
    # ./test.bash ./tests/test_float_GT.thi
    # ./test.bash ./tests/test_float.thi
    # ./test.bash ./tests/test_single_line_statements.thi
    # ./test.bash ./tests/test_basic_setup.thi
    # ./test.bash ./tests/test_load.thi
    # ./test.bash ./tests/test_function_call.thi
    # ./test.bash ./tests/test_function_call_2.thi
    # ./test.bash ./tests/test_macro_definition.thi
    # ./test.bash ./tests/test_order_independance.thi
    # ./test.bash ./tests/test_new.thi
    # ./test.bash ./tests/test_defer.thi
    # ./test.bash ./tests/test_continue.thi
    # ./test.bash ./tests/test_if.thi
    # ./test.bash ./tests/test_comments.thi
    # ./test.bash ./tests/test_field_access.thi
    # ./test.bash ./tests/test_struct.thi
    # ./test.bash ./tests/test_struct_2.thi
    #./test.bash ./tests/test_sizeof.thi
    # ./test.bash ./tests/test_array.thi
    # ./test.bash ./tests/test_tilde.thi
    # ./test.bash ./tests/test_ternary.thi
    # ./test.bash ./tests/test_for.thi
    # ./test.bash ./tests/test_pointer.thi
    # ./test.bash ./tests/test_basic_ternary.thi
    # ./test.bash ./tests/test_ternary.thi
    # ./test.bash ./tests/test_math_plus.thi
    # ./test.bash ./tests/test_plus_equal.thi
    # ./test.bash ./tests/test_math.thi
    # ./test.bash ./tests/test_window.thi
    # ./test.bash ./tests/test_basic_setup.thi
    # ./test.bash ./tests/test_char.thi
    # ./test.bash ./tests/test_defer.thi
    # ./test.bash ./tests/test_anonymous_scope.thi
    # ./test.bash ./tests/test_tilde.thi
    # ./test.bash ./tests/test_bitwise_leftshift.thi
    # ./test.bash ./tests/test_bitwise_rightshift.thi
    # ./test.bash ./tests/test_or.thi
    # ./test.bash ./tests/test_logical_or.thi
    # ./test.bash ./tests/test_logical_and.thi
    # ./test.bash ./tests/test_percent.thi
    # ./test.bash ./tests/test_percent_eq.thi
    # ./test.bash ./tests/test_not_equal_to.thi
    # ./test.bash ./tests/test_not_equal_to.thi
    # ./test.bash ./tests/test_using_unnamed_parameter.thi
    # ./test.bash ./tests/test_function_call.thi
    # ./test.bash ./tests/test_array.thi
    # ./test.bash ./tests/test_array_2d.thi
    # ./test.bash ./tests/test_variable_assignment.thi
    # ./test.bash ./tests/test_variable_declaration.thi
    # ./test.bash ./tests/test_variable_typedefered.thi
    # ./test.bash ./tests/test_variable.thi
    # ./test.bash ./tests/test_asterisk_equal.thi
    # ./test.bash ./tests/test_basic_setup.thi
    # ./test.bash ./tests/test_basic_array.thi
    # ./test.bash ./tests/test_for.thi
    # ./test.bash ./tests/test_if.thi
    # ./test.bash ./tests/test_while.thi
    # ./test.bash ./tests/test_break.thi
    # ./test.bash ./tests/test_continue.thi
    # ./test.bash ./tests/test_hex.thi
    # ./test.bash ./tests/test_anonymous_scope.thi
    # ./test.bash ./tests/test_hex_2.thi
    # ./test.bash ./tests/test_unary.thi
    # ./test.bash ./tests/test_foreign.thi
    # ./test.bash ./tests/test_triangle.thi
    # ./test.bash ./tests/test_field_access.thi
    # ./test.bash ./tests/test_load.thi
    # ./test.bash ./tests/test_load_2.thi
    # ./test.bash ./tests/test_foreign.thi
    # ./test.bash ./tests/test_stdlib.thi
    # ./test.bash ./tests/test_stdlib_2.thi
    # ./test.bash ./tests/test_string.thi
    # ./test.bash ./tests/test_break.thi
    # ./test.bash ./tests/test_continue.thi
    # ./test.bash ./tests/test_math.thi
    # ./test.bash ./tests/test_math_plus.thi
    # ./test.bash ./tests/test_math_minus.thi
    # ./test.bash ./tests/test_logical_and.thi
    # ./test.bash ./tests/test_array.thi
    # ./test.bash ./tests/test_basic_array.thi
    # ./test.bash ./tests/test_plus_equal.thi
    # ./test.bash ./tests/test_variable_assignment.thi
    # ./test.bash ./tests/test_subscript_access.thi
    # ./test.bash ./tests/test_order_independance.thi
    # ./test.bash ./tests/test_function_call.thi
    # ./test.bash ./tests/test_function_call_2.thi
    # ./test.bash ./tests/test_pointer.thi
    # ./test.bash ./tests/test_basic_array.thi
    # ./test.bash ./tests/test_anonymous_scope.thi ./tests/test_asterisk_equal.thi ./tests/test_basic_setup.thi ./tests/test_enum.thi ./tests/test_equal_to.thi ./tests/test_function_call.thi ./tests/test_function_call_2.thi ./tests/test_fwslash_equal.thi ./tests/test_greater.thi ./tests/test_hex.thi ./tests/test_hex_2.thi ./tests/test_hex_3.thi ./tests/test_less.thi ./tests/test_logical_and.thi ./tests/test_logical_or.thi ./tests/test_math.thi ./tests/test_math_div.thi ./tests/test_math_minus.thi ./tests/test_math_mul.thi ./tests/test_math_plus.thi ./tests/test_minus_equal.thi ./tests/test_neg.thi ./tests/test_not.thi ./tests/test_not_equal_to.thi ./tests/test_order_independance.thi ./tests/test_plus_equal.thi ./tests/test_ternary.thi ./tests/test_using_unnamed_parameter.thi ./tests/test_variable.thi ./tests/test_variable_assignment.thi ./tests/test_variable_declaration.thi ./tests/test_variable_typedefered.thi ./tests/test_comments.thi
    # ./test.bash ./tests/test_math.thi
    # ./test.bash ./tests/test_fib.thi
    # ./test.bash ./tests/test_label_maker.thi
    # ./test.bash ./tests/test_using_unnamed_parameter.thi
    # ./test.bash ./tests/test_function_call_2.thi
    # ./test.bash ./tests/test_basic_setup.thi
    # ./test.bash ./tests/test_basic_ternary.thi
    # ./test.bash ./tests/test_ternary.thi
    # ./test.bash ./tests/test_function_call.thi
    # ./test.bash ./tests/test_macro_definition_3.thi
    # ./test.bash ./tests/test_macro_definition_2.thi
    # ./test.bash ./tests/test_function_call_2.thi
    # ./test.bash ./tests/test_hex.thi
    # ./test.bash ./tests/test_function_call.thi
    # ./test.bash ./tests/test_macro_definition.thi
    # ./test.bash ./tests/test_asterisk_equal.thi
    # ./test.bash ./tests/test_logical_or.clear.thi
    # ./test.bash ./tests/test_if.thi
    # ./test.bash ./tests/test_for.thi
    # ./test.bash ./tests/test_while.thi
    # ./test.bash ./tests/test_not_equal_to.thi
    # ./test.bash ./tests/test_enum.thi
    # ./test.bash ./tests/test_basic_setup.thi
    # ./test.bash ./tests/test_float.thi
    # ./test.bash ./tests/test_struct.thi
    # ./test.bash ./tests/test_struct_2.thi
    # ./test.bash ./tests/test_anonymous_scope.thi
    # ./test.bash ./tests/test_order_independance.thi
    # ./test.bash  ./tests/test_variable_assignment.thi
    # ./test.bash  ./tests/test_variable_declaration.thi
    # ./test.bash  ./tests/test_variable_typedefered.thi
    # ./test.bash  ./tests/test_foreign.thi
    # ./test.bash ./tests/test_anonymous_scope.thi ./tests/test_asterisk_equal.thi ./tests/test_basic_setup.thi ./tests/test_equal_to.thi ./tests/test_foreign.thi ./tests/test_function_call.thi ./tests/test_function_call_2.thi ./tests/test_fwslash_equal.thi ./tests/test_greater.thi ./tests/test_hex.thi ./tests/test_hex_2.thi ./tests/test_hex_3.thi ./tests/test_less.thi ./tests/test_load.thi ./tests/test_load_2.thi ./tests/test_logical_and.thi ./tests/test_logical_or.thi ./tests/test_math.thi ./tests/test_math_div.thi ./tests/test_math_minus.thi ./tests/test_math_mul.thi ./tests/test_math_plus.thi ./tests/test_minus_equal.thi ./tests/test_neg.thi ./tests/test_not.thi ./tests/test_not_equal_to.thi ./tests/test_order_independance.thi ./tests/test_plus_equal.thi ./tests/test_variable.thi ./tests/test_variable_assignment.thi ./tests/test_variable_declaration.thi ./tests/test_variable_typedefered.thi
fi

#if debug
rm ./thi
