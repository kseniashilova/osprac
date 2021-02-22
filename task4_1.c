#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fd, result;
  size_t  size;
  char    resstring[14];
  char    name[] = "hello.fifo";

  (void)umask(0);

  if (mknod(name, S_IFIFO | 0666, 0) < 0) {
    printf("Can\'t create FIFO\n");
    exit(-1);
  }
  if ((fd = open(name, O_WRONLY)) < 0) {
      printf("Can\'t open FIFO for writting\n");
      exit(-1);
    }

    size = write(fd, "Hello, world!", 14);

    if (size != 14) {
      printf("Can\'t write all string to FIFO\n");
      exit(-1);
    }
    printf("First program wrote string succesfully\n");

    if (close(fd) < 0) {
      printf("first program: Can\'t close FIFO\n"); exit(-1);
    }

  return 0;
}
