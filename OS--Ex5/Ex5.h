#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAXDATASIZE 1024 // max number of bytes we can get at once
#define lockFile "lock.txt"
int fd;
struct flock lock;
#ifndef test
#define test
#include "stack.h"
#endif
