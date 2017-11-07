#include "unp.h"
#include <time.h>
#include <string.h>
// #define HOME_PAGE "202: <html><body><h1>Advance Networking Web Server<h1/></body></html>\n"

int main(int argc, char **argv){
        int n, listenfd, connfd, counter = 0;
        struct sockaddr_in servaddr;
        char buff[MAXLINE], username[16], password[64];
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

	while(counter < 3){

	if (strcmp(username, "admin") && strcmp(password, "pass"))
	{
		if(counter <3){
            snprintf(buff, sizeof(buff), "You have %d attemts left",(3 - counter));
            counter++;
		}
        /*else{
            snprintf(buff, sizeof(buff), "DENIED");
            break;
        } */
        if(counter == 2){
		Write(connfd, buff, strlen(buff));
		snprintf(buff, sizeof(buff), "DENIED");
     	}
	else
	{
		snprintf(buff, sizeof(buff),"PROCEED");
		Write(connfd, buff, strlen(buff));
		break;
	}



	} // end while
        printd("Written: %s/n", buff);
        fflush(stdout);

        Close(connfd);
        }// end for

}// end main
