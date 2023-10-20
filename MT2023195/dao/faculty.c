/*
============================================================================
Name : faculty.c
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

#include "faculty-dao.h"

void generateFacultyId(char* new_id);
void generateFacultyPassword(char* password);
void incrementFacultyNumber(const char* input_facultyno, char* new_facultyno, int size);

int insertFaculty(char name[], char email[], char dept[]) {
    char new_id[6], password[50];
    generateFacultyId(new_id);
    generateFacultyPassword(password);
    printf("%s \n", new_id);
    printf("%s \n", password);
    struct Faculty faculty;
    bzero(&faculty, sizeof(faculty));
    strcpy(faculty.faculty_id, new_id);
    strcpy(faculty.name, name);
    strcpy(faculty.email, email);
    strcpy(faculty.dept, dept);
    strcpy(faculty.password, password);
    printf("id: %s \n", faculty.faculty_id);
    printf("name: %s \n", faculty.name);
    printf("email: %s \n", faculty.email);
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/faculty.txt", O_WRONLY | O_APPEND);
    if (fd == -1) {
		perror("Error in opening the file faculty.txt. \n");
	}
    if(write(fd, &faculty, sizeof(struct Faculty)) == -1) {
        perror("Faculty details could not be written to the file");
    }
    close(fd);
    return getSuffix(faculty.faculty_id);
}

struct Faculty getFacultyDetails(char facultyno_string[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/faculty.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file faculty.txt. \n");
	}
    int facultyno;
    sscanf(facultyno_string, "FA%d", &facultyno);
    lseek(fd, (facultyno - 1) * sizeof(struct Faculty), SEEK_SET);
    struct Faculty faculty;
    read(fd, &faculty, sizeof(struct Faculty));
    close(fd);
    return faculty;
}

void generateFacultyId(char* new_id) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/faculty.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file faculty.txt. \n");
	}
    struct Faculty temp;
    int offset = lseek(fd, -1 * sizeof(struct Faculty), SEEK_END);
    printf("offset: %d \n", offset);
    // if file is empty, then it is the first faculty
    if (offset == -1) {
        sprintf(new_id, "FA001");
        printf("Inside gen: %s \n", new_id);
        return;
    }
    read(fd, &temp, sizeof(struct Faculty));
    close(fd);
    char last_id[6];
    strcpy(last_id, temp.faculty_id);
    incrementFacultyNumber(last_id, new_id, sizeof(new_id));
}

void generateFacultyPassword(char* password) {
    sprintf(password, "password");
}

void incrementFacultyNumber(const char* input_facultyno, char* new_facultyno, int size) {
    char prefix[3];
    int numeric_part;
    sscanf(input_facultyno, "%2s%d", prefix, &numeric_part);
    numeric_part++;
    snprintf(new_facultyno, size, "%s%03d", prefix, numeric_part);
    printf("New : %s\n", new_facultyno);
}

struct Faculty updateFacultyName(int facultyno, char name[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/faculty.txt", O_RDWR);
    if (fd == -1) {
		perror("Error in opening the file faculty.txt. \n");
	}
    struct Faculty faculty;

    lseek(fd, (facultyno - 1) * sizeof(struct Faculty), SEEK_SET);
    read(fd, &faculty, sizeof(struct Faculty));
    strcpy(faculty.name, name);
    lseek(fd, -1 * sizeof(struct Faculty), SEEK_CUR);
    write(fd, &faculty, sizeof(struct Faculty));

    close(fd);
    return faculty;
}

struct Faculty updateFacultyEmail(int facultyno, char email[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/faculty.txt", O_RDWR);
    if (fd == -1) {
		perror("Error in opening the file faculty.txt. \n");
	}
    struct Faculty faculty;

    lseek(fd, (facultyno - 1) * sizeof(struct Faculty), SEEK_SET);
    read(fd, &faculty, sizeof(struct Faculty));
    strcpy(faculty.email, email);
    lseek(fd, -1 * sizeof(struct Faculty), SEEK_CUR);
    write(fd, &faculty, sizeof(struct Faculty));

    close(fd);
    return faculty;
}

struct Faculty updateFacultyDepartment(int facultyno, char dept[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/faculty.txt", O_RDWR);
    if (fd == -1) {
		perror("Error in opening the file faculty.txt. \n");
	}
    struct Faculty faculty;

    lseek(fd, (facultyno - 1) * sizeof(struct Faculty), SEEK_SET);
    read(fd, &faculty, sizeof(struct Faculty));
    strcpy(faculty.dept, dept);
    lseek(fd, -1 * sizeof(struct Faculty), SEEK_CUR);
    write(fd, &faculty, sizeof(struct Faculty));

    close(fd);
    return faculty;
}

struct Faculty updateFacultyPassword(int facultyno, char password[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/faculty.txt", O_RDWR);
    if (fd == -1) {
		perror("Error in opening the file faculty.txt. \n");
	}
    struct Faculty faculty;

    lseek(fd, (facultyno - 1) * sizeof(struct Faculty), SEEK_SET);
    read(fd, &faculty, sizeof(struct Faculty));
    strcpy(faculty.password, password);
    lseek(fd, -1 * sizeof(struct Faculty), SEEK_CUR);
    write(fd, &faculty, sizeof(struct Faculty));

    close(fd);
    return faculty;
}

bool isFacultyAuthenticated(char login_id[], char password[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/faculty.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file faculty.txt. \n");
	}
    int offset = lseek(fd, -1 * sizeof(struct Faculty), SEEK_END);
    if (offset == -1) {
        // no faculty present
        return false;
    } else {
        struct Faculty faculty;
        read(fd, &faculty, sizeof(struct Faculty));
        if (getSuffix(login_id) > getSuffix(faculty.faculty_id)) {
            return false;
        } else {
            lseek(fd, 0, SEEK_SET);
            while (read(fd, &faculty, sizeof(struct Faculty)) > 0) {
                if ((strcmp(faculty.faculty_id, login_id) == 0) && (strcmp(faculty.password, password) == 0)) {
                    return true;
                }
            }
        }
    }
    return false;
}
