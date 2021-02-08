#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{

pid_t pid;

  pid = fork();
  
if(pid == -1){
	printf("ERROR of creating process\n");
}
else if (pid == 0){
	printf("Hi, I am a child process\n");
	printf("I will type 10 lines of task_4.c program (head command): \n");
	(void) execle("/bin/head", "/bin/head", "task_4.c", 0, envp);
	printf("Error on program start\n");
	exit(-1);
}
else {
	printf("Hello, I am a parent process\n");
}
  return 0;  
}
