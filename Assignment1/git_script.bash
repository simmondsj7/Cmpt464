#! /bin/bash
# This script add to git with commit of "syntax" for when I am making
# quick changes to my Cmpt 464 git repo

function push(){
    
    git add -u;
    if [ $# -eq 2 -a "$1" = "-m" ]; then
	git commit -m "$2";
    else
	git commit -m "syntax fixing"
    fi
    
    git push;
}

