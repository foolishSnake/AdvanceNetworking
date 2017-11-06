#include "unp.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv){
    FILE * hFile;
    char filename[MAXLINE], char_in[MAXLINE], read_buff[MAXLINE];

    filename = "./test.html";

    hFile = fopen(filename, "r");

    while((char_in = fgetc(hFile)) != EOF)
        {
            read_buff[count] = char_in;
            count++;
        } // end while loop
} // end main
