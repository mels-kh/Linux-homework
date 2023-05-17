#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

void send_receive_integer_over_socket(int sockfd) { 
	int int_to_send = 2, int_to_receive; //integer_to_send is something like command

	write(sockfd, &int_to_send, sizeof(int_to_send)); 

	//read(sockfd, &int_to_receive, sizeof(int_to_receive)); 

	//int_to_receive = ntohl(int_to_receive);
	printf("Integer Received from Server : %d\n", int_to_receive); 
} 

int main(int argc, char **argv) {
    const char* ip_v4 = "127.0.0.1";
    const int port = atoi(argv[1]);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_v4, &server_address.sin_addr) <= 0) {
        std::cerr << "inet_pton error: " << strerror(errno) << std::endl;
        return 1;
    }

    // Connecting to server
    int client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_fd == -1) {
        std::cerr << "socket error: " << strerror(errno) << std::endl;
        return 1;
    }

    int status = connect(client_fd, (struct sockaddr*)&server_address, sizeof(server_address));
    if (status == -1) {
        std::cerr << "connect error: " << strerror(errno) << std::endl;
        close(client_fd);
        return 1;
    }


    char* hello;
    char buffer[1024] = { 0 };
    std::cout << "insert number\n";
    std::cin >> hello;
    send(client_fd, hello, strlen(hello), 0);
    printf("number from client sent\n");
    read(client_fd, buffer, 1024);
    printf("%s\n", buffer);
    

  close(client_fd);
    return 0;
}