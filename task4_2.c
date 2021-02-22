#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

int     fd, result;

  size_t  size;
  char    resstring[14];
  char    name[] = "hello.fifo";

  (void)umask(0);

    if ((fd = open(name, O_RDONLY)) < 0) {
      printf("Can\'t open FIFO for reading\n");
      exit(-1);
    }

    size = read(fd, resstring, 14);

    if (size < 0) {
      printf("Can\'t read string from FIFO\n");
      exit(-1);
    }

    printf("Second program read string: %s\n", resstring);

    if (close(fd) < 0) {
      printf("second program: Can\'t close FIFO\n"); exit(-1);
    }
   return 0;
}
