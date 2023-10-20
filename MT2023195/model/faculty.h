/*
============================================================================
Name : faculty.h
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#ifndef FACULTY_MODEL
#define FACULTY_MODEL

struct Faculty {
    char faculty_id[6]; 
    char name[50];
    char email[50];
    char dept[50];
    char password[50];
};

#endif
