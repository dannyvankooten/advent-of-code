#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void parse(char *s, char needles[64][16], char replacements[64][16], int *nreplacements, char *medicine) {
    int n = 0;
    int i;
    while (*s != '\0') {
        while (*s != '\n') {
            for (i = 0; *(s + i) != ' '; i++);
            strncpy(needles[n], s, i);
            needles[n][i] = '\0';
            s += i;

            s += 4; // skip " => "

            for (i = 0; *(s + i) != '\n'; i++);
            strncpy(replacements[n], s, i);
            replacements[n][i] = '\0';

            s += i + 1;
            n++;
        }

        // parse medicine
        s++;
        char *m = medicine;
        while (*s != '\0' && *s != '\n') {
            *m++ = *s++;
        }
        *m = '\0';
        if (*s == '\n') s++;
    }

    *nreplacements = n;
}

int pt1(char needles[64][16], char replacements[64][16], int nreplacements, char *medicine) {
    char *molecules = (char *) malloc(1024 * 512 * sizeof(char));
    int nmolecules = 0;
    int pt1 = 0;

    char *m = medicine;
    while (*m != '\0') {
        for (int r = 0; r < nreplacements; r++) {
            if (strncmp(m, needles[r], strlen(needles[r])) != 0) {
                continue;
            }

            char *molecule = molecules + (nmolecules * 512 * sizeof(char));
            strncpy(molecule, medicine, m-medicine);
            strcat(molecule, replacements[r]);
            strcat(molecule, m + strlen(needles[r]));

            // is this a unique addition
            int unique = 1;
            for (int j = 0; j < nmolecules; j++) {
                if (strcmp(molecules + j * 512 * sizeof(char), molecule) == 0) {
                    unique = 0;
                    break;
                }
            }
            pt1 += unique;
            nmolecules++;
        }

        m++;
    }

    free(molecules);

    return pt1;
}

char *first_unmatched(char *a, char *b) {
    while (*a == *b && *a != '\0' && *b != '\0') {
        a++;
        b++;
    }

    if (*b == '\0') {
        return NULL;
    }

    return b;
}

int main() {
    clock_t start_t, end_t;
    start_t = clock();

    char input[1024*32];
    FILE *fp = fopen("input.txt", "r");
    size_t nread = fread(input, 1, 1024*32, fp);
    fclose(fp);
    input[nread] = '\0';

    char needles[64][16];
    char replacements[64][16];
    int nreplacements = 0;
    char medicine[512];
    parse(input,  needles, replacements, &nreplacements, medicine);

    int a1 = pt1(needles, replacements, nreplacements, medicine);

    int pt2 = 0;
    // TODO: Since we have multiple possible replacements
    // We need to split off into a recursion at every replacement with multiple replacement values
    // We can bail a recursion whenever the medicine prefix doesn't match.
    char result[1024*16];
    char buf[1024*16];
    strcpy(result, "e");

    printf("--- Day 19: Medicine for Rudolph ---\n");
    printf("Part 1: %d\n", a1);
    printf("Part 2: %d\n", pt2);

    end_t = clock();
    double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC * 1000;
    printf("Time: %.2fms\n", total_t);

    return 0;
}