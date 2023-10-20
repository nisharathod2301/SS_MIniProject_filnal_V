
/*
============================================================================
Name : read-course-records.c
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

struct Course {
    char course_id[6];
    char name[50];
    int credits;
    int total_seats;
    int filled_seats;
    char faculty_id[6];
    bool isActivated;
};

int main() {
    int fd = open("course.txt", O_RDONLY);
    if (fd == -1) {
		printf("File course.txt doesn't exist \n");
		return 0;
	}
   
   printf("Course Details : \n");
   int record_no = 0;

   struct Course std;

   while (read(fd, &std, sizeof(struct Course)) > 0) {
      record_no = record_no + 1;
      printf("Record Number : %d \n", record_no);
      printf("Course No. : %s \n", std.course_id);
      printf("Name : %s \n", std.name);
      printf("faculty_id : %s \n", std.faculty_id);
      printf("Active status: %s \n", std.isActivated ? "Activated" : "De-activated");
      printf("total_seats : %d \n", std.total_seats);
      printf("filled_seats : %d \n \n", std.filled_seats);
   }

   close(fd);
}
