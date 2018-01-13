#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
/* VERSION 1.2
 *
 * scanner.h - public interface to scanner.c, the scanner module
 *
 */

extern int readInt(FILE *);
extern double readReal(FILE *);
extern char readChar(FILE *);
extern char readRawChar(FILE *);
extern char *readString(FILE *, int);
extern char *readToken(FILE *);
extern char *readLine(FILE *);
extern void *allocate(size_t);
extern void *reallocate(void *,size_t);
#endif
