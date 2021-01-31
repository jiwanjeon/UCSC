#include "universe.h"
#include <assert.h>
#include <stdlib.h>

// The Universe Structure
struct Universe {
  int rows;
  int cols;
  bool **grid;
  bool toroidal;
};

// function to check the given row and column
static bool check_bounds(Universe *u, int r, int c) {
  return r >= 0 && r < u->rows && c >= 0 && c < u->cols;
}

// apply modular arithmetic for row if the universe is toroidal
static int wrap_row(Universe *u, int r) {
  if (r < 0) {
    r = u->rows + r;
  }
  return r % u->rows;
}

// apply modular arithmetic for column if the universe is toroidal
static int wrap_col(Universe *u, int c) {
  if (c < 0) {
    c = u->cols + c;
  }
  return c % u->cols;
}

// This is the constructor function that creates a Universe.
// The ﬁrst two parameters it accepts are the number of rows and cols,
// indicating the dimensions of the underlying boolean grid.
// The last parameter toroidal is a boolean.
// If the value of toroidal is true, then the universe is toroidal.
// The return type of this function is of type Universe *,
// meaning the function should return a pointer to a Universe.
Universe *uv_create(int rows, int cols, bool toroidal) {
  Universe *u = malloc(sizeof(*u));
  assert(u);
  u->cols = cols;
  u->rows = rows;
  u->toroidal = toroidal;

  u->grid = (bool **)calloc(rows, sizeof(int *));
  assert(u->grid);
  for (int r = 0; r < rows; r += 1) {
    u->grid[r] = (bool *)calloc(cols, sizeof(bool));
    assert(u->grid[r]);
  }

  return u;
}

// This is the destructor function for a Universe.
// Simply put, it frees any memory allocated for a Universe
// by the constructor function.
void uv_delete(Universe *u) {
  if (u) {
    for (int r = 0; r < u->rows; r += 1) {
      free(u->grid[r]);
    }
    free(u->grid);
    free(u);
  }
}

// This function returns the number of rows in the
// speciﬁed Universe
int uv_rows(Universe *u) {
  assert(u);
  return u->rows;
}

// This function returns the number of columns in the
// speciﬁed Universe
int uv_cols(Universe *u) {
  assert(u);
  return u->cols;
}

// This function simply marks the cell at row r and column c as live.
// If the speciﬁed row and column lie outside the bounds of the universe,
// nothing changes.
void uv_live_cell(Universe *u, int r, int c) {
  assert(u);
  if (check_bounds(u, r, c)) {
    u->grid[r][c] = true;
  }
}

// This function simply marks the cell at row r and column c as dead.
// If the speciﬁed row and column lie outside the bounds of the universe,
// nothing changes.
void uv_dead_cell(Universe *u, int r, int c) {
  assert(u);
  if (check_bounds(u, r, c)) {
    u->grid[r][c] = false;
  }
}

// This function returns the value of the cell at row r and column c.
// If the row and column are out-of-bounds, false is returned.
// Again, true means the cell is live.
bool uv_get_cell(Universe *u, int r, int c) {
  assert(u);
  return check_bounds(u, r, c) && u->grid[r][c];
}

// This function will populate the Universe with row-column
// pairs read in from infile.
bool uv_populate(Universe *u, FILE *infile) {
  assert(u);
  assert(infile);
  int r, c;
  while (fscanf(infile, "%d %d", &r, &c) == 2) {
    if (!check_bounds(u, r, c)) {
      return false;
    }
    uv_live_cell(u, r, c);
  }
  return true;
}

// This function returns the number of live neighbors
// adjacent to the cell at row r and column c.
int uv_census(Universe *u, int r, int c) {
  assert(u);
  int lives = 0;
  if (check_bounds(u, r, c)) {
    for (int dr = -1; dr <= 1; ++dr) {
      for (int dc = -1; dc <= 1; ++dc) {
        if (!(dr == 0 && dc == 0)) {
          if (u->toroidal) {
            // apply modular arithmetic
            if (uv_get_cell(u, wrap_row(u, r + dr), wrap_col(u, c + dc))) {
              lives += 1;
            }
          } else {
            if (uv_get_cell(u, r + dr, c + dc)) {
              lives += 1;
            }
          }
        }
      }
    }
  }
  return lives;
}

// This functions prints out the universe to outfile.
void uv_print(Universe *u, FILE *outfile) {
  assert(u);
  assert(outfile);
  for (int r = 0; r < u->rows; ++r) {
    for (int c = 0; c < u->cols; ++c) {
      if (uv_get_cell(u, r, c)) {
        fputc('o', outfile);
      } else {
        fputc('.', outfile);
      }
    }
    fputc('\n', outfile);
  }
}
