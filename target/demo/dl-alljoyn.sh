#!/bin/bash

case "$1" in
( '' )  root="$PWD/demo/aj/allseen" ;;
( * )   root="$1" ;;
esac

function downloadexpand() {

    echo "Downloading and extracting https://git.allseenalliance.org/cgit/$2/$3.git/snapshot/$4.zip"

    ALLJOYNRETRY=3
    while [ $ALLJOYNRETRY -gt 0 ]; do

        ( cd "$1/$2/$3" 2>/dev/null ) && { echo >&2 "error, directory $1/$2/$3 already exists" ; exit 2 ; }
        rm -rf "$1/$2/$3"

        (
            set -e
            cd "$1/$2"    || { echo >&2 "error, directory $1/$2 not found" ; exit 2 ; }
            rm -rf "$4" "$4.zip"
            w_opt="-nv --no-check-certificate"
            wget $w_opt "https://git.allseenalliance.org/cgit/$2/$3.git/snapshot/$4.zip"
            unzip -q "$4.zip" 
            mv -f  "$4" "$3"
            rm -rf "$4.zip" "$4"
        )

        ( cd "$1/$2/$3" )
        if [ $? == 0 ]; then
            break
        else 
            { echo >&2 "error, directory $1/$2/$3 not found after downloading and expanding" ; }
        fi

        let ALLJOYNRETRY=ALLJOYNRETRY-1
    done

    if [ $ALLJOYNRETRY == 0 ]; then
        echo "Error: AllJoyn downloadexpand failure"
        return 2
    fi
}

echo "Installing open source files in $root"

mkdir -p "$root/core" "$root/services" || : ok


downloadexpand  "$root" core        ajtcl       af24e93af4d6cd55aae8a2b97635d8004fdae8c6
if [ $? != 0 ]; then
    echo "Error: AllJoyn download failure"
    exit $?
fi

downloadexpand  "$root" services    base_tcl    c0867c9bd03680197a811ee462247eaaed7011cb
if [ $? != 0 ]; then
    echo "Error: AllJoyn download failure"
    exit $?
fi


