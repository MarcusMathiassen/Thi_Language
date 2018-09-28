#!/usr/bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
GRAY='\033[1;30m'
YELLOW='\033[1;31m'
NC='\033[0m' # No Color


test_files=$@
if [ ${#test_files} == 0 ]; then
    test_files="./tests/*.thi"
fi
failed_test_files=()
passing_test_files=()

num_tests_total=${#test_files[@]}
num_tests_passed=0
num_tests_compiled_successfully=0
test_counter=0

thi_executable=./thi

for tf in $test_files
do
    name="test_${test_counter}"
    ((test_counter++))

    # Compile the file
    $thi_executable $tf $name

    if [ -f $name ]; then
        # Run it
        ./${name}
        res=$?
        ((num_tests_compiled_successfully++))
        passing_test_files+=" $tf"
    else
        failed_test_files+=" $tf"
        echo -e "${GRAY}[TEST] ${NC}${RED}DID NOT COMPILE. ${NC}"
        continue
    fi

    if [ $res == 1 ]; then
        ((num_tests_passed++))
        echo -e "${GRAY}[TEST] ${NC}${GREEN} PASSED ${NC} ${GRAY} $tf ${NC} "
    else
        failed_test_files+=" $tf"
        echo -e "${GRAY}[TEST] ${NC}${RED} FAILED ${NC} ${GRAY} $tf ${NC} "
        echo -e "${GRAY}[TEST] returned $res ${NC}"
    fi
    
    rm $name
done

echo -e "${GRAY}[TEST] ${NC}${GRAY}PASSED/COMPILED/TOTAL: ${num_tests_passed}/${num_tests_compiled_successfully}/${test_counter} ${NC}"

if [ ${#failed_test_files[@]} -ne 0 ]; then
    echo -e "${GRAY}[TEST] ${NC}${GREEN}PASSED TESTS: ${passing_test_files} ${NC}"
    echo -e "${GRAY}[TEST] ${NC}${RED}FAILED TESTS: ${failed_test_files} ${NC}"
else
    echo -e "${GRAY}[TEST] ${NC}${GREEN}ALL TESTS PASSED${NC}"
fi


