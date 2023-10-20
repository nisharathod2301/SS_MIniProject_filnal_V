/*
============================================================================
Name : common.c
Author : Nisha Rathod
Description : Academia portal
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "common.h"

int getSuffix(char input_id[]) {
    int numeric_part;
    if(sscanf(input_id + 2, "%d", &numeric_part) == 1) {
        return numeric_part;
    } else {
        return -1;
    }
}

int isFacultyEmpty(struct Faculty faculty) {
    return (memcmp(&faculty, &(struct Faculty){0}, sizeof(struct Faculty)) == 0);
}

int isStudentEmpty(struct Student student) {
    return (memcmp(&student, &(struct Student){0}, sizeof(struct Student)) == 0);
}

int isCourseEmpty(struct Course course) {
    return (memcmp(&course, &(struct Course){0}, sizeof(struct Course)) == 0);
}

int isPasswordValid(char password[]) {
    int alpha = 0;
    int digit = 0;
    int special = 0;
    int n = strlen(password);
    if (n < 5 || n > 20) {
        return -1;
    }
    for (int i = 0; i < n; i++) {
        if ('a' <= password[i] && password[i] <= 'z') {
            alpha++;
        } else if ((0 <= (password[i] - '0')) && ((password[i] - '0') <= 9)) {
            digit++;
        } else if (password[i] == '#' || password[i] == '$' || password[i] == '@') {
            special++;
        }
    }
    if (alpha == 0) {
        return -2;
    } else if (digit == 0) {
        return -3;
    } else if (special == 0) {
        return -4;
    }
    return 0;
}
