#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
int is_txt_file(const char* files) {
	int res = _stricmp(files, "txt");
	return res;
}
int createfile_archive(TCHAR* username, struct GlobalConfig* cfg, int data) {
    if (cfg == NULL || username == NULL) return -1;

    TCHAR filename[256];
    int i = 0;
    FILE* file = NULL;
    char content[512];

    while (1) {
        if (i == 0)
            _stprintf(filename, _T("%s.txt"), username);
        else
            _stprintf(filename, _T("%s_%d.txt"), username, i);

        file = _tfopen(filename, _T("r"));
        if (file == NULL) break;

        fclose(file);
        i++;
    }

    file = _tfopen(filename, _T("w"));
    if (file == NULL) {
        perror("Failed to create file");
        return -1;
    }

    _ftprintf(file, _T("%s\n"), username);

    snprintf(content, sizeof(content),
        "%d,%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %d",
        data, cfg->gravity, cfg->thrust, cfg->v_term_up, cfg->v_term_down,
        cfg->energy_max, cfg->energy_consume, cfg->energy_rec_fall,
        cfg->energy_rec_floor, cfg->energy_restart_p, cfg->obstacle_speed);

    fputs(content, file);

    fclose(file);
    return 1;
}