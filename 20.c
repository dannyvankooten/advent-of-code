#include <assert.h>
#include <err.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tile tile_t;
struct tile {
  int32_t id;
  char grid[100];
  int32_t x;
  int32_t y;
};

typedef struct monster monster_t;
const struct monster {
  int32_t width;
  int32_t height;
  char* body;
} sea_monster = {
    .width = 20,
    .height = 3,
    .body =
        ""
        "                  # \n"
        "#    ##    ##    ###\n"
        " #  #  #  #  #  #   ",
};

#define W 10
#define H 10

int32_t parse_tiles_from_input(tile_t* tiles, char* file) {
  int32_t ntiles = 0;
  FILE* f = fopen(file, "r");
  if (!f) {
    err(EXIT_FAILURE, "error reading input file");
  }
  char linebuf[BUFSIZ] = {0};
  while (fgets(linebuf, BUFSIZ, f) != NULL) {
    tile_t* t = &tiles[ntiles++];
    t->x = -1;
    t->y = -1;

    // first line contains the tile ID: Tile 2311:
    sscanf(linebuf, "Tile %d:", &t->id);

    int32_t y = 0;
    while (fgets(linebuf, BUFSIZ, f) != NULL && *linebuf != '\n') {
      char* s = linebuf;
      for (int32_t x = 0; *s != '\n' && *s != '\0'; s++, x++) {
        t->grid[y * W + x] = *s;
      }

      y++;
    }
  }
  fclose(f);
  return ntiles;
}

void print_tile(tile_t* tile) {
  printf("Tile %d:\n", tile->id);
  for (int32_t y = 0; y < H; y++) {
    for (int32_t x = 0; x < W; x++) {
      printf("%c", tile->grid[y * W + x]);
    }
    printf("\n");
  }
}

void print_tiles(tile_t* tiles, int32_t ntiles) {
  for (int32_t i = 0; i < ntiles; i++) {
    print_tile(&tiles[i]);
  }
}

// https://en.wikipedia.org/wiki/Cartesian_coordinate_system#Rotation
void rotate(char* grid, int32_t size) {
  char new_grid[size * size];
  for (int32_t y = 0, x2 = size - 1; y < size; y++, x2--) {
    for (int32_t x = 0, y2 = 0; x < size; x++, y2++) {
      new_grid[y2 * size + x2] = grid[y * size + x];
    }
  }

  memcpy(grid, new_grid, size * size);
}

void flip(char* grid, int32_t size, char ax) {
  // 0 1 2 3 4 5 6 7 8 9
  // 9 8 7 6 5 4 3 2 1 0
  char new_grid[size * size];
  for (int32_t y = 0; y < size; y++) {
    for (int32_t x = 0; x < size; x++) {
      switch (ax) {
        case 'x':
          new_grid[y * size + (size - 1 - x)] = grid[y * size + x];
          break;

        case 'y':
          new_grid[(size - 1 - y) * size + x] = grid[y * size + x];
          break;
      }
    }
  }

  memcpy(grid, new_grid, size * size);
}

bool tiles_edges_match(tile_t* t1, tile_t* t2, char edge) {
  int32_t x1, x2, y1, y2;

  // edge can be one of 'N', 'E', 'S', 'W'
  switch (edge) {
    case 'N':
      y1 = 0;
      y2 = H - 1;
      for (int32_t x = 0; x < W; x++) {
        if (t1->grid[y1 * W + x] != t2->grid[y2 * W + x]) {
          return false;
        }
      }
      break;
    case 'S':
      y1 = H - 1;
      y2 = 0;
      for (int32_t x = 0; x < W; x++) {
        if (t1->grid[y1 * W + x] != t2->grid[y2 * W + x]) {
          return false;
        }
      }
      break;
    case 'E':
      x1 = W - 1;
      x2 = 0;
      for (int32_t y = 0; y < H; y++) {
        if (t1->grid[y * W + x1] != t2->grid[y * W + x2]) {
          return false;
        }
      }
      break;
    case 'W':
      x1 = 0;
      x2 = W - 1;
      for (int32_t y = 0; y < H; y++) {
        if (t1->grid[y * W + x1] != t2->grid[y * W + x2]) {
          return false;
        }
      }
      break;
    default:
      err(EXIT_FAILURE, "invalid edge argument: %c", edge);
      break;
  }

  return true;
}

void remove_image_borders(char* image, tile_t** tiles, int32_t image_size) {
  int32_t img_y = 0;
  int32_t img_x = 0;
  int32_t img_width = (W - 2) * image_size;
  for (int32_t image_row = 0; image_row < image_size; image_row++) {
    for (int32_t y = 1; y < H - 1; y++) {
      for (int32_t ti = 0; ti < image_size; ti++) {
        tile_t* t = tiles[image_row * image_size + ti];
        for (int32_t x = 1; x < W - 1; x++) {
          image[img_y * img_width + img_x++] = t->grid[y * W + x];
        }
      }
    }
  }
}

void print_image(tile_t** image, int32_t image_size) {
  for (int32_t image_y = 0; image_y < image_size; image_y++) {
    for (int32_t y = 0; y < H; y++) {
      for (int32_t image_x = 0; image_x < image_size; image_x++) {
        tile_t* t = image[image_y * image_size + image_x];
        for (int32_t x = 0; x < W; x++) {
          printf("%c", t != NULL ? t->grid[y * W + x] : '?');
        }
      }

      printf("\n");
    }
  }
}

void print_image_ids(tile_t** image, int32_t size) {
  for (int32_t y = 0; y < size; y++) {
    for (int32_t x = 0; x < size; x++) {
      tile_t* t = image[y * size + x];
      printf("%d\t", t != NULL ? t->id : 0);
    }
    printf("\n");
  }
}

