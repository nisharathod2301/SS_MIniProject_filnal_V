Run server and client in separate terminals using this command : 

Server -

gcc -o server server.c ./module/student.c ./module/faculty.c ./module/admin.c ./dao/student.c ./dao/faculty.c ./dao/course.c ./commons/common.c

./server

Client -

gcc -o client client.c

./client

Username: AD000
password: test123
