#ifndef utils_h
#define utils_h

/**
 * reads a file
 * returns 1 for success
 */

int readFile(char *filename, char ***content, int *lines);

/**
 * char to int
 * returns -1 if char is not a number
 */

int cToI(char c);

#endif