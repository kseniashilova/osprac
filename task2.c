#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int fd1[2], fd2[2], result;

  size_t size;
  char  new_string_parent[17];
  char  new_string_child[18];

//пытаемся открыть пайп
  if (pipe(fd1) < 0) {
    printf("Can\'t open FIRST pipe\n");
    exit(-1);
  }

  if (pipe(fd2) < 0) {
    printf("Can\'t open SECOND pipe\n");
    exit(-1);
  }
//FORK
  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

//PARENT - пишет в fd1, читает из fd2
//ЗАПИСЬ
//закрывает чтение для fd1
    if (close(fd1[0]) < 0) {
      printf("parent: Can\'t close reading side of FIRST pipe\n"); 
      exit(-1);
    }
//пишет с помощью первого пайпа строку
    size = write(fd1[1], "Hello from parent", 18);
//проверяет результат
    if (size != 18) {
      printf("Can\'t write all string to FIRST pipe\n");
      exit(-1);
    }
//Закрывает запись для fd1
    if (close(fd1[1]) < 0) {
      printf("parent: Can\'t close writing side of FIRST pipe\n"); exit(-1);
    }

//ЧТЕНИЕ
//закрывает запись в fd2
if (close(fd2[1]) < 0) {
      printf("parent: Can\'t close writing side of SECOND pipe\n"); 
	exit(-1);
    }
//читает из fd2
    size = read(fd2[0], new_string_parent, 17);
//проверяет результат
    if (size < 0) {
      printf("Can\'t read string from SECOND pipe\n");
      exit(-1);
    }

    printf("Parent exit, parent read:%s\n", new_string_parent);
//закрывает чтение
    if (close(fd2[0]) < 0) {
      printf("parent: Can\'t close reading side of SECOND pipe\n"); 
      exit(-1);
    }
	

  } else {

    //CHILD - пишет в fd2, читает из fd1

//ЗАПИСЬ
//закрывает чтение для fd2
    if (close(fd2[0]) < 0) {
      printf("child: Can\'t close reading side of SECOND pipe\n"); 
      exit(-1);
    }
//пишет с помощью второго пайпа строку
    size = write(fd2[1], "Hello from child", 17);
//проверяет результат
    if (size != 17) {
      printf("Can\'t write all string to SECOND pipe\n");
      exit(-1);
    }
//Закрывает запись для fd2
    if (close(fd2[1]) < 0) {
      printf("child: Can\'t close writing side of SECOND pipe\n"); 
      exit(-1);
    }

//ЧТЕНИЕ
//закрывает запись в fd1
if (close(fd1[1]) < 0) {
      printf("child: Can\'t close writing side of FIRST pipe\n"); 
	exit(-1);
    }
//читает из fd1
    size = read(fd1[0], new_string_child, 18);
//проверяет результат
    if (size < 0) {
      printf("Can\'t read string from FIRST pipe\n");
      exit(-1);
    }

    printf("Child exit, child read:%s\n", new_string_child);
//закрывает чтение
    if (close(fd1[0]) < 0) {
      printf("child: Can\'t close reading side of FIRST pipe\n"); 
      exit(-1);
    }
	

  }

  return 0;
}
