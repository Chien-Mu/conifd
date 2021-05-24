#include "ping.h"
#include "common.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <time.h>

static unsigned short checksum(unsigned short *buf, int bufsz) {
    unsigned long sum = 0xffff;

    while (bufsz > 1) {
        sum += *buf;
        buf++;
        bufsz -= 2;
    }

    if (bufsz == 1)
        sum += *(unsigned char *)buf;

    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

int ping(char *ipv4, unsigned int timeout_sec) {
    int sockfd;
    struct icmphdr hdr;
    struct sockaddr_in addr;
    char buf[1024];
    struct icmphdr *icmphdrptr;
    struct iphdr *iphdrptr;
    struct timeval tv;
    unsigned char ret = EXIT_FAILURE;

    addr.sin_family = PF_INET;

    if ((inet_pton(PF_INET, ipv4, &addr.sin_addr)) < 0) {
        return EXIT_FAILURE;
    }

    sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        return EXIT_FAILURE;
    }

    memset(&hdr, 0, sizeof(hdr));

    hdr.type             = ICMP_ECHO;
    hdr.code             = 0;
    hdr.checksum         = 0;
    hdr.un.echo.id       = 0;
    hdr.un.echo.sequence = 0;

    hdr.checksum = checksum((unsigned short *)&hdr, sizeof(hdr));

    tv.tv_sec  = timeout_sec;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);

    if ((sendto(sockfd, (char *)&hdr, sizeof(hdr), 0, (struct sockaddr *)&addr, sizeof(addr))) < 1) {
        return EXIT_FAILURE;
    }
    //printf("We have sended an ICMP packet to %s\n", ipv4);

    //printf("Waiting for ICMP echo...\n");
    memset(buf, 0, sizeof(buf));
    if ((recv(sockfd, buf, sizeof(buf), 0)) < 1) {
        return EXIT_FAILURE;
    }

    iphdrptr   = (struct iphdr *)buf;
    icmphdrptr = (struct icmphdr *)(buf + (iphdrptr->ihl) * 4);

    //printf("icmphdrptr->type = %d\n", icmphdrptr->type);
    switch (icmphdrptr->type) {
    case 0:
        ret = EXIT_SUCCESS;
        break;
    default:
        ret = EXIT_FAILURE;
        break;
    }

    close(sockfd);
    return ret;
}
