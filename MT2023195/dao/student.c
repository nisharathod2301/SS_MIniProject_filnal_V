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

#include "student-dao.h"

void generateStudentId(char* new_id);
void generateStudentPassword(char* password);
void incrementRollNumber(const char* input_rollno, char* new_rollno, int size);

int insertStudent(char name[], int age, char email[]) {
    char new_id[6], password[50];
    generateStudentId(new_id);
    generateStudentPassword(password);
    printf("%s \n", new_id);
    printf("%s \n", password);
    struct Student student;
    bzero(&student, sizeof(student));
    strcpy(student.std_id, new_id);
    strcpy(student.name, name);
    student.age = age;
    strcpy(student.email, email);
    strcpy(student.password, password);
    student.no_of_courses_enrolled = 0;
    student.isActivated = true;
    printf("id: %s \n", student.std_id);
    printf("name: %s \n", student.name);
    printf("age: %d \n", student.age);
    printf("email: %s \n", student.email);
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_WRONLY | O_APPEND);
    if (fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}
    if(write(fd, &student, sizeof(struct Student)) == -1) {
        perror("Student details could not be written to the file");
    }
    close(fd);
    insertStudentInAllCourses(student.std_id);
    int rollno;
    sscanf(student.std_id, "MT%d", &rollno);
    return rollno;
}

struct Student getStudentDetails(char rollno_string[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}
    int rollno;
    sscanf(rollno_string, "MT%d", &rollno);
    lseek(fd, (rollno - 1) * sizeof(struct Student), SEEK_SET);
    struct Student student;
    read(fd, &student, sizeof(struct Student));
    close(fd);
    return student;
}

void generateStudentId(char* new_id) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}
    struct Student temp;
    int offset = lseek(fd, -1 * sizeof(struct Student), SEEK_END);
    printf("offset: %d \n", offset);
    // if file is empty, then it is the first student
    if (offset == -1) {
        sprintf(new_id, "MT001");
        printf("Inside gen: %s \n", new_id);
        return;
    }
    read(fd, &temp, sizeof(struct Student));
    close(fd);
    char last_id[6];
    strcpy(last_id, temp.std_id);
    incrementRollNumber(last_id, new_id, sizeof(new_id));
}

void generateStudentPassword(char* password) {
    sprintf(password, "password");
}

void incrementRollNumber(const char* input_rollno, char* new_rollno, int size) {
    char prefix[3];
    int numeric_part;
    sscanf(input_rollno, "%2s%d", prefix, &numeric_part);
    numeric_part++;
    snprintf(new_rollno, size, "%s%03d", prefix, numeric_part);
    printf("New roll: %s\n", new_rollno);
}

struct Student updateStudentName(int rollno, char name[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_RDWR);
    if (fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}
    struct Student student;

    lseek(fd, (rollno - 1) * sizeof(struct Student), SEEK_SET);
    read(fd, &student, sizeof(struct Student));
    strcpy(student.name, name);
    lseek(fd, -1 * sizeof(struct Student), SEEK_CUR);
    write(fd, &student, sizeof(struct Student));

    close(fd);
    return student;
}

struct Student updateStudentAge(int rollno, int age) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_RDWR);
    if (fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}
    struct Student student;

    lseek(fd, (rollno - 1) * sizeof(struct Student), SEEK_SET);
    read(fd, &student, sizeof(struct Student));
    student.age = age;
    lseek(fd, -1 * sizeof(struct Student), SEEK_CUR);
    write(fd, &student, sizeof(struct Student));

    close(fd);
    return student;
}

struct Student updateStudentEmail(int rollno, char email[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_RDWR);
    if (fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}
    struct Student student;

    lseek(fd, (rollno - 1) * sizeof(struct Student), SEEK_SET);
    read(fd, &student, sizeof(struct Student));
    strcpy(student.email, email);
    lseek(fd, -1 * sizeof(struct Student), SEEK_CUR);
    write(fd, &student, sizeof(struct Student));

    close(fd);
    return student;
}

bool isStudentAuthenticated(char login_id[], char password[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}
    int offset = lseek(fd, -1 * sizeof(struct Student), SEEK_END);
    if (offset == -1) {
        // no student present
        return false;
    } else {
        struct Student student;
        read(fd, &student, sizeof(struct Student));
        if (getSuffix(login_id) > getSuffix(student.std_id)) {
            return false;
        } else {
            lseek(fd, 0, SEEK_SET);
            while (read(fd, &student, sizeof(struct Student)) > 0) {
                if ((strcmp(student.std_id, login_id) == 0) && (strcmp(student.password, password) == 0)) {
                    return true;
                }
            }
        }
    }
    return false;
}

struct Student updateStudentPassword(int rollno, char password[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_RDWR);
    if (fd == -1) {
		perror("Error in opening the file student.txt. \n");
	}
    struct Student student;

    lseek(fd, (rollno - 1) * sizeof(struct Student), SEEK_SET);
    read(fd, &student, sizeof(struct Student));
    strcpy(student.password, password);
    lseek(fd, -1 * sizeof(struct Student), SEEK_CUR);
    write(fd, &student, sizeof(struct Student));

    close(fd);
    return student;
}

bool isStudentActivated(char student_id[]) {
    struct Student student = getStudentDetails(student_id);
    return student.isActivated;
}
