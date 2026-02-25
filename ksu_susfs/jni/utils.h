#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <android/log.h>
#include <sys/stat.h>
#include <ctype.h>

int isNumeric(char* str);
int get_file_stat(char *pathname, struct stat* sb);
void print_help_banner(void);









#endif
