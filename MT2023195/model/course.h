/*
============================================================================
Name : course.h
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#ifndef COURSE_MODEL
#define COURSE_MODEL

struct Course {
    char course_id[6];
    char name[50];
    int credits;
    int total_seats;
    int filled_seats;
    char faculty_id[6]; 
    bool isActivated;
};

#endif
