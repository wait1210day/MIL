#!/usr/bin/bash

declare dirs="common compiler include IR IRVM parser"
declare lines=0

for file in $(echo $(find $dirs | grep -v asmjit | grep -E '\.cc|\.h|\.c'))
do
    s=$(cat -n $file | tail -n -1 | awk '{print $1}')
    if [[ "x$s" = "x" ]]; then s=0; fi
    echo "$file: $s lines"
    lines=$(expr $lines + $s)
done

echo "$lines lines total."
