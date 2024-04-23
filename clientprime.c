#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
  int sock = 0;
  struct sockaddr_in serv_addr;
  int num;
  int valread;
  int is_prime_result;
  
  // Create a socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  
  // Set server IP address (replace with the actual server IP)
  // You can use "localhost" for connecting to the server on the same machine
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    perror("inet_pton failed");
    exit(EXIT_FAILURE);
  }

  // Connect to the server
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("connect failed");
    exit(EXIT_FAILURE);
  }
  
  // Get the number from the user
  printf("Enter a number: ");
  scanf("%d", &num);
  
  // Send the number to the server
  send(sock , &num, sizeof(num), 0 );
  
  // Receive the result (0/1) from the server
  valread = read( sock , &is_prime_result, sizeof(is_prime_result));
  if (valread == 0) {
    printf("Connection closed by server\n");
  } else if (valread < 0) {
    perror("read failed");
    exit(EXIT_FAILURE);
  } else {
    // Print the result
    printf("Number: %d is %s prime\n", num, (is_prime_result) ? "a" : "not");
  }
  
  // Close the socket
  return 0;
}
