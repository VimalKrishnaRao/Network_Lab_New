#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

// Function to check if a number is prime
int is_prime(int num) {
  if (num <= 1) return 0;
  if (num <= 3) return 1;
  if (num % 2 == 0 || num % 3 == 0) return 0;
  for (int i = 5; i * i <= num; i += 6) {
    if (num % i == 0 || num % (i + 2) == 0) return 0;
  }
  return 1;
}

int main() {
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  
  // Create a socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  
  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  
  // Bind the socket to the address
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  
  // Listen for incoming connections
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  
  printf("Server listening on port %d\n", PORT);
  
  while (1) {
    // Accept the incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
  
    int num;
    
    // Read the number sent by the client
    valread = read(new_socket, &num, sizeof(num));
    if (valread == 0) {
      break;
    } else if (valread < 0) {
      perror("read");
      exit(EXIT_FAILURE);
    }
  
    // Check if the number is prime
    int is_prime_result = is_prime(num);
  
    // Send the result (0/1) to the client
    send(new_socket, &is_prime_result, sizeof(is_prime_result), 0);
  
    printf("Number received: %d\n", num);
    printf("Sent result: %d (0 - Not Prime, 1 - Prime)\n", is_prime_result);
    
    close(new_socket);
  }

  close(server_fd);
  return 0;
}
