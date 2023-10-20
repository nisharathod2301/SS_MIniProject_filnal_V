/*
============================================================================
Name : read-std-records.c
Author : Nisha Rathod
Roll numner: MT2023195
Description : Academia portal
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

struct Student {
    char std_id[6];
    char name[50];
    int age;
    char email[50];
    char password[50];
    int no_of_courses_enrolled;
    bool isActivated;
};
int main() {
    int fd = open("student.txt", O_RDONLY);
    if (fd == -1) {
		printf("File student.txt doesn't exist \n");
		return 0;
	}
   
   printf("Students Details : \n");
   int record_no = 0;

   struct Student std;

   while (read(fd, &std, sizeof(struct Student)) > 0) {
      record_no = record_no + 1;
      printf("Record Number : %d \n", record_no);
      printf("Roll No. : %s \n", std.std_id);
      printf("Name : %s \n", std.name);
      printf("Email : %s \n", std.email);
      printf("Password : %s \n", std.password);
      printf("No_of_courses_enrolled: %d \n", std.no_of_courses_enrolled);
      printf("Active status: %s \n \n", std.isActivated ? "Activated" : "De-activated");
   }

   close(fd);
}
