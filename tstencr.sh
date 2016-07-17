#!/bin/bash
if [ -z "$2" ]
then
echo "Usage: $0 filename password"
echo "Example: $0 ekiss.o abcdef"
exit 1
fi
if [ -e "$1" ]
then
md5sum "$1"
cat "$1" | ekissencr $2 | ekissencr $2 | md5sum
echo "The two md5sums should be equal"
else
echo "filename does not exist"
echo "Usage: $0 filename password"
echo "Example: $0 ekiss.o abcdef"
exit 1
fi
