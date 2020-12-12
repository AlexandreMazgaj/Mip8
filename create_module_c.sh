#!/bin/bash

touch source/headers/$1.h
touch source/$1.c

module=$1

echo "#include \"$1.h\"" > source/$1.c
type="$(tr '[:lower:]' '[:upper:]' <<< ${module:0:1})${module:1}"
command="typedef struct $1 {
    
} $type;"
echo "$command" > source/headers/$1.h