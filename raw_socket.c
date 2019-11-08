#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

#include "udp.h"


#define MAX_DATA_SIZE 1024


void hexdump(unsigned char *data, unsigned int data_bytes)
{
    int bin_p, ascii_p;

    bin_p = ascii_p = 0;

    while(bin_p < data_bytes){
        int j;
        int whitespaces;
        for(j = 0; j < 8 && bin_p < data_bytes; j++){
            printf("%02x ", data[bin_p++]);
        }

        whitespaces = (8 - j) * 3;
        for(j = 0; j < whitespaces; j++){
            printf(" ");
        }

        for(j = 0; j < 8 && ascii_p < data_bytes; j++){
            if(isprint(data[ascii_p])){
                printf("%c", data[ascii_p++]);
            }else{
                printf(".");
                ascii_p++;
            }
        }

        printf("\n");
    }
}


int main(void)
{
    int raw_sock;
    int raw_sock_b;
    uint8_t packet[ETH_DATA_LEN];
    uint8_t udp_packet[ETH_DATA_LEN];

    uint8_t packet_b[ETH_DATA_LEN];
    uint8_t udp_packet_b[ETH_DATA_LEN];

    uint8_t data[MAX_DATA_SIZE];
     uint8_t data_b[MAX_DATA_SIZE];
    char *sending_data = "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS";
    char *sending_data_b = "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT";

    char *dsthost = "192.168.2.20";
    char *srchost = "192.168.2.10";


    char *dsthost_b = "192.168.1.20";
    char *srchost_b = "192.168.1.10";

    unsigned int packet_size;
    unsigned int packet_size_b;

    unsigned int data_size;
    unsigned int data_size_b;

    struct sockaddr_in src_addr;
    struct sockaddr_in dst_addr;

    struct sockaddr_in src_addr_b;
    struct sockaddr_in dst_addr_b;

    src_addr.sin_family = AF_INET;
    src_addr.sin_port = htons(5000);
    inet_aton(srchost, &src_addr.sin_addr);

    src_addr_b.sin_family = AF_INET;
    src_addr_b.sin_port = htons(6000);
    inet_aton(srchost_b, &src_addr_b.sin_addr);

    dst_addr.sin_family = AF_INET;
    dst_addr.sin_port = htons(5001);
    inet_aton(dsthost, &dst_addr.sin_addr);

    dst_addr_b.sin_family = AF_INET;
    dst_addr_b.sin_port = htons(6001);
    inet_aton(dsthost_b, &dst_addr_b.sin_addr);

    strcpy((char *)data, sending_data);
    data_size = strlen(sending_data);

    strcpy((char *)data_b, sending_data_b);
    data_size_b = strlen(sending_data_b);

    printf("[+] Build UDP packet...\n\n");
    packet_size = build_udp_packet(src_addr, dst_addr, udp_packet, data, data_size);
    hexdump(udp_packet, packet_size);
    printf("\n\n");

    printf("[+] Build UDP packet...\n\n");
    packet_size_b = build_udp_packet(src_addr_b, dst_addr_b, udp_packet_b, data_b, data_size_b);
    hexdump(udp_packet_b, packet_size_b);
    printf("\n\n");

    printf("[+] Build IP packet...\n\n");
    packet_size = build_ip_packet(src_addr.sin_addr, dst_addr.sin_addr, IPPROTO_UDP, packet, udp_packet, packet_size);
    hexdump(packet, packet_size);
    printf("\n\n");

    printf("[+] Build IP packet...\n\n");
    packet_size_b = build_ip_packet(src_addr_b.sin_addr, dst_addr_b.sin_addr, IPPROTO_UDP, packet_b, udp_packet_b, packet_size_b);
    hexdump(packet_b, packet_size_b);
    printf("\n\n");

    printf("[+] Send UDP packet...\n");
    if((raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0){
        perror("socket");
        exit(1);
    }
    if((raw_sock_b = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0){
        perror("socket");
        exit(1);
    }
    int i = 1;
    while(1){
        printf("%d\n",i%4);
        if(i%20){
            send_udp_packet(raw_sock, src_addr, dst_addr, data, data_size);
        } else {
            printf("**************************\n");
            send_udp_packet(raw_sock_b, src_addr_b, dst_addr_b, data_b, data_size_b);
        }
        i++;
        usleep(100);
    }
    
    return 0;
}
