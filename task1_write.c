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
//открытие
  FILE *file;
  file = fopen(pathname, "r");
//указатель на конец файла
  fseek(file, 0L, SEEK_END);
  file_size = ftell(file);
//возврат на начало
  rewind(file);



  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((shmid = shmget(key, 
      sizeof(int) + sizeof(char)*file_size, 
      0666|IPC_CREAT)) < 0) {
 
      printf("Can\'t create shared memory\n");
      exit(-1);
  }
  if((ptr = (int *)shmat(shmid, NULL, 0))==
          (int *)(-1)){
       printf("Can't attach shared memory\n");
       exit(-1);
  }
//сохранение в shared memory
  *ptr = file_size;
  my_program = (char *)(ptr + 1);

  for(int i = 0; i < file_size; i++){
      my_program[i] = fgetc(file);
  }

  fclose(file);

  if (shmdt(ptr) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
