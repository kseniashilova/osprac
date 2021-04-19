/*КОММЕНТАРИЙ
Начальное значение семафора = 0

Изначально после запуска child выполняется D(s, 1)
После того, как parent записал строку, 
оповещаем child и блокируем
То естm A(s, 2) и Z(s)

Child оповещается и читает строку
затем пишет и блокируется с помощью D(s, 1)

Parent ждал -> читает и пишет и тд

Итого:
начальное значение sem = 0
parent: write, A,  Z, read
child: D, read, write, D
*/





#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <stdlib.h>

void D(int semid, int n) {
   
    struct sembuf buf;
    buf.sem_op = -n;
    buf.sem_flg = 0;
    buf.sem_num = 0;

    semop(semid, &buf, 1);
}

void A(int semid, int n) {
   
    struct sembuf buf;
    buf.sem_op = n;
    buf.sem_flg = 0;
    buf.sem_num = 0;

    semop(semid, &buf, 1);
}

void Z(int semid) {
   
    struct sembuf buf;
    buf.sem_op = 0;
    buf.sem_flg = 0;
    buf.sem_num = 0;

    semop(semid, &buf, 1);
}

void change_str(char* str, size_t sz) {
     if(str[sz - 1] == '!'){
         str[sz - 1] = '?';
     }
     else{
          str[sz - 1] = '!';
     }   
}

int main() {

    int fd[2], result;
    size_t size;
    char resstring[14] = "Hello, world!";
    struct sembuf mybuf;
    int semid;

    key_t key; 
    char pathname[] = "task_3.c";

    int N;
    //читаем число
    do{
      printf("(N >= 2) N = ? \n");
      scanf("%d", &N);
    } while(N < 2);

 

    if (pipe(fd) < 0) {
        printf("Can\'t create pipe\n");
        exit(-1);
    }

    if ((semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get semid\n");
        exit(-1);
    }
    

    result = fork();

    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {
        //Parent
       for (int i = 0; i < N; ++i){
        size = write(fd[1], resstring, 14);
        
        if (size != 14) {
            printf("Can\'t write all string\n");
            exit(-1);
        } else {
            printf("Parent wrote string. (%s)\n", resstring);
        }
        //A,Z
        A(semid, 2);
        Z(semid);

        size = read(fd[0], resstring, 14);

        if (size < 0) {
            printf("Can\'t read string\n");
            exit(-1);
        }
        printf("Parent read string: %s\n", resstring);
        
       }
       close(fd[0]);
       close(fd[1]);
       printf("Parent exit\n");
    } else {
       //Child
       for(int i = 0; i < N; i++){
       D(semid, 1);
       size = read(fd[0], resstring, 14);

       if (size < 0) {
            printf("Can\'t read string\n");
            exit(-1);
       }
       printf("Child read string: %s\n", resstring);
       //Изменяем строку на (последний символ)
       change_str(resstring, 13);

       size = write(fd[1], resstring, 14);

       if (size != 14) {
            printf("Can\'t write all string\n");
            exit(-1);
       } else {
            printf("Child wrote string. (%s)\n", resstring);
       }
      
       D(semid, 1);
       }
       close(fd[0]);
       close(fd[1]);
       printf("Child exit\n");
    }

    return 0;
}
