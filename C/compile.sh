#!/bin/bash

function help() {
    cat << EOF
Usage: $0 [SAVE_DIR] [FILE1_PATH] [FILE2_PATH]

Compiles two C files and saves the compiled output to the specified directory.

Arguments:
  SAVE_DIR      Directory where the compiled files will be saved (default: ../compiled)
  FILE1_PATH    Path to the first C file (default: ./src/client.c)
  FILE2_PATH    Path to the second C file (default: ./src/server.c)

Options:
  --help        Display this help message
EOF
    exit 0
}

if [[ "$1" == "--help" ]]; then
    help
fi


SAVE_DIR=${1:-"./compiled"}
FILE1_PATH=${2:-"./src/client.c"}
FILE2_PATH=${3:-"./src/server.c"}

if [[ ! -f "$FILE1_PATH" ]]; then
    echo "Error: File1 does not exist: $FILE1_PATH"
    exit 1
fi

if [[ ! -f "$FILE2_PATH" ]]; then
    echo "Error: File2 does not exist: $FILE2_PATH"
    exit 1
fi

mkdir -p "$SAVE_DIR"

gcc "$FILE1_PATH" -o "$SAVE_DIR/$(basename "$FILE1_PATH" .c)"
echo "Compiled $FILE1_PATH to $SAVE_DIR/$(basename "$FILE1_PATH" .c)"

gcc "$FILE2_PATH" -o "$SAVE_DIR/$(basename "$FILE2_PATH" .c)"
echo "Compiled $FILE2_PATH to $SAVE_DIR/$(basename "$FILE2_PATH" .c)"

