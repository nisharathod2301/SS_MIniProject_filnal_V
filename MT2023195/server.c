/*
======================================================================================
Name : server.c
Author : Nishtha Paul
======================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

#include "./constants/menu.h"
#include "./constants/configs.h"
#include "./module/admin-ops.h"
#include "./module/student-ops.h"
#include "./module/faculty-ops.h"
#include "./dao/student-dao.h"
#include "./dao/faculty-dao.h"

#define PORT 8080
#define MAX_CONNECTIONS 5

void *handle_client(void *client_socket_fd);
bool isAuthenticated(int choice, char login_id[], char password[]);

int main() {
    int server_socket_fd, client_socket_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;

    // Create socket
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket_fd, MAX_CONNECTIONS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening...");

    while (1) {
        // Accept a client connection
        client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket_fd == -1) {
            perror("Accept failed");
            continue;
        }
        printf("Accepted connection from %s: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket_fd) != 0) {
            perror("Thread creation failed");
            close(client_socket_fd);
        }
    }

    close(server_socket_fd);
    return 0;
}

void *handle_client(void *arg) {
    int client_socket_fd = *((int *)arg);
    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;

    bzero(write_buffer, sizeof(write_buffer));
    bzero(read_buffer, sizeof(read_buffer));

    strcpy(write_buffer, INTRODUCTION);
    strcat(write_buffer, "\n");
    strcat(write_buffer, ROLE_MENU);

    if (write(client_socket_fd, write_buffer, strlen(write_buffer)) == -1)
        perror("Error while sending first introduction to the user");
    if (read(client_socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
        perror("Error while reading role from client");
    }

    int choice = atoi(read_buffer);
    printf("Choice: %d \n", choice);

    if (choice > 0 && choice < 4) {
        char login_id[1000], password[1000];

        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));
        
        strcpy(write_buffer, " Please enter your Login Credentials n");
        strcat(write_buffer, "=  Login Id: ");
        if (write(client_socket_fd, write_buffer, strlen(write_buffer)) == -1)
            perror("Error while asking user login id");
        if (read(client_socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
            perror("Error while reading user login id from client");
        }

        strcpy(login_id, read_buffer);

        bzero(write_buffer, sizeof(write_buffer));
        bzero(read_buffer, sizeof(read_buffer));

        strcpy(write_buffer, "=  Password: ");
        if (write(client_socket_fd, write_buffer, strlen(write_buffer)) == -1)
            perror("Error while asking user password");
        if (read(client_socket_fd, read_buffer, sizeof(read_buffer)) == -1) {
            perror("Error while reading user password from client");
        }

        strcpy(password, read_buffer);

        if (isAuthenticated(choice, login_id, password)) {
            switch(choice) {
                case 1 : 
                    handle_admin_operations(client_socket_fd);
                    break;
                case 2 :
                    handle_student_operations(client_socket_fd, login_id);
                    break;
                case 3 :
                    handle_faculty_operations(client_socket_fd, login_id);
                    break;
                default :
                    bzero(write_buffer, sizeof(write_buffer));
                    strcpy(write_buffer, "Invalid option");
                    write(client_socket_fd, write_buffer, strlen(write_buffer));
                    break;
            }
        } else {
            bzero(write_buffer, sizeof(write_buffer));
            strcpy(write_buffer, "Login Id and Password are incorrect.\nPress enter to exit.");
            write(client_socket_fd, write_buffer, strlen(write_buffer));
        }
    } else {
        bzero(write_buffer, sizeof(write_buffer));
        strcpy(write_buffer, "Incorrect option selected. Try again next time.\nPress enter to exit.");
        write(client_socket_fd, write_buffer, strlen(write_buffer));
    }

    printf("Terminating connection to client!\n");
    close(client_socket_fd);
    pthread_exit(NULL);
}

bool isAuthenticated(int choice, char login_id[], char password[]) {
    printf("in isAuthenticated \n");
    if ((choice == 1) && (strcmp(login_id, ADMIN_LOGIN_ID) == 0) && (strcmp(password, ADMIN_PASSWORD) == 0)) {
        return true;
    } else if (choice == 2 && login_id[0] == 'M' && login_id[1] == 'T') {
        return isStudentAuthenticated(login_id, password);
    } else if (choice == 3 && login_id[0] == 'F' && login_id[1] == 'A') {
        return isFacultyAuthenticated(login_id, password);
    } else {
        return false;
    }
}
