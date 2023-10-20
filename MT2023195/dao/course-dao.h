/*
============================================================================
Name : course-dao.h
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#ifndef COURSE_DAO
#define COURSE_DAO

#include "../model/course.h"
#include "../model/student-course.h"
#include "../commons/common.h"
#include "../module/student-ops.h"

int insertCourse(char faculty_id[], char name[], int total_seats, int credits);
bool isCourseIDValid(char course_id[]);
struct Course doesCourseExist(char course_id[]);
struct Course isCourseOfferedByFaculty(char course_id[], char faculty_id[]);
struct Course* findCoursesByFacultyId(char* course_id, const char* faculty_id, int* num_matches);
void insertStudentInAllCourses(char student_id[]);
bool isCourseActivated(char course_id[]);
struct Course* getAllActivatedCourses(int* num_matches);
struct Course* getAllEnrolledCourses(char student_id[], int* num_matches);
void deEnrollStudents(int reduced_seats, char course_id[]);
void deEnrollAllStudentsInCourse(char course_id[]);

#endif
