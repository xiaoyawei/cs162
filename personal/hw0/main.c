#include <stdio.h>
#include <sys/resource.h>
#include <stdarg.h> 

int main() {
  struct rlimit lim;
  if (getrlimit(RLIMIT_STACK, &lim) == -1) { 
    exit(-1);
  } else { 
    printf("stack size: %ld\n", (long) lim.rlim_cur);
  }
  if (getrlimit(RLIMIT_NPROC, &lim) == -1) { 
    exit(-1);
  } else { 
    printf("process limit: %ld\n", (long) lim.rlim_cur);
  }
  if (getrlimit(RLIMIT_NOFILE, &lim) == -1) { 
    exit(-1);
  } else { 
    printf("max file descriptors: %ld\n", (long) lim.rlim_cur);
  }
  return 0;
}
