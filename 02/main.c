#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *f = fopen("input.txt", "r");
    char buf[BUFSIZ];
    char token[16];
    int count_valid = 0;

    while ((fgets(buf, BUFSIZ, f) != NULL)) {
        char *l = buf;
        
        // read lower limit
        char *t = token;
        int lower_limit = 0;
        while (*l != '-') {
            *t = *l;
            t++; 
            l++;
        }
        *t = '\0';
        lower_limit = strtol(token, NULL, 10);

        // consume "-"
        l++;

        // read higher limit
        t = token;
        int higher_limit = 0;
        while (*l != ' ') {
            *t = *l;
            t++; 
            l++;
        }
        *t = '\0';
        higher_limit = strtol(token, NULL, 10);

        // consume space
        while (*l == ' ') l++;

        // read char requirement
        char c = *l++;

        // skip ":" and " "
        while (*l == ' ' || *l == ':') l++;

        // read until "\n", check if c is between low and higher limit
        if ( (l[lower_limit-1] == c && l[higher_limit-1] != c ) || (l[higher_limit-1] == c && l[lower_limit-1] != c) ) {
            count_valid++;
        }
         
    }
    fclose(f);

    printf("%d valid passwords\n", count_valid);
}
