#!/bin/bash

# Daniel Wright	diw5233@psu.edu
# Evan Gutman	elg5195@psu.edu

#checks number of arguments
if [ $# -eq 0 ]; then
    echo "0 arguments provided. 3 required."
    exit 1
fi
if [ $# -eq 1 ]; then
    echo "Only 1 argument provided. 3 required."
    exit 1
fi
if [ $# -eq 2 ]; then
    echo "Only 2 arguments provided. 3 required."
    exit 1
fi
if [ $# -gt 3 ]; then
    echo "Too many arguments. 3 required."
    exit 1
fi

start=$(date +%s%N)

#opens the input file and removes all non-alphanumeric characters
cat "$1" | LC_CTYPE=C sed 's/[^0-9A-Za-z ]//g' |
#converts from uppercase to lowercase
tr '[:upper:]' '[:lower:]' |
#puts everything on a new line
sed 's/ /\n/g' |
#deletes all the empty lines
sed '/^\s*$/d' |
#sorts the list, counts the occurence of each word and prints to an output file
sort |  uniq -c | awk '{print $2", "$1}' > "$2"

#outputs the runtime to an output file
echo "runtime: $(echo "scale=6;($(date +%s%N) - ${start})/(1*10^09)" | bc) seconds" > "$3"



