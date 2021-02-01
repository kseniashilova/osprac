#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid;

  pid = fork();
  
if(pid == -1){
	printf("ERROR of creating process\n");
}
else if (pid == 0){
	printf("Hi, I am a child process\n");
}
else {
	printf("Hello, I am a parent process\n");
}

  return 0;
}
