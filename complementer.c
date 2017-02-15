#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int startProcesses(char * tok, char * intL);

int main(){    

   	/* initialize varialbes */
	int pipeFD;
    FILE* inputFD;
    char * myfifo = "/tmp/myfifo";
	char input[1024], complement[1024], fileP[1024], fileP2[1024];
    char * fileToken, * fileToken2,  * intLength;

    /* get input file */
    printf("Enter filepath for minunend: \n");
    scanf("%s",fileP2);
    fileToken2 = strtok(fileP2, "\0"); 
    printf("Enter filepath for subtrahend: \n");
    scanf("%s",fileP);
    fileToken = strtok(fileP,"\0");

    printf("Enter number of bits in each number: \n");
    
    intLength = (char*)malloc(15);

    scanf("%s",intLength);
    
    if ((inputFD = fopen(fileToken, "r")) == NULL){
        printf("file not found\n");
        exit(1);
    }   

    /* begin the other two processes */
    startProcesses(fileToken2,intLength);
   
    /* parse and complement data */
    int i = 0;
    while (fscanf(inputFD,"%s",input) != EOF){
        while(input[i] != '\0') {
            if(input[i] == '1'){
                complement[i] = '0';
            } else {
                complement[i] = '1';
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

    return 0;
}

int startProcesses(char * fileTok, char * intL){
    
    /* instantiate arguments */
    char * argv1[3];
    argv1[0] = "incrementer";
    argv1[1] = intL;
    argv1[2] = NULL;

    char * argv2[4];
    argv2[0] = "adder";
    argv2[1] = fileTok;
    argv2[2] = intL;
    argv2[3] = NULL;
   
    int pid,pid2; 

    if((pid = fork()) < 0) {
        perror ("fork failed");
        exit(1);
    }
    /* begin other two processes */
    if(pid == 0) { 
        execvp(argv1[0], argv1);
    } else {
        if((pid2 = fork()) < 0) {
            perror ("fork failed");
            exit(1);
        }
        if(pid2 == 0){
            execvp(argv2[0],argv2);
        }

                                                                                        
    }
    return 0;
} 
