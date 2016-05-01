#!/bin/bash

#
# Create the expected results file (expected_results.txt)...
#

cat > expected_results.txt <<LIMIT
24
21
0.5
355
113
error: divide by 0
1
0
error: divide by 0
1
3.14159
-7
0.666667
6.28319
6.28319
inf
123.45
123.45
3
error: bad token
1.5
2.3
0
0
1.5
error: primary expected
1
0
1.5
0
1
error: primary expected
57.2958
0
error: primary expected
1
0
error: bad token
1
0
LIMIT

#
# List of commands to test as a single command line string...
#

cmds="4*3*2;(1+2)*(3+4);1/2;x=355;y=113;p=x/z;z=0;p=x/z;x/y;-3-4;2/3;x=2*pi;x;1e240*1e240;x=y=z=123.45;y;pi=3;1.5^2.3;exp(2.3*log(1.5));sin(pi/2);atan(1)*deg;rand();atan2(1,0)"

#
# Create a file of commands, but one per line (commands.txt)
#

(
cat <<LIMIT
$cmds
LIMIT
) | sed "s/;/\n/g" >commands.txt

#
# Test 1
#

echo Test calc command line arguments...
./calc "$cmds" &> test.out
rc=$?
if [ "$rc" != "7" ]; then
	echo ./calc returned the wrong number of errors: $rc s/b 7
	exit
fi
cmp test.out expected_results.txt || (echo "tests fail, try diff test.out expected_results.txt"; exit)

#
# Test 2
#

echo Test calc standard input...
./calc &> test.out <commands.txt
rc=$?
if [ "$rc" != "7" ]; then
	echo ./calc returned the wrong number of errors: $rc s/b 7
	exit
fi
cmp test.out expected_results.txt || (echo "tests fail, try diff test.out expected_results.txt"; exit)

# 
# Test 3
#

echo Test too many calc command line arguments...
./calc one 2 three &> test.out
rc=$?
if [ "$rc" != "1" ]; then
	echo ./calc returned the wrong number of errors: $rc s/b 7
	exit
fi

# 
# Cleanup and return...
#

rm -f test.out commands.txt expected_results.txt

echo All tests passed!
