#include "unp.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv){

    socklen_t len; // Gets the client addressing information(see Accept call)
    int n, listenfd, connfd, char_in, count = 0;
    /*  'n' return value for read.
        'listenfd' is the listen socket
        'connfd' is the connected socket
        'char_in' reads the next char in the file 'hFile'
        'count' is used to set the index in the 'wbuff,
    */
    struct sockaddr_in servaddr, cliaddr;
    /*  'servaddr' is the address structor for the server
        'cliaddr' is the address structor for the client
    */
    char buff[40], wbuff[MAXLINE], rbuff[MAXLINE], cmd[16], path1[64] = "", path[64], vars[16];
    /*  'buff' is used to store client address information for printing to screen
        'wbuff' is the buffer filed by the 'hFile' (index.html or error.html)
        and then writen to the 'connfd' connected socket
        'rbuff' is a buffer for reading form the 'connfd' the connected socket
        'cmd' is used to store first part of the client request ie 'GET index.html'
        'path1' is used to to creat the path name for the file that has to be returned by the server
        'path' is used to store the second part of the client request ie 'www.dit.ie'
        'vars' is used to store the third the part of the client request
    */


    FILE * hFile;
    /* 'hFile' is of type FILE this is used to get a stream from a file.
    'hFile' is used to read the server output files */

    if (argc != 2)
        err_quit("uasge: a.out <Port>");
    /*  if tests if the argc from the commaned line = 2
        if they don't gives an error message and closes the program
    */

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    /*  Socket() creats am Un-named stock in the kernel
        Socket() returns a int value for the socket descriptor (listenfd get this value)
        Socket() takes 3 args
            1. domain/family, this uses 'AF_INET' for IPv4 addressing
            2. ‘SOCK_STREAM’ this is the transport layer protocol to be used
            3. '0' lets the kernel pick the transport layer protocol
               '0' is the default value for TCP
    */

    bzero(&servaddr, sizeof(servaddr));
    /*  bzero erases the memory space for 'servaddr'
        done to remove any off addressing that might be in the struct
    */
    servaddr.sin_family = AF_INET;
    /*  sets the .sin_familt for the struct to 'AF_INET' IPv4 addressing
        The value has to be the same as the first parameter to the Socket()
    */
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /*  sets the ip address the connection is to use.
        '(INADDR_ANY)' sets the IP address to IP for the local machine
        'htonl(INADDR_ANY), converst the IP to network byte order
    */
    servaddr.sin_port = htons(atoi(argv[1]));
    /*  sets the connection port number
        'atoi(argv[1])' converts the string from argv[1] to an int
        'htons(atoi(argv[1]))' then converts the int to network byte order
    */

    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
    /*  Binds the 'listenfd' the stocket with the addressing information just added to the
        'servaddr' struct
    */

    Listen(listenfd, LISTENQ);
    /*  Turns the 'listenfd' into to a passive listen socket
    */

    for (; ;){


        len = sizeof(cliaddr);
        /*  sets 'len' to the size of the client struct
            this will be used to get client addressing information
        */
        connfd = Accept(listenfd, (SA *) &cliaddr, &len);
        /*  Accept the next client connection
            request and retrieves the client address
        */

        printf("\nConnection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
        /*  prints to screen the client ip address and port number
            'inet_ntop' converts the network byte order IP address
            back to a string of IP dot decimal notation
            'ntohs' converts the network byte order back to host byte order
        */


        while ( (n = read(connfd, rbuff, MAXLINE)) > 0){
        /*  while loop will read the data coing from the 'connfd' connected socket
            Parameters for read.
                1. file of socket descriptor, this uses 'connfd' the connected socket
                2. the buffer to copy data to, for this its 'rbuff'
                3. the size of the buffer in parameter 2
            Returns from read assigned to 'n'
                If successful, the number of bytes read as an int.
                If it reads no data if returns an int '-1'
                If the starting position for the read operation is at the end of the file or beyond it returns int '0'.
        */


            rbuff[n] = 0;
            /*  NULL terminates 'rbuff' before screen print
                the value of 'n' will be the number of bytes added to the buffer
            */

            if (fputs(rbuff, stdout) == EOF)
                err_sys("fputs error");
            /*  prints the contents of 'rbuff' to the screen 'stdout'
            */

            if (strstr(rbuff,"\r\n\r\n") >0)
            /*  'strstr()' looks for a string fragment in a larger string
                In this we look for the string "\r\n\r\n" (the end of the client request)
                in the client request,
                'strstr()' return a pointer to the start of the string fragment,
                it returns NULL 0 if the string is not found this breaks the read loop
            */
            {
                break;
            } // end if

        } //end while

        if (n < 0)
            err_sys("read error");
        /*  error check the read test if n < 0 if it is there was a read error
        */

        sscanf(rbuff, "%s %s %s", cmd, path, vars);
        /*  there are no erorrs in the input
            sscanf is going to parse the client request
            it will added the information to the 3 variables
        */

        strcat(path1, path);
        /*  'strcat()' will concatenate 'path1' and 'path'
            this will mean that 'path' is appened to 'path1'
        */

        if(strcmp (path1, "./") == 0)
        /*  'strcmp()' test if 'path1' = "./"
            if they are the same it returns 0
            if true it copies "./index.html" (default file path) to the var 'path1'
        */
        {
            strcpy(path1, "./index.html");
        }

        hFile = fopen(path1, "r");
        /*  opens the 'index.html' in read mode as 'hFile'
        */

        if (hFile == NULL)
        /*  Tests if the hFile is available
            if the file is not founf it opens the "error.html" file
        */
        {
            hFile = fopen("error.html", "r");
        }

        strcpy(wbuff,"");
        /*  replaces any data in 'wbuff' with ""(empty string)
        */

        while((char_in = fgetc(hFile)) != EOF)
        /*  while loop writes one char at a time from the file to 'char_in' untill it gets to the EOF
            writes the contents of 'chat_in' to the 'wbuff' increses the 'count' by one each loop
        */
        {
            wbuff[count] = char_in;
            count++;
        } // end while loop

        wbuff[count] = 0;
        // sets the the index of wbuff to o(start)

        Write(connfd, wbuff, strlen(wbuff));
        /*  Writes the contents of the 'wbuff' to the 'connfd' the connected socket
        */
        count = 0;
        // sets count to 0 for the next listen socket loop
        fclose(hFile);
        // closes the file
        strcpy(path1, ".");
        // reset the 'path1' to the default "."

        Close(connfd);
        // close the 'connfd'

    } //end for

} //end main
