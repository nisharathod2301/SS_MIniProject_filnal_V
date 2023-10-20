/*
============================================================================
Name : faculty-dao.h
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#ifndef FACULTY_DAO
#define FACULTY_DAO

#include "../model/faculty.h"
#include "../commons/common.h"

int insertFaculty(char name[], char email[], char dept[]);
struct Faculty getFacultyDetails(char facultyno_string[]);

struct Faculty updateFacultyName(int facultyno, char name[]);
struct Faculty updateFacultyEmail(int facultyno, char email[]);
struct Faculty updateFacultyDepartment(int facultyno, char dept[]);
struct Faculty updateFacultyPassword(int facultyno, char password[]);

bool isFacultyAuthenticated(char login_id[], char password[]);

#endif
