#------------------
# PART 1 TEST CASES
#------------------
# Test 1: Basic commands: NEW, VIEW, TYPE, REMOVE (Positive test case)
# Test 2: New lists of different sizes (Positive test case)
# Test 3: Plenty of lists (Positive test case)
# Test 4: New lists of different types (Positive test case)
# Test 5: New lists with different numbers of spaces (Positive test case)
# Test 6: New lists with scientific notation (Positive test case)
# Test 7: New lists with invalid types (Negative test case)
# Test 8: Invalid NEW commands (Negative test case)
# Test 9: Invalid VIEW commands (Negative test case)
# Test 10: Invalid TYPE commands (Negative test case)
# Test 11: Invalid REMOVE commands (Negative test case)
#------------------
# PART 2 TEST CASES
#------------------
# Test 1: Basic INSERT commands (Positive test case)
# Test 2: Basic DELETE commands (Positive test case)
# Test 3: Basic INSERT + DELETE commands (Positive test case)
# Test 4: Invalid INSERT commands (Negative test case)
# Test 5: Invalid DELETE commands (Negative test case)
#------------------
# PART 3 TEST CASES
#------------------
# Test 1: Creating nested lists (Positive test case)
# Test 2: Nested lists with plenty of reference types (Positive test case)
# Test 3: Creating invalid nested lists - containing other nested lists (Negative test case)
# Test 4: Valid VIEW-NESTED commands (Positive test case)
# Test 5: Invalid VIEW-NESTED commands (Negative test case)
# Test 6: Inserting reference types into lists (Positive test case)
# Test 7: Inserting nested lists into nested lists (Edge test case)
# Test 8: Inserting reference type into referenced lists (Edge test case)
# Test 9: Deleting reference types from lists (Positive test case)
# Test 10: Trying to remove referenced lists (Edge test case)

make > /dev/null

# Indicates whether a test has passed based on diff exit value
status () {
    if [ $(echo $?) -eq 0 ]; then
        echo "Test $count: Test Passed!"
    else
        echo "Test $count: Test Failed..."
    fi

    count=$((count+=1))
}

# Keep track of the which tests are running
part=0

# Loop over the tests in each folder in ./tests
for i in $(seq 3);
do
    count=1
    part=$((part + 1))

    echo "---------------------"
    echo "| Part ${part} Test Cases |"
    echo "---------------------"

    #Total .in files = total files / 2
    x=$(ls tests/part$part | wc -l)
    y=2
    z=$(printf "%.0f" $(($x/$y)))

    # Run test cases for the current part
    for i in $(seq $z);
    do
        ./mtll < tests/part$part/test$count.in | diff - tests/part$part/test$count.expected
        status
    done
done