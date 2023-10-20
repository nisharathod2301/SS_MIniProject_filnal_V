/*
============================================================================
Name : common.h
Author : Nisha Rathod
Description : Academia portal
============================================================================
*/

#ifndef COMMONS
#define COMMONS

#include "../model/faculty.h"
#include "../model/student.h"
#include "../model/course.h"

int getSuffix(char input_id[]);
int isStudentEmpty(struct Student student);
int isFacultyEmpty(struct Faculty faculty);
int isCourseEmpty(struct Course course);
int isPasswordValid(char password[]);

#endif
