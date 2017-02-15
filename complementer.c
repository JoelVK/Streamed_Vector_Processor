#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv){    

   	/* initialize varialbes */
	int pipeFD, intLength;
    FILE* inputFD;
    char * myfifo = "/tmp/myfifo";
	char input[1024], complement[1024], filePath[1024];
    
    /* get input file */
    printf("Enter filepath for subtrahend: \n");
    scanf("%s",filePath);
    printf("Enter number of bits in each number: \n");
    scanf("%d",&intLength);
    inputFD = fopen(filePath, O_RDONLY);

    /* parse and complement data */
    int i = 0;
    while (fscanf(inputFD,"%s",input) != EOF){
        while(input[i] != '\0') {
            if(input[i] == '1'){
                input[i] = '0';
            } else {
                input[i] = '1';
            }
            i++;
        }
    }

    /* create the pipe */
    mkfifo(myfifo, 0666);

    /* write input to pipe */
    pipeFD = open(myfifo, O_WRONLY);
    write(pipeFD, complement, strlen(complement));
    close(pipeFD);

    /* remove the FIFO */
    unlink(myfifo);

    return 0;}
