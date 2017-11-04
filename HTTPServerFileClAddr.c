#include "unp.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv){

    socklen_t len;
    int n, listenfd, connfd, char_in, count = 0;
    struct sockaddr_in servaddr, cliaddr;
    char buff[40], wbuff[MAXLINE], rbuff[MAXLINE], cmd[16], path1[64] = "", path[64], vars[16];


    FILE * hFile;

    if (argc != 2)
        err_quit("uasge: a.out <Port>");

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

    Listen(listenfd, LISTENQ);

    for (; ;){

        len = sizeof(cliaddr);

        connfd = Accept(listenfd, (SA *) &cliaddr, &len);
        //error in line 35
        printf("\nConnection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntons(cliaddr.sin_port));



        while ( (n = read(connfd, rbuff, MAXLINE)) > 0){


            rbuff[n] = 0;

            if (fputs(rbuff, stdout) == EOF)
                err_sys("fputs error");

            if (strstr(rbuff,"\r\n\r\n") >0)
            {
                break;
            } // end if

        } //end while

        if (n < 0)
            err_sys("read error");

        sscanf(rbuff, "%s %s %s", cmd, path, vars);

        strcat(path1, path);

        if(strcmp (path1, "./") == 0)
        {
            strcpy(path1, "./index,html");
        }

        hFile = fopen(path1, "r");

        if (hFile == NULL)
        {
            hFile = fopen("error.html", "r");
        }

        strcpy(wbuff,"");

        while((char_in = fgetc(hFile)) != EOF)
        {
            wbuff[count] = char_in;
            count++;
        }

        wbuff[count] = 0;

        Write(connfd, wbuff, strlen(wbuff));
        count = 0;
        fclose(hFile);
        strcpy(path1, ".");

        Close(connfd);

    } //end for

} //end main
