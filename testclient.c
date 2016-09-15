#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> 
#include <string.h> 
#include <sys/wait.h>

#define bufsize 1024

int main(int argc, char *argv[]){

  // We are going to make a continuous loop client,
  // it's main job is to accept a string from std.in,
  // it will then send the message to the server in the hopes
  // that the server will respond by capitalising the message.

  // Doing so will allow me to open many clients to connect
  // to one server

  int sock, rval;
  struct hostent *host; // the server
  struct sockaddr_in server;  // not a pointer. the server address
  char buf[bufsize];

   // look up hostname (server) using DNS
  if ((host = gethostbyname(argv[1])) == 0) {
    fprintf(stderr, "%s: unknown host\n", argv[1]); 
    return(-1);  
  }

  // Set up fields for socket to point to host and port
  bcopy(host->h_addr, &server.sin_addr, host->h_length);
  server.sin_family = AF_INET;  
  server.sin_port = htons(atoi(argv[2])); // port no.
 
  // Create socket  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    printf("Error opening socket\n");
    return(-1);
  }

  // connect (3-way handshake)

  // connect (when we connect, this is essentially doing a three way connection)
  if (connect(sock, (struct sockaddr *) &server, sizeof(server))){
    printf("ERROR, connecting\n");
    return(-1);
  }

  // now we are going to loop forever
  while (1){
    // we are going to wait for a message from std.in
    printf("Enter a message to send to the server... \n");
    gets(buf);

    // send the message to the server
    if (write(sock, buf, bufsize) < 0){
      perror("Error: sending the messages\n");
    }
  }
}
