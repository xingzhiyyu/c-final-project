#include <windows.h>
#include <stdio.h>
#include <string.h>

int is_txt_file(const char* files) {
	int res = _stricmp(files, "txt");
	return res;
}