/*
============================================================================
Name : student-dao.h
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#ifndef STUDENT_DAO
#define STUDENT_DAO

#include "../model/student.h"
#include "course-dao.h"
#include "../model/student.h"
#include "../commons/common.h"

int insertStudent(char name[], int age, char email[]);
struct Student getStudentDetails(char rollno_string[]);

struct Student updateStudentName(int rollno, char name[]);
struct Student updateStudentAge(int rollno, int age);
struct Student updateStudentEmail(int rollno, char email[]);
struct Student updateStudentPassword(int rollno, char password[]);
bool isStudentActivated(char student_id[]);

bool isStudentAuthenticated(char login_id[], char password[]);

#endif
