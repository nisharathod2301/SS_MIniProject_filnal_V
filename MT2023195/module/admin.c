/*
============================================================================
Name : admin.c
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "admin-ops.h"
#include "../constants/menu.h"
#include "../constants/view-details.h"
#include "../model/student.h"
#include "../model/faculty.h"
#include "../dao/student-dao.h"
#include "../dao/faculty-dao.h"
#include "../commons/common.h"

int createStudent(int socket_fd);
struct Student updateStudent(int socket_fd);
struct Student updateStudentActivateStatus(int socket_fd, bool isActive);
int createFaculty(int socket_fd);
struct Faculty updateFaculty(int socket_fd);

void handle_admin_operations(int socket_fd) {
    printf("in handle_admin_operations \n");
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;
    int itr = 0;

    do {
        bzero(write_buffer, sizeof(write_buffer));

        itr++;
        if (itr == 1) {
            strcpy(write_buffer, " Authenthication Successful! \n");
        }
        strcat(write_buffer, ADMIN_MENU);

        if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1)
            perror("Error while displaying admin menu to the user");
        printf("- \n");
        bzero(read_buffer, sizeof(read_buffer));
        if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
            perror("Error while reading from client");
        }
        int choice = atoi(read_buffer);
        printf("Enter choice: %d \n", choice);

        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));

        struct Student student;
        struct Faculty faculty;

        switch(choice) {
            case 1 : 
                printf("Add student \n");
                int rollno = createStudent(socket_fd);
                char student_id[6];
                sprintf(student_id, "MT%03d", rollno);
                strcat(write_buffer, " Student added! \n");
                strcat(write_buffer, " Login Id of student : ");
                strcat(write_buffer, student_id);
                strcat(write_buffer, " \n");
                break;
            case 2 :
                printf("View student details \n");
                strcat(write_buffer, "Enter the roll number of the student \nyou want want to view: ");
                if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
                    perror("Error while asking the client to enter roll no to view student details");
                }
                if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
                    perror("Error while reading roll no of student from client");
                }
                bzero(write_buffer, sizeof(write_buffer));
                student = getStudentDetails(read_buffer);
                sprintf(write_buffer, STUDENT_DETAILS, student.std_id, student.name, student.age, student.email, student.password, student.no_of_courses_enrolled, student.isActivated ? "activated" : "de-activated");
                strcat(write_buffer, "\n");
                break;
            case 3 :
                printf("Modify student details \n");
                student = updateStudent(socket_fd);
                if (isStudentEmpty(student)) {
                    bzero(write_buffer, sizeof(write_buffer));
                    strcpy(write_buffer, "Invalid option was selected. Please try again. \n");
                } else {
                    strcat(write_buffer, "Student details are updated successfully \n");
                    sprintf(write_buffer, STUDENT_DETAILS, student.std_id, student.name, student.age, student.email, student.password, student.no_of_courses_enrolled, student.isActivated ? "activated" : "de-activated");
                    strcat(write_buffer, "\n");
                }
                break;
            case 4 :
                printf("Add a faculty \n");
                int facultyno = createFaculty(socket_fd);
                char faculty_id[6];
                sprintf(faculty_id, "FA%03d", facultyno);  
                strcat(write_buffer, " Faculty added  \n");
                strcat(write_buffer, " Login Id of faculty : ");
                strcat(write_buffer, faculty_id);
                strcat(write_buffer, " \n");
                break;
            case 5 :
                printf("View faculty details \n");
                strcat(write_buffer, "Enter the faculty id \nyou want want to view: ");
                if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
                    perror("Error while asking the client to enter roll no to view faculty details");
                }
                if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
                    perror("Error while reading faculty no of faculty from client");
                }
                bzero(write_buffer, sizeof(write_buffer));
                faculty = getFacultyDetails(read_buffer);
                sprintf(write_buffer, FACULTY_DETAILS, faculty.faculty_id, faculty.name, faculty.email, faculty.dept, faculty.password);
                strcat(write_buffer, "\n");
                break;
            case 6 :
                printf("Modify faculty details \n");
                faculty = updateFaculty(socket_fd);
                if (isFacultyEmpty(faculty)) {
                    bzero(write_buffer, sizeof(write_buffer));
                    strcpy(write_buffer, "Invalid option \n");
                } else {
                    strcat(write_buffer, "Faculty details are updated \n");
                    sprintf(write_buffer, FACULTY_DETAILS, faculty.faculty_id, faculty.name, faculty.email, faculty.dept, faculty.password);
                    strcat(write_buffer, "\n");
                }
                break;
            default :
                bzero(write_buffer, sizeof(write_buffer));
                strcpy(write_buffer, "Invalid option \n");
                break;
        }
        strcat(write_buffer, "Do you want to continue (y/n)? : ");
        if(write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
            perror("Error while asking the client: to continue admin flow");
        }
        bzero(read_buffer, sizeof(read_buffer));
        if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
            perror("Error while reading from client: to continue admin flow");
        }
        printf("%s \n", read_buffer);
    } while (strcmp(read_buffer, "y") == 0);
}

int createStudent(int socket_fd) {
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcpy(write_buffer, "Enter the name of student: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    char name[50];
    strcpy(name, read_buffer);

    strcpy(write_buffer, "Enter the age of student: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    int age = atoi(read_buffer);

    strcpy(write_buffer, "Enter the email of student: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    char email[50];
    strcpy(email, read_buffer);
    return insertStudent(name, age, email);
}

struct Student updateStudent(int socket_fd) {
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;
    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    struct Student updatedStudent;
    memset(&updatedStudent, 0, sizeof(struct Student));

    strcat(write_buffer, "Enter the roll number of the student \nyou want want to update: ");
    if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
        perror("Error while asking the client to enter roll no to update student details");
    }
    if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Error while reading roll no of student from client");
    }
    int rollno;
    sscanf(read_buffer, "MT%03d", &rollno);


    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcat(write_buffer, UPDATE_STUDENT_MENU);
    if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
        perror("Error while asking the client to enter which student details has to be updated");
    }
    if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Error while reading which student details has to be updated from client");
    }
    int choice = atoi(read_buffer);
    printf("- %d", choice);
    if (choice > 3) {
        return updatedStudent;
    }

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcat(write_buffer, "Enter the new value: ");
    if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
        perror("Error while asking the client to enter the value to be updated");
    }
    if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Error while reading the value to be updated from client");
    }

    switch(choice) {
        case 1:
            printf("updating... \n");
            updatedStudent = updateStudentName(rollno, read_buffer);
            break;
        case 2:
            printf("updating... \n");
            updatedStudent = updateStudentAge(rollno, atoi(read_buffer));
            break;
        case 3:
            printf("updating... \n");
            updatedStudent = updateStudentEmail(rollno, read_buffer);
            break;
        default:
            printf("Enter Valid choice\n");
            break;
    }
    return updatedStudent;
}

int createFaculty(int socket_fd) {
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcpy(write_buffer, "Enter the name of faculty: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    char name[50];
    strcpy(name, read_buffer);

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcpy(write_buffer, "Enter the email of faculty: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    char email[50];
    strcpy(email, read_buffer);

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcpy(write_buffer, "Enter department of faculty: ");
    write(socket_fd, write_buffer, strlen(write_buffer));
    read(socket_fd, read_buffer, sizeof(read_buffer));
    char dept[50];
    strcpy(dept, read_buffer);

    return insertFaculty(name, email, dept);
}

struct Faculty updateFaculty(int socket_fd) {
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;
    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));
    struct Faculty updatedFaculty;
    memset(&updatedFaculty, 0, sizeof(struct Faculty));

    strcat(write_buffer, "Enter the faculty number of the faculty \nyou want want to update: ");
    if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
        perror("Error while asking the client to enter faculty no to update faculty details");
    }
    if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Error while reading faculty no of faculty from client");
    }
    int facultyno;
    sscanf(read_buffer, "FA%03d", &facultyno);

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcat(write_buffer, UPDATE_FACULTY_MENU);
    if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
        perror("Error while asking the client to enter which faculty details has to be updated");
    }
    if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Error while reading which faculty details has to be updated from client");
    }
    int choice = atoi(read_buffer);
    if (choice > 3) {
        return updatedFaculty;
    }

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcat(write_buffer, "Enter the new value: ");
    if (write(socket_fd, write_buffer, strlen(write_buffer)) == -1) {
        perror("Error while asking the client to enter the value to be updated");
    }
    if (read(socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Error while reading the value to be updated from client");
    }

    switch(choice) {
        case 1:
            printf("updating... \n");
            updatedFaculty = updateFacultyName(facultyno, read_buffer);
            break;
        case 2:
            printf("updating... \n");
            updatedFaculty = updateFacultyEmail(facultyno, read_buffer);
            break;
        case 3:
            printf("updating... \n");
            updatedFaculty = updateFacultyDepartment(facultyno, read_buffer);
            break;
        default:
            printf("invalid choice\n");
            break;
    }
    return updatedFaculty;
}
