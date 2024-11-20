#!/bin/bash

COMPILE_SH=${1:-"./compile.sh"}
LAUNCH_SH=${2:-"./launch_compiled.sh"}


function terminate(){
    local FILE=$1
    echo "Error: file not found: $FILE"
    exit 1
}

if [[ ! -f "$COMPILE_SH" ]]; then
    terminate "$COMPILE_SH"
fi

if [[ ! -f "$LAUNCH_SH" ]]; then
    terminate "$LAUNCH_SH"
fi

bash $COMPILE_SH
bash $LAUNCH_SH

echo "Core launch: succes"
