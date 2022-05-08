#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main(){
    char *a = "whatr";
    char *b = a;
    printf("%p\n", a);
    printf("%p\n", b);
}
