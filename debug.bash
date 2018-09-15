#!/usr/bin/env bash

RED='\033[0;31m'
GREEN='\033[0;32m'
GRAY='\033[1;30m'
YELLOW='\033[1;31m'
NC='\033[0m' # No Color


test_file=$@
if [ ${#test_file} == 0 ]; then
    echo -e "${GRAY} [DEBUG] ${NC}${RED}MISSING INPUT FILE${NC}"
fi

thi_executable=./bin/thi


# Compile the file
name="./test_0"
thi $test_file ./test_0
if [ -f $name ]; then
    # Debug it
    lldb $name
    res=$?
else
    echo -e "${GRAY} [DEBUG] ${NC}${RED}DID NOT COMPILE. ${NC}"
fi

if [ $res == 1 ]; then
    ((num_tests_passed++))
    echo -e "${GRAY} [DEBUG] ${NC}${GREEN} PASSED ${NC} ${GRAY} '$tf' ${NC} "
else
    failed_test_files+=" $tf"
    echo -e "${GRAY} [DEBUG] ${NC}${RED} FAILED ${NC} ${GRAY} '$tf' ${NC} "
    echo -e "${GRAY} [DEBUG] returned $res ${NC}"
fi

rm $name

echo -e "${GRAY} [DEBUG] ${NC}${GRAY}FINISHED. RETURNED: ${res} ${NC}"

