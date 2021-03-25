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
