/**
 * @file sender.c
 * @author Rehan Nagoor (rnagoorm@uoguelph.ca)
 * @brief program that sends data from the input file in 10 byte packets ot the ip specified
 * @version 0.1
 * @date 2022-10-31
 *
 */

#include "header.h"

/**
 * @brief Main file
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char const *argv[])
{
   if (argc != 3)
   {
      fprintf(stderr, "usage: ./sender <receiver-IP-address> <filename>/n");
      exit(1);
   }

   char msg[11];
   FILE *fp;
   int sockfd;
   struct addrinfo hints, *servinfo, *p;
   int rv;
   int numbytes;

   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_INET;      // set to AF_INET to use IPv4
   hints.ai_socktype = SOCK_DGRAM; // set for UDP Datagram sockets

   // Create the addrinfo and check if successfull
   if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0)
   {
      fprintf(stderr, "sender: getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
   }

   // loop through all the results and make a socket
   for (p = servinfo; p != NULL; p = p->ai_next)
   {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
                           p->ai_protocol)) == -1)
      {
         perror("sender: socket");
         continue;
      }

      break;
   }

   // if there are no sockets to create after it has looped through
   if (p == NULL)
   {
      fprintf(stderr, "sender: failed to create socket\n");
      return 2;
   }

   // open the file to sendto in sizes of 10 bytes
   fp = fopen(argv[2], "r");
   if (fp == NULL)
   {
      fprintf(stderr, "sender: File \"%s\" does not exist.\n", argv[2]);
      exit(1);
   }

   // loop while we get the msg from the input file in 10 byte
   // messages and send each one as a packet
   while (fgets(msg, sizeof(msg), fp))
   {
      if ((numbytes = sendto(sockfd, msg, strlen(msg), 0,
                             p->ai_addr, p->ai_addrlen)) == -1)
      {
         perror("sender: sendto");
         exit(1);
      }
      printf("Sent: %s\n", msg);
   }

   freeaddrinfo(servinfo); // free the addrinfo structure

   // printf("sender: sent %d bytes to %s\n", numbytes, argv[1]);

   // close the socket and file
   close(sockfd);
   fclose(fp);

   return 0;
}
