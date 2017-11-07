#include "unp.h"
#include <time.h>
#include <string.h>
// #define HOME_PAGE "202: <html><body><h1>Advance Networking Web Server<h1/></body></html>\n"

int main(int argc, char **argv){
        int n, listenfd, connfd;
        struct sockaddr_in servaddr;
        char buff[MAXLINE], username[16], password[64],vers[16];
        //time_t ticks;

        if (argc !=2)
                err_quit("usage: httpserver <Port>");

        listenfd = Socket(AF_INET, SOCK_STREAM, 0);

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(atoi(argv[1]));

        Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

        Listen(listenfd, LISTENQ);

        for (;;)
        {
                connfd = Accept(listenfd, (SA *) NULL, NULL);

	while ( (n = read(connfd, buff, MAXLINE)) > 0)
	{
		buff[n] = 0;

		if (fputs(buff, stdout) == EOF)
			err_sys("fputs error");

		if (strstr(buff,"\r\n\r\n")>0)
			{
			break;
			}
	}// end while

	if (n < 0)
		err_sys("read error");

	sscanf(buff, "%s %s", username, password);

	if (strcmp(username, "admin") || strcmp(password "pass"))
	{
		snprintf(buff, sizeof(buff), "PROCEED");
	}
	else
	{
		snprintf(buff, sizeof(buff), "DENIED");
	}

	Write(connfd, buff, strlen(buff));


        Close(connfd);
        }// end for

}// end main
