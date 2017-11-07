#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
    FILE * hFile;
    char filename[MAXLINE], char_in[MAXLINE], read_buff[MAXLINE];
    int n;

    //filename = "./test.html";
    strcpy(filename, "./test.html");

    hFile = fopen(filename, "r");

    n = read(*hFile, read_buff, sizeof(read_buff));
    read_buff[n] = 0;

    if (fputs(read_buff, stdout) == EOF)
                /*  if statement test if the 'recvline' buffer is at the 'EOF'
                    end of file. If it is it throws an error
                    If not it prints to screen using the 'stdout'
                */
                       // err_sys("fputs error");

    fclose(hFile);
    return(0);
} // end main
