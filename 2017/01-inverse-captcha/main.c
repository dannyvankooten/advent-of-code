#include <stdlib.h>
#include <stdio.h>


int solve(const char *input, size_t size, int offset) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        int next = (i + offset) % (size-1);
        if (input[i] == input[next]) {
            sum += input[i] - '0';
        }
    }

    return sum;
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        abort();
    }
    char input[32*1024];
    size_t nread = fread(input, 1, 32*1024, fp);
    input[nread] = '\0';

    printf("%d\n", solve(input, nread, 1));
    printf("%d\n", solve(input, nread, (nread-1)/2));
    return 0;
}
