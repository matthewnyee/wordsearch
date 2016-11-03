//////////////////////////////////////////////////////////////////////////////////////////////////
//												//
// Matt Yee											//
// 2 Nov 2016											//
// New Beginnings Theory II									//
// Programming Assignment 3									//
// ws1.c											//
//												//
//////////////////////////////////////////////////////////////////////////////////////////////////
// Word search game										//
// Comments written by Matt Yee									//
// Code otherwise copied from Chong Kim for educational purposes				//
// https://github.com/chongkim/wordsearch/blob/master/c/wordsearch.c                            //
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *read_file(char *filename)
{
  FILE *fp = fopen(filename, "r");
  fseek(fp, 0, SEEK_END);	// zero offset from end
  long fsize = ftell(fp);	// position of end of file (in bites)
  fseek(fp, 0, SEEK_SET);	// reset file "back to beginning"
  char *buf = malloc(fsize+1);	// + 1 for null character
  fread(buf, 1, fsize, fp);	// 1 character, fsize # of times
  buf[fsize] = 0;		// does this add null char???
  fclose(fp);
  return buf;
}

struct data_t {
  char **grid;
  int gridlen;		// # of rows
  char **words;
  int wordslen;		// # of  words
};

void parse_data(char *buf, struct data_t *data)
{
  int size = 1;
  char *b = buf;

// parsing word grid
  data->grid = NULL;
  data->gridlen = 0;
	// for loop:
	// start: updates buf to continue onto next line?
	// end: as long as string is not empty
	// step: "continue"
  for (char *s = strsep(&b, "\n"); s != NULL && *s; s = strsep(&b, "\n")) {
    if (data->gridlen % size == 0)
      data->grid = realloc(data->grid, sizeof(char*)*(data->gridlen + size));
    data->grid[data->gridlen++] = s;
  }


// parsing words to search
  data->words = NULL;
  data->wordslen = 0;
// search for commas,	 stop when hit end of string (NULL)
  for (char *s = strsep(&b, ","); s != NULL; s = strsep(&b, ",")) {
    while (isspace(*s))
      s++;
    if (data->wordslen % size == 0)
      data->words = realloc(data->words, sizeof(char*)*(data->wordslen + size));
    data->words[data->wordslen++] = s;
    while (*s) {
      *s = toupper(*s);
      s++;
    }
  }
}

struct rc_t {
  int row;
  int col;
  struct rc_t *next;
};

int in_path(struct rc_t *path, int row, int col)
{
  return path == NULL ? 0 : path->row == row && path->col == col ? 1 : in_path(path->next, row, col);
}

void search(struct data_t data, int row, int col, char *word, struct rc_t *path)
{
  if (!*word) {
    for (struct rc_t *rc = path; rc; rc = rc->next) {
      data.grid[rc->row][rc->col] |= 0x80;
    }
  } else if (0 <= row && row < data.gridlen && 0 <= col && col < strlen(data.grid[row]) &&
      !in_path(path, row, col) && (*word == (data.grid[row][col] & 0x7f) || *word == '?')) {
    for (int r = -1; r <= 1; ++r) {
      for (int c = -1; c <= 1; ++c) {
        struct rc_t new_path = { row, col, path };
        search(data, row+r, col+c, word+1, &new_path);
      }
    }
  }
}

int main(int argc, char *argv[])
{
  char *buf = read_file(argv[1]);	// reads in file
  struct data_t data;
  parse_data(buf, &data);

  for (int i = 0; i < data.wordslen; ++i) {
    for (int row = 0; row < data.gridlen; ++row) {
      int len = strlen(data.grid[row]);
      for (int col = 0; col < len; ++col) {
        search(data, row, col, data.words[i], NULL);
      }
    }
  }
  for (int row = 0; row < data.gridlen; ++row) {
    int len = strlen(data.grid[row]);//if this in the for loop param would execute every time
    char *sep = "";			// first time around, separate on nothing ("")
    for (int col = 0; col < len; ++col) {
      char ch = data.grid[row][col];
      printf("%s%c", sep, ch & 0x80 ? ch & 0x7f : '.'); // bitwise and?
      sep = " ";			// for rest of loop, separate on " "
    }
    puts("");
  }
  puts("");
  char *sep = "";
  for (int i = 0; i < data.wordslen; ++i) {
    printf("%s%s", sep, data.words[i]);
    sep = ", ";
  }
  puts("");
  free(buf);	// free memory allocated to buf (in function read_file)
  return 0;
}
