#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main (int argc, char **argv){
    
    int pipeFD;
    char * myfifo = "/tmp/myfifo";
    char * myfifo2 = "/tmp/myfifo2";
    char complement[1024];


    /* read input from pipe */
    pipeFD = open(myfifo, O_RDONLY);
    read(pipeFD, complement, sizeof(complement));

    int i = 0;
    int j = 0;

    while(i < (int)strlen(complement)){
        while(j <= atoi(argv[1])){
            if(complement[i] == 1){
                complement[i] = 0;
                i++;
                j++;
            }
            else if(complement[i] == 0){
                complement[i] = 1;
                i += (atoi(argv[1]) - j);
                break;
            }
        }
        j = 0;
    }
    close(pipeFD);
    unlink(myfifo);
    
    mkfifo(myfifo2,0666);

    pipeFD = open(myfifo2,O_WRONLY);
    write(pipeFD, complement, strlen(complement));
    close(pipeFD);

    unlink(myfifo2);
    
	return 0;
}
