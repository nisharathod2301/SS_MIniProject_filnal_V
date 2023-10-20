/*
============================================================================
Name : student-ops.h
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#ifndef STUDENT_OPS
#define STUDENT_OPS

void handle_student_operations(int socket_fd, char login_id[]);
int deEnrollStudentInCourse(char student_id[], char course_id[]);

#endif
