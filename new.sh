#!/bin/bash

if [ $# != 1 ]; then
    echo "Usage: $(basename "$0") <day-number>" >&2
    exit 1
fi

FILE="$1.c"
if [ -f "$FILE" ]; then
    read -p "File exists. Overwrite with default template? (N/y)" ANSWER
    if [ "$ANSWER" != "Y" ]; then
        exit
    fi
fi;

touch "$1.txt"
touch $FILE
cat > $FILE <<- EOM
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <assert.h>

int main() {
    FILE *f = fopen("$1.txt", "r");
    if (!f) err(EXIT_FAILURE, "error reading input file");
    char linebuf[BUFSIZ] = {0};
    while (fgets(linebuf, BUFSIZ, f) != NULL) {
        
    }
    fclose(f);
}
EOM