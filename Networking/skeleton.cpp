#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

// Create your own IP header
struct ipheader {
    unsigned char      iph_ihl:4, iph_ver:4;
    unsigned char      iph_tos;
    unsigned short int iph_len;
    unsigned short int iph_ident;
    unsigned short int iph_flag:3, iph_offset:13;
    unsigned char      iph_ttl;
    unsigned char      iph_protocol;
    unsigned short int iph_chksum;
    unsigned int       iph_sourceip;
    unsigned int       iph_destip;
};

int main() {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sockfd < 0) {
        perror("Socket error");
        exit(1);
    }

    char buffer[1024];
    memset(buffer, 0, 1024);

    // Fill IP header
    struct ipheader *ip = (struct ipheader *) buffer;
    ip->iph_ver = 4;
    ip->iph_ihl = 5;
    ip->iph_tos = 0;
    ip->iph_len = htons(sizeof(struct ipheader));
    ip->iph_ident = htons(54321);
    ip->iph_ttl = 64;
    ip->iph_protocol = IPPROTO_TCP;
    ip->iph_sourceip = inet_addr("192.168.0.2");
    ip->iph_destip = inet_addr("192.168.0.1");

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = ip->iph_destip;

    // Send packet
    if (sendto(sockfd, buffer, sizeof(struct ipheader), 0,
               (struct sockaddr *)&dest, sizeof(dest)) < 0) {
        perror("Send failed");
    } else {
        printf("Packet sent successfully\n");
    }

    close(sockfd);
    return 0;
}
