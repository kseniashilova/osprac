#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  int     shmid;
  char    pathname[] = "task1_write.c";
  key_t   key;

  char* my_program;
  int file_size = 0;
  int* ptr;


  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((shmid = shmget(key, sizeof(int) + sizeof(char)*file_size, 0)) < 0) {
 
      printf("Can\'t create shared memory\n");
      exit(-1);
  }
  if((ptr = (int *)shmat(shmid, NULL, 0))==
          (int *)(-1)){
       printf("Can't attach shared memory\n");
       exit(-1);
  }
  file_size = *ptr;
  my_program = (char *)(ptr + 1);

  for(int i = 0; i < file_size; i++){
      putchar(my_program[i]);
  }

  if (shmdt(ptr) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  //удаление
  if(shmctl(shmid, IPC_RMID, NULL) < 0){
    printf("Can't delete shared memory\n");
    exit(-1);
  }
  return 0;
}
