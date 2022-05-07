#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
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
        printf("%s\n", file_path);
        perror("Error: ");
        exit(0);
    }
    unsigned int size = (unsigned int)getFilesize(file_path);
    char *my_string = calloc(sizeof(char) * size + 1, 1);
    if (my_string == NULL){
        puts("Not enough space");
        exit(-1);
    }
    while (fread(my_string, sizeof(char), size/2, fp) == size/2){

    }
    fclose(fp);
    return my_string;
}


Battery_stat init_battery(){
    Battery_stat my_battery;
    my_battery = malloc(sizeof(struct _battery_stat));

    return my_battery;
}

void reload_battery(Battery_stat my_battery){
    char prefix[][20] = {"/capacity", "/status"};
    const unsigned int len = sizeof(prefix) / sizeof(prefix[0]);
    for (int stat = 0; stat < len; stat++){
        char capacity[strlen(BATTERY_PATH) + strlen(prefix[stat]) + 1];
        capacity[0] = 0;
        strcat(capacity, BATTERY_PATH);
        strcat(capacity, prefix[stat]);
        char *cap = get_content(capacity);
        rstrip(cap);
        if (strcmp(prefix[stat], "/status") == 0){
            strcpy(my_battery->status, cap);
        }
        else if (strcmp(prefix[stat], "/capacity") == 0){
            my_battery->battery_level = atoi(cap);
        }
        free(cap);
    }
}

void free_bat(Battery_stat my_battery){
    free(my_battery);
}

    //os.environ['DBUS_SESSION_BUS_ADDRESS'] = "unix:path=/run/dbus/system_bus_socket"
    //os.environ["DISPLAY"] = ":0"
    
void notify(char *text){
    system("systemctl --user import-environment DISPLAY WAYLAND_DISPLAY SWAYSOCK");
    //char notify[] = "notify-send";
    //putenv("DBUS_SESSION_BUS_ADDRESS=unix:path=/run/dbus/system_bus_socket");
    char notify[] = "notify-send";
    char *notify_string = malloc(sizeof(char) * (strlen(notify) + strlen(text) + 1 + 1));
    notify_string[0] = 0;
    strcat(notify_string, notify);
    strcat(notify_string, " ");
    strcat(notify_string, text);
    puts(notify_string);
    system(notify_string);
    free(notify_string);
}

void handler(){
    write(1, "Shutting down battery notification", 45);
}

int main(){
    signal(SIGINT, handler);
    Battery_stat my_battery = init_battery();
    char str[5];
    while(1){
        reload_battery(my_battery);
        int length = snprintf(NULL, 0, "%d", my_battery->battery_level);
        snprintf( str, length + 1 , "%d", my_battery->battery_level);
        notify(my_battery->status);
        notify(str);
        sleep(10);
    }
    free_bat(my_battery);
    
    return 0;
}
