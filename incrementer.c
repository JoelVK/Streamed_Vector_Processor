#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main (){
    
    int pipeFD;
    char * myfifo = "/tmp/myfifo";
    char complement[1024], incremented[1024];


    /* read input from pipe */
    pipeFD = open(myfifo, O_RDONLY);
    read(pipeFD, complement, sizeof(complement));
    close(pipeFD);

    unlink(myfifo);
    
    

	return 0;
}
