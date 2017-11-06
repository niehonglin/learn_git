#!/bin/bash

case "$1" in
( '' )  root="$PWD/demo/aj/allseen" ;;
( * )   root="$1" ;;
esac

function copytodestination() {

    echo "Copying sample source files to sample destination"

    (
        cp "$1/core/ajtcl/test/svclite.c" "$1/../alljoyn/aj_qca4004/apps/aj_svclite/"
    )

    return 0
}

(
    LAUNCH_DIR=$(echo $(cd $(dirname $0); pwd )) 
    "$LAUNCH_DIR/dl-alljoyn.sh" "$1" 

    if [ $? != 0 ]; then
        echo "Error: AllJoyn download failure"
        exit 1
    fi

    copytodestination "$root"
)
