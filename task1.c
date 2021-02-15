#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
  int     fd;
  size_t  size;
  char    string[] = "Hello from Ksenia\n";
  char new_string[18];
  
  (void)umask(0);
  
//Открытие файла
  if ((fd = open("myfile.txt", O_WRONLY | O_CREAT, 0666)) < 0) {
    printf("Error of opening file\n");
    exit(-1);
  }
//Запись строки в файл (существующий или нет)
  size = write(fd, string, 18);

//Проверка результата
  if (size != 18) {
    printf("Error of writing all string\n");
    exit(-1);
  }

if (close(fd) < 0) {
    printf("Error of closing file\n");
  }

//Открытие файла
  if ((fd = open("myfile.txt", O_RDONLY, 0666)) < 0) {
    printf("Error of opening file\n");
    exit(-1);
  }
//Считывание из файла
  size = read(fd, new_string, 18);

//Проверка результата считывания
  if (size < 0) {
    printf("Error of reading all string\n");
    exit(-1);
  }
//Печатаем на экран
  printf("Read string: ");
  for(int i = 0; i < 18; i++){
	printf("%c", new_string[i]);
}
//printf("%s", new_string);
printf("size: %d\n", size);
  if (close(fd) < 0) {
    printf("Error of closing file\n");
  }

  return 0;
}
