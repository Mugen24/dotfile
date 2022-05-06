#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#define BATTERY_PATH "/sys/class/power_supply/BAT0"

struct _battery_stat {
    char status[20];
    unsigned int battery_level;
};
typedef struct _battery_stat * Battery_stat;

/**
 *  * Get the size of a file.
 *   * @return The filesize, or 0 if the file does not exist.
 *    */
size_t getFilesize(const char* filename) {
        struct stat st;
        if(stat(filename, &st) != 0) {
                    return 0;
                        
        }
        return st.st_size;   
            
}

void rstrip(char *string){
    unsigned long len = strlen(string);
    for (int i = len - 1; i >= 0; i--){
        if (string[i] == '\n' || string[i] == ' '){
            string[i] = 0;
        }
        else {
            break;
        }
    }
}

char *get_content(char *file_path){
    FILE *fp;
    fp = fopen(file_path, "rb");
    if ( fp == NULL ){
        perror("Error: ");
    }
    unsigned int size = (unsigned int)getFilesize(file_path);
    char *my_string = malloc(sizeof(char) * size + 1);
    while (fread(my_string, sizeof(char), size/2, fp) == size/2){

    }
    fclose(fp);
    return my_string;
}


Battery_stat init_battery(){
    Battery_stat temp;   
    temp = malloc(sizeof(struct _battery_stat));
    return temp;
}

int main(){
    char prefix[] = "/capacity";
    char capacity[strlen(BATTERY_PATH) + strlen(prefix) + 1];
    strcat(capacity, BATTERY_PATH);
    strcat(capacity, prefix);
    char *cap = get_content(capacity);
    rstrip(cap);
    printf("%s\n", cap);
    free(cap);
    return 0;
}
