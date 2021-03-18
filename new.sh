#!/bin/bash

if [ "$#" -lt 1 ]; then
    echo "Provide a day number. Example: new 01"
    exit
fi


mkdir -p "$1"
FILE="$1/main.c"

if [ -f "$FILE" ]; then
    read -p "File exists. Overwrite with default template? (N/y)" ANSWER
    if [ "$ANSWER" != "Y" ]; then
        exit
    fi
fi;

touch "$1/input.txt"
touch $FILE
cat > $FILE <<- EOM
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        
    }
    fclose(f);
}
EOM