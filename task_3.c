#include <stdlib.h>
#include <stdio.h>
int main(int argc, char *argv[], char *envp[]){
//значения argv
printf("argv: program name = %s\n", argv[0]);
for(int i = 1; i < argc; i++){
	printf("argv [%d] = %s\n", i, argv[i]);
}
//значения envp
int i = 0;
while(envp[i] != NULL){
	printf("envp[%d] = %s\n", i, envp[i]);
	i++;
}
return 0;
}
