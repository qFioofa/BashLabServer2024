#!/bin/bash

function help() {
    cat << EOF
Usage: $0 [SCRIPT1_PATH] [SCRIPT2_PATH]

Launches two Lua scripts in separate terminal windows.

Arguments:
  SCRIPT1_PATH    Path to the first Lua script (default: ./scripts/client.lua)
  SCRIPT2_PATH    Path to the second Lua script (default: ./scripts/server.lua)

Options:
  --help          Display this help message and exit

Example:
  $0 ./scripts/my_client.lua ./scripts/my_server.lua
EOF
    exit 0
}

if [[ "$1" == "--help" ]]; then
    help
fi

SCRIPT1=${1:-"./server.lua"}
SCRIPT2=${2:-"./client.lua"}

function terminate(){
    local FILE=$1
    echo "Error: Lua script not found: $FILE"
    exit 1
}

if [[ ! -f "$SCRIPT1" ]]; then
    terminate "$SCRIPT1"
fi

if [[ ! -f "$SCRIPT2" ]]; then
    terminate "$SCRIPT2"
fi

SCRIPT1_NAME=$(basename "$SCRIPT1")
SCRIPT2_NAME=$(basename "$SCRIPT2")

gnome-terminal --title="$SCRIPT1_NAME" -- bash -c "lua $SCRIPT1; exec bash"
gnome-terminal --title="$SCRIPT2_NAME" -- bash -c "lua $SCRIPT2; exec bash"

echo "$SCRIPT1_NAME and $SCRIPT2_NAME launched in separate terminal windows."
