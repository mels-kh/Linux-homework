#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cerrno>
#include <string>
#include <sstream>



#define MAX_SIZE 1000005
 
// Function to generate N prime numbers using
// Sieve of Eratosthenes
void PrimeNumbers(std::vector<int>& primes)
{
    // Create a boolean array "IsPrime[0..MAX_SIZE]" and
    // initialize all entries it as true. A value in
    // IsPrime[i] will finally be false if i is
    // Not a IsPrime, else true.
    bool IsPrime[MAX_SIZE];
    memset(IsPrime, true, sizeof(IsPrime));
 
    for (int p = 2; p * p < MAX_SIZE; p++) {
        // If IsPrime[p] is not changed, then it is a prime
        if (IsPrime[p] == true) {
            // Update all multiples of p greater than or
            // equal to the square of it
            // numbers which are multiple of p and are
            // less than p^2 are already been marked.
            for (int i = p * p; i < MAX_SIZE; i += p)
                IsPrime[i] = false;
        }
    }
 
    // Store all prime numbers
    for (int p = 2; p < MAX_SIZE; p++)
        if (IsPrime[p])
            primes.push_back(p);
}

void send_recive (int server_socket){
    struct sockaddr_in server_address;
    while(true){
        int addrlen = sizeof(server_address);
        int client_socket = accept(server_socket, (struct sockaddr*)NULL, NULL);
        if(client_socket < 0){
            std::cerr << "accept error: " << strerror(errno) << std::endl;
            close(server_socket);
            exit(0);
        }

        std::cout << "connected " << std::endl;
     // line
        
        char buffer[1024] = { 0 };
        read(client_socket, buffer, 4);
        //convert string to int
        int number = std::stoi(buffer);
        
        //find prime number
        std::vector<int> primes;
        
        // Function call
        PrimeNumbers(primes);

        std::cout << number << " -th prime number is " << primes[number] << std::endl;
        std::stringstream temp_str;
        temp_str << primes[number];
        char const* hello = temp_str.str().c_str();
        send(client_socket, hello, 8, 0);
        printf("number sent\n");
    }
}

int main(int argc, char **argv){
    const char* ip_v4 = "127.0.0.1";
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0){
        std::cerr << "socket error: " << strerror(errno) << std::endl;
        close(server_socket);
        exit(0);
    }
   
    //taking port from command-line
    int server_port = atoi(argv[1]);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    if (inet_pton(AF_INET, ip_v4, &server_address.sin_addr) <= 0) {
        std::cerr << "inet_pton error: " << strerror(errno) << std::endl;
        return 1;
    }

    //giving address to socket
    int bind_ = bind(server_socket, (struct sockaddr*)&server_address , sizeof(server_address));
    if(bind_ < 0){
        std::cerr << "bind error: " << strerror(errno) << std::endl;
        close(server_socket);
        exit(0);
    }
    std::cout << "waiting for client" << std:: endl;

    
    if(listen(server_socket, 5) < 0){
        std::cerr << "listen error: " << strerror(errno) << std::endl;
        close(server_socket);
        exit(0);
    }

    send_recive(server_socket);
    
    
    close(server_socket);
}