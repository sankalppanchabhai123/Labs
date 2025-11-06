#!/bin/bash

echo "=== Arithmetic Comparison using If-Else ==="

# Read two numbers
echo "Enter first number:"
read num1

echo "Enter second number:"
read num2

echo "-----------------------------------"

# Equal comparison
if [ $num1 -eq $num2 ]
then
    echo "$num1 is EQUAL to $num2"
else
    echo "$num1 is NOT EQUAL to $num2"
fi

# Greater/Less comparison
if [ $num1 -gt $num2 ]
then
    echo "$num1 is GREATER THAN $num2"
elif [ $num1 -lt $num2 ]
then
    echo "$num1 is LESS THAN $num2"
else
    echo "$num1 is EQUAL to $num2"
fi