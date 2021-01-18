#include <stdlib.h>
#include <stdio.h>
int main(){
printf("User:  %d\n", getuid());
printf("Group:  %d\n", getgid());
return 0;
}
