#include "unp.h"
#include <time.h>
#include <string.h>
#define HOME_PAGE "202: <html><body><h1>Advance Networking Web Server</h1></body></html>\n"

int main(int argc, char **argv){
	pid_t pid;
        int n, listenfd, connfd;
        struct sockaddr_in servaddr;
        char buff[MAXLINE], cmd[16], path[64],vers[16];
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

	if ( (pid = Fork()) == 0){
		Close(listenfd);
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

	sscanf(buff, "%s %s %s", cmd, path, vers);

	if (!strcmp(path, "/index.html"))
	{
		snprintf(buff, sizeof(buff), HOME_PAGE);
	}
	else
	{
		snprintf(buff, sizeof(buff), "ERROR 404: page not found\r\n");
	}

	Write(connfd, buff, strlen(buff));
getchar();
        Close(connfd);

	exit(0);
	}// end child process code

        Close(connfd);
        }// end for

}// end main
