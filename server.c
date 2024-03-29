  // raw_sock.c
  #include<stdio.h>
  #include<stdlib.h>
  #include<string.h>
  #include<netinet/ip.h>
  #include<sys/socket.h>
  #include<arpa/inet.h>

  int main() {
      // Structs that contain source IP addresses
      struct sockaddr_in source_socket_address, dest_socket_address;

      int packet_size;

      // Allocate string buffer to hold incoming packet data
      unsigned char *buffer = (unsigned char *)malloc(65536);
      // Open the raw socket
      int sock = socket (PF_INET, SOCK_RAW, IPPROTO_UDP);
      if(sock == -1)
      {
          //socket creation failed, may be because of non-root privileges
          perror("Failed to create socket");
          exit(1);
      }
      int counter = 0;
      int counter_b = 0;
      while(1) {
        // recvfrom is used to read data from a socket
        packet_size = recvfrom(sock , buffer , 65536 , 0 , NULL, NULL);
        if (packet_size == -1) {
          printf("Failed to get packets\n");
          return 1;
        }

        struct iphdr *ip_packet = (struct iphdr *)buffer;

        memset(&source_socket_address, 0, sizeof(source_socket_address));
        source_socket_address.sin_addr.s_addr = ip_packet->saddr;
        memset(&dest_socket_address, 0, sizeof(dest_socket_address));
        dest_socket_address.sin_addr.s_addr = ip_packet->daddr;


        //if(!strcmp((char *)inet_ntoa(source_socket_address.sin_addr), "192.168.2.11")){
          printf("*********************************\n");
          printf("Packet Size (bytes): %d\n",ntohs(ip_packet->tot_len));
          printf("Protocol: %d\n",ntohs(ip_packet->protocol));
          printf("Source Port: %u\n", ntohs(source_socket_address.sin_port));
          printf("Source Address: %s\n", (char *)inet_ntoa(source_socket_address.sin_addr));
          printf("Destination Port: %u\n", ntohs(dest_socket_address.sin_port));
          printf("Destination Address: %s\n", (char *)inet_ntoa(dest_socket_address.sin_addr));
          printf("Identification: %d\n\n", ntohs(ip_packet->id));
          if(!strcmp((char *)inet_ntoa(source_socket_address.sin_addr), "192.168.2.10")){
            counter = counter + ntohs(ip_packet->tot_len);
          } else if(!strcmp((char *)inet_ntoa(source_socket_address.sin_addr), "192.168.1.10")){
              counter_b = counter_b + ntohs(ip_packet->tot_len);
            }
            
            if (!counter%25){
              printf("SRC1-%d  SRC2-%d\n",counter, counter_b);
            }
          /*
          for (int i = 0; i < packet_size; i++) {
            printf("0x%.2X-", buffer[i]);
          }*/
          printf("\n");
        //}
        
      }

      return 0;
  }
