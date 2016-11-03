//////////////////////////////////////////////////////////////////////////
//									//
// Matt Yee							     	//
// 2 Nov 2016							   	//
// New Beginnings Theory II					   	//
// Programming Assignment 3					   	//
// ws2.c							   	//
//									//
//////////////////////////////////////////////////////////////////////////
// Wordsearch game						     	//
// Searches through letter grid for matches to strings provided by user	//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ws2.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function main											//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
  int size, i, j;
  char *board = build_board(argv[1], &size);		// Build board from filename inputted by user
  char input[30];

  while(1){
    printf("\n");					// Print board
    for (i = 0; i <  size; i++){
      for (j = 0; j < size; j++){
        printf("%c ", *(board + i*size + j));
      }
      printf("\n");
    }

    printf("\n\nPlease enter string to search for:");
    scanf("%s", input);					// Get user input string
    for(i = 0; i < strlen(input); i++){
      input[i] = tolower(input[i]);			// Convert user input string to lowercase
    }

    search(board, input, size);				// Search board for user input string
  }

  return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function builds board from file name supplied by arguments.						//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

char * build_board(char *filename, int *size){
  FILE *fp = fopen(filename, "r");
  char line[30];
  fgets(line, 3, fp);		// Get first line (should be number)
  *size = atoi(line);	// Convert to integer, save as size of board
  char *board = (char *)malloc(*size * *size * sizeof(char));	// Build dynamically sized board
  fgets(line, 25, fp); 		// Get blank line
  fgets(line, 25, fp); 		// Get blank line

  // Build board, convert all letters to lowercase
  int i, j;
  for (i = 0; i < *size; i++){
    fgets(line, *size + 2, fp);
      for (j = 0; j < *size; j++){
        *(board + j + i*(*size)) = tolower(line[j]);
      }
    fgets(line, *size + 2, fp);
  }

  return board;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function searches for matches of first letter of word.						//
// If match found, calls helper functions "neighbor" to search for matching path.			//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void search(char *board, char input[30], int size){
  int len, i, j, k, l;
  len = strlen(input);				// Size of user input string
  for (i = 0; i <  size; i++){
    for (j = 0; j < size; j++){
      if (*(board + i*size + j) == input[0]){		
        *(board + i*size + j) = toupper(input[0]); // Make match location ("origin") uppercase
        k = i; l = j;
        neighbor(&k, &l, board, size, input); 	// Search adjacent coordinates for next letter in string
        *(board + i*size + j) = input[0]; // Make cuurent origin lowercase before proceeding to next origin
      }
    }
  } 
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function takes in coordinate of match location ("origin") of first letter of input word.     	//
// Searches all adjacent ("neighbor") positions for a match with second letter of input.		//
// If match found, makes match position uppercase and continues searching in that direction by calling  //
//	helper function "direction".									//
// Avoids re-checking of origin for a match.                                                   		//
// Finishes by returning all neighbor positions to original lowercase state.                   	 	//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void neighbor(int *k, int *l, char *board, int size, char input[30]){
  int i, j; // k and l are origin coordinates

  for (i = -1; i <= 1; i++){
    j = -1;
    if (*(board + (i + *k)*size + (j + *l)) == input[1]) {
      *(board + (i + *k)*size + (j + *l)) = toupper(input[1]);
      direction(i, j, k, l, input, board, size);
    }
    j = 1;
    if (*(board + (i + *k)*size + (j + *l)) == input[1]) {
      *(board + (i + *k)*size + (j + *l)) = toupper(input[1]);
      direction(i, j, k, l, input, board, size);
    }
  }

  if (*(board + (1 + *k)*size + (0 + *l)) == input[1]) {	// Zero included in arithmetic for clarity
    *(board + (1 + *k)*size + (0 + *l)) = toupper(input[1]);	// i = 1, j = 0
    direction(1, 0, k, l, input, board, size);
  }
  if (*(board + (-1 + *k)*size + (0 + *l)) == input[1]) {	// Zero included in arithmetic for clarity
    *(board + (-1 + *k)*size + (0 + *l)) = toupper(input[1]);	// i = -1, j = 0
    direction(-1, 0, k, l, input, board, size);
  }

  // After testing all adjacent neighbor positions, make them all lowercase again
  // Leave origin uppercase
  for (i = -1; i <= 1; i++){
    j = -1;
    *(board + (i + *k)*size + (j + *l)) = tolower(*(board + (i + *k)*size + (j + *l)));
  }
  for (i = -1; i <= 1; i++){
    j = 1;
    *(board + (i + *k)*size + (j + *l)) = tolower( *(board + (i + *k)*size + (j + *l)));
  }
  *(board + (1 + *k)*size + (0 + *l)) = tolower(*(board + (1 + *k)*size + (0 + *l)));
  *(board + (*k - 1)*size + (0 + *l)) = tolower( *(board + (*k - 1)*size + (0 + *l)));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function takes in match location coordinates ("origin") and step coordinates.		 	//
// Proceeds along step coordinates, stopping if a mismatch is found. 				 	//
// If mismatch found, converts entire board to lowercase except for origin.			 	//
// Else prints board with all matches in uppercase, then returns board to lowercase except for origin.	//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void direction(int i, int j, int *k, int *l, char input[30], char *board, int size){
  // k and l are origin coordinates
  // i and j are step coordiantes or direction
  int m, n, mult;	// mult serves as iterator, mult * step coordinates will yield potential path
  mult = 2;		// Skip mult = 0, 1 because mult = 0 is origin and mult = 1 are neighbor positions

  while (mult < strlen(input)){

    if (*(board + (mult*i + *k)*size + (mult*j + *l)) != input[mult]) {
      // Found mismatch while testing potential path
      for (m = 0; m < size; m++){      // Make entire board lowercase, except for origin (stays uppercase)
        for (n = 0; n < size; n++){
          *(board + m*size + n) = tolower(*(board + m*size + n));
        }
      }
      *(board + (*k)*size + *l) = toupper(*(board + (*k)*size + *l)); // Make origin uppercase
      return; // Exit function here after mismatch found
    }

    // Make current step uppercase to indicate match
    *(board + (mult*i + *k)*size + (mult*j + *l)) = toupper(*(board + (mult*i + *k)*size + (mult*j + *l)));
    mult += 1; // Continue testing next step on potential path
  }

  // If finish testing potential path, then word found
  // Print board by printing uppercase positions (all matches) and replacing lowercase positions with period
  printf("\nWord found:\n");
  for (m = 0; m <  size; m++){
    for (n = 0; n < size; n++){
      if (*(board + m*size + n) < 97) printf("%c ", *(board + m*size + n));
      else printf(". ");
    }
    printf("\n");
  }
  printf("\n");

  // Return entire board to lowercase state
  // Keep origin uppercase
  for (m = 0; m < size; m++){
    for (n = 0; n < size; n++){
      *(board + m*size + n) = tolower(*(board + m*size + n));
    }
  }
  *(board + (*k)*size + *l) = toupper(*(board + (*k)*size + *l));
}
