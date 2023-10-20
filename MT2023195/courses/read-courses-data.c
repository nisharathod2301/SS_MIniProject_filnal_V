/*
============================================================================
Name : read-courses-data.h
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

struct Student_Course {
    char std_id[6];
    char course_id[6];
    bool hasEnrolled;
    time_t enrolledTimestamp;
};

int main() {
    int fd = open("CO001.txt", O_RDONLY);
    if (fd == -1) {
		printf("File doesn't exist \n");
		return 0;
	}
   
   printf("Course Details : \n");
    
   struct Student_Course temp;
   while (read(fd, &temp, sizeof(struct Student_Course)) > 0) {
      printf("Student No. : %s \n", temp.std_id);
      printf("Course No. : %s \n", temp.course_id);
      printf("Status: %s \n", temp.hasEnrolled ? "Enrolled" : "Not enrolled");
      printf("Time last active : %ld \n \n", (long)temp.enrolledTimestamp);
   }

   close(fd);
}
