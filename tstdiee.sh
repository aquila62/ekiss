#!/bin/bash
if [ -z "$2" ]
then
echo "Usage: $0 alphabet password"
echo "Example: $0 abc qrs"
echo "Example: $0 abcdef qrs"
exit 1
fi
genalfa "$1" | ekissencr "$2" | dieharder -g 200 -a
