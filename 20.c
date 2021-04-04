#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputs/20.h"

#define W 10
#define H 10


enum edge {
  EDGE_N,
  EDGE_E,
  EDGE_S,
  EDGE_W,
  EDGE_NONE = -1,
};
typedef enum edge edge_t;

enum ax {
  AX_X = 'x',
  AX_Y = 'y',
};
typedef enum ax ax_t;

enum match {
  NO_MATCH,
  MATCH,
  INVERSE_MATCH
};
typedef enum match match_t;

typedef struct tile tile_t;
struct tile {
  int32_t id;
  char grid[W*H];
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

int32_t parse_tiles_from_input(tile_t* tiles) {
  int32_t ntiles = 0;
  const unsigned char *s = input;
  while (*s != '\0') {
    tile_t* t = &tiles[ntiles++];
    t->x = -1;
    t->y = -1;

    // first line contains the tile ID: Tile 2311:
    s += 5;
    t->id = 0;
    while (isdigit(*s)) {
      t->id = (t->id * 10) + (*s - '0');
      s++;
    }

    s++; // ':'
    s++; // '\n'

    int32_t y = 0;
    while (*s != '\0') {
      for (int32_t x = 0; *s != '\n' && *s != '\0'; s++, x++) {
        t->grid[y * W + x] = *s;
      }

      // linebreak: new row
      if (*s == '\n') {
        s++;
        y++;
      }

      // empty line: new tile
      if (*s == '\n') {
        s++;
        break;
      }
    }
  }

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

  memcpy(grid, new_grid, size * size * sizeof(char));
}

void flip(char* grid, int32_t size, ax_t ax) {
  // 0 1 2 3 4 5 6 7 8 9
  // 9 8 7 6 5 4 3 2 1 0
  char new_grid[size * size];
  
  for (int32_t y = 0; y < size; y++) {
    for (int32_t x = 0; x < size; x++) {
      switch (ax) {
        case AX_X:
          new_grid[y * size + (size - 1 - x)] = grid[y * size + x];
          break;

        case AX_Y:
          new_grid[(size - 1 - y) * size + x] = grid[y * size + x];
          break;
      }
    }
  }

  memcpy(grid, new_grid, size * size * sizeof(char));
}


void
extract_edge(tile_t* t, edge_t e, char dest[W], bool inverse) {
// extract_edge(tile_t* t, edge_t e, char dest[W]) {

  switch (e) {
    case EDGE_N:
      for (int32_t y=0, x=0; x < W; x++) {
        dest[x] = t->grid[y * W + x];
      }
    break;

    case EDGE_E:
      for (int32_t y=0, x=W-1; y < H; y++) {
        dest[y] = t->grid[y * W + x];
      }
    break;

    case EDGE_S:
      for (int32_t y=H-1, x=0; x < W; x++) {
        dest[x] = t->grid[y * W + x];
      }
    break;

    case EDGE_W:
      for (int32_t y=0, x=0; y < H; y++) {
        dest[y] = t->grid[y * W + x];
      }
    break;

    default: 
      err(EXIT_FAILURE, "can not extract EDGE_NONE"); 
      break;
  }

  if (inverse) {
    char tmp;
    for (int32_t i=0, j = W-1; i < j; i++, j--) {
      tmp = dest[i];
      dest[i] = dest[j];
      dest[j] = tmp;
    }
  }
}

match_t
cmp_edges(char edge_1[W], char edge_2[W]) {
  // edges match as-is
  if (memcmp(edge_1, edge_2, W * sizeof(char)) == 0) {
    return MATCH;
  }

  // reversed edge matches
  int32_t i;
  for (i=0; i < W; i++) {
    if (edge_1[i] != edge_2[W-1-i]) {
      break;
    }
  }
  if (i == W) {
    return INVERSE_MATCH;
  }

  // no match
  return NO_MATCH;
}

bool 
fit_other_on_edge(tile_t* t1, tile_t *t2, edge_t e1) {
  char edge_t1[W];
  char edge_t2[W];
  extract_edge(t1, e1, edge_t1, false);
  edge_t desired_edge = (e1 + 2) % 4;

  for (edge_t actual_edge=EDGE_N; actual_edge <= EDGE_W; actual_edge++) {    
    extract_edge(t2, actual_edge, edge_t2, (desired_edge / 2) != (actual_edge / 2));
    match_t match = cmp_edges(edge_t1, edge_t2);
    if (match > 0) {
       while (actual_edge != desired_edge) {
          rotate(t2->grid, W);
          actual_edge = (actual_edge + 1) % 4;
        }

        if (match == INVERSE_MATCH) {
          flip(t2->grid, W, desired_edge == EDGE_N || desired_edge == EDGE_S ? AX_X : AX_Y);
        }

        return true;
    }
  }

  return false;
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

  memcpy(image, new_image, size*size*sizeof(tile_t *));
}

int32_t count_sea_monster_in_image(const char* image,
                                   int32_t image_size) {
  int32_t count = 0;
  for (int32_t y = 0; y < image_size - sea_monster.height; y++) {
    for (int32_t x = 0; x < image_size - sea_monster.width; x++) {
      const char* i = &image[y * image_size + x];
      const char* m = sea_monster.body;
      int32_t y_search = y;
      int32_t x_search = x;

      while (*m == ' ' || *i == *m) {
        i++;
        m++;

        // reached end of monster, it's a match!
        if (*m == '\0') {
          count++;
        }

        // monster line matched, 
        // move to same x position in next row
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

  int32_t ntiles = parse_tiles_from_input(tiles);
  // print_tiles(tiles, ntiles);

  // init empty image
  int32_t image_size = (int32_t)sqrt((double)ntiles);
  tile_t** image =
      (tile_t**)calloc(image_size * image_size, sizeof(tile_t*));
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
      // skip tiles already in image
      tile_t* t2 = &tiles[j]; // (incl. self)
      if (t2->x >= 0) {
        continue;
      }

      // skip tiles at edge of image or for which all neighbors are known
      const bool neighbors[4] = {
        t1->y > 0 ? image[(t1->y-1) * image_size + t1->x] : false,                      // N
        t1->x < image_size - 1 ? image[t1->y * image_size + t1->x + 1] != NULL : true,  // E
        t1->y < image_size - 1 ? image[(t1->y+1) * image_size + t1->x] != NULL : true,  // S
        t1->x > 0 ? image[t1->y * image_size + t1->x - 1] != NULL : false,              // W
      };
      if (neighbors[EDGE_N] && neighbors[EDGE_E] && neighbors[EDGE_S] && neighbors[EDGE_W]) {
        continue;
      }      
      
      if (!neighbors[EDGE_E] && fit_other_on_edge(t1, t2, EDGE_E)) {
        t2->y = t1->y;
        t2->x = t1->x + 1;
        image[t2->y * image_size + t2->x] = t2;
        continue;
      } else if (!neighbors[EDGE_S] && fit_other_on_edge(t1, t2, EDGE_S)) {
        t2->y = t1->y + 1;
        t2->x = t1->x;
        image[t2->y * image_size + t2->x] = t2;
        continue;
      } else if (!neighbors[EDGE_N] && fit_other_on_edge(t1, t2, EDGE_N)) {
        // if t1 was at northern edge, shift entire image south
        if (t1->y == 0) {
          shift_image(image, image_size, 1, 0);
        }
        t2->y = t1->y - 1;
        t2->x = t1->x;
        image[t2->y * image_size + t2->x] = t2;
        continue;
      } else if (!neighbors[EDGE_W] && fit_other_on_edge(t1, t2, EDGE_W)) {
        // if t1 was at western edge, shift entire image east
        if (t1->x == 0) {
          shift_image(image, image_size, 0, 1);
        }
        t2->y = t1->y;
        t2->x = t1->x - 1;
        image[t2->y * image_size + t2->x] = t2;
        continue;
      }
    }
  }

  // repeat fitting if image is not yet fully figured out yet
  for (int32_t i=0; i < image_size * image_size; i++) {
    if (image[i] == NULL) {
      goto STARTOVER;
    }
  }

  // print_image_ids(image, image_size);
  // print_image(image, image_size);

  // generate our final image, plain char array
  int32_t final_image_size = (W - 2) * image_size;
  char* final_image = (char*) malloc(final_image_size * final_image_size);
  if (!final_image) {
    err(EXIT_FAILURE, "error allocating memory for image");
  }
  remove_image_borders(final_image, image, image_size);

  int32_t sea_monster_count = 0;
  for (int32_t r = 0; r < 3; r++) {
    for (int32_t f = 0; f < 2; f++) {
      sea_monster_count = count_sea_monster_in_image(
          final_image, final_image_size);
      if (sea_monster_count > 0) {
        goto DETERMINE_WATER_ROUGHNESS;
      }

      flip(final_image, final_image_size, AX_Y);
    }
    rotate(final_image, final_image_size);
  }

DETERMINE_WATER_ROUGHNESS:;

  // count # in monster
  int32_t c1 = 0;
  for (const char *m = sea_monster.body; *m != '\0'; m++) {
    if (*m == '#') {
      c1++;
    }
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
