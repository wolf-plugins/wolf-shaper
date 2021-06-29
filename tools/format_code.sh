#!/usr/bin/env bash

# This script formats all the cpp/hpp files in a specific directory using clang-format.

set -e
shopt -s globstar

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments."
	echo "Usage: format_code.sh <dir>"
	exit 1
fi

for cfile in "$1"/**/*.{cpp,hpp}; do
	echo "Formatting $cfile"
	clang-format -style=file -i "$cfile" 
done
