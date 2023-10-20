/*
============================================================================
Name : student.c
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>

#include "student-ops.h"
#include "../constants/configs.h"
#include "../constants/menu.h"
#include "../constants/view-details.h"
#include "../model/student.h"
#include "../model/course.h"
#include "../model/student-course.h"
#include "../dao/course-dao.h"
#include "../dao/student-dao.h"

int deEnrollStudentInCourse(char student_id[], char course_id[]);
int enrollStudentInCourse(char student_id[], char course_id[]);

void handle_student_operations(int socket_fd, char login_id[]) {
    printf("handle_student_operations");
    char read_buffer[1000], write_buffer[1000];
    int itr = 0;

    do {
        bzero(write_buffer, sizeof(write_buffer));

        itr++;
        if (itr == 1) {
            strcpy(write_buffer, " You are successfully authenticated\n");
        }
    
        strcat(write_buffer, STUDENT_MENU);

        if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1)
            perror("Error while displaying student menu to the user");
        printf("student menu written \n");
        bzero(read_buffer, sizeof(read_buffer));
        if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
            perror("Error while reading from client");
        }
        int choice = atoi(read_buffer);
        printf("Choice: %d \n", choice);

        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));

        struct Course course;
        
        switch(choice) {
            case 1 :
                // view all active courses
                printf("View all courses \n");
                int num_matches = 0;
                struct Course* courses = getAllActivatedCourses(&num_matches);
                if (num_matches > 0) {
                    strcpy(write_buffer, "***** List of all courses : *****\n");
                    strcat(write_buffer, "                                                    \n");
                    strcat(write_buffer, " Course ID     Course Name                          \n");
                    for (int i = 0; i < num_matches; i++) {
                        char buffer[100];
                        sprintf(buffer, " %s         %s\n", courses[i].course_id, courses[i].name);
                        strcat(write_buffer, buffer);
                    }
                    strcat(write_buffer, "                                                    \n");
                    strcat(write_buffer, "\n");
                } else {
                    sprintf(write_buffer, "No courses found.\n");
                }
                free(courses);
                break;
            case 2 :
                printf("Enroll in a course \n");
                strcpy(write_buffer, "Enter the id of the course: ");
                write(socket_fd, write_buffer, strlen(write_buffer));
                read(socket_fd, read_buffer, sizeof(read_buffer));
                bzero(write_buffer, sizeof(write_buffer));
                char course_id2[50];
                strcpy(course_id2, read_buffer);

                if (isCourseIDValid(course_id2)) {
                    struct Course course = doesCourseExist(course_id2);
                    if (isCourseEmpty(course)) {
                        strcpy(write_buffer, "This Course ID is invalid. \nPlease try again with a valid one. \n");
                    } else {
                        if (isCourseActivated(course_id2)) {
                            int status = enrollStudentInCourse(login_id, course_id2);
                            if (status == -1) {
                                strcpy(write_buffer, "This Course is full. \nPlease try to enroll in other courses mentioned in choice 1. \n");
                            } else if (status == -2) {
                                strcpy(write_buffer, "You have already enrolled in this course. Choose any other course.\n");
                            } else if (status == -3) {
                                strcpy(write_buffer, "You have enrolled in 4 courses and cannot enroll in any more. \n");
                            } else {
                                strcpy(write_buffer, "***** Enrollment successful *****\n");
                            }
                        } else {
                            strcpy(write_buffer, "This Course ID is not activated. \nPlease try to enroll in the courses mentioned in choice 1. \n");
                        }
                    }
                } else {
                    strcpy(write_buffer, "Format of Course ID is incorrect. Please try again. \n");
                }
                break;
            case 3 :
                printf("View enrolled courses \n");
                num_matches = 0;
                struct Course* enrolledCourses = getAllEnrolledCourses(login_id, &num_matches);
                if (num_matches > 0) {
                    strcpy(write_buffer, "***** List of enrolled courses : *****\n");
                    strcat(write_buffer, "                                                    \n");
                    strcat(write_buffer, " Course ID     Course Name                          \n");
                    for (int i = 0; i < num_matches; i++) {
                        char buffer[100];
                        sprintf(buffer, " %s         %s\n", enrolledCourses[i].course_id, enrolledCourses[i].name);
                        strcat(write_buffer, buffer);
                    }
                    strcat(write_buffer, "                                                    \n");
                    strcat(write_buffer, "\n");
                } else {
                    sprintf(write_buffer, "No courses found.\n");
                }
                free(enrolledCourses);
                break;
            case 4 :
                printf("De-enroll in a course \n");
                strcpy(write_buffer, "Enter the id of the course: ");
                write(socket_fd, write_buffer, strlen(write_buffer));
                read(socket_fd, read_buffer, sizeof(read_buffer));
                bzero(write_buffer, sizeof(write_buffer));
                char course_id4[50];
                strcpy(course_id4, read_buffer);

                if (isCourseIDValid(course_id4)) {
                    struct Course course = doesCourseExist(course_id4);
                    if (isCourseEmpty(course)) {
                        strcpy(write_buffer, "This Course ID is invalid. \nPlease try again with a valid one. \n");
                    } else {
                        if (isCourseActivated(course_id4)) {
                            int status = deEnrollStudentInCourse(login_id, course_id4);
                            if (status == -1) {
                                strcpy(write_buffer, "You are not enrolled in this course. Choose any other course.\n");
                            } else {
                                strcpy(write_buffer, "***** De enrollment successful *****\n");
                            }
                        } else {
                            strcpy(write_buffer, "This Course ID is not activated. \nPlease try to enroll in the courses mentioned in choice 1. \n");
                        }
                    }
                } else {
                    strcpy(write_buffer, "Format of Course ID is incorrect. Please try again. \n");
                }
                break;
            case 5: 
                printf("Change password\n");
                strcpy(write_buffer, PASSWORD_INSTRUCTIONS);
                strcat(write_buffer, "Enter the new password: ");

                write(socket_fd, write_buffer, strlen(write_buffer));
                read(socket_fd, read_buffer, sizeof(read_buffer));
                bzero(write_buffer, sizeof(write_buffer));
                char password[50];
                strcpy(password, read_buffer);
                int result = isPasswordValid(password);
                if (result == -1) {
                    strcpy(write_buffer, "Length of the password must be from 5 to 20 characters long. Please try again. \n");
                } else if (result == -2) {
                    strcpy(write_buffer, "Password should have atleast one alphabet. Please try again. \n");
                } else if (result == -3) {
                    strcpy(write_buffer, "Password should have atleast one digit. Please try again. \n");
                } else if (result == -4) {
                    strcpy(write_buffer, "Password should have atleast one special character. Please try again. \n");
                } else {
                    struct Student updatedStudent = updateStudentPassword(getSuffix(login_id), password);
                    strcpy(write_buffer, "Password updated successfully\n");
                    strcat(write_buffer, "Updated Student details: ");
                    char buff[1000];
                    sprintf(buff, STUDENT_DETAILS, updatedStudent.std_id, updatedStudent.name, updatedStudent.age, updatedStudent.email, updatedStudent.password, updatedStudent.no_of_courses_enrolled, updatedStudent.isActivated ? "activated" : "de-activated");
                    strcat(write_buffer, buff);
                    strcat(write_buffer, "\n");
                }
                break;
            default :
                bzero(write_buffer, sizeof(write_buffer));
                strcpy(write_buffer, "Invalid option was selected. Please try again. \n");
                break;
        }
        strcat(write_buffer, "Do you want to continue (y/n)? : ");
        if(write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
            perror("Error while asking the client: to continue student flow");
        }
        bzero(read_buffer, sizeof(read_buffer));
        if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
            perror("Error while reading from client: to continue student flow");
        }
        printf("%s \n", read_buffer);
    } while (strcmp(read_buffer, "yes") == 0);
}

int enrollStudentInCourse(char student_id[], char course_id[]) {
    int all_courses_fd = open("/Users/nisharathod/Desktop/MT2023195/data/course.txt", O_RDWR);
    if (all_courses_fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    int courseno = getSuffix(course_id);

    struct flock flockstr1;
    flockstr1.l_whence = SEEK_SET;
    flockstr1.l_start =  ( courseno - 1 ) * sizeof(struct Course);
    flockstr1.l_len = sizeof(struct Course);
    flockstr1.l_pid = getpid();
    flockstr1.l_type = F_WRLCK;
    printf("Waiting to acquire lock on course.txt\n");
    fcntl(all_courses_fd, F_SETLKW, &flockstr1);
    printf("Acquired lock on course %s \n", course_id);

    struct Course course;
    lseek(all_courses_fd, (courseno - 1) * sizeof(struct Course), SEEK_SET);
    read(all_courses_fd, &course, sizeof(struct Course));
    int noOfTotalSeats = course.total_seats;
    int noOfFilledSeats = course.filled_seats;
    if (noOfFilledSeats == noOfTotalSeats) {
        return -1;
    }

    char path[100];
    sprintf(path, "/Users/nisharathod/Desktop/MT2023195/courses/%s.txt", course.course_id);
    int student_course_fd = open(path, O_RDWR);
    if (student_course_fd == -1) {
		perror("Error in opening the file COxxx.txt. \n");
	}

    int studentno = getSuffix(student_id);

    struct flock flockstr2;
    flockstr2.l_whence = SEEK_SET;
    flockstr2.l_start =  ( studentno - 1 ) * sizeof(struct Student_Course);
    flockstr2.l_len = sizeof(struct Student_Course);
    flockstr2.l_pid = getpid();
    flockstr2.l_type = F_WRLCK;
    printf("Waiting to acquire lock on courses/%s.txt file \n", course_id);
    fcntl(student_course_fd, F_SETLKW, &flockstr2);
    printf("Acquired lock on student %s \n", student_id);

    struct Student_Course std_course;
    lseek(student_course_fd, (studentno - 1) * sizeof(struct Student_Course), SEEK_SET);
    read(student_course_fd, &std_course, sizeof(struct Student_Course));
    if (std_course.hasEnrolled == true) {
        return -2;
    }


    int student_fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_RDWR);
    if (student_fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}

    struct flock flockstr3;
    flockstr3.l_whence = SEEK_SET;
    flockstr3.l_start =  ( studentno - 1 ) * sizeof(struct Student);
    flockstr3.l_len = sizeof(struct Student);
    flockstr3.l_pid = getpid();
    flockstr3.l_type = F_WRLCK;
    printf("Waiting to acquire lock on student.txt file \n");
    fcntl(student_fd, F_SETLKW, &flockstr3);
    printf("Acquired lock on student %s \n", student_id);

    struct Student student;
    lseek(student_fd, (studentno - 1) * sizeof(struct Student), SEEK_SET);
    read(student_fd, &student, sizeof(struct Student));
    if (student.no_of_courses_enrolled == 4) {
        return -3;
    }

    printf("Updated no of filled seats in course.txt file \n");
    lseek(all_courses_fd, -1 * sizeof(struct Course), SEEK_CUR);
    course.filled_seats = course.filled_seats + 1;
    write(all_courses_fd, &course, sizeof(struct Course));

    printf("Updated enrollment status and timestamp in COxxx.txt file \n");
    lseek(student_course_fd, -1 * sizeof(struct Student_Course), SEEK_CUR);
    time_t currentTimestamp;
    time(&currentTimestamp);
    std_course.enrolledTimestamp = currentTimestamp;
    std_course.hasEnrolled = true;
    write(student_course_fd, &std_course, sizeof(struct Student_Course));

    printf("Updated no of courses enrolled in student.txt file \n");
    lseek(student_fd, -1 * sizeof(struct Student), SEEK_CUR);
    student.no_of_courses_enrolled = student.no_of_courses_enrolled + 1;
    printf("%d", student.no_of_courses_enrolled);
    write(student_fd, &student, sizeof(struct Student));

    flockstr1.l_type = F_UNLCK;
    fcntl(all_courses_fd, F_SETLKW, &flockstr1);
    flockstr2.l_type = F_UNLCK;
    fcntl(student_course_fd, F_SETLKW, &flockstr2);
    flockstr3.l_type = F_UNLCK;
    fcntl(student_fd, F_SETLKW, &flockstr3);

    printf("All Locks released \n");

    close(all_courses_fd);
    close(student_course_fd);
    close(student_fd);
    return 0;
}

int deEnrollStudentInCourse(char student_id[], char course_id[]) {
    int all_courses_fd = open("/Users/nisharathod/Desktop/MT2023195/data/course.txt", O_RDWR);
    if (all_courses_fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    int courseno = getSuffix(course_id);

    struct flock flockstr1;
    flockstr1.l_whence = SEEK_SET;
    flockstr1.l_start =  ( courseno - 1 ) * sizeof(struct Course);
    flockstr1.l_len = sizeof(struct Course);
    flockstr1.l_pid = getpid();
    flockstr1.l_type = F_WRLCK;
    printf("Waiting to acquire lock on course.txt\n");
    fcntl(all_courses_fd, F_SETLKW, &flockstr1);
    printf("Acquired lock on course %s \n", course_id);

    struct Course course;
    lseek(all_courses_fd, (courseno - 1) * sizeof(struct Course), SEEK_SET);
    read(all_courses_fd, &course, sizeof(struct Course));

    char path[100];
    sprintf(path, "/Users/nisharathod/Desktop/MT2023195/courses/%s.txt", course_id);
    printf("%s \n", path);
    int student_course_fd = open(path, O_RDWR);
    if (student_course_fd == -1) {
		perror("Error\n");
	}

    int studentno = getSuffix(student_id);

    struct flock flockstr2;
    flockstr2.l_whence = SEEK_SET;
    flockstr2.l_start =  ( studentno - 1 ) * sizeof(struct Student_Course);
    flockstr2.l_len = sizeof(struct Student_Course);
    flockstr2.l_pid = getpid();
    flockstr2.l_type = F_WRLCK;
    printf("Waiting to acquire lock on courses/%s.txt file \n", course_id);
    fcntl(student_course_fd, F_SETLKW, &flockstr2);
    printf("Acquired lock on student %s \n", student_id);

    struct Student_Course std_course;
    lseek(student_course_fd, (studentno - 1) * sizeof(struct Student_Course), SEEK_SET);
    read(student_course_fd, &std_course, sizeof(struct Student_Course));
    if (std_course.hasEnrolled == false) {
        return -1;
    }

    int student_fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_RDWR);
    if (student_fd == -1) {
		perror("Error. \n");
	}

    struct flock flockstr3;
    flockstr3.l_whence = SEEK_SET;
    flockstr3.l_start =  ( studentno - 1 ) * sizeof(struct Student);
    flockstr3.l_len = sizeof(struct Student);
    flockstr3.l_pid = getpid();
    flockstr3.l_type = F_WRLCK;
    printf("Waiting to acquire lock on student.txt file \n");
    fcntl(student_fd, F_SETLKW, &flockstr3);
    printf("Acquired lock on student %s \n", student_id);

    struct Student student;
    lseek(student_fd, (studentno - 1) * sizeof(struct Student), SEEK_SET);
    read(student_fd, &student, sizeof(struct Student));

    printf("Updated no of filled seats in course.txt file \n");
    lseek(all_courses_fd, -1 * sizeof(struct Course), SEEK_CUR);
    course.filled_seats = course.filled_seats - 1;
    write(all_courses_fd, &course, sizeof(struct Course));

    printf("Updated enrollment status \n");
    lseek(student_course_fd, -1 * sizeof(struct Student_Course), SEEK_CUR);
    std_course.hasEnrolled = false;
    write(student_course_fd, &std_course, sizeof(struct Student_Course));

    printf("Updated no of courses enrolled in student.txt file \n");
    lseek(student_fd, -1 * sizeof(struct Student), SEEK_CUR);
    student.no_of_courses_enrolled = student.no_of_courses_enrolled - 1;
    printf("%d", student.no_of_courses_enrolled);
    write(student_fd, &student, sizeof(struct Student));

    flockstr1.l_type = F_UNLCK;
    fcntl(all_courses_fd, F_SETLKW, &flockstr1);
    flockstr2.l_type = F_UNLCK;
    fcntl(student_course_fd, F_SETLKW, &flockstr2);
    flockstr3.l_type = F_UNLCK;
    fcntl(student_fd, F_SETLKW, &flockstr3);

    printf("All Locks released \n");

    close(all_courses_fd);
    close(student_course_fd);
    close(student_fd);
    return 0;
}
