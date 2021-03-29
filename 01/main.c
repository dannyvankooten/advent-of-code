#include <stdio.h>
#include <stdlib.h>
 
int main()  {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        return 1;
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
                    printf("Result: %d", numbers[i] * numbers[j] * numbers[k]);
                    return 0;
                }
            }
        }
    }
}