#!/bin/bash

if [[ `basename $PWD` == "kite" && -d doc ]]; then
    if [[ ! -f ~/.sid ]]; then
        read -p "Enter your 10-digit student ID: " sid
        echo "$sid" > ~/.sid
    fi
    sid=`tail -1 ~/.sid`
    make clean > /dev/null
    rm -rf .git *.swp $sid.tar assembly.sh functions.sh optimization.sh branch.sh cache.sh
    cd ..;
    tar cf $sid.tar --exclude="kite/doc" kite
    mv $sid.tar kite
else
    echo "Error: $0 must run in the kite/ directory"
fi

