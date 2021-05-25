#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>

char *change_prev_path(char *prev_path, int *const prev_path_len,
                       const char *const new_path, const int *const new_path_len) {
    if (*prev_path_len != *new_path_len) {
        *prev_path_len = *new_path_len;
        free(prev_path);
        prev_path = (char *) malloc((*prev_path_len + 1) * sizeof(char));
    }
    for (int i = 0; i < *prev_path_len + 1; i++) {
        prev_path[i] = new_path[i];
    }
    return prev_path;
}

char *change_path(char *path, int *const path_len) {
    int i;
    for (i = *path_len - 1; i > -1; i--) {
        if (path[i] == 'z') {
            path[i] = 'a';
        } else {
            path[i]++;
            break;
        }
    }
    if (i == -1) {
        (*path_len)++;
        char *new_path = (char *) malloc((*path_len + 1) * sizeof(char));
        for (int j = 0; j < *path_len - 1; j++) {
            new_path[j] = path[j];
        }
        new_path[*path_len - 1] = 'a';
        new_path[*path_len] = '\0';
        free(path);
        path = new_path;
        new_path = NULL;
    }
    return path;
}

int main() {
    int fd;

    if ((fd = open("a", O_CREAT | O_WRONLY, 0666)) < 0) {
        printf("Problems with creating original file. ");
        exit(-1);
    }
    if (write(fd, "Original file", 13) < 13) {
        printf("Problems with writing to original file. ");
        exit(-1);
    }
    if (close(fd) < 0) {
        printf("Problems with closing original file. ");
        exit(-1);
    }

    int prev_path_len = 1;
    char *prev_path = (char *) malloc(2 * sizeof(char));
    prev_path[0] = 'a';
    prev_path[1] = '\0';
    int new_path_len = 1;
    char *new_path = (char *) malloc(2 * sizeof(char));
    new_path[0] = 'b';
    new_path[1] = '\0';

    int recursion_depth;
    for (recursion_depth = 0;; recursion_depth++) {
        if (symlink(prev_path, new_path) < 0) {
            printf("Problems with creating a new soft link. ");
            free(prev_path);
            free(new_path);
            exit(-1);
        }

        if ((fd = open(new_path, O_WRONLY, 0666)) < 0) {
            break;
        }
        if (close(fd) < 0) {
            printf("Problems with closing the new soft link. ");
            free(prev_path);
            free(new_path);
            exit(-1);
        }
        prev_path = change_prev_path(prev_path, &prev_path_len, new_path, &new_path_len);
        new_path = change_path(new_path, &new_path_len);
    }

    printf("Recursion depth: %d", recursion_depth);
    free(prev_path);
    free(new_path);
    return 0;
}