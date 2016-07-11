#!/bin/bash
if [ -z "$1" ]
then
echo "Usage: $0 text"
echo "Example: $0 \"the quick brown fox\""
echo "Example: $0 \'the quick brown fox\'"
exit 1
fi
echo "$1" | captcha | pnmtopng >pic.png
display pic.png
