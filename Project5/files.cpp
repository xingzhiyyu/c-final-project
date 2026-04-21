#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
int is_txt_file(const char* files) {
	int res = _stricmp(files, "txt");
	return res;
}
int createfile_archive(const char* text, struct GlobalConfig *cfg,int data) {
    char filename[256];
    int i = 0;
    FILE* file;
	char content[512];
    while (1) {
        if (i == 0)
            snprintf(filename, sizeof(filename), "userarchive");
        else
            snprintf(filename, sizeof(filename), "userarchive%d", i);

        file = fopen(filename, "r");
        if (file == NULL) {
            break;
        }
        fclose(file); 
        i++;
    }
    file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to create file");
        return -1; 
    }
     sprintf(content,
        "%d,%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %d",
        data,cfg->gravity, cfg->thrust, cfg->v_term_up, cfg->v_term_down,
        cfg->energy_max, cfg->energy_consume, cfg->energy_rec_fall, cfg->energy_rec_floor, cfg->energy_restart_p,
        cfg->obstacle_speed);
     fputs(content, file);
     fclose(file);
     return 1;
}