void shift_image(tile_t** image, int32_t size, int8_t shift_y, int8_t shift_x) {
  tile_t* new_image[size * size];
  for (int32_t i = 0; i < size * size; i++)
    new_image[i] = NULL;
  tile_t* t;

  for (int32_t y = 0; y < size - shift_y; y++) {
    for (int32_t x = 0; x < size - shift_x; x++) {
      int8_t y2 = y + shift_y;
      int8_t x2 = x + shift_x;

      t = image[y * size + x];
      if (t) {
        t->x = x2;
        t->y = y2;
        new_image[y2 * size + x2] = t;
      }
    }
  }

  for (int32_t i = 0; i < size * size; i++) {
    image[i] = new_image[i];
  }
}

int32_t count_sea_monster_in_image(const char* image,
                                   int32_t image_size,
                                   const monster_t* monster) {
  int32_t count = 0;
  for (int32_t y = 0; y < image_size - monster->height; y++) {
    for (int32_t x = 0; x < image_size - monster->width; x++) {
      const char* i = &image[y * image_size + x];
      const char* m = monster->body;
      int32_t y_search = y;
      int32_t x_search = x;

      while (*m == ' ' || *i == *m) {
        i++;
        m++;

        if (*m == '\0') {
          count++;
        }

        if (*m == '\n') {
          y_search++;
          x_search = x;
          i = &image[y_search * image_size + x_search];
          m++;
        }
      }
    }
  }

  return count;
}

int day20() {
  tile_t* tiles = (tile_t*)malloc(200 * sizeof(tile_t));
  if (!tiles) {
    err(EXIT_FAILURE, "could not allocate memory for tiles");
  }

  int32_t ntiles = parse_tiles_from_input(tiles, "inputs/20.txt");
  // print_tiles(tiles, ntiles);

  // init empty image
  int32_t image_size = (int32_t)sqrt((double)ntiles);
  tile_t** image =
      (tile_t**)calloc(image_size * image_size * 2, sizeof(tile_t*));
  if (!image) {
    err(EXIT_FAILURE, "could not allocate memory for image");
  }

  // For each tile, find NESW neighbors and place in image grid
  image[0] = &tiles[0];
  tiles[0].y = 0;
  tiles[0].x = 0;

STARTOVER:;
  for (int32_t i = 0; i < ntiles; i++) {
    // only work with tiles already in image
    tile_t* t1 = &tiles[i];
    if (t1->y < 0) {
      continue;
    }

    for (int32_t j = 0; j < ntiles; j++) {
      // skip self
      if (i == j) {
        continue;
      }

      // skip tiles already in image
      tile_t* t2 = &tiles[j];
      if (t2->x >= 0) {
        continue;
      }

      for (int8_t r = 0; r < 4; r++) {
        if (tiles_edges_match(t1, t2, 'E')) {
          if (t1->x == image_size - 1) {
            shift_image(image, image_size, 0, -1);
          }
          t2->y = t1->y;
          t2->x = t1->x + 1;
          image[t2->y * image_size + t2->x] = t2;
          goto STARTOVER;
        } else if (tiles_edges_match(t1, t2, 'S')) {
          if (t1->y == image_size - 1) {
            shift_image(image, image_size, -1, 0);
          }
          t2->y = t1->y + 1;
          t2->x = t1->x;

          image[t2->y * image_size + t2->x] = t2;
          goto STARTOVER;
        } else if (tiles_edges_match(t1, t2, 'N')) {
          // if t1 was at northern edge, shift it down
          if (t1->y == 0) {
            shift_image(image, image_size, 1, 0);
          }
          t2->y = t1->y - 1;
          t2->x = t1->x;
          image[t2->y * image_size + t2->x] = t2;
          goto STARTOVER;
        } else if (tiles_edges_match(t1, t2, 'W')) {
          // if t1 was at western edge, shift it down
          if (t1->x == 0) {
            shift_image(image, image_size, 0, 1);
          }
          t2->y = t1->y;
          t2->x = t1->x - 1;
          image[t2->y * image_size + t2->x] = t2;
          goto STARTOVER;
        }

        if (r == 3) {
          flip(t2->grid, W, 'x');
        } else {
          rotate(t2->grid, W);
        }
      }
    }
  }

  // print_image_ids(image, image_size);
  // print_image(image, image_size);

  // generate our final image, plain char array
  int32_t final_image_size = (W - 2) * image_size;
  char* final_image = malloc((W - 2) * image_size * (H - 2) * image_size);
  if (!final_image) {
    err(EXIT_FAILURE, "error allocating memory for image");
  }
  remove_image_borders(final_image, image, image_size);

  int32_t sea_monster_count = 0;
  for (int32_t r = 0; r < 3; r++) {
    for (int32_t f = 0; f < 2; f++) {
      sea_monster_count = count_sea_monster_in_image(
          final_image, final_image_size, &sea_monster);
      if (sea_monster_count > 0) {
        goto DETERMINE_WATER_ROUGHNESS;
      }

      flip(final_image, final_image_size, 'x');
    }
    rotate(final_image, final_image_size);
  }

DETERMINE_WATER_ROUGHNESS:;

  // count # in monster
  int32_t c1 = 0;
  const char* m = sea_monster.body;
  while (*m != '\0') {
    if (*m == '#') {
      c1++;
    }

    m++;
  }

  // count # in image
  int32_t c2 = 0;
  for (int32_t i = 0; i < final_image_size * final_image_size; i++) {
    if (final_image[i] == '#') {
      c2++;
    }
  }

  int32_t answer = c2 - (c1 * sea_monster_count);
  printf("%d\n", answer);
  assert(2009 == answer);

  free(final_image);
  free(image);
  free(tiles);
  return 0;
}
