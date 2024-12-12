#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_dynamic_string(char **str) {
    if(*str != NULL) free(*str);
    int capacity = 10;
    int length = 0;
    char ch;
    *str = malloc(capacity * sizeof(char));
    if(*str == NULL) return;

    while((ch = getchar()) != '\n'){
        if(length + 1 >= capacity){
            capacity += 10;
            char *new_str = realloc(*str, capacity * sizeof(char));
            if(new_str == NULL){
                free(*str);
                *str = NULL;
                return;
            }
            *str = new_str;
        }
        (*str)[length++] = ch;
    }
    (*str)[length] = '\0';
}

int main() {
    char *str = NULL;
    printf("Enter a string: ");
    read_dynamic_string(&str);
    if(str != NULL) {
        printf("You entered: %s\n", str);
        // free(str);
    }
    read_dynamic_string(&str);
    if(str != NULL) {
        printf("You entered: %s\n", str);
        // free(str);
    }
    return 0;
}
