/*
============================================================================
Name : read-faculty-records.c
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

struct Faculty {
    char faculty_id[6]; //FAxxx\0
    char name[50];
    char email[50];
    char dept[50];
    char password[50];
};
int main() {
    int fd = open("faculty.txt", O_RDONLY);
    if (fd == -1) {
		printf("File faculty.txt doesn't exist \n");
		return 0;
	}
   
   printf("Faculty Details : \n");
   int record_no = 0;

   struct Faculty std;

   while (read(fd, &std, sizeof(struct Faculty)) > 0) {
      record_no = record_no + 1;
      printf("Record Number : %d \n", record_no);
      printf("Roll No. : %s \n", std.faculty_id);
      printf("Name : %s \n", std.name);
      printf("Email : %s \n", std.email);
      printf("Dept : %s \n", std.dept);
      printf("Password : %s \n \n", std.password);
   }

   close(fd);
}
