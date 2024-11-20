#!/bin/bash

function help() {
    cat << EOF
Usage: $0 [COMP1_PATH] [COMP2_PATH]

Launches two compiled C executables in separate terminal windows.

Arguments:
  COMP1_PATH    Path to the first executable (default: ./compiled/client)
  COMP2_PATH    Path to the second executable (default: ./compiled/server)

Options:
  --help        Display this help message and exit

Example:
  $0 ./compiled/my_client ./compiled/my_server
EOF
    exit 0
}

if [[ "$1" == "--help" ]]; then
    help
fi

COMP1=${1:-"./compiled/client"}
COMP2=${2:-"./compiled/server"}

function terminate(){
    local FILE=$1
    echo "Error: Compiled file executable not found: $FILE"
    exit 1
}

if [[ ! -f "$COMP1" ]]; then
    terminate "$COMP1"
fi

if [[ ! -f "$COMP2" ]]; then
    terminate "$COMP2"
fi

COMP1_NAME=$(basename "$COMP1")
COMP2_NAME=$(basename "$COMP2")

gnome-terminal --title="$COMP1_NAME" -- bash -c "$COMP1; exec bash"
gnome-terminal --title="$COMP2_NAME" -- bash -c "$COMP2; exec bash"

echo "$COMP1_NAME and $COMP2_NAME launched in separate terminal windows."
