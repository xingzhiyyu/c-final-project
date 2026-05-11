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
        return -1;
    }

    // 分数
    _ftprintf(file, _T("%d\n"), data);

    // 用户名
    _ftprintf(file, _T("%s\n"), username);

    // 配置数据
    _ftprintf(file,
        _T("%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %d\n"),
        cfg->gravity,
        cfg->thrust,
        cfg->v_term_up,
        cfg->v_term_down,
        cfg->energy_max,
        cfg->energy_consume,
        cfg->energy_rec_fall,
        cfg->energy_rec_floor,
        cfg->energy_restart_p,
        cfg->obstacle_speed
    );

    fclose(file);
    return 1;
}
int search_archive(TCHAR* username) {
    int tryNum = 0;
    TCHAR filename[256];
    TCHAR data[20];
    int found = 0;
    int maxNumber = 0;
    do {
        if (tryNum == 0)
            _stprintf(filename, _T("%s.txt"), username);
        else
            _stprintf(filename, _T("%s_%d.txt"), username, tryNum);
        FILE* file = _tfopen(filename, _T("r"));
        if (file == NULL) {
            break;
        }
        if (_fgetts(data, 20, file) != NULL) {

            int num;
            if (_stscanf(data, _T("%d"), &num) == 1) {
                if (num > maxNumber)
                    maxNumber = num;
                found = 1;
            }
            fclose(file);
        }tryNum++;
    } while (1);
            
        
	
    return found ? maxNumber : -1;

}