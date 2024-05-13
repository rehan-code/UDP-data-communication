/**
 * @file receiver.c
 * @author Rehan Nagoor (rnagoorm@uoguelph.ca)
 * @brief program that receives data from the port of 20592 and displays it with
 *       the ip and port no of the sender
 * @version 0.1
 * @date 2022-10-31
 *
 */
#include "header.h"

/**
 * @brief Get the address from the structure of based on whether it is IPv4 or IPv6
 *
 * @param sa , the socket address pointer
 * @return void* of the IPv4 or IPv6 sockaddr address
 */
void *get_in_addr(struct sockaddr *sa)
{
   if (sa->sa_family == AF_INET) // if IPv4
   {
      return &(((struct sockaddr_in *)sa)->sin_addr);
   }

   return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

/**
 * @brief Get the port from the structure based on whether it is IPv4 or IPv6
 *
 * @param sa , the socket address pointer
 * @return int of the IPv4 or IPv6 sockaddr port
 */
int get_in_port(struct sockaddr *sa)
{
   if (sa->sa_family == AF_INET) // if IPv4
   {
      return (((struct sockaddr_in *)sa)->sin_port);
   }

   return (((struct sockaddr_in6 *)sa)->sin6_port);
}

/**
 * @brief Main function
 *
 * @return int
 */
int main(void)
{
   int sockfd;
   struct addrinfo hints, *servinfo, *p;
   int rv;
   int numbytes;
   struct sockaddr_storage their_addr;
   char buf[MAXBUFLEN];
   socklen_t addr_len;
   char s[INET6_ADDRSTRLEN];

   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_INET;      // set to AF_INET to use IPv4
   hints.ai_socktype = SOCK_DGRAM; // set for UDP Datagram sockets
   hints.ai_flags = AI_PASSIVE;    // use my IP

   // Create the addrinfo and check if successfull
   if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0)
   {
      fprintf(stderr, "receiver: getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
   }

   // loop through all the results and bind to the first one we can
   for (p = servinfo; p != NULL; p = p->ai_next)
   {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
                           p->ai_protocol)) == -1)
      {
         perror("receiver: socket");
         continue;
      }

      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
      {
         close(sockfd);
         perror("receiver: bind");
         continue;
      }

      break;
   }

   // if there are no sockets to bind after it has looped through
   if (p == NULL)
   {
      fprintf(stderr, "receiver: failed to bind socket\n");
      return 2;
   }
   freeaddrinfo(servinfo); // free the addrinfo structure

   printf("waiting to recvfrom...\n");

   // keep looping to keep listening for information
   while (1)
   {
      // recvfrom to get any data that was sent
      addr_len = sizeof their_addr;
      if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN - 1, 0,
                               (struct sockaddr *)&their_addr, &addr_len)) == -1)
      {
         perror("recvfrom");
         exit(1);
      }

      // print the received packet address, port and data
      printf("\nreceived packet from %s port: %d\n",
             inet_ntop(their_addr.ss_family,
                       get_in_addr((struct sockaddr *)&their_addr),
                       s, sizeof s),
             get_in_port((struct sockaddr *)&their_addr));
      // printf("receiver: packet is %d bytes long\n", numbytes);
      buf[numbytes] = '\0';
      printf("%s\n", buf);
   }

   // close the socket
   close(sockfd);

   return 0;
}
