#include  "unp.h"
#include <string.h>
#include <stdio.h>

 
int main (int argc, char **argv)
{
        int sockfd, n, counter = 0;
        char recvline[MAXLINE + 1], outbuff[MAXLINE +1];
        struct sockaddr_in servaddr;
		struct hostent *hptr;
		char **pptr;
		char *ptr;
 
        if (argc != 3)
                err_quit("usage: a.out <IPaddress>");
 
        if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                err_sys("socket error");
 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[2]));
		
		ptr = argv[1];
		if ( (hptr = gethostbyname(ptr)) == NULL) 
			err_msg("gethostbyname error for host: %s: %s",ptr, hstrerror(h_errno));
		
		pptr = hptr->h_addr_list;
		memcpy(&servaddr.sin_addr, *pptr, sizeof(pptr));
		
        //if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <=0)
        //        err_quit("inet_pton error for %s", argv[1]);
 
        if (connect(sockfd, (SA *) & servaddr, sizeof(servaddr)) < 0)
                err_sys("connect error");
 
        snprintf(outbuff,sizeof(outbuff), "GET /index.html HTTP/1.1\r\nHost:www.comp.dit.ie\r\nConnection: close\r\n\r\n");
        Write(sockfd, outbuff, strlen(outbuff));
 
        while ( (n =  read(sockfd, recvline, MAXLINE)) > 0) {
                counter++;
                recvline[n] = 0;
                if (fputs(recvline, stdout) == EOF)
                        err_sys("fputs error");
 
        } //end while
        if (n < 0)
                err_sys("read error");
 
        printf("counter = %d\n", counter);
        exit(0);
			
} // end main
