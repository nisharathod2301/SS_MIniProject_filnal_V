/*
============================================================================
Name : student.h
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#ifndef STUDENT_MODEL
#define STUDENT_MODEL

struct Student {
    char std_id[6]; 
    char name[50];
    int age;
    char email[50];
    char password[50];
    int no_of_courses_enrolled;
    bool isActivated;
};

#endif
