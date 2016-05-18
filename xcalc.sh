#!/bin/bash

#############################################################################
# Initialize
#############################################################################

#
# Create the expected results for command line testing; expected_results1.txt.
#	(all commands are in one line sperated by ';')
#

cat > expected_results1.txt <<LIMIT
	24
	21
	0.5
calc: undefined variable 'z' near line 1
calc: divide by 0 near line 1
	3.14159
	-7
	0.666667
	6.28319
	inf
	123.45
	123.45
	123.45
calc: can not modify constant variable 'pi' near line 1
	3
	2.54103
	2.54103
	1
	45
	1.5708
LIMIT

#
# Create the expected results for standard input testing (expected_results1.txt)...
#	(each command sperate lines)
#

cat > expected_results2.txt <<LIMIT
	24
	21
	0.5
calc: undefined variable 'z' near line 7
calc: divide by 0 near line 9
	3.14159
	-7
	0.666667
	6.28319
	inf
	123.45
	123.45
	123.45
calc: can not modify constant variable 'pi' near line 19
	3
	2.54103
	2.54103
	1
	45
	1.5708
LIMIT

#
# List of commands to test as a single command line string...
#

cmds="4*3*2;(1+2)*(3+4);1/2;x=355;y=113;p=x/z;z=0;p=x/z;x/y;-3-4;2/3;x=2*pi;x;1e240*1e240;x=y=z=123.45;x;y;z;pi=3;1.5^2.3;exp(2.3*log(1.5));sin(pi/2);atan(1)*deg;atan2(1,0)"

#
# Create a file of commands, but one command per line (commands.txt)
#

(
cat <<LIMIT
$cmds
LIMIT
) | sed "s/;/\n/g" >commands.txt

#############################################################################
# The Tests
#############################################################################

#
# Test 1a - run the commands from a single command line argument
#

echo Test "calc expr" ...
./calc "$cmds" &> test.out
nerrors=$?
if [ "$nerrors" != "3" ]; then
	echo ./calc returned the wrong number of errors: $nerrors s/b 3
	exit
fi
cmp test.out expected_results1.txt
if [ "$?" != "0" ]; then
	echo "Test output (test.out) does not match expected (expexted_results1.txt):"
	diff test.out expected_results1.txt
	exit
fi

#
# Test 1b - run the commands from a single command line argument
#

echo Test "calc -e expr" ...
./calc -e "$cmds" &> test.out
nerrors=$?
if [ "$nerrors" != "3" ]; then
	echo ./calc returned the wrong number of errors: $nerrors s/b 3
	exit
fi
cmp test.out expected_results1.txt
if [ "$?" != "0" ]; then
	echo "Test output (test.out) does not match expected (expexted_results1.txt):"
	diff test.out expected_results1.txt
	exit
fi

#
# Test 2a - read standard input
#

echo Test calc standard input ...
./calc &> test.out <commands.txt
nerrors=$?
if [ "$nerrors" != "3" ]; then
	echo ./calc returned the wrong number of errors: $nerrors s/b 3
	exit
fi
cmp test.out expected_results2.txt
if [ "$?" != "0" ]; then
	echo "Test output (test.out) does not match expected (expexted_results.txt):"
	diff test.out expected_results2.txt
	exit
fi

#
# Test 2b - read standard input
#

echo Test "calc -f -" ...
./calc -f - &> test.out  <commands.txt
nerrors=$?
if [ "$nerrors" != "3" ]; then
	echo ./calc returned the wrong number of errors: $nerrors s/b 3
	exit
fi
cmp test.out expected_results2.txt
if [ "$?" != "0" ]; then
	echo "Test output (test.out) does not match expected (expexted_results.txt):"
	diff test.out expected_results2.txt
	exit
fi

#
# Test 3a - read from a fle
#

echo Test "calc -f file" ...
./calc &> test.out -f commands.txt
nerrors=$?
if [ "$nerrors" != "3" ]; then
	echo ./calc returned the wrong number of errors: $nerrors s/b 3
	exit
fi
cmp test.out expected_results2.txt
if [ "$?" != "0" ]; then
	echo "Test output (test.out) does not match expected (expexted_results.txt):"
	diff test.out expected_results2.txt
	exit
fi

#
# Test 3b - try to read from a non-existant file
#

echo Test "calc -f file, where file doesn't exist ..."
./calc -f blif 2> test.out
nerrors=$?
if [ "$nerrors" != "1" ]; then
	echo "calc did not return a error!"
	exit
fi

# 
# Cleanup and return...
#

rm -f test.out commands.txt expected_results*.txt

echo All tests passed!
