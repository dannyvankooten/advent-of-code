#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int day1()  {
    FILE *f = fopen("01.input", "r");
    if (!f) {
        err(EXIT_FAILURE, "error reading input file");
    }
    char buf[BUFSIZ];
    int numbers[BUFSIZ];
    int n = 0;
    while ((fgets(buf, BUFSIZ, f) != NULL)) {
        numbers[n++] = strtol(buf, NULL, 10);
    }
    fclose(f);

    for (int i=0; i < n; i++) {
        for (int j=i; j <n; j++) {
            for (int k=j; k < n; k++) {
                if (numbers[i] + numbers[j] + numbers[k] == 2020) {
                    printf("%d\n", numbers[i] * numbers[j] * numbers[k]);
                    return 0;
                }
            }
        }
    }
    return 0;
}