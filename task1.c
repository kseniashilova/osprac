#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

//Для файла path (длина названия - path_len) создаёт максимально возможное количество мягких
//связей (на выходе значение будет сохранено в recursion_depth).
void create_links(const char *const path, int path_len, int *const recursion_depth) {
    //Имя новой ссылки.
    char *new_path = (char *) malloc((path_len + 2) * sizeof(char));
    strcpy(new_path, path);
    //Увеличиваем последний символ, пока он меньше 'z' (или 'Z').
    //В противном случае, добавляем новый символ 'a'.
    if (new_path[path_len - 1] == 'z' || new_path[path_len - 1] == 'Z') {
        strcat(new_path, "a");
        path_len++;
    } else {
        new_path[path_len - 1]++;
    }
    //Если не удалось создать новую ссылку, запоминаем, что глубина рекурсии 0 и
    //возвращаемся в вверх по рекурсии.
    if (symlink(path, new_path) == -1) {
        *recursion_depth = 0;
        return;
    }
    //Пытаемся создать новые ссылки.
    create_links(new_path, path_len, recursion_depth);
    //После этих попыток увеличиваем на 1 глубину рекурсии.
    (*recursion_depth)++;
    //Закрываем ссылку.
    unlink(new_path);
    //Освобождаем динамически выделенную память.
    free(new_path);
}

int main() {
    //Ссылка на изначальный файл.
    FILE *f;
    //Переменная для глубины рекурсии.
    int recursion_depth;

    //Открываем изначальный файл для записи (то есть создаём при отсутствии).
    if ((f = fopen("a", "w")) == NULL) {
        printf("Problems with creating original file. ");
        exit(-1);
    }
    //Пишем в файл какие-либо данные (опционально).
    if (fputs("Original file", f) == EOF) {
        printf("Problems with writing to original file. ");
        exit(-1);
    }

    //Запускаем функцию создания мягких ссылок.
    create_links("a", 1, &recursion_depth);
    //Выводим получившееся значение глубины рекурсии.
    printf("Recursion depth: %d", recursion_depth);

    //Закрываем файл.
    if (fclose(f) == EOF) {
        printf("Problems with closing original file. ");
        exit(-1);
    }
    return 0;
}
