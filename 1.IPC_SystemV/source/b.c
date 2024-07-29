#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <memory.h>
#include "onepurch.h"


int foo() {

char *s = "abcde";
char *t = "12345";

#if defined(LINUX) && defined(ANDROID)
printf ("%s\n", s);
#else
printf ("%s\n", t);
#endif

return 0;
}

