#include  "unp.h"
#include <string.h>
#include <stdio.h>


int main (int argc, char **argv)
{
        int sockfd, n, counter = 0;
        /*  'sockfd' is the connevted socket
            'n' is used to hold the return value from 'read()'
            'counter' is used to count how many times
            a while loop has to run to read the incoming buffer
        */
        char recvline[MAXLINE + 1], outbuff[MAXLINE +1];
        /*  'recvline' is a char buffer to hold the
            incoming stream from the connected socket
            'outbuff' is a buffer for the outgoing http request
        */
        struct sockaddr_in servaddr;
        /*  'servaddr' is the addressing struct for connection to the server
        */
		struct hostent *hptr;
		/*  'gethostbyname()' returns a struct hostent item
            '*hptr' will be used to catch this struct when returned
		*/
		char **pptr;
		// '**pptr' is a pointer to a pointer 'pptr' is used to hold the ip address fron gethostbyname()

		char *ptr;
		// '*ptr' is a pointer used to hold argc[1] domain name for passing to gethostbyname()

        if (argc != 4)
        /*  Test to see how many command line argument there are
            if there is not 4 throws up a message and quits the program
        */
                err_quit("usage: a.out <IPaddress> <Port Number> <Page Name>");

        if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        /*  Tests if 'sockfd' gets created successfully
            IF there is an error 'socket()' returns a value less than 0
            Will throw the eer_sys saying socket error
        */
                err_sys("socket error");

        bzero(&servaddr, sizeof(servaddr));
        // fill the 'servaddr' struct with 0 to overwrite any data that might be there already
        servaddr.sin_family = AF_INET;
        // sets '.sin.family' to 'AF_INET' IPv4 addressing
        servaddr.sin_port = htons(atoi(argv[2]));
        /*  sets 'sin'port' 'atoi(argv[2])' converts the string in the
            third(port number) command line argument to an int
            'htons()' converts the int to network byte order
        */
		ptr = argv[1];
		/*  assigns the value of command line argument 2 to 'ptr'
            argument 2 should be the host to connect to ie 'www.dit.ie'
		*/
		if ( (hptr = gethostbyname(ptr)) == NULL)
            /*  if 'gethostbyname()' fails to return anything 'hptr' will be NULL
                the if statement will throw up an error message.
            */
			err_msg("gethostbyname error for host: %s: %s",ptr, hstrerror(h_errno));

		pptr = hptr->h_addr_list;
        /*  assigns the value from the hptr->h_addr_list
            should be an ip address to the 'pptr'
        */
		memcpy(&servaddr.sin_addr, *pptr, sizeof(pptr));
		/*  'memcpy()' will copy the array at '*pptr' to
            '&servaddr.sin_addr'
		*/
		//memcpy(&servaddr.sin_addr, *pptr, sizeof(*pptr)); this line would not compile

        //if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <=0)
        //        err_quit("inet_pton error for %s", argv[1]);

        if (connect(sockfd, (SA *) & servaddr, sizeof(servaddr)) < 0)
        /*  'connect()' will try to make a connection with 'sockfd'
            using the address information in the struct 'servaddr'
            if the cinnection is made 'connect()' returns 0
            if it fails it returns -1, the if statement test for the
            -1 if it find it throws an error message
        */
                err_sys("connect error");

        snprintf(outbuff,sizeof(outbuff), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",argv[3], argv[1]);
        /*  'snprint()' is going to format a string for the get request
            it is going to use aguments 2(the resourse wanted could be a file 'index.html'
            and 4 (is the host name ie 'www.dit.ie' from the command line in
            'snprint()' is going to copy the formated string to 'outbuff'
        */
        Write(sockfd, outbuff, strlen(outbuff));
        /*  'Write()' is used to write to a file descriptor
            the descriptor for this is the 'sockfd' the connected socket
            if successful 'Wrire()' returns the number of bytes sent
            if there is an error it returns -1
        */

        /*  after sending a HTML request we have to read the reply from the server */
        while ( (n =  read(sockfd, recvline, MAXLINE)) > 0) {
        /*  while loop will read from the connected socket 'sockfd'
            the data read will be added to the 'recvline' buffer
            'read()' returns the number of bytes read, if there is an
            error the return value is -1
            the while is testing that the 'read()' return value is greater than 0
            meaning that data was read
        */
                counter++;
                /* 'counter' variable will increse by one each iteration of the loop */
                recvline[n] = 0;
                /*  0 or NULL will be append to the end of the 'recvline' buffer  */
                if (fputs(recvline, stdout) == EOF)
                /*  if statement test if the 'recvline' buffer is at the 'EOF'
                    end of file. If it is it throws an error
                    If not it prints to screen using the 'stdout'
                */
                        err_sys("fputs error");

        } //end while
        if (n < 0)
        /*  if statement test if the value of 'n'(the return value form 'read()')
            is less than 0, if it is there was an error with read
            it then throws an error message
        */
                err_sys("read error");

        printf("counter = %d\n", counter);
        /*  prints to screen the value of 'counter'
            this will be the number of iterantions of the
            while loop for the read
        */

        exit(0);
        /*
            Quits the program
        */

} // end main
