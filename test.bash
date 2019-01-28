#!/usr/bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
GRAY='\033[1;30m'
RGB_GRAY='\033[38;2;110;110;110;m'
RGB_WHITE='\033[38;2;190;190;190;m'
YELLOW='\033[1;31m'
NC='\033[0m' # No Color


test_files=$@
if [ ${#test_files} == 0 ]; then
    test_files="./tests/*.thi"
fi

failed_test_files=()
compiled_but_failed_test_files=()
passing_test_files=()

num_tests_total=${#test_files[@]}
num_tests_passed=0
num_tests_compiled_successfully=0
test_counter=0

thi_executable=./thi

for tf in $test_files
do
    # Compile the file
    $thi_executable -f $tf -v -d
    ((test_counter++))

    filename="${tf##*/}"                      # Strip longest match of */ from start
    name="${filename%.[^.]*}"

    if [ -f $name ]; then
        # Run it
        ./${name} "Hello" "Hei"
        res=$?
        ((num_tests_compiled_successfully++))
        passing_test_files+=" $tf"
    else
        failed_test_files+=" $tf"
        echo -e "${RGB_GRAY}[TEST] ${NC}${RED} $tf DID NOT COMPILE. ${NC}"
        continue
    fi

    if [ $res == 1 ]; then
        ((num_tests_passed++))
        echo -e "${RGB_GRAY}[TEST] ${NC}${GREEN} PASSED ${NC} ${RGB_GRAY} $tf ${NC} "
    else
        compiled_but_failed_test_files+=" $tf"
        failed_test_files+=" $tf"
        echo -e "${RGB_GRAY}[TEST] ${NC}${RED} FAILED ${NC} ${RGB_GRAY} $tf returned ${NC}${RGB_WHITE}$res ${NC}"
    fi

    rm $name
done


echo -e "${RGB_GRAY}[TEST] ${NC}${RGB_GRAY} PASSD/COMPILED/TOTAL: ${num_tests_passed}/${num_tests_compiled_successfully}/${test_counter} ${NC}"
if [ ${#failed_test_files[@]} -ne 0 ]; then
    echo -e "${RGB_GRAY}[TEST] ${NC}${GREEN} PASSED: ${passing_test_files} ${NC}"
    echo -e "${RGB_GRAY}[TEST] ${NC}${RED} FAILED: ${failed_test_files} ${NC}"
    if [ ${#compiled_but_failed_test_files[@]} -ne 0 ]; then
        echo -e "${RGB_GRAY}[TEST] ${NC}${YELLOW} COMPILED BUT FAILED: ${compiled_but_failed_test_files} ${NC}"
    fi
else
    echo -e "${RGB_GRAY}[TEST] ${NC}${GREEN} ALL TESTS PASSED${NC}"
fi
