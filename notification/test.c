#include <stdio.h>
#include <string.h>
void rstrip(char *string){
    unsigned long len = strlen(string);
    for (int i = len - 1; i >= 0; i--){
        if (string[i] != '\n'){
            printf("what?: ");
            printf("%d\n", string[i]);
            break;
        }
        else {
            string[i] = 0;
        }
    }
}

int main(){
    FILE *ptr;
    char buffer[100];

    ptr = fopen("test.txt", "r");

    if (NULL == ptr){
        printf("smth wrong");
    }

    while (!feof(ptr)){
        fread(buffer, sizeof(buffer), 1, ptr);
    }
    printf("%s", buffer);
    for (int i = 0; i < strlen(buffer); i++){
        printf("%c ", buffer[i]);
    }
    rstrip(buffer);
    printf("%s", buffer);
    fclose(ptr);
    return 1;
}
