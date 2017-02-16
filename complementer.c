#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int startProcesses(char * tok, char * intL);
int complementer(char * subName, char * subtrahend);
int incrementer(char * complement);
int adder();

int main(){    

   	/* initialize varialbes */
	char fileP[1024], fileP2[1024];
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
    

    /* begin the other two processes */
    startProcesses(fileToken2,intLength);
  
    /* cleanup */
    free(intLength);
  
    return 0;
}

int startProcesses(char * fileTok, char * intL){
    

    /* instantiate arguments */
    int pipeFD1[2], pipeFD2[2];
    char * argv1[1];
    argv1[1] = intL;

    char * argv2[2];
    argv2[1] = fileTok;
    argv2[2] = intL;
   
    int pid,pid2; 

    /* make complementer to incrementer pipe */
    pipe(pipeFD1);

    if((pid = fork()) < 0) {
        perror ("fork failed");
        exit(1);
    }
    /* begin other two processes */
    if(pid == 0) {
        close(pipeFD1[1]);
        pipe(pipeFD2);
        if((pid2 = fork()) < 0) {
            perror ("fork failed");
            exit(1);
        }
        if(pid2 == 0){
            close(pipeFD2[1]);
            adder(argv2);
        }
        else{
            close(pipeFD2[0]);
            incrementer();
        }                                                                                
    }
    else{
        close(pipeFD1[0])
        complementer();
    }
    return 0;
}

int complementer(char * subName, char * subtrahend){
	/* parse and complement data */
    int i = 0;
    FILE * inputFD;
    if ((inputFD = fopen(subName, "r")) == NULL){
        printf("file not found\n");
        exit(1);
    }  

    while (fscanf(inputFD,"%s",subtrahend) != EOF){
        while(subtrahend[i] != '\0') {
            if(subtrahend[i] == '1'){
                subtrahend[i] = '0';
            } else {
                subtrahend[i] = '1';
            }
            i++;
        }
    }

}

int incrementer(char * complement){
    
    char complement[1024];

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
	return 0;
}

int adder(){
    
    char complement[1024], minunend[1024], result[1024];

    int i = 0;
    int j = 0;
    int carry = 0;

    while(i < (int)strlen(complement)){
        
        /* start at the least significant bit in nuber */
        i += atoi(argv[2]);

        while(j <= atoi(argv[2])){
            if(complement[i] == 1 && minunend[i] == 1){
                if(carry == 1){
                    result[i] = 1;
                    carry = 1;
                }
                else{
                    result[i] = 0;
                    carry = 1;
                }
            }
            else if(complement[i] == 1 && minunend[i] == 0){
                if(carry == 1){
                    result[i] = 0;
                    carry = 1;
                }
                else{
                    result[i] = 1;
                    carry = 0;
                }
            }
            else if(complement[i] == 0 && minunend[i] == 1){
                if(carry == 1){
                    result[i] = 0;
                    carry = 1;
                }
                else{
                    result[i] = 1;
                    carry = 0;
                }
            }
            else if(complement[i] == 0 && minunend[i] == 0){
                if(carry == 1){
                    result[i] = 1;
                    carry = 0;
                }
                else{
                    result[i] = 0;
                    carry = 0;
                }
            }
			i--;
			j++;
			
        }
		i += atoi(argv[2]);
		j = 0;
    }
    printf("%s", result);

	return 0;
}
