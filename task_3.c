/*КОММЕНТАРИЙ
P = D(S, 1)
V = A(S, 1)
Начальное значение семафора = 0

После записи блокируем процесс с помощью P (то есть D), 
чтобы нельзя было считать сообщение от самого себя
После прочтения будем азблокировать для записи другого процесса
с помощью V (то есть A)

То есть:
write() -> D
read() -> A
*/





#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <stdlib.h>

int P(int semid, struct sembuf *buf) {
   
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
}

int V(int semid, struct sembuf *buf) {
   
    buf->sem_op = 1;
    buf->sem_flg = 0;
    buf->sem_num = 0;

    return semop(semid, buf, 1);
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
    int N;


    if (pipe(fd) < 0) {
        
        printf("Can\'t create pipe\n");
        exit(-1);
    }

    key_t key; 
    char pathname[] = "task_3.c";
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0) {
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t get semid\n");
            exit(-1);
        }
        printf("Semaphore is created\n");
    }
    //читаем число
    printf("(N >= 2) N = ? \n");
    scanf("%d", &N);

    result = fork();

    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {
        
       for (int i = 0; i < N; ++i){
        size = write(fd[1], resstring, 14);
        
        if (size != 14) {
            printf("Can\'t write all string\n");
            exit(-1);
        } else {
            printf("Parent wrote string.\n");
        }
        //P
         P(semid, &mybuf);

        size = read(fd[0], resstring, 14);

        if (size < 0) {
            printf("Can\'t read string\n");
            exit(-1);
        }
        printf("Parent read string: %s\n", resstring);
        //V
        V(semid, &mybuf);
       }
       close(fd[1]);
       close(fd[0]);
       printf("Parent exit\n");
    } else {
       for(int i = 0; i < N; i++){
        size = read(fd[0], resstring, 14);

        if (size < 0) {
            printf("Can\'t read string\n");
            exit(-1);
        }
        printf("Child read string: %s\n", resstring);

        //V - разблокируем
        V(semid, &mybuf);
        change_str(resstring, 13);
        size = write(fd[1], resstring, 14);


        if (size != 14) {
            printf("Can\'t write all string\n");
            exit(-1);
        } else {
            printf("Child wrote string\n");
        }
//P - блокируем, чтобы нельзя было считать себя
        P(semid, &mybuf);
       }
       close(fd[1]);
       close(fd[0]);
       printf("Child exit\n");
    }

    return 0;
}
