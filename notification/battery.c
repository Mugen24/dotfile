#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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
    while (fread(my_string, sizeof(char), size, fp) == size){

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
    
//Level = urgency 1 to 3. 3 being the most urgent
void notify(char *text, int level){
    system("systemctl --user import-environment DISPLAY WAYLAND_DISPLAY SWAYSOCK");
    //char notify[] = "notify-send";
    //putenv("DBUS_SESSION_BUS_ADDRESS=unix:path=/run/dbus/system_bus_socket");
    char notify[50];
    if (level == 1){
        strcpy(notify, "notify-send -t 2000 -u low");
    }

    else if (level == 2){
        strcpy(notify, "notify-send -t 2000 -u normal");
    }

    else if (level == 3){
        strcpy(notify, "notify-send -t 2000 -u critical");
    }
    char *notify_string = malloc(sizeof(char) * (strlen(notify) + strlen(text) + 1 + 1));
    notify_string[0] = 0;
    strcat(notify_string, notify);
    strcat(notify_string, " ");
    strcat(notify_string, text);
    puts(notify_string);
    system(notify_string);
    free(notify_string);
}

//https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
int exists(char *path){
    FILE *file;
    if ((file = fopen(path, "r"))){
        fclose(file);
        return 1;
    }
    return 0;
}

int main(){
    Battery_stat my_battery = init_battery();
    //for converting int to str
    char str[5];

    reload_battery(my_battery);
    char *status = "to init for no error";
    char *bat_path = "/tmp/battery_status";
    if (exists(bat_path)){
        status = get_content(bat_path);
        rstrip(status);
    }
    else {
        char command[100] = "echo ";
        strcat(command, my_battery->status);
        strcat(command, " > ");
        strcat(command, bat_path);
        system(command);
        exit(1);
    }


    int length = snprintf(NULL, 0, "%d", my_battery->battery_level);
    snprintf( str, length + 1 , "%d", my_battery->battery_level);

    if (my_battery->battery_level < 10){
        if (strcmp(my_battery->status, "Discharging") == 0){
            notify(my_battery->status, 3);
            notify(str, 3);
        }
    }

    //Notify when plug or unpluged

    puts(my_battery->status);
    puts(status);
    if (strcmp(my_battery->status, status) != 0){
        notify(my_battery->status, 1);
        notify(str, 1);
        
        //Note the changes to /tmp/battery_status
        char command[100] = "echo ";
        strcat(command, my_battery->status);
        strcat(command, " > ");
        strcat(command, bat_path);
        system(command);
    }

    free_bat(my_battery);
    free(status);
    
    return 0;
}
