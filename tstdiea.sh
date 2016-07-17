#!/bin/bash
if [ -z "$1" ]
then
echo "Usage: $0 alphabet"
echo "Example: $0 abc"
echo "Example: $0 abcdefghijklmnopqrstuvwxyz"
echo "Example: $0 0123456789"
exit 1
fi
genalfa "$1" | dieharder -g 200 -a
