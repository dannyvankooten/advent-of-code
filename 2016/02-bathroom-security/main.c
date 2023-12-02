#include <stdlib.h>
#include <stdio.h>

size_t read_input(char *dst) {
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "error reading input.txt\n");
        abort();
    }
    size_t nread = fread(dst, 1, 64*1024, fp);
    fclose(fp);
    dst[nread++] = '\0';
    return nread;
}

void solve(char *s, char * const *grid, int gridsize) {
    int x = (gridsize-1) / 2;
    int y = x;

    while (*s != '\0') {
        switch (*s) {
            case '\n':
                fprintf(stdout, "%c", grid[y][x]);
            break;

            case 'U':
                if (y > 0 && grid[y-1][x] != ' ') {
                    y--;
                }
            break;

            case 'L':
                if (x > 0 && grid[y][x-1] != ' ') {
                    x--;
                }
            break;

            case 'R':
                if (x < (gridsize-1) && grid[y][x+1] != ' ') {
                    x++;
                }
            break;

            case 'D':
                if (y < (gridsize-1) && grid[y+1][x] != ' ') {
                    y++;
                }
            break;
        }

        s++;
    }

    printf("\n");
}

int main() {
   char input[64*1024];
   read_input(input);

    char *grid1[] = {
       "123",
       "456",
       "789",
   };
   solve(input, grid1, 3);

   char *grid2[] = {
       "  1  ",
       " 234 ",
       "56789",
       " ABC ",
       "  D  ",
   };
   solve(input, grid2, 5);
}
