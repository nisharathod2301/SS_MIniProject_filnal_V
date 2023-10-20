/*
============================================================================
Name : student-course.h
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#ifndef STUDENT_COURSE_MODEL
#define STUDENT_COURSE_MODEL

struct Student_Course {
    char std_id[6]; 
    char course_id[6];
    bool hasEnrolled;
    time_t enrolledTimestamp;
};

#endif
