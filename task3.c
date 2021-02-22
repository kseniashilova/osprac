#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
  int pipe_size = 0;
  int     fd[2];
  size_t size;
  char* str = "1";
  //пробуем открыть пайп
  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }


  fcntl(fd[1], F_SETFL, O_NONBLOCK);
  size = 1;
  while(size == 1){
     size = write(fd[1], str, 1);
     pipe_size++;
  }
  printf("pipe size: %d\n", pipe_size - 1);

  return 0;
}
