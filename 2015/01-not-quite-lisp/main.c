#include <stdlib.h>
#include <stdio.h>

int main() {
    FILE *fp = fopen("input.txt", "r");
    char input[64*1024];
    size_t ninput = fread(input, 1, 64*1024, fp);

    int v = 0;
    int pos = -1;
    for (int i = 0; i < ninput; i++) {
        if (input[i] == '(') {
            v++;
        } else if (input[i] == ')') {
            v--;
        }

        if (pos == -1 && v == -1) {
            pos = i + 1;
        }
    }

    fprintf(stdout, "part 1: %d\n", v);
    fprintf(stdout, "part 2: %d\n", pos);
    return 0;
}
