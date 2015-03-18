#!/bin/bash
FAIL=0

make clean
make clean_list_test
make customer_list_setup
make customer_list
if [ $? -ne 0 ]; then
  echo "**** Make (compilation) FAILED"
  FAIL=1
fi

runtest() {
  NAME=$1
  EXPRETVAL=$2
  INFILE=$3
  EXPFILE=$4
  AOUTFILE=$5
  ARGFILE=$6
  EXPARGFILE=$7
  STARTFILE=$8
  LOCALFAIL=0
  rm -f $AOUTFILE
  cp $NAME/$STARTFILE $NAME/$ARGFILE
  ./customer_list $NAME/$ARGFILE < $NAME/$INFILE > $NAME/$AOUTFILE
  RETVAL=$?

  if [ $RETVAL -ne $EXPRETVAL ]; then
    echo "Test $NAME FAILED - RETVAL wrong. Expected: $EXPRETVAL Actual: $RETVAL"
    FAIL=1
    LOCALFAIL=1
  fi
  DIFFDATA=`diff -q $NAME/$EXPFILE $NAME/$AOUTFILE`
  if [ $? -ne 0 ]; then
    echo "Test $NAME FAILED - diff of console output did not match: $DIFFDATA"
    FAIL=1
    LOCALFAIL=1
  fi
  DIFFDATA=`diff -q $NAME/$EXPARGFILE $NAME/$ARGFILE`
  if [ $? -ne 0 ]; then
    echo "Test $NAME FAILED - diff of customer file did not match: $DIFFDATA"
    FAIL=1
    LOCALFAIL=2
  fi
  if [ $LOCALFAIL -eq 0 ]; then
    echo "Test $NAME PASS"
  fi

  #Valgrind for the test
  rm -f $AOUTFILE
  cp $NAME/$STARTFILE $NAME/$ARGFILE
  valgrind --leak-check=yes --error-exitcode=1 ./customer_list $NAME/$ARGFILE < $NAME/$INFILE > $NAME/$AOUTFILE
  RETVAL=$?
  LOCALFAIL=0
  if [ $RETVAL -ne 0 ]; then
    echo "Test $NAME VALGRIND FAILED - RETVAL wrong. Expected: 0 Actual: $RETVAL"
    FAIL=1
    LOCALFAIL=1
  fi
  if [ $LOCALFAIL -eq 0 ]; then
    echo "Test $NAME VALGRIND PASS"
  fi

}

runtest test_01 0 in_01 expected_01 actual_01 customers_01.txt exp_customers_01.txt start_customers_01.txt
runtest test_02 0 in_02 expected_02 actual_02 customers_02.txt exp_customers_02.txt start_customers_02.txt
runtest test_03 0 in_03 expected_03 actual_03 customers_03.txt exp_customers_03.txt start_customers_03.txt
runtest test_04 0 in_04 expected_04 actual_04 customers_04.txt exp_customers_04.txt start_customers_04.txt
runtest test_05 0 in_05 expected_05 actual_05 customers_05.txt exp_customers_05.txt start_customers_05.txt
runtest test_06 0 in_06 expected_06 actual_06 customers_06.txt exp_customers_06.txt start_customers_06.txt
runtest test_07 0 in_07 expected_07 actual_07 customers_07.txt exp_customers_07.txt start_customers_07.txt
runtest test_08 0 in_08 expected_08 actual_08 customers_08.txt exp_customers_08.txt start_customers_08.txt
runtest test_09 0 in_09 expected_09 actual_09 customers_09.txt exp_customers_09.txt start_customers_09.txt
runtest test_10 0 in_10 expected_10 actual_10 customers_10.txt exp_customers_10.txt start_customers_10.txt
runtest test_11 0 in_11 expected_11 actual_11 customers_11.txt exp_customers_11.txt start_customers_11.txt
runtest test_12 0 in_12 expected_12 actual_12 customers_12.txt exp_customers_12.txt start_customers_12.txt
runtest test_13 0 in_13 expected_13 actual_13 customers_13.txt exp_customers_13.txt start_customers_13.txt

#test_14 - no command line argument
  ./customer_list > test_14/actual_14
  RETVAL=$?
  LOCALFAIL=0
  if [ $RETVAL -ne 1 ]; then
    echo "Test test_14 FAILED - RETVAL wrong. Expected: 1 Actual: $RETVAL"
    FAIL=1
    LOCALFAIL=1
  fi
  DIFFDATA=`diff -q test_14/expected_14 test_14/actual_14`
  if [ $? -ne 0 ]; then
    echo "Test test_14 FAILED - diff of console output did not match: $DIFFDATA"
    FAIL=1
    LOCALFAIL=1
  fi
  if [ $LOCALFAIL -eq 0 ]; then
    echo "Test test_14 PASS"
  fi
  #Valgrind for the test
  rm -f test_14/actual_14
  valgrind --leak-check=yes --error-exitcode=1 ./customer_list > test_14/actual_14
  RETVAL=$?
  LOCALFAIL=0
  if [ $RETVAL -ne 1 ]; then
    echo "Test test_14 VALGRIND FAILED - RETVAL wrong. Expected: 1 Actual: $RETVAL"
    FAIL=1
    LOCALFAIL=1
  fi
  if [ $LOCALFAIL -eq 0 ]; then
    echo "Test test_14 VALGRIND PASS"
  fi


#test_15 - missing file
  ./customer_list no_file.txt > test_15/actual_15
  RETVAL=$?

  if [ $RETVAL -ne 2 ]; then
    echo "Test test_15 FAILED - RETVAL wrong. Expected: 2 Actual: $RETVAL"
    FAIL=1
    LOCALFAIL=1
  fi
  DIFFDATA=`diff -q test_15/expected_15 test_15/actual_15`
  if [ $? -ne 0 ]; then
    echo "Test test_15 FAILED - diff of console output did not match: $DIFFDATA"
    FAIL=1
    LOCALFAIL=1
  fi
  if [ $LOCALFAIL -eq 0 ]; then
    echo "Test test_15 PASS"
  fi
  #Valgrind for the test
  rm -f test_15/actual_15
  valgrind --leak-check=yes --error-exitcode=1 ./customer_list no_file.txt > test_15/actual_15
  RETVAL=$?
  LOCALFAIL=0
  if [ $RETVAL -ne 2 ]; then
    echo "Test test_15 VALGRIND FAILED - RETVAL wrong. Expected: 2 Actual: $RETVAL"
    FAIL=1
    LOCALFAIL=1
  fi
  if [ $LOCALFAIL -eq 0 ]; then
    echo "Test test_15 VALGRIND PASS"
  fi

#Final check for fails
  if [ $FAIL -eq 0 ]; then
    exit 0
  else 
    echo "FAILING TESTS!"
    exit 13
  fi
