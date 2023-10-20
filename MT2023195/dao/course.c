/*
============================================================================
Name : course.c
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

#include "./course-dao.h"
#include "../model/student-course.h"

void generateCourseId(char* new_id);
void incrementCourseId(const char* input_courseid, char* new_courseid, int size);
void insertAllStudentsInCourse(char course_id[]);
int compareTimestamps(const void* a, const void* b);

int insertCourse(char faculty_id[], char name[], int total_seats, int credits) {
    char new_id[6];
    generateCourseId(new_id);
    struct Course course;
    bzero(&course, sizeof(course));
    strcpy(course.course_id, new_id);
    strcpy(course.name, name);
    course.total_seats = total_seats;
    course.filled_seats = 0;
    strcpy(course.faculty_id, faculty_id);
    course.isActivated = true;
    course.credits = credits;
    printf("id: %s \n", course.course_id);
    printf("name: %s \n", course.name);
    int fd = open("/data/course.txt", O_WRONLY | O_APPEND);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    printf("id: %s \n", course.course_id);
    if(write(fd, &course, sizeof(struct Course)) == -1) {
        perror("Course details could not be written to the file");
    }
    close(fd);
    insertAllStudentsInCourse(course.course_id);
    return getSuffix(course.course_id);
}

void generateCourseId(char* new_id) {
    int fd = open("/data/course.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    struct Course temp;
    int offset = lseek(fd, -1 * sizeof(struct Course), SEEK_END);
    printf("offset: %d \n", offset);
    if (offset == -1) {
        sprintf(new_id, "CO001");
        printf("Inside gen: %s \n", new_id);
        return;
    }
    read(fd, &temp, sizeof(struct Course));
    close(fd);
    char last_id[6];
    strcpy(last_id, temp.course_id);
    incrementCourseId(last_id, new_id, sizeof(new_id));
}

void incrementCourseId(const char* input_courseid, char* new_courseid, int size) {
    char prefix[3];
    int numeric_part;
    sscanf(input_courseid, "%2s%d", prefix, &numeric_part);
    numeric_part++;
    snprintf(new_courseid, size, "%s%03d", prefix, numeric_part);
    printf("New course id: %s\n", new_courseid);
}

bool isCourseIDValid(char course_id[]) {
    return (course_id[0] == 'C' && course_id[1] == 'O');
}

struct Course doesCourseExist(char course_id[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/course.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    struct Course course;
    while (read(fd, &course, sizeof(struct Course)) > 0) {
        printf("course id %s ", course.course_id);
        printf("faculty id %s \n", course.faculty_id);
        if (strcmp(course.course_id, course_id) == 0) {
            return course;
        }
    }
    memset(&course, 0, sizeof(struct Course));
    return course;
}

struct Course isCourseOfferedByFaculty(char course_id[], char faculty_id[]) {
    int fd = open("/data/course.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    struct Course course;
    while (read(fd, &course, sizeof(struct Course)) > 0) {
        printf("course id %s ", course.course_id);
        printf("faculty id %s \n", course.faculty_id);
        if (strcmp(course.course_id, course_id) == 0 && strcmp(course.faculty_id, faculty_id) == 0) {
            return course;
        }
    }
    memset(&course, 0, sizeof(struct Course));
    return course;
}

struct Course* findCoursesByFacultyId(char* course_id, const char* faculty_id, int* num_matches) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/course.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}

    struct Course* course_array = NULL;
    *num_matches = 0;
    struct Course course;

    while (read(fd, &course, sizeof(struct Course)) > 0) {
        if (strcmp(course.faculty_id, faculty_id) == 0) {
            course_array = realloc(course_array, (*num_matches + 1) * sizeof(struct Course));
            if (course_array == NULL) {
                perror("Memory allocation error");
                close(fd);
                free(course_array);
                return NULL;
            }
            course_array[*num_matches] = course;
            (*num_matches)++;
        }
    }
    
    close(fd);
    return course_array;
}

bool isCourseActivated(char course_id[]) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/course.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    struct Course course;
    lseek(fd, (getSuffix(course_id) - 1) * sizeof(struct Course), SEEK_SET);
    read(fd, &course, sizeof(struct Course));
    return course.isActivated;
}

void insertStudentInAllCourses(char student_id[]) {
    struct Student_Course std_course;
    bzero(&std_course, sizeof(std_course));
    strcpy(std_course.std_id, student_id);
    std_course.hasEnrolled = false;

    std_course.enrolledTimestamp = 0;

    int all_courses_fd = open("/Users/nisharathod/Desktop/MT2023195/data/course.txt", O_RDONLY);
    if (all_courses_fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}
    struct Course course;
    while (read(all_courses_fd, &course, sizeof(struct Course)) > 0) {
        strcpy(std_course.course_id, course.course_id);
        char path[100];
        sprintf(path, "/courses/%s.txt", course.course_id);
        int fd = open(path, O_WRONLY | O_APPEND);
        write(fd, &std_course, sizeof(struct Student_Course));
        close(fd);
    }

    close(all_courses_fd);
}

struct Course* getAllActivatedCourses(int* num_matches) {
    int fd = open("/Users/nisharathod/Desktop/MT2023195/data/course.txt", O_RDONLY);
    if (fd == -1) {
		perror("Error in opening the file course.txt. \n");
	}

    struct Course* course_array = NULL;
    *num_matches = 0;
    struct Course course;

    while (read(fd, &course, sizeof(struct Course)) > 0) {
        if (course.isActivated == true) {
            course_array = realloc(course_array, (*num_matches + 1) * sizeof(struct Course));
            if (course_array == NULL) {
                perror("Memory allocation error");
                close(fd);
                free(course_array);
                return NULL;
            }
            course_array[*num_matches] = course;
            (*num_matches)++;
        }
    }
    
    close(fd);
    return course_array;
}

struct Course* getAllEnrolledCourses(char student_id[], int* num_matches) {
    int course_fd = open("/Users/nisharathod/Desktop/MT2023195/data/course.txt", O_RDONLY);
    if (course_fd == -1) {
		perror("Error in opening the file course.txt.\n");
	}

    struct Course* course_array = NULL;
    *num_matches = 0;
    struct Course course;

    while (read(course_fd, &course, sizeof(struct Course)) > 0) {
        if (course.isActivated == true) {
            char path[100];
            sprintf(path, "/courses/%s.txt", course.course_id);
            int student_course_fd = open(path, O_RDONLY);
            struct Student_Course std_course;
            lseek(student_course_fd, (getSuffix(student_id) - 1) * sizeof(struct Student_Course), SEEK_SET);
            read(student_course_fd, &std_course, sizeof(struct Student_Course));

            if (std_course.hasEnrolled == true) {
                course_array = realloc(course_array, (*num_matches + 1) * sizeof(struct Course));
                if (course_array == NULL) {
                    perror("Memory allocation error");
                    close(student_course_fd);
                    close(course_fd);
                    free(course_array);
                    return NULL;
                }
                course_array[*num_matches] = course;
                (*num_matches)++;
            }
            close(student_course_fd);
        }
    }
    close(course_fd);
    return course_array;
}

void insertAllStudentsInCourse(char course_id[]) {
    char path[100];
    sprintf(path, "/courses/%s.txt", course_id);
    printf("%s \n", path);
    int student_course_fd = open(path, O_CREAT | O_WRONLY, 0744);

    struct Student student;
    bzero(&student, sizeof(struct Student));
    int student_fd = open("/Users/nisharathod/Desktop/MT2023195/data/student.txt", O_RDONLY);
    while (read(student_fd, &student, sizeof(struct Student)) > 0) {
        printf("%s \n", student.std_id);
        struct Student_Course std_course;
        bzero(&std_course, sizeof(struct Student_Course));
        strcpy(std_course.std_id, student.std_id);
        strcpy(std_course.course_id, course_id);
        std_course.hasEnrolled = false;
        std_course.enrolledTimestamp = 0;
        write(student_course_fd, &std_course, sizeof(struct Student_Course));
    }

    close(student_fd);
    close(student_course_fd);
}

void deEnrollAllStudentsInCourse(char course_id[]) {
    char path[100];
    sprintf(path, "/courses/%s.txt", course_id);
    printf("%s \n", path);
    int student_course_fd = open(path, O_RDWR);

    struct Student_Course student_course;
    bzero(&student_course, sizeof(struct Student_Course));
    while (read(student_course_fd, &student_course, sizeof(struct Student_Course)) > 0) {
        deEnrollStudentInCourse(student_course.std_id, course_id);
    }

    close(student_course_fd);
}

void deEnrollStudents(int reduced_seats, char course_id[]) {
    char path[100];
    sprintf(path, "/courses/%s.txt", course_id);
    printf("%s \n", path);
    int student_course_fd = open(path, O_RDWR);
    struct Student_Course* std_course_array = NULL;
    int num_matches = 0;
    struct Student_Course std_course;

    while (read(student_course_fd, &std_course, sizeof(struct Student_Course)) > 0) {
        if (std_course.hasEnrolled == true) {
            std_course_array = realloc(std_course_array, (num_matches + 1) * sizeof(struct Student_Course));
            if (std_course_array == NULL) {
                perror("Memory allocation error");
                close(student_course_fd);
                free(std_course_array);
                return;
            }
            std_course_array[num_matches] = std_course;
            (num_matches)++;
        }
    }

    qsort(std_course_array, num_matches, sizeof(struct Student_Course), compareTimestamps);

    if (num_matches > 0) {
        for (int i = 0; i < reduced_seats; i++) {
            deEnrollStudentInCourse(std_course_array[i].std_id, course_id);
        }
    }

    free(std_course_array);

}

int compareTimestamps(const void* a, const void* b) {
    const struct Student_Course* std_course_a = (const struct Student_Course*)a;
    const struct Student_Course* std_course_b = (const struct Student_Course*)b;

    if (std_course_a->enrolledTimestamp > std_course_b->enrolledTimestamp) {
        return -1;
    } else if (std_course_a->enrolledTimestamp < std_course_b->enrolledTimestamp) {
        return 1;
    } else {
        return 0;
    }
}
