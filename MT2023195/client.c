/*
============================================================================
Name : client.c
Author : Nisha Rathod
Description : Academia portal
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // Replace with the server's IP address
#define SERVER_PORT 8080

void connection_handler(int socket_fd);

int main() {
    int client_socket_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    // Create socket
    client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket_fd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(client_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    } else {
        printf("Connected to server at %s:%d\n", SERVER_IP, SERVER_PORT);
        int client_port = getsockname(client_socket_fd, (struct sockaddr*)&client_addr, &addr_len);
        printf("Client Port = %d\n", ntohs(client_addr.sin_port));

        connection_handler(client_socket_fd);
    }
    // Close the client socket
    close(client_socket_fd);

    return 0;
}

void connection_handler(int socket_fd) {

    char read_buffer[1000], write_buffer[1000];
    int bytes_rcvd, bytes_sent;

    do {
        bzero(read_buffer, sizeof(read_buffer));
        bzero(write_buffer, sizeof(write_buffer));
        
        bytes_rcvd = read(socket_fd, read_buffer, sizeof(read_buffer));
        if (bytes_rcvd < 0) {
            perror("Error while reading from client socket");
        } else if (bytes_rcvd == 0) {
            printf("Visit us again!\n");
            printf("Closing connection...\n");
        } else {
            printf("%s", read_buffer);
            fflush(stdin);
            scanf("%[^\n]", write_buffer);
            write(socket_fd, write_buffer, strlen(write_buffer));
        }
    } while (bytes_rcvd > 0);
}
